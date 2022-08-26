#include <linux/gpio/consumer.h>
#include <linux/delay.h>

//子节点接收变量
extern struct gpio_desc *gpiod_node[9];
//extern int Mode[4];
extern int Mode[6];

//模式一齐闪齐灭
void led_one_func(void)
{
    while ((Mode[1] != 2) && Mode[1])
    {
        gpiod_set_value(gpiod_node[0], !gpiod_get_value(gpiod_node[0]));
        gpiod_set_value(gpiod_node[1], !gpiod_get_value(gpiod_node[1]));
        gpiod_set_value(gpiod_node[2], !gpiod_get_value(gpiod_node[2]));
        mdelay(500);
    }
    //中途换模式管脚清空
    gpiod_set_value(gpiod_node[0], 0);
    gpiod_set_value(gpiod_node[1], 0);
    gpiod_set_value(gpiod_node[2], 0);
}

//模式二循环亮灭
void led_two_func(void)
{

    while (Mode[1] && (Mode[1] != 3))
    {
        gpiod_set_value(gpiod_node[0], !gpiod_get_value(gpiod_node[0]));
        mdelay(500);
        if ((Mode[1] && (Mode[1] == 3)))
            break;
        gpiod_set_value(gpiod_node[1], !gpiod_get_value(gpiod_node[1]));
        mdelay(500);
        if ((Mode[1] && (Mode[1] == 3)))
            break;
        gpiod_set_value(gpiod_node[2], !gpiod_get_value(gpiod_node[2]));
        mdelay(500);
    }
    //中途换模式管脚清空
    gpiod_set_value(gpiod_node[0], 0);
    gpiod_set_value(gpiod_node[1], 0);
    gpiod_set_value(gpiod_node[2], 0);
}

//模式三
void led_three_func(void)
{
    int i;
    while (Mode[1] && (Mode[1] != 1))
    {
        for (i = 0; i < 6; i++)
        {
            gpiod_set_value(gpiod_node[i % 3], 1);
            mdelay(300);
            if ((Mode[1] && (Mode[1] == 1)))
                return;
            gpiod_set_value(gpiod_node[i % 3], 0);
            mdelay(300);
            if ((Mode[1] && (Mode[1] == 1)))
                return;
        }
        for (i = 0; i < 9; i++)
        {
            gpiod_set_value(gpiod_node[i % 3], !gpiod_get_value(gpiod_node[i % 3]));
            mdelay(300);
            if ((Mode[1] && (Mode[1] == 1)))
                return;
        }
        for (i = 0; i < 4; i++)
        {
            gpiod_set_value(gpiod_node[0], (i + 1) % 2);
            gpiod_set_value(gpiod_node[1], (i + 1) % 2);
            gpiod_set_value(gpiod_node[2], (i + 1) % 2);
            mdelay(500);
            if ((Mode[1] && (Mode[1] == 1)))
                return;
        }
    }
}

void led_func(void)
{
    int i;
    while (Mode[1])
    {
        //功能实现
        switch (Mode[1])
        {
        case 1: //模式一
            led_one_func();
            break;
        case 2: //模式二
            led_two_func();
            break;
        case 3: //模式三
            led_three_func();
            //中途换模式管脚清空
            gpiod_set_value(gpiod_node[0], 0);
            gpiod_set_value(gpiod_node[1], 0);
            gpiod_set_value(gpiod_node[2], 0);
            break;
        }
    }
    for (i = 0; i < 3; i++)
    {
        gpiod_set_value(gpiod_node[i], 0);
    }
   // Mode[1] = !Mode[1];
    return;
}
