#ifndef __TDB__TOMATOD_TIMER_H_
#define __TDB__TOMATOD_TIMER_H_

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

#include "task/task.h"
#include "ut/utarray.h"

#define TDB_TIMER_STACK_SIZE       32768
#define TDB_TIMER_CHANNEL_BACKLOG  10

#define TDB_TIMER_ALARM_MAGIC      0xDEADBEEF

typedef struct TDB_timer_t {
    Channel* timer_chan;
    Channel* alarm_chan;
    uint32_t r;
    UT_array* t;
    bool cancel;
} TDB_timer_t;

int TDB_timer_init(TDB_timer_t** timer);
void TDB_timer_start(TDB_timer_t* timer, int ms);
void TDB_timer_task(TDB_timer_t* timer);

void TDB_timer_reset(TDB_timer_t* timer, int ms);
void TDB_timer_cancel(TDB_timer_t* timer);

void TDB_timer_free(TDB_timer_t* timer);

#endif /* __TDB__TOMATOD_TIMER_H_ */
