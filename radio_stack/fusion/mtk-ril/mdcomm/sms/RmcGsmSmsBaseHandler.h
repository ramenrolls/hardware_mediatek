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

#ifndef __RMC_GSM_SMS_BASE_HANDLER_H__
#define __RMC_GSM_SMS_BASE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcGsmSmsDefs.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcGsmSmsBaseHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcGsmSmsBaseHandler);

  public:
    RmcGsmSmsBaseHandler(int slot_id, int channel_id);

  protected:
    // Utility functions
    int smsPackPdu(const char* smsc, const char* tpdu, char* pdu, bool check);
    bool smsFoCheck(int fo);
    bool smsPidCheck(int pid);
    bool smsDcsCheck(int dcs);
    void smsDecodeDcs(int dcs, SMS_ENCODING_ENUM* alphabet_type, SMS_MESSAGE_CLASS_ENUM* msg_class,
                      bool* is_compress);
    bool smsIsLenIn8bit(int dcs);
    unsigned int smsMsgLenInOctet(int dcs, int len);
    int smsHexCharToDecInt(char* hex, int length);
    bool smsSubmitPduCheck(int pdu_len, char* pdu_ptr, TPDU_ERROR_CAUSE_ENUM* error_cause);
    bool smsDeliverPduCheck(int pdu_len, char* pdu, TPDU_ERROR_CAUSE_ENUM* error_cause);
    int atGetCmsError(const sp<RfxAtResponse> p_response);
    void showCurrIncomingSmsType();
    int smsCheckReceivedPdu(int length, char* pdu, bool check);
    int hexCharToInt(char c);
    void hexStringToBytes(const char* in, int inLength, char* out, int outLength);

  private:
    void setTag(String8 tag) { mTag = tag; }

  private:
    String8 mTag;
};
#endif /* __RMC_GSM_SMS_BASE_HANDLER_H__ */
