#ifndef __HAVEN__HAVEND_STATE_CONSENSUS_STATE_H_
#define __HAVEN__HAVEND_STATE_CONSENSUS_STATE_H_

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

#include "state.h"
#include "events.h"

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions);
int HAVEN_exec_consensus_state(HAVEN_state_t *state, int event, HAVEN_server_t* server);

#endif /* __HAVEN__HAVEND_STATE_CONSENSUS_STATE_H_ */

/* EOF */
