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

#ifndef __MTKPHONENUMBERUTILS_H_
#define __MTKPHONENUMBERUTILS_H_

#include <string>

using std::string;

class MtkPhoneNumberUtils {
  public:
    virtual ~MtkPhoneNumberUtils();

    /**
     * Compare phone numbers a and b, return true if they're identical
     * enough for caller ID purposes.
     *
     * - Compares from right to left
     * - requires MIN_MATCH (7) characters to match
     * - handles common trunk prefixes and international prefixes
     *   (basically, everything except the Russian trunk prefix)
     *
     * Note that this method does not return false even when the two phone numbers
     */
    static bool compareLoosely(string a, string b);

    /**
     * @return the "username" part of the specified SIP address,
     *         i.e. the part before the "@" character (or "%40").
     *
     * @param number SIP address of the form "username@domainname"
     *               (or the URI-escaped equivalent "username%40domainname")
     * @see #isUriNumber
     */
    static string getUsernameFromUriNumber(string number);

    /**
     * Check if it's special emergency number which will dial as normal call
     * but show as emergency UI.
     *
     * @return if the number is special emergency number or not
     *
     * @param number number to check
     */
    static bool isSpecialEmergencyNumber(int slotId, const char* number);

  private:
    static int indexOfLastNetworkChar(string a);
    static int minPositive(int a, int b);
    static bool isDialable(char c);
    static bool matchIntlPrefix(string a, int len);
    static bool matchTrunkPrefix(string a, int len);
    static bool matchIntlPrefixAndCC(string a, int len);
    static bool isNonSeparator(char c);
    static bool isISODigit(char c);
    static bool isCTCardType(int slot_id);

  public:
    /*
     * Special characters
     *
     * (See "What is a phone number?" doc)
     * 'p' --- GSM pause character, same as comma
     * 'n' --- GSM wild character
     * 'w' --- GSM wait character
     */
    static const char PAUSE;
    static const char WAIT;
    static const char WILD;

    //// MIN match length for CT/CTA
    static const int MIN_MATCH;
    static const int MIN_MATCH_CTA;
};

#endif /* __MTKPHONENUMBERUTILS_H_ */
