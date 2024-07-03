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

#ifndef __GSMSMS_PARSER_UTILS_H__
#define __GSMSMS_PARSER_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>
#include "SmsParserUtils.h"
#include "SmsAddress.h"
/*****************************************************************************
 * Class SmsMessage
 *****************************************************************************/
class GsmSmsAddress : public SmsAddress {
  public:
    GsmSmsAddress(BYTE* data, int length);
    virtual ~GsmSmsAddress();

  public:
    static const int OFFSET_ADDRESS_LENGTH;
    static const int OFFSET_TOA;
    static const int OFFSET_ADDRESS_VALUE;
};

#endif /* __GSMSMS_PARSER_UTILS_H__ */
