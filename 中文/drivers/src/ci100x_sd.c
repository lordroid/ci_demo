/**
  ******************************************************************************
  * @文件    ci100x_sd.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-8-31
  * @概要    这个文件是chipintelli公司的CI100X芯片程序的SD卡驱动函数.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 

#include "ci100x_sd.h"
#include "ci100x_uart.h"

#include "includes.h"

/**
  * @功能:配置SPI1 SD对应的 DMA控制器
  * @注意:无       
  * @参数:1.dmachannel DMA的通道
  *       2.sddma_config 配置参数指针
  * @返回值:0成功;-1,传输超时
  */
int SD_DMAC_ENABLE(DMACChannelx dmachannel)
{
	 DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 0);
     return 0;
}
int SD_DMAC_M2P_P2MConfig(DMACChannelx dmachannel,\
  SDDMAInit_Struct* sddma_config)
{
	DMAC_AHBMasterx srcmaster,destmaster,tmpmaster;
    INCREMENTx srcinc,destinc;
  	DMAC_Peripherals Peripherals;
	unsigned int memaddr,master_flag=0;
  	volatile int timeout = 0x7FFFFFFF;
	if(M2P_DMA == sddma_config->flowctrl)
	{
		srcinc = INCREMENT;
    	destinc = NOINCREMENT;
		memaddr = sddma_config->srcaddr;
		master_flag =0;
		Peripherals = DMAC_Peripherals_SPI1_TX;
	}
	else if(P2M_DMA == sddma_config->flowctrl)
	{
		srcinc = NOINCREMENT;
      	destinc = INCREMENT;
		memaddr = sddma_config->destaddr;
		master_flag =1;
		Peripherals = DMAC_Peripherals_SPI1_RX;
	}
	else
	{
		return -1;
	}
  	if((GDMA_SDRAM_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_SDRAM_ADDR+GDMA_SDRAM_SIZE))
	{
		tmpmaster = DMAC_AHBMaster2;
	}
	else if((GDMA_PCMRAM_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_PCMRAM_ADDR+GDMA_PCMRAM_SIZE))
	{
		tmpmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_FFTRAM_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_FFTRAM_ADDR+GDMA_FFTRAM_SIZE))
	{
		tmpmaster = DMAC_AHBMaster1;
	}
	else if((GDMA_SRAM0_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_SRAM0_ADDR+GDMA_SRAM0_SIZE))
	{
		tmpmaster = DMAC_AHBMaster1;
	}  
	else if((GDMA_SRAM1_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_SRAM1_ADDR+GDMA_SRAM1_SIZE))
	{
		tmpmaster = DMAC_AHBMaster1;
	}
    else if ((GDMA_CSRAM_ADDR <= memaddr) && (memaddr\
	  +sddma_config->bytesize <= GDMA_CSRAM_ADDR+GDMA_CSRAM_SIZE))
	{
		tmpmaster = DMAC_AHBMaster1;
	}
	else
	{
		return -1;
	}
	if(!master_flag)
	{
		srcmaster = tmpmaster;
		destmaster = \
		  (DMAC_AHBMaster1 == srcmaster)?DMAC_AHBMaster2:DMAC_AHBMaster1;
	}
	else 
	{
		destmaster = tmpmaster;
	  	srcmaster = \
		  (DMAC_AHBMaster1 == destmaster)?DMAC_AHBMaster2:DMAC_AHBMaster1;
	}
	
	DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 18);
    while((DMAC->DMACChannel[dmachannel].DMACCxConfiguration &\
	  (1 << 17)) && timeout--);
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 0);
	
	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(0x1FFF << 19);
	
    DMAC->DMACIntTCClear = 0xff;
	
    DMAC->DMACIntErrClr = 0xff;
	
	DMAC->DMACConfiguration &= ~(3 << 1);
	DMAC->DMACConfiguration |=(1 << 0);
	
    DMAC->DMACChannel[dmachannel].DMACCxSrcAddr = sddma_config->srcaddr;
	DMAC->DMACChannel[dmachannel].DMACCxDestAddr = sddma_config->destaddr;
	
	DMAC->DMACChannel[dmachannel].DMACCxLLI = 0;
	
    DMAC_ChannelProtectionConfig(dmachannel,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
    DMAC_ChannelTCInt(dmachannel,ENABLE);
    DMAC_ChannelSourceConfig(dmachannel,srcinc,srcmaster,\
        sddma_config->transferwidth,sddma_config->busrtsize);
    DMAC_ChannelDestConfig(dmachannel,destinc,destmaster,\
        sddma_config->transferwidth,sddma_config->busrtsize);
    DMAC_ChannelTransferSize(dmachannel,sddma_config->bytesize);

	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 18);
    
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration |=(1<<CHANNELINTMASK_ITC);

	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &=~ (1<<CHANNELINTMASK_IE);
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~((0x7 << 11) | (0xF << 6) \
	  | (0xF << 1));
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= ((sddma_config->flowctrl << 11)\
	  | (Peripherals << 6) | (Peripherals << 1));

	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 16);

	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 18);
   // DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 0);
	return 0;
}

