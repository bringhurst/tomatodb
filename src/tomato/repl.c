#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "repl.h"

void TDB_repl_completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
    }
}

void TDB_repl_start(void) {
    char *line;

    linenoiseSetCompletionCallback(TDB_repl_completion);
    linenoiseHistoryLoad("tomato_history"); /* Load the history at startup */

    while((line = linenoise("tomato> ")) != NULL) {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            printf("echo: '%s'\n", line);
            linenoiseHistoryAdd(line); /* Add to the history. */
            linenoiseHistorySave("tomato_history"); /* Save the history on disk. */
        } else if (!strncmp(line,"/historylen",11)) {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(len);
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }

        free(line);
    }
}
