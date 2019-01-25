/**
  ******************************************************************************
  * @文件    ci100x_iisdma.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的IIS_DMA模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __IISDMA_H
#define __IISDMA_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "misc.h"   
#include "smt.h"

//#ifndef IISDMA_BASE
#define IISDMA_BASE  0X40015000 
//#endif
typedef struct {
    volatile unsigned int IISxDMARADDR;
    volatile unsigned int IISxDMARNUM;
    volatile unsigned int IISxDMATADDR0;
    volatile unsigned int IISxDMATNUM0;
    volatile unsigned int IISxDMATADDR1;
    volatile unsigned int IISxDMATNUM1;
}IISDMAChanx_TypeDef;

typedef struct {
    volatile unsigned int IISDMACTRL;
    IISDMAChanx_TypeDef   IISxDMA[3];
    volatile unsigned int IISDMAPTR;
    volatile unsigned int IISDMASTATE;
    volatile unsigned int IISDMACLR;
    volatile unsigned int IISDMAIISCLR;
    volatile unsigned int IISDMARADDR[3];
}IISDMA_TypeDef;

#define IISDMA ((IISDMA_TypeDef*)IISDMA_BASE)

typedef enum{
		IISxDMA_TX_EN 	=1,
		IISxDMA_RX_EN	=0,
}IISDMA_TXRX_ENx;

typedef enum{
	IISDMA_Priority_IIS0_IIS1_IIS2 =0,
    IISDMA_Priority_IIS0_IIS2_IIS1 =1,
    IISDMA_Priority_IIS1_IIS0_IIS2 =2,
    IISDMA_Priority_IIS1_IIS2_IIS0 =3,
    IISDMA_Priority_IIS2_IIS0_IIS1 =4,
    IISDMA_Priority_IIS2_IIS1_IIS0 =5,
}IISDMA_Priorityx;

typedef enum{
		IIS0DMA=0,
		IIS1DMA=1,
		IIS2DMA=2,
}IISxDMA;

typedef enum{
		IISDMA_TXRXSINGALESIZE16bytes =1,
		IISDMA_TXRXSINGALESIZE32bytes =2,
		IISDMA_TXRXSINGALESIZE48bytes =3,
		IISDMA_TXRXSINGALESIZE64bytes =4,
		IISDMA_TXRXSINGALESIZE80bytes =5,
		IISDMA_TXRXSINGALESIZE96bytes =6,
		/*...*/
		IISDMA_TXRXSINGALESIZE128bytes =8,
		IISDMA_TXRXSINGALESIZE496bytes =31,
}IISDMA_TXRXSingaleSIZEx;

typedef enum{
	IISDMA_RXTX1RollbackADDR 	=0,
	IISDMA_RXTX2RollbackADDR	=1,
	IISDMA_RXTX3RollbackADDR	=3,
	/*...*/
	IISDMA_RXTX16RollbackADDR	=15,
    IISDMA_RXTX20RollbackADDR   = 19,
	IISDMA_RXTX32RollbackADDR	=31,
	IISDMA_RXTX64RollbackADDR   =63,
	IISDMA_RXTX256RollbackADDR  =255,
	IISDMA_RXTX512RollbackADDR  =511,
	IISDMA_RXTX1000RollbackADDR =999,
	IISDMA_RXTX1001RollbackADDR	=1000,
	IISDMA_RXTX1024RollbackADDR	=1023,
}IISDMA_RXTXxRollbackADDR;

typedef enum{
	IISDMA_RX1Interrupt 	=0,
	IISDMA_RX2Interrupt		=1,
	IISDMA_RX3Interrupt		=2,
 
	/*.........*/
	IISDMA_RX32Interrupt	=31,
}IISDMA_RXxInterrupt;

typedef enum{
		IIS2DMA_RXTurnInterrupt     =14,
		IIS1DMA_RXTurnInterrupt	    =13,
		IIS0DMA_RXTurnInterrupt	    =12,
		IIS0DMA_RXInterrupt			=0,
		IIS0DMA_TXInterrupt			=1,
		IIS1DMA_RXInterrupt			=2,
		IIS1DMA_TXInterrupt			=3,
		IIS2DMA_RXInterrupt			=4,
		IIS2DMA_TXInterrupt			=5,
}IISxDMA_RXTurnInterrupt;

typedef enum{
	IISDMA_TXAADRRollbackInterrupt_ADDR0=0,
	IISDMA_TXAADRRollbackInterrupt_ADDR1=1,
}IISDMA_TXADDRRollbackInterrupt;


int IISDMA_ChannelIntENConfig(IISxDMA iisxdma,IISDMA_TXRX_ENx iisdmaintbit,FunctionalState cmd);
int IISDMA_EN(FunctionalState cmd);
int IISxDMA_RADDR(IISxDMA iisxdma,unsigned int rxaddr);
int IISxDMA_RNUM(IISxDMA iisxdma,IISDMA_RXxInterrupt iisrxtointerrupt ,\
    IISDMA_RXTXxRollbackADDR rollbacktimes,IISDMA_TXRXSingaleSIZEx rxsinglesize);
int IISxDMA_TADDR0(IISxDMA iisxdma,unsigned int txaddr0);
int IISxDMA_TNUM0(IISxDMA iisxdma,IISDMA_RXTXxRollbackADDR rollbackaddr,\
	IISDMA_TXRXSingaleSIZEx txsinglesize);
int IISxDMA_TADDR1(IISxDMA iisxdma,unsigned int txaddr1);
int IISxDMA_TNUM1(IISxDMA iisxdma,IISDMA_RXTXxRollbackADDR rollbackaddr,\
	IISDMA_TXRXSingaleSIZEx txsinglesize);
int IISDMA_PriorityConfig(IISxDMA iisxdma,IISDMA_Priorityx iisdma_priority);
void IISDMA_INTClear(void);
char IISDMA_STATE(IISDMA_TXRX_ENx iisdmastat);
int IISDMA_RXCompleteClear(IISxDMA iisxdma);
int IISxDMA_TXADDRRollbackInterruptClear(IISxDMA iisxdma,\
	IISDMA_TXADDRRollbackInterrupt txrestart_addr);
int IISDMA_RXTXClear(IISxDMA iisxdma,IISDMA_TXRX_ENx iisdmarxtx);
unsigned int IISxDMA_RXADDR(IISxDMA iisxdma);
int IISDMA_ChannelENConfig(IISxDMA iisxdma,IISDMA_TXRX_ENx iisdmaintbit,FunctionalState cmd);
int IISDMA_ADDRRollBackINT(IISxDMA iisxdma,IISDMA_TXRX_ENx iisdmaintbit,FunctionalState cmd);
void IISDMA_DMICModeConfig(FunctionalState cmd);
int IISDMA_ADDRRollBackSTATE(IISxDMA iisxdma,IISDMA_TXRX_ENx iisdmaintbit);
void IISDMA_DMICModeConfig(FunctionalState cmd);
int IISDMA_DATABUSBUSY(IISxDMA iisxdma);

#ifdef __cplusplus
}
#endif 

#endif /*__IISDMA_H*/