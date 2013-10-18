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
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_timer_init(TDB_timer_t** timer)
{
    *timer = (TDB_timer_t*) malloc(sizeof(TDB_timer_t));

    if(*timer == NULL) {
        LOG(TDB_LOG_ERR, "Failed to allocate a new timer.");
        return TDB_ERROR;
    }

    (*timer)->cancel = false;

    (*timer)->timer_chan = chancreate(sizeof(TDB_timer_t*), TDB_TIMER_CHANNEL_BACKLOG);
    (*timer)->alarm_chan = chancreate(sizeof(uint32_t), TDB_TIMER_CHANNEL_BACKLOG);

    utarray_new((*timer)->t, &ut_int_icd);

    return TDB_SUCCESS;
}

void TDB_timer_start(TDB_timer_t* timer, int ms)
{
    TDB_timer_reset(timer, ms);
    taskcreate((void (*)(void*)) TDB_timer_task, (void*) timer, TDB_TIMER_STACK_SIZE);
}

void TDB_timer_task(TDB_timer_t* timer)
{
    TDB_timer_t* next = NULL;

    int* front_time;
    int* ele_time;
    int elapsed_time = 0;
    int recv_time = 0;

    //LOG(TDB_LOG_DBG, "Entering timer task.");

    taskname("timer");
    taskstate("new");

    for(;;) {
        while(timer->timer_chan->nbuf > 0) {
            next = chanrecvp(timer->timer_chan);

            if(next->cancel == true) {
                LOG(TDB_LOG_DBG, "A timer was canceled. Exiting timer task.");
                taskstate("exiting");
                TDB_timer_free(timer);
                break;
            }

            //LOG(TDB_LOG_DBG, "Appending `%d' ms to this timer task.", next->r);
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
        }
        else {
            //LOG(TDB_LOG_DBG, "A timer was triggered. Sending alarm, then exiting timer task.");
            chansendul(timer->alarm_chan, TDB_TIMER_ALARM_MAGIC);
            break;
        }
    }

    //LOG(TDB_LOG_DBG, "Leaving timer task.");
    taskexit(EXIT_SUCCESS);
}

void TDB_timer_reset(TDB_timer_t* timer, int ms)
{
    TDB_timer_t* nt = (TDB_timer_t*) malloc(sizeof(TDB_timer_t));
    nt->r = ms;
    nt->cancel = false;
    chansendp(timer->timer_chan, nt);
}

void TDB_timer_cancel(TDB_timer_t* timer)
{
    TDB_timer_t* nt = (TDB_timer_t*) malloc(sizeof(TDB_timer_t));
    nt->r = 0;
    nt->cancel = true;
    chansendp(timer->timer_chan, nt);
}

void TDB_timer_free(TDB_timer_t* timer)
{
    chanfree(timer->timer_chan);
    chanfree(timer->alarm_chan);
    utarray_free(timer->t);
    free(timer);
}

/* EOF */
