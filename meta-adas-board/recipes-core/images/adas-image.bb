SUMMARY = "ADAS BSP lab image"
LICENSE = "MIT"
inherit core-image
IMAGE_FEATURES += "ssh-server-dropbear"
IMAGE_INSTALL += "kernel-modules can-utils i2c-tools ethtool adas-gateway"
