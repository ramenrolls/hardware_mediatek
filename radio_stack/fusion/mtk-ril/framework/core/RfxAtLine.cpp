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

#include "RfxAtLine.h"
#include "RfxMisc.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RfxAtLine"

static const char* s_finalResponsesSuccess[] = {
        "OK", "CONNECT" /* some stacks start up data on another channel */
};

/**
 * returns 1 if line is a final response indicating error
 * See 27.007 annex B
 * WARNING: NO CARRIER and others are sometimes unsolicited
 */
static const char* s_finalResponsesError[] = {
        "ERROR",       "+CMS ERROR:",
        "+CME ERROR:", "NO CARRIER", /* sometimes! */
        "NO ANSWER",   "NO DIALTONE",
        "+C2K ERROR:", "+EC2KCME ERROR", /* for distinguish CME error between MD1 and MD3 */
};

/**
 * returns 1 if line is a intermediate response
 * Such as
 * AT+CMGW <CR>
 * >XXXXXX  <CTRL+Z> or <ESC>
 * OK
 * WARNING: NO CARRIER and others are sometimes unsolicited
 */
static const char* s_IntermediatePattern[] = {
        "> ",
};

static const char* s_finalResponsesSuccessInNumeric[] = {
        "0",
        "1",
};

static const char* s_finalResponsesErrorInNumeric[] = {
        "2", "3", "4", "6", "7", "8", "+CMS ERROR:", "+CME ERROR:", "+C2K ERROR:",
};

static const char* s_ackResponse[] = {"ACK"};

RfxAtLine::RfxAtLine(const RfxAtLine& other) {
    // Only copy THIS node
    RFX_ASSERT(other.m_pNext == NULL);

    m_line = (char*)calloc(strlen(other.m_line) + 1, sizeof(char));
    if (m_line == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        m_pNext = NULL;
        m_pCur = NULL;
        return;
    }
    memcpy(m_line, other.m_line, strlen(other.m_line));
    m_line[strlen(other.m_line)] = '\0';
    m_pNext = NULL;

    // initialize p_cur
    m_pCur = m_line;
}

RfxAtLine::RfxAtLine(const char* line, RfxAtLine* next) {
    m_line = (char*)calloc(strlen(line) + 1, sizeof(char));
    if (m_line == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        m_pNext = NULL;
        m_pCur = NULL;
        return;
    }
    memcpy(m_line, line, strlen(line));
    m_line[strlen(line)] = '\0';
    m_pNext = next;

    // initialize p_cur
    m_pCur = m_line;
}

RfxAtLine::~RfxAtLine() {
    if (m_pNext) {
        delete (m_pNext);
    }

    m_pCur = NULL;
    if (m_line) {
        free(m_line);
    }
}

/**
 * Starts tokenizing an AT response string
 * Set err to -1 if this is not a valid response string, 0 on success.
 * updates m_pCur with current position
 */
void RfxAtLine::atTokStart(int* err) {
    *err = 0;
    m_pCur = m_line;
    if (m_pCur == NULL) {
        *err = -1;
        return;
    }

    // skip prefix
    // consume "^[^:]:"

    m_pCur = strchr(m_pCur, ':');

    if (m_pCur == NULL) {
        *err = -1;
        return;
    }

    m_pCur++;
}

char* RfxAtLine::atTokChar(int* err) {
    *err = 0;
    if (m_pCur == NULL) {
        *err = -1;
        return NULL;
    }
    m_pCur = strchr(m_pCur, '(');

    if (m_pCur == NULL) {
        *err = -1;
        return NULL;
    }

    return (m_pCur++);
}

void RfxAtLine::skipWhiteSpace() {
    if (m_pCur == NULL) return;

    while (*m_pCur != '\0' && isspace(*m_pCur)) {
        m_pCur++;
    }
}

void RfxAtLine::skipNextComma() {
    if (m_pCur == NULL) return;

    while (*m_pCur != '\0' && *m_pCur != ',') {
        m_pCur++;
    }

    if (*m_pCur == ',') {
        m_pCur++;
    }
}

char* RfxAtLine::nextTok() {
    char* ret;

    skipWhiteSpace();

    if (m_pCur == NULL) {
        ret = NULL;
    } else if (*m_pCur == '"') {
        m_pCur++;
        ret = strsep(&m_pCur, "\"");
        skipNextComma();
    } else if (*m_pCur == '(' && *(m_pCur + 1) == '"') {
        m_pCur = m_pCur + 2;
        ret = strsep(&m_pCur, "\"");
        skipNextComma();
    } else {
        ret = strsep(&m_pCur, ",");
    }

    return ret;
}

/**
 * Parses the next integer in the AT response line and places it in *p_out
 * Set err to 0 on success and -1 on fail
 * updates m_pCur
 * "base" is the same as the base param in strtol
 */

int RfxAtLine::atTokNextintBase(int base, int uns, int* err) {
    int out;
    char* ret;
    *err = 0;

    if (m_pCur == NULL) {
        *err = -1;
        return 0;
    }

    ret = nextTok();

    if (ret == NULL) {
        *err = -1;
        return 0;
    } else {
        long l;
        char* end;

        if (uns)
            l = strtoul(ret, &end, base);
        else
            l = strtol(ret, &end, base);

        out = (int)l;

        if (end == ret) {
            *err = -1;
            return 0;
        }
        return out;
    }
}

