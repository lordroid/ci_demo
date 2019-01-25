#include "smt_cs42l52.h"
#include "ci100x_uart.h"
#include "ci100x_scu.h"
 
#include "misc.h"
#include <string.h>
#include "cs42l52.h"
#include "adpcm.h"
#include "ci100x_iis.h"
#include "ci100x_spiflash.h"
#include "smt_iis.h"
#include "includes.h"
#include "com2_task.h"

#if USE_S_MIC_DENOISE
#include "ci_denoise.h"
#endif 

#define ADC_USE_ES8388
//#define USE_CS42L52 
//#define USE_ES7144LV_DS

#define ES8388_CONTROL1         0x00
#define ES8388_CONTROL2         0x01
#define ES8388_CHIPPOWER        0x02
#define ES8388_ADCPOWER         0x03
#define ES8388_DACPOWER         0x04
#define ES8388_CHIPLOPOW1       0x05
#define ES8388_CHIPLOPOW2       0x06
#define ES8388_ANAVOLMANAG      0x07
#define ES8388_MASTERMODE       0x08
#define ES8388_ADCCONTROL1      0x09
#define ES8388_ADCCONTROL2      0x0a
#define ES8388_ADCCONTROL3      0x0b
#define ES8388_ADCCONTROL4      0x0c
#define ES8388_ADCCONTROL5      0x0d
#define ES8388_ADCCONTROL6      0x0e
#define ES8388_ADCCONTROL7      0x0f
#define ES8388_ADCCONTROL8      0x10
#define ES8388_ADCCONTROL9      0x11
#define ES8388_ADCCONTROL10     0x12
#define ES8388_ADCCONTROL11     0x13
#define ES8388_ADCCONTROL12     0x14
#define ES8388_ADCCONTROL13     0x15
#define ES8388_ADCCONTROL14     0x16

#define ES8388_DACCONTROL1      0x17
#define ES8388_DACCONTROL2      0x18
#define ES8388_DACCONTROL3      0x19
#define ES8388_DACCONTROL4      0x1a
#define ES8388_DACCONTROL5      0x1b
#define ES8388_DACCONTROL6      0x1c
#define ES8388_DACCONTROL7      0x1d
#define ES8388_DACCONTROL8      0x1e
#define ES8388_DACCONTROL9      0x1f
#define ES8388_DACCONTROL10     0x20
#define ES8388_DACCONTROL11     0x21
#define ES8388_DACCONTROL12     0x22
#define ES8388_DACCONTROL13     0x23
#define ES8388_DACCONTROL14     0x24
#define ES8388_DACCONTROL15     0x25
#define ES8388_DACCONTROL16     0x26
#define ES8388_DACCONTROL17     0x27
#define ES8388_DACCONTROL18     0x28
#define ES8388_DACCONTROL19     0x29
#define ES8388_DACCONTROL20     0x2a
#define ES8388_DACCONTROL21     0x2b
#define ES8388_DACCONTROL22     0x2c
#define ES8388_DACCONTROL23     0x2d
#define ES8388_DACCONTROL24     0x2e
#define ES8388_DACCONTROL25     0x2f
#define ES8388_DACCONTROL26     0x30
#define ES8388_DACCONTROL27     0x31
#define ES8388_DACCONTROL28     0x32
#define ES8388_DACCONTROL29     0x33
#define ES8388_DACCONTROL30     0x34

#define ES8388_LADC_VOL         ES8388_ADCCONTROL8
#define ES8388_RADC_VOL         ES8388_ADCCONTROL9

#define ES8388_LDAC_VOL         ES8388_DACCONTROL4
#define ES8388_RDAC_VOL         ES8388_DACCONTROL5

#define ES8388_LOUT1_VOL        ES8388_DACCONTROL24
#define ES8388_ROUT1_VOL        ES8388_DACCONTROL25
#define ES8388_LOUT2_VOL        ES8388_DACCONTROL26
#define ES8388_ROUT2_VOL        ES8388_DACCONTROL27

#define ES8388_ADC_MUTE         ES8388_ADCCONTROL7
#define ES8388_DAC_MUTE         ES8388_DACCONTROL3

#define ES8388_IFACE            ES8388_MASTERMODE

#define ES8388_ADC_IFACE        ES8388_ADCCONTROL4
#define ES8388_ADC_SRATE        ES8388_ADCCONTROL5

#define ES8388_DAC_IFACE        ES8388_DACCONTROL1
#define ES8388_DAC_SRATE        ES8388_DACCONTROL2

/*CONFIGURATION REGISTER DEFINITION*/
#define ES8374_00_RESET                         0x00
#define ES8374_01_CLOCK_MANAGER                 0x01
#define ES8374_02_CLOCK_MANAGER                 0x02   
#define ES8374_03_CLOCK_MANAGER                 0x03
#define ES8374_04_CLOCK_MANAGER                 0x04
#define ES8374_05_CLOCK_MANAGER                 0x05   
#define ES8374_06_CLOCK_MANAGER                 0x06
#define ES8374_07_CLOCK_MANAGER                 0x07
#define ES8374_08_CLOCK_MANAGER                 0x08
#define ES8374_09_CLOCK_MANAGER                 0x09
#define ES8374_0A_CLOCK_MANAGER                 0x0A
#define ES8374_0B_CLOCK_MANAGER                 0x0B
#define ES8374_0C_CLOCK_MANAGER                 0x0C
#define ES8374_0D_CLOCK_MANAGER                 0x0D    
#define ES8374_0E_CLOCK_MANAGER                 0x0E    
#define ES8374_0F_SDP                           0x0F  
#define ES8374_10_SDP                           0x10
#define ES8374_11_SDP                           0x11
#define ES8374_12_SYSTEM                        0x12
#define ES8374_13_SYSTEM                        0x13   
#define ES8374_14_ANALOG_REFERENCE              0x14    
#define ES8374_15_ANALOG_POWER_DOWN             0x15
#define ES8374_16_ANALOG_LOW_POWER_MODE         0x16
#define ES8374_17_REFERENCE_AND_POWER_MODE      0x17
#define ES8374_18_BIAS_SELECTION                0x18
//0x19
#define ES8374_1A_MONO_OUT_SELECTION            0x1A
#define ES8374_1B_MONO_OUT_GAIN                 0x1B
#define ES8374_1C_MIXER                         0x1C
#define ES8374_1D_MIXER_GAIN                    0x1D
#define ES8374_1E_SPEAKER                       0x1E
#define ES8374_1F_SPEAKER                       0x1F
#define ES8374_20_SPEAKER                       0x20
#define ES8374_21_PGA                           0x21
typedef struct
{
    uint32_t index;
    uint32_t ostick;
    uint32_t buf_flag;/*0xff:last buf*/
    uint16_t *buf;
}msg_pcm_buf_queue_t;

#define ES8374_22_PGA_GAIN          0x22
//0x23
#define ES8374_24_ADC_CONTROL       0x24
#define ES8374_25_ADC_CONTROL       0x25
#define ES8374_26_ALC_CONTROL       0x26
#define ES8374_27_ALC_CONTROL       0x27
#define ES8374_28_ALC_CONTROL       0x28
#define ES8374_29_ALC_CONTROL       0x29
#define ES8374_2A_ALC_CONTROL       0x2A
#define ES8374_2B_ALC_CONTROL       0x2B
#define ES8374_2C_ADC_CONTROL       0x2C
#define ES8374_2D_ADC_CONTROL       0x2D
#define ES8374_2E_ADC_CONTROL       0x2E
#define ES8374_2F_ADC_CONTROL       0x2F
#define ES8374_30_ADC_CONTROL       0x30
#define ES8374_31_ADC_CONTROL       0x31  
#define ES8374_32_ADC_CONTROL       0x32  
#define ES8374_33_ADC_CONTROL       0x33  
#define ES8374_34_ADC_CONTROL       0x34  
#define ES8374_35_ADC_CONTROL       0x35  
#define ES8374_36_DAC_CONTROL       0x36  
#define ES8374_37_DAC_CONTROL       0x37  
#define ES8374_38_DAC_CONTROL       0x38  
#define ES8374_39_DAC_CONTROL       0x39  
#define ES8374_3A_DAC_CONTROL       0x3A  
#define ES8374_3B_DAC_CONTROL       0x3B  
#define ES8374_3C_DAC_CONTROL       0x3C  
#define ES8374_3D_DAC_CONTROL       0x3D  
#define ES8374_3E_DAC_CONTROL       0x3E  
#define ES8374_3F_DAC_CONTROL       0x3F  
#define ES8374_40_DAC_CONTROL       0x40  
#define ES8374_41_DAC_CONTROL       0x41  
#define ES8374_42_DAC_CONTROL       0x42  
#define ES8374_43_DAC_CONTROL       0x43  
#define ES8374_44_DAC_CONTROL       0x44  
#define ES8374_6D_DAC_CONTROL       0x6D 

