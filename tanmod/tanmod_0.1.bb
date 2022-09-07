SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

SRC_URI = "file://Makefile \
           file://tanmod_driver.c \
	   file://chardev.c \
	   file://start.c \
           file://stop.c \
	   file://procfs1_read.c \
           file://procfs1_read_and_write.c \
           file://sysfs.c \
           file://procfs_seq.c \
           file://ioctl.c \
           file://mutex.c \
           file://spinlock.c \
           file://read_write_spinlock.c \
           file://atomic_op.c \
           file://tasklet.c \
           file://tasklet_with_interrupt.c \
           file://tasklet_dynamic.c \
           file://workqueue_static.c \
           file://chardev1.h \
           file://COPYING \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-tanmod_driver"
