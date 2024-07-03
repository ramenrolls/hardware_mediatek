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

#ifndef SAP_SOCKET_MANAGER_H_
#define SAP_SOCKET_MANAGER_H_

#include <telephony/mtk_ril.h>

static pthread_mutex_t sap_socket_gsm_mutex[MAX_SIM_COUNT];
static pthread_mutex_t sap_socket_c2k_mutex[MAX_SIM_COUNT];
static pthread_mutex_t sap_socket_bt_mutex[MAX_SIM_COUNT];
static int socket_sap_gsm_fds[MAX_SIM_COUNT];
static int socket_sap_c2k_fds[MAX_SIM_COUNT];
static int socket_sap_bt_fds[MAX_SIM_COUNT];
const char PROPERTY_RIL_FULL_UICC_TYPE[MAX_SIM_COUNT][35] = {
        "vendor.gsm.ril.fulluicctype",
        "vendor.gsm.ril.fulluicctype.2",
        "vendor.gsm.ril.fulluicctype.3",
        "vendor.gsm.ril.fulluicctype.4",
};

class SapSocketManager {
  public:
    SapSocketManager();
    static bool setSapSocket(RIL_SOCKET_ID socketId, RILD_RadioTechnology_Group group, int fd);
    static bool sendSapMessageToRild(void* data, int datalen, RIL_SOCKET_ID socketId);

  private:
    static RILD_RadioTechnology_Group choiceDestViaCurrCardType(int slotId);
};
#endif
