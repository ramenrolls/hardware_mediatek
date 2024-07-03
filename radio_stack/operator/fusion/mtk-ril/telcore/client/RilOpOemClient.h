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

#ifndef __RIL_OP_OEM_CLIENT_H__
#define __RIL_OP_OEM_CLIENT_H__

#include "client/RilOemClient.h"

class RilOpOemClient : public RilOemClient {
  public:
    RilOpOemClient(int identity, char* socketName);
    virtual ~RilOpOemClient();
    virtual void requestComplete(RIL_Token token, RIL_Errno e, void* response, size_t responselen);

  protected:
    virtual int handleSpecialRequestWithArgs(int argCount, char** args);

  private:
    void executeRemoteSimUnlockCommand(int slotId, char* command, char* data);

  private:
    static int mRsuFd;
};
#endif