#define ES8374_71_DAC_CONTROL       0x71 

#define ES8374_73_DAC_CONTROL       0x73 

ES_IC_TYPE g_ESIC_Type = IC_TYPE_8388;
extern void IISx_TXInit_noenable(IIS_TypeDef* IISx,IIS_DMA_TXInit_Typedef* IIS_DMA_Init_Struct);

char* play_buf;
uint8_t play_buf_index;

void DelayMs(int ms)
{
    int i=0;
    while(ms--)
    {
        for(i=0;i<0x1A80;i++);
    }
}

void I2C_WriteEs8388Data(unsigned int regaddr,char val)
{
    int ret, WriteTimes;
    
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};    
    struct i2c_client client = {0};
    
    WriteTimes = 0x00;
WriteLoop:    
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 80;// 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 50000000;/*40 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*0:write*/
    client.addr = 0x10;/*ES8388 device addr 0x10*/
    strcpy(client.name,"es8388"); 
    buf[0] = regaddr;
    buf[1] = val;
    ret = i2c_master_send(IIC0,&client,buf,2);
    if(RETRUN_ERR == ret)
    {
        WriteTimes++;
        if(3>WriteTimes)
            goto  WriteLoop;  
    }
//    DelayMs(5);
}    

int I2C_ReadEs8388Data(unsigned int regaddr)
{
    int ret,WriteTimes;
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_msg msgs[2];
    struct i2c_client client;
    WriteTimes = 0x00;
    
WriteLoop:
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 80;/*100k*/
    InitStruct.I2C_INPUT_CLK = 50000000;/*50 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*1:read*/
    client.addr = 0x10;/*es8388 device addr 0x10*/
    
    buf[0] = regaddr;
    msgs[0].addr = client.addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = (unsigned char*)buf;
    msgs[1].addr = client.addr;
    msgs[1].flags = 1;
    msgs[1].len = 1;
    msgs[1].buf = (unsigned char*)buf;

    ret = i2c_master_transfer(&i2c0_master,msgs,2);
    if(RETRUN_ERR == ret)
    {
        WriteTimes++;
        if(3>WriteTimes)
            goto  WriteLoop;  
    }
    return (unsigned char)buf[0];   
}

void I2C_WriteEs8374Data(unsigned int regaddr,char val)
{
    I2C_WriteEs8388Data(regaddr,val);
}

int I2C_ReadEs8374Data(unsigned int regaddr)
{
    return I2C_ReadEs8388Data(regaddr);
}
void write_smt_cs42l52(unsigned int regaddr,char val)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_client client = {0};
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 40000000;/*40 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*0:write*/
    client.addr = 0x4a;/*cs42l52 device addr*/
    strcpy(client.name,"smt_cs42l52"); 
    buf[0] = regaddr;
    buf[1] = val;
    i2c_master_send(IIC0,&client,buf,2);
}

int read_smt_cs42l52(unsigned int regaddr)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_msg msgs[2];
    struct i2c_client client;
    InitStruct.I2C_IO_BASE = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 40000000;/*40 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC0,&InitStruct);
    client.flags = 0;/*1:read*/
    client.addr = 0x4a;/*cs42l52 device addr*/
    
    buf[0] = regaddr;
    msgs[0].addr = client.addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = (unsigned char*)buf;
    msgs[1].addr = client.addr;
    msgs[1].flags = 1;
    msgs[1].len = 1;
    msgs[1].buf = (unsigned char*)buf;

    i2c_master_transfer(&i2c0_master,msgs,2);
    return (unsigned char)buf[0];
}

void cs42l52_RequiredInit(void)
{
    unsigned  char tmp = 0;
      write_smt_cs42l52(0x00,0x99);
    write_smt_cs42l52(0x3E,0XBA);
    write_smt_cs42l52(0X47,0X80);
    tmp = read_smt_cs42l52(0x32);
    write_smt_cs42l52(0X32,(1<<7)|tmp);
    tmp = read_smt_cs42l52(0x32);
    write_smt_cs42l52(0x32,(~(1<<7))&tmp);
    write_smt_cs42l52(0x00,0x00);
    for(volatile  unsigned int delay =0;delay<0xFFF;delay++);
}

void SequentailWrite_cs42l52(char wordaddr,char *pbuf,int cnt)
{
    I2C_InitStruct InitStruct = {0};
    char buf[256] = {0};
    struct i2c_client client = {0};
    InitStruct.I2C_IO_BASE = (unsigned int)IIC1;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 10000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC1,&InitStruct);
    client.flags = 0;/*0:write*/
    client.addr = 0x4a;/* device addr*/
    strcpy(client.name,"cs42l52"); 
    buf[0] = (wordaddr |(1<<7));
    strncpy(&buf[1],pbuf,cnt);
    i2c_master_send(IIC1,&client,buf,cnt+1);
}

void SequentailRead_cs42l52(char regaddr,char *pbuf,int cnt)
{
    I2C_InitStruct InitStruct = {0};
    char buf[2] = {0};
    struct i2c_msg msgs[2];
    struct i2c_client client;
    InitStruct.I2C_IO_BASE = (unsigned int)IIC1;
    InitStruct.I2C_CLOCK_SPEED = 100;/*100k*/
    InitStruct.I2C_INPUT_CLK = 10000000;/*10 Mhz*/
    InitStruct.TIMEOUT = 0X5FFFFF;
    i2c_init(IIC1,&InitStruct);

    client.flags = 0;/*1:read*/
    client.addr = 0x4a;/*cs42l52 device addr*/
    
    buf[0] = (regaddr |(1<<7));
    msgs[0].addr = client.addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = (unsigned char*)buf;
    msgs[1].addr = client.addr;
    msgs[1].flags = 1;
    msgs[1].len = cnt;
    msgs[1].buf = (unsigned char*)pbuf;
    i2c_master_transfer(&i2c1_master,msgs,2);
}

void reset_20921(void)
{ 
    DelayMs(10);
    RST_20921_LOW;
//    DelayMs(200);
    DelayMs(2);            //Start up time as 10ms
    RST_20921_HIGH;
    
}

void GPIO_I2C_STOP(void)
{
    Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
    Scu_SetIOReuse(I2C0_SCL_PAD,THIRD_FUNCTION);
    Scu_SetIOReuse(I2C0_SDA_PAD,THIRD_FUNCTION);
    
    GPIO_Output(GPIO0,GPIO_Pin13,1);
    GPIO_Output(GPIO0,GPIO_Pin14,1);
    
    /*set SCL 0*/
    GPIO_DATA_Set(GPIO0,GPIO_Pin13,0);
    DelayMs(1);
    /*set SDA 0*/
    GPIO_DATA_Set(GPIO0,GPIO_Pin14,0);
    /*DELAY  SET SCL 1*/
    DelayMs(1);
    GPIO_DATA_Set(GPIO0,GPIO_Pin13,1);
    DelayMs(1);
    /*DELAY SET SDA 1*/
    GPIO_DATA_Set(GPIO0,GPIO_Pin14,1);
    DelayMs(1);
}

void es8388_i2c_init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
 //   GPIO_I2C_STOP();
    Scu_SetDeviceGate((unsigned int)IIC0,ENABLE);
    Scu_Setdevice_Reset((unsigned int)IIC0);
    NVIC_InitStruct.NVIC_IRQChannel = IIC0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_ResetRelease((unsigned int)IIC0);
    Scu_SetIOReuse(I2C0_SCL_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2C0_SDA_PAD,FIRST_FUNCTION);
}

void cs42l52_i2c_init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIC0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    Scu_SetDeviceGate((unsigned int)IIC0,ENABLE);
    Scu_Setdevice_Reset((unsigned int)IIC0);
    Scu_Setdevice_ResetRelease((unsigned int)IIC0);
    Scu_SetIOReuse(I2C0_SCL_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2C0_SDA_PAD,FIRST_FUNCTION);
}

void cs42l52_test(void)
{
    char w_buf[20]={"ABCDEFG"},r_buf[20] = {0};
    cs42l52_i2c_init();
    mprintf("0x1=%x\n",read_smt_cs42l52(0x01));
    
    write_smt_cs42l52(0x2,0x32);
    //write_smt_cs42l52(0x3,0x33);
    //write_smt_cs42l52(0x4,0x34);
    //write_smt_cs42l52(0x5,0x35);
    mprintf("0x2=%x\n",read_smt_cs42l52(0x02));
    SequentailWrite_cs42l52(0x2,w_buf,3);
    //mprintf("0x2=%x\n",read_smt_cs42l52(0x02));
    SequentailRead_cs42l52(0x2,r_buf,5);
    mprintf("r_buf %s\n",r_buf);
}


