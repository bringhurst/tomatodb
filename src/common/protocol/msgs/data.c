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

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_msg_data_send(int fd, HVN_msg_data_t* msg_in)
{
    msgpack_sbuffer sbuf;
    msgpack_packer pck;
    HVN_db_op_t* op;

    msgpack_sbuffer_init(&sbuf);
    msgpack_packer_init(&pck, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_map(&pck, 3);

    msgpack_pack_string(&pck, HVN_MSG_DATA_KEY_TYPE, -1);
    msgpack_pack_int32(&pck, msg_in->type);

    msgpack_pack_string(&pck, HVN_MSG_DATA_KEY_TRANSACTION, -1);
    msgpack_pack_int32(&pck, msg_in->transaction);

    msgpack_pack_string(&pck, HVN_MSG_DATA_KEY_OPERATIONS, -1);
    msgpack_pack_array(&pck, utarray_len(msg_in->operations));

    for(op = (HVN_db_op_t*) utarray_front(msg_in->operations); op != NULL; \
            op = (HVN_db_op_t*) utarray_next(msg_in->operations, op)) {

        if(HVN_msgpack_pack_dbop(&pck, op) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Failed to send a packed data message.");
            return HVN_ERROR;
        }
    }

    if(HVN_msg_fdwrite(fd, sbuf.size, sbuf.data) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to send a packed data message.");
        return HVN_ERROR;
    }

    msgpack_sbuffer_destroy(&sbuf);
    return HVN_SUCCESS;
}

int HVN_msg_data_recv(HVN_msg_data_t** msg_out, int fd)
{
    msgpack_zone mempool;
    msgpack_object unpck;

    char* read_msg;
    size_t read_msg_len;

    *msg_out = (HVN_msg_data_t*) malloc(sizeof(HVN_msg_data_t));
    if(*msg_out == NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate an incoming data message.");
        return HVN_ERROR;
    }

    msgpack_zone_init(&mempool, 2048);

    if(HVN_msg_fdread(fd, &read_msg_len, &read_msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not read an incoming data message.");
        return HVN_ERROR;
    } else {

        // FIXME: change to a map
        if(HVN_msg_unpack_array(&unpck, &read_msg, &read_msg_len, HVN_msg_unpack_dbop) != HVN_SUCCESS) {

        }
    }

    msgpack_unpack(read_msg, read_msg_len, NULL, &mempool, &deserialized);

    return HVN_SUCCESS;
}

int HVN_msg_resp_data_send(int fd, HVN_msg_resp_data_t* msg_in)
{
}

int HVN_msg_resp_data_recv(HVN_msg_data_t** msg_out, int fd)
{

}

/* EOF */
