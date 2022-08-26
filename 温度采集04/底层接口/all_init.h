#ifndef __ALL_INIT_H__
#define __ALL_INIT_H__
// ioctl指令码定义
#define HUM_TEM _IOW('a', 0, int)     //温湿度实时读取
#define LED_MODE _IOW('a', 1, int)    // led模式
#define FAN_MODE _IOW('a', 2, int)    //风扇模式
#define MOTOR_MODE _IOW('a', 3, int)  //马达模式
#define BUZZER_MODE _IOW('a', 4, int) //蜂鸣器模式
#define LED_ALL_GND _IOW('a', 5, int) //共阴极数码管显示
// 指令码中第三个参数的大小
#define DEV_SIZE(msg) ((msg >> 16) & 0x3FFF)

//定义功能枚举
enum MODE
{
    OFF = 0,
    ON,
    FAN = 3,
    MOTOR,
    BUZZER,
};

//中断函数
irqreturn_t key_handler(int main, void *key);

//初始化函数
void i2c_init_func(void);
int node_init(void);
int init_chrdev(void);
void del_node(void);

// LED函数
void led_one_func(void);
void led_two_func(void);
void led_func(void);

//模拟PWM函数
void pwm_func(struct gpio_desc *pwm, int pwm_mode);

//底层数据交互函数
long cdev_ioctl(struct file *file, unsigned int cmd, unsigned long falg);
#endif