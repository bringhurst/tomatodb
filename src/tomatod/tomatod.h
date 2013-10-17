#ifndef __TDB__TOMATOD_TOMATOD_H_
#define __TDB__TOMATOD_TOMATOD_H_

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

#include "context.h"

#define TDB_SETTINGS_DB_PREFIX "/settings"
#define TDB_DATA_DB_PREFIX     "/data"

void TDB_print_version(void);
void TDB_print_usage(void);

int TDB_handle_tomatod_cli_args(TDB_ctx_t* ctx, int argc, char* argv[]);

int TDB_listen_and_accept(TDB_ctx_t* ctx);
void TDB_install_signal_handlers(void);
void TDB_signal_handle_SIGINT(int sig);

#endif /* __TDB__TOMATOD_TOMATOD_H_ */
