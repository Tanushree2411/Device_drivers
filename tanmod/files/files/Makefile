obj-m += procfs1_read.o
obj-m += procfs1_read_and_write.o
obj-m += ioctl.o
obj-m += procfs_seq.o
obj-m += sysfs.o
obj-m += chardev.o
obj-m += tanmod_driver.o
obj-m += startstop.o
obj-m += mutex.o
obj-m += spinlock.o
obj-m += read_write_spinlock.o
obj-m += atomic_op.o
obj-m += tasklet.o
obj-m += tasklet_with_interrupt.o
obj-m += tasklet_dynamic.o
obj-m += workqueue_static.o
startstop-objs := start.o stop.o

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:

	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install
clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers


