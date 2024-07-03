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

#ifndef __RFX_DISPATCH_THREAD_H__
#define __RFX_DISPATCH_THREAD_H__

#include <utils/threads.h>
#include "RfxObject.h"
#include "RfxMessage.h"
#include "RfxDispatchQueue.h"
#include "RfxMainThread.h"
#include "RfxRilAdapter.h"
#include "RfxLog.h"
#include <utils/Parcel.h>
#include "RfxMclMessage.h"
#include <vendor/mediatek/ims/radio_stack/platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "RilClientConstants.h"

using android::Parcel;
using ::android::sp;
using ::android::Thread;

typedef void* (*PthreadPtr)(void*);

extern "C" void* rfx_process_request_messages_loop(void* arg);
extern "C" void* rfx_process_response_messages_loop(void* arg);
extern "C" void* rfx_process_urc_messages_loop(void* arg);
extern "C" void* rfx_process_status_sync_messages_loop(void* arg);

typedef struct {
    int requestNumber;
    void (*dispatchFunction)(RIL_RequestFunc onRequest, Parcel& p, struct RfxRequestInfo* pRI);
    int (*responseFunction)(Parcel& p, void* response, size_t responselen);
} RfxCommandInfo;

typedef struct RfxRequestInfo {
    int32_t token;  // this is not RIL_Token
    RfxCommandInfo* pCI;
    struct RfxRequestInfo* p_next;
    char cancelled;
    char local;  // responses to local commands do not go back to command process
    RIL_SOCKET_ID socket_id;
    ClientId clientId;
    int request;
} RfxRequestInfo;

typedef struct RfxSapSocketRequest {
    int token;
    MsgHeader* curr;
    struct RfxSapSocketRequest* p_next;
    RIL_SOCKET_ID socketId;
} RfxSapSocketRequest;

typedef struct {
    int requestNumber;
    int (*responseFunction)(Parcel& p, void* response, size_t responselen);
    // WakeType wakeType;
} RfxUnsolResponseInfo;

typedef struct MessageObj {
    sp<RfxMessage> msg;
    struct MessageObj* p_next;
} MessageObj;

static Dispatch_queue<MessageObj> dispatchRequestQueue;
static Dispatch_queue<MessageObj> dispatchResponseQueue;
static Dispatch_queue<MessageObj> dispatchUrcQueue;
static Dispatch_queue<MessageObj> dispatchStatusSyncQueue;
static Dispatch_queue<MessageObj> pendingQueue;
MessageObj* createMessageObj(const sp<RfxMessage>& message);

class RfxDispatchThread : public Thread {
  private:
    RfxDispatchThread();

    virtual ~RfxDispatchThread();

  public:
    static RfxDispatchThread* init();

    void enqueueRequestMessage(int request, void* data, size_t datalen, RIL_Token t,
                               RIL_SOCKET_ID socket_id, int clientId = -1);
    void enqueueSapRequestMessage(int request, void* data, size_t datalen, RIL_Token t,
                                  RIL_SOCKET_ID socket_id);
    static void enqueueResponseMessage(const sp<RfxMclMessage>& msg);
    static void enqueueSapResponseMessage(const sp<RfxMclMessage>& msg);
    static void enqueueUrcMessage(const sp<RfxMclMessage>& msg);
    static void enqueueSapUrcMessage(const sp<RfxMclMessage>& msg);
    static void enqueueStatusSyncMessage(const sp<RfxMclMessage>& msg);
    static void addMessageToPendingQueue(const sp<RfxMessage>& message);
    static void clearPendingQueue();
    static void removeMessageFromPendingQueue(int token);
    static void updateConnectionState(RIL_SOCKET_ID socket_id, int isConnected);

  private:
    virtual bool threadLoop();

    void processRequestMessageLooper();
    void processResponseMessageLooper();
    void processUrcMessageLooper();
    void processStatusSyncMessageLooper();

    friend void* ::rfx_process_request_messages_loop(void* arg);
    friend void* ::rfx_process_response_messages_loop(void* arg);
    friend void* ::rfx_process_urc_messages_loop(void* arg);
    friend void* ::rfx_process_status_sync_messages_loop(void* arg);

  private:
    static RfxDispatchThread* s_self;

    pthread_t requestThreadId;
    pthread_t responseThreadId;
    pthread_t urcThreadId;
    pthread_t statusSyncThreadId;
};
#endif /* __RFX_DISPATCH_THREAD_H__ */
