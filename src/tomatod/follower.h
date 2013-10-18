#ifndef __TDB__TOMATOD_FOLLOWER_H
#define __TDB__TOMATOD_FOLLOWER_H

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

#include "replica.h"

#define TDB_REPLICA_FOLLOWER_ALT_NK  3

#define TDB_REPLICA_FOLLOWER_ALT_APPEND_KEY  0
#define TDB_REPLICA_FOLLOWER_ALT_TIMER_KEY   1
#define TDB_REPLICA_FOLLOWER_ALT_VOTE_KEY    2

int TDB_replica_follower(TDB_replica_t* replica);

#endif /* __TDB__TOMATOD_FOLLOWER_H */
