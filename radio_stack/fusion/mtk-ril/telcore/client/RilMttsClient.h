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

#ifndef __RIL_MTTSCLIENT_H__
#define __RIL_MTTSCLIENT_H__

#include "RilClient.h"

using namespace android;

class RilMttsClient : public RilClient {
  public:
    RilMttsClient(int identity, char* socketName);

    // Override
    void processCommands(void* buffer, size_t buflen, int clientId);
    void handleUnsolicited(int slotId, int unsolResponse, void* data, size_t datalen,
                           UrcDispatchRule rule);
    bool isNeedToCache(int unsolResponse);
};

#endif /* __RIL_MTTSCLIENT_H__ */
