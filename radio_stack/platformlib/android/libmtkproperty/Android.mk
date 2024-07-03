LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    mtk_properties.c

LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include/log \
    vendor/mediatek/ims/radio_stack/platformlib/include/config

LOCAL_SHARED_LIBRARIES := libmtkrillog

LOCAL_MODULE := libmtkproperty
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_CFLAGS := -Werror -Wall -Wextra
include $(BUILD_SHARED_LIBRARY)
