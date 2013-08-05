#ifndef __HAVEN_HAVEND_STATE_H
#define __HAVEN_HAVEND_STATE_H

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

#include "havend.h"

typedef void (*HAVEN_action_cb)(HAVEN_ctx_t* ctx);

typedef struct HAVEN_state_actions_t {

    /* Actions for the newbie machine. */
    HAVEN_action_cb HAVEN_newbie_setup_cb;
    HAVEN_action_cb HAVEN_newbie_listen_cb;
    HAVEN_action_cb HAVEN_newbie_exit_cb;
    HAVEN_action_cb HAVEN_newbie_error_cb;

    /* Actions for the consensus machine. */
//    HAVEN_action_cb HAVEN_consensus_init_location_cb;
//    HAVEN_action_cb HAVEN_consensus_init_new_cb;
//    HAVEN_action_cb HAVEN_consensus_join_existing_cb;
//    HAVEN_action_cb HAVEN_consensus_become_follower_cb;
//    HAVEN_action_cb HAVEN_consensus_become_candidate_cb;
//    HAVEN_action_cb HAVEN_consensus_become_leader_cb;
//    HAVEN_action_cb HAVEN_consensus_exit_cb;
//    HAVEN_action_cb HAVEN_consensus_error_cb;

    /* Actions for the client machine. */
//    HAVEN_action_cb HAVEN_client_location_register_cb;
//    HAVEN_action_cb HAVEN_client_setup_cb;
//    HAVEN_action_cb HAVEN_client_handle_client_cb;
//    HAVEN_action_cb HAVEN_client_handle_location_query_cb;
//    HAVEN_action_cb HAVEN_client_handle_consensus_query_cb;
//    HAVEN_action_cb HAVEN_client_exit_cb;

} HAVEN_state_actions_t;

#endif /* __HAVEN_HAVEND_STATE_H */
