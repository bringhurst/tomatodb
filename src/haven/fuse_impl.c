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

#include "fuse_impl.h"

int HVN_fuse_opers_getattr(const char* path, struct stat* stbuf)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(path);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(stbuf);

    //FIXME: not implemented.
    return -1;
}

int HVN_fuse_opers_getdir(const char* path, fuse_dirh_t buf, \
                          fuse_dirfil_t filler)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(path);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(buf);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(filler);

    //FIXME: not implemented.
    return -1;
}

int HVN_fuse_opers_open(const char* path, struct fuse_file_info_compat* fi)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(path);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(fi);

    //FIXME: not implemented.
    return -1;
}

int HVN_fuse_opers_read(const char* path, char* buf, size_t size, \
                        off_t offset, struct fuse_file_info_compat* fi)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(path);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(buf);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(size);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(offset);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(fi);

    //FIXME: not implemented.
    return -1;
}

int main(int argc, char** argv)
{
    struct fuse_operations_compat2 fuse_opers;
    /*
        fuse_opers.getattr = HVN_fuse_opers_getattr;
        fuse_opers.getdir  = HVN_fuse_opers_getdir;
        fuse_opers.open    = HVN_fuse_opers_open;
        fuse_opers.read    = HVN_fuse_opers_read;
    */
    return fuse_main(argc, argv, &fuse_opers);
}

/* EOF */
