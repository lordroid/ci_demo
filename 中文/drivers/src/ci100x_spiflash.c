/**
  ******************************************************************************
  * @文件    ci100x_spiflash.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数控制SPI外接FLASH.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli 公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#include "ci100x_spiflash.h"
#include "ci100x_dma.h"
#include "ci100x_scu.h"
#include "ci100x_uart.h"
 
extern volatile int g_dma_translate_ok;
/**
  * @功能:设置写 状态寄存器 时间 Tw
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tw 状态寄存器自写时间 tw
  * @返回值:无
  */
void SPIFlash_WrStatusRegTime(SPICInit_Typedef* QSPICx,unsigned int tw)
{
    QSPICx->SPIC_CSR_00 = tw & 0xffffff;
}

/**
  * @功能:设置 spiflash 的spi总线模式
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.spimode 总线模式 
  *		    Spi_BusMode_StdSpi 标准spi,Spi_BusMode_DualSpi 两线式,Spi_BusMode_QuadSpi 四线式
  * @返回值:无
  */
void SPIFlash_BusMode(SPICInit_Typedef* QSPICx,Spi_BusMode spimode)
{
    QSPICx->SPIC_CSR_01	= spimode;
}

/**
  * @功能:设置 spiflash 的控制器的等待擦除完成的步进计数值
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.erase_counter_tap 
  * @返回值:无
  */
void SPIFlash_EraseCounterTap(SPICInit_Typedef* QSPICx,unsigned int erase_counter_tap)
{
    QSPICx->SPIC_CSR_02 = erase_counter_tap;
}

/**
  * @功能:spiflash 的控制器 dma_en 在HCLK时钟域，当CPU读写数据时，dma_en_hclk_status的状态
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组,只能是QSPIC
  * @返回值:0,CPU可以读写数据；非0,CPU不能读写数据
  */
unsigned int SPIFlash_DMAENHclk_Status(SPICInit_Typedef* QSPICx)
{
    volatile unsigned int tmp = 0xfffff;
    while((QSPICx->SPIC_CSR_03 & 1) && tmp--);
    return QSPICx->SPIC_CSR_03;
}


/**
  * @功能:spiflash 的控制器 SPI协议模式配置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.spiclkmode SPI时钟模式，共有4中，但SPI FLASH器件只支持模式0和模式3 	
  * @返回值:无
  */
void SPIFlash_SpiclkMode(SPICInit_Typedef* QSPICx,Spi_ClkMode spiclkmode)
{
    QSPICx->SPIC_CSR_04 &= ~(1 << 2);
    QSPICx->SPIC_CSR_04 |= (spiclkmode << 2);
}

/**
  * @功能:spiflash 的控制器fast_read_io设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.fast_read_io  	
  * @返回值:无
  */
void SPIFlash_FastReadIO(SPICInit_Typedef* QSPICx,Fast_ReadIO fast_read_io)
{
    QSPICx->SPIC_CSR_04 &= ~(1 << 1);
    QSPICx->SPIC_CSR_04 |= (fast_read_io << 1);
}

/**
  * @功能:spiflash 的控制器fast_read 使能设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.cmd  ENABLE 使用fast_read ，DISABLE 不使用fast_read 	
  * @返回值:无
  */
void SPIFlash_FastRead(SPICInit_Typedef* QSPICx,FunctionalState cmd)
{
    if(cmd != ENABLE)
    {
      	QSPICx->SPIC_CSR_04 &= ~(1 << 0);
    }
    else
    {
      	QSPICx->SPIC_CSR_04 |= (1 << 0);
    }
}

/**
  * @功能:spiflash 的控制器读写数据字节数设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.sizebyte 字节数	，写,只能是256(spiflash只能按页写，因此必须为页的大小),；读，没有限制
  * @返回值:无
  */
void SPIFlash_WrRdNums(SPICInit_Typedef* QSPICx,unsigned int sizebyte)
{
    QSPICx->SPIC_CSR_05 = sizebyte;
}

/**
  * @功能:spiflash 的控制器命令码设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.cmdcode spi部分命令码(见FLASH手册)
  *       3.cmdtype 本QSPI控制器，支持的命令类型 ，comcode必须配合命令类型使用
  *       4.dmaen 必须为ENABLE
  * @返回值:无
  */
void SPIFlash_CommandConfig(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode \
  cmdcode,SpiFlash_CmdType cmdtype,FunctionalState dmaen)
{
    QSPICx->SPIC_CSR_06 &= ~((0xFF << 6) | (1 << 5) | (0xF << 1));
    QSPICx->SPIC_CSR_06 |= (cmdcode << 6) | (1 << 5) | (cmdtype << 1);
}

