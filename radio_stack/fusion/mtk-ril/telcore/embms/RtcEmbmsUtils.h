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

#ifndef __RTC_EMBMS_UTILS_H__
#define __RTC_EMBMS_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "rfx_properties.h"
#include <utils/Vector.h>
#include "RfxStatusDefs.h"
#include "RfxController.h"
#include "RtcEmbmsSessionInfo.h"

/*****************************************************************************
 * Class RpDataUtils
 *****************************************************************************/

class RtcEmbmsUtils {
  public:
    RtcEmbmsUtils();
    virtual ~RtcEmbmsUtils();

    static RtcEmbmsSessionInfo* findSessionByTransId(Vector<RtcEmbmsSessionInfo*>* list,
                                                     int trans_id, int* index);
    static RtcEmbmsSessionInfo* findSessionByTmgi(Vector<RtcEmbmsSessionInfo*>* list, int tmgi_len,
                                                  char* pTmgi, int* index);

    static void freeSessionList(Vector<RtcEmbmsSessionInfo*>* list);
    static int getDefaultDataSlotId();
    static bool revertTmgi(const uint8_t* input, char* output, int length);
    static bool convertTmgi(const char* input, uint8_t* output);
    static bool isEmbmsSupport();
    static bool isAtCmdEnableSupport();
    static bool isRjilSupport();
    static bool isDualLteSupport();
    static char* addLogMask(int i);
};

#endif /* __RTC_EMBMS_UTILS_H__ */
