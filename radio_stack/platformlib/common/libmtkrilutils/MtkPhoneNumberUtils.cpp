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

#include <string>
#include <mtk_log.h>
#include <mtk_properties.h>
#include "MtkPhoneNumberUtils.h"
#include "libmtkrilutils.h"

using std::string;

#define LOG_TAG "LibMtkPhoneNumberUtils"

const char MtkPhoneNumberUtils::PAUSE = ',';
const char MtkPhoneNumberUtils::WAIT = ';';
const char MtkPhoneNumberUtils::WILD = 'N';
const int MtkPhoneNumberUtils::MIN_MATCH = 7;
const int MtkPhoneNumberUtils::MIN_MATCH_CTA = 11;

static const char PROPERTY_SPECIAL_ECC_LIST[MAX_SIM_COUNT][MAX_PROP_CHARS] = {
        "vendor.ril.special.ecclist",
        "vendor.ril.special.ecclist1",
        "vendor.ril.special.ecclist2",
        "vendor.ril.special.ecclist3",
};

MtkPhoneNumberUtils::~MtkPhoneNumberUtils() {}

bool MtkPhoneNumberUtils::compareLoosely(string a, string b) {
    int ia = 0;
    int ib = 0;
    int matched = 0;
    int numberNonDialableCharsInA = 0;
    int numberNonDialableCharsInB = 0;
    if (a.empty() || b.empty()) {
        return a.compare(b) == 0;
    }
    if (a.length() == 0 || b.length() == 0) {
        return false;
    }
    ia = indexOfLastNetworkChar(a);
    ib = indexOfLastNetworkChar(b);
    while (ia >= 0 && ib >= 0) {
        char ca = ' ';
        char cb = ' ';
        bool skipCmp = false;
        ca = a.at(ia);
        if (!isDialable(ca)) {
            ia--;
            skipCmp = true;
            numberNonDialableCharsInA++;
        }
        cb = b.at(ib);
        if (!isDialable(cb)) {
            ib--;
            skipCmp = true;
            numberNonDialableCharsInB++;
        }
        if (!skipCmp) {
            if (cb != ca && ca != WILD && cb != WILD) {
                break;
            }
            ia--;
            ib--;
            matched++;
        }
    }
    // MIN match length for CT/CTA,not add the isCtaSupport or isOP09Support
    int minMatchLen = MIN_MATCH;
    char ctaSupport[MTK_PROPERTY_VALUE_MAX] = {0};
    char op09Support[MTK_PROPERTY_VALUE_MAX] = {0};
    mtk_property_get("ro.vendor.mtk_cta_support", ctaSupport, "0");
    mtk_property_get("persist.vendor.operator.optr", op09Support, "");
    if (strcmp(ctaSupport, "1") == 0 || strcmp(op09Support, "OP09") == 0) {
        minMatchLen = MIN_MATCH_CTA;
    }
    if (matched < minMatchLen) {
        int effectiveAlen = a.length() - numberNonDialableCharsInA;
        int effectiveBlen = b.length() - numberNonDialableCharsInB;
        // if the mumber of dialable chars in a and b match, but the matched chars < MIN_MATCH,
        if (effectiveAlen == effectiveBlen && effectiveAlen == matched) {
            return true;
        }
        return false;
    }
    // AT least one string has matched completely;
    if (matched >= minMatchLen && (ia < 0 || ib < 0)) {
        return true;
    }
    /// @}
    /*
     * Now, what remains must be one of the following for a
     * match:
     *
     *  - a '+' on one and a '00' or a '011' on the other
     *  - a '0' on one and a (+,00)<country code> on the other
     *     (for this, a '0' and a '00' prefix would have succeeded above)
     */
    if (matchIntlPrefix(a, ia + 1) && matchIntlPrefix(b, ib + 1)) {
        return true;
    }
    if (matchTrunkPrefix(a, ia + 1) && matchIntlPrefixAndCC(b, ib + 1)) {
        return true;
    }
    if (matchTrunkPrefix(b, ib + 1) && matchIntlPrefixAndCC(a, ia + 1)) {
        return true;
    }

    return false;
}

string MtkPhoneNumberUtils::getUsernameFromUriNumber(string number) {
    // The delimiter between username and domain name can be
    // either "@" or "%40" (the URI-escaped equivalent.)
    int delimiterIndex = number.find('@');
    if (delimiterIndex < 0) {
        delimiterIndex = number.find("%40");
    }
    if (delimiterIndex < 0) {
        // Logw("getUsernameFromUriNumber: no delimiter found in SIP addr" + number)
        delimiterIndex = number.length();
    }
    return number.substr(0, delimiterIndex);
}