#define CS42L52_ID 0XE3
int cs42l52_init(CS42L52Init_TypeDef* CS42L52Init_Struct)
{
    cs42l52_i2c_init();
    cs42l52_RequiredInit();
    CS42L52_DefaultConfig();
    head_tail_regs_test();
    return 0;
}


void CODE_CS42L52Config(void)
{
    cs42l52_i2c_init();
    //Codec_PowerDown_Entile_Codec(0);
    Codec_PowerOnInitial(0);
    //Codec_SetSampleRate(FS_16KHz);
    Codec_Set_Music_Dac_Mode(0);
    Codec_SetVolume(0,200);
}

void i2s1_record_realtime_play(void)
{
    CS42L52Init_TypeDef CS42L52Init_Struct = {0};
    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct = {0};
    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,0x18);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.txaddr0 = 0x70000000;
    IIS_DMA_Init_Struct.txaddr1 = 0x70000000+0x7d20;
    IIS_DMA_Init_Struct.rollbackaddr0size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rollbackaddr1size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.tx0singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.tx1singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.txdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.txdatafmt = IIS_TXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
    RX_IIS_DMA_Init_Struct.rxaddr = 0x70000000;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1001RollbackADDR;
    RX_IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE32bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
    
    CS42L52Init_Struct.Mclk =12000000;
    CS42L52Init_Struct.Lrck =3200000;
    cs42l52_init(&CS42L52Init_Struct);
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);    
}

void i2s1_record_realtime_play_handler(void)
{
    static unsigned int addr = 0x7d20,flag = 0;
    if(!flag)
    {
      IISxDMA_RADDR(IIS1DMA,0x70000000 + addr);
    }
    else
    {
      IISxDMA_RADDR(IIS1DMA,0x70000000 + 0);
    }
    flag = !flag;
}

void i2s_slave_record_test(void)
{
//    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct={0};
    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,16);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    
    RX_IIS_DMA_Init_Struct.rxaddr = 0x70000000;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1001RollbackADDR;
    RX_IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE32bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
 
    //cs42l52_init(&CS42L52Init_Struct);
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);

}

void i2s_cs42l52_record(unsigned int sramaddr,unsigned int bytesize)
{
    //CS42L52Init_TypeDef CS42L52Init_Struct = {0};
    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct = {0};
    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,26);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.txaddr0 = 0x70000000;
    IIS_DMA_Init_Struct.txaddr1 = 0x70000000 + 0x7d20;
    IIS_DMA_Init_Struct.rollbackaddr0size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rollbackaddr1size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.tx0singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.tx1singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.txdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.txdatafmt = IIS_TXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
    RX_IIS_DMA_Init_Struct.rxaddr = sramaddr;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize = (IISDMA_RXTXxRollbackADDR)(bytesize/(8*4*32) -1);
    RX_IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE128bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
    cs42l52_i2c_init();
    cs42l52_RequiredInit();
    mic_spk_cfg();
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
    cs42l52_mute(1);
    IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_TX_EN,DISABLE);
}

void i2splay_16k_16bit_test(void)
{
    CS42L52Init_TypeDef CS42L52Init_Struct = {0};
    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct = {0};
//    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct={0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,16*2);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.txaddr0 = 0x70000000;
    IIS_DMA_Init_Struct.txaddr1 = 0x70000000+0x7d20;
    IIS_DMA_Init_Struct.rollbackaddr0size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rollbackaddr1size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.tx0singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.tx1singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.txdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.txdatafmt = IIS_TXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample384Fs;
    
    CS42L52Init_Struct.Mclk =12000000;
    CS42L52Init_Struct.Lrck =3200000;
    cs42l52_init(&CS42L52Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
}
volatile  unsigned int flag_1 = 0,flag_2 = 0,flag_start = 0;

struct wav_struct
{
    char chRIFF[4];
     int dwRIFFLen;
     char chWAVE[4];
     char chFMT[4];
     int dwFMTLen;
     short pcmfmt;
    short channel;
    unsigned int frequency;
    int bps;
    unsigned short datablock;
    unsigned short bits;
     char chDATA[4];
     unsigned int dwDATALen;
} WAV;
const struct iiscfg_struct
{    
    IIS_OverSample oversample;
    unsigned char  mclk_div;
    unsigned int frq;
}IISCFG[]=
{
    //PLL 400Mhz
      {IIS_OverSample384Fs,63, 8260},{IIS_OverSample384Fs,43,12000},
    {IIS_OverSample384Fs,47,11080},{IIS_OverSample256Fs,49,16000},
    {IIS_OverSample384Fs,24,22050},{IIS_OverSample384Fs,22,24000},
    {IIS_OverSample384Fs,16,32000},{IIS_OverSample384Fs,12,44100},
    {IIS_OverSample384Fs,11,48000},{IIS_OverSample384Fs,8,64000},
    {IIS_OverSample256Fs, 9,88200},{IIS_OverSample256Fs,8,96000},
};
#define Decode_BufSize (1024)//(512)
#define Decode_BufNum (4)
char* wavplay_buf[2] ={0};

void iis1_ES8388_play_cmd()
{
    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct = {0};
    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,49);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.txaddr0 = 0x70000000;
    IIS_DMA_Init_Struct.txaddr1 = 0x70000000 + 0x7d20;
    IIS_DMA_Init_Struct.rollbackaddr0size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rollbackaddr1size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.tx0singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.tx1singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.txdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.txdatafmt = IIS_TXDATAFMT_IIS;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
#if USE_S_MIC_DENOISE

    RX_IIS_DMA_Init_Struct.rxaddr = DENOISE_IN_DATA_BASE_ADDR;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX32RollbackADDR;//(IISDMA_RXTXxRollbackADDR)(bytesize/(8*4*32) -1);
    RX_IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
    IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    //IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    
     IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);    
#else 
    RX_IIS_DMA_Init_Struct.rxaddr = 0x20020000;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;//(IISDMA_RXTXxRollbackADDR)(bytesize/(8*4*32) -1);
    RX_IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
    IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    //IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    
     IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
#endif 
     
}
  
void iis1_ES8388_rec_test()
{
    IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct = {0};
    IIS_DMA_RXInit_Typedef RX_IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};

    //清空准备写入的flash空间
    memset((void *)0x70500000,0xff,1024*1024);

    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
    
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)IIS1,49);
    Scu_SetClkDiv_Enable((unsigned int)IIS1);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.txaddr0 = 0x70000000;
    IIS_DMA_Init_Struct.txaddr1 = 0x70000000 + 0x7d20;
    IIS_DMA_Init_Struct.rollbackaddr0size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rollbackaddr1size = IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.tx0singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.tx1singlesize = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.txdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.txdatafmt = IIS_TXDATAFMT_IIS;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    RX_IIS_DMA_Init_Struct.rxaddr          = 0x70500000;
    RX_IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    RX_IIS_DMA_Init_Struct.rollbackaddrsize= IISDMA_RXTX64RollbackADDR;
    RX_IIS_DMA_Init_Struct.rxsinglesize    = IISDMA_TXRXSINGALESIZE16bytes;
    RX_IIS_DMA_Init_Struct.rxdatabitwide   = IIS_TXRXDATAWIDE16BIT;
    RX_IIS_DMA_Init_Struct.sck_lrck        = IIS_BUSSCK_LRCK64;
    RX_IIS_DMA_Init_Struct.rxdatafmt       = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    RX_IIS_DMA_Init_Struct.oversample      = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&RX_IIS_DMA_Init_Struct);
    IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
    IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,DISABLE);
    
     IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
}


unsigned int decodeaddr=0,decode_counter=0,decode_step=Decode_BufSize/8,WavFileSize = 0;
static char* wav_buf[2]={0},i2s_end = 0;

