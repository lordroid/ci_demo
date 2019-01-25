/**
  ******************************************************************************
  * @文件    ci100x_spi.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的SPI模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_spi.h"

/**
  * @目标:初始化SPI
  * @注意:无          
  * @参数: SPIx : 配置SPI基址SPI_InitStruct: SPI初始化配置参数
  * @返回值:无
  */
void SPI_Init(SPI_TypeDef* SPIx,SPI_InitTypeDef* SPI_InitStruct)
{
    SPIx->SPICPSR = SPICPSR_CPSDVSR;
    SPIx->SPIIER = 0;
    SPIx->SPIICR = 3;
    SPIx->SPICR0 = SPI_InitStruct->CSS | SPI_InitStruct->DMAEN | SPI_InitStruct->TXIFL | \
                               SPI_InitStruct->DSS | SPI_InitStruct->RXIFL | SPI_InitStruct->FIFOC | \
                               SPI_InitStruct->SPH | SPI_InitStruct->SPO | SPI_InitStruct->SCR;
    SPIx->SPICR1 = SPI_InitStruct->SPHCTM | SPI_InitStruct->SOD | SPI_InitStruct->MS |\
                               SPI_InitStruct->LBM;
    
}
/**
  * @目标:使能或禁用对应的SPI
  * @注意:无          
  * @参数: SPIx : 配置SPI基址state: 0 禁用 1使能
  * @返回值:无
  */
void SPI_Cmd(SPI_TypeDef* SPIx,int state)
{
    if(state == 0)
    {
        SPIx->SPICR1 &= ~(0x1 << 0);
    }
    else
    {
        SPIx->SPICR1 |= (0x1 << 0);
    }
}
/**
  * @目标:配置SPI传输数据位宽
  * @注意:无          
  * @参数: SPIx : 配置SPI基址SPI_DataSize: 0 禁用 1使能
  * @返回值:无
  */
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
    SPIx->SPICR0 &= ~(0x1f << 0);
    SPIx->SPICR0 |= (SPI_DataSize << 0);
}
/**
  * @目标:配置SPI 发送单笔数据
  * @注意:无          
  * @参数: SPIx : 配置SPI基址data: 发送的数据
  * @返回值:无
  */
void SPI_SendData(SPI_TypeDef* SPIx,uint16_t data)
{
    while(!(SPIx->SPISR & SPI_TXFIFO_NONFULL));
    SPIx->SPIDR = data;
}
/**
  * @目标:配置SPI 接收单笔数据
  * @注意:无          
  * @参数: SPIx : 配置SPI基址
  * @返回值:接收到的数据
  */
uint32_t SPI_ReceiveData(SPI_TypeDef* SPIx)
{
    SPI_SendData(SPIx,0xa5);
    return SPIx->SPIDR;
}

/**
  * @目标:配置SPI 发送多笔数据
  * @注意:
  * @参数: SPIx : 配置SPI基址pWrbuf: 发送的数据缓存len:数据长度flag:数据发送接口(通过APB还是AHB发送)
  * @返回值:无
  */
void SPI_Transmit(SPI_TypeDef * SPIx,unsigned int *pWrbuf, unsigned int len,unsigned int flag)
{
    unsigned int * spi_sendbase;
    unsigned int i;
    if(flag == FIFO_CPUAPB)
    {
        spi_sendbase = (unsigned int *)(&(SPIx->SPIDR));
    }
    else
    {
        spi_sendbase = (unsigned int *)0x61000000;
    }
    for(i = 0; i < len; i++)
    {
        while(!(SPIx->SPISR & SPI_TXFIFO_NONFULL));
        *spi_sendbase = *(pWrbuf + i);
    }
    return ;
}
/**
  * @目标:配置SPI接收 多笔数据
  * @注意: 接收数据前必须先调用发送函数，以产生SPI时钟
  * @参数: SPIx : 配置SPI基址pRdbuf: 接收的数据缓存len:数据长度flag:数据发送接口(通过APB还是AHB发送)
  * @返回值:无
  */
void SPI_Receive(SPI_TypeDef* SPIx,unsigned int *pRdbuf, unsigned int len,unsigned int flag)
{
    unsigned int *spi_receivebase;
    unsigned int i;
    if(flag == FIFO_CPUAPB)
    {
        spi_receivebase = (unsigned int *)(&(SPIx->SPIDR));
    }
    else
    {
        spi_receivebase = (unsigned int *)0x61000000;
    }
    for(i = 0; i < len; i++)
    {
        while(!(SPIx->SPISR & SPI_RXFIFO_NONEMPTY));
        *(pRdbuf + i) = *spi_receivebase;
    }
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/