#ifndef __HVN__HAVEND_TIMER_H_
#define __HVN__HAVEND_TIMER_H_

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

#define HVN_TIMER_STACK_SIZE       32768
#define HVN_TIMER_CHANNEL_BACKLOG  10

#define HVN_TIMER_ALARM_MAGIC      0xDEADBEEF

typedef struct HVN_timer_t {
    Channel* timer_chan;
    Channel* alarm_chan;
    uint32_t r;
    UT_array* t;
    bool cancel;
} HVN_timer_t;

int HVN_timer_init(HVN_timer_t** timer);
void HVN_timer_start(HVN_timer_t* timer, int ms);
void HVN_timer_task(HVN_timer_t* timer);

void HVN_timer_reset(HVN_timer_t* timer, int ms);
void HVN_timer_cancel(HVN_timer_t* timer);

void HVN_timer_free(HVN_timer_t* timer);

#endif /* __HVN__HAVEND_TIMER_H_ */