/**
  * @功能:等待spiflash 的控制器命令有效设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  * @返回值:无
  */
void SPIFlash_WaitCommandVaild(SPICInit_Typedef* QSPICx)
{
    volatile  int timeout = 0x2FFFFFFF;
  	while((QSPICx->SPIC_CSR_06 & (1 << 0)) && timeout--);
}	   
  
/**
  * @功能:spiflash 的控制器命令有效设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.cmd ENABLE 命令有效，DISABLE 命令清除
  * @返回值:无
  */
void SPIFlash_CommandVaild(SPICInit_Typedef* QSPICx,FunctionalState cmd)
{
    if(cmd != DISABLE)
    {
      	QSPICx->SPIC_CSR_06 |= (1 << 0);
      	SPIFlash_WaitCommandVaild(QSPICx);
    }
    else
    {
      	QSPICx->SPIC_CSR_06 &= ~(1 << 0);
    }
}

/**
  * @功能:spiflash 的控制器中断设置(不推荐使用QSPI中断)
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.中断不可用
  * @返回值:无
  */
void SPIFlash_Interrupt(SPICInit_Typedef* QSPICx)
{
    QSPICx->SPIC_CSR_07 = (1 << 1);
}

/**
  * @功能:spiflash 的控制器 向DMA发出请求的数据字节数 
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.size
  * @返回值:无
  */
void SPIFlash_DMARequestSize(SPICInit_Typedef* QSPICx,DMARequestSizeBytes\
  size)
{
    QSPICx->SPIC_CSR_08 = size;
}

/**
  * @功能:spiflash 的flash 读写地址
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr
  * @返回值:无
  */
void SPIFlash_WrRdAddr(SPICInit_Typedef* QSPICx,unsigned int addr)
{
    QSPICx->SPIC_CSR_09 = addr;
}

/**
  * @功能:spiflash 的对flash页写操作时，flash的自写稳定时间
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tpp
  * @返回值:无
  */
void SPIFlash_PageProgramTpp(SPICInit_Typedef* QSPICx,unsigned int tpp)
{
    QSPICx->SPIC_CSR_10 = tpp;
}

/**
  * @功能:spiflash 的对flash的扇区擦除操作时，flash的擦除完成时间
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tse
  * @返回值:无
  */
void SPIFlash_SectorErase4kTse(SPICInit_Typedef* QSPICx,unsigned int tse)
{
    QSPICx->SPIC_CSR_11 = tse;
}

/**
  * @功能:spiflash 的对flash的块擦除(32Kbytes)操作时，flash的擦除完成时间
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tbe1
  * @返回值:无
  */
void SPIFlash_BlockErase32kTbe1(SPICInit_Typedef* QSPICx,unsigned int tbe1)
{
    QSPICx->SPIC_CSR_12 = tbe1;
}

/**
  * @功能:spiflash 的对flash的块擦除(64Kbytes)操作时，flash的擦除完成时间
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tbe2
  * @返回值:无
  */
void SPIFlash_BlockErase64kTbe2(SPICInit_Typedef* QSPICx,unsigned int tbe2)
{
    QSPICx->SPIC_CSR_13 = tbe2;
}

/**
  * @功能:spiflash 的对flash的片擦除(全片)操作时，flash的擦除完成时间
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tce
  * @返回值:无
  */
void SPIFlash_ChipEraseTce(SPICInit_Typedef* QSPICx,unsigned int tce)
{
    QSPICx->SPIC_CSR_14 = tce;
}

/**
  * @功能:spiflash 的对flash的CS取消选定时间设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tcsh1,读操作取消CS时间
  *       3.tcsh2,擦除/写操作取消CS时间
  * @返回值:无
  */
void SPIFlash_CSdeselecteTime(SPICInit_Typedef* QSPICx,unsigned int tcsh1,unsigned int tcsh2)
{
    QSPICx->SPIC_CSR_15 = (tcsh1 << 5) | (tcsh2 << 0);
}

/**
  * @功能:spiflash 的对flash的Power Down时间设置
  * @注意:无       
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.tres,释放Power Down 时间
  *       3.tdp,cs取消到power down时间
  * @返回值:无
  */
void SPIFlash_PowerDownTime(SPICInit_Typedef* QSPICx,unsigned int tres,unsigned int tdp)
{
    QSPICx->SPIC_CSR_16 = (tres << 16) | (tdp << 0);
}

/**
  * @功能:配置SPI 非boot 模式
  * @注意:无       
  * @参数:1.无
  * @返回值:无
  */
