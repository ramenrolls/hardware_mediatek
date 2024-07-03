ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_VENDOR_MODULE := true

LOCAL_SRC_FILES:= \
    carrierconfig.c

LOCAL_SHARED_LIBRARIES:= \
    libmtkcutils

LOCAL_CFLAGS += -D__DIMS_INTERNAL__

LOCAL_C_INCLUDES = vendor/mediatek/ims/include/ril/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../mtk-ril/framework/include

LOCAL_EXPORT_C_INCLUDE_DIRS := vendor/mediatek/ims/include/ril/include

LOCAL_MODULE:= libcarrierconfig
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_CLANG := true
LOCAL_SANITIZE := integer

include $(BUILD_SHARED_LIBRARY)

endif
