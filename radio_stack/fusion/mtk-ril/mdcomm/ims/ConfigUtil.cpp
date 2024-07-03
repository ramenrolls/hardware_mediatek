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

#include "ConfigUtil.h"
#include "rfx_properties.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "RfxLog.h"

#define RFX_LOG_TAG "CONFIG-UTIL"

void skipWhiteSpace(char** p_cur) {
    if (*p_cur == NULL) return;
    while (**p_cur != '\0' && isspace(**p_cur)) {
        (*p_cur)++;
    }
}
void skipNextComma(char** p_cur) {
    if (*p_cur == NULL) return;
    while (**p_cur != '\0' && **p_cur != ',') {
        (*p_cur)++;
    }
    if (**p_cur == ',') {
        (*p_cur)++;
    }
}

char* nextTok(char** p_cur) {
    char* ret = NULL;
    skipWhiteSpace(p_cur);
    if (*p_cur == NULL) {
        ret = NULL;
    } else if (**p_cur == '"') {
        (*p_cur)++;
        ret = strsep(p_cur, "\"");
        skipNextComma(p_cur);
    } else {
        ret = strsep(p_cur, ",");
    }

    if ((ret != NULL) && (strlen(ret) == 0)) {
        ret = NULL;
    }
    RFX_LOG_D(RFX_LOG_TAG, "nextTok ret:%s", ret);
    return ret;
}