void iis1_play_pcm_handler(void)
{
    static uint8_t dma_buf_index=0;
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken_buf = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    msg_pcm_buf_queue_t buf_msg;
    static uint32_t dma_buf_addr;/*for under run,used last address*/

    /*here buf queue will used unblocked mode, so no HigherPriorityTaskWoken*/
    /*unblock list is more better? queue biger, but more safe*/
    xResult = xQueueReceiveFromISR(play_IT_queue,&buf_msg,&xHigherPriorityTaskWoken_buf);
    if(pdFAIL == xResult)
    {
        mprintf("play under run\n");
    }
    else
    {
        /*if under run,no update dma_buf_addr,just used last*/
        dma_buf_addr = (uint32_t)buf_msg.buf;
    }
    
    /*update dma buf*/
    if(0 == dma_buf_index)
    {
        IISxDMA_TADDR0(IIS1DMA,dma_buf_addr);
    }
    else
    {
        IISxDMA_TADDR1(IIS1DMA,dma_buf_addr);
    }

    /*record which dma tx int done*/ 
    dma_buf_index ^= 1;

    xResult = xQueueSendToBackFromISR(play_ack_queue,&dma_buf_addr,&xHigherPriorityTaskWoken);

    if((xResult != pdFAIL)&&(pdTRUE == xHigherPriorityTaskWoken))
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void iis1_cs42l52_adpcmplay_cmd_handler(void)
{
    static int odd = 0;

    if(decode_counter*decode_step >= WavFileSize)
    {
        WavFileSize = 0;
        if(0==i2s_end)
        {
            i2s_end =0x1;
        }

        memset(wav_buf[odd],0,Decode_BufSize);
    }else
    {
        int len =WavFileSize -decode_counter*decode_step;
        if(len<decode_step)
        {
            memset(wav_buf[odd],0,Decode_BufSize);
            de_codeadpcm_mono_to_stereo((char*)(decodeaddr+decode_counter*\
                decode_step),len,(short*)wav_buf[odd]);
        }
        else
        {
            de_codeadpcm_mono_to_stereo((char*)(decodeaddr+decode_counter*\
                      decode_step),decode_step,(short*)wav_buf[odd]);
        }
        decode_counter++;
    }
    odd =!odd;
    flag_1++;
}


void iis1_record_handler(void)
{
     static unsigned int addr = 0;
     addr += 0x7d20;
     IISxDMA_RADDR(IIS1DMA,0x70000000 + addr);
     if(addr > 8 * 1024 * 1024)
     {
        IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,DISABLE);
        mprintf("%s done\n",__func__);
     }
}


void de_codeadpcm(char* src,unsigned int size,short* dst)
{
     unsigned int realsize = size;
     unsigned char tmp = 0,*psrc = (unsigned char*)src;
     for(unsigned int i = 0; i < realsize; i++)
     {
        tmp = *psrc++;
        *dst++ = ADPCM_Decode(tmp & 0xF);
        *dst++ = ADPCM_Decode(0xF & (tmp >> 4));
     }
}

void de_codeadpcm_mono_to_stereo(char* src,unsigned int size,short* dst)
{
     unsigned int realsize = size;
     unsigned char tmp = 0,*psrc = (unsigned char*)src;
     short dsttmp = 0;
     if(!src ||!dst ||!size)
     {
        return ;
     }
     for(unsigned int i = 0; i < realsize; i++)
     {
        tmp = *psrc++;
        dsttmp = ADPCM_Decode(tmp & 0xF);
        *dst++ = dsttmp;
        *dst++ = dsttmp;
        dsttmp = ADPCM_Decode((tmp >> 4) & 0xF);
        *dst++ = dsttmp;
        *dst++ = dsttmp;
     }
}

void de_codeadpcm_stereo_to_stereo(char* src,unsigned int size,short* dst)
{
     unsigned int realsize = size;
     unsigned char tmp = 0,*psrc = (unsigned char*)src;
     short dsttmp = 0;
     for(unsigned int i = 0; i < realsize; i++)
     {
        tmp = *psrc++;
        dsttmp = ADPCM_Decode(tmp & 0xF);
        *dst++ = dsttmp;
        dsttmp = ADPCM_Decode((tmp >> 4) & 0xF);
        *dst++ = dsttmp;
     }
}

void mute_on_es8388_output(void)
{
//    I2C_WriteEs8388Data(0x19,0x24);
}

void mute_off_es8388_output(void)
{
//    I2C_WriteEs8388Data(0x19,0x20);
}

void iis1_8374_play_init(void)
{
    mprintf("\n enter 8374 init \n");

    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);

    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
   
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    IIS_CHSELConfig(IIS1,IIS_AUDIO_OUTPUT,DISABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
   // es8388_i2c_init();

    mprintf("es8374 init\n");
    I2C_WriteEs8374Data(ES8374_00_RESET,0x3f);    //IC rst start
    mprintf("step 1\n");
    DelayMs(50);
    I2C_WriteEs8374Data(ES8374_00_RESET,0x03);     //IC rst stop
    //mprintf("i2s read 0x00 = 0x%x\n",realtek_i2c_read(0x00));
    I2C_WriteEs8374Data(ES8374_01_CLOCK_MANAGER,0x6e); //IC clk on    
    I2C_WriteEs8374Data(ES8374_05_CLOCK_MANAGER,0x11); //clk div set
    I2C_WriteEs8374Data(0x06,0x01); 
    I2C_WriteEs8374Data(0x07,0x00); 
    I2C_WriteEs8374Data(0x0f,0x08); 
    /* PLL MODE */
    I2C_WriteEs8374Data(0x6f,0xa0);    //pll set : mode enable
    I2C_WriteEs8374Data(0x72,0x41);    //pll set: mode set
    I2C_WriteEs8374Data(ES8374_09_CLOCK_MANAGER,0x1);    //pll set:reset on,set start
    I2C_WriteEs8374Data(ES8374_0C_CLOCK_MANAGER,0x08); //pll set : k 22 08
    I2C_WriteEs8374Data(ES8374_0D_CLOCK_MANAGER,0x13);    //pll set: k 2e 13
    I2C_WriteEs8374Data(ES8374_0E_CLOCK_MANAGER,0xe0);    //pll set :k c6 e0
    I2C_WriteEs8374Data(ES8374_0A_CLOCK_MANAGER,0x3a);    //pll set:
    I2C_WriteEs8374Data(ES8374_0B_CLOCK_MANAGER,0x07);    //pll set: n
    I2C_WriteEs8374Data(ES8374_09_CLOCK_MANAGER,0x41);    //pll set reset off, set stop
    I2C_WriteEs8374Data(ES8374_24_ADC_CONTROL,0x08);    //ADC set
    I2C_WriteEs8374Data(ES8374_36_DAC_CONTROL,0x00);    //DAC set
    I2C_WriteEs8374Data(ES8374_10_SDP,0x0d);    //adc left justified 16bit  modify 0x0c
    I2C_WriteEs8374Data(ES8374_11_SDP,0x0c);    //dac std iis 16bit
    I2C_WriteEs8374Data(ES8374_12_SYSTEM,0x30);    //timming set
    I2C_WriteEs8374Data(ES8374_13_SYSTEM,0x20);    //timming set        
    //input 1
    I2C_WriteEs8374Data(ES8374_21_PGA,0x50);
    I2C_WriteEs8374Data(ES8374_22_PGA_GAIN,0x52);//0x55//0c50//modify 0x77    
    I2C_WriteEs8374Data(ES8374_21_PGA,0x10);//0x14    //ADC set: SEL LIN2 CH+PGAGAIN=15 db
/**************By Terangel 20171225***************************/
    I2C_WriteEs8374Data(ES8374_2C_ADC_CONTROL,0x05);//High Pass Filter    
    I2C_WriteEs8374Data(ES8374_2D_ADC_CONTROL,0x05);    
    
    I2C_WriteEs8374Data(ES8374_00_RESET,0x80);    //IC start
    DelayMs(20);
    I2C_WriteEs8374Data(ES8374_14_ANALOG_REFERENCE,0x8a);    //ic start
    I2C_WriteEs8374Data(ES8374_15_ANALOG_POWER_DOWN,0x40);    //ic start  //0x40
    I2C_WriteEs8374Data(ES8374_1A_MONO_OUT_SELECTION,0xe1);    //mono out set    
//    I2C_WriteEs8374Data(ES8374_1A_MONO_OUT_SELECTION,0xa0);    //mono out set
    I2C_WriteEs8374Data(ES8374_1B_MONO_OUT_GAIN,0x0B);    //mono out set     0x19
    I2C_WriteEs8374Data(ES8374_1C_MIXER,0x50);    // spk set
//    I2C_WriteEs8374Data(ES8374_1C_MIXER,0x90);    // spk set
    I2C_WriteEs8374Data(ES8374_1D_MIXER_GAIN,0x0B);    //spk set
//    I2C_WriteEs8374Data(ES8374_1D_MIXER_GAIN,0x00);    //spk set
//    I2C_WriteEs8374Data(ES8374_1F_SPEAKER,0x06);    //spk set, Circuit Protection, Speaker D low power mode
    I2C_WriteEs8374Data(ES8374_1F_SPEAKER,0x07);    //spk set, Circuit Protection, Speaker D low power mode   
    I2C_WriteEs8374Data(ES8374_1E_SPEAKER,0x00);    //spk on//20180224
//    I2C_WriteEs8374Data(ES8374_1E_SPEAKER,0xa7);    //spk on//20180224

    I2C_WriteEs8374Data(ES8374_25_ADC_CONTROL,0x00);    //adc volume on
    I2C_WriteEs8374Data(ES8374_38_DAC_CONTROL,0x00);    //dac volumel on
    I2C_WriteEs8374Data(ES8374_08_CLOCK_MANAGER,0x08);    //dac volumel on

    I2C_WriteEs8374Data(ES8374_37_DAC_CONTROL,0x30);    //auto mute
    I2C_WriteEs8374Data(ES8374_6D_DAC_CONTROL,0x60);    //sel:GPI1=DMIC CLK OUT+SEL:GPO2=PLL CLK OUT
    /*set adc alc funtcion*/
/**************Modified By Terangel 20180129***************************/
    I2C_WriteEs8374Data(ES8374_26_ALC_CONTROL,0x4A);//0x4c    //ALC MAX GAIN  modify  0x50    
    I2C_WriteEs8374Data(ES8374_28_ALC_CONTROL,0x70);    //ALC LEVEL   modify  0x80
//    I2C_WriteEs8374Data(ES8374_26_ALC_CONTROL,0x4C);//0x4c    //ALC MAX GAIN  modify  0x50    
//    I2C_WriteEs8374Data(ES8374_28_ALC_CONTROL,0x80);    //ALC LEVEL   modify  0x80
    I2C_WriteEs8374Data(ES8374_27_ALC_CONTROL,0x08);  //0x08
    I2C_WriteEs8374Data(ES8374_29_ALC_CONTROL,0x05); //0x12    
    /*DAC OPEN*/    
    I2C_WriteEs8374Data(ES8374_71_DAC_CONTROL,0x05); 
    I2C_WriteEs8374Data(ES8374_73_DAC_CONTROL,0x70);    
    DelayMs(20);
    I2C_WriteEs8374Data(ES8374_36_DAC_CONTROL,0x40);
    DelayMs(20);
    I2C_WriteEs8374Data(ES8374_36_DAC_CONTROL,0x00);    
    I2C_WriteEs8374Data(ES8374_01_CLOCK_MANAGER,0x7f); //IC clk on
    mprintf("es8374 init end \n"); 
    

#if USE_ORIGIN_PLAY_FLOW
    wav_buf[0]=(char*)ci100x_malloc(Decode_BufSize);
    if(!wav_buf[0])
    {
        mprintf("wav_buf[0] 分配失败！\n");
    }
    wav_buf[1]=(char*)ci100x_malloc(Decode_BufSize);
    if(!wav_buf[1])
    {
        mprintf("wav_buf[1] 分配失败！\n");
    }  
#else
    play_buf = (char*)ci100x_malloc(Decode_BufSize*Decode_BufNum);
    configASSERT(play_buf);
    play_buf_index = 0;
#endif
}

