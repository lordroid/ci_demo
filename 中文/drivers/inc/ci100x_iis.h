/**
  ******************************************************************************
  * @文件    ci100x_iis.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的IIS模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */

#ifndef __IIS_H
#define __IIS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "misc.h"
#include "ci100x_scu.h"
#include "ci100x_iisdma.h" 

#ifndef IIS0_BASE
#define IIS0_BASE (unsigned int )0x4004C000
#endif
#ifndef  IIS1_BASE
#define IIS1_BASE (unsigned int )0x4004D000
#endif
#ifndef IIS2_BASE
#define IIS2_BASE (unsigned int )0x4004E000
#endif
typedef struct{
    volatile unsigned int IISTXCTRL;
    volatile unsigned int IISRXCTRL;
    volatile unsigned int IISGBCTRL;
    volatile unsigned int IISCHSEL;
}IIS_TypeDef;   


#define IIS0 ((IIS_TypeDef* )IIS0_BASE)
#define IIS1 ((IIS_TypeDef* )IIS1_BASE)
#define IIS2 ((IIS_TypeDef* )IIS2_BASE)

typedef enum{
		IIS_TXRXDATAWIDE16BIT	=0,
		IIS_TXRXDATAWIDE24BIT	=1,
		IIS_TXRXDATAWIDE32BIT	=2,
		IIS_TXRXDATAWIDE20BIT	=3,
		IIS_TXRXDATAWIDE8BIT	=4,
}IIS_TXRXDATAWIDEx;

typedef enum{
	IIS_TXFIFOLEVEL1_2=0,
	IIS_TXFIFOLEVEL1_4=1,
}IIS_TXFIFOLEVELx;

typedef enum{
	 IIS_TXCHNUM_STEREO	=0,
	 IIS_TXCHNUM_FOURE 	=1,
	 IIS_TXCHNUM_SIX	=2, 
}IIS_TXCHNUMx;

typedef enum{
	 IIS_RXFIFOLEVEL1_4 =0,
	 IIS_RXFIFOLEVEL1_8	=1,
	 IIS_RXFIFOLEVEL1_16=2,
	 IIS_RXFIFOLEVEL1_32=3,
}IIS_RXFIFOLEVELx; 


typedef enum{
		IIS_BUSSCK_LRCK32=0,
		IIS_BUSSCK_LRCK64=1,
}IIS_BUSSCK_LRCKx;


typedef enum{
		IIS_RXDATAFMT_IIS				=0,
		IIS_RXDATAFMT_LEFT_JUSTIFIED	=1,
		IIS_RXDATAFMT_RIGHT_JUSTIFIED	=2,
		
		IIS_TXDATAFMT_IIS				=3,
		IIS_TXDATAFMT_LEFT_JUSTIFIED	=4,
		IIS_TXDATAFMT_RIGHT_JUSTIFIED	=5,
}IIS_TXRXDATAFMTx;

typedef enum{
		IIS_AUDIO_INPUT 	=0,
		IIS_AUDIO_OUTPUT 	=1,
}IIS_CHSELAUDIOINOUTx;

typedef enum{
		IIS_MERGE_16BITTO32BIT		=1,
		IIS_MERGE_NONE				=0,
}IIS_MERGEx;

typedef enum{
		IIS_LR_LEFT_HIGH_RIGHT_LOW=0,
		IIS_LR_RIGHT_HIGH_LEFT_LOW=1,
}IIS_LR_SELx;

typedef enum{
		IIS_RXMODE_MONO		=1,
		IIS_RXMODE_STEREO	=0,
}IIS_RXMODEx;

typedef enum{
    IIS_OverSample128Fs=0,
    IIS_OverSample192Fs=1,
    IIS_OverSample256Fs=2,
    IIS_OverSample384Fs=3,
}IIS_OverSample;

typedef struct{
    unsigned int txaddr0;
    unsigned int txaddr1;
    IISDMA_RXTXxRollbackADDR rollbackaddr0size;
    IISDMA_RXTXxRollbackADDR rollbackaddr1size;
    IISDMA_TXRXSingaleSIZEx tx0singlesize;
    IISDMA_TXRXSingaleSIZEx tx1singlesize;
    IIS_TXRXDATAWIDEx txdatabitwide;
    IIS_BUSSCK_LRCKx sck_lrck;
    IIS_TXRXDATAFMTx txdatafmt;
    IIS_OverSample oversample;
}IIS_DMA_TXInit_Typedef;
    
typedef struct{
    unsigned int rxaddr;
    IISDMA_RXxInterrupt rxinterruptsize;
    IISDMA_RXTXxRollbackADDR rollbackaddrsize;
    IISDMA_TXRXSingaleSIZEx rxsinglesize;
    IIS_TXRXDATAWIDEx rxdatabitwide;
    IIS_BUSSCK_LRCKx sck_lrck;
    IIS_TXRXDATAFMTx rxdatafmt;
    IIS_OverSample oversample;
}IIS_DMA_RXInit_Typedef;



void IISx_RXInit(IIS_TypeDef* IISx,IIS_DMA_RXInit_Typedef* IIS_DMA_Init_Struct);
void IISx_TXInit(IIS_TypeDef* IISx,IIS_DMA_TXInit_Typedef* IIS_DMA_Init_Struct);
int IIS_RXMODEConfig(IIS_TypeDef* IISx,IIS_MERGEx merge,\
		IIS_LR_SELx lr_sel,IIS_RXMODEx iisrxmode);
int IIS_TXEN(IIS_TypeDef* IISx,FunctionalState cmd);
int IIS_RXEN(IIS_TypeDef* IISx,FunctionalState cmd);
int IIS_EN(IIS_TypeDef* IISx,FunctionalState cmd);
int IIS_CHSELConfig(IIS_TypeDef* IISx,IIS_CHSELAUDIOINOUTx iischselinout\
	,FunctionalState cmd);
void i2s0_pcmtopcmram(void);
#ifdef __cplusplus
}
#endif 

#endif /*__IIS_H*/