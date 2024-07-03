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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "SmsParserUtils.h"
#include "SmsAddress.h"
/*****************************************************************************
 * Class SmsAddress
 *****************************************************************************/
#define SMS_ADDR_EMAIL_GATEWAY_MAX_LEN 4

const int SmsAddress::TON_UNKNOWN = 0;
const int SmsAddress::TON_INTERNATIONAL = 1;
const int SmsAddress::TON_NATIONAL = 2;
const int SmsAddress::TON_NETWORK = 3;
const int SmsAddress::TON_SUBSCRIBER = 4;
const int SmsAddress::TON_ALPHANUMERIC = 5;
const int SmsAddress::TON_ABBREVIATED = 6;

SmsAddress::SmsAddress() : mTon(0), mAddress(""), mOrigBytes(NULL) {}

string SmsAddress::getAddressString() { return mAddress; }

bool SmsAddress::isAlphanumeric() { return mTon == TON_ALPHANUMERIC; }

bool SmsAddress::isNetworkSpecific() { return mTon == TON_NETWORK; }

bool SmsAddress::couldBeEmailGateway() {
    return mAddress.length() <= SMS_ADDR_EMAIL_GATEWAY_MAX_LEN;
}
