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

#ifndef SS_CONFIG
#define SS_CONFIG

class SSConfig {
  public:
    static const char* NOT_SHOW_FORWARDED_TOAST[];
    static const char* USSI_WITH_NO_LANG[];
    static const char* CONVERT_409_TO_NOT_SUPPROT_CALLER_ID[];
    static const char* CONVERT_409_TO_NOT_SUPPROT_CB[];
    static const char* NOT_SUPPORT_USSI_OVER_CALL[];
    static bool notShowForwardedToast(const char* mccmnc);
    static bool ussiWithNoLang(const char* mccmnc);
    static bool convert409ToNotSupportCallerID(const char* mccmnc);
    static bool convert409ToNotSupportCB(const char* mccmnc);
    static bool isNotSupportUSSIOverCall(const char* mccmnc);

  private:
    static bool isInList(const char* mccmnc, const char* list[], unsigned long size);
};
#endif /* SS_CONFIG */
