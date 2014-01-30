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

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include "common.h"
#include "log.h"
#include "socket.h"
#include "task/task.h"

/** The debug stream to write log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to write messages for. */
extern TDB_loglevel TDB_debug_level;

int TDB_msg_fdwrite(int fd, size_t len, char* msg)
{
    size_t net_len = htonl(len);

    if(TDB_fdwriten(fd, (char*) &net_len, sizeof(uint32_t)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to write the length of a message.");
        return TDB_ERROR;
    }

    if(TDB_fdwriten(fd, msg, len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to write a message.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_msg_fdread(int fd, size_t* len, char** msg)
{
    if(TDB_fdreadn(fd, (char*) len, sizeof(uint32_t)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to read the length of a message.");
        return TDB_ERROR;
    }

    *len = ntohl(*len);
    LOG(TDB_LOG_DBG, "Length of incoming message is `%zu'+4.", *len);

    *msg = (char*) malloc(sizeof(char) * *len);

    if(TDB_fdreadn(fd, *msg, *len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to read a message.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_fdwriten(int fd, char* buf, size_t len)
{
    int rc;

    LOG(TDB_LOG_INFO, "Entered TDB_fdwriten fd=%d, buf=`%s', len=`%zu'.", fd, buf, len);

    while(len > 0) {
        if((rc = fdwrite(fd, buf, len)) <= 0) {
            if(rc < 0 && errno == EINTR) {
                rc = 0;
            }
            else if(rc < 0 && errno == EPIPE) {
                LOG(TDB_LOG_ERR, "The client unexpectedly disconnected.");
                return TDB_ERROR;
            }
            else {
                LOG(TDB_LOG_ERR, "An error occurred while writing to the socket (errno=%d). %s", \
                    errno, strerror(errno));
                return TDB_ERROR;
            }
        }

        len -= rc;
        buf += rc;
    }

    return TDB_SUCCESS;
}

int TDB_fdreadn(int fd, char* buf, size_t len)
{
    int rc;

    while(len > 0) {
        rc = fdread(fd, buf, len);

        if(rc < 0) {
            if(errno == EINTR) {
                continue;
            }

            LOG(TDB_LOG_ERR, "An error occurred while reading from the socket (errno=%d). %s", \
                errno, strerror(errno));
            return TDB_ERROR;
        }

        if(rc == 0) {
            LOG(TDB_LOG_ERR, "An error occurred while reading from the socket. Unexpected EOF.");
            return TDB_ERROR;
        }

        buf += rc;
        len -= rc;
    }

    return TDB_SUCCESS;
}

/* EOF */