void QSPINoBootSet(void)
{
    SCU->CKCFG_LOCK = 0x51AC0FFE;
    SCU->SYS_CTRL &= ~(1 << 1);
    //SCU->CKCFG_LOCK = 0;
}

/**
  * @功能:配置SPI FLASH对应的 DMA控制器
  * @注意:无       
  * @参数:1.dmachannel DMA的通道
  *       2.spidma_config 配置参数指针
  * @返回值:无
  */
static void SPIFLASH_DMAC_M2P_P2MConfig(DMACChannelx dmachannel,SPIDMAInit_Struct* spidma_config)
{
    DMAC_AHBMasterx srcmaster,destmaster;
    INCREMENTx srcinc,destinc;
    srcmaster = DMAC_AHBMaster1;
    destmaster = DMAC_AHBMaster2;
    if(spidma_config->srcaddr >= 0x70000000)
    {
      	srcmaster = DMAC_AHBMaster2;
      	destmaster = DMAC_AHBMaster1;
    }
    if(spidma_config->destaddr >= 0x70000000)
    {
      	srcmaster = DMAC_AHBMaster1;
      	destmaster = DMAC_AHBMaster2;
    }
    if(M2P_DMA == spidma_config->flowctrl)
    {
    	srcinc = INCREMENT;
    	destinc = NOINCREMENT;
    }
    else if(P2M_DMA==spidma_config->flowctrl)
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
	
    DMAC_ChannelSoureAddr(dmachannel,spidma_config->srcaddr);
    DMAC_ChannelDestAddr(dmachannel,spidma_config->destaddr);
	
    DMAC_ChannelLLI(dmachannel,0,DMAC_AHBMaster1);
	
    DMAC_ChannelProtectionConfig(dmachannel,ACCESS_USERMODE,NONBUFFERABLE,NONCACHEABLE);
    DMAC_ChannelTCInt(dmachannel,ENABLE);
    DMAC_ChannelSourceConfig(dmachannel,srcinc,srcmaster,\
        spidma_config->transferwidth,spidma_config->busrtsize);
    DMAC_ChannelDestConfig(dmachannel,destinc,destmaster,\
        spidma_config->transferwidth,spidma_config->busrtsize);
    DMAC_ChannelTransferSize(dmachannel,spidma_config->bytesize);
	
    DMAC_ChannelHalt(dmachannel,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_ITC,DISABLE);
    DMAC_ChannelInterruptMask(dmachannel,CHANNELINTMASK_IE,ENABLE);
	
    DMAC_ChannelConfig(dmachannel,DMAC_Peripherals_SPI0,DMAC_Peripherals_SPI0,spidma_config->flowctrl);
    DMAC_ChannelLock(dmachannel,DISABLE);
    DMAC_ChannelEnable(dmachannel);
}
 
/**
  * @功能:SPI flash 读ManfacturerIDDeviceID (标准SPI模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.manid 读取到的ManfacturerID存储首地址
  *       3.deviceid 读取到的DeviceID存储首地址
  * @返回值：无
  */
void SpiFlash_ReadManfacturerIDDeviceID(SPICInit_Typedef* QSPICx,\
  unsigned char* manid,unsigned char* deviceid)
{
    SPIDMAInit_Struct spidma_config;
    char rx_buf[12] = {0};
	volatile int timeout = 0x7FFFFFFF;
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,2);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_ReadmanufacturerIDDeviceID,\
        SpiFlash_CmdType_ReadManufacturerID,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)rx_buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = 1;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
    *manid = rx_buf[0];
    *deviceid = rx_buf[1];
}

/**
  * @功能:SPI flash 读JedecID (标准SPI模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.jedecid 读取到的id存储首地址，至少3个字节 
  * @返回值：无
  */
void SpiFlash_ReadJedecID(SPICInit_Typedef* QSPICx,unsigned char* jedecid)
{
    SPIDMAInit_Struct spidma_config;
    char rx_buf[12] = {0};
	volatile int timeout = 0x7FFFFFFF;
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,3);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_ReadJedecID,\
       SpiFlash_CmdType_ReadJedecID,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)rx_buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = 1;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
    jedecid[0] = rx_buf[0];/*manuf ID*/
    jedecid[1] = rx_buf[1];
    jedecid[2] = rx_buf[2];
}

/**
  * @功能:SPI flash 读取状态寄存器 (标准SPI模式)
  * @注意:多用于查busy位，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.read_status_regx 
  *             SpiFlash_CmdCode_ReadStatusReg1 读状态寄存器1的值
  *             SpiFlash_CmdCode_ReadStatusReg2 读状态寄存器2的值
  * @返回值：无
  */
