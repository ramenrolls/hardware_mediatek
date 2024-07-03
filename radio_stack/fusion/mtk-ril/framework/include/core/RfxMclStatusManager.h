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

#ifndef __RFX_MCL_STATUS_MANAGER_H__
#define __RFX_MCL_STATUS_MANAGER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxDefs.h"
#include "RfxStatusDefs.h"
#include "RfxVariant.h"
#include "utils/Mutex.h"

using ::android::Mutex;

/*****************************************************************************
 * Class RfxMclStatusManager
 *****************************************************************************/

class RfxMclStatusManager {
  public:
    RfxMclStatusManager() : m_slot_id(RFX_SLOT_ID_UNKNOWN) {
        for (int i = 0; i < RFX_STATUS_KEY_END_OF_ENUM; i++) {
            m_status_list[i] = NULL;
        }
    }

    explicit RfxMclStatusManager(int slot_id);

    virtual ~RfxMclStatusManager() {}

    static void init();

    static RfxMclStatusManager* getMclStatusManager(int slot_id);

    static RfxMclStatusManager* getNonSlotMclStatusManager();

  public:
    bool getBoolValue(const RfxStatusKeyEnum key, bool default_value) const;

    bool getBoolValue(const RfxStatusKeyEnum key) const;

    void setBoolValue(const RfxStatusKeyEnum key, bool value, bool force_notify = false,
                      bool is_default = false);

    int getIntValue(const RfxStatusKeyEnum key, int default_value) const;

    int getIntValue(const RfxStatusKeyEnum key) const;

    void setIntValue(const RfxStatusKeyEnum key, int value, bool force_notify = false,
                     bool is_default = false);

    float getFloatValue(const RfxStatusKeyEnum key, float default_value) const;

    float getFloatValue(const RfxStatusKeyEnum key) const;

    void setFloatValue(const RfxStatusKeyEnum key, float value, bool force_notify = false,
                       bool is_default = false);

    int64_t getInt64Value(const RfxStatusKeyEnum key, int64_t default_value) const;

    int64_t getInt64Value(const RfxStatusKeyEnum key) const;

    void setInt64Value(const RfxStatusKeyEnum key, int64_t value, bool force_notify = false,
                       bool is_default = false);

    const String8& getString8Value(const RfxStatusKeyEnum key, const String8& default_value) const;

    const String8& getString8Value(const RfxStatusKeyEnum key) const;

    void setString8Value(const RfxStatusKeyEnum key, const String8& value,
                         bool force_notify = false, bool is_default = false);

    const RfxNwServiceState& getServiceStateValue(const RfxStatusKeyEnum key,
                                                  const RfxNwServiceState& default_value) const;

    const RfxNwServiceState& getServiceStateValue(const RfxStatusKeyEnum key) const;

    void setServiceStateValue(const RfxStatusKeyEnum key, const RfxNwServiceState& value,
                              bool force_notify = false, bool is_default = false);

    const RfxCallState& getCallStateValue(const RfxStatusKeyEnum key,
                                          const RfxCallState& default_value) const;

    const RfxCallState& getCallStateValue(const RfxStatusKeyEnum key) const;

    void setCallStateValue(const RfxStatusKeyEnum key, const RfxCallState& value,
                           bool force_notify = false, bool is_default = false);

    const RfxVariant& getValue(const RfxStatusKeyEnum key, const RfxVariant& default_value) const;

    const RfxVariant& getValue(const RfxStatusKeyEnum key) const;

    // module uses
    void setValue(const RfxStatusKeyEnum key, const RfxVariant& value, bool force_notify = false,
                  bool is_default = false);

    // framework uses
    void setValueByRfx(const RfxStatusKeyEnum key, const RfxVariant& value, bool force_notify,
                       bool is_default, bool is_status_sync, bool update_for_mock = false);

    void setValueInternal(const RfxStatusKeyEnum key, const RfxVariant& value, bool force_notify,
                          bool is_default, bool is_status_sync, bool update_for_mock = false);

    int getSlotId() const { return m_slot_id; }

    static const RfxVariant& getDefaultValue(const RfxStatusKeyEnum key);
    static const char* getKeyString(const RfxStatusKeyEnum key);

