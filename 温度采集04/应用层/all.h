#ifndef __ALL_H__
#define __ALL_H__

//错误提示
#define PERROR_ERR(msg) do{\
    fprintf(stdout,"%s:%s:%d\n",__FILE__,__func__,__LINE__);\
    perror(msg);}while(0)

//ioctl指令码定义
#define HUM_TEM       _IOW('a',0,int)         //温湿度实时读取
#define LED_MODE      _IOW('a',1,int)        //led模式
#define FAN_MODE      _IOW('a',2,int)       //风扇模式
#define MOTOR_MODE    _IOW('a',3,int)      //马达模式
#define BUZZER_MODE   _IOW('a',4,int)     //蜂鸣器模式
#define LED_ALL_GND   _IOW('a',5,int)    //共阴极数码管显示

// 指令码中第三个参数的大小
#define DEV_SIZE(msg)    ((msg >> 16) & 0x3FFF)

//温湿度数据处理函数
void hum_tmp(int fd);

//menu功能选择
void menu_one(void);

//menu_LED模式选择
void menu_led(void);

//menu_风扇
void menu_fan(void);

//menu_马达
void menu_motor(void);

//menu_蜂鸣器
void menu_buzzer(void);

//功能处理函数
int mydev_func(int fd);

#endif