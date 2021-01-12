# -- Makefile -----
# obj-m  := hello.o

KDIR ?= /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

test:
	sudo insmod hello.ko
	sudo rmmod hello.ko
	sudo dmesg

# default:
# 	$(MAKE) -C $(KDIR) M=$$PWD

# Module specific targets
# genbin:
# 	echo "X" > 8123_bin.o_shipped
