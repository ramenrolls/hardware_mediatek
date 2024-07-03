LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= gsm0710muxd
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MULTILIB := 32
LOCAL_INIT_RC := gsm0710muxd.rc

LOCAL_SRC_FILES:= \
    src/gsm0710muxd.c \
    src/gsm0710muxd_fc.c

LOCAL_SHARED_LIBRARIES := \
    libmtkcutils libmtkrillog libmtkproperty

LOCAL_CFLAGS := \
    -DMUX_ANDROID \
    -D__CCMNI_SUPPORT__ \
    -D__MUXD_FLOWCONTROL__

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 2)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_2
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 3)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_3
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 4)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_4
endif

ifeq ($(MTK_MUX_CHANNEL), 64)
    LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
endif

LOCAL_CFLAGS += -D__PRODUCTION_RELEASE__

ifeq ($(MTK_VT3G324M_SUPPORT),yes)
    LOCAL_CFLAGS += -D__ANDROID_VT_SUPPORT__
endif

ifeq ($(MTK_RIL_MODE), c6m_1rild)
    LOCAL_CFLAGS += -DFUSION_RILD
endif

LOCAL_C_INCLUDES := \
    vendor/mediatek/ims/radio_stack/common_headers/ccci/include \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include/log

include $(BUILD_EXECUTABLE)
