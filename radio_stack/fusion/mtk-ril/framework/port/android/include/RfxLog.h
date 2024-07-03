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

#ifndef __RFX_LOG_H__
#define __RFX_LOG_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "rfx_gt_log.h"
#include <mtk_log.h>
#include "utils/String8.h"

using ::android::String8;

/*****************************************************************************
 * Example
 *****************************************************************************/
/* Add a debug log with your tag, write it like:
 * RFX_LOG_D(tag, "this is a sample");
 *
 * Print a variable, write it like:
 * RFX_LOG_D(tag, "this is a sample %d", variable);
 *
 * Print multi-variable, write it like:
 * RFX_LOG_D(tag, "this is a sample %d,%d", variable1, variable2);
 *
 * Staple output format
 * %c  char
 * %s  char* string
 * %d  sign decimal
 * %p  pointer
 * %x  hex
 *
 * Add a debug log with your condition and tag, write it like:
 * RFX_LOG_D_IF(condition, tag, "this is a sample");
 * When condition is not 0 (this is true), the log will be printed, otherwise, no log printed.
 *
 */

/*****************************************************************************
 * Define
 *****************************************************************************/

/*
 * Simplified macro to send a verbose radio log message using the user given tag - _rfx_tag.
 */
#ifndef RFX_LOG_V
#define __RFX_LOG_V(_rfx_tag, ...)                                         \
    do {                                                                   \
        if (__rfx_is_gt_mode()) {                                          \
            String8 tagString = String8::format("%s%s", "[GT]", _rfx_tag); \
            mtkLogV(tagString, __VA_ARGS__);                               \
        } else {                                                           \
            mtkLogV(_rfx_tag, __VA_ARGS__);                                \
        }                                                                  \
    } while (0)

#if LOG_NDEBUG
#define RFX_LOG_V(_rfx_tag, ...)                \
    do {                                        \
        if (0) {                                \
            __RFX_LOG_V(_rfx_tag, __VA_ARGS__); \
        }                                       \
    } while (0)
#else
#define RFX_LOG_V(_rfx_tag, ...) __RFX_LOG_V(_rfx_tag, __VA_ARGS__)
#endif
#endif

#define CONDITION(cond) (__builtin_expect((cond) != 0, 0))

#ifndef RFX_LOG_V_IF
#if LOG_NDEBUG
#define RFX_LOG_V_IF(cond, _rfx_tag, ...) ((void)0)
#else
#define RFX_LOG_V_IF(cond, _rfx_tag, ...) \
    ((CONDITION(cond)) ? ((void)mtkLogV(_rfx_tag, __VA_ARGS__)) : (void)0)
#endif
#endif

/*
 * Simplified macro to send a debug radio log message using the user given tag - _rfx_tag.
 */
#ifndef RFX_LOG_D
#define RFX_LOG_D(_rfx_tag, ...)                                           \
    do {                                                                   \
        if (__rfx_is_gt_mode()) {                                          \
            String8 tagString = String8::format("%s%s", "[GT]", _rfx_tag); \
            mtkLogD(tagString, __VA_ARGS__);                               \
        } else {                                                           \
            mtkLogD(_rfx_tag, __VA_ARGS__);                                \
        }                                                                  \
    } while (0)

#endif

#ifndef RFX_LOG_D_IF
#define RFX_LOG_D_IF(cond, _rfx_tag, ...) \
    ((CONDITION(cond)) ? ((void)mtkLogD(_rfx_tag, __VA_ARGS__)) : (void)0)
#endif

/*
 * Simplified macro to send an info radio log message using the user given tag - _rfx_tag.
 */
#ifndef RFX_LOG_I
#define RFX_LOG_I(_rfx_tag, ...)                                           \
    do {                                                                   \
        if (__rfx_is_gt_mode()) {                                          \
            String8 tagString = String8::format("%s%s", "[GT]", _rfx_tag); \
            mtkLogI(tagString, __VA_ARGS__);                               \
        } else {                                                           \
            mtkLogI(_rfx_tag, __VA_ARGS__);                                \
        }                                                                  \
    } while (0)

#endif

#ifndef RFX_LOG_I_IF
#define RFX_LOG_I_IF(cond, _rfx_tag, ...) \
    ((CONDITION(cond)) ? ((void)mtkLogI(_rfx_tag, __VA_ARGS__)) : (void)0)
#endif

/*
 * Simplified macro to send a warning radio log message using the user given tag - _rfx_tag.
 */
#ifndef RFX_LOG_W
#define RFX_LOG_W(_rfx_tag, ...)                                           \
    do {                                                                   \
        if (__rfx_is_gt_mode()) {                                          \
            String8 tagString = String8::format("%s%s", "[GT]", _rfx_tag); \
            mtkLogW(tagString, __VA_ARGS__);                               \
        } else {                                                           \
            mtkLogW(_rfx_tag, __VA_ARGS__);                                \
        }                                                                  \
    } while (0)

#endif

#ifndef RFX_LOG_W_IF
#define RFX_LOG_W_IF(cond, _rfx_tag, ...) \
    ((CONDITION(cond)) ? ((void)mtkLogW(_rfx_tag, __VA_ARGS__)) : (void)0)
#endif

/*
 * Simplified macro to send an error radio log message using the user given tag - _rfx_tag.
 */
#ifndef RFX_LOG_E
#define RFX_LOG_E(_rfx_tag, ...)                                           \
    do {                                                                   \
        if (__rfx_is_gt_mode()) {                                          \
            String8 tagString = String8::format("%s%s", "[GT]", _rfx_tag); \
            mtkLogE(tagString, __VA_ARGS__);                               \
        } else {                                                           \
            mtkLogE(_rfx_tag, __VA_ARGS__);                                \
        }                                                                  \
    } while (0)

#endif

#ifndef RFX_LOG_E_IF
#define RFX_LOG_E_IF(cond, _rfx_tag, ...) \
    ((CONDITION(cond)) ? ((void)mtkLogE(_rfx_tag, __VA_ARGS__)) : (void)0)
#endif

#ifndef RFX_ASSERT
#define RFX_ASSERT(_expr)                                                     \
    do {                                                                      \
        if (!(_expr)) {                                                       \
            RFX_LOG_E("RFX_ASSERT", "RFX_ASSERT:%s, %d", __FILE__, __LINE__); \
            mtkAssert(NULL);                                                  \
            exit(0);                                                          \
        }                                                                     \
    } while (0)
#endif

#endif /* __RFX_LOG_H__ */