//divpara 02=40M 3=26M 4=20M 8=10M 0x10=5M 0x20=2.5M
#define SD_CLK_3P17MHZ 0x3F
#define SD_CLK_4P16MHZ 0x30
#define SD_CLK_6P25MHZ 0x20
#define SD_CLK_10MHZ 0x14
#define SD_CLK_12P5MHZ 0x10
#define SD_CLK_20MHZ 0x0A
#define SD_CLK_25MHZ 0x08
#define SD_CLK_50MHZ 0x04
#define SD_CLK_100MHZ 0x02 //default 

void Set_SD_Trans_Clk(char divpara)
{
    if(divpara<2)  divpara = 2;
    if(divpara>0x3f)  divpara = 0x3f;
      
    Scu_SetDeviceGate((unsigned int)SD_BASE,DISABLE);
    Scu_Setdiv_Parameter((unsigned int)SD_DIV_BASE,divpara);//6bit
    Scu_SetClkDiv_Enable((unsigned int)SPI2);
}


/**
  * @功能:SD SPI接口初始化
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SD_SPIInit(void)
{
	 volatile unsigned int i = 1000;
  	 NVIC_InitTypeDef NVIC_InitStruct = {0};
	 SCU->CKCFG_LOCK = 0x51AC0FFE;
     while(!(SCU->CKCFG_LOCK));
	 SCU->SYS_CTRL |= (0x3UL << 13);
     Set_SD_Trans_Clk(SD_CLK_20MHZ);//SD_CLK_2P5MHZ);
	 Scu_SetDeviceGate((unsigned int)SPI1,ENABLE);
	 Scu_SetDeviceGate((unsigned int)SPI2,ENABLE);
	 Scu_SetDeviceGate(DMA_CTRL_BASE,ENABLE);
	 //SCU->CKCFG_LOCK = 0;

	 Scu_Setdevice_Reset(SPI2_BASE);
	 Scu_Setdevice_Reset(SPI1_BASE);
	 Scu_Setdevice_Reset(DMA_CTRL_BASE);
	 Scu_Setdevice_ResetRelease(SPI2_BASE);
	 Scu_Setdevice_ResetRelease(SPI1_BASE);
	 Scu_Setdevice_ResetRelease(DMA_CTRL_BASE);
	 SD->SDCREG = 0x3;
	 i = 1000;
     Scu_SetIOReuse(SPI1_DOUT_PAD,FIRST_FUNCTION); 
	 while(i--);
	 NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
	 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	 NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStruct);
    
     NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);    
}

/**
  * @功能:SD 发送命令0
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
int SD_SendCmd0(void)
{
    int timeout;
	volatile unsigned int tmp = 0;
	int retry_times;
    
retry:  
	retry_times = 0;
    timeout = TIMEOUT_CMD;
	do{    
        if(timeout==0)
        {
            mprintf("cmd0 A retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));

	SD->SDCMDREG = ((0x95 >> 1) << 6);
	SD->SDCMDREG1 = 0;
	SD->SDCMDREG |= (1 << 27);
	tmp = 30;
	while(tmp--);
	tmp = SD->SDCMDREG;
    
    timeout = TIMEOUT_CMD;
  	do{    
        if(timeout==0)
        {
            mprintf("cmd0 B retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
		retry_times++;
		if(retry_times>20)
			return -1;
	}while(tmp & (1 << 27));
    return 0;
}

/**
  * @功能:SD 发送命令55
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SD_SendCmd55(void)
{
	volatile unsigned int tmp = 0;
    int timeout;
retry:  
    timeout = TIMEOUT_CMD;
	do{    
        if(timeout==0)
        {
            mprintf("cmd55 A retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
	SD->SDCMDREG = ((0x65 >> 1) << 6)|(55 << 0);
	SD->SDCMDREG |= (1 << 27);
    
    timeout = TIMEOUT_CMD;
    do{    
        if(timeout==0)
        {
            mprintf("cmd55 B retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
}

/**
  * @功能:SD 发送命令8
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SD_SendCmd8(void)
{
	volatile unsigned int tmp = 0;
    int timeout;
    
retry:      
    timeout = TIMEOUT_CMD;
    
	do{    
        if(timeout==0)
        {
            mprintf("cmd8 A retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
	SD->SDCMDREG = (0x3 << 22)|((0x87 >> 1) << 6)|(8 << 0);
	SD->SDCMDREG1 = 0x1AA;
	SD->SDCMDREG |= (1 << 27);
    
    timeout = TIMEOUT_CMD;
    do{    
        if(timeout==0)
        {
            mprintf("cmd8 B retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
}

/**
  * @功能:SD 发送命令17
  * @注意:无       
  * @参数:无
  * @返回值:无
  */