// Check if the number is special emergency number (show as emergency call
// but dial using normal call flow)
bool MtkPhoneNumberUtils::isSpecialEmergencyNumber(int slotId, const char* number) {
    char specialEccList[MTK_PROPERTY_VALUE_MAX] = {0};

    mtk_property_get(PROPERTY_SPECIAL_ECC_LIST[slotId], specialEccList, "");
    mtkLogD(LOG_TAG, "[isSpecialEmergencyNumber] specialEccList: %s", specialEccList);

    char* p = NULL;
    p = strtok(specialEccList, ",");
    while (p) {
        if (strcmp(p, number) == 0) {
            mtkLogD(LOG_TAG, "[isSpecialEmergencyNumber] return true");
            return true;
        }
        // next number
        p = strtok(NULL, ",");
    }
    mtkLogD(LOG_TAG, "[isSpecialEmergencyNumber] return false");
    return false;
}

int MtkPhoneNumberUtils::indexOfLastNetworkChar(string a) {
    int pIndex = 0;
    int wIndex = 0;
    int origLenth = 0;
    int trimIndex = 0;
    origLenth = a.length();
    pIndex = a.find(PAUSE);
    wIndex = a.find(WAIT);
    trimIndex = MtkPhoneNumberUtils::minPositive(pIndex, wIndex);
    if (trimIndex < 0) {
        return origLenth - 1;
    } else {
        return trimIndex - 1;
    }
}

/**
 * or -1 if both are negative
 */
int MtkPhoneNumberUtils::minPositive(int a, int b) {
    if (a >= 0 && b >= 0) {
        return (a < b) ? a : b;
    } else if (a >= 0) { /*&& b < 0*/
        return a;
    } else if (b >= 0) { /*&& a < 0*/
        return b;
    } else { /* a < 0 && b < 0*/
        return -1;
    }
}

bool MtkPhoneNumberUtils::isDialable(char c) {
    return (c >= '0' && c <= '9') || c == '*' || c == '#' || c == '+' || c == WILD;
}

bool MtkPhoneNumberUtils::matchIntlPrefix(string a, int len) {
    /* '([^0-9*#+pwn]\+[^0-9*#+pwn] | [^0-9*#+pwn]0(0|11)[^0-9*#+pwn] )$' */
    /*        0       1                           2 3 45               */
    int state = 0;
    for (int i = 0; i < len; i++) {
        char c = a[i];
        switch (state) {
            case 0:
                if (c == '+')
                    state = 1;
                else if (c == '0')
                    state = 2;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 2:
                if (c == '0')
                    state = 3;
                else if (c == '1')
                    state = 4;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 4:
                if (c == '1')
                    state = 5;
                else if (isNonSeparator(c))
                    return false;
                break;
            default:
                if (isNonSeparator(c)) return false;
                break;
        }
    }
    return state == 1 || state == 3 || state == 5;
}

bool MtkPhoneNumberUtils::matchTrunkPrefix(string a, int len) {
    bool found = false;
    for (int i = 0; i < len; i++) {
        char c = a[i];
        if (c == '0' && !found) {
            found = true;
        } else if (isNonSeparator(c)) {
            return false;
        }
    }
    return found;
}

bool MtkPhoneNumberUtils::matchIntlPrefixAndCC(string a, int len) {
    /*  [^0-9*#+pwn]*(\+|0(0|11)\d\d?\d? [^0-9*#+pwn] $ */
    /*      0          1 2 3 45  6 7  8                 */
    int state = 0;
    for (int i = 0; i < len; i++) {
        char c = a[i];
        switch (state) {
            case 0:
                if (c == '+')
                    state = 1;
                else if (c == '0')
                    state = 2;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 2:
                if (c == '0')
                    state = 3;
                else if (c == '1')
                    state = 4;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 4:
                if (c == '1')
                    state = 5;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 1:
            case 3:
            case 5:
                if (isISODigit(c))
                    state = 6;
                else if (isNonSeparator(c))
                    return false;
                break;
            case 6:
            case 7:
                if (isISODigit(c))
                    state++;
                else if (isNonSeparator(c))
                    return false;
                break;
            default:
                if (isNonSeparator(c)) return false;
                break;
        }
    }
    return state == 6 || state == 7 || state == 8;
}

bool MtkPhoneNumberUtils::isNonSeparator(char c) {
    return (c >= '0' && c <= '9') || c == '*' || c == '#' || c == '+' || c == WILD || c == WAIT ||
           c == PAUSE;
}

bool MtkPhoneNumberUtils::isISODigit(char c) { return c >= '0' && c <= '9'; }
