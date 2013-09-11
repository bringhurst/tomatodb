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

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_fdwriten(int fd, char* buf, size_t len);
int HVN_fdreadn(int fd, char* buf, size_t len);

int HVN_msg_fdwrite(int fd, size_t len, char* msg)
{
    size_t net_len = htonl(len);

    if(HVN_fdwriten(fd, (char*) &net_len, sizeof(uint32_t)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write the length of a message.");
        return HVN_ERROR;
    }

    if(HVN_fdwriten(fd, msg, len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a message.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_msg_fdread(int fd, size_t* len, char** msg)
{
    if(HVN_fdreadn(fd, (char*) len, sizeof(uint32_t)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read the length of a message.");
        return HVN_ERROR;
    }

    *len = ntohl(*len);
    LOG(HVN_LOG_DBG, "Length of incoming message is `%zu'+4.", *len);

    *msg = (char*) malloc(sizeof(char) * *len);

    if(HVN_fdreadn(fd, *msg, *len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a message.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_fdwriten(int fd, char* buf, size_t len)
{
    int rc;

    LOG(HVN_LOG_INFO, "Entered HVN_fdwriten fd=%d, buf=`%s', len=`%zu'.", fd, buf, len);

    while(len > 0) {
        if((rc = fdwrite(fd, buf, len)) <= 0) {
            if(rc < 0 && errno == EINTR) {
                rc = 0;
            }
            else if(rc < 0 && errno == EPIPE) {
                LOG(HVN_LOG_ERR, "The client unexpectedly disconnected.");
                return HVN_ERROR;
            }
            else {
                LOG(HVN_LOG_ERR, "An error occurred while writing to the socket (errno=%d). %s", \
                    errno, strerror(errno));
                return HVN_ERROR;
            }
        }

        len -= rc;
        buf += rc;
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

            LOG(HVN_LOG_ERR, "An error occurred while reading from the socket (errno=%d). %s", \
                errno, strerror(errno));
            return HVN_ERROR;
        }

        if(rc == 0) {
            LOG(HVN_LOG_ERR, "An error occurred while reading from the socket. Unexpected EOF.");
            return HVN_ERROR;
        }

        buf += rc;
        len -= rc;
    }

    return HVN_SUCCESS;
}

/* EOF */
