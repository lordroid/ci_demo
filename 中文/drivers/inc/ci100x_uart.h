/**
  ******************************************************************************
  * @文件    ci100x_uart.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的通用异步串行口收发控制模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */

#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "smt.h"
  
  
#define UART_CLK 25000000

#ifndef UART0_BASE
#define UART0_BASE (unsigned int)0X40043000
#endif
#ifndef UART1_BASE
#define UART1_BASE (unsigned int)0x40044000
#endif
 
#define UART0_DMA_ADDR 0X63000000
#define UART1_DMA_ADDR 0X64000000

   
typedef struct {
    volatile unsigned int UARTRdDR;
    volatile unsigned int UARTWrDR;
    volatile unsigned int UARTRxErrStat;
    volatile unsigned int UARTFlag;
    volatile unsigned int UARTIBrd;
    volatile unsigned int UARTFBrd;
    volatile unsigned int UARTLCR;
    volatile unsigned int UARTCR;
    volatile unsigned int UARTFIFOlevel;
    volatile unsigned int UARTMaskInt;
    volatile unsigned int UARTRIS;
    volatile unsigned int UARTMIS;
    volatile unsigned int UARTICR;
    volatile unsigned int UARTDMACR;
    volatile unsigned int UARTTimeOut;
    volatile unsigned int REMCR;
    volatile unsigned int REMTXDATA;
    volatile unsigned int REMRXDATA;
    volatile unsigned int REMINTCLR;
    volatile unsigned int REMINTSTAE;
}UART_TypeDef;

#define UART0 ((UART_TypeDef* )UART0_BASE)
#define UART1 ((UART_TypeDef* )UART1_BASE)

typedef enum{
	UART_FramingError	=0,
	UART_ParityError	=1,
	UART_BreakError		=2,
	UART_OverrunError	=3,
}UART_ERRORFLAG;

typedef enum{
	UART_CTS	=0,
	UART_DSR	=1,
	UART_DCD	=2,
	UART_RI		=3,
	UART_BUSY	=4,
	UART_RXFF	=5,
	UART_RXFE	=6,
	UART_TXFF	=7,
	UART_TXFE	=8,
	UART_EOC	=9,
	UART_EDF	=10,	
}UART_FLAGStatus;

typedef enum{
    UART_BaudRate2400		=2400,
    UART_BaudRate4800		=4800,
	UART_BaudRate9600		=9600,
	UART_BaudRate19200		=19200,
	UART_BaudRate38400		=38400,
	UART_BaudRate57600		=57600,
	UART_BaudRate115200		=115200,	
	UART_BaudRate230400		=230400,
	UART_BaudRate380400		=380400,
	UART_BaudRate460800		=460800,
	UART_BaudRate921600		=921600,
}UART_BaudRate;

typedef enum{
	UART_WordLength_5b	=0,
	UART_WordLength_6b	=1,
	UART_WordLength_7b	=2,
	UART_WordLength_8b	=3,
}UART_WordLength;

typedef enum{
	UART_StopBits_1		=0,
	UART_StopBits_1_5	=1,
	UART_StopBits_2		=2,
}UART_StopBits;

typedef enum{
	UART_Parity_No 		=0,
	UART_Parity_Even	=1,
	UART_Parity_Odd		=2,
}UART_Parity;

typedef enum{
 	UART_Byte	=1,
	UART_Word	=0,
}UART_ByteWord;

typedef enum{
	UART_CTSHardwareFlowCtrl	=15,
	UART_RTSHardwareFlowCtrl	=14,
	UART_Out2_RI				=13,
	UART_Out1_DCD				=12,
	UART_RTS					=11,
	UART_DTR					=10,
	UART_RXE					=9,
	UART_TXE					=8,
	UART_LoopBackE				=7,
	UART_NCED					=1,
}UART_CRBitCtrl;

