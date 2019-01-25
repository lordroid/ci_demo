#include "ci100x_spiflash.h"
#include "ci100x_scu.h"
#include "ci100x_uart.h"
#include "ci100x_dma.h"
#include "smt_spiflash.h"
#include <string.h>
#include "user_config.h"


static unsigned int SPI_mode = -1;/*0：4 线模式；1：1线模式*/
unsigned char M7_0 =0;
/**
  * @功能:QSPI flash读取数据到sdram中
  * @注意:先调用   QSPI_Init()函数    
  * @参数:1.sdramaddr sdram的地址
  *       2.flashaddr flash的地址
  *       3.size 读取的字节数(4Kbytes位单位,不足4kbytes 按4kytes算)
  * @返回值:无
  */
void flashtosdram(unsigned int sdramaddr,unsigned int flashaddr, unsigned int size)
{
	if( (size> SPI_FLASH_SIZE) || (flashaddr + size> SPI_FLASH_SIZE) )
    {
        mprintf("\n [flash addr or size error ] \n");
        return ;
    }
    size = (size / (4 * 1024)) * 4 * 1024+((size % (4 * 1024))?(4 * 1024):0);
	if(0==SPI_mode)
    {
        SpiFlash_FastReadSectorQuadIO(QSPIC,flashaddr,(char *)(sdramaddr),\
            size / (4 * 1024));
    }else if(1==SPI_mode)
    {
        SpiFlash_ReadDataSector(QSPIC,flashaddr,(char*)sdramaddr,size/(4*1024));
    }else
    {
        mprintf("\n [flash mode error %d ] \n",SPI_mode);
    }
}

/**
  * @功能:sdram写数据到QSPI中
  * @注意:先调用   QSPI_Init()函数    
  * @参数:1.sdramaddr sdram的地址
  *       2.flashaddr flash的地址
  *       3.size 写入的字节数(最小单位256byte)
  * @返回值:无
  */
#define SPI_PAGE 256

void sdramtoflash(unsigned int flashaddr,unsigned int sdramaddr,unsigned int size)
{ 
	if( (size> SPI_FLASH_SIZE) || (flashaddr + size> SPI_FLASH_SIZE) )
    {
        mprintf("\n [flash addr or size error ] \n");
        return ;
    }
    size = (size / (SPI_PAGE)) *SPI_PAGE+((size % (SPI_PAGE))?(SPI_PAGE):0);
	if(0==SPI_mode)
    {
        SpiFlash_QuadInputSequencePageProgram(QSPIC,(char *)(sdramaddr),flashaddr,size/SPI_PAGE);
    }else if(1==SPI_mode)
    {
        SpiFlash_SequencePageProgram(QSPIC,(char *)(sdramaddr),flashaddr,size/SPI_PAGE);
    }else
    {
        mprintf("\n [flash mode error %d ] \n",SPI_mode);
    }
}

/**
  * @功能:QSPI 初始化
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void QSPI_Init(void)
{
    unsigned char manid,deviceid;
    unsigned char jeceid[4] = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
	
	NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
	Scu_SetDeviceGate(DMA_CTRL_BASE,ENABLE);
    Scu_Setdevice_Reset(SPI0_BASE);
    Scu_Setdevice_ResetRelease(SPI0_BASE);
	
    Scu_SetDeviceGate(SPI0_BASE,ENABLE);
	
	QSPINoBootSet();
    SpiFlash_Powerdown(QSPIC);

    SpiFlash_ReleasePowerdown(QSPIC);

    SpiFlash_Reset_Set(QSPIC);

    SpiFlash_Protect(QSPIC,DISABLE);
	
    SpiFlash_ReadJedecID(QSPIC,jeceid);
    mprintf("jeceid =%x %x %x\n",jeceid[0],jeceid[1],jeceid[2]);
    SpiFlash_ReadManfacturerIDDeviceID(QSPIC,&manid,&deviceid);	
    M7_0 = jeceid[0];
    switch(M7_0)
    {
        case 0xEF:/*winbond*/
        case 0x1C:/*eon*/
          SPI_mode = 0;/*4 线模式*/
          mprintf("[QSPI]\n");
          break; 
        case 0xC2:/*MXIC:2线*/
        case 0xC8:/*GigaDevice:4线*/
        case 0xBF:/*SST25VF064C: 2线*/
        case 0x01:/*SPANSION: 4线*/
          SPI_mode = 1;/*1 线模式*/
          mprintf("[STDSPI]\n");
          break; 
        default:
        SPI_mode =1;/* 1线模式*/
        mprintf("[STDSPI]\n");
    }
}
