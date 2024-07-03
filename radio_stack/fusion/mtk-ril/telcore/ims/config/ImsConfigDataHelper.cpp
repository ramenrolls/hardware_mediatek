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

#include "ImsConfigDataHelper.h"
#include "RtcImsConfigDef.h"
#include "telephony/mtk_ims_config.h"

using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

map<int, ConfigSettings> ImsConfigDataHelper::sConfigData = {
#include "ImsConfigData.h"
};

vector<shared_ptr<ConfigValue>> ImsConfigDataHelper::getConfigData(int operatorId) {
    vector<ConfigValue> opData;
    if (operatorId == Operator::OP_06) {
        opData = {
#include "op/Op06ImsConfigData.h"
        };
    } else if (operatorId == Operator::OP_08) {
        opData = {
#include "op/Op08ImsConfigData.h"
        };
    } else if (operatorId == Operator::OP_12) {
        opData = {
#include "op/Op12ImsConfigData.h"
        };
    } else if (operatorId == Operator::OP_236) {
        opData = {
#include "op/Op236ImsConfigData.h"
        };
    }

    vector<shared_ptr<ConfigValue>> configData;
    map<int, ConfigSettings>::iterator iter;
    iter = sConfigData.begin();
    for (int i = 0; i < (int)sConfigData.size(); i++, iter++) {
        shared_ptr<ConfigValue> data = make_shared<ConfigValue>();
        data->configId = iter->first;
        data->unit = IMS_CONFIG_UNIT_NONE;
        data->provsionValue = IMS_CONFIG_VALUE_NO_DEFAULT;
        for (int j = 0; j < (int)opData.size(); j++) {
            if (data->configId == opData[j].configId) {
                data->unit = opData[j].unit;
                data->provsionValue = opData[j].provsionValue;
            }
        }
        configData.push_back(data);
    }
    return configData;
}

int ImsConfigDataHelper::getConfigDataType(int configId) { return sConfigData[configId].mimeType; }

bool ImsConfigDataHelper::isProvisionToModem(int configId) {
    return !sConfigData[configId].provsionStr.empty();
}

string ImsConfigDataHelper::getConfigProvisionStr(int configId) {
    return sConfigData[configId].provsionStr;
}

int ImsConfigDataHelper::getConfigId(string provisionStr) {
    map<int, ConfigSettings>::iterator iter;
    iter = sConfigData.begin();
    for (int i = 0; i < (int)sConfigData.size(); i++, iter++) {
        if ((iter->second).provsionStr == provisionStr) {
            return iter->first;
        }
    }
    return -1;
};
