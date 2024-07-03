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

#ifndef MERGERULE_H
#define MERGERULE_H
#include "FilterStructure.h"

#define RR_LOG_TAG "WpfaRuleRegister"

#define COMMON_ARG_LENGTH 20
#define IP_LENGTH 50

#define MAX_IPTABLE_ARG_NUM 20

#define REGISTER_NFQUEUE_QNUM "90"
#define REGISTER_ICMP_NFQUEUE_QNUM "91"

#define MERGE_IPTABLE_ARG_OK WPFA_REG_ACCPET
#define MERGE_IPTABLE_ARG_FAILED WPFA_RULE_ERROR

int icmpMergeArgs(wifiProxy_filter_ip_ver_e ipType, WPFA_filter_reg_t filter, int operation);
int espAhMergeArgs(wifiProxy_filter_ip_ver_e ipType, WPFA_filter_reg_t filter, int operation);
int commonMergeArgs(wifiProxy_filter_ip_ver_e ipType, WPFA_filter_reg_t filter, int operation);
void convertIpAddress(char* dst, uint8_t* ip, wifiProxy_filter_ip_ver_e ipType);
int initIptablesChain();

#endif  // MERGERULE_H
