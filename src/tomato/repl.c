#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"
#include "task/task.h"

void TDB_repl_cmd_task(char* cmd)
{
    printf("task echo: '%s'\n", cmd);
}

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
    }
}

void TDB_repl_start(void)
{
    char* line;

    linenoiseSetCompletionCallback(TDB_repl_completion);
    linenoiseHistoryLoad("tomato_history"); /* Load the history at startup */

    while((line = linenoise("tomato> ")) != NULL) {
        if(line[0] != '\0' && line[0] != '/') {
            taskcreate((void (*)(void*)) TDB_repl_cmd_task, (void*) line, TDB_CMD_STACK_SIZE);
            taskyield();
            linenoiseHistoryAdd(line);
            linenoiseHistorySave("tomato_history");
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
