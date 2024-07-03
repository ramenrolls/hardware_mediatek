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

#ifndef __RFX_IMS_MESSAGE_OP_ID_HEADER__
#define __RFX_IMS_MESSAGE_OP_ID_HEADER__

// Request msg id
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SET_DIGITS_LINE)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SET_TRN)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_DIAL_FROM)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SEND_USSI_FROM)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_CANCEL_USSI_FROM)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SET_EMERGENCY_CALL_CONFIG)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_DEVICE_SWITCH)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_CANCEL_DEVICE_SWITCH)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SET_DIGITS_REG_STATUS)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_SWITCH_RCS_ROI_STATUS)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_UPDATE_RCS_CAPABILITIES)
RFX_MSG_ID_EXPN(RFX_MSG_REQUEST_UPDATE_RCS_SESSION_INFO)
// URC msg id
RFX_MSG_ID_EXPN(RFX_MSG_UNSOL_DIGITS_LINE_INDICATION)
RFX_MSG_ID_EXPN(RFX_MSG_UNSOL_GET_TRN_INDICATION)
RFX_MSG_ID_EXPN(RFX_MSG_UNSOL_RCS_DIGITS_LINE_INFO)

#endif
