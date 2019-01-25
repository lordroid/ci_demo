/**
  ******************************************************************************
  * @文件    ci100x_dma.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件用于chipintelli公司的CI100X芯片的DMA控制器.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_dma.h"

/**
  * @功能:DMA通道屏蔽后的中断状态(只要有通道传输完成中断或者通道传输错误中断的一个)
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道无中断；非0，该通道有中断
  */
int DMAC_IntStatus(DMACChannelx dmachannel)
{
    return DMAC->DMACIntStatus & (1 << dmachannel);
}
/**
  * @功能:DMA通道传输完成中断状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道无传输完成中断；非0，该通道有传输完成中断
  */
int DMAC_IntTCStatus(DMACChannelx dmachannel )
{
    return DMAC->DMACIntTCStatus & (1 << dmachannel);
} 

/**
  * @功能:清除DMA通道传输完成中断状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_IntTCClear(DMACChannelx dmachannel)
{
    if(DMACChannelALL == dmachannel)
    {
      DMAC->DMACIntTCClear = 0xff;
    }
    else
    {
      DMAC->DMACIntTCClear |= (1 << dmachannel);
    }
}
/**
  * @功能:DMA通道传输错误中断状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道无传输错误中断；非0，该通道有传输错误中断
  */
int DMAC_IntErrorStatus(DMACChannelx dmachannel)
{
    return DMAC->DMACIntErrorStatus & (1 << dmachannel);
}
/**
  * @功能:清除DMA通道传输错误中断状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_IntErrorClear(DMACChannelx dmachannel)
{
    if(DMACChannelALL == dmachannel)
    {
      	DMAC->DMACIntErrClr = 0xff;
    }
    else
    {
      	DMAC->DMACIntErrClr |= (1 << dmachannel);
    }
}
/**
  * @功能:DMA通道原始的(屏蔽前)中断状态(只要有通道传输完成中断或者通道传输错误中断的一个)
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道无中断；非0，该通道有中断
  */
int DMAC_RawIntTCStatus(DMACChannelx dmachannel)
{
    return DMAC->DMACRawIntTCStatus & (1 << dmachannel);	
}
/**
  * @功能:DMA通道传输错误原始(屏蔽前)中断状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道无传输错误中断；非0，该通道有传输错误中断
  */
int DMAC_RawErrorIntStatus(DMACChannelx dmachannel)
{
    return DMAC->DMACRawIntErrorStatus & (1 << dmachannel);
}
/**
  * @功能:DMA通道使能状态
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:0，该通道没有被使能；非0，该通道已使能
  */
int DMAC_ChannelEnableStatus(DMACChannelx dmachannel)
{
    return DMAC->DMACEnbldChns & (1 << dmachannel);
}
/**
  * @功能:DMA通道软件突发传输请求
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_SoftwareBurstReq(DMACChannelx dmachannel)
{
    DMAC->DMACSoftBReq |= (1 << dmachannel); 
}
/**
  * @功能:DMA通道软件单次传输请求
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_SoftwareSingleReq(DMACChannelx dmachannel)
{
    DMAC->DMACSoftSReq |= (1 << dmachannel);
}
/**
  * @功能:DMA通道软件最后一次突发传输请求
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_SoftwareLastBurstReq(DMACChannelx dmachannel)
{
    DMAC->DMACSoftLBReq |= (1 << dmachannel);
}
/**
  * @功能:DMA通道软件最后一次单次传输请求
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  * @返回值:无
  */
void DMAC_SoftwareLastSingleReq(DMACChannelx dmachannel)
{
    DMAC->DMACSoftLSReq |= (1 << dmachannel);
}
/**
  * @功能:DMA控制器AHB MASTER 字节端序的配置
  * @注意: 无      
  * @参数:1.dmamaster  DMAC_AHBMaster1，DMAC_AHBMaster2
  *		  2.endianmode 	LittleENDIANMODE 小端，低字节存放低地址；BigENDIANMODE，大端，低字节存放高地址
  * @返回值:无
  */
