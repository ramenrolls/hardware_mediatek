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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
//#include <cutils/properties.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
#include <time.h>
#include <dlfcn.h>
//#include <sys/sysinfo.h>

#include "ccci_log.h"
#include "ccci_common.h"

/********************************************************************************
 * Configure region
 ********************************************************************************/
#define LK_LOAD_MD_FILE "/sys/kernel/ccci/lk_md"
#define KERNEL_SETTING_FILE "/sys/kernel/ccci/kcfg_setting"

#define KERNEL_SETTING_BUF_SIZE 4096

/****************************************************************************/
/****  lk load status check support section   *******************************/
/****************************************************************************/
int check_lk_load_md_status(int md_id) {
    int fd;
    int size;
    char* lk_info_buf;
    int ret = 0;

    // Because there is no md3 only case, check MD1 enough
    if (md_id != 1)  // Not MD_SYS1
        return 0;

    fd = open(LK_LOAD_MD_FILE, O_RDONLY);
    if (fd < 0) {
        CCCI_LOGD("Kernel load modem image case");
        return 0;
    }

    lk_info_buf = malloc(4096);
    if (lk_info_buf == NULL) {
        CCCI_LOGE("Alloc pasing memory for lk info fail");
        close(fd);
        return -1;
    }

    size = read(fd, lk_info_buf, 4096 - 1);
    if (size <= 0) {
        CCCI_LOGE("LK info read fail:%d", errno);
        ret = -1;
        goto _Exit;
    }

    lk_info_buf[size - 1] = '\0'; /* Make sure string has terminate */
    CCCI_LOGD("%s(%d)", lk_info_buf, size);

    if (strstr(lk_info_buf, "LK Load MD:[Disabled]")) {
        CCCI_LOGD("LK load modem feature disable\n");
        goto _Exit;
    }

    if (strstr(lk_info_buf, "LK load MD success!")) {
        CCCI_LOGD("LK load modem success\n");
        goto _Exit;
    }

_Exit:
    free(lk_info_buf);
    close(fd);

    return ret;
}

/****************************************************************************/
/****  Common util API section   ********************************************/
/****************************************************************************/
unsigned int str2uint(char* str) {
    /* max 32bit integer is 4294967296, buf[16] is enough */
    char buf[16];
    int i;
    int num;
    int base = 10;
    int ret_val;
    if (NULL == str) return 0;

    i = 0;
    while (i < 16) {
        /* Format it */
        if ((str[i] == 'X') || (str[i] == 'x')) {
            buf[i] = 'x';
            if (i != 1)
                return 0; /* not 0[x]XXXXXXXX */
            else if (buf[0] != '0')
                return 0; /* not [0]xXXXXXXXX */
            else
                base = 16;
        } else if ((str[i] >= '0') && (str[i] <= '9'))
            buf[i] = str[i];
        else if ((str[i] >= 'a') && (str[i] <= 'f')) {
            if (base != 16) return 0;
            buf[i] = str[i];
        } else if ((str[i] >= 'A') && (str[i] <= 'F')) {
            if (base != 16) return 0;
            buf[i] = str[i] - 'A' + 'a';
        } else if (str[i] == 0) {
            buf[i] = 0;
            i++;
            break;
        } else
            return 0;

        i++;
    }

    num = i - 1;
    ret_val = 0;
    if (base == 10) {
        for (i = 0; i < num; i++) ret_val = ret_val * 10 + buf[i] - '0';
    } else if (base == 16) {
        for (i = 2; i < num; i++) {
            if (buf[i] >= 'a')
                ret_val = ret_val * 16 + buf[i] - 'a' + 10;
            else
                ret_val = ret_val * 16 + buf[i] - '0';
        }
    }
    return ret_val;
}

/****************************************************************************/
/****  Kernel setting parse section   ***************************************/
/****************************************************************************/
static int k_setting_md_en[5];
static int k_setting_ccci_drv_ver;

