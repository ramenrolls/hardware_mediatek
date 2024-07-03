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

#ifndef SS_UTIL
#define SS_UTIL

#include <vector>
#include <string>
#include "RfxDefs.h"
#include "SuppServDef.h"

using namespace std;

#define UNUSED(x) ((void)(x))

const char* InfoClassToMmiBSCodeString(AtInfoClassE infoClass);
int MmiBSCodeToInfoClassX(int serviceCode, int ssServiceClassFeature);
const char* ssStatusToOpCodeString(SsStatusE status);
char* convertToUCS2(char* data);
const char* callBarFacToServiceCodeStrings(const char* fac);
GsmCbsDcsE checkCbsDcs(int dcs);
const char* callForwardReasonToServiceCodeString(CallForwardReasonE cfreason);
void setErrorMessageFromXcap(int slotId, AT_CME_Error errorCode, const char* message);
void clearErrorMessageFromXcap(int slotId);
bool isReallyDialable(char c);
bool isStartsPostDial(char c);
char* extractNetworkPortionAlt(char* phoneNumber);
char* encryptString(char* input);
#endif /* SS_UTIL */