  private:
    void updateValueToTelCore(int slot_id, const RfxStatusKeyEnum key, const RfxVariant value,
                              bool force_notify = false, bool is_default = false);

  private:
    typedef struct _StatusListEntry {
        RfxVariant value;
    } StatusListEntry;

  private:
    static RfxMclStatusManager* s_self[MAX_SIM_COUNT + 1];
    int m_slot_id;
    StatusListEntry* m_status_list[RFX_STATUS_KEY_END_OF_ENUM];
    mutable Mutex m_mutex[RFX_STATUS_KEY_END_OF_ENUM];
};

inline bool RfxMclStatusManager::getBoolValue(const RfxStatusKeyEnum key,
                                              bool default_value) const {
    return getValue(key, RfxVariant(default_value)).asBool();
}

inline bool RfxMclStatusManager::getBoolValue(const RfxStatusKeyEnum key) const {
    return getValue(key).asBool();
}

inline void RfxMclStatusManager::setBoolValue(const RfxStatusKeyEnum key, bool value,
                                              bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline int RfxMclStatusManager::getIntValue(const RfxStatusKeyEnum key, int default_value) const {
    return getValue(key, RfxVariant(default_value)).asInt();
}

inline int RfxMclStatusManager::getIntValue(const RfxStatusKeyEnum key) const {
    return getValue(key).asInt();
}

inline void RfxMclStatusManager::setIntValue(const RfxStatusKeyEnum key, int value,
                                             bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline float RfxMclStatusManager::getFloatValue(const RfxStatusKeyEnum key,
                                                float default_value) const {
    return getValue(key, RfxVariant(default_value)).asFloat();
}

inline float RfxMclStatusManager::getFloatValue(const RfxStatusKeyEnum key) const {
    return getValue(key).asFloat();
}

inline void RfxMclStatusManager::setFloatValue(const RfxStatusKeyEnum key, float value,
                                               bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline int64_t RfxMclStatusManager::getInt64Value(const RfxStatusKeyEnum key,
                                                  int64_t default_value) const {
    return getValue(key, RfxVariant(default_value)).asInt64();
}

inline int64_t RfxMclStatusManager::getInt64Value(const RfxStatusKeyEnum key) const {
    return getValue(key).asInt64();
}

inline void RfxMclStatusManager::setInt64Value(const RfxStatusKeyEnum key, int64_t value,
                                               bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline const String8& RfxMclStatusManager::getString8Value(const RfxStatusKeyEnum key,
                                                           const String8& default_value) const {
    return getValue(key, RfxVariant(default_value)).asString8();
}

inline const String8& RfxMclStatusManager::getString8Value(const RfxStatusKeyEnum key) const {
    return getValue(key).asString8();
}

inline void RfxMclStatusManager::setString8Value(const RfxStatusKeyEnum key, const String8& value,
                                                 bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline const RfxNwServiceState& RfxMclStatusManager::getServiceStateValue(
        const RfxStatusKeyEnum key, const RfxNwServiceState& default_value) const {
    return getValue(key, RfxVariant(default_value)).asServiceState();
}

inline const RfxNwServiceState& RfxMclStatusManager::getServiceStateValue(
        const RfxStatusKeyEnum key) const {
    return getValue(key).asServiceState();
}

inline void RfxMclStatusManager::setServiceStateValue(const RfxStatusKeyEnum key,
                                                      const RfxNwServiceState& value,
                                                      bool force_notify, bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

inline const RfxCallState& RfxMclStatusManager::getCallStateValue(
        const RfxStatusKeyEnum key, const RfxCallState& default_value) const {
    return getValue(key, RfxVariant(default_value)).asCallState();
}

inline const RfxCallState& RfxMclStatusManager::getCallStateValue(
        const RfxStatusKeyEnum key) const {
    return getValue(key).asCallState();
}

inline void RfxMclStatusManager::setCallStateValue(const RfxStatusKeyEnum key,
                                                   const RfxCallState& value, bool force_notify,
                                                   bool is_default) {
    setValue(key, RfxVariant(value), force_notify, is_default);
}

#endif /* __RFX_STATUS_MANAGER_H__ */
