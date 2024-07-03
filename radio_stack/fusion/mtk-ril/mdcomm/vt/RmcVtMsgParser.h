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

#ifndef __RMC_VT_MSG_PARSER_H__
#define __RMC_VT_MSG_PARSER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "imsvt_imcb_sap.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

const int MSG_ID_WRAP_IMSVT_MD_BEGIN = MSG_ID_WRAP_IMSVT_IMCB_BEGIN + 1000;
const int MSG_ID_WRAP_IMSVT_MD_ANBR_CONFIG_UPDATE_IND = MSG_ID_WRAP_IMSVT_MD_BEGIN + 1;
const int MSG_ID_WRAP_IMSVT_MD_ANBR_REPORT_REQ = MSG_ID_WRAP_IMSVT_MD_BEGIN + 2;
const int MSG_ID_WRAP_IMSVT_MD_INTER_RAT_STATUS_IND = MSG_ID_WRAP_IMSVT_MD_BEGIN + 3;

typedef struct {
    int config;
    int ebi;
    int is_ul;
    int bitrate;
    int bearer_id;
    int pdu_session_id;
    int ext_param;
} vt_srv_anbr_struct;

typedef struct {
    int call_id;
    int sim_slot_id;
    vt_srv_anbr_struct anbr_config;
} vt_srv_ril_msg_anbr_struct;

#define VT_IMCB_CAPIND vt_srv_imcb_msg_get_cap_ind_struct
#define VT_IMCB_CAP vt_srv_imcb_msg_get_cap_rsp_struct
#define VT_IMCB_BW vt_srv_imcb_msg_mod_bw_req_t

#define VT_IMCB_CONFIG vt_srv_imcb_msg_param_t
#define VT_IMCB_INIT vt_srv_imcb_msg_config_init_ind_struct
#define VT_IMCB_UPD vt_srv_imcb_msg_config_update_ind_struct
#define VT_IMCB_DEINIT vt_srv_imcb_msg_config_deinit_ind_struct

#define VT_IMCB_REQ vt_srv_imcb_msg_session_modify_req_struct
#define VT_IMCB_IND vt_srv_imcb_msg_session_modify_ind_struct
#define VT_IMCB_RSP vt_srv_imcb_msg_session_modify_rsp_struct
#define VT_IMCB_CNF vt_srv_imcb_msg_session_modify_cnf_struct

#define VT_IMCB_CANCEL_REQ vt_srv_imcb_msg_session_modify_cancel_req_struct
#define VT_IMCB_CANCEL_IND vt_srv_imcb_msg_session_modify_cancel_ind_struct
#define VT_IMCB_CANCEL_CNF vt_srv_imcb_msg_session_modify_cancel_cnf_struct

#define VT_IMCB_HOSTART vt_srv_imcb_handover_start_ind_struct
#define VT_IMCB_HOSTOP vt_srv_imcb_handover_stop_ind_struct
#define VT_IMCB_PDN_HOBEGIN vt_srv_imcb_pdn_handover_begin_ind_struct
#define VT_IMCB_PDN_HOEND vt_srv_imcb_pdn_handover_end_ind_struct

#define VT_ANBR_REQ vt_srv_ril_msg_anbr_struct
#define VT_ANBR_CONFIG vt_srv_anbr_struct

class RmcVtMsgParser {
  private:
    RmcVtMsgParser();

  public:
    virtual ~RmcVtMsgParser();

    static RmcVtMsgParser* getInstance();
    void startSession(char* input, int type);
    void stopSession();
    int getSlot();

    static RmcVtMsgParser* s_RmcVtMsgParser;

  private:
    int mType;

    VT_IMCB_CAPIND* mCapIndStruct;
    VT_IMCB_CAP* mCapRspStruct;
    VT_IMCB_BW* mBwReqStruct;

    VT_IMCB_INIT* mInitIndStruct;
    VT_IMCB_UPD* mUpdateIndStruct;
    VT_IMCB_DEINIT* mDeinitIndStruct;

    VT_IMCB_REQ* mSessionModifyReqStruct;
    VT_IMCB_IND* mSessionModifyIndStruct;
    VT_IMCB_RSP* mSessionModifyRspStruct;
    VT_IMCB_CNF* mSessionModifyCnfStruct;

    VT_IMCB_CANCEL_REQ* mSessionModifyCancelReqStruct;
    VT_IMCB_CANCEL_IND* mSessionModifyCancelIndStruct;
    VT_IMCB_CANCEL_CNF* mSessionModifyCancelCnfStruct;

    VT_IMCB_HOSTART* mHOStartIndStruct;
    VT_IMCB_HOSTOP* mHOStopIndStruct;
    VT_IMCB_PDN_HOBEGIN* mHOPDNBeginIndStruct;
    VT_IMCB_PDN_HOEND* mHOPDNEndIndStruct;
    VT_ANBR_REQ* mANBRRequestStruct;
};

#endif /* __RMC_MSIM_REQ_HANDLER_H__ */
