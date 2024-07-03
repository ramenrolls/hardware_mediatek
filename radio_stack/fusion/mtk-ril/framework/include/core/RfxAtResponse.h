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

#ifndef __RFX_AT_RESPONSE_H__
#define __RFX_AT_RESPONSE_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils/RefBase.h"
#include "RfxAtLine.h"
#include "RfxDefs.h"
#include "RfxMisc.h"

using ::android::RefBase;

class RfxAtResponse : public virtual RefBase {
  public:
    RfxAtResponse()
        : m_type(INIT),
          m_success(-1),
          m_ack(0),
          m_finalResponse(NULL),
          m_pIntermediates(NULL),
          m_err(0),
          m_responsePrefix(NULL) {}
    RfxAtResponse(AtCommandType type, int success, RfxAtLine* finalResponse, RfxAtLine* line,
                  int err, const char* responsePrefix)
        : m_type(type),
          m_success(success),
          m_ack(0),
          m_finalResponse(finalResponse),
          m_pIntermediates(line),
          m_err(err),
          m_responsePrefix(responsePrefix) {}
    RfxAtResponse(AtCommandType type, const char* responsePrefix)
        : m_type(type),
          m_success(-1),
          m_ack(0),
          m_finalResponse(NULL),
          m_pIntermediates(NULL),
          m_err(0),
          m_responsePrefix(responsePrefix) {}
    ~RfxAtResponse() {
        if (m_pIntermediates) {
            delete (m_pIntermediates);
        }
        if (m_finalResponse) {
            delete (m_finalResponse);
        }
    };

  public:
    AtCommandType getCommandType() const { return m_type; }
    void setCommandType(AtCommandType type) { m_type = type; }
    int getSuccess() const { return m_success; }
    void setSuccess(int success) { m_success = success; }
    RfxAtLine* getFinalResponse() const { return m_finalResponse; }
    // void setFinalResponse(char* finalResponse);
    void setFinalResponse(RfxAtLine* finalResponse);
    RfxAtLine* getIntermediates() const { return m_pIntermediates; }
    // void setIntermediates(char* line);
    void setIntermediates(RfxAtLine* line);
    int getError() const { return m_err; }
    void setError(int err) { m_err = err; }
    int getIsAck() const { return m_ack; }
    void setAck(int ack) { m_ack = ack; }

    const char* getResponsePrefix() const { return m_responsePrefix; }
    /*void setResponsePrefix(char *responsePrefix) {
        m_responsePrefix = responsePrefix;
    }*/
    AT_CME_Error atGetCmeError();
    int isATCmdRspErr();
    void reverseIntermediates();
    bool isAtSent();
    bool isAtResponseFail();

  private:
    AtCommandType m_type;
    int m_success;               /* true if final response indicates success (eg "OK") */
    int m_ack;                   /* true if it is Ack response */
    RfxAtLine* m_finalResponse;  /* eg OK, ERROR */
    RfxAtLine* m_pIntermediates; /* any intermediate responses */
    int m_err;
    const char* m_responsePrefix;
};
#endif
