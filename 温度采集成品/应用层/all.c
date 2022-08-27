#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "all.h"

//温湿度阈值
int hum_or_tmp[2] = {3200, 8000}; //[0]:温度   [1]:湿度
//功能处理函数
int mydev_func(int fd)
{
	int i = 50000;
	char choice = 0;
	unsigned int falg;
	int falg1 = 0;
	int data;
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
		while (getchar() != 10)
			;
		switch (choice)
		{
		case '1': // LED
			menu_led();
			ioctl(fd, LED_MODE, 0);
			break;
		case '2': //风扇fan
			menu_fan();
			ioctl(fd, FAN_MODE);
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
			printf("请输入你要显示的数字0~9999>>>>");
			scanf("%d", &data);
			ioctl(fd, FAN_MODE, &data);
			break;
		case '6': //温度阈值设置
			set_hum_tmp();
			break;
		case '7': //返回到温度实时监测
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

//温湿度数据处理函数
void hum_tmp(int fd)
{
	unsigned long tmp_hum;
	unsigned int sum[4];
	float rtmp, rhum = 1;
	int i, falg = 4;
	int falg_hum=0,falg_tmp=0;	//报警使能
	while (tmp_hum != 12)
	{
		i = 5000;
		tmp_hum = rhum;
		ioctl(fd, HUM_TEM, &tmp_hum); //获取数据
		if (rhum == 1)
		{
			rtmp = (tmp_hum * 175.72) / 65536 - 46.85; //计算温度
			printf("当前的温度为:%.2f℃\r", rtmp);
			fflush(stdout);
			tmp_hum = rtmp * 100; //转换数据为千位整数
			//温度阈值判断
			if (tmp_hum>=hum_or_tmp[0]){
				ioctl(fd,WARNING_MAX,1);
				falg_tmp = 1;
			}else if(falg_tmp){
				ioctl(fd,WARNING_MAX,0);
				falg_tmp = 0;
			}
		}
		else if (rhum == 2)
		{
			rtmp = (tmp_hum * 125) / 65536 - 6; //计算湿度
			printf("当前的湿度为:%.2f%%\r", rtmp);
			fflush(stdout);
			tmp_hum = rtmp * 100; //转换数据为千位整数
			//湿度阈值判断
			if (tmp_hum>=hum_or_tmp[1]){
				ioctl(fd,WARNING_MAX,1);
				falg_hum = 1;
			}else if(falg_hum){
				ioctl(fd,WARNING_MAX,0);
				falg_hum = 0;
			}
		}
		//温湿度阈值判断
		sum[0] = tmp_hum / 1000;	 //获取千位数据
		sum[1] = tmp_hum / 100 % 10; //获取百位数据
		sum[2] = tmp_hum / 10 % 10;	 //获取十位数剧
		sum[3] = tmp_hum % 10;		 //获取个位数据

		//延时，显示的时长
		while (i--)
		{
			if (falg == 0)
				falg = 4;
			tmp_hum = sum[falg - 1];
			ioctl(fd, LED_ALL_GND, tmp_hum);
			if ((tmp_hum == 10) || (tmp_hum == 11) || (tmp_hum == 12))
				break;
			falg--;
		}
		if (tmp_hum == 10)
		{
			rhum = 1; //显示温度
		}
		else if (tmp_hum == 11)
		{
			rhum = 2; //显示湿度
		}
		else
		{
			rhum = rhum == 2 ? 1 : 2; //温湿度交替
		}
	}
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

//温湿度阈值设置
void set_hum_tmp(void)
{
	float tmp, hum; //温湿度阈值保存
	printf("请输入你要设置的温度阈值(格式:100以内):");
	scanf("%f", &tmp);
	printf("请输入你要设置的湿度阈值(格式:100以内):");
	scanf("%f", &hum);
	hum_or_tmp[0] = tmp * 100; //转换温度
	hum_or_tmp[1] = hum * 100; //转换湿度
}