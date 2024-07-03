LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtk_log.c

LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include/log

LOCAL_SHARED_LIBRARIES := liblog

LOCAL_MODULE := libmtkrillog
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_CFLAGS := -Werror -Wall -Wextra
include $(BUILD_SHARED_LIBRARY)
