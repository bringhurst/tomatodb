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
    struct timeval* increment_by = NULL;
    struct timeval* elapsed;
    struct timeval* zero;

    struct timeval* remaining_p = &(timer->remaining);
    struct timeval* max_p = &(timer->max);

    elapsed = (struct timeval*) malloc(sizeof(struct timeval));
    zero = (struct timeval*) malloc(sizeof(struct timeval));

    timerclear(zero);    

    while(timercmp(remaining_p, zero, >)) {
        timerclear(elapsed);
        HVN_timer_timeval_add_ms(elapsed, taskdelay(\
                                 (remaining_p->tv_sec * 1000) + \
                                 (remaining_p->tv_usec / 1000)));

        if(timer->die == true) {
            LOG(HVN_LOG_DBG, "A dead timer has expired.");
            HVN_timer_free(timer);
            taskexit(HVN_SUCCESS);
        }

        //FIXME: needs fresh memory to store the result?
        timersub(remaining_p, elapsed, remaining_p);

        while((increment_by = chanrecvp(timer->chan_increment)) != NULL) {
            timeradd(remaining_p, increment_by, remaining_p);
        }

        if(timerisset(remaining_p) == 0) {
            LOG(HVN_LOG_DBG, "A live timer has expired. Executing callback and exiting timer task.");
            (*(timer->cb))(timer->arg);
            taskexit(HVN_SUCCESS);
        }

        // Don't let the remaining time surpass the max time.
        if(timercmp(remaining_p, max_p, >)) {
            remaining_p->tv_sec = max_p->tv_sec;
            remaining_p->tv_usec = max_p->tv_usec;
        }
    }

    free(zero);
    free(elapsed);
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

void HVN_timer_timeval_add_ms(struct timeval *time, unsigned long ms) {
    time->tv_sec = time->tv_sec + (ms / 1000);
    time->tv_usec = time->tv_usec + ((ms % 1000) * 1000);

    if (time->tv_usec > 1000000) {
        time->tv_usec -= 1000000;
        time->tv_sec++;
    }
}

/* EOF */