SD_Error SD_SendCmd17(unsigned int addr)
{
	volatile unsigned int tmp = 0;
    int timeout;
retry:      
    timeout = TIMEOUT_CMD;
    
	do{    
        if(timeout==0)
        {
            mprintf("cmd17 retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
	SD->SDCMDREG = 0x003Fc011;
	SD->SDCMDREG1 = addr;
	SD->SDCMDREG |= (1<< 27);
    return SD_OK;
}

/**
  * @功能:SD 发送命令41
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SD_SendCmd41(void)
{
	volatile unsigned int tmp = 0;
    int timeout;
retry:      
    timeout = TIMEOUT_CMD;
    
	do{    
        if(timeout==0)
        {
            mprintf("cmd41 A retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));

	SD->SDCMDREG = (0x4 << 22)|((0x77 >> 1) << 6)|(41 << 0);
	SD->SDCMDREG1 = 0x40000000;
	SD->SDCMDREG |= (1 << 27);
    
    timeout = TIMEOUT_CMD;
    do{    
        if(timeout==0)
        {
            mprintf("cmd41 B retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
}

/**
  * @功能:SD 发送命令24
  * @注意:无       
  * @参数:addr:地址
  * @返回值:无
  */
void SD_SendCmd24(unsigned int addr)
{
	volatile unsigned int tmp = 0;
	int timeout;

retry:      
    timeout = TIMEOUT_CMD;
    
	do{    
        if(timeout==0)
        {
            mprintf("cmd41 A retry");
            SD_SPIInit();//reinit	
            goto retry;
        }
        timeout --;
		tmp = SD->SDCMDREG;
	}while(tmp & (1 << 27));
    
	SD->SDCMDREG = 0x003FE018;
	SD->SDCMDREG1 = addr;
	SD->SDCMDREG |= (1 << 27);
}

/**
  * @功能:SD 软件延时
  * @注意:无       
  * @参数:dly
  * @返回值:无
  */
static void sdio_delay(int dly)
{
    volatile int n;
    while(dly--)
    {
        for(n=0;n<1000;n++);
    }
}

/**
  * @功能:SD 软件延时
  * @注意:无       
  * @参数:dly
  * @返回值:无
  */
static SD_Error SD_PowerOn(void)
{
	unsigned int receive1 = 0x0;
  	volatile int timeout =0x7FFFFFFF;
	if(SD_SendCmd0()<0)
		return SD_ERROR;
	while((!sd_transend) && timeout--);
    sd_transend = 0;
	receive1 = SD->SDCRREG1;
	if(receive1 & (1 << 0))
	{
		SD_SendCmd8();
		timeout =0x7FFFFFFF;
		while((!sd_transend) && timeout-- );
		sd_transend = 0;
		receive1 = SD->SDCMDREG1;
		if(receive1 & (1 << 16))
		{
			sdio_delay(900);
			/* delay 1s*/
		}
	}
	else
	{
		//return SD_ERROR;
	}
	do{
		SD_SendCmd55();
		timeout =0x7FFFFFFF;
		while((!sd_transend) && timeout--);
		sd_transend = 0;
		sdio_delay(100);
		SD_SendCmd41();
		timeout =0x7FFFFFFF;
		while((!sd_transend) && timeout--);
		sd_transend = 0;
		receive1 = SD->SDCRREG1;
		sdio_delay(1000);
	}while((receive1 & (1 << 24))!=0);
	return SD_OK;
}

/**
  * @功能:SD 软件延时
  * @注意:无       
  * @参数:dly
  * @返回值:无
  */
extern void DelayMs(int ms);
SD_Error SD_Init(void)
{
	static int sd_init = 0;
	if(!sd_init)
	{
  		SD_SPIInit();
		DelayMs(2);
		if( SD_PowerOn() != SD_OK)
		{
			return SD_ERROR;
		}
		sd_init = 1;
	}
	/*DMA 初始化*/
	return SD_OK;
}

/**
  * @功能:SD 块读
  * @注意:无 ReadBlockAddr 必须是512的倍数      
  * @参数:dly
  * @返回值:无
  */
SD_Error SD_ReadBlock(unsigned char *readbuff, 
                      unsigned int ReadBlockAddr)
{
  	SDDMAInit_Struct sddma_config;
  
	volatile unsigned int timeout = 100000;//0x7FFFFFFF;
 	static int retryR=0;
 
 retry:

	
	/* DMA CFG*/
	sddma_config.flowctrl = P2M_DMA;
	sddma_config.busrtsize = BURSTSIZE128;
	sddma_config.transferwidth = TRANSFERWIDTH_32b;
	sddma_config.srcaddr = 0x61000000;
	sddma_config.destaddr = (unsigned int)readbuff;
	sddma_config.bytesize = 128;
	g_dma_translate_ok1 = 0;
	 
	SD_DMAC_M2P_P2MConfig(DMACChannel1,&sddma_config);
	SD_SendCmd17(ReadBlockAddr);

	SD_DMAC_ENABLE(DMACChannel1);

	timeout = TIMEOUT_R;
	while((!sd_transend))
	{
		if(timeout == 0)
		{		
			mprintf("_R_%d @%x",retryR++,ReadBlockAddr);					
			SD_SPIInit();//reinit			
			goto retry;
		}
		timeout --;
	}
    sd_transend = 0; 
	//timeout = 0x7FFFFFFF;
	//while((!g_dma_translate_ok1) && timeout--);
	timeout = TIMEOUT_R;
	while((!g_dma_translate_ok1))
	{
		if(timeout == 0)
		{		
			mprintf("_r_%d @%x",retryR,ReadBlockAddr);					
			SD_SPIInit();//reinit			
			goto retry;
		}
		timeout --;
	}
	g_dma_translate_ok1 = 0;
	return SD_OK;
}

/**
  * @功能:SD 多块读
  * @注意:无       
  * @参数:dly
  * @返回值:无
  */
SD_Error SD_ReadMultiBlocks(unsigned char *readbuff, 
                            unsigned int ReadBlockAddr, 
                            unsigned int NumberOfBlocks)
{
	return SD_OK;
}


/**
  * @功能:SD 块写
  * @注意:无       
  * @参数:
  * @返回值:无
  */

SD_Error SD_WriteBlock(unsigned char *writebuff, 
                       unsigned int WriteBlockAddr)
{
	SDDMAInit_Struct sddma_config;
	volatile unsigned int timeout = 100000;//0x7FFFFFFF;
	static int retryW=0;	
//	vTaskSuspendAll();
	
retry:

	/*DMA 配置 */ 
	sddma_config.flowctrl = M2P_DMA;
	sddma_config.busrtsize = BURSTSIZE128;
	sddma_config.transferwidth = TRANSFERWIDTH_32b;
	sddma_config.srcaddr = (unsigned int)writebuff;
	sddma_config.destaddr = 0x61000000;
	sddma_config.bytesize = 128;
	g_dma_translate_ok1 = 0;


	SD_DMAC_M2P_P2MConfig(DMACChannel1,&sddma_config);
	SD_SendCmd24(WriteBlockAddr);	
	SD_DMAC_ENABLE(DMACChannel1);

	timeout = TIMEOUT_W;

	while((!sd_transend))
	{		
		if( timeout == 0)
		{		
			mprintf("_W_%d @%x",retryW++,WriteBlockAddr);			
			SD_SPIInit();//reinit			
			goto retry;
		}
		timeout--;
	}
    sd_transend = 0; 
	timeout = 0x7FFFFFFF;
	//while((!g_dma_translate_ok1) && timeout--);
	timeout = TIMEOUT_R;
	while((!g_dma_translate_ok1))
	{
		if(timeout == 0)
		{		
			mprintf("_r_%d @%x",retryW,WriteBlockAddr);					
			SD_SPIInit();//reinit			
			goto retry;
		}
		timeout --;
	}
	g_dma_translate_ok1 = 0;
	return SD_OK;
}

/**
  * @功能:SD 多块写
  * @注意:无       
  * @参数:
  * @返回值:无
  */
SD_Error SD_WriteMultiBlocks(unsigned char *writebuff, 
                             unsigned int WriteBlockAddr,  
                             unsigned int NumberOfBlocks)
{
	return SD_OK;
}

/**
  * @功能:带互斥信号量保护的 读取SD卡文件函数 
  * @注意:无       
  * @参数:
  * @返回值: 
  */
FRESULT m_fread (
	FIL *fp, 		/* Pointer to the file object */
	void *buff,		/* Pointer to data buffer */
	UINT btr,		/* Number of bytes to read */
	UINT *br		/* Pointer to number of bytes read */
	  )
{
	int resize=btr;
	int size=0;
  	xSemaphoreTake(xFileMutexlockSemaphore,portMAX_DELAY);
	while(resize)
	{
	  	btr = (resize >= 512)?512:resize;
		f_read(fp,(void*)((char*)buff+size),btr,br);
		if(!*br)
		{
			break;
		}
		size	+= *br;
		resize -= *br;
	}
	xSemaphoreGive(xFileMutexlockSemaphore);
	return FR_OK;
}

/**
  * @功能:带互斥信号量保护的 写SD卡文件函数 
  * @注意:无       
  * @参数:
  * @返回值: 
  */
FRESULT m_fwrite (
	FIL *fp,			/* Pointer to the file object */
	const void *buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT *bw			/* Pointer to number of bytes written */
	  )
{
  	int resize=btw;
	int size=0;
	unsigned char buff1[512];
	//
	xSemaphoreTake(xFileMutexlockSemaphore,portMAX_DELAY);
	while(resize)
	{
	  	btw = (resize >= 512)?512:resize;
		memcpy(buff1,(void*)((char*)buff+size),btw);
	//	vTaskDelay(100);
	//memset(buff1,0xE1,512);

		f_write(fp,(void*)buff1,btw,bw);

		size	+= *bw;
		resize -= *bw;
	}
	xSemaphoreGive(xFileMutexlockSemaphore);
	return FR_OK;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
