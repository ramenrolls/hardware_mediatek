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

#ifndef WPFA_DRIVER_BASE_DATA_H
#define WPFA_DRIVER_BASE_DATA_H

#include <stdlib.h>

/*
 * =============================================================================
 *                     define
 * =============================================================================
 */

class WpfaDriverBaseData;  // Predefined class

#define WPFA_DECLARE_DATA_CLASS(_class_name)                           \
  private:                                                             \
    _class_name(const _class_name& other);                             \
                                                                       \
  public:                                                              \
    _class_name(void* data, int length);                               \
    virtual ~_class_name();                                            \
                                                                       \
  public:                                                              \
    static WpfaDriverBaseData* copyDataByData(void* data, int length); \
    static WpfaDriverBaseData* copyDataByObj(const WpfaDriverBaseData* data)

#define WPFA_IMPLEMENT_DATA_CLASS(_class_name)                                       \
    WpfaDriverBaseData* _class_name::copyDataByData(void* data, int length) {        \
        _class_name* ret = new _class_name(data, length);                            \
        return ret;                                                                  \
    }                                                                                \
                                                                                     \
    WpfaDriverBaseData* _class_name::copyDataByObj(const WpfaDriverBaseData* data) { \
        _class_name* ret = new _class_name(data->getData(), data->getDataLength());  \
        return ret;                                                                  \
    }

#define WPFA_UNUSED(x) ((void)(x))
/*
 * =============================================================================
 *                     class
 * =============================================================================
 */

class WpfaDriverBaseData {
  public:
    WpfaDriverBaseData(void* data, int length) : mData(NULL), mLength(0) {
        WPFA_UNUSED(data);
        WPFA_UNUSED(length);
    }

    virtual ~WpfaDriverBaseData() {}

    void* getData() const { return mData; }

    int getDataLength() const { return mLength; }

  protected:
    void* mData;
    int mLength;
};

#endif /* end of WPFA_DRIVER_BASE_DATA_H */
