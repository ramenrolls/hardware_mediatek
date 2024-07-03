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

#ifndef __RTC_MODE_DEFS_H__
#define __RTC_MODE_DEFS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxDefs.h"
#include "RfxStatusDefs.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
/*LOG_TAG*/
#define RTC_MODE_CONTROLLER_TAG "RtcModeCont"

/* CARD_TYPE*/
#define CARD_TYPE_INVALID (-1)
#define CARD_TYPE_NONE (0)
#define CARD_TYPE_SIM RFX_CARD_TYPE_SIM
#define CARD_TYPE_USIM RFX_CARD_TYPE_USIM
#define CARD_TYPE_RUIM RFX_CARD_TYPE_RUIM
#define CARD_TYPE_CSIM RFX_CARD_TYPE_CSIM

/* OPERATOR*/
#define OPERATOR_OM "OM"
#define OPERATOR_OP09 "OP09"

/*Card type state*/
#define CARD_STATE_INVALID (-1)
#define CARD_STATE_HOT_PLUGIN (0)
#define CARD_STATE_HOT_PLUGOUT (1)
#define CARD_STATE_NO_CHANGED (2)
#define CARD_STATE_NOT_HOT_PLUG (3)
#define CARD_STATE_CARD_TYPE_CHANGED (4)

/*Rat mode for ECC*/
#define RAT_MODE_INVALID (-1)
#define CDMA_EVDO_AUTO PREF_NET_TYPE_CDMA_EVDO_AUTO
#define LTE_WCDMA_GSM PREF_NET_TYPE_LTE_GSM_WCDMA

/*System property of C capability slot*/
#define PROPERTY_C_CAPABILITY_SLOT "persist.vendor.radio.c_capability_slot"

/*System property for common slot and VSIM check*/
#define PROPERTY_COMMON_SLOT_SUPPORT "ro.vendor.mtk_sim_hot_swap_common_slot"
#define PROPERTY_EXTERNAL_SIM_ENABLED "vendor.gsm.external.sim.enabled"

#endif /* __RTC_MODE_DEFS_H__  */
