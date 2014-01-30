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

enum TDB_proto_msg_type_enum {
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

struct TDB_replica_info_t {
    opaque uuid[16];
    int port;
    string address<>;
};

/*
 * Bootstrap msg definition.
 */

/* A bootstrap message is a request to tell a remote idle follower that it
 * wishes to join a group of replicas which have yet to achieve 2f+1 :: f==1.
 * Fields in the struct coorespond to requestor identification so the remote
 * replica may add the new replica to its hook array.
 */
struct TDB_msg_bootstrap_t {
    TDB_proto_msg_type_enum type;
    opaque uuid[16];
    int port;
    string address<>;
};

/* A bootstrap response message contains the list of replicas that the remote
 * replica currently knows as existing in the current quorum group.
 */
struct TDB_msg_resp_bootstrap_t {
    TDB_proto_msg_type_enum type;
    bool success;
    TDB_replica_info_t known_replicas<>;
};

/*
 * Connect msg definition.
 */

struct TDB_msg_connect_t {
    TDB_proto_msg_type_enum type;
    opaque magic[4];
    opaque api_version[4];
};

struct TDB_msg_resp_connect_t {
    TDB_proto_msg_type_enum type;
    opaque api_version[4];
    bool success;
};

/*
 * Consensus msg definitions.
 */

struct TDB_msg_consensus_log_entry_t {
    opaque data<>;
};

/* A consensus message is a consolidated RequestVote and AppendEntries message.
 * To construct a RequestVote message, simply exclude log entries and a commit
 * index and enable is_vote. To construct an AppendEntries, set is_vote to
 * false and include relevant log entries and a commit index.
 */
struct TDB_msg_consensus_t {
    TDB_proto_msg_type_enum type;
    bool is_vote;
    opaque uuid_id[16];
    u_long term;
    u_long log_index;
    u_long log_term;
    u_long commit_index;
    TDB_msg_consensus_log_entry_t log_entries<>;
};

/* A consensus response message is a reply to consensus messages in all cases.
 * When the initial request is a vote, the success response determines if the
 * vote was granted or not. If the request was an append entries, then the
 * response cooresponds to the follower's log state.
 */
struct TDB_msg_resp_consensus_t {
    TDB_proto_msg_type_enum type;
    u_long term;
    bool success;
};

/*
 * Control msg definition.
 */

enum TDB_msg_control_action_type_enum {
    /* Actions to control socket state. */
    ATTACH_APPEND = 1,
    ATTACH_DATA   = 2,
    ATTACH_VOTE   = 3,

    /* Action to put a replica into an idle follower bootstrap state. */
    IDLE_FOLLOWER = 4,

    /* Actions to shut down an existing replica. */
    DESTROY       = 5,
    EXIT          = 6
};

/* Control messages contain an action and the UUID of the replica to apply the
 * action to.
 */
struct TDB_msg_control {
    TDB_proto_msg_type_enum type;
    TDB_msg_control_action_type_enum action;
    opaque uuid[16];
};

/* A control message response contains a success field to determine if the
 * control action was successful.
 */
struct TDB_msg_resp_control_t {
    TDB_proto_msg_type_enum type;
    bool success;
};

/*
 * Data msg definition.
 */

/* Transaction control. */
enum TDB_msg_data_transaction_enum {
    SINGLE = 1, /* Not associated with a transaction */
    BEGIN  = 2,
    COMMIT = 3,
    ABORT  = 4
};

/* The primary type of data operation. */
enum TDB_msg_data_verb_enum {
    PUT     = 1,
    GET     = 2,
    DELETE  = 3,
    WATCH   = 4,
    UNWATCH = 5
};

/* The mode of the primary operation type. */
enum TDB_msg_data_mode_enum {
    READ_WRITE     = 1,
    READ_ONLY      = 2,
    READ_BOUNDED   = 3,
    READ_TIMESTAMP = 4
};

/* A data message operation. */
struct TDB_msg_data_op_t {
    TDB_msg_data_verb_enum verb;
    opaque data<>;
};

/* A data message contains one or more ordered operations to perform on the
 * database state machine.
 */
struct TDB_msg_data_t {
    TDB_proto_msg_type_enum type;
    TDB_msg_data_mode_enum mode;
    TDB_msg_data_transaction_enum transaction;
    TDB_msg_data_op_t ops<>;
};

/* A data response message contains a success field to determine if the data
 * message was successful.
 */
struct TDB_msg_resp_data_t {
    TDB_proto_msg_type_enum type;
    bool success;
};

/* EOF */
