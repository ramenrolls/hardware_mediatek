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

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtk_log.h>
#include <mtk_properties.h>

#include "ratconfig.h"

#undef LOG_TAG
#define LOG_TAG "RatConfigC"

#define TRUE 1
#define FALSE 0

/** the block of defination
    must be sync with
    RatConfiguration.java **/
#define PROPERTY_BUILD_RAT_CONFIG "ro.vendor.mtk_protocol1_rat_config"
#define PROPERTY_RAT_CONFIG "ro.vendor.mtk_ps1_rat"
#define PROPERTY_IS_USING_DEFAULT_CONFIG "ro.boot.opt_using_default"

#define CDMA "C"
#define LteFdd "Lf"
#define LteTdd "Lt"
#define WCDMA "W"
#define TDSCDMA "T"
#define GSM "G"
#define NR "N"
#define DELIMITER "/"

#define MASK_NR (1 << 6)
#define MASK_CDMA (1 << 5)
#define MASK_LteFdd (1 << 4)
#define MASK_LteTdd (1 << 3)
#define MASK_WCDMA (1 << 2)
#define MASK_TDSCDMA (1 << 1)
#define MASK_GSM (1)
/*************************/

static unsigned int max_rat = 0x0;
static int max_rat_initialized = FALSE;
static unsigned int actived_rat = 0x0;
static int is_default_config = 1;

static unsigned int ratToBitmask(const char* rat);
static unsigned int getMaxRat();
static int checkRatConfig(unsigned int iRat);
static unsigned int getRatConfig();

/*
 * transfer the rat from "L/T/G" to bitmask.
 * @return unsigned int, rat in bitmask.
 */
static unsigned int ratToBitmask(const char* rat) {
    unsigned int iRat = 0x0;
    if (strstr(rat, CDMA) != NULL) {
        iRat |= MASK_CDMA;
    }
    if (strstr(rat, LteFdd) != NULL) {
        iRat |= MASK_LteFdd;
    }
    if (strstr(rat, LteTdd) != NULL) {
        iRat |= MASK_LteTdd;
    }
    if (strstr(rat, WCDMA) != NULL) {
        iRat |= MASK_WCDMA;
    }
    if (strstr(rat, TDSCDMA) != NULL) {
        iRat |= MASK_TDSCDMA;
    }
    if (strstr(rat, GSM) != NULL) {
        iRat |= MASK_GSM;
    }
    if (strstr(rat, NR) != NULL) {
        iRat |= MASK_NR;
    }
    return iRat;
}

/*
 * get the rat of project config
 * @return int, the rat of project config in bitmask.
 */
static unsigned int getMaxRat() {
    if (!max_rat_initialized) {
        char rat[MTK_PROPERTY_VALUE_MAX];
        int mIsDefault = 1;
        mtk_property_get(PROPERTY_BUILD_RAT_CONFIG, rat, "");
        mtkLogD(LOG_TAG, "getMaxRat: initial %s", rat);
        max_rat = ratToBitmask(rat);
        mtk_property_get(PROPERTY_IS_USING_DEFAULT_CONFIG, rat, "1");
        mIsDefault = atoi(rat);
        if (mIsDefault != 0) {
            // is_using_default is not 0, using default config.
            is_default_config = 1;
        } else {
            is_default_config = 0;
        }
        max_rat_initialized = TRUE;
    }
    return max_rat;
}

/*
 * check the iRat is supported by project config
 * @params unsinged int iRat, the rat to be check.
 * @return int,
 *           1, supported.
 *           0, not supported.
 */
static int checkRatConfig(unsigned int iRat) {
    unsigned int maxrat = 0x0;
    maxrat = getMaxRat();
    if ((iRat | maxrat) == maxrat) {
        return TRUE;
    } else {
        mtkLogD(LOG_TAG, "checkRatConfig: not supported (%X/%X)", iRat, maxrat);
        return FALSE;
    }
}

/*
 * get the active rat with the bitmask format
 * @return unsigned int, the active rat in bitmask.
 */
static unsigned int getRatConfig() {
    char rat[MTK_PROPERTY_VALUE_MAX] = "";
    unsigned int default_rat_config = getMaxRat();
    if (default_rat_config == 0) {
        actived_rat = 0;
        return actived_rat;
    }
    if (is_default_config) {
        actived_rat = default_rat_config;
        return actived_rat;
    }
    mtk_property_get(PROPERTY_RAT_CONFIG, rat, "");
    if (strlen(rat) > 0) {
        actived_rat = ratToBitmask(rat);
        if (checkRatConfig(actived_rat) == FALSE) {
            mtkLogD(LOG_TAG, "getRatConfig: illegal value of PROPERTY_RAT_CONFIG, set max");
            actived_rat = getMaxRat();
        }
    } else {
        mtkLogD(LOG_TAG, "getRatConfig: active rat is not exist, initialize");
        actived_rat = getMaxRat();
    }
    return actived_rat;
}

/*
 * check C2k suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isC2kSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_CDMA) == MASK_CDMA ? TRUE : FALSE;
    return result;
}

/*
 * check LteFdd suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isLteFddSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_LteFdd) == MASK_LteFdd ? TRUE : FALSE;
    return result;
}

/*
 * check LteTdd suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isLteTddSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_LteTdd) == MASK_LteTdd ? TRUE : FALSE;
    return result;
}

/*
 * check Wcdma suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isWcdmaSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_WCDMA) == MASK_WCDMA ? TRUE : FALSE;
    return result;
}

/*
 * check Tdscdma suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isTdscdmaSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_TDSCDMA) == MASK_TDSCDMA ? TRUE : FALSE;
    return result;
}

/*
 * check GSM suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isGsmSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_GSM) == MASK_GSM ? TRUE : FALSE;
    return result;
}

/*
 * check NR suppport
 * @return int, cases as following
 *       1, rat is active and project config supports it.
 *       0, rat is inactive no matter project config supports.
 */
int RatConfig_isNrSupported() {
    int result = (getMaxRat() & getRatConfig() & MASK_NR) == MASK_NR ? TRUE : FALSE;
    return result;
}
