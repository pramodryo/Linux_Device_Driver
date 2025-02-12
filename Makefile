 
# Define the module name (without .ko)
obj-m := Driver.o
 
# Kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build
 
# Current directory
PWD := $(shell pwd)
 
# Default target to build the module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
 
# Clean target to remove generated files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

