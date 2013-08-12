#ifndef __HVN__HAVEND_STATE_EVENTS_H_
#define __HVN__HAVEND_STATE_EVENTS_H_

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

typedef enum HVN_state_events {
    CREATE_NEW=0,
    CREATE_NEW_LOCATION=1,
    JOIN_EXISTING=2,
    CREATED_NEW_LOCATION=3,
    CREATED_NEW=4,
    JOINED_EXISTING=5,
    HEARTBEAT_TIMEOUT=6,
    HANDLE_LOG_APPEND=7,
    LEAVE=8,
    ELECTION_TIMEOUT=9,
    ELECTION_SUCCESS=10,
    DISCOVERED_HIGHER_TERM=11,
    GONE=12
} HVN_state_events;

#endif /* __HVN__HAVEND_STATE_EVENTS_H_ */
