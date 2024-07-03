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

#ifndef __RFX_IMS_DIALOG_HANDLER_H__
#define __RFX_IMS_DIALOG_HANDLER_H__

#include <string>
#include <vector>
#include "RfxSignal.h"
#include "RfxDialog.h"

using ::android::sp;

/**
 * RtcImsDialogHandler, the class is used to handle DEP(Dialog event package) data,
 * the data is xml raw data that received from EIMSEVTPKG, and type is 2.
 */
class RtcImsDialogHandler {
  public:
    RtcImsDialogHandler(int slot);
    ~RtcImsDialogHandler();

    // Handle IMS Dialog event package raw data.
    void handleImsDialogMessage(const sp<RfxMessage>& message);
    string concatData(int isFirst, string origData, string appendData);
    string recoverDataFromAsciiTag(string data);
    string replaceAll(string& str, const string& old_value, const string& new_value);
    static void replace(char* str, int n, const char* newStr);
    int getPhoneId();

  private:
    static const string TAG_NEXT_LINE;
    static const string TAG_RETURN;
    static const string TAG_DOUBLE_QUOTE;
    int mSlot;
    string mDepData;
};

/**
 * DepMessageHandler, which is responsible for parsing Dialog event package.
 */
class DepMessageHandler : public DefaultHandler {
  public:
    static const string DIALOG_INFO;
    static const string DIALOG;
    static const string VERSION;
    static const string ID;
    static const string SA_EXCLUSIVE;
    static const string STATE;

    static const string LOCAL;
    static const string REMOTE;
    static const string IDENTITY;
    static const string TARGET;
    static const string URI;

    static const string DIRECTION;
    static const string INITIATOR;
    static const string RECEIVER;

    static const string PARAM;
    static const string PNAME;
    static const string PVAL;

    static const string MEDIA_ATTRIBUTES;
    static const string MEDIA_TYPE;
    static const string MEDIA_DIRECTION;
    static const string PORT0;

    static const string TRUE;

  public:
    DepMessageHandler();
    virtual ~DepMessageHandler();
    void startElement(string nodeName, string nodeValue, string attributeName,
                      string attributeValue);
    void endElement(string nodeName);

  private:
    vector<sp<RfxDialog>> mDialogInfo;
    sp<RfxDialog> mDialog;
    sp<RfxMediaAttribute> mMediaAttr;

    bool mDialogStart;
    bool mLocalStart;
    bool mMediaAttrStart;
    bool mRemoteStart;

    int mVersion;
    int mDepState;

  public:
    vector<sp<RfxDialog>> getDialogInfo();
    int getVersion() const;

    enum { DEP_STATE_UNKNOWN, DEP_STATE_FULL, DEP_STATE_PARTIAL };
};
#endif /* __RFX_IMS_DIALOG_HANDLER_H__ */
