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

#ifndef __RFX_VARIANT_H__
#define __RFX_VARIANT_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "utils/String8.h"
#include "utils/Vector.h"
#include "RfxBasics.h"
#include "RfxNwServiceState.h"
#include "RfxCallState.h"
#include "RfxImsCallInfo.h"

using ::android::String8;
using ::android::Vector;

/*****************************************************************************
 * Class RfxVariant
 *****************************************************************************/

/*
 * RfxVariant is a union for the common types
 */
class RfxVariant {
    // Define
  public:
    // Value type enum
    enum ValueTypeEnum {
        // Null type
        DATA_TYPE_NULL,
        // bool type
        DATA_TYPE_BOOL,
        // int type
        DATA_TYPE_INT,
        // float type
        DATA_TYPE_FLOAT,
        // String type
        DATA_TYPE_STRING8,
        // ServiceState type
        DATA_TYPE_SERVICE_STATE,
        // CallState type
        DATA_TYPE_CALL_STATE,
        // Char Vector
        DATA_TYPE_CHAR_VECTOR,
        // int64_t type
        DATA_TYPE_INT64,
        // String Vector
        DATA_TYPE_STRING8_VECTOR,
        // ImsCallInfo Vector type
        DATA_TYPE_IMS_CALL_INFO_VECTOR,

        DATA_TYPE_END_OF_ENUM
    };

    // Constructor / Destructor
  public:
    // Default constructor
    RfxVariant() : m_type(DATA_TYPE_NULL) {}

    // Copy constructor
    RfxVariant(const RfxVariant& other) : m_type(DATA_TYPE_NULL) { initWith(other); }

    // Destructor
    ~RfxVariant() { deinit(); }

    /*
     * construct by value in different types
     */
    explicit RfxVariant(bool value);
    explicit RfxVariant(int value);
    explicit RfxVariant(float value);
    explicit RfxVariant(int64_t value);
    explicit RfxVariant(const String8& value);
    explicit RfxVariant(const RfxNwServiceState& value);
    explicit RfxVariant(const RfxCallState& value);
    explicit RfxVariant(const Vector<char>& value);
    explicit RfxVariant(const Vector<String8>& value);
    explicit RfxVariant(const Vector<RfxImsCallInfo*>& value);
    // Method
  public:
    // Return the value type of the variant
    ValueTypeEnum get_type() const { return m_type; }

    // Get value as bool type in the variant
    bool asBool() const;
    // Get value as int type in the variant
    int asInt() const;
    // Get value as float type in the variant
    float asFloat() const;
    // Get value as String8 type in the variant
    const String8& asString8() const;

    const RfxNwServiceState& asServiceState() const;

    const RfxCallState& asCallState() const;

    const Vector<char>& asCharVector() const;

    const Vector<String8>& asString8Vector() const;

    const Vector<RfxImsCallInfo*>& asImsCallInfo() const;

    // Get value as int64_t type in the variant
    int64_t asInt64() const;

    String8 toString() const;

    // Operator
  public:
    // operator =
    RfxVariant& operator=(const RfxVariant& other);

    // operator ==
    bool operator==(const RfxVariant& other);

    // operator !=
    bool operator!=(const RfxVariant& other);

  private:
    // the type of the value stored in the variant
    ValueTypeEnum m_type;

    // the value stored in the variant
    union {
        /*
         * Primitive types
         */
        bool boolValue;
        int intValue;
        float floatValue;
        int64_t int64Value;
        /*
         * Classes
         */
        uint8_t string8Buffer[sizeof(String8)];
        uint8_t serviceStateBuffer[sizeof(RfxNwServiceState)];
        uint8_t callStateBuffer[sizeof(RfxCallState)];
        uint8_t charVectorBuffer[sizeof(Vector<char>)];
        uint8_t string8VectorBuffer[sizeof(Vector<String8>)];
        uint8_t imsCallVectorBuffer[sizeof(Vector<RfxImsCallInfo*>)];
    } m_value;

    void initWith(const RfxVariant& other);
    void deinit();
    void assignWith(const RfxVariant& other);
    bool isEqualWith(const RfxVariant& other);
};

inline RfxVariant::RfxVariant(bool value) : m_type(DATA_TYPE_BOOL) { m_value.boolValue = value; }

inline RfxVariant::RfxVariant(int value) : m_type(DATA_TYPE_INT) { m_value.intValue = value; }

inline RfxVariant::RfxVariant(float value) : m_type(DATA_TYPE_FLOAT) { m_value.floatValue = value; }

inline RfxVariant::RfxVariant(int64_t value) : m_type(DATA_TYPE_INT64) {
    m_value.int64Value = value;
}

inline RfxVariant::RfxVariant(const String8& value) : m_type(DATA_TYPE_STRING8) {
    new (m_value.string8Buffer) String8(value);
}

