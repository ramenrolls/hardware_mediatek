LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    libmtkrilutils.c \
    mtk_rs.c \
    librilutils.c \
    record_stream.c \
    proto/sap-api.proto \
    MtkPhoneNumberUtils.cpp

LOCAL_SHARED_LIBRARIES := \
    libmtkcutils libmtkrillog libmtkproperty libratconfig libmtkconfigutils

ifneq ($(MTK_NUM_MODEM_PROTOCOL),1)
    LOCAL_CFLAGS += -DMD_PS_COUNT
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 2)
    LOCAL_CFLAGS += -DMD_PS_COUNT_2
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 3)
    LOCAL_CFLAGS += -DMD_PS_COUNT_3
endif

ifeq ($(MTK_NUM_MODEM_PROTOCOL), 4)
    LOCAL_CFLAGS += -DMD_PS_COUNT_4
endif

LOCAL_CFLAGS += -D__PRODUCTION_RELEASE__

ifeq ($(MTK_RIL_MODE), c6m_1rild)
    LOCAL_CFLAGS += -DFUSION_RIL
endif

# remove the below if not support libcrypto
LOCAL_CFLAGS += -DMTK_SHA_SUPPORT
LOCAL_SHARED_LIBRARIES += libcrypto

#used to check if support telephonyware
LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

LOCAL_C_INCLUDES += \
    vendor/mediatek/ims/include/ril/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/log \
    vendor/mediatek/ims/radio_stack/platformlib/include/property \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/utils \
    vendor/mediatek/ims/radio_stack/platformlib/include/mtkrilutils \
    vendor/mediatek/ims/radio_stack/platformlib/include/config

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    vendor/mediatek/ims/include/ril/include

LOCAL_PROTOC_OPTIMIZE_TYPE := nanopb-c-enable_malloc

# If not declare LOCAL_MULTILIB, it will build both 32/64 bit lib.
# In c6m_rild 64bit project, all modules that use libmtkrilutils are
# 64bit, it is not necessary to build 32bit libmtkrilutils.
ifeq ($(MTK_RIL_MODE), c6m_1rild)
    LOCAL_MULTILIB := first
endif

LOCAL_MODULE:= libmtkrilutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk

include $(BUILD_SHARED_LIBRARY)
