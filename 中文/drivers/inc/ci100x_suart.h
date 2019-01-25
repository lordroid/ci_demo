/**
  ******************************************************************************
  * @文件    ci100x_suart.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的SUART模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef _S_UART_H_
#define _S_UART_H_
#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"

typedef enum{ 
  B1200=1200,
  B2400=2400,
  B4800=4800,
  B9600=9600,
  B14400=14400,
  B19200=19200,
  B56000=56000,
  B38400=38400,
  B57600=57600,
  B115200=115200
}S_Uart_Baudrate;

#define BAUDRATE   B38400

typedef enum{
  SUART0,
  SUART1
}S_Uart_Device;

typedef enum{
  WORD_SIZE_5,
  WORD_SIZE_6,
  WORD_SIZE_7,
  WORD_SIZE_8
}S_Uart_WordSize;

typedef enum{
  ODD,
  EVEN,
  NONE
}S_Uart_Parity;

typedef enum{
	  LCR_STOP_BIT_1,
		LCR_STOP_BIT_2
}S_Uart_StopBit;


typedef enum{
    S_Uart_CTRL_C = 0,
    S_Uart_FrameError = 1,
    S_Uart_ParityError = 2
}SEnum_Uart_Error;






int S_Uart_Open( S_Uart_Device uartid,void (*handler)(char error));

int S_Uart_Close( S_Uart_Device uartid);

int S_Uart_ChangeBaudrate(S_Uart_Device uartid,  S_Uart_Baudrate baudrate);
 
int S_Uart_SetParity(S_Uart_Device uartid,  S_Uart_Parity parity);

int S_Uart_SetStopBit(S_Uart_Device uartid,  S_Uart_StopBit stopbit);

int S_Uart_SetWordSize(S_Uart_Device uartid,  S_Uart_WordSize wordsize);
 
int S_Uart_SetTXMode( S_Uart_Device uartid, int  bQuery);

int S_Uart_SetRXMode( S_Uart_Device uartid, int bQuery);

int S_Uart_GetChar(S_Uart_Device uartid,  unsigned char *ch);

int S_Uart_PutChar(S_Uart_Device uartid, unsigned char ch);

int S_Uart_Init( S_Uart_Device uartid);

void S_Uart_StartDMARxTx (void);

void S_UART_ClearRxBuffer(S_Uart_Device uartid);

int S_Uart_GetCharUnBlock(S_Uart_Device uartid, unsigned char *ch);

void S_UART1_int(void);
void S_UART0_int(void);
typedef struct
{
    unsigned char    *Buffer;
    unsigned int       BufferSize;
    unsigned int       ReadPtr;
    unsigned int       WritePtr;
} Struct_CircleBuffer, *PStruct_CircleBuffer;

void S_CircleBuffer_Init(PStruct_CircleBuffer pcirclebuffer,unsigned char * buffer,int buffersize);
void S_CircleBuffer_Clear( PStruct_CircleBuffer pcirclebuffer);
int S_CircleBuffer_IsEmpty(PStruct_CircleBuffer pcirclebuffer);
int S_CircleBuffer_IsFull(PStruct_CircleBuffer pcirclebuffer);
int S_CircleBuffer_Read(PStruct_CircleBuffer pcirclebuffer,unsigned char * ch);
int S_CircleBuffer_Write(PStruct_CircleBuffer pcirclebuffer,unsigned char ch);


/* UART registers addr definition */
#define S_UART_RBR       0x00    /* Receive Buffer Register (32 bits, R) */
#define S_UART_THR       0x00    /* Transmit Holding Register (32 bits, W) */
#define S_UART_DLL       0x00    /* Divisor Latch(Low)  (32 bits, R/W) */
#define S_UART_IER       0x01    /* Interrupt Enable Register (32 bits, R/W) */
#define S_UART_DLH       0x01    /* Divisor Latch(High) (32 bits, R/W) */
#define S_UART_IIR       0x02    /* Interrupt Identity Register (32 bits, R) */
#define S_UART_FCR       0x02    /* fifo Countrol Register (32 bits, W) */
#define S_UART_LCR       0x03    /* Line Control Register (32 bits, R/W) */
#define S_UART_MCR       0x04    /* Modem Control Register (32 bits, W) */
#define S_UART_LSR       0x05    /* Line Status Register (32 bits, R) */
#define S_UART_MSR       0x06    /* Modem Status Register (32 bits, R/W) */
#define S_UART_USR       0x1f    /* UART Status Register (32 bits, R/W) */


#define UART_BUSY_TIMEOUT      1000000
#define UART_RECEIVE_TIMEOUT   1000
#define UART_TRANSMIT_TIMEOUT  1000


/* UART register bit definitions */

#define USR_UART_BUSY           0x01
#define LSR_DATA_READY          0x01
#define LSR_THR_EMPTY           0x20
#define IER_RDA_INT_ENABLE      0x02
#define IER_THRE_INT_ENABLE     0x01
#define IIR_NO_ISQ_PEND         0x01

#define LCR_SET_DLAB            0x80       /* enable r/w DLR to set the baud rate */
#define LCR_PARITY_ENABLE	    0x08       /* parity enabled */
#define LCR_PARITY_EVEN         0x10   /* Even parity enabled */
#define LCR_PARITY_ODD          0xef   /* Odd parity enabled */
#define LCR_WORD_SIZE_5         0xfc   /* the data length is 5 bits */
#define LCR_WORD_SIZE_6         0x01   /* the data length is 6 bits */
#define LCR_WORD_SIZE_7         0x02   /* the data length is 7 bits */
#define LCR_WORD_SIZE_8         0x03   /* the data length is 8 bits */
#define LCR_STOP_BIT1           0xfb   /* 1 stop bit */
#define LCR_STOP_BIT2           0x04  /* 1.5 stop bit */

#define S_LSR_PFE              0x80     
#define S_LSR_TEMT             0x40
#define S_LSR_THRE             0x40
#define	S_LSR_BI               0x10
#define	S_LSR_FE               0x08
#define	S_LSR_PE               0x04
#define	S_LSR_OE               0x02
#define	S_LSR_DR               0x01
#define S_LSR_TRANS_EMPTY      0x20


/*config the uart */

#define S_UART_TXBUFFERSIZE 32
#define S_UART_RXBUFFERSIZE 32
typedef struct S_UART_Info_t 
{
    unsigned int id;
    volatile unsigned int* addr;
    unsigned int irq ;
    unsigned int priority;
    int bopened;
    void  (* handler)(char error);
    S_Uart_Baudrate baudrate;
    S_Uart_Parity parity;
    S_Uart_WordSize word;
    S_Uart_StopBit stop; 
    int btxquery; 
    int brxquery; 
    unsigned char txbuffer[S_UART_TXBUFFERSIZE];
    unsigned char rxbuffer[S_UART_RXBUFFERSIZE];
    Struct_CircleBuffer txcirclebuffer; 
    Struct_CircleBuffer rxcirclebuffer;
} Struct_UartInfo, *PStruct_UartInfo;


#ifdef __cplusplus
}
#endif
#endif
