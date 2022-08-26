#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include "all_init.h"
extern void init_i2c(void);
extern void spi_init(void);
extern void spi_del(void);
extern void del_i2c(void);

struct file_operations fops = {
    .unlocked_ioctl = cdev_ioctl,
};

//资源分配入口
static int __init all_init(void)
{
    int res;
    //申请设备节点，搭建应用层交互环境
    res = init_chrdev();
    if(res < 0){
        printk("init chrdev error\n");
    }
    //调用外部初始化函数
    res = node_init();
    if(res < 0){
        printk("node init error\n");
    }
    init_i2c();
    spi_init();
    return 0;
}

//资源释放出口
static void __exit all_exit(void)
{
    
    //I2C注销
    spi_del();
    //spi注销
    del_i2c();
    //释放函数
    del_node();
}

module_init(all_init);
module_exit(all_exit);
MODULE_LICENSE("GPL");