typedef enum{
	UART_FIFOLevel1_8	=0,
	UART_FIFOLevel1_4	=1,
	UART_FIFOLevel1_2	=2,
	UART_FIFOLevel3_4	=3,
  	UART_FIFOLevel7_8	=4,
	UART_FIFOLevel1		=5,
	UART_FIFOLevel2		=6,
}UART_FIFOLevel;

typedef enum{
	UART_ErrorDataInt		=11,
	UART_OverrunErrorInt	=10,
	UART_BreakErrorInt		=9,
	UART_ParityErrorInt		=8,
	UART_FramingErrorInt	=7,
	UART_RXTimeoutInt		=6,
	UART_TXInt				=5,
	UART_RXInt				=4,
	UART_DSRModemInt		=3,
	UART_DCDModemInt		=2,
	UART_CTSModemInt		=1,
	UART_RIModemInt			=0,
	UART_AllInt				=12,
}UART_IntMask;	
	
typedef enum{
	UART_RXDMA		=0,
	UART_TXDMA		=1,
}UART_TXRXDMA;


void UartPollingSenddata(UART_TypeDef* UARTx, char ch);
void UARTPollingConfig(UART_TypeDef* UARTx);
void UART_IntMaskConfig(UART_TypeDef* UARTx,UART_IntMask intmask,FunctionalState cmd);
void UARTInterruptConfig(UART_TypeDef* UARTx);
void UARTDMAConfig(UART_TypeDef* UARTx);
int UART_MaskIntState(UART_TypeDef* UARTx,UART_IntMask intmask);
void UART_IntClear(UART_TypeDef* UARTx,UART_IntMask intmask); 
unsigned char UART_RXDATA(UART_TypeDef* UARTx);
int UART_ERRORSTATE(UART_TypeDef* UARTx,UART_ERRORFLAG uarterrorflag);
void UART_TXDATAConfig(UART_TypeDef* UARTx,unsigned int val);
int UART_FLAGSTAT(UART_TypeDef* UARTx,UART_FLAGStatus uartflag);
int UART_BAUDRATEConfig(UART_TypeDef* UARTx,UART_BaudRate uartbaudrate);
void UART_FIFOClear(UART_TypeDef* UARTx);
int UART_LCRConfig(UART_TypeDef* UARTx,UART_WordLength wordlength,\
  		UART_StopBits uartstopbits,UART_Parity uartparity);
int UART_TXFIFOByteWordConfig(UART_TypeDef* UARTx,UART_ByteWord uarttxfifobit);
void UART_EN(UART_TypeDef* UARTx,FunctionalState cmd);
void UART_CRConfig(UART_TypeDef* UARTx,UART_CRBitCtrl crbitctrl,FunctionalState cmd);
void UART_RXFIFOConfig(UART_TypeDef* UARTx,UART_FIFOLevel fifoleve);
void UART_TXFIFOConfig(UART_TypeDef* UARTx,UART_FIFOLevel fifoleve);
void UART_IntMaskConfig(UART_TypeDef* UARTx,UART_IntMask intmask,FunctionalState cmd);
int UART_RawIntState(UART_TypeDef* UARTx,UART_IntMask intmask);
int UART_MaskIntState(UART_TypeDef* UARTx,UART_IntMask intmask);
void UART_IntClear(UART_TypeDef* UARTx,UART_IntMask intmask);
void UART_TXRXDMAConfig(UART_TypeDef* UARTx,UART_TXRXDMA uartdma);
void UART_TimeoutConfig(UART_TypeDef* UARTx,unsigned short time);
void UARTPollingConfig(UART_TypeDef* UARTx);
int _printf(char* fmt,...);
int _puts(char* str);

#define SMT_PRINTF


#if defined(SMT_PRINTF)
	#define mprintf(fmt,...) _printf(fmt,##__VA_ARGS__)
#else
	#define mprintf(fmt,...) 
#endif

#ifdef __cplusplus
}
#endif

#endif /*__UART_H*/