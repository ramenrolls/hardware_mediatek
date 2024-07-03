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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <android/log.h>
#include <log/log.h>
#include <inttypes.h>

#include <mtk_log.h>

#define MAX_LOG_LEN 1024

void mtkLogD(const char* tag, const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, MAX_LOG_LEN, fmt, args);
    va_end(args);
    __android_log_buf_write(LOG_ID_RADIO, ANDROID_LOG_DEBUG, tag, buf);
}

void mtkLogV(const char* tag, const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, MAX_LOG_LEN, fmt, args);
    va_end(args);
    __android_log_buf_write(LOG_ID_RADIO, ANDROID_LOG_VERBOSE, tag, buf);
}

void mtkLogI(const char* tag, const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, MAX_LOG_LEN, fmt, args);
    va_end(args);
    __android_log_buf_write(LOG_ID_RADIO, ANDROID_LOG_INFO, tag, buf);
}

void mtkLogW(const char* tag, const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, MAX_LOG_LEN, fmt, args);
    va_end(args);
    __android_log_buf_write(LOG_ID_RADIO, ANDROID_LOG_WARN, tag, buf);
}

void mtkLogE(const char* tag, const char* fmt, ...) {
    char buf[MAX_LOG_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, MAX_LOG_LEN, fmt, args);
    va_end(args);
    __android_log_buf_write(LOG_ID_RADIO, ANDROID_LOG_ERROR, tag, buf);
}

void mtkAssert(char* pErrMsg) {
    if (pErrMsg) {
        LOG_ALWAYS_FATAL("%s", pErrMsg);
    } else {
        LOG_ALWAYS_FATAL();
    }
}
