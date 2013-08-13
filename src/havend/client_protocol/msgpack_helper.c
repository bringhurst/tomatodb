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

#include "msgpack_helper.h"
#include "log.h"
#include "havend.h"

#include "task/task.h"

#include <errno.h>

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_msgpack_fdwrite(int fd, size_t* len, char** msg)
{

}

int HVN_msgpack_fdread(int fd, size_t* len, char** msg)
{
    uint32_t packed_len;

    if(HVN_fdreadn(fd, (char*) &packed_len, sizeof(uint32_t)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read the length of a msgpack message.");
        return HVN_ERROR;
    }

    // FIXME: check packed_len for sanity and max record length.
    
    *msg = (char*) malloc(sizeof(char) * packed_len);
    
    if(HVN_fdreadn(fd, msg, packed_len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a msgpack message.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_fdreadn(int fd, char* buf, size_t len)
{
    int rc;

    while(len > 0) {
        rc = fdread(fd, buf, len);

        if(rc < 0) {
            if(errno == EINTR) {
                continue;
            }

            LOG(HVN_LOG_ERR, "An error occurred while reading from the socket. %s", strerror(errno));
            return HVN_ERROR;
        }

        if(rc == 0) {
            LOG(HVN_LOG_ERR, "An error occurred while reading from the socket. Unexpected message truncation.");
            return HVN_ERROR;
        }

        buf += rc;
        len -= rc;
    }

    return HVN_SUCCESS;
}

/* EOF */