void DMAC_Config(DMAC_AHBMasterx dmamaster,ENDIANMODE endianmode)
{
	DMAC->DMACConfiguration  &= ~(1 << (1 + dmamaster));
	DMAC->DMACConfiguration  |= (endianmode << (1 + dmamaster));
}
/**
  * @功能:DMA控制器使能
  * @注意: 无      
  * @参数:1.cmd  ENABLE,使能；DISABLE,禁止
  * @返回值:无
  */
void DMAC_EN(FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	DMAC->DMACConfiguration |= (1 << 0);
    }
    else
    {
      	for(int i = 0; i < 8; i++)
      	{
        	if(DMAC_ChannelEnableStatus((DMACChannelx)i))
        	{
          		DMAC_ChannelDisable((DMACChannelx)i);
        	}
      	}
    	DMAC->DMACConfiguration &= ~(1 << 0);	
    }
}
/**
  * @功能:DMA通道源地址配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  *       2.addr 源地址
  * @返回值:无
  */
void DMAC_ChannelSoureAddr(DMACChannelx dmachannel,unsigned int addr)
{
    DMAC->DMACChannel[dmachannel].DMACCxSrcAddr = addr;
}
/**
  * @功能:DMA通道目标地址配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7
  *       2.addr 目标地址
  * @返回值:无
  */
void DMAC_ChannelDestAddr(DMACChannelx dmachannel,unsigned int addr)
{
    DMAC->DMACChannel[dmachannel].DMACCxDestAddr = addr;
}

/**
  * @功能:DMA通道 LLI 配置
  * @注意: 无      
  * @参数:1.dmalli 下一个LLI结构体的首地址，(该地址必须是4字节边界对齐),LLI结构体必须依次包含：源地址、目标地址、下一个LLI的首地址、通道控制寄存器
  *       2.dmamaster 加载LLI的AHB MASTER
  * @返回值:无
  */
void DMAC_ChannelLLI(DMACChannelx dmachannel,unsigned int dmalli,DMAC_AHBMasterx dmamaster)
{
    DMAC->DMACChannel[dmachannel].DMACCxLLI = ((dmalli >> 2) << 2) | (dmamaster << 0);
}
/**
  * @功能:DMA通道源的相关配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.inc 源地址是否增长，INCREMENT，增长；NOINCREMENT，不增长
  *       3.dmamaster 源  搬运使用的DMA AHB master接口，
  *       4.transferwidth 源 传输数据的位宽
  *       5.brustsize 源 突发传输的大小
  * @返回值:无
  */
void DMAC_ChannelSourceConfig(DMACChannelx dmachannel,INCREMENTx inc,\
 	DMAC_AHBMasterx dmamaster,TRANSFERWIDTHx  transferwidth,BURSTSIZEx brustsize)
{
    DMAC->DMACChannel[dmachannel].DMACCxControl &= ~((1 << 26)|(1 << 24)|\
		(0x7 << 18)|(0x7 << 12));
    DMAC->DMACChannel[dmachannel].DMACCxControl |=\
		(inc << 26)|(dmamaster << 24)|(transferwidth << 18)|(brustsize << 12);
}
/**
  * @功能:DMA通道目标的相关配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.inc 目标地址是否增长，INCREMENT，增长；NOINCREMENT，不增长
  *       3.dmamaster 目标  搬运使用的DMA AHB master接口，
  *       4.transferwidth 目标 传输数据的位宽
  *       5.brustsize 目标 突发传输的大小
  * @返回值:无
  */
void DMAC_ChannelDestConfig(DMACChannelx dmachannel,INCREMENTx inc,\
 	DMAC_AHBMasterx dmamaster,TRANSFERWIDTHx  transferwidth,BURSTSIZEx \
	  brustsize)
{
    DMAC->DMACChannel[dmachannel].DMACCxControl &= ~((1<<27)|(1<<25)|\
		(0x7<<21)|(0x7<<15));
    DMAC->DMACChannel[dmachannel].DMACCxControl |=\
		(inc<<27)|(dmamaster<<25)|(transferwidth<<21)|(brustsize<<15);
}
/**
  * @功能:DMA通道传输完成中断使能配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.cmd ENABLE，中断使能；DISABLE，中断禁止
  * @返回值:无
  */
