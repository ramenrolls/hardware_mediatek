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

#ifndef __RMC_CDMA_SMS_MEM_FULL_H__
#define __RMC_CDMA_SMS_MEM_FULL_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaSmsMemFullHdlr
 *****************************************************************************/
class RmcCdmaSmsMemFullHdlr : public RmcBaseUrcHandler {
  public:
    RmcCdmaSmsMemFullHdlr(RfxBaseHandler* h) : RmcBaseUrcHandler(h) {}
    virtual ~RmcCdmaSmsMemFullHdlr() {}

  public:
    virtual bool onHandleRawUrc(RfxAtLine* line);
    virtual RmcBaseUrspData* onGetUrcData(int slotId);

  private:
    String8 m_memType;
};

#endif /* __RMC_CDMA_SMS_MEM_FULL_H__ */
