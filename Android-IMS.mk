ifeq ($(TARGET_PROVIDES_MEDIATEK_IMS_STACK),true)

MTK_IMS_ROOT_PATH := $(call my-dir)

# MTK IMS
include $(MTK_IMS_ROOT_PATH)/radio_stack/Android.mk

endif # TARGET_PROVIDES_MEDIATEK_IMS_STACK
