#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>

//定义父节点接收变量
struct device_node *father_node;
//定义子节点接收变量
unsigned int son_node[9];
//定义键的名称
char *key[] = {"led1", "led2", "led3", "fan", "motor", "buzzer", "key1", "key2", "key3"};
/* LED、风扇、马达、蜂鸣器、按键设备树信息
    all_dev{
        compatible = "hqyj,led-key-interrupt";
        interrupt-parent = <&gpiof>;
        interrupts = <9 0>,<7 0>,<8 0>;
        led1 = <&gpioe 10 0>;
        led2 = <&gpiof 10 0>;
        led3 = <&gpioe 8  0>;
        fan  = <&gpioe 9 0>;
        motor = <&gpiof 6 0>;
        buzzer = <&gpiob 6 0>;
        key1 = <&gpiof 9 0>;
        key2 = <&gpiof 7 0>;
        key3 = <&gpiof 8 0>;
    };
*/

//中断函数
irqreturn_t irq_handler(int i, void *j)
{
    int falg = (int)j;
    switch (falg)
    {
    case 6:
        printk("\n进来了!!!!!!");
        break;
    case 7:
        printk("\n进来了#########");
        break;
    case 8:
        printk("\n进来了~~~~~~~~");
        break;
    }

    return IRQ_HANDLED;
}

//资源分配入口
static int __init all_init(void)
{
    int i, res;
    //获取父结点
    father_node = of_find_compatible_node(NULL, NULL, "hqyj,led-key-interrupt");
    if (father_node == NULL)
    {
        printk("of find compatible error\n");
        return 0;
    }

    //测试信息
    printk("%s\n", father_node->full_name);
    for (i = 0; i < ARRAY_SIZE(son_node); i++)
    {
        if (i < 6)
        {
            son_node[i] = of_get_named_gpio(father_node, key[i], 0);
            if (son_node[i] < 0)
            {
                printk("get named gpio error %d\n", i);
                return son_node[i];
            }
        }
        //注册中断
        else
        {
            son_node[i] = irq_of_parse_and_map(father_node, i - 6);
            if (son_node[i] < 0)
            {
                printk("irq_of_parse_and_map %d\n", i);
                return son_node[i];
            }
            res = request_irq(son_node[i], irq_handler, IRQF_TRIGGER_FALLING, key[i], (void *)i);
            if (res)
            {
                printk("request_irq error\n");
                return res;
            }
        }
    }
    return 0;
}

//资源释放出口
static void __exit all_exit(void)
{
    int i;
    for (i = 6; i < ARRAY_SIZE(son_node); i++)
    {
        free_irq(son_node[i], (void *)i);
    }
    printk("释放\n");
}

module_init(all_init);
module_exit(all_exit);
MODULE_LICENSE("GPL");