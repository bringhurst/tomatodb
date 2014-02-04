#ifndef TDB__COMMON_KVS_BATCH_H
#define TDB__COMMON_KVS_BATCH_H

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

#include "common.h"

typedef struct TDB_kvs_batch_t {

} TDB_kvs_batch_t;

int TDB_kvs_batch_init(TDB_kvs_batch_t** batch);
void TDB_kvs_batch_free(TDB_kvs_batch_t* batch);

int TDB_kvs_batch_append(TDB_kvs_batch_t* kvs, \
                         char* key, size_t key_len, \
                         void* value, size_t value_len);

#endif /* TDB__COMMON_KVS_BATCH_H */
