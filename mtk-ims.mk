# Apps
PRODUCT_PACKAGES += \
    ImsService

# Extra Libs
PRODUCT_PACKAGES += \
    libsysenv \
    libmtk_drvb

# Radio IMS
PRODUCT_PACKAGES += \
    libsink \
    libimsma \
    libsource \
    vtservice \
    libsignal \
    libcomutils \
    libvt_avsync \
    libimsma_rtp \
    libvcodec_cap \
    libimsma_adapt \
    libvcodec_capenc \
    libmtk_vt_service \
    libmtk_vt_wrapper \
    libmtkshifter_vendor \
    libmtklimiter_vendor \
    libimsma_socketwrapper

# Radio IMS Boot Jar
PRODUCT_BOOT_JARS += \
    mediatek-common \
    mediatek-ims-base

# Radio IMS Jars
PRODUCT_PACKAGES += \
    mediatek-common \
    mediatek-ims-base

# Video Telephony
PRODUCT_PACKAGES += \
    vtservice_hidl \
    vendor.mediatek.hardware.videotelephony@1.0 \
    vendor.mediatek.hardware.videotelephony@1.0-impl
