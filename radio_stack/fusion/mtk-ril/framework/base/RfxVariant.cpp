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

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxVariant.h"

/*****************************************************************************
 * Class RfxVariant
 *****************************************************************************/

RfxVariant& RfxVariant::operator=(const RfxVariant& other) {
    if (m_type == other.m_type) {
        assignWith(other);
    } else {
        deinit();
        initWith(other);
    }

    return *this;
}

bool RfxVariant::operator==(const RfxVariant& other) { return isEqualWith(other); }

bool RfxVariant::operator!=(const RfxVariant& other) { return !isEqualWith(other); }

bool RfxVariant::isEqualWith(const RfxVariant& other) {
    if (m_type == other.m_type) {
        bool ret = false;
        switch (m_type) {
            case DATA_TYPE_NULL:
                ret = true;
                break;

            case DATA_TYPE_BOOL:
                ret = (m_value.boolValue == other.asBool());
                break;

            case DATA_TYPE_INT:
                ret = (m_value.intValue == other.asInt());
                break;

            case DATA_TYPE_FLOAT:
                ret = (m_value.floatValue == other.asFloat());
                break;

            case DATA_TYPE_INT64:
                ret = (m_value.int64Value == other.asInt64());
                break;

            case DATA_TYPE_STRING8:
                ret = ((*(String8*)(m_value.string8Buffer)) == other.asString8());
                break;

            case DATA_TYPE_SERVICE_STATE:
                ret = ((*(RfxNwServiceState*)(m_value.serviceStateBuffer)) ==
                       other.asServiceState());
                break;

            case DATA_TYPE_CALL_STATE:
                ret = ((*(RfxCallState*)(m_value.callStateBuffer)) == other.asCallState());
                break;

            case DATA_TYPE_CHAR_VECTOR:
                ret = (*(const Vector<char>*)(m_value.charVectorBuffer)).size() ==
                      other.asCharVector().size();
                if (ret) {
                    int r = memcmp((*(const Vector<char>*)(m_value.charVectorBuffer)).array(),
                                   other.asCharVector().array(), other.asCharVector().size());
                    ret = (r == 0);
                } else {
                    ret = false;
                }
                break;

            case DATA_TYPE_STRING8_VECTOR:
                ret = (*(const Vector<String8>*)(m_value.string8VectorBuffer)).size() ==
                      other.asString8Vector().size();
                if (ret) {
                    int r = memcmp((*(const Vector<String8>*)(m_value.string8VectorBuffer)).array(),
                                   other.asString8Vector().array(), other.asString8Vector().size());
                    ret = (r == 0);
                } else {
                    ret = false;
                }
                break;

            case DATA_TYPE_IMS_CALL_INFO_VECTOR:
                ret = (*(const Vector<RfxImsCallInfo*>*)(m_value.imsCallVectorBuffer)).size() ==
                      other.asImsCallInfo().size();
                if (ret) {
                    int r = memcmp((*(const Vector<RfxImsCallInfo*>*)(m_value.imsCallVectorBuffer))
                                           .array(),
                                   other.asImsCallInfo().array(), other.asImsCallInfo().size());
                    ret = (r == 0);
                } else {
                    ret = false;
                }
                break;
            default:
                RFX_ASSERT(0);
        }
        return ret;
    } else {
        return false;
    }
}

void RfxVariant::initWith(const RfxVariant& other) {
    RFX_ASSERT(m_type == DATA_TYPE_NULL);

    // Assign new type
    m_type = other.m_type;

    switch (other.m_type) {
        case DATA_TYPE_NULL:
            // Do nothing
            break;

        case DATA_TYPE_BOOL:
            m_value.boolValue = other.asBool();
            break;

        case DATA_TYPE_INT:
            m_value.intValue = other.asInt();
            break;

        case DATA_TYPE_FLOAT:
            m_value.floatValue = other.asFloat();
            break;

        case DATA_TYPE_INT64:
            m_value.int64Value = other.asInt64();
            break;

        case DATA_TYPE_STRING8:
            new (m_value.string8Buffer) String8(other.asString8());
            break;

        case DATA_TYPE_SERVICE_STATE:
            new (m_value.serviceStateBuffer) RfxNwServiceState(other.asServiceState());
            break;

        case DATA_TYPE_CALL_STATE:
            new (m_value.callStateBuffer) RfxCallState(other.asCallState());
            break;

        case DATA_TYPE_CHAR_VECTOR:
            new (m_value.charVectorBuffer) Vector<char>(other.asCharVector());
            break;

        case DATA_TYPE_STRING8_VECTOR:
            new (m_value.string8VectorBuffer) Vector<String8>(other.asString8Vector());
            break;

        case DATA_TYPE_IMS_CALL_INFO_VECTOR:
            new (m_value.imsCallVectorBuffer) Vector<RfxImsCallInfo*>(other.asImsCallInfo());
            break;

        default:
            RFX_ASSERT(0);
    }
}

