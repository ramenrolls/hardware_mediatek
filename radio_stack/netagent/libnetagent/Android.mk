LOCAL_PATH:= $(call my-dir)

###############################
# NetAgent library
###############################

include $(CLEAR_VARS)
LOCAL_MODULE            := libnetagent
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER      := mtk
LOCAL_MODULE_TAGS       := optional
LOCAL_MULTILIB          := both

SRC                     :=
SRC                     += $(foreach dir, $(LOCAL_PATH), $(wildcard $(dir)/*.c) $(wildcard $(dir)/na/*.cpp))

LOCAL_SRC_FILES         := $(patsubst $(LOCAL_PATH)/%, %, $(SRC))
LOCAL_SRC_FILES         += ipmac/ip_mac.c
LOCAL_ARM_MODE          := arm

LOCAL_CFLAGS            += -D __ANDROID__ -O0 -g -Wno-address -Wno-unused-value -Wno-unused-parameter -Wno-ignored-qualifiers

LOCAL_SHARED_LIBRARIES  := libc libmtkcutils libnetutils libmtkrillog liblog libcutils libmtkares
LOCAL_PRELINK_MODULE    := false

LOCAL_C_INCLUDES := vendor/mediatek/ims/radio_stack/common_headers/ccci/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/log
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/utils

include $(BUILD_STATIC_LIBRARY)
