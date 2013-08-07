#ifndef __HAVEN__HAVEND_STATE_STATE_H_
#define __HAVEN__HAVEND_STATE_STATE_H_

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

#define TRANSITION(ACTION) next = state->actions->ACTION(server)

typedef void (*HAVEN_action_cb)(HAVEN_server_t* ctx);

typedef struct HAVEN_state_actions_t {

    /* Actions for the consensus machine. */
    HAVEN_action_cb HAVEN_consensus_init_location_cb;
    HAVEN_action_cb HAVEN_consensus_init_new_cb;
    HAVEN_action_cb HAVEN_consensus_join_existing_cb;
    HAVEN_action_cb HAVEN_consensus_become_follower_cb;
    HAVEN_action_cb HAVEN_consensus_become_candidate_cb;
    HAVEN_action_cb HAVEN_consensus_become_leader_cb;
    HAVEN_action_cb HAVEN_consensus_exit_cb;
    HAVEN_action_cb HAVEN_consensus_error_cb;

} HAVEN_state_actions_t;

typedef struct HAVEN_state_t {
    int cs;
    void* data;
    HAVEN_state_actions_t* actions;
} HAVEN_state_t;

#endif /* __HAVEN__HAVEND_STATE_STATE_H_ */
