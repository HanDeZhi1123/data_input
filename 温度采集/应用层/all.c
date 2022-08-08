#include <stdio.h>
#include <sys/ioctl.h>
#include "all.h"

//功能处理函数
void mydev_func(int fad)
{
	char choice = 0;
	char falg;
	//调用一级功能菜单
	menu_one();
	do
	{
		falg = 0;
		printf("请输入你的选择\n");
		scanf("%c", &choice);
		while(getchar()!=10);
		switch (choice)
		{
		case '1': // LED
			menu_led();
			//ioctl();
			break;
		case '2': //马达motor
			menu_motor();
			//ioctl();
			break;
		case '3': //风扇fan
			menu_fan();
			//ioctl();
			break;
		case '4': //蜂鸣器buzzer
			menu_buzzer();
			//ioctl();
			break;
		case '5': //退出exit
			break;
		default:
			printf("输入错误请重新输入\n");
			falg = 1;
		}
	}while(falg);
}

//menu功能选择
void menu_one(void)
{
    printf("*****************************\n");
	printf("******功能选择****************\n");
    printf("****1、LED\n");
	printf("****2、马达\n");
	printf("****3、风扇\n");
	printf("****4、蜂鸣器\n");
	printf("****5、数码显示\n");
    printf("****6、温湿度阈值设置\n");
    printf("****7、返回温湿度测试模式\n");
    printf("*****************************\n");
}

//menu_LED模式选择
void menu_led(void)
{
    printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
    printf("******2、退出请按<KEY2>\n");
    printf("*****************************\n");
}

//menu_风扇
void menu_fan(void)
{
    printf("******************************\n");
	printf("******1、加大风量请按<KEY1>\n");
    printf("******2、减小风量请按<KEY2>\n");
    printf("******2、退出请按<KEY3>\n");
    printf("******************************\n");
}

//menu_马达
void menu_motor(void)
{
    printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
    printf("******2、退出请按<KEY2>\n");
    printf("*****************************\n");
}

//menu_蜂鸣器
void menu_buzzer(void)
{
    printf("*****************************\n");
	printf("******1、模式切换请按<KEY1>\n");
    printf("******2、退出请按<KEY2>\n");
    printf("*****************************\n");
}
