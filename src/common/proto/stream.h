#ifndef TDB__COMMON_PROTO_STREAM_H
#define TDB__COMMON_PROTO_STREAM_H

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
#include <rpc/xdr.h>

typedef struct TDB_xdr_stream_t {
    int* fd;
    XDR xdrs;
} TDB_xdr_stream_t;

int TDB_init_xdr_stream(TDB_xdr_stream_t** xdr_stream, int* fd);
void TDB_free_xdr_stream(TDB_xdr_stream_t* xdr_stream);

int TDB_fdwriten(TDB_xdr_stream_t* xdr_stream, char* buf, int len);
int TDB_fdreadn(TDB_xdr_stream_t* xdr_stream, char* buf, int len);

#endif /* TDB__COMMON_PROTO_STREAM_H */
