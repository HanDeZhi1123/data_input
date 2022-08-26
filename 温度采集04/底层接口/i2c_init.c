#include <linux/i2c.h>
#include <linux/module.h>
#define TMP_ADDR 0xe3
#define HUM_ADDR 0xe5

struct i2c_client *gclient;

//温湿度数据协议
int i2c_read_tmp_hum(unsigned char reg)
{
    int ret;
    unsigned char r_buf[] = {reg};
    unsigned short data;
    // 1.封装消息
    struct i2c_msg r_msg[] = {
        [0] = {
            .addr = gclient->addr,
            .flags = 0,
            .len = 1,
            .buf = r_buf,
        },
        [1] = {
            .addr = gclient->addr,
            .flags = 1,
            .len = 2,
            .buf = (__u8 *)&data,
        },
    };
    // 2发送消息
    ret = i2c_transfer(gclient->adapter, r_msg, ARRAY_SIZE(r_msg));
    if (ret != ARRAY_SIZE(r_msg))
    {
        printk("i2c read serial or firmware error\n");
        return -EAGAIN;
    }
    data = data >> 8 | data << 8;
    return data;
}

int si7006_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    gclient = client;
    return 0;
}
int si7006_remove(struct i2c_client *client)
{
    return 0;
}
const struct of_device_id i2c_oftable[] = {
    {
        .compatible = "hqyj,si7006",
    },
    {/*end*/}};

struct i2c_driver si7006 = {
    .probe = si7006_probe,
    .remove = si7006_remove,
    .driver = {
        .name = "heihei",
        .of_match_table = i2c_oftable,
    }};
void init_i2c(void)
{
    i2c_register_driver(THIS_MODULE, &si7006); //注册
}
void del_i2c(void)
{
    i2c_del_driver(&si7006); //注销
}