#include <linux/of.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/spi/spi.h>
#include "all_init.h"

#define NAME "STM32_CAN"
#define GET_CMD_SIZE(cmd) ((cmd >> 16) & 0x3fff)
//定义键的名称
char *key[] = {"led1", "led2", "led3", "fan", "motor",
               "buzzer", "key1", "key2", "key3"};

//所有工作函数使能
// int Enable[4] = {0, 0, 0, 0};       //所有模式使能

/*所有模式使能Mode[0]:温湿度、Mode[1]:LED、Mode[2]:风扇、蜂鸣器、马达、Mode[3]:定時器*/
int Mode[4] = {9, 1, 1, 1};
int timer_s[2] = {100, 1}; // timer_s[0]用于表示1s,timer_s[1]表示占用时间
//父节点接收变量
struct device_node *father_node;

//定时器结构体信息
struct timer_list pwm_timer;

//子节点接收变量
struct gpio_desc *gpiod_node[9];
u16 addr; // i2c从机地址结点信息
//中断号
unsigned int irq_node[3];
unsigned int major = 0;
struct class *cls = NULL;
struct device *dev = NULL;
//外部信息
extern struct file_operations fops;
extern int i2c_read_tmp_hum(unsigned char reg);
extern void out_spi(int falg, int sum, int dp);
extern void led_func(void);

//小数显示条件
int sum1 = 0;

//定时器中断处理函数
void timer_irq_func(struct timer_list *timer)
{
    //修改当前IO口电平状态
    switch (timer->flags)
    {
    case FAN:
        gpiod_set_value(gpiod_node[FAN], !gpiod_get_value(gpiod_node[FAN]));
        break;
    case MOTOR:
        gpiod_set_value(gpiod_node[MOTOR], !gpiod_get_value(gpiod_node[MOTOR]));
        break;
    case BUZZER:
        gpiod_set_value(gpiod_node[BUZZER], !gpiod_get_value(gpiod_node[BUZZER]));
        break;
    }

    //启动定时器设置
    Mode[3] = ON;
}

//模拟PWM函数
void pwm_func(struct gpio_desc *pwm, int pwm_mode)
{
    int falg_mode = 0; //高低电平定时时间控制
    int time_max, time_min;
    //打开定时器时间设置、硬件功能使能
    Mode[2] = Mode[3] = ON;
    pwm_timer.flags = pwm_mode;
    while (Mode[2])
    {
        if (Mode[3])
        {
            time_min = timer_s[0] - timer_s[1] * 15;                                        //低电平时间
            time_max = timer_s[0] - time_min;                                               //高电平时间
            pwm_timer.expires = falg_mode == OFF ? jiffies + time_max : jiffies + time_min; // 0高电平时间、1低电平时间
            falg_mode = !falg_mode;                                                         //高低电平定时时间控制转换
            //再次启动定时器
            mod_timer(&pwm_timer, pwm_timer.expires);
            Mode[3] = OFF; //关闭定时器时间设置
        }
    }
    //关闭对应状态
    gpiod_set_value(pwm, OFF);
}

//数据处理函数
long cdev_ioctl(struct file *file, unsigned int cmd, unsigned long falg)
{
    int ret, data;
    switch (cmd)
    {
        //温湿度数据获取模式
    case HUM_TEM:
        if (falg == 1)
        {
            data = i2c_read_tmp_hum(0xe3); //获取温度数据
        }
        else if (falg == 2)
        {
            data = i2c_read_tmp_hum(0xe5); //获取湿度数据
        }
        ret = copy_to_user((void *)falg, &data, GET_CMD_SIZE(cmd));
        if (ret)
        {
            printk("copy data to user error\n");
            return -EIO;
        }
        break;
        // LED模式
    case LED_MODE:
        // Enable[0] = 1;
        Mode[1] = 1;
        led_func();
        break;
        //风扇模式
    case FAN_MODE:
        pwm_func(gpiod_node[FAN], FAN);
        break;
        //振动马达模式
    case MOTOR_MODE:
        pwm_func(gpiod_node[MOTOR], MOTOR);
        break;
        //蜂鸣器模式
    case BUZZER_MODE:
        pwm_func(gpiod_node[BUZZER], BUZZER);
        break;
        //数码管显示模式
    case LED_ALL_GND:
        if (sum1 == 4)
            sum1 = 0;
        out_spi(falg, sum1, 1);
        sum1++;
        break;
    case WARNING_MAX:
        switch (falg)
        {
        case OFF:
            gpiod_set_value(gpiod_node[FAN], OFF);
            gpiod_set_value(gpiod_node[BUZZER], OFF);
        case ON:
            gpiod_set_value(gpiod_node[FAN], ON);
            gpiod_set_value(gpiod_node[BUZZER], ON);
        }
    case WARNING_MIN:
        if (falg == OFF)
        {
            gpiod_set_value(gpiod_node[MOTOR], OFF);
        }
        else if (falg == ON)
        {
            gpiod_set_value(gpiod_node[MOTOR], ON);
        }
    }
    return 0;
}