char SpiFlash_ReadStatusReg(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode \
  read_status_regx)
{
    SPIDMAInit_Struct spidma_config;
    char rx_buf[12] = {0};
	volatile int timeout = 0x7FFFFFFF;
	
    QSPINoBootSet();
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,2);
    SPIFlash_CommandConfig(QSPICx,read_status_regx,\
        SpiFlash_CmdType_RdStatusReg,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)rx_buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = 1;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
    return rx_buf[0];
}

/**
  * @功能:SPI flash 写状态寄存器(标准SPI模式)
  * @注意: 写状态寄存器前，最好检测一下flash的保护状态，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.reg1 状态寄存器1的值
  *       3.reg2 状态寄存器2的值
  * @返回值：无
  */
void SpiFlash_WrStatusReg(SPICInit_Typedef* QSPICx,char reg1,char reg2)
{
    SPIDMAInit_Struct spidma_config;
    char tx_buf[12] = {0};
    volatile int timeout = 0x7FFFFFFF;
	tx_buf[0] = reg1;
	tx_buf[1] = reg2;
	
    SPIFlash_PageProgramTpp(QSPICx,0x0);
    SPIFlash_WrStatusRegTime(QSPICx,0x0);
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,2);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_WrStatusReg,\
	  SpiFlash_CmdType_WrStatusReg,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    spidma_config.srcaddr =(unsigned int)tx_buf ;
    spidma_config.destaddr=SPI0FIFO_BASE;
    spidma_config.transferwidth=TRANSFERWIDTH_32b;
    spidma_config.flowctrl=M2P_DMA;
    spidma_config.busrtsize=BURSTSIZE1;
    spidma_config.bytesize=1;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    SPIFlash_WaitCommandVaild(QSPICx);
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1) \
	  & (1 << 0)) && timeout--);	
}
/**
  * @功能:SPI flash powerdown  序列(标准SPI模式)
  * @注意: 一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  * @返回值：无
  */
void SpiFlash_Powerdown(SPICInit_Typedef* QSPICx)
{
    volatile unsigned int tDP = 0x5ffff;
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,0);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_Powerdown,\
	  SpiFlash_CmdType_Powerdown,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,0x0);
    SPIFlash_CSdeselecteTime(QSPICx,0,0);
    SPIFlash_PowerDownTime(QSPICx,0,0);
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while(tDP--);
}
/**
  * @功能:SPI flash 释放powerdown  序列(标准SPI模式)
  * @注意: 一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  * @返回值：无
  */
void SpiFlash_ReleasePowerdown(SPICInit_Typedef* QSPICx)
{
    volatile unsigned int tRES1 = 0x5ffff;
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,0);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_ReleasePowerdown,SpiFlash_CmdType_Releasepowerdown,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,0x0);
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while(tRES1--);
}
/**
  * @功能:SPI flash flash的擦除 (标准SPI模式)
  * @注意:操作前、后必须读取状态寄存器reg2,查看busy位，是否完成操作，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.erase_type_code 擦除命令 类型 
  *			SpiFlash_CmdCode_SectorErase4k	扇区擦除 ，大小是4kbytes ，地址必须按4kbyte对齐
  *      	SpiFlash_CmdCode_BlockErase32k	块擦除，大小是32kbytes,地址必须按32kbytes对齐
  *     	SpiFlash_CmdCode_BlockErase64k	块擦除，大小是32kbytes,地址必须按32kbytes对齐
  *     	SpiFlash_CmdCode_ChipErase	全片擦除，地址无所谓
  * @返回值：无
  */
void SpiFlash_Erase(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode \
  erase_type_code,unsigned int addr)
{
    SpiFlash_CmdType erasetype;
    volatile int timeout = 0x7FFFFFFF;
    switch (erase_type_code)
    {
    case SpiFlash_CmdCode_SectorErase4k:
      	erasetype=SpiFlash_CmdType_SectorErase4k;
      	SPIFlash_SectorErase4kTse(QSPICx,0x0);
     	break;
    case SpiFlash_CmdCode_BlockErase32k:
      	erasetype=SpiFlash_CmdType_BlockErase32k;
      	SPIFlash_BlockErase32kTbe1(QSPICx,0x0);
      	break;
    case SpiFlash_CmdCode_BlockErase64k:
      	erasetype=SpiFlash_CmdType_BlockErase64k;
      	SPIFlash_BlockErase64kTbe2(QSPICx,0x0);
      	break;
    case SpiFlash_CmdCode_ChipErase:
      	erasetype=SpiFlash_CmdType_ChipErase;
      	SPIFlash_ChipEraseTce(QSPICx,0x0);
      	break;
    default:
      	erasetype=SpiFlash_CmdType_SectorErase4k;
      	SPIFlash_SectorErase4kTse(QSPICx,0x0);
      	break;
    }
    
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,0);
    SPIFlash_CommandConfig(QSPICx,erase_type_code,erasetype,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,addr);

    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) &&  timeout--);
}
/**
  * @功能:SPI flash Page 编程 (标准SPI模式，一线模式)
  * @注意:一页大小是256字节，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.page_start_addr 要写入的页首地址，必须为某也的起始地址，即按256对齐
  *       3.buf 要写入的数据缓冲首地址，大小必须不小于256字节
  * @返回值：无
  */
