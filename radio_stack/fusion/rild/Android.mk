ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
  rild.c

LOCAL_SHARED_LIBRARIES := \
  libmtkrillog \
  libmtkproperty \
  libmtkcutils \
  librilfusion \
  libmtkrilutils \
  libsysenv \
  libmtkconfigutils

LOCAL_CFLAGS := -DRIL_SHLIB

LOCAL_CFLAGS += -DMTK_SYSENV_SUPPORT
LOCAL_SHARED_LIBRARIES += liblog

ifneq ($(MTK_NUM_MODEM_PROTOCOL),1)
    LOCAL_CFLAGS += -DANDROID_MULTI_SIM
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 2)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_2
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 3)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_3
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 4)
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_4
endif

LOCAL_CFLAGS += -D__PRODUCTION_RELEASE__

LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
LOCAL_CFLAGS += -DMTK_IMS_CHANNEL_SUPPORT
LOCAL_CFLAGS += -DMTK_USE_HIDL
LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/librilfusion \
    vendor/mediatek/ims/include/libsysenv \
    vendor/mediatek/ims/radio_stack/platformlib/include/log \
    vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/utils \
    vendor/mediatek/ims/radio_stack/platformlib/include/config

LOCAL_C_INCLUDES += vendor/mediatek/ims/include/ril/include

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE:= mtkfusionrild
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MODULE_TAGS := optional
LOCAL_INIT_RC := mtkrild.rc

LOCAL_MULTILIB := first

include $(BUILD_EXECUTABLE)

endif
