#ifndef __HVN__HAVEND_HOOK_H
#define __HVN__HAVEND_HOOK_H

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

typedef struct HVN_hook_t {
    uuid_t remote_uuid;
    int report_port;
    char* remote_address;
    int fd_append;
    int fd_vote;
    Channel* append_chan;
    Channel* vote_chan;
    Channel* exit_chan;
} HVN_hook_t;

#endif /* __HVN__HAVEND_HOOK_H */
