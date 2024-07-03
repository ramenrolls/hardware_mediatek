LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=libUtilsHeaders

LOCAL_EXPORT_C_INCLUDE_DIRS:=$(LOCAL_PATH) \
                             $(LOCAL_PATH) ../cutils

include $(BUILD_HEADER_LIBRARY)
