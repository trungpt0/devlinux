obj-m += itg_based_gpio.o

ARCH=arm
CP=arm-linux-gnueabi-
KDIR=/home/esacev/tmp/lib/modules/5.10.168/build

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CP) -C $(KDIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CP) -C $(KDIR) M=$(PWD) clean
