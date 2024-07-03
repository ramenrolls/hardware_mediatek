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

#include "RfxXmlParser.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RfxXmlParser"

RfxXmlParser::~RfxXmlParser() {}

void RfxXmlParser::parse(DefaultHandler* parsedData, string xmlData) {
    TiXmlDocument* myDoc = new TiXmlDocument();
    myDoc->Parse(xmlData.c_str());
    TiXmlElement* rootElement = myDoc->RootElement();
    parseSubElement(rootElement, parsedData);
    delete myDoc;
}

void RfxXmlParser::parseSubElement(TiXmlElement* subElement, DefaultHandler* parsedData) {
    if (NULL == subElement) {
        return;
    }
    string nodeName = "";
    string attributeName = "";
    string attributeValue = "";
    string nodeValue = "";

    TiXmlAttribute* subAttribute = subElement->FirstAttribute();
    // handle node without attribute.
    if (subAttribute == 0) {
        nodeName = subElement->Value();
        nodeValue = "";
        parsedData->startElement(nodeName, nodeValue, attributeName, attributeValue);
    }
    //
    while (subAttribute) {
        nodeName = subElement->Value();
        nodeValue = "";
        attributeName = subAttribute->Name();
        attributeValue = subAttribute->Value();
        parsedData->startElement(nodeName, nodeValue, attributeName, attributeValue);
        subAttribute = subAttribute->Next();
    }

    if (NULL != subElement->FirstChildElement()) {
        parseSubElement((TiXmlElement*)subElement->FirstChild(), parsedData);
    } else {
        if (0 != subElement->GetText()) {
            nodeValue = subElement->GetText();
        } else {
            nodeValue = "";
        }
        nodeName = subElement->Value();
        attributeName = "";
        attributeValue = "";
        parsedData->startElement(nodeName, nodeValue, attributeName, attributeValue);
    }

    nodeName = subElement->Value();
    parsedData->endElement(nodeName);

    if (subElement->NextSiblingElement()) {
        parseSubElement(subElement->NextSiblingElement(), parsedData);
    }
}
