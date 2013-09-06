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
#include "log.h"
#include "timer.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_timer_init(HVN_timer_t** timer)
{
    *timer = (HVN_timer_t*) malloc(sizeof(HVN_timer_t));

    if(*timer == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate a new timer.");
        return HVN_ERROR;
    }

    (*timer)->cancel = false;

    (*timer)->timer_chan = chancreate(sizeof(HVN_timer_t*), HVN_TIMER_CHANNEL_BACKLOG);
    (*timer)->alarm_chan = chancreate(sizeof(uint32_t), HVN_TIMER_CHANNEL_BACKLOG);

    utarray_new((*timer)->t, &ut_int_icd);

    return HVN_SUCCESS;
}

void HVN_timer_start(HVN_timer_t* timer)
{
    taskcreate((void (*)(void *)) HVN_timer_task, (void*) timer, HVN_TIMER_STACK_SIZE);
}

void HVN_timer_task(HVN_timer_t* timer)
{
    HVN_timer_t* next = NULL;

    int* front_time;
    int* ele_time;
    int elapsed_time = 0;
    int recv_time = 0;

    taskname("timer");
    taskstate("new");

    for(;;) {
        while(timer->timer_chan->nbuf > 0) {
            next = chanrecvp(timer->timer_chan);

            if(next->cancel == true) {
                LOG(HVN_LOG_DBG, "A timer was canceled. Exiting timer task.");
                taskstate("exiting");
                HVN_timer_free(timer);
                break;
            }

            //LOG(HVN_LOG_DBG, "Appending `%d' ms to this timer task.", next->r);
            recv_time = next->r;
            utarray_push_back(timer->t, &recv_time);
            free(next);
        }

        if(utarray_len(timer->t) > 0) {
            taskstate("sleeping");

            front_time = (int*) utarray_front(timer->t);
            elapsed_time = taskdelay(*front_time);

            taskstate("awake");

            for(ele_time = (int*) utarray_front(timer->t); ele_time != NULL; \
                    ele_time = (int*) utarray_next(timer->t, ele_time)) {

                *ele_time -= elapsed_time;
                if(*ele_time < 0) {
                    utarray_erase(timer->t, 0, 1);
                }
            }
        } else {
            LOG(HVN_LOG_DBG, "A timer was triggered. Sending alarm, then exiting timer task.");
            chansendul(timer->alarm_chan, HVN_TIMER_ALARM_MAGIC);
            break;
        }
    }

    taskexit(EXIT_SUCCESS);
}

void HVN_timer_reset(HVN_timer_t* timer, int ms)
{
    HVN_timer_t* nt = (HVN_timer_t*) malloc(sizeof(HVN_timer_t));
    nt->r = ms;
    nt->cancel = false;
    chansendp(timer->timer_chan, nt);
}

void HVN_timer_cancel(HVN_timer_t* timer)
{
    HVN_timer_t* nt = (HVN_timer_t*) malloc(sizeof(HVN_timer_t));
    nt->r = 0;
    nt->cancel = true;
    chansendp(timer->timer_chan, nt);
}

void HVN_timer_free(HVN_timer_t* timer)
{
    chanfree(timer->timer_chan);
    chanfree(timer->alarm_chan);
    utarray_free(timer->t);
    free(timer);
}

/* EOF */
