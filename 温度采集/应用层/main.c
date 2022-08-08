#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "all.h"

#define BUF_ZERO(zer) memset(zer,0,sizeof(zer))

int main(int argc, const char *argv[])
{
	int fd[8];			//保存LED、马达、风扇、蜂鸣器设备节点的文件描述符
	char dev[14];  		
	BUF_ZERO(fd);
	//获取设备节点文件描述符
	for(int i=0;i<1;i++){
		BUF_ZERO(dev);
		sprintf(dev,"/dev/mydev%d",i);
		fd[i] = open(dev,O_RDWR);
		printf("%d%d\n",fd[i],i);
		if(fd[0] == -1){
			PERROR_ERR("open");
			return -1;
		}
	}

	//循环实现功能选择
	while (1)
	{
		//输出当前模式及模式切换方法
		printf("已启动温湿度实时采集模式\n");
		printf("进入其他模式请双击<KEY1>\n");

		//睡眠等待信号唤醒
		mydev_func(1);
	}

	return 0;
}
