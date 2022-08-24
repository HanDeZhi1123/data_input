#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "all.h"

//功能处理函数
int mydev_func(int fd)
{
	int i=50000;
	char choice = 0;
	unsigned int  falg,sum[4];
	int falg1 = 0;
	unsigned long tmp_hum;
	float rtmp, rhum;
	//调用一级功能菜单
	menu_one();
	do
	{
		falg = 0;
		printf("请输入你的选择>>>>");
		fflush(stdout);
		do
		{
			scanf("%c", &choice);
		} while (choice == 10);
		while (getchar() != 10);
		switch (choice)
		{
		case '1': // LED
			menu_led();
			ioctl(fd, FAN_MODE,&tmp_hum);
			rtmp = (tmp_hum*175.72)/65536-46.85;
			tmp_hum = rtmp*100;
			sum[0] = tmp_hum/1000;
			sum[1] = tmp_hum/100%10;
			sum[2] = tmp_hum/10%10;
			sum[3] = tmp_hum%10;
			while(i--){
				if(falg1==4)
					falg1=0;
				tmp_hum = sum[falg1];
				ioctl(fd, LED_MODE,tmp_hum);
				falg1++;
			}
			break;
		case '2': //风扇fan
			menu_fan();
			for(int i = 0;i<3;i++){
			ioctl(fd, FAN_MODE,&tmp_hum);
			rtmp = (tmp_hum*175.72)/65536-46.85;
			printf("当前温度为：%.2f\n",rtmp);
			sleep(1);
			}
			break;
		case '3': //马达motor
			menu_motor();
			ioctl(fd, MOTOR_MODE);
			break;
		case '4': //蜂鸣器buzzer
			menu_buzzer();
			ioctl(fd, BUZZER_MODE);
			break;
		case '5': //数码管显示
			break;
		case '6': //温度阈值设置
			break;
		case '7': //返回温度实时监测
			break;
		case '8': //结束程序
			falg = 1;
			break;
		default:
			printf("输入错误请重新输入\n");
			fflush(stdout);
			falg = 2;
		}
	} while ((falg != 1) && (falg));
	return falg;
}

// menu功能选择
void menu_one(void)
{
	printf("*****************************\n");
	printf("******功能选择****************\n");
	printf("****1、LED\n");
	printf("****2、风扇\n");
	printf("****3、马达\n");
	printf("****4、蜂鸣器\n");
	printf("****5、数码显示\n");
	printf("****6、温湿度阈值设置\n");
	printf("****7、返回温湿度测试模式\n");
	printf("****8、退出程序\n");
	printf("*****************************\n");
}

// menu_LED模式选择
void menu_led(void)
{
	printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
	printf("******2、退出请按<KEY2>\n");
	printf("*****************************\n");
}

// menu_风扇
void menu_fan(void)
{
	printf("******************************\n");
	printf("******1、加大风量请按<KEY1>\n");
	printf("******2、减小风量请按<KEY2>\n");
	printf("******2、退出请按<KEY3>\n");
	printf("******************************\n");
}

// menu_马达
void menu_motor(void)
{
	printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
	printf("******2、退出请按<KEY2>\n");
	printf("*****************************\n");
}

// menu_蜂鸣器
void menu_buzzer(void)
{
	printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
	printf("******2、退出请按<KEY2>\n");
	printf("*****************************\n");
}
