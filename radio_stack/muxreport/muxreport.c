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

#include <errno.h>
#include <fcntl.h>
#include <paths.h>

#define LOG_TAG "RIL_Mux"

#include "mtk_log.h"
#include "mtk_properties.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "hardware/ccci_intf.h"

#ifdef ALOGV
#define LOGV ALOGV
#endif
#ifdef ALOGD
#define LOGD ALOGD
#endif
#ifdef ALOGE
#define LOGE ALOGE
#endif
#ifdef ALOGI
#define LOGI ALOGI
#endif
#ifdef ALOGW
#define LOGW ALOGW
#endif

#define EXT_MD_IOC_MAGIC 'E'
#define EXT_MD_IOCTL_LET_MD_GO _IO(EXT_MD_IOC_MAGIC, 1)
#define EXT_MD_IOCTL_REQUEST_RESET _IO(EXT_MD_IOC_MAGIC, 2)
#define EXT_MD_IOCTL_POWER_ON_HOLD _IO(EXT_MD_IOC_MAGIC, 3)

#define RESET_MODE_ASSERT 1
#define RESET_MODE_RESET 2
#define STOP_MD 3
#define START_MD 4
#define RESET_MODE_ASSERT_MD2 5
#define RESET_MODE_RESET_MD2 6
#define STOP_MD2 7
#define START_MD2 8

#define EXT_MD_IOC_CHANNEL "/dev/ext_md_ctl2"

#if defined(PURE_AP_USE_EXTERNAL_MODEM)
#define EXT_MD_IOC_MAGIC 'E'
#define EXT_MD_IOCTL_LET_MD_GO _IO(EXT_MD_IOC_MAGIC, 1)
#define EXT_MD_IOCTL_REQUEST_RESET _IO(EXT_MD_IOC_MAGIC, 2)
#define EXT_MD_IOCTL_POWER_ON_HOLD _IO(EXT_MD_IOC_MAGIC, 3)
#define EXT_MD_IOCTL_POWER_ON _IO(EXT_MD_IOC_MAGIC, 100)
#define EXT_MD_IOCTL_POWER_OFF _IO(EXT_MD_IOC_MAGIC, 102)
#define EXT_MD_IOCTL_RESET _IO(EXT_MD_IOC_MAGIC, 103)
#define EXT_MD_IOCTL_R8_TO_PC _IO(EXT_MD_IOC_MAGIC, 104)
#define EXT_MD_IOCTL_R8_TO_AP _IO(EXT_MD_IOC_MAGIC, 105)
#define EXT_MD_IOCTL_R8_DOWNLOAD _IO(EXT_MD_IOC_MAGIC, 106)

#define EXT_MD_MONITOR_DEV "/dev/ext_md_ctl0"
#endif

int isMD1Enabled() {
    int rlt = 0;
    char propperty_value[MTK_PROPERTY_VALUE_MAX] = {0};

    mtk_property_get("ro.vendor.mtk_md1_support", propperty_value, "0");
    rlt = atoi(propperty_value);
    if (rlt > 0) {
        return 1;
    } else {
        return 0;
    }
}

int isMD2Enabled() {
    int rlt = 0;
    char propperty_value[MTK_PROPERTY_VALUE_MAX] = {0};

    mtk_property_get("ro.vendor.mtk_md2_support", propperty_value, "0");
    rlt = atoi(propperty_value);
    if (rlt > 0) {
        return 1;
    } else {
        return 0;
    }
}

int isMD5Enabled() {
    int rlt = 0;
    char propperty_value[MTK_PROPERTY_VALUE_MAX] = {0};

    mtk_property_get("ro.vendor.mtk_md5_support", propperty_value, "0");
    rlt = atoi(propperty_value);
    if (rlt > 0) {
        return 1;
    } else {
        return 0;
    }
}

int ctl_modem(int request, int index) {
    int ret_ioctl_val = -1;
    int reset_parm = -1;
    int ccci_sys_fd = -1;
    char dev_node[32] = {0};

    switch (index) {
        case RESET_MODE_ASSERT:
        case RESET_MODE_RESET:
        case STOP_MD:
        case START_MD:
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            snprintf(dev_node, 32, "%s", EXT_MD_IOC_CHANNEL);
            mtkLogD(LOG_TAG, "Extern modem IOCTL [%s, %d, %d]", dev_node, request, index);
#else
            if (isMD1Enabled()) {
                snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_MUXR_IOCTL, MD_SYS1));
                mtkLogD(LOG_TAG, "[muxreport] MD1 IOCTL [%s, %d, %d]", dev_node, request, index);
            } else if (isMD5Enabled()) {
                snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_MUXR_IOCTL, MD_SYS5));
                mtkLogD(LOG_TAG, "[muxreport] MD5 IOCTL [%s, %d, %d]", dev_node, request, index);
            } else {
                mtkLogD(LOG_TAG, "[muxreport] Invalid MD option! request: %d, index: %d", request,
                        index);
                return -1;
            }
#endif
            break;
        case RESET_MODE_ASSERT_MD2:
        case RESET_MODE_RESET_MD2:
        case STOP_MD2:
        case START_MD2:
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            snprintf(dev_node, 32, "%s", EXT_MD_IOC_CHANNEL);
            mtkLogD(LOG_TAG, "Extern modem IOCTL [%s, %d, %d]", dev_node, request, index);
