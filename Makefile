# IEEE 802.11 Wireless Operations Hacker
ifneq ($(KERNELRELEASE),)
	obj-m := wlsops_hook.o
	wlsops_hook-objs := hack_entry.o wlsops.o
else
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build

all:build-krn

build-krn:
	$(MAKE) -C $(KDIR) M=$(PWD)

insmod:
	sudo insmod wlsops_hook.ko

rmmod:
	sudo rmmod wlsops_hook

dmesg:
	sudo dmesg

clean:
	rm -f .cache.mk .*.cmd
	rm -f *.o *.o.cmd *.ko *.mod.c *.symvers *.order
	rm -rf .tmp_versions
endif