#obj-$(CONFIG_NAVEEN_CUSTOM_MODULE) += wait_queue.o
obj-m += platform_driver2.o
obj-m += platform_device2.o

KERNELDIR ?= /home/naveen/repos/hikey/src/linux/
#If you use this, then your module will be built against your current host machine kernel headers
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build/

all: modules

modules:
	$(MAKE) -C $(KERNELDIR) ARCH=arm64 CROSS_COMPILE=${ARM64} M=$(shell pwd) $@
	#$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

install:
	$(MAKE) -C $(KERNELDIR) ARCH=arm64 CROSS_COMPILE=${ARM64} INSTALL_MOD_PATH=/home/naveen/rootfs-arm64/ modules_install
	#sudo cp *.ko ~/rootfs-arm64/modules/

clean:
	rm -rf *.o *.ko
#
# This basically translates all the commands to :
# make modules -> make -C blahblah M=blahblah modules
# make modules_install -> make -C blahblah M=blahblah modules_install
# make help -> make -C blahblah M=blahblah help
# make clean -> make -C blahblah M=blahblah clean
