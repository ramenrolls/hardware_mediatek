LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    net_cap.cpp

LOCAL_HEADER_LIBRARIES := libMtkLogHeaders libNetCapHeaders libUtilsHeaders

LOCAL_SHARED_LIBRARIES := \
    libmtkrillog \
    libutils

LOCAL_MODULE := libmtknetcap
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_STATIC_LIBRARIES := libcutils
LOCAL_CFLAGS := -Werror -Wall -Wextra
include $(BUILD_SHARED_LIBRARY)
