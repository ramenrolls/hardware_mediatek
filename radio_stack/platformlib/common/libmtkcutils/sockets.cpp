/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This file contains socket implementation that can be shared between
// platforms as long as the correct headers are included.

#include <cutils/sockets.h>

int socket_get_local_port(cutils_socket_t sock) {
    sockaddr_storage addr;
    socklen_t addr_size = sizeof(addr);

    if (getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &addr_size) == 0) {
        // sockaddr_in and sockaddr_in6 always overlap the port field.
        return ntohs(reinterpret_cast<sockaddr_in*>(&addr)->sin_port);
    }
    return -1;
}
