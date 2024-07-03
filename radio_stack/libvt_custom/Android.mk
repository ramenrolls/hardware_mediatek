LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
		custom_vt_video_enc_setting.cpp

LOCAL_PRELINK_MODULE:= false
LOCAL_MODULE:= libvt_custom
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MULTILIB := 32
include $(BUILD_SHARED_LIBRARY)
