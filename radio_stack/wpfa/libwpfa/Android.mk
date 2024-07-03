LOCAL_PATH:= $(call my-dir)

###############################
# Wpfa library
###############################

include $(CLEAR_VARS)
LOCAL_MODULE            := libwpfa
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER      := mtk
LOCAL_MODULE_TAGS       := optional
LOCAL_MULTILIB          := first


SRC                     :=
SRC                     += $(foreach dir, $(LOCAL_PATH), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/wpfaDriver/*.cpp))

LOCAL_SRC_FILES         := $(patsubst $(LOCAL_PATH)/%, %, $(SRC))

LOCAL_ARM_MODE          := arm

LOCAL_CFLAGS            += -D __ANDROID__ -O0 -g -Wno-address -Wno-unused-value -Wno-unused-parameter -Wno-ignored-qualifiers

LOCAL_SHARED_LIBRARIES  := libmtkcutils libmtkrillog libmtknetcap libmtkutils libccci_util
LOCAL_PRELINK_MODULE    := false

LOCAL_HEADER_LIBRARIES := libMtkLogHeaders libWpfaHeaders libUtilsHeaders libMtkRilUtilsHeaders libNetCapHeaders

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
