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

#include "RfxAtResponse.h"

/*void RfxAtResponse::setFinalResponse(char* finalResponse) {
    RfxAtLine final = new RfxAtLine(finalResponse, NULL);
    m_finalResponse = final;
}*/

void RfxAtResponse::setFinalResponse(RfxAtLine* finalResponse) { m_finalResponse = finalResponse; }

/*void RfxAtResponse::setIntermediates(char* line) {
    RfxAtLine intermediates = new RfxAtLine(line, NULL);
    if (m_pIntermediates == NULL) {
        m_pIntermediates = intermediates;
    } else {
        // find the last one position
        RfxAtLine* tmp = m_pIntermediates;
        while ((tmp=tmp->getNext()) != NULL) {
        }
        tmp->setNext(intermediates);
    }
}*/

void RfxAtResponse::setIntermediates(RfxAtLine* line) {
    if (m_pIntermediates == NULL) {
        m_pIntermediates = line;
    } else {
        // find the last one position
        RfxAtLine* tmp = m_pIntermediates;
        while (tmp->getNext() != NULL) {
            tmp = tmp->getNext();
        }
        tmp->setNext(line);
    }
}

AT_CME_Error RfxAtResponse::atGetCmeError() {
    int ret;
    int err;
    char* p_cur;

    if (m_success > 0) return CME_SUCCESS;

    if (m_finalResponse == NULL) return CME_ERROR_NON_CME;

    if (RfxMisc::strStartsWith(m_finalResponse->getLine(), "ERROR")) return CME_UNKNOWN;

    if (!RfxMisc::strStartsWith(m_finalResponse->getLine(), "+CME ERROR:"))
        return CME_ERROR_NON_CME;

    // make use of RfxAtLine
    m_finalResponse->atTokStart(&err);

    if (err < 0) return CME_ERROR_NON_CME;

    ret = m_finalResponse->atTokNextint(&err);

    if (err < 0) return CME_ERROR_NON_CME;

    return (AT_CME_Error)ret;
}

int RfxAtResponse::isATCmdRspErr() { return (m_err < 0 || 0 == m_success) ? 1 : 0; }

void RfxAtResponse::reverseIntermediates() {
    RfxAtLine *pcur, *pnext;

    pcur = m_pIntermediates;
    m_pIntermediates = NULL;

    while (pcur != NULL) {
        pnext = pcur->getNext();
        pcur->setNext(m_pIntermediates);
        m_pIntermediates = pcur;
        pcur = pnext;
    }
}

/*
** Indicate command send to mux or not
*/
bool RfxAtResponse::isAtSent() { return (m_err > 0); }

/*
** Command has sent to modem and get response.
** This API will return success or failure of response
*/
bool RfxAtResponse::isAtResponseFail() { return (m_err < 0) || (m_success <= 0); }
