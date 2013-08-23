#ifndef __HVN_COMMON_COMMON_H
#define __HVN_COMMON_COMMON_H

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

// FIXME: make this configurable.
#define HVN_BASE_STATE_DIR "/var/lib/haven"

#define HVN_SUCCESS (1)
#define HVN_ERROR (-1)

#ifndef _POSIX_HOST_NAME_MAX
#define _POSIX_HOST_NAME_MAX 255
#endif

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 4096
#endif

int HVN_ensure_directory_exists(char* path);

#endif /* __HVN_COMMON_COMMON_H */
