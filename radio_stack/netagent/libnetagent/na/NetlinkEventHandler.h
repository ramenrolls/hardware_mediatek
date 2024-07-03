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

#ifndef __NETLINK_EVENT_HANDLER_H__
#define __NETLINK_EVENT_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <netinet/in.h>
#include <cstdlib>
#include <sysutils/NetlinkEvent.h>
#include <sysutils/NetlinkListener.h>

#include "NetAgentService.h"

#include "utils/String8.h"
#include "utils/Vector.h"

using ::android::String8;
using ::android::Vector;
/*****************************************************************************
 * Class NetlinkEventHandler
 *****************************************************************************/
typedef enum {
    ACTION_UNKNOWN = 0,
    ACTION_ADDR_UPDATED = 1,
    ACTION_ADDR_REMOVED = 2,
} ACTION;

typedef struct {
    int interfaceId;
    int ipv6PrefixLength;
    char address[INET6_ADDRSTRLEN];
    NA_CMD cmd;
    ACTION action;
} NetlinkEventReqInfo;

typedef enum {
    NETLINK_RET_GENERIC_FAILURE = 0,
    NETLINK_RET_SUCCESS = 1,
    NETLINK_RET_REQ_INFO_CHANGED = 2,
    NETLINK_RET_REQ_INFO_NO_CHANGED = 3,
    NETLINK_RET_REQ_INFO_NEW = 4
} NETLINK_RET_STATUS;

class NetlinkEventHandler : public NetlinkListener {
  public:
    NetlinkEventHandler(NetAgentService* na, int listenerSocket, int format);
    virtual ~NetlinkEventHandler();

    int start(void);
    int stop(void);
    int getCommandType(void* obj, NA_CMD* cmd);
    int getAddress(void* obj, char* address);
    int getInterfaceId(void* obj, unsigned int* interfaceId);
    int getIpv6PrefixLength(void* obj, int* ipv6PrefixLength);
    int getAction(void* obj, ACTION* action);
    void removeLastReqInfo(int interfaceId);
    int setLastReqInfo(void* obj);
    int hasLastReqInfoChanged(void* obj);
    void freeNetlinkEventObj(void* obj);
    void freeNetlinkEventObj(NetlinkEventReqInfo* pReqInfo);
    static int stringToBinaryBase(char* str, int base, int* err);
    static const char* actionToString(ACTION action);
    static String8 reqInfoToString(NetlinkEventReqInfo* pReqInfo);
    int getTransIntfId(int interfaceId);

  protected:
    void onEvent(NetlinkEvent* evt);
    void enqueueReqInfo(NA_CMD cmd, char* address, int ipv6PrefixLength, int interfaceId,
                        ACTION action);

  private:
    NetAgentService* m_pNetAgent;
    Vector<NetlinkEventReqInfo*>* m_pLastNetlinkEventReqInfo;
};
#endif /* __NETLINK_EVENT_HANDLER_H__ */
