#include "smt_iis.h"
#include "ci100x_iisdma.h"
#include "ci100x_uart.h"
#include "includes.h"

/**
  * @功能:IIS0 slaver 模式接收数据存到PCM RAM中
  * @注意:IIS_DMA 接收16Kbytes I2S数据 地址绕回去PCM RAM起始地址
  *       I2S FS:16k 16位 SCK/LRCK =64,左对齐格式。 
  * @参数:无
  * @返回值:无
  */
void IIS0_SlaveRxpcm(void)
{
	IIS_DMA_RXInit_Typedef IISDMARX_Init_Struct = {0};
	Scu_Setdevice_Reset((unsigned int)IIS0);
    Scu_Setdevice_ResetRelease((unsigned int)IIS0);
	Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
	Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	IISDMARX_Init_Struct.rxaddr = 0x20020000;
    IISDMARX_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IISDMARX_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IISDMARX_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IISDMARX_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IISDMARX_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
#if IIS0_RX___IIS_STD    
    IISDMARX_Init_Struct.rxdatafmt = IIS_RXDATAFMT_IIS;
#else
    IISDMARX_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
#endif
    IISDMARX_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
	IISx_RXInit(IIS0,&IISDMARX_Init_Struct);	
	IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,DISABLE);
}

/**
  * @功能:IIS1 slaver 模式接收数据存到PCM RAM中
  * @注意:IIS_DMA 接收16Kbytes I2S数据 地址绕回去PCM RAM起始地址
  *       I2S FS:16k 16位 SCK/LRCK =64,左对齐格式。 
  * @参数:无
  * @返回值:无
  */
void IIS1_SlaveRxpcm(void)
{
	IIS_DMA_RXInit_Typedef IISDMARX_Init_Struct = {0};
	Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
	Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
	Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
	IISDMARX_Init_Struct.rxaddr = 0x20020000;
    IISDMARX_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IISDMARX_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IISDMARX_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IISDMARX_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IISDMARX_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IISDMARX_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    IISDMARX_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
	IISx_RXInit(IIS1,&IISDMARX_Init_Struct);	
	IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,DISABLE);
}

/**
  * @功能:IIS0 slaver 模式接收数据存到PCM RAM中
  * @注意:IIS_DMA 接收1Kbytes I2S数据 来一次中断       
  * @参数:无
  * @返回值:无
  */
void i2s0_pcmtopcmram(void)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    while(IISDMA_DATABUSBUSY(IIS0DMA));
    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_Reset((unsigned int)IIS0);
    Scu_Setdevice_ResetRelease((unsigned int)IIS0);
	
	Scu_Setdevice_Reset((unsigned int)IIS1);
    Scu_Setdevice_ResetRelease((unsigned int)IIS1);
	
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
	
    Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	
	Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
	Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
	
    NVIC_Init(&NVIC_InitStruct);
    IIS_DMA_Init_Struct.rxaddr = 0x20020000;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IIS_RXMODEConfig(IIS1,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
	IISx_RXInit(IIS0,&IIS_DMA_Init_Struct);	
	//IIS_DMA_Init_Struct.rxaddr = 0x70500000;
	//IISx_RXInit(IIS1,&IIS_DMA_Init_Struct);	
    IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,DISABLE);
}

/**
  * @功能:IIS0接收数据存到SRAM中
  * @注意:       
  * @参数:1.sramaddr SRAM中存储数据地址
  *       2.bytesize   数据字节数
  * @返回值:无
  */
void i2s0_record_tosram(unsigned int sramaddr,unsigned int bytesize)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    while(IISDMA_DATABUSBUSY(IIS0DMA));
	
    Scu_Setdevice_Reset((unsigned int)IIS_DMA_BASE);
    Scu_Setdevice_ResetRelease((unsigned int)IIS_DMA_BASE);
		
    Scu_Setdevice_Reset((unsigned int)IIS0);
    Scu_Setdevice_ResetRelease((unsigned int)IIS0);
	
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
	
    Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	
    NVIC_Init(&NVIC_InitStruct);
    IIS_DMA_Init_Struct.rxaddr = sramaddr;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize = (IISDMA_RXTXxRollbackADDR)(bytesize/( 16) - 1);//IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS0,&IIS_DMA_Init_Struct);	
    //IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,DISABLE);
}

