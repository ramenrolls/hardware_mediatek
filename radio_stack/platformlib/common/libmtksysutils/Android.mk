LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    FrameworkClient.cpp \
    FrameworkCommand.cpp \
    FrameworkListener.cpp \
    NetlinkEvent.cpp \
    NetlinkListener.cpp \
    ServiceManager.cpp \
    SocketClient.cpp \
    SocketListener.cpp

LOCAL_C_INCLUDES += \
    vendor/mediatek/ims/radio_stack/platformlib/include \
    vendor/mediatek/ims/radio_stack/platformlib/include/property

LOCAL_SHARED_LIBRARIES := \
  libmtkrillog \
  libmtkproperty \
  libmtkcutils

LOCAL_PROTOC_OPTIMIZE_TYPE := nanopb-c-enable_malloc

LOCAL_MODULE:= libmtksysutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk

include $(BUILD_SHARED_LIBRARY)
