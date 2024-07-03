ifeq ($(MTK_RIL_MODE), c6m_1rild)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    libril/rilop_service.cpp \
    libril/rilop.cpp \
    mtk-ril/framework/core/RfxOpControllerFactory.cpp \
    mtk-ril/telcore/oem/RtcOpOemController.cpp \
    mtk-ril/mdcomm/power/RmcOpRadioRequestHandler.cpp \
    mtk-ril/telcore/client/RilOpOemClient.cpp \
    mtk-ril/telcore/client/RtcOpRilClientController.cpp \
    mtk-ril/telcore/data/RtcOpDataController.cpp \
    mtk-ril/telcore/nw/RtcOpRatSwitchController.cpp \
    mtk-ril/telcore/sim/RtcCommSimOpController.cpp \
    mtk-ril/mdcomm/sim/RmcGsmSimOpRequestHandler.cpp \
    mtk-ril/mdcomm/sim/RmcCdmaSimOpRequestHandler.cpp \
    mtk-ril/mdcomm/sim/RmcCommSimOpRequestHandler.cpp \
    mtk-ril/mdcomm/sim/RmcGsmSimOpUrcHandler.cpp \
    mtk-ril/mdcomm/sim/RmcCdmaSimOpUrcHandler.cpp \
    mtk-ril/mdcomm/sim/RmcCommSimOpUrcHandler.cpp \
    mtk-ril/telcore/ims/RtcOpImsController.cpp \
    mtk-ril/mdcomm/ims/RmcOpImsControlRequestHandler.cpp \
    mtk-ril/mdcomm/ims/RmcOpImsControlUrcHandler.cpp \
    mtk-ril/mdcomm/nw/RmcOpNetworkRequestHandler.cpp \
    mtk-ril/mdcomm/nw/RmcOpNetworkUrcHandler.cpp \
    mtk-ril/telcore/cc/RtcOpCallController.cpp \
    mtk-ril/mdcomm/cc/RmcOpCallControlImsRequestHandler.cpp \
    mtk-ril/framework/base/RfxDialFromData.cpp \
    mtk-ril/framework/base/RfxOpRsuRequestData.cpp \
    mtk-ril/framework/base/RfxOpRsuResponseData.cpp \
    mtk-ril/telcore/ss/RtcOpSuppServController.cpp \
    mtk-ril/mdcomm/ss/RmcOpSuppServRequestHandler.cpp \
    mtk-ril/mdcomm/nw/RmcOpRatSwitchHandler.cpp \
    mtk-ril/mdcomm/cc/RmcOpCallControlCommonRequestHandler.cpp \
    mtk-ril/mdcomm/data/RmcOpDcImsReqHandler.cpp \
    mtk-ril/telcore/cc/RtcOpRedialController.cpp \
    mtk-ril/mdcomm/cc/RmcOpCallControlUrcHandler.cpp\
    mtk-ril/mdcomm/sms/RmcOpSCBMUrcHandler.cpp\
    mtk-ril/mdcomm/sms/RmcOpSCBMRequestHandler.cpp\
    mtk-ril/telcore/capabilityswitch/RtcOpCapabilitySwitchChecker.cpp

LOCAL_SHARED_LIBRARIES := \
    libmtkrillog \
    libmtkutils \
    libmtkcutils \
    libmtkhardware_legacy \
    libmtkrilutils \
    librilfusion \
    libmtk-ril \
    libratconfig

#use android binder, HIDL
LOCAL_CFLAGS += -DMTK_USE_HIDL
LOCAL_SHARED_LIBRARIES += libhwbinder libhidlbase libhidltransport \
    android.hardware.radio@1.0 \
    vendor.mediatek.hardware.radio_op@2.0 \
    vendor.mediatek.hardware.radio_op@1.2 \
    vendor.mediatek.hardware.radio_op@1.3 \
    vendor.mediatek.hardware.radio_op@1.4

LOCAL_STATIC_LIBRARIES := \
    libprotobuf-c-nano-enable_malloc

LOCAL_CFLAGS += -Wno-unused-parameter
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

ifeq ($(MTK_TC1_FEATURE),yes)
    LOCAL_CFLAGS += -DMTK_TC1_FEATURE
endif

LOCAL_CFLAGS += -DMTK_OPERATOR_ADDON

LOCAL_C_INCLUDES += external/nanopb-c \
        frameworks/native/include/binder \
        vendor/mediatek/ims/radio_stack/include \
        vendor/mediatek/ims/radio_stack/fusion/include \
        vendor/mediatek/ims/radio_stack/fusion/include/telephony \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/ \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/capabilityswitch \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/oem \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/sim \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/base \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/core \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/port/android/include \
        vendor/mediatek/ims/radio_stack/fusion/libril \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/data \
        vendor/mediatek/ims/radio_stack/netagent/libnetagent/na \
        vendor/mediatek/ims/include/ril/include/telephony \
        vendor/mediatek/ims/radio_stack/fusion/include/telephony \
        vendor/mediatek/ims/radio_stack/fusion/include/telephony \
        vendor/mediatek/ims/radio_stack/fusion/libril \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/core \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/framework/include/base \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/capabilityswitch \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/oem \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/sim \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/sim \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/ims \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/ims \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/nw \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/cc \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/cc \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/ss \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/mdcomm/ss \
        vendor/mediatek/ims/radio_stack/common_headers/ccci/include \
        vendor/mediatek/ims/radio_stack/platformlib/include/mtkrilutils \
        vendor/mediatek/ims/radio_stack/platformlib/include/log \
        vendor/mediatek/ims/radio_stack/platformlib/include/property \
        vendor/mediatek/ims/radio_stack/platformlib/include \
        vendor/mediatek/ims/radio_stack/platformlib/include/utils \
        vendor/mediatek/ims/radio_stack/platformlib/include/mtkrilutils \
        vendor/mediatek/ims/radio_stack/platformlib/include/tinyxml \
        vendor/mediatek/ims/radio_stack/fusion/mtk-ril/telcore/client \
        vendor/mediatek/ims/radio_stack/operator/fusion/libril \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/framework/include \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/framework/include/base \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/framework/include/core \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/mdcomm \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/mdcomm/sim \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/sim \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/ims \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/cc \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/capabilityswitch \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/oem \
        vendor/mediatek/ims/radio_stack/operator/fusion/mtk-ril/telcore/ss

LOCAL_CFLAGS += -DMTK_MUX_CHANNEL_64
LOCAL_CFLAGS += -DMTK_USE_HIDL
LOCAL_CFLAGS += -DMTK_IMS_CHANNEL_SUPPORT
LOCAL_CFLAGS += -DMTK_TELEPHONYWARE_SUPPORT

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/../include

LOCAL_MULTILIB := first

LOCAL_MODULE:= libmtk-rilop
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_CLANG := true
LOCAL_SANITIZE := integer

include $(BUILD_SHARED_LIBRARY)

endif
