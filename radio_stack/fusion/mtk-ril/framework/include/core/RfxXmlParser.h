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

#ifndef __RFX_XML_PARSER_H__
#define __RFX_XML_PARSER_H__

#include <string>

#include "RfxLog.h"
#include "RfxDefs.h"
#include "tinyxml.h"
#include "utils/RefBase.h"
#include "utils/String8.h"

// using ::android::String8;
using ::android::RefBase;
using ::android::sp;

using namespace std;

class DefaultHandler : public virtual RefBase {
  public:
    virtual ~DefaultHandler(){};
    virtual void startElement(string nodeName, string nodeValue, string attributeName,
                              string attributeValue) = 0;
    virtual void endElement(string nodeName) = 0;
};

class RfxXmlParser : public virtual RefBase {
  public:
    virtual ~RfxXmlParser();
    void parse(DefaultHandler* parsedData, string xmlData);
    void parseSubElement(TiXmlElement* subElement, DefaultHandler* parsedData);
};
#endif
