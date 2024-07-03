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

#ifndef __RFX_SOCKET_STATE_MESSAGE_H__
#define __RFX_SOCKET_STATE_MESSAGE_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "Rfx.h"
#include "RfxObject.h"

/*****************************************************************************
 * Class RfxSocketStateMessage
 *****************************************************************************/
class RfxSocketStateMessage : public virtual RefBase {
  public:
    RfxSocketStateMessage(RILD_RadioTechnology_Group groupId, bool isConnected, int slotId,
                          int socFd)
        : mGroupId(groupId), mIsConnected(isConnected), mSlotId(slotId), mSocketFd(socFd) {}

    RILD_RadioTechnology_Group getGroupId() { return mGroupId; }
    bool getIsConnected() { return mIsConnected; }
    int getSlotId() { return mSlotId; }
    int getSocketFd() { return mSocketFd; }

  private:
    RILD_RadioTechnology_Group mGroupId;
    bool mIsConnected;
    int mSlotId;
    int mSocketFd;
};

#endif /* __RFX_SOCKET_STATE_MESSAGE_H__ */
