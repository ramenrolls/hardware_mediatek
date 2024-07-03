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

#ifndef __RFX_MCL_MESSAGE__H__
#define __RFX_MCL_MESSAGE__H__

#include "utils/RefBase.h"
#include <utils/Timers.h>
#include <telephony/mtk_ril.h>
#include "RfxDefs.h"
#include "RfxBaseData.h"
#include "RfxDataCloneManager.h"
#include "RfxAtLine.h"
#include "RfxStatusDefs.h"
#include "RfxVariant.h"

using ::android::RefBase;
using ::android::sp;

class RfxMclMessage : public virtual RefBase {
  private:
    RfxMclMessage();
    virtual ~RfxMclMessage();

  public:
    RFX_MESSAGE_TYPE getType() const { return m_type; }
    int getId() const { return m_id; }
    int getChannelId() const { return m_channel_id; }
    void setChannelId(int channelId) { m_channel_id = channelId; }
    int getSlotId() const { return m_slot_id; }
    int getClientId() const { return m_client_id; }
    RfxBaseData* getData() const { return m_data; }
    RfxAtLine* getRawUrc() const { return m_raw_urc; }
    RfxAtLine* getRawUrc2() const { return m_raw_urc2; }
    RIL_Errno getError() const { return m_err; }
    void setError(RIL_Errno err) { m_err = err; }
    /*void setData(void *data, int length) {
        RfxBaseData *response_data = RfxDataCloneManager::copyData(m_id, data, length);
        m_response_data = response_data;
    }*/
    nsecs_t getDelayTime() const { return m_delayTime; }
    int getToken() const { return m_token; }
    MTK_RIL_REQUEST_PRIORITY getPriority() const { return m_priority; }
    void setPriority(MTK_RIL_REQUEST_PRIORITY priority) { m_priority = priority; }
    RfxStatusKeyEnum getStatusKey() const { return m_key; }
    RfxVariant getStatusValue() const { return m_value; }
    bool getForceNotify() const { return m_force_notify; }
    bool getIsDefault() const { return m_is_default; }
    bool getIsUpdateForMock() const { return m_update_for_mock; }
    void setSendToMainProtocol(bool toMainProtocol) { mSendToMainProtocol = toMainProtocol; }
    bool getSendToMainProtocol() { return mSendToMainProtocol; }
    RIL_Token getRilToken() const { return rilToken; }

    String8 toString() const;

    int getMainProtocolSlotId() const { return m_main_protocol_slot_id; }

    void setMainProtocolSlotId(int mainProtocolSlotId) {
        m_main_protocol_slot_id = mainProtocolSlotId;
    }

    nsecs_t getTimeStamp() const { return mTimeStamp; }

    bool getAddAtFront() const { return mAddAtFront; }

    void setAddAtFront(bool value) { mAddAtFront = value; }

  public:
    static sp<RfxMclMessage> obtainRequest(int id, RfxBaseData* data, int slot_id, int token,
                                           bool sendToMainProtocol, RIL_Token rilToken,
                                           nsecs_t timeStamp, bool addAtFront);

    static sp<RfxMclMessage> obtainRequest(int id, RfxBaseData* data, int slot_id, int token,
                                           bool sendToMainProtocol, RIL_Token rilToken,
                                           nsecs_t delayTime, nsecs_t timeStamp, bool addAtFront);

    static sp<RfxMclMessage> obtainResponse(int id, RIL_Errno err, const RfxBaseData& data,
                                            sp<RfxMclMessage> msg, bool copyData = false);

    static sp<RfxMclMessage> obtainResponse(RIL_Errno err, const RfxBaseData& data,
                                            sp<RfxMclMessage> msg);

    // framework use only, for raw URC
    static sp<RfxMclMessage> obtainRawUrc(int channel_id, RfxAtLine* line1, RfxAtLine* line2);

    static sp<RfxMclMessage> obtainUrc(int id, int slot_id, const RfxBaseData& data);

    static sp<RfxMclMessage> obtainEvent(
            int id, const RfxBaseData& data, int channel_id, int slot_id, int client_id = -1,
            int token = -1, nsecs_t delayTime = 0,
            MTK_RIL_REQUEST_PRIORITY priority =
                    MTK_RIL_REQUEST_PRIORITY::MTK_RIL_REQUEST_PRIORITY_MEDIUM);

    static sp<RfxMclMessage> obtainStatusSync(int slot_id, RfxStatusKeyEnum key,
                                              const RfxVariant value, bool force_notify,
                                              bool is_default, bool update_for_mock = false);

    // SAP
    static sp<RfxMclMessage> obtainSapRequest(int id, RfxBaseData* data, int slot_id, int token);

    static sp<RfxMclMessage> obtainSapResponse(int id, RIL_Errno e, const RfxBaseData& data,
                                               sp<RfxMclMessage> msg, bool copyData = false);

    static sp<RfxMclMessage> obtainSapUrc(int id, int slot_id, const RfxBaseData& data);

  private:
    RFX_MESSAGE_TYPE m_type;  // msg type: request, response, urc
    int m_id;
    RfxBaseData* m_data;
    int m_channel_id;
    int m_slot_id;
    int m_client_id;
    int m_token;  // pair with RfxMessage
    RfxAtLine* m_raw_urc;
    RfxAtLine* m_raw_urc2;  // for SMS
    RIL_Errno m_err;
    nsecs_t m_delayTime;
    MTK_RIL_REQUEST_PRIORITY m_priority;
    RIL_Token rilToken;

    // for StatusManager
    RfxStatusKeyEnum m_key;
    RfxVariant m_value;
    bool m_force_notify;
    bool m_is_default;
    bool m_update_for_mock;

    bool mSendToMainProtocol;
    int m_main_protocol_slot_id;
    nsecs_t mTimeStamp;
    bool mAddAtFront;
};

#endif
