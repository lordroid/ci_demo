#include "smt_at24c08b.h"
#include <string.h>

void AT24C08B_ByteWrite(char wordaddr,char data)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_client client = {0};
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 48000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*0:write*/
    client.addr = SMT_AT24C08B_ADDR;/* device addr*/
    strcpy(client.name,"at24c08b"); 
    buf[0] = wordaddr;
    buf[1] = data;
    i2c_master_send(IIC0,&client,buf,2);
}  
void AT24C08B_PageWrite(char wordaddr,char *pbuf,int cnt)
{
    I2C_InitStruct InitStruct = {0};
    char buf[256] = {0};
    struct i2c_client client = {0};
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 10000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*0:write*/
    client.addr = SMT_AT24C08B_ADDR;/* device addr*/
    strcpy(client.name,"at24c08b"); 
    buf[0] = wordaddr;
    strncpy(&buf[1],pbuf,cnt);
    i2c_master_send(IIC0,&client,buf,cnt+1);
}

char AT24C08B_CurrentAddressRead(void)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_msg msgs[2];
    struct i2c_client client;
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 48000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);

    client.flags = 0;/*1:read*/
    client.addr = SMT_AT24C08B_ADDR;/*device addr*/
	
    msgs[0].addr = client.addr;
    msgs[0].flags = 1;
    msgs[0].len = 1;
    msgs[0].buf = (unsigned char*)buf;
    i2c_master_transfer(&i2c0_master,msgs,1);
    return buf[0];
}

char AT24C08B_RandomRead(char wordaddr)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_msg msgs[2];
    struct i2c_client client;
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 10000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);

    client.flags = 0;/*1:read*/
    client.addr = SMT_AT24C08B_ADDR;/*device addr*/
	
    buf[0] = wordaddr;
    msgs[0].addr = client.addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = (unsigned char*)buf;
    msgs[1].addr = client.addr;
    msgs[1].flags = 1;
    msgs[1].len = 1;
    msgs[1].buf = (unsigned char*)buf;
    i2c_master_transfer(&i2c0_master,msgs,2);
    return buf[0];
}

void AT24C08B_SequentailRead(char *pbuf,int cnt)
{
    I2C_InitStruct InitStruct = {0};
    struct i2c_msg msgs[1];
    struct i2c_client client;
    
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 10000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);

    client.flags = 0;/*1:read*/
    client.addr = SMT_AT24C08B_ADDR;/*device addr*/
	
    msgs[0].addr = client.addr;
    msgs[0].flags = 1;
    msgs[0].len = cnt;
    msgs[0].buf = (unsigned char*)pbuf;
    i2c_master_transfer(&i2c0_master,msgs,1);
}

void AT24C08B_I2CInit(void)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIC0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    Scu_SetDeviceGate((unsigned int)IIC0,ENABLE);
    Scu_SetIOReuse(I2C0_SCL_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2C0_SDA_PAD,FIRST_FUNCTION);
}

void SMT_AT24C08Btest(void)
{
    unsigned int delay = 0x5fffff;
    char tmp = 0x53,rx_buf[20] = {0};
    //char tx_buf[]={"i2c0 test"};
    AT24C08B_I2CInit();
    AT24C08B_ByteWrite(0x20,tmp);
    while(delay--);
    rx_buf[0] = AT24C08B_RandomRead(0x20);
    if(tmp == rx_buf[0])
    {
      mprintf("Byte write & RandomRead OK\n");
    }
    else
    {
      mprintf("Byte write & RandomRead Failed %x\n",rx_buf[0]);
    }
    //mprintf("current read:%x\n",AT24C08B_CurrentAddressRead());
    //AT24C08B_PageWrite(0x0,tx_buf,8);
    //AT24C08B_SequentailRead(rx_buf,8);
    //mprintf("sequen read:%s\n",rx_buf);
}