inline RfxVariant::RfxVariant(const RfxNwServiceState& value) : m_type(DATA_TYPE_SERVICE_STATE) {
    new (m_value.serviceStateBuffer) RfxNwServiceState(value);
}

inline RfxVariant::RfxVariant(const RfxCallState& value) : m_type(DATA_TYPE_CALL_STATE) {
    new (m_value.callStateBuffer) RfxCallState(value);
}

inline RfxVariant::RfxVariant(const Vector<char>& value) : m_type(DATA_TYPE_CHAR_VECTOR) {
    new (m_value.charVectorBuffer) Vector<char>(value);
}

inline RfxVariant::RfxVariant(const Vector<String8>& value) : m_type(DATA_TYPE_STRING8_VECTOR) {
    new (m_value.string8VectorBuffer) Vector<String8>(value);
}

inline RfxVariant::RfxVariant(const Vector<RfxImsCallInfo*>& value)
    : m_type(DATA_TYPE_IMS_CALL_INFO_VECTOR) {
    new (m_value.imsCallVectorBuffer) Vector<RfxImsCallInfo*>(value);
}

inline bool RfxVariant::asBool() const {
    RFX_ASSERT(m_type == DATA_TYPE_BOOL);
    return m_value.boolValue;
}

inline int RfxVariant::asInt() const {
    RFX_ASSERT(m_type == DATA_TYPE_INT);
    return m_value.intValue;
}

inline float RfxVariant::asFloat() const {
    RFX_ASSERT(m_type == DATA_TYPE_FLOAT);
    return m_value.floatValue;
}

inline int64_t RfxVariant::asInt64() const {
    RFX_ASSERT(m_type == DATA_TYPE_INT64);
    return m_value.int64Value;
}

inline const String8& RfxVariant::asString8() const {
    RFX_ASSERT(m_type == DATA_TYPE_STRING8);
    return *(String8*)m_value.string8Buffer;
}

inline const RfxNwServiceState& RfxVariant::asServiceState() const {
    RFX_ASSERT(m_type == DATA_TYPE_SERVICE_STATE);
    return *(RfxNwServiceState*)m_value.serviceStateBuffer;
}

inline const RfxCallState& RfxVariant::asCallState() const {
    RFX_ASSERT(m_type == DATA_TYPE_CALL_STATE);
    return *(RfxCallState*)m_value.callStateBuffer;
}

inline const Vector<char>& RfxVariant::asCharVector() const {
    RFX_ASSERT(m_type == DATA_TYPE_CHAR_VECTOR);
    return *(Vector<char>*)m_value.charVectorBuffer;
}

inline const Vector<String8>& RfxVariant::asString8Vector() const {
    RFX_ASSERT(m_type == DATA_TYPE_STRING8_VECTOR);
    return *(Vector<String8>*)m_value.string8VectorBuffer;
}

inline const Vector<RfxImsCallInfo*>& RfxVariant::asImsCallInfo() const {
    RFX_ASSERT(m_type == DATA_TYPE_IMS_CALL_INFO_VECTOR);
    return *(Vector<RfxImsCallInfo*>*)m_value.imsCallVectorBuffer;
}

/*****************************************************************************
 * Class RfxVariantUtil
 *****************************************************************************/

/*
 * Internal used. RfxVariantUtil provides the type information.
 */
template <class _value_type>
class RfxVariantUtil {
  public:
    typedef _value_type ret_type;
    typedef _value_type arg_type;
};

template <>
class RfxVariantUtil<bool> {
  public:
    typedef bool ret_type;
    typedef bool arg_type;

    inline static bool getVariantValue(const RfxVariant& variant) { return variant.asBool(); }
};

template <>
class RfxVariantUtil<int> {
  public:
    typedef int ret_type;
    typedef int arg_type;

    inline static int getVariantValue(const RfxVariant& variant) { return variant.asInt(); }
};

template <>
class RfxVariantUtil<float> {
  public:
    typedef float ret_type;
    typedef float arg_type;

    inline static float getVariantValue(const RfxVariant& variant) { return variant.asFloat(); }
};

template <>
class RfxVariantUtil<String8> {
  public:
    typedef const String8& ret_type;
    typedef const String8& arg_type;

    inline static const String8& getVariantValue(const RfxVariant& variant) {
        return variant.asString8();
    }
};

template <>
class RfxVariantUtil<RfxNwServiceState> {
  public:
    typedef const RfxNwServiceState& ret_type;
    typedef const RfxNwServiceState& arg_type;

    inline static const RfxNwServiceState& getVariantValue(const RfxVariant& variant) {
        return variant.asServiceState();
    }
};

template <>
class RfxVariantUtil<int64_t> {
  public:
    typedef int64_t ret_type;
    typedef int64_t arg_type;

    inline static int64_t getVariantValue(const RfxVariant& variant) { return variant.asInt64(); }
};

#endif /* __RFX_VARIANT_H__ */