void RfxVariant::deinit() {
    // Call the destructor
    switch (m_type) {
        case DATA_TYPE_NULL:
        case DATA_TYPE_BOOL:
        case DATA_TYPE_INT:
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_INT64:
            // no destructor for primitive types, do nothing
            break;

        case DATA_TYPE_STRING8:
            ((String8*)m_value.string8Buffer)->~String8();
            break;

        case DATA_TYPE_SERVICE_STATE:
            ((RfxNwServiceState*)m_value.serviceStateBuffer)->~RfxNwServiceState();
            break;

        case DATA_TYPE_CALL_STATE:
            ((RfxCallState*)m_value.callStateBuffer)->~RfxCallState();
            break;

        case DATA_TYPE_CHAR_VECTOR:
            ((Vector<char>*)m_value.charVectorBuffer)->~Vector<char>();
            break;

        case DATA_TYPE_STRING8_VECTOR:
            ((Vector<String8>*)m_value.string8VectorBuffer)->~Vector<String8>();
            break;

        case DATA_TYPE_IMS_CALL_INFO_VECTOR:
            ((Vector<RfxImsCallInfo*>*)m_value.imsCallVectorBuffer)->~Vector<RfxImsCallInfo*>();
            break;

        default:
            RFX_ASSERT(0);
    }

    m_type = DATA_TYPE_NULL;
}

void RfxVariant::assignWith(const RfxVariant& other) {
    RFX_ASSERT(m_type == other.m_type);

    switch (m_type) {
        case DATA_TYPE_NULL:
            // do nothing
            break;

        case DATA_TYPE_BOOL:
            m_value.boolValue = other.m_value.boolValue;
            break;

        case DATA_TYPE_INT:
            m_value.intValue = other.m_value.intValue;
            break;

        case DATA_TYPE_FLOAT:
            m_value.floatValue = other.m_value.floatValue;
            break;

        case DATA_TYPE_INT64:
            m_value.int64Value = other.m_value.int64Value;
            break;

        case DATA_TYPE_STRING8:
            *(String8*)m_value.string8Buffer = *(String8*)other.m_value.string8Buffer;
            break;

        case DATA_TYPE_SERVICE_STATE:
            *(RfxNwServiceState*)m_value.serviceStateBuffer =
                    *(RfxNwServiceState*)other.m_value.serviceStateBuffer;
            break;

        case DATA_TYPE_CALL_STATE:
            *(RfxCallState*)m_value.callStateBuffer = *(RfxCallState*)other.m_value.callStateBuffer;
            break;

        case DATA_TYPE_CHAR_VECTOR:
            *(Vector<char>*)m_value.charVectorBuffer =
                    *(Vector<char>*)other.m_value.charVectorBuffer;
            break;

        case DATA_TYPE_STRING8_VECTOR:
            *(Vector<String8>*)m_value.string8VectorBuffer =
                    *(Vector<String8>*)other.m_value.string8VectorBuffer;
            break;

        case DATA_TYPE_IMS_CALL_INFO_VECTOR:
            *(Vector<RfxImsCallInfo*>*)m_value.imsCallVectorBuffer =
                    *(Vector<RfxImsCallInfo*>*)other.m_value.imsCallVectorBuffer;
            break;

        default:
            RFX_ASSERT(0);
    }
}

String8 RfxVariant::toString() const {
    switch (m_type) {
        case DATA_TYPE_NULL:
            // do nothing
            break;

        case DATA_TYPE_BOOL:
            return String8::format("%s", m_value.boolValue ? "true" : "false");
            break;

        case DATA_TYPE_INT:
            return String8::format("%d", m_value.intValue);
            break;

        case DATA_TYPE_FLOAT:
            return String8::format("%.2f", m_value.floatValue);
            break;

        case DATA_TYPE_INT64:
            return String8::format("%lld", (long long)m_value.int64Value);
            break;

        case DATA_TYPE_STRING8:
            return *(String8*)m_value.string8Buffer;
            break;

        case DATA_TYPE_SERVICE_STATE:
            return ((RfxNwServiceState*)m_value.serviceStateBuffer)->toString();
            break;

        case DATA_TYPE_CALL_STATE:
            return ((RfxCallState*)m_value.callStateBuffer)->toString();
            break;

        case DATA_TYPE_CHAR_VECTOR:
            return String8("Char Vector");
            break;

        case DATA_TYPE_STRING8_VECTOR:
            return String8("String8 Vector");
            break;

        case DATA_TYPE_IMS_CALL_INFO_VECTOR:
            return String8("Ims call Vector");
            break;

        default:
            RFX_ASSERT(0);
    }
    return String8("");
}
