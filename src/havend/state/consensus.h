#ifndef __HAVEN__HAVEND_STATE_CONSENSUS_EVENTS_H_
#define __HAVEN__HAVEND_STATE_CONSENSUS_EVENTS_H_

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

typedef enum HAVEN_state_events_consensus {
    HANDLE_LOG_APPEND = 0,
    HEARTBEAT_TIMEOUT = 1,
    ELECTION_TIMEOUT = 2,
    ELECTION_SUCCESS = 3,
    DISCOVERED_HIGHER_TERM = 4,
    SETUP_LOCATION_SERVICE = 5,
    JOIN_EXISTING_QUORUM = 6
    CREATE_REGULAR = 7,
    CREATE_LOCATION = 8,
    JOIN = 9,
    CREATE_NEW_QUORUM = 10
} HAVEN_state_events_consensus;

#endif /* __HAVEN__HAVEND_CONSENSUS_EVENTS_H_ */
