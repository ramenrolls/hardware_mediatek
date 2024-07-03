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

#ifndef __SMS_ADDRESS_H__
#define __SMS_ADDRESS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "SmsParserUtils.h"
/*****************************************************************************
 * Class SmsMessage
 *****************************************************************************/
class SmsAddress {
  public:
    SmsAddress();
    string getAddressString();
    bool isAlphanumeric();
    bool isNetworkSpecific();
    bool couldBeEmailGateway();

  public:
    static const int TON_UNKNOWN;
    static const int TON_INTERNATIONAL;
    static const int TON_NATIONAL;
    static const int TON_NETWORK;
    static const int TON_SUBSCRIBER;
    static const int TON_ALPHANUMERIC;
    static const int TON_ABBREVIATED;
    int mTon;
    string mAddress;
    BYTE* mOrigBytes;
};

#endif /* __SMS_ADDRESS_H__ */
