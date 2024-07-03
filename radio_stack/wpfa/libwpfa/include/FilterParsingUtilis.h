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

#ifndef FILTERPARSINGUTILS_H
#define FILTERPARSINGUTILS_H

#define IPTALBE_NEW_RULE 0
#define IPTALBE_DELETE_RULE 1

#define FP_LOG_TAG "WpfaParsing"

#include "FilterStructure.h"
#include "RuleRegisterUtilis.h"

int checkFilterConfig(wifiProxy_filter_config_e filter_config, wifiProxy_filter_config_e config);
void dumpFilter(WPFA_filter_reg_t filter);

int executeFilterReg(wifiproxy_m2a_reg_dl_filter_t m2a_reg_dl_filter);
int executeFilterDeReg(uint32_t fid);

filter_md_ap_ver_enum getUsingFilterVersion();
void setUsingFilterVersion(filter_md_ap_ver_enum ver);

// For testing, will remove
void testingFilter();

void initTestingFilterStructue(wifiProxy_filter_reg_t* filter);
void initTestingFilterStructue1(wifiProxy_filter_reg_t* filter);
void initTestingFilterStructue2(wifiProxy_filter_reg_t* filter);
void initTestingFilterStructue3(wifiProxy_filter_reg_t* filter);
void initTestingFilterStructue7(wifiProxy_filter_reg_t* filter);
void initTestingFilterStructue8(wifiProxy_filter_reg_t* filter);

#endif  // FILTERPARSINGUTILS_H
