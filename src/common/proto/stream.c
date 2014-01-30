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

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "log.h"
#include "stream.h"
#include "task/task.h"

/** The debug stream to write log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to write messages for. */
extern TDB_loglevel TDB_debug_level;

int TDB_fdwriten(TDB_xdr_stream_t* xdr_stream, char* buf, int len)
{
    int rc = -1;

    LOG(TDB_LOG_INFO, "Entered TDB_fdwriten fd=%d, buf=`%s', len=`%d'.", xdr_stream->fd, buf, len);

    while(len > 0) {
        if((rc = fdwrite(xdr_stream->fd, buf, len)) <= 0) {
            if(rc < 0 && errno == EINTR) {
                rc = 0;
            }
            else if(rc < 0 && errno == EPIPE) {
                LOG(TDB_LOG_ERR, "The client unexpectedly disconnected.");
                return -1;
            }
            else {
                LOG(TDB_LOG_ERR, "An error occurred while writing to the socket (errno=%d). %s", \
                    errno, strerror(errno));
                return -1;
            }
        }

        len -= rc;
        buf += rc;
    }

    return rc;
}

int TDB_fdreadn(TDB_xdr_stream_t* xdr_stream, char* buf, int len)
{
    int rc = -1;

    while(len > 0) {
        rc = fdread(xdr_stream->fd, buf, len);

        if(rc < 0) {
            if(errno == EINTR) {
                continue;
            }

            LOG(TDB_LOG_ERR, "An error occurred while reading from the socket (errno=%d). %s", \
                errno, strerror(errno));
            return -1;
        }

        if(rc == 0) {
            LOG(TDB_LOG_ERR, "An error occurred while reading from the socket. Unexpected EOF.");
            return -1;
        }

        buf += rc;
        len -= rc;
    }

    return rc;
}

int TDB_init_xdr_stream(TDB_xdr_stream_t* xdr_stream)
{
    /* FIXME: Handle memory allocation failures in xdrrec_create. */
    xdrrec_create(&(xdr_stream->xdrs), 0, 0, (void*) xdr_stream, \
        (int (*)(char*,char*,int)) TDB_fdreadn, \
        (int (*)(char*,char*,int)) TDB_fdwriten);

    return TDB_SUCCESS;
}

/* EOF */
