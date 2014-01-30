/*
 * Copyright 2013 Los Alamos National Security, LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Jon Bringhurst <jon@bringhurst.org>
 */

/*
 *  General protocol definitions.
 */

enum HVN_msg_type_e {
    /* Client request msg types. */
    BOOTSTRAP   = 1,
    CONNECT     = 2,
    CONSENSUS   = 3,
    CONTROL     = 4,
    DATA        = 5,

    /* Server response msg types. */
    BOOTSTRAP_R = 6,
    CONNECT_R   = 7,
    CONSENSUS_R = 8,
    CONTROL_R   = 9,
    DATA_R      = 10
};

/*
 * Bootstrap msg definition.
 */

/* A bootstrap message is a request to tell a remote idle follower that it
 * wishes to join a group of replicas which have yet to achieve 2f+1 :: f==1.
 * Fields in the struct coorespond to requestor identification so the remote
 * replica may add the new replica to its hook array.
 */
struct HVN_msg_bootstrap_t 
    HVN_msg_type_e type;
    opaque uuid[16];
    int port;
    opaque address<254>;
};

/* A bootstrap response message contains the list of replicas that the remote
 * replica currently knows as existing in the current quorum group.
 */
struct HVN_msg_resp_bootstrap_t {
    HVN_msg_type_e type;
    bool success;
    UT_array* known_replicas
};

/*
 * Connect msg definition.
 */

struct HVN_msg_connect_t {
    opaque magic[4];
    opaque version[4];
    HVN_msg_type_e type;
};

struct HVN_msg_resp_connect_t {
    HVN_msg_type_e type;
    opaque version[4];
    bool success;
};

/*
 * Consensus msg definitions.
 */

/* A consensus message is a consolidated RequestVote and AppendEntries message.
 * To construct a RequestVote message, simply exclude log entries and a commit
 * index and enable is_vote. To construct an AppendEntries, set is_vote to
 * false and include relevant log entries and a commit index.
 */
struct HVN_msg_consensus_t {
    HVN_msg_type_e type;
    bool is_vote;
    opaque uuid_id[16];
    u_long term;
    u_long log_index;
    u_long log_term;
    u_long commit_index;
    UT_array* log_entries;
};

/* A consensus response message is a reply to consensus messages in all cases.
 * When the initial request is a vote, the success response determines if the
 * vote was granted or not. If the request was an append entries, then the
 * response cooresponds to the follower's log state.
 */
typedef struct HVN_msg_resp_consensus_t {
    uint64_t term;
    bool success;
} HVN_msg_resp_consensus_t;

int HVN_msg_consensus_send(int fd, HVN_msg_consensus_t* msg_in);
int HVN_msg_consensus_recv(HVN_msg_consensus_t** msg_out, int fd);

int HVN_msg_resp_consensus_send(int fd, HVN_msg_resp_consensus_t* msg_in);
int HVN_msg_resp_consensus_recv(HVN_msg_consensus_t** msg_out, int fd);

/*
 * Control msg definition.
 */

/* Actions to control socket state. */
#define HVN_MSG_CNTL_ACTION_ATTACH_APPEND  0xD2
#define HVN_MSG_CNTL_ACTION_ATTACH_DATA    0xD3
#define HVN_MSG_CNTL_ACTION_ATTACH_VOTE    0xD4

/* Action to put a replica into an idle follower bootstrap state. */
#define HVN_MSG_CNTL_ACTION_IDLE_FOLLOWER  0xD5

/* Actions to shut down an existing replica. */
#define HVN_MSG_CTRL_ACTION_DESTROY        0xD6
#define HVN_MSG_CTRL_ACTION_EXIT           0xD7

/* Control messages contain an action and the UUID of the replica to apply the
 * action to.
 */
typedef struct HVN_msg_control_t {
    uint32_t action;
    uuid_t uuid;
} HVN_msg_control_t;

/* A control message response contains a success field to determine if the
 * control action was successful.
 */
typedef struct HVN_msg_resp_control_t {
    bool success;
} HVN_msg_resp_control_t;

int HVN_msg_control_send(int fd, HVN_msg_control_t* msg_in);
int HVN_msg_control_recv(HVN_msg_control_t** msg_out, int fd);

int HVN_msg_resp_control_send(int fd, HVN_msg_resp_control_t* msg_in);
int HVN_msg_resp_control_recv(HVN_msg_control_t** msg_out, int fd);

/*
 * Data msg definition.
 */

/* Protocol map keys. */
#define HVN_MSG_DATA_KEY_TYPE            "type"
#define HVN_MSG_DATA_KEY_TRANSACTION     "transaction"
#define HVN_MSG_DATA_KEY_OPERATIONS      "operations"

/* Transaction control. */
#define HVN_MSG_DATA_TRANSACTION_SINGLE  0xE1 /* Not associated with a transaction */
#define HVN_MSG_DATA_TRANSACTION_BEGIN   0xE2
#define HVN_MSG_DATA_TRANSACTION_COMMIT  0xE3
#define HVN_MSG_DATA_TRANSACTION_ABORT   0xE4

/* The primary type of data operation. */
#define HVN_MSG_DATA_VERB_PUT            0xE5
#define HVN_MSG_DATA_VERB_GET            0xE6
#define HVN_MSG_DATA_VERB_DELETE         0xE7
#define HVN_MSG_DATA_VERB_WATCH          0xE8
#define HVN_MSG_DATA_VERB_UNWATCH        0xE9

/* The mode of the primary operation type. */
#define HVN_MSG_DATA_MODE_RW             0xEA /* Read-write */
#define HVN_MSG_DATA_MODE_RO             0xEB /* Read-only */
#define HVN_MSG_DATA_MODE_RB             0xEC /* Read-bounded */
#define HVN_MSG_DATA_MODE_RT             0xED /* Read-timestamp */

/* A data message contains one or more ordered operations to perform on the
 * database state machine.
 */
typedef struct HVN_msg_data_t {
    uint32_t mode;
    uint32_t transaction;
    UT_array* ops; /* HVN_db_op_t[] */
} HVN_msg_data_t;

/* A data response message contains a success field to determine if the data
 * message was successful.
 */
typedef struct HVN_msg_resp_data_t {
    bool success;
} HVN_msg_resp_data_t;

/* EOF */
