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

enum msg_type {
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

struct replica_info {
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
struct msg_bootstrap {
    msg_type type;
    opaque uuid[16];
    int port;
    string address<>;
};

/* A bootstrap response message contains the list of replicas that the remote
 * replica currently knows as existing in the current quorum group.
 */
struct msg_resp_bootstrap {
    msg_type type;
    bool success;
    replica_info known_replicas<>;
};

/*
 * Connect msg definition.
 */

struct msg_connect {
    opaque magic[4];
    opaque api_version[4];
    msg_type type;
};

struct msg_resp_connect {
    opaque api_version[4];
    msg_type type;
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
struct msg_consensus {
    msg_type type;
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
struct msg_resp_consensus {
    u_long term;
    bool success;
};

/*
 * Control msg definition.
 */

enum msg_control_action_type {
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
struct msg_control {
    msg_control_action_type action;
    opaque uuid[16];
};

/* A control message response contains a success field to determine if the
 * control action was successful.
 */
struct msg_resp_control {
    bool success;
};

/*
 * Data msg definition.
 */

/* Transaction control. */
enum msg_data_transaction {
    SINGLE = 1, /* Not associated with a transaction */
    BEGIN  = 2,
    COMMIT = 3,
    ABORT  = 4
};

/* The primary type of data operation. */
enum msg_data_verb {
    PUT     = 1,
    GET     = 2,
    DELETE  = 3,
    WATCH   = 4,
    UNWATCH = 5
};

/* The mode of the primary operation type. */
enum msg_data_mode {
    READ_WRITE     = 1,
    READ_ONLY      = 2,
    READ_BOUNDED   = 3,
    READ_TIMESTAMP = 4
};

/* A data message operation. */
struct msg_data_op {
    msg_data_verb verb;
    opaque data<>;
};

/* A data message contains one or more ordered operations to perform on the
 * database state machine.
 */
struct msg_data {
    msg_data_mode mode;
    msg_data_transaction transaction;
    msg_data_op ops<>;
};

/* A data response message contains a success field to determine if the data
 * message was successful.
 */
struct msg_resp_data {
    bool success;
};

/* EOF */
