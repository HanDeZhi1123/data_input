#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "all.h"

#define BUF_ZERO(zer) memset(zer, 0, sizeof(zer))

int main(int argc, const char *argv[])
{
	int falg = 0;
	int fd; //保存LED、马达、风扇、蜂鸣器设备节点的文件描述符
	system("insmod all_func.ko");
	//获取设备节点文件描述符
	fd = open("/dev/device_dev", O_RDWR);
	if (fd == -1)
	{
		PERROR_ERR("open");
		return -1;
	}
	//循环实现功能选择
	while (falg!=1)
	{
		//输出当前模式及模式切换方法
		printf("已启动温湿度实时采集模式\n");
		printf("进入其他模式请按<KEY1>\n");
		//调用温湿度数据处理函数
		hum_tmp(fd);
		//其他功能函数
		falg = mydev_func(fd);
	}
	printf("********程序已安全退出*********\n");
	close(fd);
	return 0;
}