void DMAC_ChannelTCInt(DMACChannelx dmachannel,FunctionalState cmd)
{
    if(cmd != ENABLE)
    {
      	DMAC->DMACChannel[dmachannel].DMACCxControl &= ~(1 << 31);
    }
    else
    {
      	DMAC->DMACChannel[dmachannel].DMACCxControl |= (unsigned int)(1 \
		  << 31);	
    }
}
/**
  * @功能:DMA通道保护位的设置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.access_mode  访问模式，ACCESS_USERMODE 用户模式；ACCESS_PRIVILEGEDMODE，特权模式
  *       3.bufferable 缓冲使能设置，NONBUFFERABLE 无缓冲；BUFFERABLE 缓冲使能
  *		  4.cacheable cache缓存使能设置，CACHEABLE cache使能；NONCACHEABLE 无cache
  * @返回值:无
  */
void DMAC_ChannelProtectionConfig(DMACChannelx dmachannel,ACCESS_MODE access_mode,\
	BUFFERABLEx bufferable,CACHEABLEx cacheable)
{
    DMAC->DMACChannel[dmachannel].DMACCxControl &= ~(0x7 << 28);
    DMAC->DMACChannel[dmachannel].DMACCxControl |= (((cacheable<<2)\
	|(bufferable<<1)|(access_mode<<0))<<28);
}
/**
  * @功能:DMA通道传输大小配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.size 传输大小(1~4095)，如果为0，DMA不会进行数据传输
  * @返回值:无
  */
void DMAC_ChannelTransferSize(DMACChannelx dmachannel,unsigned short size)
{
    volatile unsigned int tmp = 0;
    size &= 0xFFFUL;
    
    tmp = DMAC->DMACChannel[dmachannel].DMACCxControl;
    tmp &= ~(0xFFF << 0);
    tmp |= (size << 0);
    DMAC->DMACChannel[dmachannel].DMACCxControl = tmp;
}

/**
  * @功能:DMA通道传输禁止
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  * @返回值:无
  */
void DMAC_ChannelDisable(DMACChannelx dmachannel)
{
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 18);
    while(DMAC->DMACChannel[dmachannel].DMACCxConfiguration & (1 << 17));
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 0);
}

/**
  * @功能:DMA通道传输使能
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  * @返回值:无
  */
void DMAC_ChannelEnable(DMACChannelx dmachannel)
{
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 18);
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 0);
}
/**
  * @功能:DMA通道中断屏蔽配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.channelintmask CHANNELINTMASK_ITC 传输完成中断；CHANNELINTMASK_IE 传输错误中断
  *       3.cmd ENABLE,屏蔽；DISABLE,不屏蔽
  * @返回值:无
  */
void DMAC_ChannelInterruptMask(DMACChannelx dmachannel,CHANNELINTMASKx \
	channelintmask,FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &=~ (1<<channelintmask);
    }
    else
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration |=(1<<channelintmask);
    }
}
/**
  * @功能:DMA通道其他相关配置
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.destperiph 目标外设
  *       3.srcperiph  源外设
  *       4.flowctrl 流控类型选择
  * @返回值:无
  */
void DMAC_ChannelConfig(DMACChannelx dmachannel,char destperiph,\
  char srcperiph ,DMAC_FLOWCTRL flowctrl)
{
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~((0x7 << 11) | (0xF << 6) | (0xF << 1));
    DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= ((flowctrl << 11) | (destperiph << 6) | (srcperiph << 1));
}
/**
  * @功能:DMA通道传输锁
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.cmd  ENABLE 传输锁住；DISABLE 传输不锁
  * @返回值:无
  */
void DMAC_ChannelLock(DMACChannelx dmachannel,FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 16);
    }
    else
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 16);
    }
}

/**
  * @功能:DMA通道传输(中停)Halt
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.cmd  ENABLE 传输被Halt；DISABLE 传输不Halt
  * @返回值:无
  */
void DMAC_ChannelHalt(DMACChannelx dmachannel,FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 18);
    }
    else
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(1 << 18);
    }
}

/**
  * @功能:DMA通道传输Power控制
  * @注意: 无      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.cmd  ENABLE Power关闭；DISABLE Power打开
  * @返回值:无
  */
