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

#include "common.h"
#include "cmd_help.h"

void TDB_cmd_help_print_bootstrap()
{
    printf("bootstrap help goes here.\n");
}

void TDB_cmd_help_print_connect()
{
    printf("connect help goes here.\n");
}

void TDB_cmd_help_print_consensus()
{
    printf("consensus help goes here.\n");
}

void TDB_cmd_help_print_control()
{
    printf("control help goes here.\n");
}

void TDB_cmd_help_print_data()
{
    printf("data help goes here.\n");
}

void TDB_cmd_help_print_general()
{
    printf("# usage: help <command>\n");
    printf("# Currently available commands are:\n");
    printf("#  * bootstrap\n");
    printf("#  * connect\n");
    printf("#  * consensus\n");
    printf("#  * control\n");
    printf("#  * data\n");
}

void TDB_cmd_help(char* cmd)
{
    switch(cmd[5]) {
        case 'b':
            TDB_cmd_help_print_bootstrap();
            return;
        case 'c':
            switch(cmd[8]) {
                case 'n':
                    TDB_cmd_help_print_connect();
                    return;
                case 's':
                    TDB_cmd_help_print_consensus();
                    return;
                case 't':
                    TDB_cmd_help_print_control();
                    return;
                default:
                    goto general_help;
            }
        case 'd':
            TDB_cmd_help_print_data();
            return;
    }

general_help:
    TDB_cmd_help_print_general();
}

/* EOF */
