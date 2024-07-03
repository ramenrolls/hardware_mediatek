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

#ifndef RIL_IMSCONFIGDATA_H
#define RIL_IMSCONFIGDATA_H

#include "RtcImsConfigDef.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

class ImsConfigDataHelper {
  private:
    ImsConfigDataHelper();
    virtual ~ImsConfigDataHelper();
    static std::map<int, ConfigSettings> sConfigData;

  public:
    static std::vector<std::shared_ptr<ConfigValue>> getConfigData(int operatorId);
    static int getConfigDataType(int configId);
    static bool isProvisionToModem(int configId);
    static std::string getConfigProvisionStr(int configId);
    static int getConfigId(std::string provisionStr);
};

#endif  // RIL_IMSCONFIGDATA_H
