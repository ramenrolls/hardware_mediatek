LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtkconfigutils.c

LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/property
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/config

LOCAL_SHARED_LIBRARIES := \
    libmtkconfig

LOCAL_MODULE := libmtkconfigutils

LOCAL_CFLAGS := -Werror -Wall -Wextra
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
include $(BUILD_SHARED_LIBRARY)