#else
            if (isMD2Enabled()) {
                snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_MUXR_IOCTL, MD_SYS2));
                mtkLogD(LOG_TAG, "[muxreport] MD2 IOCTL [%s, %d, %d]", dev_node, request, index);
            } else if (isMD5Enabled()) {
                snprintf(dev_node, 32, "%s", ccci_get_node_name(USR_MUXR_IOCTL, MD_SYS5));
                mtkLogD(LOG_TAG, "[muxreport] MD5 IOCTL [%s, %d, %d]", dev_node, request, index);
            } else {
                mtkLogD(LOG_TAG, "[muxreport] Invalid MD option! request: %d, index: %d", request,
                        index);
                return -1;
            }
#endif
            break;
        default:
            mtkLogD(LOG_TAG, "[muxreport] Not supported request and index [%d, %d]", request,
                    index);
            break;
    }
    ccci_sys_fd = open(dev_node, O_RDWR | O_NONBLOCK);

    if (ccci_sys_fd < 0) {
        mtkLogD(LOG_TAG, "id=%d with index=%d, errno: %d", ccci_sys_fd, index, errno);
        mtkAssert("ASSERT!!!!");
    }

    ret_ioctl_val = ioctl(ccci_sys_fd, request, &reset_parm);
    mtkLogD(LOG_TAG, "ret_val=%d with index=%d", ret_ioctl_val, index);

    close(ccci_sys_fd);
    return ret_ioctl_val;
}

int main(int argc, char* argv[]) {
    char propperty_value[MTK_PROPERTY_VALUE_MAX] = {0};
    int usage_case = 0;

    if (argc > 1) {
        usage_case = atoi(argv[1]);
    } else {
        mtk_property_get("vendor.ril.mux.report.case", propperty_value, NULL);
        usage_case = atoi(propperty_value);
    }

    mtkLogD(LOG_TAG, "[muxreport] Invoke mux app mechanism with case=%d\n", usage_case);
    printf("Invoke mux app mechanism with case=%d\n", usage_case);
    if (usage_case == RESET_MODE_ASSERT || usage_case == RESET_MODE_RESET ||
        usage_case == RESET_MODE_ASSERT_MD2 || usage_case == RESET_MODE_RESET_MD2) {
        // blocking the TRM runs more thane once
        mtk_property_get("vendor.ril.muxreport.run", propperty_value, "0");
        if (strcmp(propperty_value, "1") == 0) {
            mtkLogE(LOG_TAG, "[muxreport] TRM is still running\n");
            return 0;
        }
        mtk_property_set("vendor.ril.muxreport.run", "1");
    }
    mtkLogD(LOG_TAG, "MD info: MD1:%d MD2:%d MD5:%d\n", isMD1Enabled(), isMD2Enabled(),
            isMD5Enabled());
    if (isMD1Enabled() == 0 && isMD2Enabled() == 0 && isMD5Enabled() == 0) {
#if !defined(PURE_AP_USE_EXTERNAL_MODEM)
        mtkLogD(LOG_TAG,
                "[muxreport] Invoke mux app mechanism not supported due for WIFI only projects");
        return 0;
#endif
    }

    switch (usage_case) {
        case RESET_MODE_ASSERT:
            ctl_modem(CCCI_IOC_FORCE_MD_ASSERT, usage_case);
            break;
        case RESET_MODE_RESET:
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            ctl_modem(EXT_MD_IOCTL_RESET, usage_case);
#else
            ctl_modem(CCCI_IOC_MD_RESET, usage_case);
#endif
            break;
        case STOP_MD:
        case STOP_MD2:
            printf("Stop modem\n");
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            ctl_modem(EXT_MD_IOCTL_POWER_OFF, usage_case);
#else
            ctl_modem(CCCI_IOC_SEND_STOP_MD_REQUEST, usage_case);
#endif
            break;
        case START_MD:
        case START_MD2:
            printf("Start modem\n");
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            ctl_modem(EXT_MD_IOCTL_POWER_ON, usage_case);
#else
            ctl_modem(CCCI_IOC_SEND_START_MD_REQUEST, usage_case);
#endif
            break;
        case RESET_MODE_ASSERT_MD2:
            // only support internal md2
            if (isMD2Enabled() || isMD5Enabled()) {
                ctl_modem(CCCI_IOC_FORCE_MD_ASSERT, usage_case);
            } else {
                mtkLogD(LOG_TAG, "[muxreport] Not support mux app mechanism with case=%d\n",
                        usage_case);
                printf("Not support mux app mechanism with case=%d\n", usage_case);
            }
            break;
        case RESET_MODE_RESET_MD2:
            mtk_property_set("vendor.ril.mux.report.case", "6");  // silent reboot
#if defined(PURE_AP_USE_EXTERNAL_MODEM)
            ctl_modem(EXT_MD_IOCTL_REQUEST_RESET, usage_case);
#else
            ctl_modem(CCCI_IOC_MD_RESET, usage_case);
#endif
            break;

        default:
            if (argc > 1) printf("Invalid muxreport with case=%d", usage_case);
            mtkLogD(LOG_TAG, "[muxreport] Invalid muxreport with case=%d", usage_case);
            break;
    }
    return 0;
}
