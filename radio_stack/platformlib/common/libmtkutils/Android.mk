LOCAL_PATH:= $(call my-dir)

commonSources:= \
    CallStack.cpp \
    JenkinsHash.cpp \
    LinearTransform.cpp \
    NativeHandle.cpp \
    Printer.cpp \
    RefBase.cpp \
    SharedBuffer.cpp \
    Static.cpp \
    StopWatch.cpp \
    String8.cpp \
    String16.cpp \
    Threads.cpp \
    Timers.cpp \
    Unicode.cpp \
    VectorImpl.cpp \
    Parcel.cpp \
    SystemClock.cpp \

host_commonCflags := -DLIBMTKUTILS_NATIVE=1 $(TOOL_CFLAGS) -Werror

# For the device, static
# =====================================================
include $(CLEAR_VARS)

# we have the common sources, plus some device-specific stuff
LOCAL_SRC_FILES:= \
    $(commonSources) \
    BlobCache.cpp \
    Looper.cpp \
    ProcessCallStack.cpp \

ifeq ($(TARGET_ARCH),mips)
LOCAL_CFLAGS += -DALIGN_DOUBLE
endif
LOCAL_CFLAGS += -Werror -fvisibility=protected

LOCAL_SHARED_LIBRARIES := \
    libmtkcutils \
    libmtkproperty \
    libmtkrillog \
    libladder

LOCAL_MODULE := libmtkutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_CLANG := true
LOCAL_SANITIZE := integer
LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/utils
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/log
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/property
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/iop

# Move libladder uses from here
LOCAL_C_INCLUDES += vendor/mediatek/opensource/external/libudf/libladder

include $(BUILD_STATIC_LIBRARY)

# For the device, shared
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE:= libmtkutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_WHOLE_STATIC_LIBRARIES := libmtkutils
LOCAL_SHARED_LIBRARIES := \
        libmtkcutils \
        libmtkproperty \
        libmtkrillog \
        libladder

LOCAL_CFLAGS := -Werror
LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/platformlib/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/utils
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/log
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/property
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/iop

# Move libladder uses from here
LOCAL_C_INCLUDES += vendor/mediatek/opensource/external/libudf/libladder

# If not declare LOCAL_MULTILIB, it will build both 32/64 bit lib.
# In c6m_rild 64bit project, all modules that use libmtkutils are
# 64bit, it is not necessary to build 32bit libmtkrilutils.
ifeq ($(MTK_RIL_MODE), c6m_1rild)
    LOCAL_MULTILIB := first
endif


LOCAL_CLANG := true
LOCAL_SANITIZE := integer

include $(BUILD_SHARED_LIBRARY)
