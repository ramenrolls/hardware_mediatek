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

#ifndef __RFX_CONTROLLER_H__
#define __RFX_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <sys/types.h>
#include "utils/String8.h"
#include "RfxDefs.h"
#include "RfxMessage.h"
#include "RfxObject.h"
#include "RfxStatusManager.h"
#include "RfxTimer.h"
#include "RfxSignal.h"
#include "RfxMessageId.h"

using ::android::String8;
using ::android::Vector;

typedef RfxCallback1<const sp<RfxMessage>&> RfxWaitResponseTimedOutCallback;

typedef enum {
    RESPONSE_STATUS_NO_MATCH_AND_SAVE,
    RESPONSE_STATUS_HAVE_MATCHED,
    RESPONSE_STATUS_HAVE_MATCHED_WITH_C_CHANGED,
    RESPONSE_STATUS_ALREADY_SAVED,
    RESPONSE_STATUS_HAVE_BEEN_DELETED,
    RESPONSE_STATUS_INVALID
} ResponseStatus;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RfxController;

class RfxWaitReponseTimerHelper : public RfxObject {
    RFX_DECLARE_CLASS(RfxWaitReponseTimerHelper);

  public:
    RfxWaitReponseTimerHelper() {}

    RfxWaitReponseTimerHelper(const sp<RfxMessage>& message,
                              const RfxCallback1<const sp<RfxMessage>&>& callback,
                              RfxController* controller)
        : m_reponse_msg(message), m_callback1(callback), m_controller(controller) {}

  public:
    void onTimer();

  public:
    sp<RfxMessage> m_reponse_msg;
    RfxCallback1<const sp<RfxMessage>&> m_callback1;
    wp<RfxController> m_controller;
};

class RfxWaitReponseEntry {
  public:
    RfxWaitReponseEntry() : m_reponse_msg(NULL), m_timer_handle(NULL), m_entry_helper(NULL) {}

    RfxWaitReponseEntry(const sp<RfxMessage>& message)
        : m_reponse_msg(message), m_timer_handle(NULL), m_entry_helper(NULL) {}

    RfxWaitReponseEntry(const sp<RfxMessage>& message, const TimerHandle& timerHandle,
                        const wp<RfxWaitReponseTimerHelper>& entryHelper)
        : m_reponse_msg(message), m_timer_handle(timerHandle), m_entry_helper(entryHelper) {}

    ~RfxWaitReponseEntry() {}

  public:
    void deleteEntryHelper() {
        if (m_entry_helper != NULL) {
            RfxWaitReponseTimerHelper* helper = m_entry_helper.promote().get();
            if (helper != NULL) {
                RFX_OBJ_CLOSE(helper);
            }
        }
    }

  public:
    sp<RfxMessage> m_reponse_msg;
    TimerHandle m_timer_handle;
    wp<RfxWaitReponseTimerHelper> m_entry_helper;
};

/*
 * Class RfxController,
 *  prototype of base controller class, define the virtual
 *  functions which sub-class need to override, implement
 *  the default behaviors of controllers
 */
class RfxController : public RfxObject {
    RFX_DECLARE_CLASS(RfxController);
    // Constructor / Destructor
  public:
    // Constructor
    RfxController();

    virtual ~RfxController();

  public:
    enum HANDLER_PRIORITY { NORMAL, DEFAULT = NORMAL, MEDIUM, HIGH, HIGHEST };

