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
#include <string.h>
#include <stdlib.h>
#include "RmcCdmaBcRangeParser.h"

/*****************************************************************************
 * Class RmcCdmaBcRangeParser
 *****************************************************************************/
void RmcCdmaBcRangeParser::split(char* src, const char* sep, char dest[][MAX_RANGE_CHARACTER],
                                 int* num) {
    char* next;
    int count = 0;
    if (src == NULL || strlen(src) == 0) {
        return;
    }

    if (sep == NULL || strlen(sep) == 0) {
        return;
    }
    next = strtok(src, sep);
    while (next != NULL) {
        strncpy(*dest++, next, MAX_RANGE_CHARACTER - 1);
        count++;
        next = strtok(NULL, sep);
    }
    *num = count;
}

char* RmcCdmaBcRangeParser::trim(char* s) {
    int i;
    if (s == NULL) {
        return NULL;
    }
    char* start = NULL;
    while (*s) {
        if ((*s != '\t') && (*s != '\n') && (*s != ' ')) {
            start = s;
            break;
        }
        s++;
    }
    if (start == NULL) {
        return NULL;
    }

    for (i = strlen(s) - 1; i >= 0; i--) {
        if ((s[i] != '\t') && (s[i] != '\n') && (s[i] != ' ')) {
            break;
        }
        s[i] = '\0';
    }
    return start;
}

char* RmcCdmaBcRangeParser::skipQuote(char* s) {
    int i;
    if (s == NULL) {
        return NULL;
    }
    i = strlen(s) - 1;
    if (s[i] == '\"') {
        s[i] = '\0';
    }
    if (s[0] == '\"') {
        s++;
    }
    return s;
}

int RmcCdmaBcRangeParser::getRangeFromModem(char* cateogry, Range* r) {
    char ranges[MAX_RANGE][MAX_RANGE_CHARACTER];
    memset(ranges, 0, sizeof(ranges));
    int num;
    int validNum = 0;
    int i;
    split(cateogry, ",", ranges, &num);
    for (i = 0; i < num; i++) {
        char range[MAX_RANGE][MAX_RANGE_CHARACTER];
        memset(range, 0, sizeof(range));
        int n;
        split(skipQuote(trim(ranges[i])), "-", range, &n);
        if (n == VALID_RANGE_ENDPOINT_NUM) {
            r[validNum].start = atoi(range[0]);
            r[validNum].end = atoi(range[1]);
            validNum++;
        }
    }
    return validNum;
}

void RmcCdmaBcRangeParser::getRange(Vector<Range>& range, SortedVector<int>& input) {
    int num = 0;
    int i;
    int start = input[0];
    int n = input.size();
    for (i = 1; i < n; i++) {
        if (input[i] > input[i - 1] + 1) {
            Range r;
            r.start = start;
            r.end = input[i - 1];
            range.push(r);
            start = input[i];
        }
    }
    Range r;
    r.start = start;
    r.end = input[i - 1];
    range.push(r);
    return;
}
