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
{RIL_UNSOL_DIGITS_LINE_INDICATION, radio::digitsLineIndicationInd, WAKE_PARTIAL},
        {RIL_UNSOL_GET_TRN_INDICATION, radio::getTrnIndicationInd, WAKE_PARTIAL},
        // MTK-START: SIM TMO RSU
        {RIL_UNSOL_MELOCK_NOTIFICATION, radio::onSimMeLockEvent, WAKE_PARTIAL},
        // MTK-END
        {RIL_UNSOL_MODULATION_INFO, radio::responseModulationInfoInd, WAKE_PARTIAL},
        {RIL_UNSOL_RCS_DIGITS_LINE_INFO, radio::digitsLineIndication, WAKE_PARTIAL},
        {RIL_UNSOL_ENTER_SCBM, radio::enterSCBMInd, WAKE_PARTIAL},
        {RIL_UNSOL_EXIT_SCBM, radio::exitSCBMInd, WAKE_PARTIAL},
        {RIL_UNSOL_RSU_EVENT, radio::onRsuEvent, WAKE_PARTIAL},
#else
{RIL_UNSOL_DIGITS_LINE_INDICATION, responseStrings, WAKE_PARTIAL},
        {RIL_UNSOL_GET_TRN_INDICATION, responseStrings, WAKE_PARTIAL},
        // MTK-START: SIM TMO RSU
        {RIL_UNSOL_MELOCK_NOTIFICATION, responseInts, WAKE_PARTIAL},
        // MTK-END
        {RIL_UNSOL_ENTER_SCBM, responseVoid, WAKE_PARTIAL},
        {RIL_UNSOL_EXIT_SCBM, responseVoid, WAKE_PARTIAL},
        {RIL_UNSOL_RSU_EVENT, responseInts, WAKE_PARTIAL},
#endif  // MTK_USE_HIDL
