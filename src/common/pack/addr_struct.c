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

#include <msgpack.h>

#include "log.h"
#include "common.h"

#include "addr_struct.h"
#include "pack.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_pack_addr_msgpack(UT_array* addrs, \
                          UT_string** result)
{
    HVN_addr_t* data = NULL;
    size_t addr_len, uuid_len;

    utstring_new(*result);

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_array(&pk, utarray_len(addrs));

    while((data = (HVN_addr_t*) utarray_next(addrs, data))) {
        addr_len = strlen(data->address);
        msgpack_pack_raw(&pk, addr_len);
        msgpack_pack_raw_body(&pk, data->address, addr_len);

        msgpack_pack_uint32(&pk, data->port);

        uuid_len = sizeof(data->uuid);
        msgpack_pack_raw(&pk, uuid_len);
        msgpack_pack_raw_body(&pk, data->uuid, uuid_len);
    }
  
    utstring_bincpy(*result, sbuf.data, sbuf.size);

    // FIXME: double free here?
    //msgpack_sbuffer_free(&sbuf);
    //msgpack_packer_free(&pk);

    return HVN_SUCCESS;
}

int HVN_unpack_addr_msgpack(UT_array** result, \
                            char* addrs, \
                            size_t addrs_len)
{
    msgpack_unpacked unpacked;
    char* uuid_buf;
    size_t i, addr_num;
    HVN_addr_t* data;

    UT_icd HVN_addr_t_icd = {
        sizeof(HVN_addr_t),
        NULL, NULL, NULL
    };

    utarray_new(*result, &HVN_addr_t_icd);
    msgpack_unpacked_init(&unpacked);

    if(msgpack_unpack_next(&unpacked, addrs, addrs_len, NULL)) {
        msgpack_object root = unpacked.data;

        if(root.type == MSGPACK_OBJECT_ARRAY) {
            addr_num = root.via.array.size % 3;

            for(i = 0; i < addr_num; i++) {
                data = (HVN_addr_t*) malloc(sizeof(HVN_addr_t));

                data->address = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
                memcpy(data->address, root.via.array.ptr[2].via.raw.ptr, _POSIX_HOST_NAME_MAX - 1);
                data->address[_POSIX_HOST_NAME_MAX] = '\0';

                data->port = root.via.array.ptr[1].via.u64;

                if(root.via.array.ptr[2].via.raw.size == UUID_STR_LEN - 1) {
                    uuid_buf = (char*) malloc(sizeof(char) * UUID_STR_LEN);
                    memcpy(uuid_buf, root.via.array.ptr[2].via.raw.ptr, UUID_STR_LEN - 1);
                    uuid_buf[UUID_STR_LEN] = '\0';

                    if(uuid_parse(uuid_buf, data->uuid) == 0) {
                        LOG(HVN_LOG_DBG, "Parsed a valid UUID from the address struct.");
                    } else {
                        LOG(HVN_LOG_DBG, "Did not parse a valid UUID from the address struct.");
                        return HVN_ERROR;
                    }

                    free(uuid_buf);
                } else {
                    LOG(HVN_LOG_DBG, "Addr struct UUID is not a valid length (`%d').", root.via.array.ptr[2].via.raw.size);
                }

                utarray_push_back(*result, data);
            }
        }
    }

    msgpack_unpacked_destroy(&unpacked);
    return HVN_SUCCESS;
}

/* EOF */
