#ifndef __HVN__HAVEN_FUSE_H_
#define __HVN__HAVEN_FUSE_H_

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

#include <fuse.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int HVN_fuse_opers_getattr(const char* path, struct stat* stbuf);
int HVN_fuse_opers_getdir(const char* path, fuse_dirh_t buf, \
                          fuse_dirfil_t filler);
int HVN_fuse_opers_open(const char* path, struct fuse_file_info_compat* fi);
int HVN_fuse_opers_read(const char* path, char* buf, size_t size, \
                        off_t offset, struct fuse_file_info_compat* fi);

#endif /* __HVN__HAVEN_FUSE_H_ */
