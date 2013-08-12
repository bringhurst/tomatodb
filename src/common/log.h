#ifndef __HVN_SERVER_LOG_H
#define __HVN_SERVER_LOG_H

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

#include <stdio.h>
#include <time.h>

typedef enum {
    HVN_LOG_FATAL = 1,
    HVN_LOG_ERR   = 2,
    HVN_LOG_WARN  = 3,
    HVN_LOG_INFO  = 4,
    HVN_LOG_DBG   = 5
} HVN_loglevel;

#define LOG(level, ...) do {  \
        if (level <= HVN_debug_level) { \
            char timestamp[256]; \
            time_t ltime = time(NULL); \
            struct tm *ttime = localtime(&ltime); \
            strftime(timestamp, sizeof(timestamp), \
                     "%Y-%m-%dT%H:%M:%S", ttime); \
            if(level == HVN_LOG_DBG) { \
                fprintf(HVN_debug_stream,"[%s] [%s:%d] ", \
                        timestamp, __FILE__, __LINE__); \
            } else { \
                fprintf(HVN_debug_stream,"[%s] [%s:%d] ", \
                        timestamp, __FILE__, __LINE__); \
            } \
            fprintf(HVN_debug_stream, __VA_ARGS__); \
            fprintf(HVN_debug_stream, "\n"); \
            fflush(HVN_debug_stream); \
        } \
    } while (0)

#endif /* __HVN_SERVER_LOG_H */
