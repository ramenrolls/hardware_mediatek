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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RtcSuppServUtil.h"
#include <openssl/sha.h>

#define RFX_LOG_TAG "RtcSuppServUtil"
/*****************************************************************************
 * Class RtcSuppServUtil
 *****************************************************************************/
vector<string> RtcSuppServUtil::split(string input, string pattern) {
    string::size_type pos;
    vector<string> result;
    input += pattern;
    string::size_type size = input.size();

    for (string::size_type i = 0; i < size; i++) {
        pos = input.find(pattern, i);
        if (pos < size) {
            string s = input.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

string& RtcSuppServUtil::trim(string& input) {
    if (input.empty()) {
        return input;
    }
    input.erase(0, input.find_first_not_of(" "));
    input.erase(input.find_last_not_of(" ") + 1);
    return input;
}

string RtcSuppServUtil::sha256(const string str) {
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    string tmp = "";
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(buf, sizeof(buf), "%02x", hash[i]);
        tmp = tmp + buf;
    }
    return tmp;
}
