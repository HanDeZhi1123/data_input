# 外部的编译需要借助内核源码中的Makefile相关的规则完成驱动的编译
# 因此需要指定内核源码的路径。

# 指定开发板的内核源码的路径
# KERNELDIR := /home/linux/linux-5.10.61

# 指定ubuntu的内核源码的路径
ifeq ($(arch),arm)
	KERNELDIR := /home/ubuntu/linux-5.10.61
	CROSS_COMPILE ?= arm-linux-gnueabihf-
else
	KERNELDIR := /lib/modules/$(shell uname -r)/build
	CROSS_COMPILE ?= 
endif

modname ?=
CC := $(CROSS_COMPILE)gcc
# 执行pwd命令获取当前的路径，赋值给变量PWD
PWD := $(shell pwd)

all:
	make -C $(KERNELDIR)  M=$(PWD)   modules
#$(CC) test.c -o test
	@ # 根据内核源码中模块化编译的规则编译当前目录下的驱动文件。
	@ # 进入到内核源码目录下执行make M=$(PWD)   modules
	@ # M=$(PWD) 只编译当前目录下的驱动文件

clean:
	make -C $(KERNELDIR)  M=$(PWD)   clean
#rm test

install:
	cp *.ko   ~/nfs/rootfs
#cp test  ~/nfs/rootfs

help:
	echo "make arch=arm or x86 modname=dirivers file name"

obj-m := $(modname).o

