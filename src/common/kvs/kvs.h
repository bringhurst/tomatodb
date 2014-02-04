#ifndef TDB__COMMON_KVS_H
#define TDB__COMMON_KVS_H

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

int TDB_kvs_init(TDB_kvs_t** kvs, char* path, int storage_type);

int TDB_kvs_get(TDB_kvs_t* kvs, char* key, size_t key_len, \
                void* result, size_t* result_len);

int TDB_kvs_put_batch(TDB_kvs_t* kvs, TDB_kvs_batch_t* batch);
int TDB_kvs_put(TDB_kvs_t* kvs, char* key, size_t key_len, \
                void* value, size_t value_len);

int TDB_kvs_delete(TDB_kvs_t* kvs, char* key);

void TDB_kvs_close(TDB_kvs_t* kvs);
void TDB_kvs_destroy(TDB_kvs_t* kvs);

#endif /* TDB__COMMON_KVS_H */
