LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtkconfig.c

LOCAL_C_INCLUDES += \
    vendor/mediatek/ims/radio_stack/platformlib/include/config \
    vendor/mediatek/ims/radio_stack/platformlib/include/log \
    vendor/mediatek/ims/radio_stack/platformlib/include/property

LOCAL_SHARED_LIBRARIES := \
    libmtkrillog

LOCAL_MODULE := libmtkconfig
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_CFLAGS := -Werror -Wall -Wextra -Wno-sign-compare
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
include $(BUILD_SHARED_LIBRARY)