/**
  * @功能:IIS0接收数据存到SRAM中
  * @注意:       
  * @参数:1.sramaddr SRAM中存储数据地址
  *       2.bytesize   数据字节数
  * @返回值:无
  */
void i2s0rx_recordJlink(void)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	
    Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	
    NVIC_Init(&NVIC_InitStruct);
    IIS_DMA_Init_Struct.rxaddr = 0x70500000;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.rxdatafmt =  IIS_RXDATAFMT_LEFT_JUSTIFIED;//IIS_RXDATAFMT_IIS;//IIS_RXDATAFMT_LEFT_JUSTIFIED;//IIS_RXDATAFMT_IIS;//;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS0,&IIS_DMA_Init_Struct);	
}
void i2s0rx_recordJlink_handler(void)
{
    static unsigned int addr = 0;
    addr +=16*1024;
    if(addr >=32*1024)
    {
        addr = 0;
        *(volatile unsigned int*)0x70000000 =0x1;
    }else
    {
        *(volatile unsigned int*)0x70000000 =0x2;
     }
    IISxDMA_RADDR(IIS1DMA,0x70500000 + addr);
    
}
/**
  * @功能:IIS0接收数据存到SRAM中
  * @注意:       
  * @参数:1.sramaddr SRAM中存储数据地址
  *       2.bytesize   数据字节数
  * @返回值:无
  */
void i2s0rx_recordvoice(void)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	
    Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	
    NVIC_Init(&NVIC_InitStruct);
    IIS_DMA_Init_Struct.rxaddr = 0x70000000;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    //IIS_RXMODEConfig(IIS0,IIS_MERGE_16BITTO32BIT,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_STEREO);
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS0,&IIS_DMA_Init_Struct);	
}

void iis0_rxrecord_handler(void)
{
    static unsigned int addr = 0;
    addr += 16*1024;
    IISxDMA_RADDR(IIS0DMA,0x70000000 + addr);
    if(addr >= 16 * 1024 * 1024)
    {
      IISDMA_ChannelENConfig(IIS0DMA,IISxDMA_RX_EN,DISABLE);
      mprintf("%s done\n",__func__);
    }
}

void iis1_rxrecord_handler(void)
{
    static unsigned int addr = 0;
    addr += 0x7d20;
    IISxDMA_RADDR(IIS1DMA,0x70000000 + addr);
    if(addr > 4 * 1024 * 1024)
    {
      IISDMA_ChannelENConfig(IIS1DMA,IISxDMA_RX_EN,DISABLE);
      mprintf("%s done\n",__func__);
    }	
}

/**
  * @功能:IIS1接收数据存到SRAM中
  * @注意:       
  * @参数:1.sramaddr SRAM中存储数据地址
  *       2.bytesize   数据字节数
  * @返回值:无
  */
void i2stosram(unsigned int sramaddr,unsigned int bytesize)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct={0};
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel	=IIS_DMA_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	
	Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.rxaddr          = sramaddr;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize= (IISDMA_RXTXxRollbackADDR)(bytesize/(8*4*32) -1);//IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize    = IISDMA_TXRXSINGALESIZE128bytes;//IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.rxdatabitwide   = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck        = IIS_BUSSCK_LRCK32;
    IIS_DMA_Init_Struct.rxdatafmt       = IIS_RXDATAFMT_IIS;
    IIS_DMA_Init_Struct.oversample      = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS1,IIS_MERGE_16BITTO32BIT,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&IIS_DMA_Init_Struct);
    IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
}

/**
  * @功能:IIS1接收数据存到SRAM中
  * @注意:       
  * @参数:1.sramaddr SRAM中存储数据地址
  *       2.bytesize   数据字节数
  * @返回值:无
  */
