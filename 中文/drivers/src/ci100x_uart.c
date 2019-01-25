/**
  ******************************************************************************
  * @文件    ci100x_uart.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.1
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的UART模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_uart.h"
#include "ci100x_scu.h"
#include "misc.h"
volatile char UartInterruptReceiveDataBuf[32] = {0};
/*
*功能：UART接收寄存器接收的数据
*参数：1.UARTx，UART组，UART0,UART1
*返回：UART接收的数据
*/
unsigned char UART_RXDATA(UART_TypeDef* UARTx)
{
    return UARTx->UARTRdDR & 0xFF;
}

/*
*功能：UART错误标志状态
*参数：1.UARTx，UART组，UART0,UART1
*	   2.uarterrorflag 错误标志，
*			UART_FramingError 帧错误，
*			UART_ParityError，奇偶校验错误，
*          		 UART_BreakError break 错误
*			UART_OverrunError 运行超时错误
*返回：0，无错误标志，非0，有对应的错误标志
*/
int UART_ERRORSTATE(UART_TypeDef* UARTx,UART_ERRORFLAG uarterrorflag)
{
    return UARTx->UARTRdDR & (1 << (uarterrorflag + 8));
}

/*
*功能：UART发送数据寄存器配置
*参数：1.UARTx，UART组，UART0,UART1
*	  	2.val，要写入发送寄存器的值 
*返回：无
***/
void UART_TXDATAConfig(UART_TypeDef* UARTx,unsigned int val)
{
    UARTx->UARTWrDR = val;
}

/*
*功能：UART 标志的状态
*参数：1.UARTx，UART组，UART0,UART1
*	   2.uartflag，UART状态标志
*返回：0，对应的状态标志位为0，非0，对应的状态标志位为1
*/
int UART_FLAGSTAT(UART_TypeDef* UARTx,UART_FLAGStatus uartflag)
{
    return UARTx->UARTFlag & (1 << uartflag);
}

/*
*功能：UART的波特率配置
*参数：1.UARTx，UART组，UART0,UART1
*	   2.uartbaudrate:波特率
*返回：RETRUN_OK、PARA_ERROR（正确/参数错误）
****/
int UART_BAUDRATEConfig(UART_TypeDef* UARTx,UART_BaudRate uartbaudrate)
{
    unsigned int tmp ;
    UARTx->UARTIBrd = (UART_CLK / (16 * uartbaudrate));
    tmp = (UART_CLK % (16 * uartbaudrate)) * 8 / uartbaudrate;
    UARTx->UARTFBrd = (tmp >> 1) + (tmp & 0x1); 
    return RETRUN_OK;
}

/*
*功能：UART FIFO 清除
*参数：1.UARTx，UART组，UART0,UART1
*返回：无
*/
void UART_FIFOClear(UART_TypeDef* UARTx)
{
    UARTx->UARTLCR |= (1 << 4);
}

/*
*功能：配置UART的数据位、停止位、奇偶校验设置
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.wordlength :每帧有效数据个数，UART_WordLength_5b，...,UART_WordLength_8b,
*	   3.uartstopbits:停止位，UART_StopBits_1，UART_StopBits_1_5，UART_StopBits_2,
*	   4.uartparity :奇偶校验选择
*返回：RETRUN_OK、PARA_ERROR（正确/参数错误）
*/
int UART_LCRConfig(UART_TypeDef* UARTx,UART_WordLength wordlength,\
  		UART_StopBits uartstopbits,UART_Parity uartparity)
{
    UARTx->UARTLCR &= ~(0x7F << 0);
    UARTx->UARTLCR |= (wordlength << 5) | \
	  (uartstopbits << 2) | (uartparity << 1);
    if(uartparity != UART_Parity_No)
    {
      UARTx->UARTLCR |= (1 << 0);
    }
    return RETRUN_OK;
}

/*
*功能：设置UART的发送FIFO数据位宽
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.uarttxfifobit:UART_Byte/UART_Word ,以byte/word方式向TXFIFO发送数据	
*返回：RETRUN_OK、PARA_ERROR（正确/参数错误）
*/
int UART_TXFIFOByteWordConfig(UART_TypeDef* UARTx,UART_ByteWord uarttxfifobit)
{
    if(UART_Word == uarttxfifobit)
    {
      UARTx->UARTLCR &= ~(1 << 8);
    }
    else
    {
      UARTx->UARTLCR |= (1 << 8);
    }
    return RETRUN_OK;
}

/*
*功能：UART 使能控制
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.cmd ENABLE ,使能；DISABLE,禁止
*返回：无
*/
void UART_EN(UART_TypeDef* UARTx,FunctionalState cmd)
{
    if(cmd != ENABLE)
    {
      UARTx->UARTCR &= ~(1 << 0);
    }
    else
    {
      UARTx->UARTCR |= (1 << 0);
    }
}

/*
*功能：UART 信号使能控制
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.crbitctrl,控制信号选择
*	   3.cmd ENABLE ,使能；DISABLE,禁止
*返回：无
*/
void UART_CRConfig(UART_TypeDef* UARTx,UART_CRBitCtrl crbitctrl,FunctionalState cmd)
{
    if(cmd != ENABLE)
    {
      UARTx->UARTCR &= ~(1 << crbitctrl);
    }
    else
    {
      UARTx->UARTCR |= (1 << crbitctrl);
    }
}

/*
*功能：UART 接收FIFO 触发深度选择
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.fifoleve FIFO深度选择 
*返回：无
*/
void UART_RXFIFOConfig(UART_TypeDef* UARTx,UART_FIFOLevel fifoleve)
{
    UARTx->UARTFIFOlevel &= ~(0x7 << 3);
    UARTx->UARTFIFOlevel |= (fifoleve << 3);
}

