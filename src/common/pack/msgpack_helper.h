#ifndef __HVN__COMMON_PACK_MSGPACK_HELPER_H_
#define __HVN__COMMON_PACK_MSGPACK_HELPER_H_

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

#include <stddef.h>

int HVN_msgpack_fdread(int fd, size_t* len, char** msg);
int HVN_msgpack_fdwrite(int fd, size_t len, char* msg);

int HVN_fdreadn(int fd, char* buf, size_t len);
int HVN_fdwriten(int fd, char* buf, size_t len);

#endif /* __HVN__COMMON_PACK_MSGPACK_HELPER_H_ */
