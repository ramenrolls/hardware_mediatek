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

#ifndef __RFX_AT_LINE_H__
#define __RFX_AT_LINE_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RfxDefs.h"
#include "RfxLog.h"

class RfxAtLine {
  public:
    RfxAtLine() : m_line(NULL), m_pNext(NULL) {}

    RfxAtLine(const char* line, RfxAtLine* next);

    // copy constructor
    RfxAtLine(const RfxAtLine& other);

    ~RfxAtLine();

    RfxAtLine& operator=(const RfxAtLine& other);

  public:
    RfxAtLine* getNext() const { return m_pNext; }
    void setNext(RfxAtLine* next) { m_pNext = next; }
    char* getLine() const { return m_line; }
    void setLine(char* line) { m_line = line; }
    char* getCurrentLine() { return m_pCur; }
    void atTokStart(int* err);
    int atTokNextint(int* err);
    int atTokNexthexint(int* err);
    bool atTokNextbool(int* err);
    char* atTokNextstr(int* err);
    int atTokHasmore();
    char* atTokChar(int* err);
    void atTokEqual(int* err);
    long long atTokNextlonglong(int* err);
    int isFinalResponseSuccess();
    int isFinalResponseErrorEx(int channel_id);
    int isIntermediatePattern();
    bool isFinalResponseSuccessInNumeric();
    bool isFinalResponseErrorInNumeric();
    bool isAckResponse();

  private:
    void skipWhiteSpace();
    void skipNextComma();
    int atTokNextintBase(int base, int uns, int* err);
    long long atTokNextlonglongBase(int base, int uns, int* err);
    char* nextTok();

  private:
    char* m_line;  // should dynamic allocate memory?
    RfxAtLine* m_pNext;
    char* m_pCur;  // current position, initialize at atTokStart
};
#endif
