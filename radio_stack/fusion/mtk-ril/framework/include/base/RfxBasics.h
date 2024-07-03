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

#ifndef __RFX_BASICS_H__
#define __RFX_BASICS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxLog.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

#define RFX_UNUSED(x) ((void)(x))

#define RFX_FLAG_SET(_v, _flags) \
    do {                         \
        (_v) |= (_flags);        \
    } while (0)

#define RFX_FLAG_CLEAR(_v, _flags) \
    do {                           \
        (_v) &= ~(_flags);         \
    } while (0)

#define RFX_FLAG_HAS(_v, _flags) (((_v) & (_flags)) != 0)

#define RFX_FLAG_HAS_ALL(_v, _flags) (((_v) & (_flags)) == (_flags))

#define RFX_DECLARE_NO_COPY_CLASS(_class_name) \
  private:                                     \
    _class_name(const _class_name&);           \
    _class_name& operator=(const _class_name&)

#endif  // __RFX_BASICS_H__