void i2s1_record_tosram(void)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct={0};
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel	=IIS_DMA_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
    Scu_SetDeviceGate((unsigned int)IIS1,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	
	Scu_SetIOReuse(I2S1_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S1_SDI_PAD,FIRST_FUNCTION);
    IIS_DMA_Init_Struct.rxaddr          = 0x70000000;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize= IISDMA_RXTX1001RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize    = IISDMA_TXRXSINGALESIZE32bytes;
    IIS_DMA_Init_Struct.rxdatabitwide   = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck        = IIS_BUSSCK_LRCK32;
    IIS_DMA_Init_Struct.rxdatafmt       = IIS_RXDATAFMT_IIS;
    IIS_DMA_Init_Struct.oversample      = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS1,IIS_MERGE_16BITTO32BIT,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS1,&IIS_DMA_Init_Struct);
    IISDMA_ADDRRollBackINT(IIS1DMA,IISxDMA_RX_EN,ENABLE);
}

void i2s0rx_recordSD(void)
{
    IIS_DMA_RXInit_Typedef IIS_DMA_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
    Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	
    Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	
    NVIC_Init(&NVIC_InitStruct);
    IIS_DMA_Init_Struct.rxaddr = 0x70500000;
    IIS_DMA_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IIS_DMA_Init_Struct.rollbackaddrsize = IISDMA_RXTX1024RollbackADDR;
    IIS_DMA_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IIS_DMA_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IIS_DMA_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IIS_DMA_Init_Struct.rxdatafmt =  IIS_RXDATAFMT_LEFT_JUSTIFIED;//IIS_RXDATAFMT_LEFT_JUSTIFIED;//IIS_RXDATAFMT_IIS;//;
    IIS_DMA_Init_Struct.oversample = IIS_OverSample256Fs;
    IIS_RXMODEConfig(IIS0,IIS_MERGE_16BITTO32BIT,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_STEREO);
    //IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
    IISx_RXInit(IIS0,&IIS_DMA_Init_Struct);	
}
//////////////////////
void IIS0_SlaveRxSDRAMpcm(void)
{
	IIS_DMA_RXInit_Typedef IISDMARX_Init_Struct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IIS_DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	Scu_Setdevice_Reset((unsigned int)IIS0);
    Scu_Setdevice_ResetRelease((unsigned int)IIS0);
	Scu_SetDeviceGate((unsigned int)IIS_DMA_BASE,ENABLE);
	Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);
	Scu_SetIOReuse(I2S0_SCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_LRCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S_MCLK_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,FIRST_FUNCTION);
	IISDMARX_Init_Struct.rxaddr = SDRAM_REC_BUFFER_ADDR;
    IISDMARX_Init_Struct.rxinterruptsize = IISDMA_RX32Interrupt;
    IISDMARX_Init_Struct.rollbackaddrsize = IISDMA_RXTX64RollbackADDR;
    IISDMARX_Init_Struct.rxsinglesize = IISDMA_TXRXSINGALESIZE16bytes;
    IISDMARX_Init_Struct.rxdatabitwide = IIS_TXRXDATAWIDE16BIT;
    IISDMARX_Init_Struct.sck_lrck = IIS_BUSSCK_LRCK64;
    IISDMARX_Init_Struct.rxdatafmt = IIS_RXDATAFMT_LEFT_JUSTIFIED;
    IISDMARX_Init_Struct.oversample = IIS_OverSample256Fs;
if(2==run_mode)
{
     IIS_RXMODEConfig(IIS0,IIS_MERGE_16BITTO32BIT,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_STEREO);
}else
{
    IIS_RXMODEConfig(IIS0,IIS_MERGE_NONE,IIS_LR_LEFT_HIGH_RIGHT_LOW,IIS_RXMODE_MONO);
}    
	IISx_RXInit(IIS0,&IISDMARX_Init_Struct);	
	IISDMA_ADDRRollBackINT(IIS0DMA,IISxDMA_RX_EN,ENABLE);
}

