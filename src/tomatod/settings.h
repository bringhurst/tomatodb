#ifndef __TDB__TOMATOD_SETTINGS_H
#define __TDB__TOMATOD_SETTINGS_H

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

#include "havend.h"

#define PROCESS_UUID_SETTINGS_PATH "/process_uuid"

int TDB_set_process_uuid(TDB_ctx_t* ctx);
int TDB_get_uuid_from_file(TDB_ctx_t* ctx, char* uuid_file_path);
int TDB_generate_uuid(uuid_t* uuid);
int TDB_configure_new_process_uuid(TDB_ctx_t* ctx, char* uuid_file_path);
int TDB_prepare_settings_db(TDB_ctx_t* ctx);

#endif /* __TDB__TOMATOD_SETTINGS_H */
