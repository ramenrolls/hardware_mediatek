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

#ifdef MTK_USE_HIDL
{RIL_REQUEST_SET_DIGITS_LINE, radio::setDigitsLineResponse},
        {RIL_REQUEST_SET_TRN, radio::setTrnResponse},
        {RIL_REQUEST_VSS_ANTENNA_CONF, radio::setRxTestConfigResponse},  // Antenna Testing
        {RIL_REQUEST_VSS_ANTENNA_INFO, radio::getRxTestResultResponse},  // Antenna Testing
        {RIL_REQUEST_DIAL_FROM, radio::dialFromResponse},
        {RIL_REQUEST_SEND_USSI_FROM, radio::sendUssiFromResponse},
        {RIL_REQUEST_CANCEL_USSI_FROM, radio::cancelUssiFromResponse},
        {RIL_REQUEST_SET_EMERGENCY_CALL_CONFIG, radio::setEmergencyCallConfigResponse},
        {RIL_REQUEST_SET_DISABLE_2G, radio::setDisable2GResponse},
        {RIL_REQUEST_GET_DISABLE_2G, radio::getDisable2GResponse},
        {RIL_REQUEST_DEVICE_SWITCH, radio::deviceSwitchResponse},
        {RIL_REQUEST_CANCEL_DEVICE_SWITCH, radio::cancelDeviceSwitchResponse},
        {RIL_REQUEST_SET_INCOMING_VIRTUAL_LINE, radio::setIncomingVirtualLineResponse},
        {RIL_REQUEST_SET_DIGITS_REG_STATUS, radio::setDigitsRegStatuseResponse},
        {RIL_REQUEST_EXIT_SCBM, radio::exitSCBMResponse},
        {RIL_REQUEST_SEND_RSU_REQUEST, radio::sendRsuRequestResponse},
        {RIL_REQUEST_SWITCH_RCS_ROI_STATUS, radio::switchRcsRoiStatusResponse},
        {RIL_REQUEST_UPDATE_RCS_CAPABILITIES, radio::updateRcsCapabilitiesResponse},
        {RIL_REQUEST_UPDATE_RCS_SESSION_INFO, radio::updateRcsSessionInfoResponse},
#else
{RIL_REQUEST_SET_DIGITS_LINE, dispatchStrings, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_SET_TRN, dispatchStrings, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_DIAL_FROM, dispatchDial, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_SEND_USSI_FROM, dispatchStrings, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_CANCEL_USSI_FROM, dispatchString, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_SET_EMERGENCY_CALL_CONFIG, dispatchInts, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_1},
        {RIL_REQUEST_DEVICE_SWITCH, dispatchStrings, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_1},
        {RIL_REQUEST_CANCEL_DEVICE_SWITCH, dispatchVoid, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_INCOMING_VIRTUAL_LINE, dispatchStrings, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_EXIT_SCBM, dispatchVoid, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_SEND_RSU_REQUEST, dispatchVoid, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SWITCH_RCS_ROI_STATUS, radio::switchRcsRoiStatusResponse},
        {RIL_REQUEST_UPDATE_RCS_CAPABILITIES, radio::updateRcsCapabilitiesResponse},
        {RIL_REQUEST_UPDATE_RCS_SESSION_INFO, radio::updateRcsSessionInfoResponse},
#endif  // MTK_USE_HIDL
