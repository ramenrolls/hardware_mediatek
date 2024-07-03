ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    ViaHandler.cpp

LOCAL_SHARED_LIBRARIES := \
    libmtkcutils libmtkutils libmtk-ril libmtkproperty libmtkrillog libmtkrilutils

LOCAL_C_INCLUDES += vendor/mediatek/ims/include/ril/include \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/core \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/base \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/port/android/include \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/ \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore \
    vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm \
    vendor/mediatek/ims/radio_stack/common_headers/ccci/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include/log \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/utils

# for asprinf
LOCAL_CFLAGS := -D_GNU_SOURCE

LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
LOCAL_CFLAGS += -DMTK_IMS_CHANNEL_SUPPORT
LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

# build shared library
LOCAL_CFLAGS += -DRIL_SHLIB
LOCAL_MODULE:= libvia-ril
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MULTILIB := first
include $(BUILD_SHARED_LIBRARY)

endif
