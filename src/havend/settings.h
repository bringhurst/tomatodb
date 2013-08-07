#ifndef __HAVEN_SETTINGS_H
#define __HAVEN_SETTINGS_H

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

#include "havend.h"

#define UUID_STR_LEN (36 + 1)
#define PROCESS_UUID_SETTINGS_PATH "/process_uuid"

int HAVEN_set_process_uuid(HAVEN_ctx_t* ctx);
int HAVEN_get_uuid_from_file(HAVEN_ctx_t* ctx, char* uuid_file_path);
int HAVEN_configure_new_uuid(HAVEN_ctx_t* ctx, char* uuid_file_path);
int HAVEN_prepare_settings_db(HAVEN_ctx_t* ctx);

#endif /* __HAVEN_SETTINGS_H */
