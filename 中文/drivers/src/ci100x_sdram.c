/**
  ******************************************************************************
  * @文件    ci100x_sdram.c
  * @作者    chipintelli 软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控CI100X芯片的SDRAM控制器.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#include "ci100x_sdram.h"
#include "ci100x_uart.h"
#include "includes.h"

extern volatile unsigned int g_dma_translate_ok2;


#define col_width (9UL)
#define row_width (12UL)
#define bank_width (2UL)
#define data_width (16UL)

#define tRC                  60          //ns  min
#define tXSR                 72          //ns  min
#define tRCAR                60          //ns  min
#define tWR                  2           //tCK min
#define tRP                  15          //ns  min
#define tRCD                 15          //ns  min
#define tRAS                 42          //ns  min
#define CAL                  3           //tCK 
#define tWTR                 1           //tCK min
#define IniRefNum            8           //Time
#define tINIT                200*1000    //ns  min
#define tREF                 15.6*1000    //ns  max
#define MemBaseAddr          0x70000000
#define NumOpenBanks         4           //Num
#define SetModeReg           1
#define ReadPipe             2
#define FullRefBeforeSelf    1
#define FullRefAfterSelf     1
#define ImmediatePre         1
#define Initialize           1

#define cycle ((double)1000 / (double)SDRAM_CLK)
extern volatile unsigned int g_dma_translate_ok2;
void sdramctrl_recfg(int flag);
/**
  * @功能:计算输入的对数值
  * @注意:无          
  * @参数:input 输入值 
  * @返回值:输入值的对数值
  */
static int my_log2(int input)
{
    int result;
    if(input == 0)
    {
      return 0;
    }
    for(result = 0; ;result++)
    {
      if((input >> result) & 0x1)
      {
        break;
      }
    }
    return result;
}
/**
  * @功能:浮点数取整
  * @注意:无          
  * @参数:input 输入值 
  * @返回值:运算值
  */
static int up_int(double input)
{
    int tmp= ((int)input == input) ? (int)input : ((int)input + 1);
    return tmp;
}
/**
  * @功能:浮点数取整
  * @注意:无          
  * @参数:input 输入值 
  * @返回值:运算值
  */
static int mintime2ck(double input)
{
    return up_int(input/cycle) - 1;
}
/**
  * @功能:浮点数取整
  * @注意:无          
  * @参数:input 输入值 
  * @返回值:运算值
  */
static int maxtime2ck(double input)
{
    return (int)(input/cycle) - 1;
}
/**
  * @功能:SDRAM初始化
  * @注意:无          
  * @参数:无
  * @返回值:无
  */