void SpiFlash_PageProgram(SPICInit_Typedef* QSPICx,char* buf,\
  unsigned int page_start_addr)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;
    SPIFlash_PageProgramTpp(QSPICx,0x0);
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,256);
    SPIFlash_WrRdAddr(QSPICx,page_start_addr);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_PageProgram,\
	  SpiFlash_CmdType_Program,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_64word);
    spidma_config.srcaddr =(unsigned int)buf ;
    spidma_config.destaddr=SPI0FIFO_BASE;
    spidma_config.transferwidth=TRANSFERWIDTH_32b;
    spidma_config.flowctrl=M2P_DMA;
    spidma_config.busrtsize=BURSTSIZE256;
    spidma_config.bytesize=64;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
}

/**
  * @功能:SPI flash 连续Page 编程 (一线模式)
  * @注意:一页大小是256字节，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.page_start_addr 要写入的页首地址
  *       3.buf 要写入的数据缓冲首地址，大小必须不小于256字节与页数的积
  *       4.pages 要写入的总页数 
  * @返回值：无
  */
void SpiFlash_SequencePageProgram(SPICInit_Typedef* QSPICx,\
  char* buf,unsigned int page_start_addr,unsigned int pages)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;
    SPIFlash_PageProgramTpp(QSPICx,0x0);

    SPIFlash_CommandVaild(QSPICx,DISABLE);
    spidma_config.destaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = M2P_DMA;
    spidma_config.busrtsize = BURSTSIZE256;
    spidma_config.bytesize = 64;
    
    for(int i = 0; i < pages; i++)
    {
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,DISABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_PageProgram,\
			SpiFlash_CmdType_Program,ENABLE);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_64word);
					
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_WrRdNums(QSPICx,256);
		SPIFlash_WrRdAddr(QSPICx,page_start_addr + i * 256);
		spidma_config.srcaddr = (unsigned int)(&buf[i * 256]) ;

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout-- );
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout-- );
    }
}


/**
  * @功能:SPI flash 一次Page 编程 (四线模式)
  * @注意:一页大小是256字节，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.page_start_addr 要写入的页首地址
  *       3.buf 要写入的数据缓冲首地址，大小必须不小于256字节
  * @返回值：无
  */
void SpiFlash_QuadInputPageProgram(SPICInit_Typedef* QSPICx,char* buf,\
  unsigned int page_start_addr)
{
    volatile SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    SPIFlash_PageProgramTpp(QSPICx,0x0);
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,ENABLE);
    SPIFlash_WrRdNums(QSPICx,256);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_QuadInputPageProgram,\
	  SpiFlash_CmdType_Program,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_WrRdAddr(QSPICx,page_start_addr);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_64word);
    spidma_config.srcaddr = (unsigned int)buf ;
    spidma_config.destaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = M2P_DMA;
    spidma_config.busrtsize = BURSTSIZE256;
    spidma_config.bytesize = 64;
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,(SPIDMAInit_Struct*)&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    timeout = 0x7FFFFFFF;
	while((!g_dma_translate_ok )&& timeout--);
    g_dma_translate_ok = 0;
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
}

/**
  * @功能:SPI flash 连续Page 编程 (四线模式)
  * @注意:一页大小是256字节，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.page_start_addr 要写入的页首地址
  *       3.buf 要写入的数据缓冲首地址，大小必须不小于256字节与页数的积
  *       4.pages 要写入的总页数 
  * @返回值：无
  */
