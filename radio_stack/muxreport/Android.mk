LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= muxreport
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_INIT_RC := muxreport.rc

LOCAL_SRC_FILES:= \
    muxreport.c

LOCAL_SHARED_LIBRARIES := \
    libmtkrillog \
    libmtkproperty

LOCAL_CFLAGS := -DANDROID

LOCAL_C_INCLUDES := \
    vendor/mediatek/ims/radio_stack/common_headers/ccci/include \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include/log

include $(BUILD_EXECUTABLE)
