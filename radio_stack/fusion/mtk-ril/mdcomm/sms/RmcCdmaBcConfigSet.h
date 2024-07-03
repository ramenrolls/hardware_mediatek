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

#ifndef __RMC_CDMA_BC_CONFIG_SET_H__
#define __RMC_CDMA_BC_CONFIG_SET_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"
#include "RmcCdmaBcRangeParser.h"

/*****************************************************************************
 * Class RmcCdmaSetBcConfigReq
 *****************************************************************************/
class RmcCdmaSetBcConfigReq : public RmcMultiAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaSetBcConfigReq);

    // Override
  protected:
    virtual RmcAtSendInfo* onGetFirstAtInfo(RfxBaseHandler* h);
    virtual RmcAtSendInfo* onGetNextAtInfo(const String8& cmd, RfxBaseHandler* h);
    virtual bool onHandleIntermediates(const String8& cmd, RfxAtLine* line, RfxBaseHandler* h);

    // Implementation
  private:
    Vector<RIL_CDMA_BroadcastSmsConfigInfo> m_infos;
    Vector<RIL_CDMA_BroadcastSmsConfigInfo*> m_pInfos;
    SortedVector<int> m_categorys;
    SortedVector<int> m_languages;
    Vector<Range> m_channels;
    unsigned int m_channel;
    Vector<Range> m_lans;
    unsigned int m_lan;
    int m_mode;
    Vector<Range> m_channels_md;
    Vector<Range> m_languages_md;
    unsigned int m_channel_md;
    unsigned int m_lans_md;

    // Implementation
  private:
    bool isSelected() { return (m_infos[0].selected == 1); }

    bool sortCategoryAndLanguage();
};

#endif /* __RMC_CDMA_BC_CONFIG_SET_H__ */