void SpiFlash_QuadInputSequencePageProgram(SPICInit_Typedef* QSPICx,\
  char* buf,unsigned int page_start_addr,unsigned int pages)
{
    SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    SPIFlash_PageProgramTpp(QSPICx,0x0);
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    spidma_config.destaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = M2P_DMA;
    spidma_config.busrtsize = BURSTSIZE256;
    spidma_config.bytesize = 64;
    
    for(int i = 0; i < pages; i++)
    {
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,ENABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_QuadInputPageProgram,\
			SpiFlash_CmdType_Program,ENABLE);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_64word);
					
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_WrRdNums(QSPICx,256);
		SPIFlash_WrRdAddr(QSPICx,page_start_addr + i * 256);
		spidma_config.srcaddr = (unsigned int)(&buf[i * 256]) ;

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout-- );
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout-- );
    }
}

/**
  * @功能:SPI flash 读数据 (标准SPI模式,一线模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于size
  *       4.size 总共要读取的字节数 (size: 1~4095)
  * @返回值：无
  */
void SpiFlash_ReadData(SPICInit_Typedef* QSPICx,unsigned int addr,char \
  *buf,unsigned int size)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0))&&timeout--);

    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,size);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_ReadData,\
	  SpiFlash_CmdType_Read,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,addr);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = size / 4 + ((size % 4)?1:0);
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
	timeout = 0x7FFFFFFF;
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
}


/**
  * @功能:SPI flash 按扇区(4kbytes)快速读,(一线模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于 页的大小与扇区数的积 字节
  *       4.sectors 总共要读取的扇区(4kbytes)数
  * @返回值：无
  */
void SpiFlash_ReadDataSector(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;

	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
 
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE16;
    spidma_config.bytesize = 1024;
    
    for(unsigned int i = 0; i < sectors; i++)
    {
		SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,DISABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_ReadData,\
			SpiFlash_CmdType_Read,ENABLE);
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_16word);
		SPIFlash_WrRdNums(QSPICx,4096);
		SPIFlash_WrRdAddr(QSPICx,addr + i * 4096);
		spidma_config.destaddr=(unsigned int)&buf[i * 4096];

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout--);
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout--);
    }
}

/**
  * @功能:SPI flash 快速读 (标准SPI模式,一线模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于size
  *       4.size 总共要读取的字节数 (size: 1~4095)
  * @返回值：无
  */
void SpiFlash_FastRead(SPICInit_Typedef* QSPICx,unsigned int addr,char*\
  buf,unsigned int size)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1) \
	  & (1<<0)) && timeout--);
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,size);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastRead,\
	  SpiFlash_CmdType_Read,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,addr);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = size / 4 + ((size % 4)?1:0);
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
	timeout = 0x7FFFFFFF;
    while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;	
}

/**
  * @功能:SPI flash 快速读 (两线模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于size
  *       4.size 总共要读取的字节数(size: 1~4095)
  * @返回值：无
  */
void SpiFlash_FastReadDualOutput(SPICInit_Typedef* QSPICx,unsigned int\
  addr,char* buf,unsigned int size)
{
    SPIDMAInit_Struct spidma_config;
	volatile int timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
 
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_DualSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,ENABLE);
    SPIFlash_WrRdNums(QSPICx,size);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadDualOutput,\
	  SpiFlash_CmdType_Read,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,addr);
    spidma_config.srcaddr =   SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = size / 4 + ((size % 4)?1:0);
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    timeout = 0x7FFFFFFF;
	while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
}

/**
  * @功能:SPI flash 快速读 (四线模式)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于size
  *       4.size 总共要读取的字节数 (size: 1~4095)
  * @返回值：无
  */
void SpiFlash_FastReadQuadOutput(SPICInit_Typedef* QSPICx,unsigned int addr,\
  char *buf,unsigned int size)
{
    SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF; 
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0) )&& timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
    timeout = 0x7FFFFFFF; 
	while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1<<0)) && timeout--);	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,ENABLE);
    SPIFlash_WrRdNums(QSPICx,size);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadQuadOutput,\
	  SpiFlash_CmdType_Read,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,addr);
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.destaddr = (unsigned int)buf;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE1;
    spidma_config.bytesize = size / 4 + ((size % 4)?1:0);
    SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
	
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    timeout = 0x7FFFFFFF; 
	while((!g_dma_translate_ok) && timeout--);
    g_dma_translate_ok = 0;
}

/**
  * @功能:SPI flash 按页快速读
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于 页的大小与页数的积 字节
  *       4.pages 总共要读取的页数
  * @返回值：无
  */