int is_current_md_en(int md_id) {
    switch (md_id) {
        case 1:
            return k_setting_md_en[0];
        case 2:
            return k_setting_md_en[1];
        case 3:
            return k_setting_md_en[2];
        default:
            return 0;
    }
}

int get_ccci_drv_ver(void) { return k_setting_ccci_drv_ver; }

static int parse_k_setting(char ref_data[], int raw_size, char name[], char val[], int size) {
    int i, j = 0;
    char tmp_name[64];
    char tmp_val[64];
    int state = 0;

    CCCI_LOGV("parse_info name:%s", name);

    for (i = 0; i < raw_size; i++) {
        switch (state) {
            case 0:  // Init
                if (ref_data[i] == '[') {
                    j = 0;
                    state = 1;
                }
                break;

            case 1:  // Read name
                if (ref_data[i] == ']') {
                    state = 2;  // name parse done
                    tmp_name[j] = '\0';
                    j = 0;
                } else if ((ref_data[i] == '\r') || (ref_data[i] == '\n')) {
                    j = 0;
                    state = 0;
                } else {
                    tmp_name[j] = ref_data[i];
                    j++;
                }
                break;

            case 2:  // Get ':'
                if (ref_data[i] == ':') {
                    state = 3;  // divider found
                    tmp_val[0] = 0;
                } else
                    state = 0;  // Invalid format
                break;

            case 3:  // Read value
                if ((ref_data[i] == '\r') || (ref_data[i] == '\n')) {
                    state = 4;  // value parse done
                    tmp_val[j] = '\0';
                    j = 0;
                } else {
                    tmp_val[j] = ref_data[i];
                    j++;
                    break;
                }

            case 4:  // Check if name is match
                if (strcmp(tmp_name, name) == 0) {
                    // Copy value
                    snprintf(val, size, "%s", tmp_val);
                    return 0;
                }
                state = 0;
                break;
            default:
                break;
        }
    }
    return -1;
}

int kernel_setting_prepare(void) {
    char* ref_data_buf;
    int ref_data_len;
    char tmp_buf[64];
    int i = 0, j = 0;
    int ret = 0;
    int fd;

    ref_data_buf = (char*)malloc(KERNEL_SETTING_BUF_SIZE);

    if (ref_data_buf == NULL) {
        CCCI_LOGE("Allock kernel setting reference buffer fail, exit");
        return -1;
    }

    fd = open(KERNEL_SETTING_FILE, O_RDONLY);
    if (fd < 0) {
        CCCI_LOGE("Open ccci kernel setting file fail, exit");
        free(ref_data_buf);
        return -1;
    }

    ref_data_len = read(fd, ref_data_buf, KERNEL_SETTING_BUF_SIZE - 1);
    if (ref_data_len <= 0) {
        CCCI_LOGE("Read kernel setting info fail ret%d(%d)", ref_data_len, errno);
        ret = -1;
        goto _Exit;
    }
    ref_data_buf[ref_data_len - 1] = 0;

    if (parse_k_setting(ref_data_buf, ref_data_len, "modem en", tmp_buf, 64) < 0) {
        CCCI_LOGV("parsing modem en fail, exit");
        ret = -1;
        goto _Exit;
    }
    for (i = 0; i < 5; i++) k_setting_md_en[i] = 0;
    i = 0;
    while (1) {
        switch (tmp_buf[j]) {
            case '\0':
                break;
            case '-':
                break;
            case '1':
                k_setting_md_en[i] = 1;
                i++;
                break;
            case '0':
                i++;
                break;
            default:
                break;
        }
        if (i >= 5) break;
        j++;
    }

    if (parse_k_setting(ref_data_buf, ref_data_len, "ccci_drv_ver", tmp_buf, 64) < 0) {
        CCCI_LOGV("parsing ccci_drv_ver fail, exit");
        ret = -1;
        goto _Exit;
    }
    if (strcmp("V2", tmp_buf) == 0)
        k_setting_ccci_drv_ver = 2; /* using v2 code */
    else
        k_setting_ccci_drv_ver = 1; /* using v1 code */

_Exit:
    close(fd);
    free(ref_data_buf);
    return ret;
}
