#ifndef __HAVEN_DATABASE_H
#define __HAVEN_DATABASE_H

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

#include "havend.h"

void HAVEN_init_internal_database(HAVEN_server_context_t* ctx);
void HAVEN_close_internal_database(HAVEN_server_context_t* ctx);
int HAVEN_destroy_internal_database(HAVEN_server_context_t* ctx);

#endif /* __HAVEN_DATABASE_H */
