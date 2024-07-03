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

#ifndef __MTK_LOG_H
#define __MTK_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

void mtkLogD(const char* tag, const char* fmt, ...);
void mtkLogI(const char* tag, const char* fmt, ...);
void mtkLogV(const char* tag, const char* fmt, ...);
void mtkLogW(const char* tag, const char* fmt, ...);
void mtkLogE(const char* tag, const char* fmt, ...);
void mtkAssert(char* pErrMsg);

#undef SLOGD
#undef SLOGW
#undef SLOGE
#undef SLOGV
#undef SLOGI

#define SLOGD(...) (mtkLogD(LOG_TAG, __VA_ARGS__))
#define SLOGW(...) (mtkLogW(LOG_TAG, __VA_ARGS__))
#define SLOGE(...) (mtkLogE(LOG_TAG, __VA_ARGS__))
#define SLOGV(...) (mtkLogV(LOG_TAG, __VA_ARGS__))
#define SLOGI(...) (mtkLogI(LOG_TAG, __VA_ARGS__))

#undef ALOGD
#undef ALOGE
#undef ALOGW
#undef ALOGV
#undef ALOGI

#define ALOGD(...) (mtkLogD(LOG_TAG, __VA_ARGS__))
#define ALOGE(...) (mtkLogE(LOG_TAG, __VA_ARGS__))
#define ALOGW(...) (mtkLogW(LOG_TAG, __VA_ARGS__))
#define ALOGV(...) (mtkLogV(LOG_TAG, __VA_ARGS__))
#define ALOGI(...) (mtkLogI(LOG_TAG, __VA_ARGS__))

#undef LOG_ALWAYS_FATAL_IF
#define LOG_ALWAYS_FATAL_IF(cond, ...)     \
    do {                                   \
        if (cond) {                        \
            mtkLogE(LOG_TAG, __VA_ARGS__); \
            mtkAssert(NULL);               \
            exit(0);                       \
        }                                  \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* __MTK_LOG_H */