void iis1_8388_play_init(void)
{
    mprintf("\n enter 8388 init \n");
    Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);

    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDO_PAD,FIRST_FUNCTION);
   
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    IIS_CHSELConfig(IIS1,IIS_AUDIO_OUTPUT,DISABLE);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
    
    unsigned char readbuf[2];
    unsigned char cnt_reinit=0;
    
ES8388_REINIT:

   // es8388_i2c_init();
    I2C_WriteEs8388Data(0x0,0X86);
    DelayMs(100);
    I2C_WriteEs8388Data(0x0,0X06);
    DelayMs(100);

    I2C_WriteEs8388Data(ES8388_MASTERMODE,0x0);
    I2C_WriteEs8388Data(0x2E,0X0);//vol
    I2C_WriteEs8388Data(0x2F,0x0);
    I2C_WriteEs8388Data(0x30,0x0); //Lout2 
    I2C_WriteEs8388Data(0x31,0x0); //Rout2 
    
    I2C_WriteEs8388Data(ES8388_CHIPPOWER,0xF3);

    I2C_WriteEs8388Data(ES8388_DACCONTROL21,0x80);
#if ES8388_DAC_MICIN_AEC      
    I2C_WriteEs8388Data(ES8388_CONTROL1,0x36);
#else
    I2C_WriteEs8388Data(ES8388_CONTROL1,0x16);
#endif

    I2C_WriteEs8388Data(ES8388_CONTROL2,0x72);
    I2C_WriteEs8388Data(ES8388_DACPOWER,0x00);
#if ES8388_DAC_MICIN_AEC      
	I2C_WriteEs8388Data(ES8388_ADCPOWER,0x00);
#endif
    I2C_WriteEs8388Data(ES8388_ANAVOLMANAG,0x7C);
#if ES8388_DAC_MICIN_AEC   
    I2C_WriteEs8388Data(ES8388_ADCCONTROL1,0);//mic amp L4bit 0x44//+
    I2C_WriteEs8388Data(ES8388_ADCCONTROL2,0x00);//+
    I2C_WriteEs8388Data(ES8388_ADCCONTROL3,0x02);
#if IIS0_RX___IIS_STD
    I2C_WriteEs8388Data(ES8388_ADCCONTROL4,0x14);//std iis
#else    
    I2C_WriteEs8388Data(ES8388_ADCCONTROL4,0x15);//?—?o?32bit 00 std 01 left 02 right (4d)
#endif  
    I2C_WriteEs8388Data(ES8388_ADCCONTROL5,0x03);/* 384 FS*/
    I2C_WriteEs8388Data(0x0F,0);
    I2C_WriteEs8388Data(0x12,0);//+
#else    
    I2C_WriteEs8388Data(ES8388_ADCCONTROL1,0xbb);//mic amp L4bit 0x44
    I2C_WriteEs8388Data(ES8388_ADCCONTROL2,0xf0);
    I2C_WriteEs8388Data(ES8388_ADCCONTROL3,0x02);
#if USE_ASR8388
    I2C_WriteEs8388Data(ES8388_ADCCONTROL4,0x0d);//bit[1:0] 0 I2S,1 LjI2S
    I2C_WriteEs8388Data(ES8388_ADCPOWER,0x00);
#else
    
#if USE_6_6_156_2_16
    I2C_WriteEs8388Data(ES8388_ADCCONTROL4,0x0D);//bit[1:0] 0 I2S,1 LjI2S
 //   I2C_WriteEs8388Data(ES8388_DAC_IFACE,0x1A);//bit[2:1] 0 I2S,1 LjI2S
#else
    I2C_WriteEs8388Data(ES8388_ADCCONTROL4,0x0C);//bit[1:0] 0 I2S,1 LjI2S
#endif
    
    
#endif 
    I2C_WriteEs8388Data(ES8388_ADCCONTROL5,0x03);/* 384 FS*/
    
    I2C_WriteEs8388Data(0x12,0xe2);
#endif    
     
    I2C_WriteEs8388Data(0x13,0xa0);
    I2C_WriteEs8388Data(0x14,0x05);
    I2C_WriteEs8388Data(0x15,0x06);
#if ES8388_DAC_MICIN_AEC  
    I2C_WriteEs8388Data(0x16,0x50);
#if IIS0_RX___IIS_STD
    I2C_WriteEs8388Data(0x17,0x20);//std iis
#else    
    I2C_WriteEs8388Data(0x17,0x22);//?—?3????//bit2+bit1  00???ostd 01???oleft 02???oright
#endif    
#else    
    I2C_WriteEs8388Data(0x16,0x51);
    I2C_WriteEs8388Data(0x17,0x18);
#endif    
    I2C_WriteEs8388Data(0x18,0x02);
    I2C_WriteEs8388Data(0x19,0x24);// 0X22 bit2 auto mute close---register 上面没标注
    I2C_WriteEs8388Data(0x1a,0x00);
    I2C_WriteEs8388Data(0x1b,0x00);
    I2C_WriteEs8388Data(0x26,0x00);
    I2C_WriteEs8388Data(0x27,0xb8);
    I2C_WriteEs8388Data(0x2A,0xb8);

#if ES8388_DAC_MICIN_AEC         
    I2C_WriteEs8388Data(0x2E,0x16);//vol
    I2C_WriteEs8388Data(0x2F,0x16);
    I2C_WriteEs8388Data(0x30,0x16); //Lout2 
    I2C_WriteEs8388Data(0x31,0x16); //Rout2  