//中断函数
irqreturn_t key_handler(int main, void *key)
{
    int falg = (int)key;
    switch (falg)
    {
    case 0:
        gpiod_set_value(gpiod_node[3], (!gpiod_get_value(gpiod_node[3])));
        // Enable[0] += 1;
        Mode[1] += 1;
        if (Mode[1] == 4)
        {
            // Enable[0] = 1;
            Mode[1] = 1;
        }
        if (Mode[0] == 12)
        {
            Mode[0] = 9;
        }
        Mode[0] += 1;
        if (timer_s[1] == 6)
            timer_s[1] = 0;
        timer_s[1] += 1;
        break;
    case 1:
        gpiod_set_value(gpiod_node[4], (!gpiod_get_value(gpiod_node[4])));
        Mode[1] = 0;
        Mode[0] = 12;
        if (timer_s[1] == 1)
            timer_s[1] = 2;
        timer_s[1] -= 1;
        break;
    case 2:
        gpiod_set_value(gpiod_node[5], (!gpiod_get_value(gpiod_node[5])));
        Mode[2] = 0;
        // Enable[0] = !Enable[0];
        break;
    }
    return IRQ_HANDLED;
}

// gpio初始化
int node_init(void)
{
    int i, res;
    //获取父结点
    father_node = of_find_compatible_node(NULL, NULL, "hqyj,led-key-interrupt");
    if (father_node == NULL)
    {
        printk("of find compatible error\n");
        return 0;
    }
    //获取gpio子系统结点
    for (i = 0; i < 9; i++)
    {
        gpiod_node[i] = gpiod_get_from_of_node(father_node, key[i], 0, GPIOD_OUT_LOW, NULL);
        if (gpiod_node[i] < 0)
        {
            printk("get named gpio error %d\n", i);
            return -1;
        }
    }
    //获取中断号
    for (i = 0; i < 3; i++)
    {
        //注册中断
        irq_node[i] = gpiod_to_irq(gpiod_node[i + 6]);
        printk("%d\n", irq_node[i]);
        res = request_irq(irq_node[i], key_handler, IRQF_TRIGGER_FALLING, key[i + 6], (void *)i);
        if (res)
        {
            printk("request_irq error\n");
            return res;
        }
    }

    //初始化定时器
    pwm_timer.expires = jiffies + 1; //进来直接启动
    pwm_timer.flags = OFF;           //防止初始化时启动硬件
    //注册定时器（只要注册就会启动）
    timer_setup(&pwm_timer, timer_irq_func, 0);
    add_timer(&pwm_timer);

    return 0;
}

//设备节点创建
int init_chrdev(void)
{
    //申请设备号
    major = register_chrdev(0, NAME, &fops);
    if (major < 0)
    {
        printk("register chrdev error\n");
        goto ERR1;
    }
    //注册设备号
    // 1、向上层提交目录信息
    cls = class_create(THIS_MODULE, "cdev_class");
    if (IS_ERR(cls))
    {
        printk("device create error\n");
        goto ERR2;
    }
    // 2、向上层提交设备信息
    dev = device_create(cls, NULL, MKDEV(major, 0), NULL, "device_dev");
    if (IS_ERR(dev))
    {
        printk("device create error\n");
        goto ERR3;
    }
    return 0;
ERR3:
    device_destroy(cls, MKDEV(major, 0));
ERR2:
    class_destroy(cls);
ERR1:
    return major;
}

//釋放函數
void del_node(void)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (i < 3)
            free_irq(irq_node[i], (void *)i);
        gpiod_put(gpiod_node[i]);
    }
    del_timer(&pwm_timer);
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
}
