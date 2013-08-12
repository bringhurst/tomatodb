#ifndef __HVN__HAVEND_STATE_STATE_H_
#define __HVN__HAVEND_STATE_STATE_H_

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
 */

#include "server.h"

#define TRANSITION(ACTION) (next = state->actions->ACTION(server))

typedef int (*HVN_action_cb)(HVN_server_t* ctx);

typedef struct HVN_state_actions_t {

    /* Actions for the consensus machine. */
    HVN_action_cb HVN_consensus_create_quorum_as_leader;
    HVN_action_cb HVN_consensus_create_location_quorum;
    HVN_action_cb HVN_consensus_leave_quorum;
    HVN_action_cb HVN_consensus_shutdown_server;
    HVN_action_cb HVN_consensus_join_existing;
    HVN_action_cb HVN_consensus_become_follower;
    HVN_action_cb HVN_consensus_become_candidate;
    HVN_action_cb HVN_consensus_become_leader;

} HVN_state_actions_t;

typedef struct HVN_state_t {
    int cs;
    void* data;
    HVN_state_actions_t* actions;
} HVN_state_t;

#endif /* __HVN__HAVEND_STATE_STATE_H_ */
