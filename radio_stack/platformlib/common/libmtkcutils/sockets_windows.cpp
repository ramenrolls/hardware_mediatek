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

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms741549(v=vs.85).aspx
// claims WSACleanup() should be called before program exit, but general
// consensus seems to be that it hasn't actually been necessary for a long time,
// likely since Windows 3.1. Additionally, trying to properly use WSACleanup()
// can be extremely tricky and cause deadlock when using threads or atexit().
//
// Both adb (1) and Chrome (2) purposefully avoid WSACleanup() with no issues.
// (1) https://android.googlesource.com/platform/system/core.git/+/master/adb/sysdeps_win32.cpp
// (2) https://code.google.com/p/chromium/codesearch#chromium/src/net/base/winsock_init.cc
extern "C" bool initialize_windows_sockets() {
    // There's no harm in calling WSAStartup() multiple times but no benefit
    // either, we may as well skip it after the first.
    static bool init_success = false;

    if (!init_success) {
        WSADATA wsaData;
        init_success = (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
    }

    return init_success;
}

int socket_close(cutils_socket_t sock) { return closesocket(sock); }

int socket_set_receive_timeout(cutils_socket_t sock, int timeout_ms) {
    return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout_ms),
                      sizeof(timeout_ms));
}

ssize_t socket_send_buffers(cutils_socket_t sock, const cutils_socket_buffer_t* buffers,
                            size_t num_buffers) {
    if (num_buffers > SOCKET_SEND_BUFFERS_MAX_BUFFERS) {
        return -1;
    }

    WSABUF wsa_buffers[SOCKET_SEND_BUFFERS_MAX_BUFFERS];
    for (size_t i = 0; i < num_buffers; ++i) {
        // It's safe to cast away const here; WSABUF declares non-const
        // void* because it's used for both send and receive, but since
        // we're only sending, the data won't be modified.
        wsa_buffers[i].buf = reinterpret_cast<char*>(const_cast<void*>(buffers[i].data));
        wsa_buffers[i].len = buffers[i].length;
    }

    DWORD bytes_sent = 0;
    if (WSASend(sock, wsa_buffers, num_buffers, &bytes_sent, 0, nullptr, nullptr) != SOCKET_ERROR) {
        return bytes_sent;
    }

    return -1;
}
