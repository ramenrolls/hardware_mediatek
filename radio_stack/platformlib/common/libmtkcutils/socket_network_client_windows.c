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

#include <cutils/sockets.h>

extern bool initialize_windows_sockets();

SOCKET socket_network_client(const char* host, int port, int type) {
    if (!initialize_windows_sockets()) {
        return INVALID_SOCKET;
    }

    // First resolve the host and port parameters into a usable network address.
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = type;

    struct addrinfo* address = NULL;
    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);
    if (getaddrinfo(host, port_str, &hints, &address) != 0 || address == NULL) {
        if (address != NULL) {
            freeaddrinfo(address);
        }
        return INVALID_SOCKET;
    }

    // Now create and connect the socket.
    SOCKET sock = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (sock == INVALID_SOCKET) {
        freeaddrinfo(address);
        return INVALID_SOCKET;
    }

    if (connect(sock, address->ai_addr, address->ai_addrlen) == SOCKET_ERROR) {
        closesocket(sock);
        freeaddrinfo(address);
        return INVALID_SOCKET;
    }

    freeaddrinfo(address);
    return sock;
}
