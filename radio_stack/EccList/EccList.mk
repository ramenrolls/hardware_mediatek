#
# Ecc List XML File
#

LOCAL_PATH := vendor/mediatek/ims/radio_stack

PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,$(LOCAL_PATH)/EccList/,$(TARGET_COPY_OUT_VENDOR)/etc)