#else   
    I2C_WriteEs8388Data(0x2E,0x0);//vol
    I2C_WriteEs8388Data(0x2F,0x0);
    I2C_WriteEs8388Data(0x30,0x0); //Lout2 
    I2C_WriteEs8388Data(0x31,0x0); //Rout2  
#endif
    I2C_WriteEs8388Data(ES8388_LADC_VOL,0x00);
    I2C_WriteEs8388Data(ES8388_RADC_VOL,0x00);
    I2C_WriteEs8388Data(ES8388_DACPOWER,0x3C);
    I2C_WriteEs8388Data(ES8388_CHIPPOWER,0x00);    
    
//-----check 8388 init success or not    
    readbuf[0]=I2C_ReadEs8388Data(0);
    readbuf[1]=I2C_ReadEs8388Data(1);
    if((readbuf[0]==0xff)&& (readbuf[1]==0xff) && cnt_reinit < 10)
    {
        mprintf("\r\n 8388 init err retry%d times....\r\n\r\n",cnt_reinit++);
        DelayMs(1000);
        goto ES8388_REINIT;
    }
    mprintf("8388 reg0x17=%d\r\n",I2C_ReadEs8388Data(0x17));
    if(cnt_reinit == 10)
    {
        mprintf("\r\n 8388 init err ,then while(1)....\r\n\r\n");
        while(1)
        {
            LED_VAD_ON;
            DelayMs(100);  
            LED_VAD_OFF;
            DelayMs(100);  
        }
    }
 
#ifdef USE_ES7144LV_DS
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);
    Scu_SetIOReuse(SPI1_CLK_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO1,GPIO_Pin8,1);
#endif
    
#ifdef USE_CS42L52
    unsigned char spkhp_cfg[] = 
    {
         0x0,0xE3,0x0,0x0,0xaa,0x80,0x4/*0x0 LJ;0X4 std i2s*/,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,
        0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0x0,0x0,0x0,
        0x0,0x0,0x88,0x0,0x0,0xea,0xea,0xea,0xea,0x0,0x0,0x7f,0xc0,0x0,
        0x3f,0x0,0x0,0x0,0x0,0x0,0x0,0x3b,0x0,0x5f, 
    };
    cs42l52_i2c_init();
    cs42l52_RequiredInit();
    CS42L52_DefaultConfig();
    for(int i=2;i<sizeof(spkhp_cfg)/\
        sizeof(spkhp_cfg[0]);i++)
    {
        write_smt_cs42l52(i,spkhp_cfg[i]);
    } 
    cs42l52_mute(1);
#endif 
#if USE_ORIGIN_PLAY_FLOW
    wav_buf[0]=(char*)ci100x_malloc(Decode_BufSize);
    if(!wav_buf[0])
    {
        mprintf("wav_buf[0] 分配失败！\n");
    }
    wav_buf[1]=(char*)ci100x_malloc(Decode_BufSize);
    if(!wav_buf[1])
    {
        mprintf("wav_buf[1] 分配失败！\n");
    }  
#else
    play_buf = (char*)ci100x_malloc(Decode_BufSize*Decode_BufNum);
    configASSERT(play_buf);
    play_buf_index = 0;
#endif  
}    

void iis1_play_init(void)
{
    g_ESIC_Type = IC_TYPE_8388;
    
    es8388_i2c_init();
    DelayMs(20);
    char readbuf = I2C_ReadEs8374Data(0x28);    

    if(readbuf == 0x28) //8388 the register 0x38 default value is 0x28 
    {
        g_ESIC_Type = IC_TYPE_8388;    

    }
    else
    {
        g_ESIC_Type = IC_TYPE_8374;    
    }

    if(g_ESIC_Type == IC_TYPE_8388)
    {
        iis1_8388_play_init();
    }
    else if(g_ESIC_Type == IC_TYPE_8374)
    {
        iis1_8374_play_init();
//        ES8374_CloseSpeaker();
    }
}

void ES8388_lowpower(void)
{
    if(g_ESIC_Type == IC_TYPE_8374)
    {
//        es8388_i2c_init();
//        
//        I2C_WriteEs8388Data(0x36,0x20);
//        I2C_WriteEs8388Data(0x37,0x21);
//        I2C_WriteEs8388Data(0x1A,0x08);
//        I2C_WriteEs8388Data(0x1C,0x10);
//        I2C_WriteEs8388Data(0x1D,0x10);
//        I2C_WriteEs8388Data(0x1E,0x20);
//        I2C_WriteEs8388Data(0x24,0x20);
//        I2C_WriteEs8388Data(0x22,0x00);
//        I2C_WriteEs8388Data(0x21,0xD4);
//        I2C_WriteEs8388Data(0x15,0xBF);
//        I2C_WriteEs8388Data(0x14,0x14);
//        I2C_WriteEs8388Data(0x01,0x03);
    }
    else if(g_ESIC_Type == IC_TYPE_8388)
    {
        I2C_WriteEs8388Data(ES8388_MASTERMODE,0x0);
        
        I2C_WriteEs8388Data(ES8388_CHIPPOWER,0xC3);

        I2C_WriteEs8388Data(ES8388_CONTROL1,0x16);
        I2C_WriteEs8388Data(ES8388_CONTROL2,0x3f);
        I2C_WriteEs8388Data(ES8388_ADCPOWER,0xff);
        I2C_WriteEs8388Data(ES8388_DACPOWER,0xc0);
        I2C_WriteEs8388Data(ES8388_CHIPLOPOW1,0xff);
        I2C_WriteEs8388Data(ES8388_CHIPLOPOW2,0xff);
    }
}
void E8388_LowPower_Exit(void)
{
    if(g_ESIC_Type == IC_TYPE_8374)
    {
//        es8388_i2c_init();
//        I2C_WriteEs8388Data(0x01,0x7f);
//        I2C_WriteEs8388Data(0x14,0x8a);
//        I2C_WriteEs8388Data(0x15,0x40);
//        I2C_WriteEs8388Data(0x21,0x14);
//        I2C_WriteEs8388Data(0x22,0x55);
//        I2C_WriteEs8388Data(0x24,0x08);
//        I2C_WriteEs8388Data(0x1e,0xa7);
//        I2C_WriteEs8388Data(0x1d,0x02);
//        I2C_WriteEs8388Data(0x1c,0x90);
//        I2C_WriteEs8388Data(0x1a,0x61);
//        I2C_WriteEs8388Data(0x37,0x30);
//        I2C_WriteEs8388Data(0x36,0x00);
    }
    else if(g_ESIC_Type == IC_TYPE_8388)
    {
        I2C_WriteEs8388Data(ES8388_MASTERMODE,0x00);
        
        I2C_WriteEs8388Data(ES8388_CHIPPOWER,0x00);

        I2C_WriteEs8388Data(ES8388_CONTROL1,0x16);
        I2C_WriteEs8388Data(ES8388_CONTROL2,0x72);
        I2C_WriteEs8388Data(ES8388_ADCPOWER,0x00);
        I2C_WriteEs8388Data(ES8388_DACPOWER,0x3c);
        I2C_WriteEs8388Data(ES8388_CHIPLOPOW1,0x00);
        I2C_WriteEs8388Data(ES8388_CHIPLOPOW2,0x00);
    }
}
////////////////////////play adpcm in spi////////
//vol min =0,max = 0x21
unsigned char const volume_table[]={7,14,19,24,28,30,31,32};
void vol_set(char vol)
{  
	int volvalue;

    if(vol<=VOLUME_MAX)
    {
        if(g_ESIC_Type == IC_TYPE_8374)
        {
        	// I2C_WriteEs8388Data(0x38,0x21-vol);
            volvalue= (0x21-volume_table[vol])*2;   
    	    I2C_WriteEs8388Data(0x38,volvalue);
        }
        else if(g_ESIC_Type == IC_TYPE_8388)
        {   
            I2C_WriteEs8388Data(0x2E,volume_table[vol]);
            I2C_WriteEs8388Data(0x2F,0x0);
            I2C_WriteEs8388Data(0x30,0x0); //Lout2 
            I2C_WriteEs8388Data(0x31,0x0); //Rout2
        }        
    }
}



#if PLAY_TASK_ENABLE

