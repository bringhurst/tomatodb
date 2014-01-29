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

#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmd.h>
#include "common.h"
#include "repl.h"
#include "task/task.h"

void TDB_repl_completion(const char* buf, linenoiseCompletions* lc)
{
    switch(buf[0]) {
        case 'b':
            linenoiseAddCompletion(lc, "bootstrap");
            break;
        case 'c':
            linenoiseAddCompletion(lc, "connect");
            linenoiseAddCompletion(lc, "consensus");
            linenoiseAddCompletion(lc, "control");
            break;
        case 'd':
            linenoiseAddCompletion(lc, "data");
            break;
        case 'h':
            switch(buf[5]) {
                case 'b':
                    linenoiseAddCompletion(lc, "help bootstrap");
                    break;
                case 'c':
                    linenoiseAddCompletion(lc, "help connect");
                    linenoiseAddCompletion(lc, "help consensus");
                    linenoiseAddCompletion(lc, "help control");
                    break;
                case 'd':
                    linenoiseAddCompletion(lc, "help data");
                    break;
            }
            linenoiseAddCompletion(lc, "help");
            break;
    }
}

static int TDB_repl_hist_path(char** path)
{
    char* home_path = getenv("HOME");
    struct passwd *pw;

    *path = (char*) malloc(_POSIX_PATH_MAX * sizeof(char));

    if(home_path == NULL) {
        pw = getpwuid(getuid());
        strncpy(*path, pw->pw_dir, _POSIX_PATH_MAX);
        *path = strcat(*path, "/.tomato_history");

        if(*path != NULL) {
            return TDB_SUCCESS;
        }
    } else {
        strncpy(*path, home_path, _POSIX_PATH_MAX);
        *path = strcat(*path, "/.tomato_history");
        return TDB_SUCCESS;
    }

    free(*path);
    return TDB_ERROR;
}

void TDB_repl_start(void)
{
    char* line;
    char* hist_file;
    bool history_enabled = false;

    linenoiseSetCompletionCallback(TDB_repl_completion);

    if(TDB_repl_hist_path(&hist_file) == TDB_SUCCESS) {
        history_enabled = true;
        linenoiseHistoryLoad(hist_file);
    }

    while((line = linenoise("tomato> ")) != NULL) {
        if(line[0] != '\0' && line[0] != '/') {
            taskcreate((void (*)(void*)) TDB_cmd_task, (void*) line, TDB_CMD_STACK_SIZE);
            taskyield();

            if(history_enabled) {
                linenoiseHistoryAdd(line);
                linenoiseHistorySave(hist_file);
            }
        }
        else if(!strncmp(line, "/historylen", 11)) {
            int len = atoi(line + 11);
            linenoiseHistorySetMaxLen(len);
        }
        else if(line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }

        free(line);
    }
}

/* EOF */
