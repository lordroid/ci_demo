/**
  ******************************************************************************
  * @文件    ci100x_spi.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的SPI模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"

typedef struct
{
    unsigned int  FIFOC;  /*FIFO清空使能*/ 
    unsigned int  DMAEN;  /*DMA传输使能*/
    unsigned int  TXIFL;  /*TX FIFO 阈值设定*/
    unsigned int  RXIFL;  /*RX FIFO 阈值设定*/
    unsigned int  CSS;    /*数据单元拼接选择*/
    unsigned int  SCR;    /*SPI比特率分频系数*/
    unsigned int  SPH;    /*SPI时钟相位选择信号*/
    unsigned int  SPO;    /*SPI时钟极性选择信号*/
    unsigned int  DSS;    /*数据帧格式选择信号*/
    unsigned int  SPHCTM;     /*SPH连续转换使能*/
    unsigned int  SOD;        /*SPI SLAVE输出使能*/
    unsigned int  MS;         /*SPI主从模式选择*/
    unsigned int  LBM;        /*loopback 测试模式*/
}SPI_InitTypeDef;

/*  FIFOC */
#define FIFO_CLEAN_DISABLE  (0 << 7)
#define FIO_CLEAN_ENABLE    (1 << 7)

/* DMAEN*/
#define FIFO_CPUAPB     (0 << 24)
#define FIFO_CPUAHB     (1 << 24)
#define FIFO_DMAAHB     (2 << 24)

/*TXIFL*/
#define TX_FIFO_LEVEL1      (1 << 20)
#define TX_FIFO_LEVEL2      (2 << 20)
#define TX_FIFO_LEVEL3      (3 << 20)
#define TX_FIFO_LEVEL4      (4 << 20)
#define TX_FIFO_LEVEL5      (5 << 20)
#define TX_FIFO_LEVEL6      (6 << 20)
#define TX_FIFO_LEVEL7      (7 << 20)
#define TX_FIFO_LEVEL8      (8 << 20)
#define TX_FIFO_LEVEL9      (9 << 20)
#define TX_FIFO_LEVEL10     (10 << 20)
#define TX_FIFO_LEVEL11     (11 << 20)
#define TX_FIFO_LEVEL12     (12 << 20)
#define TX_FIFO_LEVEL13     (13 << 20)
#define TX_FIFO_LEVEL14     (14 << 20)
#define TX_FIFO_LEVEL15     (15 << 20)

/*RXIFL*/
#define RX_FIFO_LEVEL1      (1 << 16)
#define RX_FIFO_LEVEL2      (2 << 16)
#define RX_FIFO_LEVEL3      (3 << 16)
#define RX_FIFO_LEVEL4      (4 << 16)
#define RX_FIFO_LEVEL5      (5 << 16)
#define RX_FIFO_LEVEL6      (6 << 16)
#define RX_FIFO_LEVEL7      (7 << 16)
#define RX_FIFO_LEVEL8      (8 << 16)
#define RX_FIFO_LEVEL9      (9 << 16)
#define RX_FIFO_LEVEL10     (10 << 16)
#define RX_FIFO_LEVEL11     (11 << 16)
#define RX_FIFO_LEVEL12     (12 << 16)
#define RX_FIFO_LEVEL13     (13 << 16)
#define RX_FIFO_LEVEL14     (14 << 16)
#define RX_FIFO_LEVEL15     (15 << 16)

/*CSS*/
#define FIFO_16BIT_JOINT    (0x1 << 26)
#define FIFO_24BIT_JOINT    (0x2 << 26)
#define FIFO_32BIT_JOINT    (0x3 << 26)

/* clock prescale register bit fields */
#define SPICPSR_CPSDVSR		0x4		/* 分频参数配置必须在2-254*/
/*SCR*/
#define SCR_MASK    0xff

/*SPH*/
#define SPI_FIRST_EAGE  (0 << 6);
#define SPI_SECOND_EAGE (1 << 6)

/*SPO*/
#define SPI_IDEL_HIGH   (1 << 5)
#define SPI_IDEL_LOW    (0 << 5)
/*DSS*/
#define SPI_DSS_4BIT    (3 << 0)
#define SPI_DSS_5BIT    (4 << 0)
#define SPI_DSS_6BIT    (5 << 0)
#define SPI_DSS_7BIT    (6 << 0)
#define SPI_DSS_8BIT    (7 << 0)
#define SPI_DSS_9BIT    (8 << 0)
#define SPI_DSS_10BIT    (9 << 0)
#define SPI_DSS_11BIT    (10 << 0)
#define SPI_DSS_12BIT    (11 << 0)
#define SPI_DSS_13BIT    (12 << 0)
#define SPI_DSS_14BIT    (13 << 0)
#define SPI_DSS_15BIT    (14 << 0)
#define SPI_DSS_16BIT    (15 << 0)
#define SPI_DSS_17BIT    (16 << 0)
#define SPI_DSS_18BIT    (17 << 0)
#define SPI_DSS_19BIT    (18 << 0)
#define SPI_DSS_20BIT    (19 << 0)
#define SPI_DSS_21BIT    (20 << 0)
#define SPI_DSS_22BIT    (21 << 0)
#define SPI_DSS_23BIT    (22 << 0)
#define SPI_DSS_24BIT    (23 << 0)
#define SPI_DSS_25BIT    (24 << 0)
#define SPI_DSS_26BIT    (25 << 0)
#define SPI_DSS_27BIT    (26 << 0)
#define SPI_DSS_28BIT    (27 << 0)
#define SPI_DSS_29BIT    (28 << 0)
#define SPI_DSS_30BIT    (29 << 0)
#define SPI_DSS_31BIT    (30 << 0)
#define SPI_DSS_32BIT    (31 << 0)

/*SPHCTM*/

#define SPI_CONTINUOUS_TX   (1 << 4)
#define SPI_NOCONTINUOUS_TX (0 << 4)
/*SOD*/
/*SOD*/
#define SPI_TXD_ENABLE  (0 << 3)
#define SPI_TXD_DISABLE (1 << 3)

/*MS*/
#define SPI_MASTER  (0 << 2)
#define SPI_SLAVE   (1 << 2)
/*LBM*/
#define SPI_LOOPBACK_ENABLE     (1 << 1)
#define SPI_LOOPBACK_DISABLE    (0 << 1)

#define SPI_DataSize_16b    15
#define SPI_DataSize_8b      7

/* status register bit fields */
#define SPI_BUSY			0x10		/* SPI 忙标志 */	
#define SPI_RXFIFO_FULL		0x08		/* 接收FIFO满 */
#define SPI_RXFIFO_NONEMPTY	0x04		/* 接收FIFO不满 */
#define SPI_TXFIFO_NONFULL		0x02		/* 发送FIFO不满 */
#define SPI_TXFIFO_EMPTY	0x01		/* 发送FIFO满 */

 
void SPI_Init(SPI_TypeDef* SPIx,SPI_InitTypeDef* SPI_InitStruct);
void SPI_Cmd(SPI_TypeDef* SPIx,int state);
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize);
void SPI_SendData(SPI_TypeDef* SPIx,uint16_t data);
uint32_t SPI_ReceiveData(SPI_TypeDef* SPIx);
void SPI_Transmit(SPI_TypeDef * SPIx,unsigned int *pWrbuf, unsigned int len,unsigned int flag);
void SPI_Receive(SPI_TypeDef* SPIx,unsigned int *pRdbuf, unsigned int len,unsigned int flag);

#ifdef __cplusplus
}
#endif
#endif  /*SPI_H END*/
