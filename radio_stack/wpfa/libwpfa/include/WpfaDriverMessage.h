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

#ifndef WPFA_DRIVER_MESSAGE_H
#define WPFA_DRIVER_MESSAGE_H

#include "String8.h"
#include "RefBase.h"

#include <stdint.h>
#include <mtk_log.h>

#include "FilterStructure.h"

#include "WpfaDriverAcceptData.h"
#include "WpfaDriverBaseData.h"
#include "WpfaDriverRegFilterData.h"
#include "WpfaDriverDeRegFilterData.h"
#include "WpfaDriverVersionData.h"
#include "WpfaDriverULIpPkt.h"
#include "WpfaCcciDataHeaderEncoder.h"

using ::android::RefBase;
using ::android::sp;
using ::android::String8;

/*
 * =============================================================================
 *                     MACRO
 * =============================================================================
 */
#define CCCI_HEADER_SIZE (sizeof(ccci_msg_hdr_t))

#define CCCI_BODY_REG_FILTER_SIZE (sizeof(wifiproxy_m2a_reg_dl_filter_t))
#define CCCI_BODY_DEREG_FILTER_SIZE (sizeof(wifiproxy_m2a_dereg_dl_filter_t))
#define CCCI_BODY_REG_ACCEPT_SIZE (sizeof(wifiproxy_a2m_reg_reply_t))
#define CCCI_BODY_DEREG_ACCEPT_SIZE (sizeof(wifiproxy_a2m_dereg_reply_t))
#define CCCI_BODY_FILTER_VER_SIZE (sizeof(wifiproxy_ap_md_filter_ver_t))

/*
 * =============================================================================
 *                     typedef
 * =============================================================================
 */

/*
 * =============================================================================
 *                     class
 * =============================================================================
 */

class WpfaDriverMessage : public virtual RefBase {
  public:
    uint16_t getMsgId() const { return mMsgId; }

    uint16_t getTid() const { return mTid; }

    uint16_t getType() const { return mType; }

    uint16_t getParams() const { return mParams; }

    WpfaDriverBaseData* getData() const { return mData; }

    static sp<WpfaDriverMessage> obtainMessage(uint16_t msgId, uint16_t tId, uint16_t type,
                                               uint16_t params, const WpfaDriverBaseData& data);

    static sp<WpfaDriverMessage> obtainMessage(uint16_t msgId, uint16_t tId, uint16_t type,
                                               uint16_t params);

    static int getCcciMsgBodySize(uint16_t msgId);
    static WpfaDriverBaseData* copyData(uint16_t msgId, const WpfaDriverBaseData* data);

    String8 toString() const;

  protected:
  private:
    WpfaDriverMessage();
    virtual ~WpfaDriverMessage();

    uint16_t mMsgId;
    uint16_t mTid;
    uint16_t mType;
    uint16_t mParams;
    WpfaDriverBaseData* mData;
};

#endif /* end of WPFA_DRIVER_MESSAGE_H */
