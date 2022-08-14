#include <linux/of.h>
#include <linux/fs.h>
#include <linux/delay.h>
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

//父节点接收变量
struct device_node *father_node;

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
extern void out_spi(int falg,int sum);
int sum1 = 0;

//数据处理函数
long cdev_ioctl(struct file *file, unsigned int cmd, unsigned long falg)
{
    int ret, data;
    switch (cmd)
    {
    case LED_MODE:
        if(sum1==4)
            sum1 = 0;
        out_spi(falg,sum1);
        sum1++;
        break;
    case FAN_MODE:
        gpiod_set_value(gpiod_node[3], !gpiod_get_value(gpiod_node[3]));
        data = i2c_read_tmp_hum(0xe3);
        ret = copy_to_user((void *)falg, &data, GET_CMD_SIZE(cmd));
        if (ret)
        {
            printk("copy data to user error\n");
            return -EIO;
        }
        break;
    case MOTOR_MODE:
        gpiod_set_value(gpiod_node[4], !gpiod_get_value(gpiod_node[4]));
        break;
    case BUZZER_MODE:
        gpiod_set_value(gpiod_node[5], !gpiod_get_value(gpiod_node[5]));
        break;
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
        gpiod_set_value(gpiod_node[5], (!gpiod_get_value(gpiod_node[5])));
        break;
    case 1:
        gpiod_set_value(gpiod_node[4], (!gpiod_get_value(gpiod_node[4])));
        break;
    case 2:
        gpiod_set_value(gpiod_node[3], (!gpiod_get_value(gpiod_node[3])));
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
    return 0;
}

//获取spi总线结点信息

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
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    printk("释放\n");
}
