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

#include <stdlib.h>

#include "common.h"
#include "timer.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

void HVN_timer_task(HVN_timer_t* timer)
{
    //TODO: stuff
}

int HVN_timer_init(HVN_timer_t** timer, \
                   struct timeval* max, \
                   int channel_backlog, \
                   void (*func)(void* arg), \
                   void* arg)
{
    struct timeval tmp;
    struct timeval* tmp_p = &tmp;
    struct timeval* remaining_p;
    struct timeval* max_p;

    *timer = (HVN_timer_t*) malloc(sizeof(HVN_timer_t));

    if(*timer == NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory for a new timer.");
        return HVN_ERROR;
    }

    (*timer)->chan_increment = chancreate(sizeof(struct timeval*), channel_backlog);
    (*timer)->arg = arg;
    (*timer)->cb = func;

    remaining_p = &((*timer)->remaining);
    max_p = &((*timer)->max);

    timerclear(tmp_p);
    timerclear(max_p);
    timerclear(remaining_p);

    timeradd(tmp_p, max, max_p);
    timeradd(tmp_p, max, remaining_p);

    taskcreate((void (*)(void*))HVN_timer_task, *timer, HVN_TIMER_STACK_SIZE);

    return HVN_SUCCESS;
}

// Must only be called after the channel has been emptied.
void HVN_timer_free(HVN_timer_t* timer)
{
    chanfree(timer->chan_increment);
    free(timer);
}

// Prevent the callback from being executed and exit the timer task when the
// current time runs out.
void HVN_timer_die(HVN_timer_t* timer)
{
    timer->die = true;
}

int HVN_timer_increment(HVN_timer_t* timer, struct timeval* value)
{
    if(chansendp(timer->chan_increment, value) < 0) {
        LOG(HVN_LOG_ERR, "Failed to increment a timer.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

/* EOF */