void test_reset_iis0(void)
{
    uint32_t iis0_reg_backup[4];
    /*already disable,so just backup and resume*/
    /*back reg*/
    memcpy(iis0_reg_backup,(uint32_t *)IIS0_BASE,sizeof(iis0_reg_backup));
    mprintf("\niisctrl-0x%x\n",iis0_reg_backup[1]);
    /*reset*/
    Scu_Setdevice_Reset(IIS0_BASE);
    Scu_Setdevice_ResetRelease(IIS0_BASE);

    /*restore reg*/
    memcpy((uint32_t *)IIS0_BASE,iis0_reg_backup,sizeof(iis0_reg_backup));
}
void test_reset_iis1(void)
{
    uint32_t iis1_reg_backup[4];
    memcpy(iis1_reg_backup,(uint32_t *)IIS1_BASE,sizeof(iis1_reg_backup));
    mprintf("\niisctrl-0x%x\n",iis1_reg_backup[1]);
    Scu_Setdevice_Reset(IIS1_BASE);
    Scu_Setdevice_ResetRelease(IIS1_BASE);

    memcpy((uint32_t *)IIS1_BASE,iis1_reg_backup,sizeof(iis1_reg_backup));
}
static int g_end_play_voice_time = -1;
int get_PlayVoice_EndTime(void)
{
   return g_end_play_voice_time;
}
void Reset_PlayVoice_EndTime(void)
{
  	 g_end_play_voice_time = -1;
}
void set_PlayVoice_time(void)
{
	g_end_play_voice_time = xTaskGetTickCount();
}
static uint16_t* get_next_pcm_buff(void)
{
    play_buf_index++;
    if(play_buf_index >= Decode_BufNum)
    {
        play_buf_index = 0;
    }
    return (uint16_t*)(play_buf+Decode_BufSize*play_buf_index);
}

void send_4_zero_buffer(void)
{
    msg_pcm_buf_queue_t buf_msg;
    BaseType_t xResult;
    for(uint8_t i=0; i<4; i++)
    {
        uint16_t* decbuf = get_next_pcm_buff();
        memset(decbuf,0,Decode_BufSize);

        buf_msg.buf_flag = 0xff;
        buf_msg.buf = decbuf;
        xResult = xQueueSend(play_IT_queue,&buf_msg,0);
    }
}

void ES8374_CloseSpeaker(void)
{
//    I2C_WriteEs8388Data(0x1E,0x20);
}

void ES8374_OpenSpeaker(void)
{
//    I2C_WriteEs8388Data(0x1b,0x09);
//	I2C_WriteEs8388Data(0x1d,0x09);
}

extern void CodeGPIOTimeTest(void);
extern int sysinit_index;
extern void SetPlayFlagOFF(void);
extern void SetPlayFlagON(void);
void  vTask_play (void *pvParameters)
{
    static unsigned int last_freq = 0;
    int cfg_index=-1,validwavsize=0;
    int index;
    int first_powerOn=1;
    unsigned char *WavAddr=NULL;//=(unsigned char *)(SPI_ADPCM_PLAY_STARTADDR);
    int WavSize;
    int16_t* decoded_buf;
    msg_pcm_buf_queue_t buf_msg;
   	play_IT_queue = xQueueCreate(6, sizeof(msg_pcm_buf_queue_t));
    if(play_IT_queue==0)
    {
        mprintf("play_IT_queue creat fail\r\n");   
    }
    
    play_ack_queue = xQueueCreate(6, sizeof(int));
    if(play_ack_queue==0)
    {
        mprintf("play_ack_queue creat fail\r\n");   
    }
    vTaskDelay(pdMS_TO_TICKS(100));  //wait other init ok
	

    while(1)
    {
        if(first_powerOn==1)
        {
            first_powerOn = 0;
            for(index=0;index<SUPPORT_MAX_VOICE;index++)
            {
                if(AsrCmd_Struct[index].id!=-1)
                {
                    if(!strcmp(AsrCmd_Struct[index].pCmd,"welcom"))    //AsrCmd_Struct[index].pCmd
                    {
                        mprintf("find welcom index=%d\r\n",index);
                        break;
                    }
                }else
                {
                    index = -1;
                    break;
                }
            }
            
#if USE_IWDG
            if(CPU_rstType.iwtd ||CPU_rstType.lockup ||\
              CPU_rstType.softrst||CPU_rstType.srstreq||\
              CPU_rstType.wwtd)
            {
                index = -1;
            }
#endif 
        }
        else
        {  
            if(pdPASS != xQueueReceive(play_Q, &index, portMAX_DELAY))
            {
                mprintf("play task Q_rec err\r\n");
                continue;
            }
     
        }
#if DECODE_DEBUG
        index =-1;
#endif
        if(index < 0)
        {
            mprintf("[ play task index error]\r\n");
            continue;
        }

        mprintf("play task index =%d\r\n",index);
#if 1
        if((AsrCmd_Struct[index].VoiceSize<2048) || (AsrCmd_Struct[index].VoiceSize>800*1024))
        {
            mprintf("playindex file size unnormal,not play ~,[%d]size=%x\r\n",index,AsrCmd_Struct[index].VoiceSize);
            continue;
        }
        Reset_PlayVoice_EndTime();    
        timer_stop(TIMER1);//
        //MUTE_OFF;
        if(g_ESIC_Type == IC_TYPE_8374)
        { 
            ES8374_OpenSpeaker();
        }
#if 0
        vTaskSuspendAll();
        WavAddr=(unsigned char *)malloc((AsrCmd_Struct[index].VoiceSize/4096+1)*4096);
        xTaskResumeAll();
        if(NULL == WavAddr)
        {
            mprintf("buf len=%d,malloc fail!!\r\n",(AsrCmd_Struct[index].VoiceSize/4096+1)*4096);
            TIMx_us(TIMER1,SUSPEND_TIME);
            continue ;
        }else
        {
            mprintf("malloc size=%d,bufsize=%d,WavAddr=%x \r\n",(AsrCmd_Struct[index].VoiceSize/4096+1)*4096,AsrCmd_Struct[index].VoiceSize,WavAddr);
        }
#else
        WavAddr = (unsigned char *)SDRAM_VOICE_PLAY_ADDR;//??????
        if(((AsrCmd_Struct[index].VoiceSize/4096+1)*4096) > (1*1024*1024 -1024))
        {
            mprintf("play voice size too large [%d]\n",((AsrCmd_Struct[index].VoiceSize/4096+1)*4096));
            TIMx_us(TIMER1,SUSPEND_TIME);            
            set_PlayVoice_time();
            continue ;
        }
        else
        {
            mprintf("play voice size %d",((AsrCmd_Struct[index].VoiceSize/4096+1)*4096));
            SetPlayFlagON();
        }
#endif
        WavSize = AsrCmd_Struct[index].VoiceSize;

        xSemaphoreTake(SpiWrMutex, portMAX_DELAY);
        flashtosdram((unsigned int )(WavAddr),AsrCmd_Struct[index].pVoiceAddr,AsrCmd_Struct[index].VoiceSize);
        xSemaphoreGive(SpiWrMutex);    
    /////////////////
        
        NVIC_InitTypeDef NVIC_InitStruct={0};
        IIS_DMA_TXInit_Typedef IIS_DMA_Init_Struct={0};
#if USE_ASR8388
#if USE_S_MIC_DENOISE
        IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,ENABLE);
#else
        IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,DISABLE);
#endif
        while(IISDMA_DATABUSBUSY(IIS1DMA));
        IIS_RXEN(IIS1,DISABLE);
        test_reset_iis1();    
#else
        IISDMA_ChannelENConfig(IIS0DMA,IISxDMA_RX_EN,DISABLE);
        while(IISDMA_DATABUSBUSY(IIS0DMA));
        IIS_RXEN(IIS0,DISABLE);
        
        test_reset_iis0();    
#endif 
        
        NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStruct);

        Decodeindex = 0;
        Decodepredsample = 0;
        i2s_end =0;

        memcpy(&WAV,(void*)WavAddr,sizeof(WAV));
        for(int i=0;i<sizeof(IISCFG)/sizeof(*IISCFG);i++)
        {
            if((unsigned int)(IISCFG[i].frq-(unsigned int)800)<=WAV.frequency && \
              WAV.frequency <=(unsigned int)(IISCFG[i].frq+(unsigned int)800))
            {
                cfg_index=i;
                break;
            }
        }
        if(-1==cfg_index||!decode_step) 
        {
            TIMx_us(TIMER1,SUSPEND_TIME);
            IISDMA_ChannelENConfig(IIS0DMA,IISxDMA_RX_EN,ENABLE);
            IIS_TXEN(IIS0,ENABLE);
            mprintf("[WAV fmt error]\n");
            set_PlayVoice_time();
            continue ;
        }
		
		static int play_iis = 0;
        if( WAV.frequency != last_freq)
        {
            Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
            Scu_Setdiv_Parameter((unsigned int)IIS1,IISCFG[cfg_index].mclk_div);
            Scu_SetClkDiv_Enable((unsigned int)IIS1);
            last_freq = WAV.frequency;
			play_iis = 0;
        }
        decodeaddr=(unsigned int )WavAddr+44;
        decode_counter=0;
