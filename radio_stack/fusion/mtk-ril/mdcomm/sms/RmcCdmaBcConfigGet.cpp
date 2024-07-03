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
#include "RmcCdmaBcConfigGet.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaGetBcConfigReq, RmcCdmaGetBcConfigRsp,
                                RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG);

/*****************************************************************************
 * Class RmcCdmaGetBcConfigReq
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaGetBcConfigReq);
RmcCdmaGetBcConfigReq::RmcCdmaGetBcConfigReq(void* data, int length)
    : RmcMultiAtReq(data, length), m_mode(-1) {}

RmcCdmaGetBcConfigReq::~RmcCdmaGetBcConfigReq() {}

RmcAtSendInfo* RmcCdmaGetBcConfigReq::onGetFirstAtInfo(RfxBaseHandler* h) {
    RFX_UNUSED(h);
    String8 cmd("AT+ECSCB?");
    String8 responsePrefix("+ECSCB:");
    return new RmcSingleLineAtSendInfo(cmd, responsePrefix);
}

RmcAtSendInfo* RmcCdmaGetBcConfigReq::onGetNextAtInfo(const String8& cmd, RfxBaseHandler* h) {
    RFX_UNUSED(h);
    if (cmd == String8("AT+ECSCB?")) {
        String8 cmd("AT+ECSCBCHA?");
        String8 responsePrefix("+ECSCBCHA:");
        return new RmcSingleLineAtSendInfo(cmd, responsePrefix);
    } else if (cmd == String8("AT+ECSCBCHA?")) {
        String8 cmd("AT+ECSCBLAN?");
        String8 responsePrefix("+ECSCBLAN:");
        return new RmcSingleLineAtSendInfo(cmd, responsePrefix);
    }
    return NULL;
}

bool RmcCdmaGetBcConfigReq::onHandleIntermediates(const String8& cmd, RfxAtLine* line,
                                                  RfxBaseHandler* h) {
    RFX_UNUSED(h);
    int err;
    int mode = line->atTokNextint(&err);
    if (err < 0 || mode < 0 || mode > 1) {
        setError(RIL_E_SYSTEM_ERR);
        return false;
    }

    if (cmd == String8("AT+ECSCB?")) {
        m_mode = mode;
        if (mode == 0) {
            return false;
        }
    } else if (cmd == String8("AT+ECSCBCHA?") || cmd == String8("AT+ECSCBLAN?")) {
        String8 rangeStr;
        Range ranges[MAX_RANGE];
        int num = 0;
        if (mode == 1) {
            char* str = line->atTokNextstr(&err);
            if (err < 0) {
                setError(RIL_E_SYSTEM_ERR);
                return false;
            }
            rangeStr.setTo(str);
            num = RmcCdmaBcRangeParser::getRangeFromModem((char*)rangeStr.string(), ranges);
        }
        if (cmd == String8("AT+ECSCBCHA?")) {
            for (int i = 0; i < num; i++) {
                m_channels.push(ranges[i]);
            }
        } else {
            for (int i = 0; i < num; i++) {
                m_languages.push(ranges[i]);
            }
        }
    }
    return true;
}

/*****************************************************************************
 * Class RmcCdmaGetBcConfigRsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaGetBcConfigRsp);
RmcCdmaGetBcConfigRsp::RmcCdmaGetBcConfigRsp(void* data, int length) : RmcVoidRsp(data, length) {
    if (data != NULL) {
        RIL_CDMA_BroadcastSmsConfigInfo** p_cur = (RIL_CDMA_BroadcastSmsConfigInfo**)data;

        int num = length / sizeof(RIL_CDMA_BroadcastSmsConfigInfo*);
        for (int i = 0; i < num; i++) {
            m_infos.push(*(p_cur[i]));
        }
        Vector<RIL_CDMA_BroadcastSmsConfigInfo>::iterator it;
        int i = 0;
        for (it = m_infos.begin(); it != m_infos.end(); it++, i++) {
            m_pInfos.push(it);
        }
        m_data = (void*)m_pInfos.array();
        m_length = sizeof(RIL_CDMA_BroadcastSmsConfigInfo*) * m_pInfos.size();
    }
}

RmcCdmaGetBcConfigRsp::RmcCdmaGetBcConfigRsp(Vector<RIL_CDMA_BroadcastSmsConfigInfo> infos,
                                             RIL_Errno e)
    : RmcVoidRsp(e), m_infos(infos) {
    if (e == RIL_E_SUCCESS) {
        Vector<RIL_CDMA_BroadcastSmsConfigInfo>::iterator it;
        int i = 0;
        for (it = m_infos.begin(); it != m_infos.end(); it++, i++) {
            m_pInfos.push(it);
        }
        m_data = (void*)m_pInfos.array();
        m_length = sizeof(RIL_CDMA_BroadcastSmsConfigInfo*) * m_pInfos.size();
    }
}

RmcCdmaGetBcConfigRsp::~RmcCdmaGetBcConfigRsp() {}

/*****************************************************************************
 * Class RmcCdmaBcGetConfigHdlr
 *****************************************************************************/
RmcBaseRspData* RmcCdmaBcGetConfigHdlr::onGetRspData(RmcBaseReqData* req) {
    RmcCdmaGetBcConfigReq* bcConfigReq = (RmcCdmaGetBcConfigReq*)((void*)req);
    RIL_Errno e = RIL_E_SUCCESS;
    Vector<RIL_CDMA_BroadcastSmsConfigInfo> infos;
    if (bcConfigReq->isBcActivate()) {
        const Vector<Range>& channels = bcConfigReq->getChannels();
        const Vector<Range>& languages = bcConfigReq->getLanguages();
        if ((channels.size() + languages.size()) == 0) {
            RIL_CDMA_BroadcastSmsConfigInfo info;
            memset(&info, 0, sizeof(info));
            infos.push(info);
        } else {
            Vector<Range>::const_iterator it;
            for (it = channels.begin(); it != channels.end(); it++) {
                for (int i = it->start; i <= it->end; i++) {
                    RIL_CDMA_BroadcastSmsConfigInfo info;
                    info.service_category = i;
                    info.language = 0;
                    info.selected = 1;
                    infos.push(info);
                }
            }
            for (it = languages.begin(); it != languages.end(); it++) {
                for (int i = it->start; i <= it->end; i++) {
                    RIL_CDMA_BroadcastSmsConfigInfo info;
                    info.service_category = 0;
                    info.language = i;
                    info.selected = 1;
                    infos.push(info);
                }
            }
        }
    } else {
        RIL_CDMA_BroadcastSmsConfigInfo info;
        memset(&info, 0, sizeof(info));
        infos.push(info);
    }
    return new RmcCdmaGetBcConfigRsp(infos, e);
}