/**
 * Parses the next base 10 integer in the AT response line
 * and places it in *p_out
 * Set err to 0 on success and -1 on fail
 * updates m_pCur
 */
int RfxAtLine::atTokNextint(int* err) { return atTokNextintBase(10, 0, err); }

/**
 * Parses the next base 16 integer in the AT response line
 * and places it in *p_out
 * Set err to 0 on success and -1 on fail
 * updates m_pCur
 */
int RfxAtLine::atTokNexthexint(int* err) { return atTokNextintBase(16, 1, err); }

bool RfxAtLine::atTokNextbool(int* err) {
    int result;

    result = atTokNextint(err);

    if (*err < 0) {
        *err = -1;
        return false;
    }

    // booleans should be 0 or 1
    if (!(result == 0 || result == 1)) {
        *err = -1;
        return false;
    }

    return result ? true : false;
}

char* RfxAtLine::atTokNextstr(int* err) {
    *err = 0;
    if (m_pCur == NULL) {
        *err = -1;
        return NULL;
    }

    return nextTok();
}

/** returns 1 on "has more tokens" and 0 if no */
int RfxAtLine::atTokHasmore() { return !(m_pCur == NULL || *m_pCur == '\0'); }

/// M: eMBMS feature
void RfxAtLine::atTokEqual(int* err) {
    *err = 0;
    m_pCur = m_line;
    if (m_pCur == NULL) {
        *err = -1;
        return;
    }

    // skip prefix
    // consume "^[^=]:"

    m_pCur = strchr(m_pCur, '=');

    if (m_pCur == NULL) {
        *err = -1;
        return;
    }

    m_pCur++;
}

/**
 * Parses the next long long in the AT response line and places it in *p_out
 * Set err to 0 on success and -1 on fail
 * updates m_pCur
 * "base" is the same as the base param in strtoll
 */

long long RfxAtLine::atTokNextlonglongBase(int base, int uns, int* err) {
    char* ret;
    long long out;
    *err = 0;

    if (m_pCur == NULL) {
        *err = -1;
        return 0;
    }

    ret = nextTok();

    if (ret == NULL) {
        *err = -1;
        return 0;
    } else {
        long long ll;
        char* end;

        if (uns)
            ll = strtoull(ret, &end, base);
        else
            ll = strtoll(ret, &end, base);

        out = ll;

        if (end == ret) {
            *err = -1;
            return 0;
        }
    }

    return out;
}

/**
 * Parse the next base 10 long long in the AT response line
 * and places it in *p_out
 * Set err to 0 on success and -1 on fail
 * updates m_pCur
 */
long long RfxAtLine::atTokNextlonglong(int* err) { return atTokNextlonglongBase(10, 0, err); }

int RfxAtLine::isFinalResponseSuccess() {
    for (size_t i = 0; i < NUM_ELEMS(s_finalResponsesSuccess); i++) {
        if (RfxMisc::strStartsWith(m_line, s_finalResponsesSuccess[i])) {
            return 1;
        }
    }

    return 0;
}

int RfxAtLine::isFinalResponseErrorEx(int channel_id) {
    size_t i;

    int j = 0;
    for (j = 0; j < RfxRilUtils::rfxGetSimCount(); j++) {
        if ((channel_id == (int)(RIL_URC + j * RIL_CHANNEL_OFFSET)) &&
            (RfxMisc::strStartsWith(m_line, "NO CARRIER"))) {
            // [ALPS01225455]NO CARRIER in URC channel is URC, not final response for mediatek modem
            return 0;
        }
    }

    for (i = 0; i < NUM_ELEMS(s_finalResponsesError); i++) {
        if (RfxMisc::strStartsWith(m_line, s_finalResponsesError[i])) {
            return 1;
        }
    }
    return 0;
}

int RfxAtLine::isIntermediatePattern() {
    size_t i;
    for (i = 0; i < NUM_ELEMS(s_IntermediatePattern); i++) {
        if (!strcmp(m_line, s_IntermediatePattern[i])) {
            return 1;
        }
    }
    return 0;
}

bool RfxAtLine::isFinalResponseSuccessInNumeric() {
    for (size_t i = 0; i < NUM_ELEMS(s_finalResponsesSuccessInNumeric); i++) {
        if (!strcmp(m_line, s_finalResponsesSuccessInNumeric[i])) {
            return 1;
        }
    }
    return 0;
}

bool RfxAtLine::isFinalResponseErrorInNumeric() {
    for (size_t i = 0; i < NUM_ELEMS(s_finalResponsesErrorInNumeric); i++) {
        if (!strncmp(m_line, s_finalResponsesErrorInNumeric[i],
                     strlen(s_finalResponsesErrorInNumeric[i]))) {
            return 1;
        }
    }
    return 0;
}

bool RfxAtLine::isAckResponse() {
    for (size_t i = 0; i < NUM_ELEMS(s_ackResponse); i++) {
        if (!strncmp(m_line, s_ackResponse[i], strlen(s_ackResponse[i]))) {
            return true;
        }
    }
    return false;
}