    // Overridable
  protected:
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);

    virtual bool onHandleRequest(const sp<RfxMessage>& message);

    virtual bool onHandleUrc(const sp<RfxMessage>& message);

    virtual bool onHandleResponse(const sp<RfxMessage>& message);

    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

    virtual bool onCheckIfRemoveSuspendedMessage(const sp<RfxMessage>& message);

    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

    virtual bool onHandleAtciRequest(const sp<RfxMessage>& message);

    virtual bool onHandleAtciResponse(const sp<RfxMessage>& message);

    // Methods
  public:
    RfxController* getSlotRoot(int slot_id) const;

    RfxController* getSlotRoot() const;

    RfxController* findController(int slot_id, const RfxClassInfo* class_info);

    RfxController* findController(const RfxClassInfo* class_info);

    void requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol = false);
    void requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol, nsecs_t nsec);

    void requestAckToRilj(const sp<RfxMessage>& message);

    bool responseToRilj(const sp<RfxMessage>& message);

    bool responseToBT(const sp<RfxMessage>& message);

    void registerToHandleRequest(const int* request_id_list, size_t length,
                                 HANDLER_PRIORITY priority = DEFAULT);

    void unregisterToHandleRequest(const int* request_id_list, size_t length,
                                   HANDLER_PRIORITY priority = DEFAULT);

    void registerToHandleUrc(const int* urc_id_list, size_t length);

    void unregisterToHandleUrc(const int* urc_id_list, size_t length);

    void registerRequestToCsRild(const int* request_id_list, size_t length);

    void unregisterRequestToCsRild(const int* request_id_list, size_t length);

    void registerRequestToPsRild(const int* request_id_list, size_t length);

    void unregisterRequestToPsRild(const int* request_id_list, size_t length);

    void registerToHandleRequest(int slot_id, const int* request_id_list, size_t length,
                                 HANDLER_PRIORITY priority = DEFAULT);

    void unregisterToHandleRequest(int slot_id, const int* request_id_list, size_t length,
                                   HANDLER_PRIORITY priority = DEFAULT);

    void registerToHandleUrc(int slot_id, const int* urc_id_list, size_t length);

    void unregisterToHandleUrc(int slot_id, const int* urc_id_list, size_t length);

    void registerRequestToCsRild(int slot_id, const int* request_id_list, size_t length);

    void unregisterRequestToCsRild(int slot_id, const int* request_id_list, size_t length);

    void registerRequestToPsRild(int slot_id, const int* request_id_list, size_t length);

    void unregisterRequestToPsRild(int slot_id, const int* request_id_list, size_t length);

    int getSlotId() const;

    RfxStatusManager* getStatusManager(int slot_id) const;

    RfxStatusManager* getStatusManager() const;

    RfxStatusManager* getNonSlotScopeStatusManager() const;

    ResponseStatus preprocessResponse(const sp<RfxMessage>& message, sp<RfxMessage>& outResponse);

    ResponseStatus preprocessResponse(const sp<RfxMessage>& message, sp<RfxMessage>& outResponse,
                                      const RfxWaitResponseTimedOutCallback& callback,
                                      const nsecs_t time = 0);

    void logD(const char* tag, const char* fmt, ...) const;
    void logI(const char* tag, const char* fmt, ...) const;
    void logV(const char* tag, const char* fmt, ...) const;
    void logE(const char* tag, const char* fmt, ...) const;
    void logW(const char* tag, const char* fmt, ...) const;

    void addToBlackListForSwitchCDMASlot(const int* request_id_list, size_t length);
    sp<RfxMessage> processBlackListResponse(const sp<RfxMessage>& message);

    const char* idToString(int id);

    int getFeatureVersion(char* feature, int defaultVaule);

    int getFeatureVersion(char* feature);

    // Override
  public:
    virtual void onInit();

    virtual void onDeinit();

  public:
    // framework internal use
    virtual bool processMessage(const sp<RfxMessage>& message);

    bool checkIfResumeMessage(const sp<RfxMessage>& message);

    bool checkIfRemoveSuspendedMessage(const sp<RfxMessage>& message);

    bool checkIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff, int radioState);

    virtual bool processAtciMessage(const sp<RfxMessage>& message);

    virtual void clearMessages();

  private:
    void removeCachedResponse(int token);
    void setProcessedMsg(int id, nsecs_t timeStamp);
    int getProceesedMsgIndex(int id, nsecs_t timeStamp);
    String8 toString();
    void clearWaitResponseList();

  protected:
    int m_slot_id;

    int m_last_token;

  private:
    class ProcessedMsgInfo {
      public:
        ProcessedMsgInfo() : id(0), pTimeStamp(0) {}
        ProcessedMsgInfo(int _id, nsecs_t timeStamp) : id(_id), pTimeStamp(timeStamp) {}

      public:
        int id;
        nsecs_t pTimeStamp;
    };

  private:
    Vector<RfxWaitReponseEntry> m_wait_reponse_msg_list;
    Vector<ProcessedMsgInfo> m_processed_msg_list;
    Vector<int> mBlackListForSwitchCdmaSlot;

    friend RfxWaitReponseTimerHelper;
};

inline RfxController* RfxController::getSlotRoot() const { return getSlotRoot(m_slot_id); }

inline RfxController* RfxController::findController(const RfxClassInfo* class_info) {
    return findController(m_slot_id, class_info);
}

inline RfxStatusManager* RfxController::getStatusManager() const {
    return getStatusManager(m_slot_id);
}

inline RfxStatusManager* RfxController::getNonSlotScopeStatusManager() const {
    return getStatusManager(RFX_SLOT_ID_UNKNOWN);
}

inline void RfxController::registerToHandleRequest(const int* request_id_list, size_t length,
                                                   HANDLER_PRIORITY priority) {
    registerToHandleRequest(m_slot_id, request_id_list, length, priority);
}

inline void RfxController::unregisterToHandleRequest(const int* request_id_list, size_t length,
                                                     HANDLER_PRIORITY priority) {
    unregisterToHandleRequest(m_slot_id, request_id_list, length, priority);
}

inline void RfxController::registerToHandleUrc(const int* urc_id_list, size_t length) {
    registerToHandleUrc(m_slot_id, urc_id_list, length);
}

inline void RfxController::unregisterToHandleUrc(const int* urc_id_list, size_t length) {
    unregisterToHandleUrc(m_slot_id, urc_id_list, length);
}

inline void RfxController::registerRequestToCsRild(const int* request_id_list, size_t length) {
    registerRequestToCsRild(m_slot_id, request_id_list, length);
}

inline void RfxController::unregisterRequestToCsRild(const int* request_id_list, size_t length) {
    unregisterRequestToCsRild(m_slot_id, request_id_list, length);
}

inline void RfxController::registerRequestToPsRild(const int* request_id_list, size_t length) {
    registerRequestToPsRild(m_slot_id, request_id_list, length);
}

inline void RfxController::unregisterRequestToPsRild(const int* request_id_list, size_t length) {
    unregisterRequestToPsRild(m_slot_id, request_id_list, length);
}

#endif /* __RFX_CONTROLLER_H__ */
