# IEEE 802.11 Wireless Operations Hacker
ifneq ($(KERNELRELEASE),)
	obj-m := wlsops_hook.o
	wlsops_hook-objs := hack_entry.o hack_mmap.o wlsops.o
else
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build

all:build-krn build-usr

build-krn:
	$(MAKE) -C $(KDIR) M=$(PWD)

build-usr:
	$(MAKE) -C wlsctrl

install:
	sudo cp -rf wlsops_hook.ko /lib/modules/$(shell uname -r)/updates/wlsops_hook.ko
	sudo sh -c 'echo install wlsops insmod /lib/modules/$(shell uname -r)/updates/wlsops_hook.ko \
		> /lib/modprobe.d/wlsops.conf'
	sudo cp -f wlsctrl/wlsctrl /usr/bin/wlsctrl
	sudo depmod -a

modprobe:
	sudo modprobe wlsops_hook

insmod:
	sudo insmod wlsops_hook.ko

rmmod:
	sudo rmmod wlsops_hook

dmesg:
	sudo dmesg

clean:
	rm -f wlsctrl/wlsctrl
	rm -f .cache.mk .*.cmd
	rm -f *.o *.o.cmd *.ko *.mod.c *.symvers *.order
	rm -rf .tmp_versions
endif