#include <linux/spi/spi.h>
#include <linux/module.h>
struct spi_device *spi_lient;

//段显示控制
u8 code[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,\
        0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//小数段显示
u8 code1[] = {0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,\
        0xef,0xf7,0xfc,0xb9,0xde,0xf9,0xf1};
//位显示控制
u8 which[] = {0x8,0x4,0x2,0x1,0x0};

//数据发送
void out_spi(int falg,int sum,int dp)
{
    spi_write(spi_lient,&which[sum],1);
    
    if(sum==2){
        if(dp){         //小数显示打开
            spi_write(spi_lient,&code1[falg],1);
        }else{          //小数显示关闭
            spi_write(spi_lient,&code[falg],1);
        }
        
    }else{
        spi_write(spi_lient,&code[falg],1);
    }
    spi_write(spi_lient,&which[4],1);
}

//spi总线初始化
int	m74hc595_prode(struct spi_device *spi)
{
    spi_lient = spi;
    return 0;
}
int	m74hc595_remove(struct spi_device *spi)
{
    return 0;
}

const struct of_device_id spi_oftable[] = {
    {
        .compatible = "hqyj,m74hc595",
    },
    {/*end*/}};
struct spi_driver m74hc595 = {
    .probe = m74hc595_prode,
    .remove = m74hc595_remove,
    .driver = {
        .name = "hehe",
        .of_match_table = spi_oftable,
    }};
void spi_init(void)
{
    //注册函数
    spi_register_driver(&m74hc595);
    spi_write(spi_lient,&which[4],1);
}

void spi_del(void)
{
    //注销函数
    spi_write(spi_lient,&which[4],1);
    spi_unregister_driver(&m74hc595);
}