void SpiFlash_FastReadPageQuadOutput(SPICInit_Typedef* QSPICx,unsigned int\
  addr,char *buf,unsigned int pages)
{
    volatile SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1<<0)) && timeout--);
    SPIFlash_CommandVaild(QSPICx,DISABLE);

    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE16;
    spidma_config.bytesize = 64;
    
    for(unsigned int i=0;i<pages;i++)
    {
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,ENABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadQuadOutput,\
			SpiFlash_CmdType_Read,ENABLE);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_16word);
		SPIFlash_WrRdNums(QSPICx,256);
		SPIFlash_WrRdAddr(QSPICx,addr+i*256);
		spidma_config.destaddr = (unsigned int)&buf[i * 256];

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,(SPIDMAInit_Struct*)&spidma_config);
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout--);
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout--);
    }
}

/**
  * @功能:SPI flash 按扇区(4kbytes)快速读 ,(两线线模式,Dual Output)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于 页的大小与扇区数的积 字节
  *       4.sectors 总共要读取的扇区(4kbytes)数
  * @返回值：无
  */
void SpiFlash_FastReadSectorDualOutput(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors)
{
    SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
 
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE16;
    spidma_config.bytesize = 1024;
    
    for(unsigned int i = 0; i < sectors; i++)
    {
		SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,ENABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadDualOutput,\
			SpiFlash_CmdType_Read,ENABLE);
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_16word);
		SPIFlash_WrRdNums(QSPICx,4096);
		SPIFlash_WrRdAddr(QSPICx,addr + i * 4096);
		spidma_config.destaddr=(unsigned int)&buf[i * 4096];

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout--);
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout--);
    }
}

/**
  * @功能:SPI flash 按扇区(4kbytes)快速读 ,(四线模式,Quad Output)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于 页的大小与扇区数的积 字节
  *       4.sectors 总共要读取的扇区(4kbytes)数
  * @返回值：无
  */
void SpiFlash_FastReadSectorQuadOutput(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors)
{
    SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
 
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE16;
    spidma_config.bytesize = 1024;
    
    for(unsigned int i = 0; i < sectors; i++)
    {
		SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
		SPIFlash_FastRead(QSPICx,ENABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadQuadOutput,\
			SpiFlash_CmdType_Read,ENABLE);
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_16word);
		SPIFlash_WrRdNums(QSPICx,4096);
		SPIFlash_WrRdAddr(QSPICx,addr + i * 4096);
		spidma_config.destaddr=(unsigned int)&buf[i * 4096];

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
		timeout = 0x7FFFFFFF;
		while((!g_dma_translate_ok) && timeout--);
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout--);
    }
}

/**
  * @功能:SPI flash 按扇区(4kbytes)快速读 ,(四线模式,Quad IO)
  * @注意:一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.addr 读取flash的起始地址
  *       3.buf 读到的数据缓冲首地址，大小必须不小于 页的大小与扇区数的积 字节
  *       4.sectors 总共要读取的扇区(4kbytes)数
  * @返回值：无
  */
 
void SpiFlash_FastReadSectorQuadIO(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors)
{
    SPIDMAInit_Struct spidma_config;
    char reg1 = 0,reg2 = 0;
	volatile int timeout = 0x7FFFFFFF;
    do{
      	reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    }while((reg1 & (1 << 0)) && timeout--);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    reg2 |= (1 << 1);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
	timeout = 0x7FFFFFFF;
    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (1 << 0)) && timeout--);
	
    SPIFlash_CommandVaild(QSPICx,DISABLE);
 
    spidma_config.srcaddr = SPI0FIFO_BASE;
    spidma_config.transferwidth = TRANSFERWIDTH_32b;
    spidma_config.flowctrl = P2M_DMA;
    spidma_config.busrtsize = BURSTSIZE16;
    spidma_config.bytesize = 1024;
    
    for(unsigned int i = 0; i < sectors; i++)
    {
    spi_read_rty:
      
		SPIFlash_BusMode(QSPICx,Spi_BusMode_QuadSpi);
		SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
		SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_addr_data);
		SPIFlash_FastRead(QSPICx,ENABLE);
		SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_FastReadQuadIO,\
			SpiFlash_CmdType_Read,ENABLE);
		SPIFlash_Interrupt(QSPICx);
		SPIFlash_DMARequestSize(QSPICx,DMARequestSize_16word);
		SPIFlash_WrRdNums(QSPICx,4096);
		SPIFlash_WrRdAddr(QSPICx,addr + i * 4096);
		spidma_config.destaddr=(unsigned int)&buf[i * 4096];

		SPIFLASH_DMAC_M2P_P2MConfig(DMACChannel0,&spidma_config);
					
		SPIFlash_CommandVaild(QSPICx,ENABLE);
#if 0        
		timeout = 0x7FFFFFFF;
        while((!g_dma_translate_ok) && timeout--);