void sdram_init(void)
{	
    int reg_value;
    unsigned int reg0;
    unsigned int reg4;
    unsigned int regc;
    unsigned int reg8;
    unsigned int reg10;
    unsigned int reg14;
    unsigned int reg54;
    int i = 0;
    int k = 0;
    int n = 0;
    int q = 0,p = 0;
    volatile int w = 0;
    int length[4];
    int index[4];
    int curr_index = 0;
    int result[4][20];
    int end_result = 0;
    int max_length = 0;
       
    int max_index;
    int value54_set;
    int value54_set_other;

    unsigned int test_data[128] = 
    {
        0x55555555,/*flip*/
        0xaaaaaaaa,
        0x33333333,
        0xcccccccc,
        0x77777777,
        0x88888888,
        0xf0f0f0f0,
        0x0f0f0f0f,
        0xff00ff00,
        0x00ff00ff,
        0xffff0000,
        0x0000ffff,
        0xffffff00,
        0x000000ff,
        0xffffffff,
        0x00000000,
        0xaaaa5555,
        0x5555aaaa,
        0xaaaaffff,
        0xffff0000,
        0xff0000ff,
        0x00ffff00,
        0xfff00fff,
        0x000ff000,
        0x5a5a5a5a,
        0xa5a5a5a5,
        0x00000001,/*bit shift*/
        0x00000003,
        0x00000007,
        0x0000000f,
        0x00000011,
        0x00000033,
        0x00000077,
        0x000000ff,
        0x00010001,
        0x00030003,
        0x00070007,
        0x000f000f,
        0x00110011,
        0x00330033,
        0x00770077,
        0x00ff00ff,
        0x01110111,
        0x03330333,
        0x07770777,
        0x0fff0fff,
        0x11111111,
        0x33333333,
        0x77777777,
        0xffffffff,
        0x12345678,/*number*/
        0x87654321,
        0x00000000,
        0x00000001,
        0x7b4f5abd,/*nn data*/
        0xefdd943c,
        0x18c2322d,
        0xd58ead3d,
        0xea574cbc,
        0xd904323c,
        0xbdda5b3e,
        0x2c4589bc,
        0xd78d353d,
        0x2f3e2ebe,
        0x3940893d,
        0x5b434940,/*asr data*/
        0x43686970,
        0x496e7465,
        0x6c6c695d,
        0x0ab78e3f,
        0x34020000,
        0x33050000,
        0x25000000,
        0x156e1d3f,
        0x35020000,
        0xd1020000,
        0x00000000,
        0x00e80020,/*code*/
        0x0fd5ff1f,
        0x09caff1f,
        0x17caff1f,
        0x25caff1f,
        0x33caff1f,
        0x41caff1f,
        0x00000000,
        0xc16b41f4,
        0x0011c163,
        0x00994a1e,
        0x00920029,
        0x52494646,/*audio data*/
        0xa4330100,
        0x57415645,
        0x666d7420,
        0x8300b11c,
        0x18918110,
        0x32f91999,
        0x6000b59f,
        0x90401280,
        0xbc813915,
        0x08c8bb28,
        0x334292ac,
        0x666d7420,
        0x8300b11c,
        0x18918110,
        0x32f91999,
        0x6000b59f,
        0x90401280,
        0xbc813915,
        0x08c8bb28,
        0x334292ac,
        0x666d7420,
        0x8300b11c,
        0x18918110,
        0x32f91999,
        0x6000b59f,
        0x90401280,
        0xbc813915,
        0x08c8bb28,
        0x334292ac,
        0x666d7420,
        0x8300b11c,
        0x18918110,
        0x32f91999,
        0x6000b59f,
        0x90401280,
        0x32f91999,
        0x6000b59f,
        0x90401280,
};
    
    unsigned int temp_data[128];
	NVIC_InitTypeDef NVIC_InitStruct = {0};
    
    reg_value = 0;
    reg_value |= (my_log2(data_width / 16) << 13);
    reg_value |= ((col_width - 1) << 9);
    reg_value |= ((row_width - 1) << 5);
    reg_value |= ((bank_width - 1) << 3);
    reg0 = reg_value;
	
    reg_value = 0;
    reg_value |= (mintime2ck(tRC) << 22);
    reg_value |= (mintime2ck(tXSR) << 18);
    reg_value |= (mintime2ck(tRCAR) << 14);
    reg_value |= ((tWR-1) << 12);
    reg_value |= (mintime2ck(tRP) << 9);
    reg_value |= (mintime2ck(tRCD) << 6);
    reg_value |= (mintime2ck(tRAS) << 2);
    reg_value |= ((CAL-1) << 0);
    reg4 = reg_value;
	
    reg_value = 0;
    reg_value |= ((tWTR - 1) << 20);
    reg_value |= ((IniRefNum-1) << 16);
    reg_value |= (mintime2ck(tINIT+cycle) << 0);
    reg8 = reg_value;
	
    reg_value = 0;
    reg_value |= (maxtime2ck(tREF) << 0);
    reg10 = reg_value;
	
    reg_value = 0;
    reg_value |= (MemBaseAddr << 0);
    reg14 = reg_value;
	
    reg_value = 0;
    value54_set_other = data_width/8;
    value54_set = my_log2(value54_set_other);
    reg_value |= ((col_width + row_width+bank_width+value54_set - 15) << 0);
    reg54 = reg_value;
     
    reg_value = 0;
    reg_value |= ((NumOpenBanks-1) << 12);
    reg_value |= (SetModeReg << 9);
    reg_value |= (ReadPipe << 6);
    reg_value |= (FullRefAfterSelf << 5);
    reg_value |= (FullRefBeforeSelf << 4);
    reg_value |= (ImmediatePre << 3);
    reg_value |= (Initialize << 0);
    regc = reg_value;

    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x00) = reg0;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x04) = reg4;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x08) = reg8;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x10) = reg10;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x14) = reg14;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x54) = reg54;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x0c) = regc;
    while(((*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x0c)) & 0x1) != 0);
	           
    for(i = 0; i < 4; i++)
    {
        length[i] = 0;
        index[i] = 0;
    }

    for(q = 0; q < 4; q++)
    {
        for(p = 0; p < 20; p++)
        {
            result[q][p] = 0;
        } 
    }
    

    /*初始化temp_data*/
    for(n = 0; n < 128; n++)
    {
        temp_data[n] = 0x0;
    }
    /*初始化DMA，使能中断和时钟*/
    NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    Scu_SetDeviceGate(DMA_CTRL_BASE,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    //UARTPollingConfig(UART1);

    for(i = 0; i < 4; i++)
    {
      	w = 0;
      	for(k = 0; k < 20; k++)
      	{
			*(volatile unsigned int *)(0x40010000 + 0x98) = 0x0;
            
			*(volatile unsigned int *)(0x40010000 + 0x98) |= (i << 20);
			*(volatile unsigned int *)(0x40010000 + 0x98) |= (0x1 << k);

            w = 0;
			//for(n=0;n<128;n++)
			//{
            	w = 0;

                DMAC_ChannelDisable(DMACChannel2);
                DMAC_ChannelPowerDown(DMACChannel2,DISABLE);

                DMAC_IntTCClear(DMACChannel2);
                DMAC_IntErrorClear(DMACChannel2);
                DMAC_Config(DMAC_AHBMaster2,LittleENDIANMODE);
                DMAC_EN(ENABLE);

                DMAC_ChannelSoureAddr(DMACChannel2,(unsigned int)&test_data[0]);
                DMAC_ChannelDestAddr(DMACChannel2,0x70000000);

                DMAC_ChannelLLI(DMACChannel2,0,DMAC_AHBMaster2);

                DMAC_ChannelProtectionConfig(DMACChannel2,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
                DMAC_ChannelTCInt(DMACChannel2,ENABLE);
                DMAC_ChannelSourceConfig(DMACChannel2,INCREMENT,DMAC_AHBMaster2,TRANSFERWIDTH_32b,BURSTSIZE1);
                DMAC_ChannelDestConfig(DMACChannel2,INCREMENT,DMAC_AHBMaster2,TRANSFERWIDTH_32b,BURSTSIZE1);
                DMAC_ChannelTransferSize(DMACChannel2,32); /*一次bust4个字节,全部传输完成后产生中断*/

                DMAC_ChannelHalt(DMACChannel2,DISABLE);
                DMAC_ChannelInterruptMask(DMACChannel2,CHANNELINTMASK_ITC,DISABLE);
                DMAC_ChannelInterruptMask(DMACChannel2,CHANNELINTMASK_IE,ENABLE);

                DMAC_ChannelConfig(DMACChannel2,0,0,M2M_DMA);
                DMAC_ChannelLock(DMACChannel2,DISABLE);
                DMAC_ChannelEnable(DMACChannel2); 
                while(g_dma_translate_ok2 == 0);
                
                #if 1
                g_dma_translate_ok2 = 0;

                DMAC_ChannelDisable(DMACChannel2);
                DMAC_ChannelPowerDown(DMACChannel2,DISABLE);

                DMAC_IntTCClear(DMACChannel2);
                DMAC_IntErrorClear(DMACChannel2);
                DMAC_Config(DMAC_AHBMaster2,LittleENDIANMODE);
                DMAC_EN(ENABLE);

                DMAC_ChannelSoureAddr(DMACChannel2,0x70000000);
                DMAC_ChannelDestAddr(DMACChannel2,(unsigned int)&temp_data[0]);

                DMAC_ChannelLLI(DMACChannel2,0,DMAC_AHBMaster2);

                DMAC_ChannelProtectionConfig(DMACChannel2,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
                DMAC_ChannelTCInt(DMACChannel2,ENABLE);
                DMAC_ChannelSourceConfig(DMACChannel2,INCREMENT,DMAC_AHBMaster2,TRANSFERWIDTH_32b,BURSTSIZE1);
                DMAC_ChannelDestConfig(DMACChannel2,INCREMENT,DMAC_AHBMaster2,TRANSFERWIDTH_32b,BURSTSIZE1);
                DMAC_ChannelTransferSize(DMACChannel2,32); /*一次bust4个字节,全部传输完成后产生中断*/

                DMAC_ChannelHalt(DMACChannel2,DISABLE);
                DMAC_ChannelInterruptMask(DMACChannel2,CHANNELINTMASK_ITC,DISABLE);
                DMAC_ChannelInterruptMask(DMACChannel2,CHANNELINTMASK_IE,ENABLE);

                DMAC_ChannelConfig(DMACChannel2,0,0,M2M_DMA);
                DMAC_ChannelLock(DMACChannel2,DISABLE);
                DMAC_ChannelEnable(DMACChannel2); 
                while(g_dma_translate_ok2 == 0);
                g_dma_translate_ok2 = 0;
                DMAC_ChannelDisable(DMACChannel2);
                DMAC_ChannelPowerDown(DMACChannel2,DISABLE);
                #endif
                w = memcmp(test_data,temp_data,128);
                //mprintf("w = %d\n",w);
            	if(w != 0)
            	{
                
            	}
            	else
            	{

                    result[i][k] = 0x5a5a5a5a;
                    //mprintf("i= %d k= %d OK \n",i,k);
            	}
          	//}
      	}
    }
    for(q = 0; q < 4; q++)
    {
      	w = 0;
      	for(p = 0; p < 20; p++)
      	{
        	if(result[q][p] == 0x5a5a5a5a)
        	{
          		if(w == 0)
          		{
            		curr_index = p;
          		}
           		w++;
          		if(w > length[q])
           		{
             		length[q] = w;
             		index[q] = curr_index;
           		}
        	}
        	else
        	{
           		if(w > length[q])
           		{
             		length[q] = w;
             		index[q] = curr_index;
           		}
           		w = 0;
        	}
      	}
        //mprintf("l_f= %d \n",length[q]);
    }
    for(i = 0; i < 4; i++)
    {
        //mprintf("l= %d m= %d OK \n",length[i],max_length);
        if(length[i] > max_length)
        {
			max_length = length[i];
			max_index = index[i];
			end_result = i;
			 
        }
    }
    *(volatile unsigned int *)(0x40010000 + 0x98) &= ~(0x3 << 20);
    *(volatile unsigned int *)(0x40010000 + 0x98) |= (end_result << 20);
    *(volatile unsigned int *)(0x40010000 + 0x98) &= ~(0x1 << (max_index+max_length / 2));
    *(volatile unsigned int *)(0x40010000 + 0x98) &= ~(0XFFFFF);
	*(volatile unsigned int *)(0x40010000 + 0x98) |= (0x1 << (max_index+max_length / 2));
}

/**
  * @功能:SDRAM写，读数据验证
  * @注意:无          
  * @参数:无
  * @返回值:0,读写正确;非0,出错的地址
  */
void sdramctrl_recfg(int flag)
{
	if(flag)
	{
		return ;
	}
	/*1.reset sdram ctrl*/
	SCU->RSTCFG_LOCK = 0x51AC0FFE;
	SCU->SOFT_PRERST_CTRL1 &= ~(1<<3);
	for(volatile unsigned int delay=0;delay<0x80;delay++);
	SCU->SOFT_PRERST_CTRL1 |= (1<<3);
	
	/*160Mhz SDRAM clk*/
	*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x00) = 0x00001168;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x04) = 0x026E549A;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x08) = 0x00077D00;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x10) = 0x000009BF;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x14) = 0x70000000;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x54) = 0x00000009;
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x0c) = 0x000032B9;
    while(((*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0x0c)) & 0x1) != 0);
	*(volatile unsigned int *)(0x40010000 + 0x98) &= ~(0x3 << 20);
    *(volatile unsigned int *)(0x40010000 + 0x98) |= (3 << 20);
    *(volatile unsigned int *)(0x40010000 + 0x98) &= ~(0x1 << (1+0x0000000E / 2));
    *(volatile unsigned int *)(0x40010000 + 0x98) |= (0x1 << (1+0x0000000E / 2));
}

/**
  * @功能:SDRAM写，读数据验证
  * @注意:无          
  * @参数:无
  * @返回值:0,读写正确;非0,出错的地址
  */
unsigned int sdram_check(void)
{
  	unsigned int i=0,tmp=0x1234ed,addr=0x70000000;
	 
  	for(i=0;i<16*1024*1024;i+=4)
	{
  		*(volatile unsigned int*)(addr+i)=tmp+i;
  	}
	for(i=0;i<16*1024*1024;i+=4)
	{
	  	if(*(volatile unsigned int*)(addr+i)!=tmp+i){
	  		return addr;
	  	}
	}
	
	return 0;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/

