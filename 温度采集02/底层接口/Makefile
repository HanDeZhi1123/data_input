# 外部的编译需要借助内核源码中的Makefile相关的规则完成驱动的编译
# 因此需要指定内核源码的路径。

# 指定开发板的内核源码的路径
# KERNELDIR := /home/linux/linux-5.10.61

# 指定ubuntu的内核源码的路径
KERNELDIR := /home/ubuntu/linux-5.10.61
#CROSS_COMPILE ?= arm-linux-gnueabihf-

#CC := $(CROSS_COMPILE)gcc
# 执行pwd命令获取当前的路径，赋值给变量PWD
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR)  M=$(PWD)   modules
#$(CC) test.c -o test

clean:
	make -C $(KERNELDIR)  M=$(PWD)   clean
#rm test

install:
	cp *.ko   ~/nfs/rootfs
#cp test  ~/nfs/rootfs

help:
	echo "make arch=arm or x86 modname=dirivers file name"

obj-m := all_func.o
all_func-objs := all_init.o init_exit.o i2c_init.o spi_init.o
#all_func-odjs := buzzwe_sunc.o fan_func.o led_func.o motor_func.o