#else        
        timeout = 40000;
		while((!g_dma_translate_ok) && timeout--);
        if(timeout<10)
        {
            mprintf("spi read DMA timeout\r\n");
            goto spi_read_rty;
        }
#endif        
		g_dma_translate_ok = 0;
		timeout = 0x7FFFFFFF;
		while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
		  & (1 << 0)) && timeout--);
    }
}

/**
  * @功能:SPI flash 保护 打开/关闭
  * @注意:保护打开要谨慎，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  *       2.cmd ENABLE 开启保护，DISABLE 关闭保护
  * @返回值：spi flash的保护状态
  *          0 软件保护；1 保护由硬件决定 /wp为低，则保护；
  *          2 必须产生上电序列才能写；3 一次性编程保护，该flash被永久性保护	
  */
char SpiFlash_Protect(SPICInit_Typedef* QSPICx,FunctionalState cmd)
{
    unsigned char reg1 = 0,reg2 = 0,SRP1 = 0,SRP0 = 0,ret = 0;
    volatile SpiFlash_StatusProtect StatuProtect=SpiFlash_Resv;
	volatile int timeout = 0x7FFFFFFF;
	
    reg1 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1);
    reg2 = SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg2);
    SRP0 = (reg1 &(1 << 7))?1:0;
    SRP1 = (reg2 &(1 << 0))?1:0;
    StatuProtect =(SpiFlash_StatusProtect)((SRP1 << 1) | (SRP0 << 0));
	
    switch (StatuProtect)
    {
    case SpiFlash_SoftwareProtection:
     	 ret=0;/*可以正常写，必须发写使能后*/
      	break;
    case SpiFlash_HardwareProtection:
      	ret=1;/*硬件保护控制，如果/WP 引脚为低电平，不能写*/
      	break;
    case SpiFlash_PowerSupplyLock_Down:
      	ret=2;/*直到下一个 power-down power-up时序后，才能被写*/
      	SpiFlash_Powerdown(QSPIC);
      	SpiFlash_ReleasePowerdown(QSPIC);
      	break;
    case SpiFlash_OneTimeProgram:
      	ret=3;/*一次性编程保护，不能再写*/
      	return ret;
    default:
      	ret=0XFF;
      	break;	
    }
	
    if(cmd != ENABLE)
    {
      	reg1 &= ~(0x7 << 2);//block protec disable
      	reg1 &= ~(0x1 << 5);//Top/Bottom Block protect disbale
      	reg1 &= ~(0x1 << 6);//Sector /SEC 
      	reg2 &= ~(0x1 << 6);//CMP =0
      	reg2 &= ~(0x7 << 3);//BP2,BP1,BP0
    }

    while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (0x1<<0)) && timeout--);
    SpiFlash_WrStatusReg(QSPICx,reg1,reg2);
    timeout = 0x7FFFFFFF;
	while((SpiFlash_ReadStatusReg(QSPICx,SpiFlash_CmdCode_ReadStatusReg1)\
	  & (0x1<<0)) && timeout--);
    return ret;
}

/**
  * @功能:SPI flash 复位使能 和复位释放 序列
  * @注意: tRES2 的值 使得延时30 us左右，一切时序参考W25Q64FV手册      
  * @参数:1.QSPICx spiflash控制器组，只能是QSPIC
  * @返回值：无
  */
void SpiFlash_Reset_Set(SPICInit_Typedef* QSPICx)
{
    volatile unsigned int tRES2 = 0x6f;
    SPIFlash_CommandVaild(QSPICx,DISABLE);
    SPIFlash_BusMode(QSPICx,Spi_BusMode_StdSpi);
    SPIFlash_SpiclkMode(QSPICx,Spi_ClkMode0);
    SPIFlash_FastReadIO(QSPICx,Fast_ReadIO_readoutput);
    SPIFlash_FastRead(QSPICx,DISABLE);
    SPIFlash_WrRdNums(QSPICx,0);
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_EnableReset,SpiFlash_CmdType_Powerdown,ENABLE);
    SPIFlash_Interrupt(QSPICx);
    SPIFlash_DMARequestSize(QSPICx,DMARequestSize_1word);
    SPIFlash_WrRdAddr(QSPICx,0x0);
    SPIFlash_CSdeselecteTime(QSPICx,0,0);
    SPIFlash_PowerDownTime(QSPICx,0,0);
    SPIFlash_CommandVaild(QSPICx,ENABLE);
    while(tRES2--);//delay about	30 US
    SPIFlash_CommandConfig(QSPICx,SpiFlash_CmdCode_Reset,SpiFlash_CmdType_Powerdown,ENABLE);
    SPIFlash_CommandVaild(QSPICx,ENABLE);
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
