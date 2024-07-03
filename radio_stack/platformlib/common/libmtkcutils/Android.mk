LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        sockets.cpp \
        strdup16to8.c \
        strdup8to16.c \
        socket_local_client_unix.c \
        socket_local_server_unix.c \
        sockets_unix.cpp \
        native_handle.c \
        sched_policy.c \
        android_get_control_file.cpp

LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/log
LOCAL_C_INCLUDES += vendor/mediatek/ims/radio_stack/platformlib/include/utils

LOCAL_SHARED_LIBRARIES := libmtkrillog
LOCAL_STATIC_LIBRARIES += libmtkrillog_android_arm

LOCAL_MODULE := libmtkcutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_CFLAGS := -Werror -Wall -Wextra
include $(BUILD_SHARED_LIBRARY)
