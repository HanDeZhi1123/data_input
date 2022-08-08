#ifndef __ALL_H__
#define __ALL_H__

//错误提示
#define PERROR_ERR(msg) do{\
    fprintf(stdout,"%s:%s:%d\n",__FILE__,__func__,__LINE__);\
    perror(msg);}while(0)

//ioctl指令码定义
#define LED_MODE            //led模式
#define FAN_MODE            //风扇模式
#define MOTOR_MODE          //马达模式
#define BUZZER_MODE         //蜂鸣器模式


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
void mydev_func(int fad);

#endif