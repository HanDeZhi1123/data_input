#ifndef __ALL_INIT_H__
#define __ALL_INIT_H__
//ioctl指令码定义
#define HUM_TEM       _IOW('a',0,int)         //温湿度实时读取
#define LED_MODE      _IOW('a',1,int)        //led模式
#define FAN_MODE      _IOW('a',2,int)       //风扇模式
#define MOTOR_MODE    _IOW('a',3,int)      //马达模式
#define BUZZER_MODE   _IOW('a',4,int)     //蜂鸣器模式
// 指令码中第三个参数的大小
#define DEV_SIZE(msg)    ((msg >> 16) & 0x3FFF)
irqreturn_t key_handler(int main, void *key);
int node_init(void);
int init_chrdev(void);
void del_node(void);
void i2c_init_func(void);
long cdev_ioctl(struct file *file, unsigned int cmd, unsigned long falg);
#endif