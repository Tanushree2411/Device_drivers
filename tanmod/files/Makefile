obj-m += procfs1_read.o
obj-m += procfs1_read_and_write.o
obj-m += procfs_seq.o
obj-m += sysfs.o
obj-m += chardev.o
obj-m += tanmod_driver.o
obj-m += startstop.o
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


