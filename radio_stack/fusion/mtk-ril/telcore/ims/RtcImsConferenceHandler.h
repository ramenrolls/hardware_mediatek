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

#ifndef __RFX_IMS_CONFERENCE_HANDLER_H__
#define __RFX_IMS_CONFERENCE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <string.h>
#include <string>
#include <vector>

#include "RtcImsConferenceCallMessageHandler.h"

#include "RfxSignal.h"
#include "RfxTimer.h"
#include "utils/RefBase.h"
#include "utils/StrongPointer.h"
#include "utils/Looper.h"

using ::android::Looper;
using ::android::MessageHandler;
using ::android::RefBase;
using ::android::sp;

using namespace std;
/*****************************************************************************
 * Class RtcImsConferenceHandler
 *****************************************************************************/

class RtcImsConferenceHandler {
  public:
    RtcImsConferenceHandler(int slot);
    ~RtcImsConferenceHandler();
    void handleImsConfCallMessage(const sp<RfxMessage>& message);
    void startConference();
    void closeConference();
    void firstMerge(string callId_1, string callId_2, string num_1, string num_2);
    void tryAddParticipant(string addr);
    void tryOneKeyAddParticipant(string addr);
    void tryRemoveParticipant(string addr);
    bool modifyParticipantComplete();
    void modifyParticipantFailed();
    void addFirstMergeParticipant(string callId);
    void tryOneKeyAddLocalList(vector<string> users);
    void updateConferenceStateWithLocalCache();
    vector<sp<ConferenceCallUser>> getConfParticipantsInfo();
    string getConfParticipantUri(string addr, bool isRetry);
    static string getUserNameFromSipTelUriString(string uriString);
    static vector<string> splitString(string str, string c);
    static string normalizeNumberFromCLIR(string number);
    static string concatData(int isFirst, string origData, string appendData);
    static string recoverDataFromAsciiTag(string data);
    static string replaceAll(string& str, const string& old_value, const string& new_value);
    static string encodeSpecialChars(string number);

  private:
    RtcImsConferenceCallMessageHandler* parseXmlPackage(string data);
    void restoreParticipantsAddressByLocalCache();
    void restoreUnknowParticipants(vector<string> restoreUnknowCandidates);
    void setupHost(RtcImsConferenceCallMessageHandler* xmlData);
    void fullUpdateParticipants(vector<sp<ConferenceCallUser>> user);
    void partialUpdateParticipants(vector<sp<ConferenceCallUser>> user);
    void updateLocalCache(int cepState);
    void notifyConfStateUpdate();
    bool isSelfAddress(string address);
    bool updateParticipants(sp<ConferenceCallUser> oldParticipant, string to);
    void showCacheAndXmlData(string callerName);
    bool isContainParticipant(vector<string> participants, string participant);
    void removeParticipant(vector<string>& participants, string participant, bool compareLoosely);
    string getPairedAddress(const string& addr);
    bool checkCarrierConfig(const RfxStatusKeyEnum key);
    string getPairedRestoredAddress(string userAddr);
    void dumpParticipantsAddrMap();
    bool isEmptyConference();
    void insertParticipantsAddrMap(string key, string value);

  private:
    static const string TAG_NEXT_LINE;
    static const string TAG_RETURN;
    static const string TAG_DOUBLE_QUOTE;

    int m_slot_id;
    bool mIsCepNotified;
    int mConfCallId;
    int mCepVersion;
    string mCepData;
    string mHostAddr;
    string mAddingParticipant;
    string mRemovingParticipant;
    bool mRestoreImsConferenceParticipant = false;
    bool mIsConferenceActive = false;
    bool mHaveUpdatedConferenceWithMember = false;
    int mConnectedCount = 0;

    vector<sp<ConferenceCallUser>> mParticipants;
    vector<sp<ConferenceCallUser>> mConfParticipants;
    vector<sp<ConferenceCallUser>> mUnknownParticipants;
    vector<string> mLocalParticipants;
    map<string, string> mParticipantsAddrMap;
    map<string, string> mFirstMergeParticipants;
    vector<string> mOneKeyAddingParticipants;
    vector<sp<ConferenceCallUser>> mLastConfParticipants;
};

#endif /* __RFX_IMS_CONFERENCE_HANDLER_H__ */
