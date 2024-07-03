LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := \
		   src/setkey_fileio/setkey_fileio.c  \
		   src/setkey_fileio/rtnl_socket.c  \
		   src/setkey_fileio/setkey_xfrm.c  \
		   src/setkey_fileio/setkey_xfrm_parse.c  \
		   src/setkey_fileio/utils_xfrm.c

LOCAL_C_INCLUDES += \
		   $(LOCAL_PATH) \
		   $(LOCAL_PATH)/src/setkey_fileio

LOCAL_HEADER_LIBRARIES := liblog_headers
LOCAL_SHARED_LIBRARIES := libcutils libcrypto
LOCAL_CFLAGS := -DANDROID_CHANGES -DHAVE_CONFIG_H
LOCAL_CFLAGS += -Wno-sign-compare -Wno-missing-field-initializers
LOCAL_CFLAGS += -Wall

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DINIT_ENG_BUILD
endif

LOCAL_MODULE := libipsec_ims
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := \
		   src/setkey_fileio/setkey_fileio.c  \
		   src/setkey_fileio/rtnl_socket.c  \
		   src/setkey_fileio/setkey_xfrm.c  \
		   src/setkey_fileio/setkey_xfrm_parse.c  \
		   src/setkey_fileio/utils_xfrm.c

LOCAL_C_INCLUDES += \
		   $(LOCAL_PATH) \
		   $(LOCAL_PATH)/src/setkey_fileio

LOCAL_HEADER_LIBRARIES := liblog_headers
LOCAL_SHARED_LIBRARIES := libcutils libcrypto liblog

LOCAL_CFLAGS := -DANDROID_CHANGES -DHAVE_CONFIG_H
LOCAL_CFLAGS += -Wno-sign-compare -Wno-missing-field-initializers
LOCAL_CFLAGS += -Wall

ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
LOCAL_CFLAGS += -DINIT_ENG_BUILD
endif

LOCAL_MODULE := libipsec_ims_shr
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)


LOCAL_SRC_FILES := \
		   test.c

LOCAL_MODULE := test_xfrm

LOCAL_MODULE_TAGS := optional
LOCAL_HEADER_LIBRARIES := liblog_headers
LOCAL_SHARED_LIBRARIES := libc libcutils liblog libipsec_ims_shr

LOCAL_C_INCLUDES := \
		   $(LOCAL_PATH)/src \
		   $(LOCAL_PATH)/src/setkey_fileio

LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

include $(call first-makefiles-under,$(LOCAL_PATH)/src)
