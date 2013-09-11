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
  msgpack_sbuffer_init(&sbuf);
  msgpack_packer pck;
  msgpack_packer_init(&pck, &sbuf, msgpack_sbuffer_write);



// A data message contains one or more ordered operations to perform on the
// database state machine.
typedef struct HVN_msg_data_t {
    UT_array* ops; // HVN_db_op_t[]
} HVN_msg_data_t;

}

int HVN_msg_data_recv(HVN_msg_data_t** msg_out, int fd)
{

}

int HVN_msg_resp_data_send(int fd, HVN_msg_resp_data_t* msg_in)
{
}

int HVN_msg_resp_data_recv(HVN_msg_data_t** msg_out, int fd)
{

}

/* EOF */
