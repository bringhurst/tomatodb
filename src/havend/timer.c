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
    (*timer)->alarm_chan = chancreate(sizeof(HVN_timer_t*), HVN_TIMER_CHANNEL_BACKLOG);

    return HVN_SUCCESS;
}

void HVN_timer_start(HVN_timer_t* timer)
{
    taskcreate((void (*)(void *)) HVN_timer_task, (void*) timer, HVN_TIMER_STACK_SIZE);
}

void HVN_timer_task(HVN_timer_t* timer)
{
    HVN_timer_t* nt = NULL;
    HVN_timer_t* pt = NULL;

    int old_time = 0;
    int elapsed_time = 0;

    for(;;) {
        while((nt = chanrecvp(timer->timer_chan))) {
            nt->next = timer->next;
            timer->next = nt;
            if(timer->next->cancel == true) {
                LOG(HVN_LOG_DBG, "A timer was canceled. Exiting timer task.");
                HVN_timer_free(timer);
                break;
            }
        }

        if(timer->next) {
            elapsed_time = taskdelay(timer->next->r);

            nt = timer;
            pt = timer;

            while((nt = nt->next)) {
                old_time = nt->r;
                nt->r -= elapsed_time;

                if(nt->r - old_time > 0) {
                    // nt->r wrapped around, so let's remove nt from timer->t.
                    pt->next = nt->next;
                    free(nt);
                    nt = pt->next;
                }
                pt = nt;
            }
        } else {
            LOG(HVN_LOG_DBG, "A timer was triggered. Sending alarm, then exiting timer task.");
            chansendul(timer->alarm_chan, HVN_TIMER_ALARM_MAGIC);
            break;
        }
    }

    taskexit(EXIT_SUCCESS);
}

void HVN_timer_reset(HVN_timer_t* timer, uint32_t ms)
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
    HVN_timer_t* tmp = NULL;

    while((tmp = timer->next)) {
        chanfree(timer->timer_chan);
        chanfree(timer->alarm_chan);
        free(timer);
        timer = tmp;
    }
    if(timer) {
        chanfree(timer->timer_chan);
        chanfree(timer->alarm_chan);
        free(timer);
    }
}

/* EOF */
