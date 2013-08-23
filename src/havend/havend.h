#ifndef __HVN__HAVEND_HAVEND_H_
#define __HVN__HAVEND_HAVEND_H_

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

#define HVN_SETTINGS_DB_PREFIX "/settings"
#define HVN_LOG_DB_PREFIX    "/log"
#define HVN_GROUP_DB_PREFIX  "/group"

void HVN_print_version(void);
void HVN_print_usage(void);

int HVN_handle_havend_cli_args(HVN_ctx_t* ctx, int argc, char* argv[]);

int HVN_listen_and_accept(HVN_ctx_t* ctx);
void HVN_install_signal_handlers(void);

#endif /* __HVN__HAVEND_HAVEND_H_ */