#if USE_ORIGIN_PLAY_FLOW
#else
#if 1
        send_4_zero_buffer();
#else
        for(uint8_t i=0; i<Decode_BufNum-1; i++)
        {
            decoded_buf = get_next_pcm_buff();
            de_codeadpcm_mono_to_stereo((char*)(decodeaddr+decode_counter*decode_step),\
            decode_step,decoded_buf);
            decode_counter++;
            buf_msg.buf_flag = 0xff;
            buf_msg.buf = decoded_buf;
            xQueueSend(play_IT_queue,&buf_msg,0);
        }
#endif
#endif
        flag_1=0;
        flag_2=0;
        flag_start=0;
        validwavsize = ((WavSize-44)/64)*64;
        
        if(!play_iis)
        {
#if USE_ORIGIN_PLAY_FLOW
            memset(wav_buf[0],0,Decode_BufSize);
            memset(wav_buf[1],0,Decode_BufSize);
            IIS_DMA_Init_Struct.txaddr0 	= (unsigned int)wav_buf[0];
            IIS_DMA_Init_Struct.txaddr1		= (unsigned int)wav_buf[1];
#else
            memset(play_buf, 0, Decode_BufSize*Decode_BufNum);
            IIS_DMA_Init_Struct.txaddr0 	= (unsigned int)(play_buf);
            IIS_DMA_Init_Struct.txaddr1		= (unsigned int)(play_buf+Decode_BufSize);
#endif
            IIS_DMA_Init_Struct.rollbackaddr0size = (IISDMA_RXTXxRollbackADDR)(Decode_BufSize/16-1); 
            IIS_DMA_Init_Struct.rollbackaddr1size = (IISDMA_RXTXxRollbackADDR)(Decode_BufSize/16-1); 
            IIS_DMA_Init_Struct.tx0singlesize	= IISDMA_TXRXSINGALESIZE16bytes;
            IIS_DMA_Init_Struct.tx1singlesize	= IISDMA_TXRXSINGALESIZE16bytes;
            IIS_DMA_Init_Struct.txdatabitwide	= IIS_TXRXDATAWIDE16BIT;
            IIS_DMA_Init_Struct.sck_lrck		= IIS_BUSSCK_LRCK64;
#if USE_6_156_2_16
            IIS_DMA_Init_Struct.txdatafmt	= IIS_TXDATAFMT_IIS;//IIS_TXDATAFMT_LEFT_JUSTIFIED;
#else
            IIS_DMA_Init_Struct.txdatafmt    = IIS_TXDATAFMT_IIS;
#endif        
            IIS_DMA_Init_Struct.oversample    = IISCFG[cfg_index].oversample;
            //IISx_TXInit(IIS1,&IIS_DMA_Init_Struct);
			IISx_TXInit_noenable(IIS1,&IIS_DMA_Init_Struct);
            play_iis = 1;
///////////////////--szk add 20171108
            vTaskDelay(pdMS_TO_TICKS(300));
            if(g_ESIC_Type == IC_TYPE_8388)
            {
                MUTE_OFF;
            }
        }
#if !USE_ORIGIN_PLAY_FLOW
        else
        {
            //IISxDMA_TXADDRRollbackInterruptClear(IIS1DMA,IISDMA_TXAADRRollbackInterrupt_ADDR0);
            //IISDMA_RXTXClear(IIS1DMA,IISxDMA_TX_EN);
            IISxDMA_TADDR0(IIS1DMA,(unsigned int)(play_buf));
            IISxDMA_TADDR1(IIS1DMA,(unsigned int)(play_buf+Decode_BufSize));
        }
#endif
        //MUTE_OFF;
       // vTaskDelay(pdMS_TO_TICKS(200));

        IIS_TXEN(IIS1,ENABLE);
        IIS_EN(IIS1,ENABLE);
        IISDMA_EN(ENABLE);
        IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,ENABLE);
        IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_TX_EN,ENABLE);
        
		if(g_ESIC_Type == IC_TYPE_8388)
		{
        	mute_off_es8388_output();
		}

        volatile unsigned int timeout = 0xEFFFFFFF;
        WavFileSize = validwavsize;
        timeout = (unsigned int)(1000+4*((WavFileSize*1.0)/(2*WAV.frequency))*1000);// ms
         
        mprintf("play times:%d ms\n",timeout);
        int play_timeout = timeout ;
        int play_remain = 0,iis1_open = 0;
        unsigned int cur_tick =xTaskGetTickCount();
        while(((decode_counter*decode_step < validwavsize)  && WavFileSize))
        {
#if !USE_ORIGIN_PLAY_FLOW
            do{
#endif
                play_remain =  pdMS_TO_TICKS(play_timeout) - (xTaskGetTickCount()-cur_tick);
                if(xTaskGetTickCount()-cur_tick >pdMS_TO_TICKS(play_timeout))
                {
                    Scu_Setdevice_Reset(IIS1_BASE);
                    Scu_Setdevice_ResetRelease(IIS1_BASE);
                    play_iis =0;
                    mprintf("play over\n");
    #if USE_ASR8388
                    iis1_ES8388_play_cmd();
    #endif
                    //break;
                    goto end;
                }
#if !USE_ORIGIN_PLAY_FLOW
            }while(pdPASS!=xQueueReceive(play_ack_queue, &index, pdMS_TO_TICKS(16)));
#endif
            if(!iis1_open && (play_remain <= pdMS_TO_TICKS(200)))
            {
                iis1_open =1;
#if USE_ASR8388
#if USE_S_MIC_DENOISE
                IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
#else
                IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,DISABLE);
#endif 
                IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,ENABLE);
                IIS_RXEN(IIS1,ENABLE); 
                mprintf("[%d ]iis1 open\n",asrtimer_counter);
#else
                IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,DISABLE);
                IISDMA_ChannelENConfig(IIS0DMA,IISxDMA_RX_EN,ENABLE);
                IIS_RXEN(IIS0,ENABLE); 
#endif
            }
#if !USE_ORIGIN_PLAY_FLOW
            decoded_buf = get_next_pcm_buff();
            de_codeadpcm_mono_to_stereo((char*)(decodeaddr+decode_counter*decode_step),\
            decode_step,decoded_buf);
            decode_counter++;
            buf_msg.buf_flag = 0xff;
            buf_msg.buf = decoded_buf;
            xQueueSend(play_IT_queue,&buf_msg,0);
#endif
            vTaskDelay(pdMS_TO_TICKS(5));
        }
 


#if USE_ORIGIN_PLAY_FLOW
        //memset(wav_buf[0],0,Decode_BufSize);
        //memset(wav_buf[1],0,Decode_BufSize);
   
        timeout =0;
        while(i2s_end!=0x1)
        {        
            vTaskDelay(pdMS_TO_TICKS(10));
            timeout++;
            if(timeout>50)
            {
                Scu_Setdevice_Reset(IIS1_BASE);
                Scu_Setdevice_ResetRelease(IIS1_BASE);
                play_iis =0;
                mprintf("0.5s timeout\r\n");
#if USE_ASR8388
                iis1_ES8388_play_cmd();
#endif
                break;
            }
        }
#else
        //send_4_zero_buffer();
#endif

        if(g_ESIC_Type == IC_TYPE_8388)
        {
            //MUTE_ON;
           	mute_on_es8388_output();
        }
        else if(g_ESIC_Type == IC_TYPE_8374)
        {
            ES8374_CloseSpeaker();
        }

end: 
        IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_TX_EN,DISABLE);
        IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_TX_EN,DISABLE);
#if USE_ASR8388
        if(!iis1_open)
        {
#if USE_S_MIC_DENOISE
          IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
#else 
          IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,DISABLE);
#endif
            IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,ENABLE);
            IIS_RXEN(IIS1,ENABLE); 
        }
#else
        IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,DISABLE);
        IISDMA_ChannelENConfig(IIS0DMA,IISxDMA_RX_EN,ENABLE);
        IIS_RXEN(IIS0,ENABLE);
#endif         
	  set_PlayVoice_time();
        //TIMx_us(TIMER1,SUSPEND_TIME);  
        extern void start_lowpower_cout(void);
        start_lowpower_cout();
        mprintf("[%d ]play done\n",asrtimer_counter);
        SetPlayFlagOFF();
#endif            
    }
}
#endif

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
