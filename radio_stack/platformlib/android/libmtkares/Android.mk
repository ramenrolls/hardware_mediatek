LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtk_ares.c

LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include/ares

LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/libc-ares-naptr

LOCAL_CXXFLAGS += -DHAVE_CONFIG_H -DNAPTR

LOCAL_SHARED_LIBRARIES := libcares_naptr

LOCAL_MODULE := libmtkares
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_CFLAGS := -Werror -Wall -Wextra
include $(BUILD_SHARED_LIBRARY)
