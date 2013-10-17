#ifndef __TDB__COMMON_XTIME_H_
#define __TDB__COMMON_XTIME_H_

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

#include <stdbool.h>
#include <sys/time.h>

#define TDB_TIME_CHECK_BEFORE 0x01
#define TDB_TIME_CHECK_AFTER  0x03

typedef struct TDB_time_interval_t {
    struct timeval earliest;
    struct timeval latest;
} TDB_time_interval_t;

int TDB_time_now(TDB_time_interval_t* interval);

int TDB_time_after(TDB_time_interval_t* interval, bool* after);
int TDB_time_before(TDB_time_interval_t* interval, bool* before);
int TDB_time_check(TDB_time_interval_t* interval, bool* success, int mode);

void TDB_time_interval_print(TDB_time_interval_t* interval);
int TDB_time_interval_validate(TDB_time_interval_t* interval);

#endif /* __TDB__ONTIME_H_ */
