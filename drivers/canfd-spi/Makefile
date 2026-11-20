# Kernel modules are out-of-tree; build needs a Linux kernel tree/headers.
obj-m += adas_canfd.o
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