void DMAC_ChannelPowerDown(DMACChannelx dmachannel,FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration |= (1 << 19);
    }
    else
    {
      	DMAC->DMACChannel[dmachannel].DMACCxConfiguration &= ~(0x1FFF << 19);
    }
}

/**
  * @功能:DMA通道传输 内存到内存 
  * @注意: 必须给出合法地址, 实际的物理地址      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *       2.srcaddr  源地址
  *       3.destaddr 目标地址
  *       4.bytesize 传输的字节数
  * @返回值:无
  */
void DMAC_M2MConfig(DMACChannelx dmachannel,unsigned int srcaddr,\
  unsigned int destaddr,unsigned int bytesize)
{
    DMAC_AHBMasterx srcmaster,destmaster;
	
	if((GDMA_SDRAM_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_SDRAM_ADDR+GDMA_SDRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster2;
	}
	else if((GDMA_PCMRAM_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_PCMRAM_ADDR+GDMA_PCMRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_FFTRAM_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_FFTRAM_ADDR+GDMA_FFTRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else if((GDMA_SRAM0_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_SRAM0_ADDR+GDMA_SRAM0_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}  
	else if((GDMA_SRAM1_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_SRAM1_ADDR+GDMA_SRAM1_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_CSRAM_ADDR <= srcaddr) && (srcaddr+bytesize\
	  <= GDMA_CSRAM_ADDR+GDMA_CSRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else
	{
		return ;
	}
	if((GDMA_SDRAM_ADDR <= destaddr) && (destaddr+bytesize\
	  <= GDMA_SDRAM_ADDR+GDMA_SDRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}
	else if((GDMA_PCMRAM_ADDR <= destaddr) && (destaddr+bytesize\
	  <= GDMA_PCMRAM_ADDR+GDMA_PCMRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_FFTRAM_ADDR <= destaddr) && (destaddr+bytesize\
	  <= GDMA_FFTRAM_ADDR+GDMA_FFTRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster1;
	}
	else if((GDMA_SRAM0_ADDR <= destaddr) && (destaddr+bytesize\
	  <= GDMA_SRAM0_ADDR+GDMA_SRAM0_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}  
	else if((GDMA_SRAM1_ADDR <= destaddr) && (destaddr+bytesize\
	  <= GDMA_SRAM1_ADDR+GDMA_SRAM1_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}
	else
	{
		return ;
	}
    
    DMAC_ChannelDisable(dmachannel);
    DMAC_ChannelPowerDown(dmachannel,DISABLE);
    DMAC_IntTCClear(DMACChannelALL);
    DMAC_IntErrorClear(DMACChannelALL);
    DMAC_Config(DMAC_AHBMaster1,LittleENDIANMODE);
    DMAC_Config(DMAC_AHBMaster2,LittleENDIANMODE);
    DMAC_EN(ENABLE);
	
    DMAC_ChannelSoureAddr(dmachannel,srcaddr);
    DMAC_ChannelDestAddr(dmachannel,destaddr);
	
    DMAC_ChannelLLI(dmachannel,0,DMAC_AHBMaster1);

    DMAC_ChannelProtectionConfig(dmachannel,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
    DMAC_ChannelTCInt(dmachannel,ENABLE);
    DMAC_ChannelSourceConfig(dmachannel,INCREMENT,srcmaster,\
		TRANSFERWIDTH_32b,BURSTSIZE16);
    DMAC_ChannelDestConfig(dmachannel,INCREMENT,destmaster,\
		TRANSFERWIDTH_32b,BURSTSIZE16);
    DMAC_ChannelTransferSize(dmachannel,bytesize/4+((bytesize%4)?1:0));
	
    DMAC_ChannelHalt(dmachannel,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_ITC,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_IE,ENABLE);
    DMAC_ChannelConfig(dmachannel,0,0,M2M_DMA);
    DMAC_ChannelLock(dmachannel,DISABLE);
    DMAC_ChannelEnable(dmachannel);
}

/**
  * @功能:DMA通道传输 内存到外设，外设到内存 
  * @注意: 不支持外设到外设,必须给出合法地址, 实际的物理地址      
  * @参数:1.dmachannel DMA通道 DMACChannel0~7	
  *		  2.periph 外设
  *			DMAC_Peripherals_SPI0     QSPI 控制器(发送和接收同一个)
  *			DMAC_Peripherals_SPI1_RX  SPI接收
  *			DMAC_Peripherals_SPI1_TX  SPI发送
  *			DMAC_Peripherals_UART0_RX UART0 接收
  *			DMAC_Peripherals_UART0_TX UART0 发送
  *			DMAC_Peripherals_UART1_RX UART1 接收
  *			DMAC_Peripherals_UART1_TX UART1 发送
  *       2.flowctrl  传输方向，M2P_DMA:内存到外设;P2M_DMA 外设到内存
  *       3.destaddr 目标地址
  *       4.bytesize 传输的字节数
  * @返回值:无
  */
void DMAC_M2P_P2MConfig(DMACChannelx dmachannel,DMAC_Peripherals periph,\
  DMAC_FLOWCTRL flowctrl,unsigned int srcaddr,unsigned int destaddr,\
	unsigned int bytesize)
{
    DMAC_AHBMasterx srcmaster,destmaster;
    INCREMENTx srcinc,destinc;
 
	if((GDMA_SDRAM_ADDR <= srcaddr) && (srcaddr\
	  <= GDMA_SDRAM_ADDR+GDMA_SDRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster2;
	}
	else if((GDMA_PCMRAM_ADDR <= srcaddr) && (srcaddr\
	  <= GDMA_PCMRAM_ADDR+GDMA_PCMRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_FFTRAM_ADDR <= srcaddr) && (srcaddr\
	  <= GDMA_FFTRAM_ADDR+GDMA_FFTRAM_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
	else if((GDMA_SRAM0_ADDR <= srcaddr) && (srcaddr\
	  <= GDMA_SRAM0_ADDR+GDMA_SRAM0_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}  
	else if((GDMA_SRAM1_ADDR <= srcaddr) && (srcaddr\
	  <= GDMA_SRAM1_ADDR+GDMA_SRAM1_SIZE))
	{
		srcmaster = DMAC_AHBMaster1;
	}
    
    if((GDMA_SDRAM_ADDR <= destaddr) && (destaddr\
	  <= GDMA_SDRAM_ADDR+GDMA_SDRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}
	else if((GDMA_PCMRAM_ADDR <= destaddr) && (destaddr\
	  <= GDMA_PCMRAM_ADDR+GDMA_PCMRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster1;
	}
	else if ((GDMA_FFTRAM_ADDR <= destaddr) && (destaddr\
	  <= GDMA_FFTRAM_ADDR+GDMA_FFTRAM_SIZE))
	{
		destmaster = DMAC_AHBMaster1;
	}
	else if((GDMA_SRAM0_ADDR <= destaddr) && (destaddr\
	  <= GDMA_SRAM0_ADDR+GDMA_SRAM0_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}  
	else if((GDMA_SRAM1_ADDR <= destaddr) && (destaddr\
	  <= GDMA_SRAM1_ADDR+GDMA_SRAM1_SIZE))
	{
		destmaster = DMAC_AHBMaster2;
	}
    else
    {
      	destmaster = (srcmaster == DMAC_AHBMaster1)?DMAC_AHBMaster2:DMAC_AHBMaster1;
    }
    
    if(M2P_DMA == flowctrl)
    {
      	srcinc =INCREMENT;
      	destinc =NOINCREMENT;
    }
    else if(P2M_DMA == flowctrl)
    {
      	srcinc = NOINCREMENT;
      	destinc = INCREMENT;		
    }
    
    DMAC_ChannelDisable(dmachannel);
    DMAC_ChannelPowerDown(dmachannel,DISABLE);
	
    DMAC_IntTCClear(DMACChannelALL);
    DMAC_IntErrorClear(DMACChannelALL);
    DMAC_Config(DMAC_AHBMaster1,LittleENDIANMODE);
    DMAC_Config(DMAC_AHBMaster2,LittleENDIANMODE);
    DMAC_EN(ENABLE);
	
    DMAC_ChannelSoureAddr(dmachannel,srcaddr);
    DMAC_ChannelDestAddr(dmachannel,destaddr);
	
    DMAC_ChannelLLI(dmachannel,0,DMAC_AHBMaster1);
	
    DMAC_ChannelProtectionConfig(dmachannel,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
    DMAC_ChannelTCInt(dmachannel,ENABLE);
    DMAC_ChannelSourceConfig(dmachannel,srcinc,srcmaster,\
		TRANSFERWIDTH_32b,BURSTSIZE8);
    DMAC_ChannelDestConfig(dmachannel,destinc,destmaster,\
		TRANSFERWIDTH_32b,BURSTSIZE8);
    DMAC_ChannelTransferSize(dmachannel,bytesize/4+((bytesize%4)?1:0));
	
    DMAC_ChannelHalt(dmachannel,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_ITC,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_IE,ENABLE);
	
    DMAC_ChannelConfig(dmachannel,periph,periph,flowctrl);
    DMAC_ChannelLock(dmachannel,DISABLE);
    DMAC_ChannelEnable(dmachannel);
}

void DMAC_ChannelLLIControl(DMACChannelx dmachannel,unsigned int val)
{
    DMAC->DMACChannel[dmachannel].DMACCxControl = val;
}

void DMAC_LLIConfig(DMACChannelx dmachannel,DMAC_LLI * dmac_lli)
{

//	DMAC_AHBMasterx srcmaster,destmaster;
//	INCREMENTx srcinc,destinc;
 
    DMAC_ChannelDisable(dmachannel);
    DMAC_ChannelPowerDown(dmachannel,DISABLE);
	
    DMAC_IntTCClear(DMACChannelALL);
    DMAC_IntErrorClear(DMACChannelALL);
    DMAC_Config(DMAC_AHBMaster1,LittleENDIANMODE);
    DMAC_Config(DMAC_AHBMaster2,LittleENDIANMODE);
    DMAC_EN(ENABLE);
	
    DMAC_ChannelLLIControl(dmachannel,dmac_lli->Control);
	
    DMAC_ChannelSoureAddr(dmachannel,dmac_lli->SrcAddr);
    DMAC_ChannelDestAddr(dmachannel,dmac_lli->DestAddr);
	
    DMAC_ChannelLLI(dmachannel,(unsigned int)dmac_lli->NextLLI,DMAC_AHBMaster2);
	
    DMAC_ChannelHalt(dmachannel,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_ITC,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_IE,ENABLE);
	
    DMAC_ChannelConfig(dmachannel,0,0,M2M_DMA);/*m2m*/
    DMAC_ChannelLock(dmachannel,DISABLE);
    DMAC_ChannelEnable(dmachannel);
}

unsigned int DMAC_LLIContrlreturn(LLI_Control* lli_ctrl)
{
    if(!lli_ctrl)
    {
      return 0;
    }
    
    return (lli_ctrl->TCInt<<31)|\
	  	   (lli_ctrl->CacheAble<<30)|\
		   (lli_ctrl->BufferAble<<29)|\
		   (lli_ctrl->AccessMode<<28)|\
		   (lli_ctrl->DestInc<<27)|\
		   (lli_ctrl->SrcInc<<26)|\
		   (lli_ctrl->DestMaster<<25)|\
		   (lli_ctrl->SrcMaster<<24)|\
		   (lli_ctrl->DestTransferWidth<<21)|\
		   (lli_ctrl->SrcTransferWidth<<18)|\
		   (lli_ctrl->DestBurstSize<<15)|\
		   (lli_ctrl->SrcBurstSize<<12)|\
		   (lli_ctrl->TransferSize<<0);
}

char* srcbuf[]={"#0 hello IAR6.7354657657667667562343343432","#1 chipintelli",\
  "#2 bing.xu3567i7654324567890876543245678909876",\
  "#3 hello HMC6457908-=09876543","#4 DMA tail657890-0098765","#5 LLI DMA546789","#6 LAST LLI","#7 smteef fer",\
	"#8 3r45t54gntrjhnt","#9 43ergnjgnjndf","#10 weregnjdfgb"};
char destbuf[15][200]={0};

void DMAC_LLItest(DMACChannelx dmachannel)
{
    //NVIC_InitTypeDef NVIC_InitStruct = {0};
    LLI_Control lli_ctrl = {
		.TCInt			=ENABLE,
		.CacheAble		=NONCACHEABLE,
		.BufferAble		=NONBUFFERABLE,
		.AccessMode		=ACCESS_USERMODE,
		.DestInc		=INCREMENT,
		.SrcInc			=INCREMENT,
		.DestMaster		=DMAC_AHBMaster1,
		.SrcMaster		=DMAC_AHBMaster2,
		.DestTransferWidth	=TRANSFERWIDTH_32b,
		.SrcTransferWidth	=TRANSFERWIDTH_32b,
		.DestBurstSize	=BURSTSIZE4,
		.SrcBurstSize	=BURSTSIZE4,
		.TransferSize	=40,
	};
    DMAC_LLI lli[20] = {
	  [0]={
	  	.SrcAddr =(unsigned int)srcbuf[0],
		.DestAddr=(unsigned int)destbuf[0],
		.NextLLI =(unsigned int)&lli[1],
		.Control =0,
	  },
	  [1]={
	  	.SrcAddr =(unsigned int)srcbuf[1],
		.DestAddr=(unsigned int)destbuf[1],
		.NextLLI =(unsigned int)&lli[2],
		.Control =0,
	  },
	  [2]={
	  	.SrcAddr =(unsigned int)srcbuf[2],
		.DestAddr=(unsigned int)destbuf[2],
		.NextLLI =(unsigned int)&lli[3],
		.Control =0,
	  },
	  [3]={
	  	.SrcAddr =(unsigned int)srcbuf[3],
		.DestAddr=(unsigned int)destbuf[3],
		.NextLLI =(unsigned int)&lli[4],
		.Control =0,
	  },
	  [4]={
	  	.SrcAddr =(unsigned int)srcbuf[4],
		.DestAddr=(unsigned int)destbuf[4],
		.NextLLI =(unsigned int)&lli[5],
		.Control =0,
	  },
	  [5]={
	  	.SrcAddr =(unsigned int)srcbuf[5],
		.DestAddr=(unsigned int)destbuf[5],
		.NextLLI =(unsigned int)&lli[6],
		.Control =0,
	  },
	  [6]={
	  	.SrcAddr =(unsigned int)srcbuf[6],
		.DestAddr=(unsigned int)destbuf[6],
		.NextLLI =(unsigned int)&lli[7],
		.Control =0,
	  },
	  [7]={
	  	.SrcAddr =(unsigned int)srcbuf[7],
		.DestAddr=(unsigned int)destbuf[7],
		.NextLLI =(unsigned int)&lli[8],
		.Control =0,
	  },
	  [8]={
	  	.SrcAddr =(unsigned int)srcbuf[8],
		.DestAddr=(unsigned int)destbuf[8],
		.NextLLI =(unsigned int)&lli[9],
		.Control =0,
	  },
	  [9]={
	  	.SrcAddr =(unsigned int)srcbuf[9],
		.DestAddr=(unsigned int)destbuf[9],
		.NextLLI =(unsigned int)&lli[10],
		.Control =0,
	  },
	  [10]={
	  	.SrcAddr =(unsigned int)srcbuf[10],
		.DestAddr=(unsigned int)destbuf[10],
		.NextLLI =0,
		.Control =0,
	  },
	  [11]={0},
	};
    
    int tmp = DMAC_LLIContrlreturn(&lli_ctrl);
    
    for(int i = 0; i < 12; i++)
    {
      lli[i].Control=tmp;
    }
#if 0
    NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    Scu_SetDeviceGate((unsigned int)DMA,ENABLE);
#endif
    DMAC_LLIConfig(dmachannel,&lli[0]);
}


void SMT_DMA_LLItest(void)
{
    DMAC_LLItest(DMACChannel0);
}

void GDMA_Traversal(void)
{
    //Scu_SetDeviceGate((unsigned int)DMA,ENABLE);
    //DMAC_LLIConfig();
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/