/*
*功能：UART 发送FIFO 触发深度选择
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.fifoleve FIFO深度选择 
*返回：无
*/
void UART_TXFIFOConfig(UART_TypeDef* UARTx,UART_FIFOLevel fifoleve)
{
    UARTx->UARTFIFOlevel &= ~(0x7 << 0);
    UARTx->UARTFIFOlevel |= (fifoleve << 0);
}
				   
/*
*功能：UART 中断屏蔽设置
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.intmask:选择要屏蔽的中断控制位
*	   3.cmd ENABLE ,屏蔽；DISABLE,不屏蔽
*返回：无
*/
void UART_IntMaskConfig(UART_TypeDef* UARTx,UART_IntMask intmask,FunctionalState cmd)
{
    if(cmd != ENABLE)
    {
      UARTx->UARTMaskInt &= ~(1 << intmask);
    }
    else
    {
      UARTx->UARTMaskInt |= (1 << intmask);
    }
    if(UART_AllInt == intmask)
    {
      UARTx->UARTMaskInt = (cmd != ENABLE)?0:0xFFF;
    }
}

/*
*功能：UART 原始中断(中断屏蔽前)状态
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.intmask:选择要查询中断状态控制位
*返回：0 无原始中断标志，非0 有原始中断标志
*/
int UART_RawIntState(UART_TypeDef* UARTx,UART_IntMask intmask)
{
    return UARTx->UARTRIS & (1 << intmask);
}

/*
*功能：UART 屏蔽后的中断状态
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.intmask:选择要查询中断状态控制位
*返回：0 无中断标志，非0 有中断标志
*/
int UART_MaskIntState(UART_TypeDef* UARTx,UART_IntMask intmask)
{
    return UARTx->UARTMIS & (1 << intmask);
}

/*
*功能：UART 中断标志清除
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.intmask:选择要清除中断状态控制位
*返回：无
*/
void UART_IntClear(UART_TypeDef* UARTx,UART_IntMask intmask)
{
    if(UART_AllInt != intmask)
    {
      	UARTx->UARTICR |= (1 << intmask);
    }
    else
    {
      	UARTx->UARTICR = 0xFFF;
    }
}

/*
*功能：UART DMA 发送/接收控制使能
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.uartdma,UART_RXDMA,接收dma使能;UART_TXDMA,发送dma使能
*返回：无
*/
void UART_TXRXDMAConfig(UART_TypeDef* UARTx,UART_TXRXDMA uartdma)
{
    UARTx->UARTDMACR |= (1 << uartdma);
}

/*
*功能：UART 超时设置
*参数：1.UARTx，UART组，UART0,UART1,
*	   2.time,超时大小
*返回：无
*/
void UART_TimeoutConfig(UART_TypeDef* UARTx,unsigned short time)
{
    UARTx->UARTTimeOut = time & 0x3FF;
}
  
/**
  * @功能:UART 查询方式发送一个字节数据
  * @注意:无        
  * @参数: 1.UARTx UART组，UART0,UART1,
  *        2.ch 发送的字符 
  * @返回值:无
  */
void UartPollingSenddata(UART_TypeDef* UARTx, char ch)
{
    while(UARTx->UARTFlag & (0x1 << 7));
    UARTx->UARTWrDR = (unsigned int)ch;
}
/**
  * @功能:UART 查询方式接收一个字节数据
  * @注意:无        
  * @参数: 1.UARTx UART组，UART0,UART1,
  * @返回值:接收到的字符
  */
char  UartPollingReceiveData(UART_TypeDef* UARTx)
{
    while(UARTx->UARTFlag & (0x1 << 6));
    return UARTx->UARTRdDR & 0xff;
}

#include <stdarg.h>
#include <user_config.h>
#if USER_UART_USE_UART1
    #define USE_UART0_OUTPUT 
#else
    #define USE_UART1_OUTPUT     
#endif
                  
void _putchar(char ch)
{
#ifdef USE_UART0_OUTPUT
    if('\n'==ch){UartPollingSenddata(UART0,'\r');}
    UartPollingSenddata(UART0,ch);
#elif defined(USE_UART1_OUTPUT)
    if('\n'==ch){UartPollingSenddata(UART1,'\r');}
    UartPollingSenddata(UART1,ch); 
#elif defined(USE_STD_OUTPUT)
#include <stdio.h>
    putchar(ch);
#else
    return ;
#endif
}


/**
  * @功能:字符串打印
  * @注意:此函数一次性限制最多打印512字符       
  * @参数: 1.str字符串的首地址
  * @返回值:打印的字符个数
  */
int _puts(char* str)
{
	int counter=0;
	if(!str)
	{
		return 0;
	}
	while(*str && (counter < 512))
	{
		_putchar(*str++);
		counter++;
	}
	return counter;
}

#if defined(SMT_PRINTF)
#include <stdio.h>
/**
  * @功能:调试、格式化打印
  * @注意:此函数一次性限制最多打印250字符,不建议打印浮点数。       
  * @参数: 1.format,打印的格式条件
  * @返回值:打印的字符个数
  */
char printk_string[250];
int _printf(char* format,...)
{
	va_list args;
    int plen;
    va_start(args, format);
    plen    = vsnprintf(printk_string, sizeof(printk_string)/sizeof(char) - 1, format, args);
    _puts(printk_string);
    va_end(args);
    return plen;
}
#else
int _printf(char* fmt,...)
{
    return 0;
}
#endif

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/