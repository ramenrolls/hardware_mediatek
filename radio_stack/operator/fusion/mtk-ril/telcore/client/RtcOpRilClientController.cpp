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

#include "RtcOpRilClientController.h"
#include "RilOpOemClient.h"

#define RFX_LOG_TAG "RtcOpRilClientController"
#define NUM_ELEMS(a) (sizeof(a) / sizeof(a)[0])

RFX_IMPLEMENT_CLASS("RtcOpRilClientController", RtcOpRilClientController, RtcRilClientController);

const ClientInformation opClient[] = {
        {CLIENT_ID_OEM, (char*)"rild-oem", CLIENT_TYPE_OEM},
};

RtcOpRilClientController::RtcOpRilClientController() {}

RtcOpRilClientController::~RtcOpRilClientController() {}

void RtcOpRilClientController::initRilClient() {
    RtcRilClientController::initRilClient();
    logD(RFX_LOG_TAG, "init op client number: %zu", NUM_ELEMS(opClient));
    for (unsigned int i = 0; i < NUM_ELEMS(opClient); i++) {
        ClientInformation information = opClient[i];
        RilClient* client;
        switch (information.type) {
            case CLIENT_TYPE_OEM:
                client = new RilOpOemClient(information.identity, information.socketName);
                break;
            default:
                client = new RilClient(information.identity, information.socketName);
                break;
        }
        registerRilClient(client);
    }
}
