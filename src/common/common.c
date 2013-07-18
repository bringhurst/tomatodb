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

#include "log.h"
#include "common.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_ensure_directory_exists(char* path)
{
    struct stat s;
    int err = stat(path, &s);

    if(-1 == err) {
        if(ENOENT == errno) {
            if(mkdir(path, 0755) == 0) {
                return HAVEN_SUCCESS;
            }
            else {
                LOG(HAVEN_LOG_ERR, "Creating a directory at `%s' failed. %s", path, strerror(errno));
                return HAVEN_ERROR;
            }
        }
        else {
            LOG(HAVEN_LOG_ERR, "Viewing the directory at `%s' failed. %s", path, strerror(errno));
            return HAVEN_ERROR;
        }
    }
    if(S_ISDIR(s.st_mode)) {
        return HAVEN_SUCCESS;
    }

    /* The path already exists, but is not a directory. */
    LOG(HAVEN_LOG_ERR, "Creating a directory at `%s' failed. The path already exists.", path);
    return HAVEN_ERROR;
}

/* EOF */
