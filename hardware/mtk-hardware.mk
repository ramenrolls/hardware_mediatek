#
# Copyright (C) 2020 The PixelExperience Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Connectivity
$(call inherit-product, vendor/mediatek/opensource/hardware/connectivity/connectivity_package.mk)

# Hardware
PRODUCT_PACKAGES += \
    libpowerhal \
    libccci_util \
    libfpspolicy \
    power.default \
    thermal.beanpod \
    libmtkperf_client \
    libmtkaudio_utils \
    libfpspolicy-client \
    libpowerhalwrap_vendor \
    libmtkperf_client_vendor \
    vendor.mediatek.hardware.power@2.1-impl \
    android.hardware.power@1.3-impl-mediatek \
    android.hardware.usb@1.1-service-mediatek \
    vendor.mediatek.hardware.mtkpower@1.0-impl \
    vendor.mediatek.hardware.mtkpower@1.0-service
