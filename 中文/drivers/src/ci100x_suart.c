/**
  ******************************************************************************
  * @文件    ci100x_suart.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.1
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的SUART模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_suart.h"
#include "freertos.h"
#include "ci100x_uart.h"


unsigned int UART3_REIEIVE = 0;
#define UARTID_MAX   sizeof(S_Uart_Table) / sizeof(Struct_UartInfo) 
static void S_Uart_Interrupt(unsigned int irqid); 
#define APB_DEFAULT_FREQ 50000000       //pll=320MHz,APB_DEFAULT_FREQ = pll/8

/* UART端口配置列表 */
Struct_UartInfo S_Uart_Table[] =
{
    { 0, (volatile unsigned int *)(UART2_BASE),37, 0, FALSE, NULL },
    { 1, (volatile unsigned int *)(UART3_BASE),38, 0, FALSE, NULL },
};

/**
  * @功能:延迟时长
  * @注意:无        
  * @参数:sec 配置需要延迟的时间按，单位毫秒
  * @返回值:无
  */
static void mydelay(int sec)
{
    int i;
    volatile int j;
    
    for(i = 0x00; i < sec * 100; i++)
    {
      j = i;
    }
}
/**
  * @功能:SUART模块禁用
  * @注意:无        
  * @参数:无
  * @返回值:无
  */
void S_Deactive_UartModule(void)
{
    int i;
    Struct_UartInfo *info;
   
    for( i = 0; i < UARTID_MAX; i++)
    {	   
        info = &(S_Uart_Table[i]);
        info->addr[S_UART_LCR] = 0x83;
        info->addr[S_UART_DLL] = 0x1;
        info->addr[S_UART_DLH] = 0x0;
    }
}

/**
  * @功能:初始化SUART
  * @注意:波特率为19200 8位数据宽度 无奇偶校验 1个停止位 查询模式     
  * @参数:uartid为SUART序号，0 和 1
  * @返回值:SUCCESS 配置成功
  */
int S_Uart_Init( S_Uart_Device uartid)
{
    //S_Uart_ChangeBaudrate(uartid,BAUDRATE);
    S_Uart_SetParity(uartid,NONE);
    S_Uart_SetWordSize(uartid,WORD_SIZE_8);
    S_Uart_SetStopBit(uartid,LCR_STOP_BIT_1);
    S_Uart_SetRXMode(uartid,TRUE);
    S_Uart_SetTXMode(uartid,TRUE);
    return SUCCESS;
}

/**
  * @功能:打开SUART，配置SUART，初始化发送接收缓冲区
  * @注意:波特率为19200 8位数据宽度 无奇偶校验 1个停止位 查询模式     
  * @参数:uartid为SUART序号，0 和 1   (*handler)回调函数
  * @返回值:SUCCESS 配置成功  PARA_ERROR参数错误
  */
int S_Uart_Open(S_Uart_Device uartid,void (*handler)(char error))
{
    Struct_UartInfo *info;
    volatile uint32_t dum_reg;
    info = &(S_Uart_Table[uartid]);
    if (uartid >= UARTID_MAX)
    {
      return PARA_ERROR;
    }
    if (info->bopened)
    {
      return PARA_ERROR;
    }
    S_Uart_Init(uartid);
  
  	/* intilize the sending and receiving buffers */
    S_CircleBuffer_Init(&(info->txcirclebuffer), info->txbuffer, S_UART_TXBUFFERSIZE);
    S_CircleBuffer_Init(&(info->rxcirclebuffer), info->rxbuffer, S_UART_RXBUFFERSIZE);

    /*clear int*/
    //dum_reg = info->addr[S_UART_RBR];
    //info->addr[S_UART_THR] = dum_reg;
    //dum_reg = info->addr[S_UART_LSR]; 
    
    info->handler = handler;
    info->bopened = TRUE;
    return SUCCESS;
}

/**
  * @功能:关闭对应SUART
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
 int S_Uart_Close(S_Uart_Device uartid) 
{
    Struct_UartInfo *info;
  
    info = &(S_Uart_Table[uartid]);	
    if ((uartid < UARTID_MAX) && (info->bopened))
    {
        info->addr[S_UART_IER] &= ~IER_RDA_INT_ENABLE;
    	info->handler = NULL;
        info->bopened = 0;
    	return SUCCESS;
    }
    return PARA_ERROR;
}  
/**
  * @功能:修改对应SUART波特率
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 baudrate 配置的波特率(S_Uart_Baudrate类型)
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
#include <math.h>
int S_Uart_ChangeBaudrate(S_Uart_Device uartid,S_Uart_Baudrate baudrate)
{
    int divisor;		
    int timecount;
    Struct_UartInfo *info;
    timecount=0; 	
    info = &(S_Uart_Table[uartid]);
    
    if((baudrate == B4800) || (baudrate == B9600) || \
     (baudrate == B14400) || (baudrate == B19200) || \
     (baudrate == B38400) || (baudrate == B56000) || \
     (baudrate == B57600) || (baudrate == B115200))           
    {
        while((info->addr[S_UART_USR] & USR_UART_BUSY) \
  	  		&& (timecount < UART_BUSY_TIMEOUT))
    	{
          timecount++;
    	}
    	if(timecount >= UART_BUSY_TIMEOUT)
    	{
          return PARA_ERROR;
    	}
    	else
    	{
          /*baudrate=(seriak clock freq)/(16*divisor).*/
          unsigned int div;
          float fdiv=0.0;
          fdiv = ((APB_DEFAULT_FREQ >>4)*1.0)/baudrate;
          div = (unsigned int)fdiv;
          if(fabs(fdiv - div) > 0.5)
          {
            div += 1;
          }
          /*高波特率 每发完一帧 稍作延迟，降低误码率，做帧同步*/
          divisor = div;//((  APB_DEFAULT_FREQ / baudrate ) >> 4);
          info->addr[S_UART_LCR] |= LCR_SET_DLAB;
          /* DLL and DLH is lower 8-bits and higher 8-bits of divisor.*/
          info->addr[S_UART_DLL] = divisor & 0xff;         
          info->addr[S_UART_DLH] = (divisor >> 8) & 0xff;     
          
          info->addr[S_UART_LCR] &= (~LCR_SET_DLAB);
          info->baudrate = baudrate;
          return SUCCESS;
      	}
    }
    return PARA_ERROR;
}

/**
  * @功能:对应SUART 校验使能和使能奇校验或偶校验
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 parity 配置的奇偶校验 parity--ODD=8, EVEN=16, or NONE=0.
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_SetParity(S_Uart_Device uartid,S_Uart_Parity parity)
{
    Struct_UartInfo *info;
	
    info = &(S_Uart_Table[uartid]);	
    int timecount;
    timecount = 0;
  
    while((info->addr[S_UART_USR] & USR_UART_BUSY) && \
        (timecount < UART_BUSY_TIMEOUT))
    {
    	timecount++;
    }
    
    if(timecount >= UART_BUSY_TIMEOUT)
    {
    	return PARA_ERROR;
    }
    else
    {
    	switch(parity)
        {
          case NONE:     
              info->addr[S_UART_LCR] &= (~LCR_PARITY_ENABLE);
              break;
          case ODD:
              info->addr[S_UART_LCR] |= LCR_PARITY_ENABLE;
              info->addr[S_UART_LCR] &= LCR_PARITY_ODD; 
              break;
          case EVEN:
              info->addr[S_UART_LCR] |= LCR_PARITY_ENABLE;
              info->addr[S_UART_LCR] |= LCR_PARITY_EVEN;
              break;
          default:
              return PARA_ERROR;
              break;				 
    	}
    	info->parity = parity;
    	return SUCCESS;
    }
}

/**
  * @功能:修改对应SUART停止位
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 stopbit 配置的停止位  TOP_BIT_1 and STOP_BIT_2.
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_SetStopBit(S_Uart_Device uartid,S_Uart_StopBit stopbit)
{
    Struct_UartInfo *info;
    info = &(S_Uart_Table[uartid]);	
    int timecount;
    timecount = 0;
    
    while((info->addr[S_UART_USR] & USR_UART_BUSY) && \
        (timecount < UART_BUSY_TIMEOUT))
    {
    	timecount++;
    }
    
    if(timecount >= UART_BUSY_TIMEOUT)
    {
    	return PARA_ERROR;
    }
    else
    {
    	switch(info->stop)
    	{
          case LCR_STOP_BIT_1:
              info->addr[S_UART_LCR] &= LCR_STOP_BIT1;
              break;
          case LCR_STOP_BIT_2:
              info->addr[S_UART_LCR] |= LCR_STOP_BIT2;
              break;
          default:
              return PARA_ERROR;
          break;
        }
    }
    info->stop = stopbit;
    return SUCCESS;
}

/**
  * @功能:配置对应SUART数据长度
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 wordsize 配置的数据长度 5，6，7，8位
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_SetWordSize(S_Uart_Device uartid,S_Uart_WordSize wordsize)
{
    Struct_UartInfo *info;
    int timecount=0;

    info = &(S_Uart_Table[uartid]);	
    while((info->addr[S_UART_USR] & USR_UART_BUSY) && \
        (timecount < UART_BUSY_TIMEOUT))
    {
    	timecount++;
    }
    
    if(timecount >= UART_BUSY_TIMEOUT)
    {
    	return PARA_ERROR;
    }
    else
    {
        timecount = 0;
    	switch(wordsize)
    	{
          case WORD_SIZE_5:
              info->addr[S_UART_LCR] &= LCR_WORD_SIZE_5;
              break;
          case WORD_SIZE_6:
              info->addr[S_UART_LCR] &= 0xfd;
              info->addr[S_UART_LCR] |= LCR_WORD_SIZE_6;
              break;
          case WORD_SIZE_7:
              info->addr[S_UART_LCR] &= 0xfe;
              info->addr[S_UART_LCR] |= LCR_WORD_SIZE_7;
              break;
          case WORD_SIZE_8:
              info->addr[S_UART_LCR]|= LCR_WORD_SIZE_8;
              break;
          default:
              break;
    	}			
    }
    info->word = wordsize;
    return SUCCESS;
}
	
/**
  * @功能:配置对应SUART发送数据方式
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery 发送数据方式 bQuery TRUE - 查询模式, FALSE - 中断模式
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_SetTXMode(S_Uart_Device uartid, int bQuery)
{
    Struct_UartInfo *info;	
    info = &(S_Uart_Table[uartid]);	
    int timecount;
    timecount = 0;
    
    while((info->addr[S_UART_USR] & USR_UART_BUSY) && \
        (timecount < UART_BUSY_TIMEOUT))
    {
    	timecount++;
    }
    
    if(timecount >= UART_BUSY_TIMEOUT)
    {
    	return PARA_ERROR;
    }
    else
    {
    	if(bQuery)
    	{
            info->addr[S_UART_IER] &= (~IER_THRE_INT_ENABLE);
            info->btxquery = TRUE;
    	}
    	else
    	{
            info->addr[S_UART_IER] |= IER_THRE_INT_ENABLE;
            info->btxquery = FALSE;
    	}
    }
    return SUCCESS;
}

/**
  * @功能:配置对应SUART接收数据方式
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery 接收数据方式 bQuery TRUE - 查询模式, FALSE - 中断模式
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_SetRXMode(S_Uart_Device uartid, int  bQuery)
{
    Struct_UartInfo *info;	
    info = &(S_Uart_Table[uartid]);	
    int timecount;
    timecount = 0;
    
    while((info->addr[S_UART_USR] & USR_UART_BUSY) && \
        (timecount < UART_BUSY_TIMEOUT))
    {
    	timecount++;
    }
    
    if(timecount >= UART_BUSY_TIMEOUT)
    {
    	return PARA_ERROR;
    }
    else
    {
    	if(bQuery)
    	{
            info->addr[S_UART_IER] &= (~IER_RDA_INT_ENABLE);
            info->brxquery = TRUE;
    	}
    	else
    	{
            info->addr[S_UART_IER] |= IER_RDA_INT_ENABLE;
            info->brxquery = FALSE;
    	}
    }
    return SUCCESS;
}
/**
  * @功能:对应SUART接收字符(中断模式或查询模式)
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery ch 数据缓存地址
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */

int S_Uart_GetChar(S_Uart_Device uartid, unsigned char *ch)
{
    Struct_UartInfo *info;
  
    if (uartid >= UARTID_MAX)
    {
    	return PARA_ERROR;
    }
    
    info = &(S_Uart_Table[uartid]);
    if (!(info->bopened))
    {
    	return PARA_ERROR;
    }

    /*query mode*/ 
    if (info->brxquery)
    {
    	while (!(info->addr[S_UART_LSR] & LSR_DATA_READY));
       
        *ch = info->addr[S_UART_RBR];
        return SUCCESS;	
    }
    else
    { 
        /*irq mode*/
    	if(TRUE == S_CircleBuffer_Read(&(info->rxcirclebuffer), ch))
    	return SUCCESS;
    }
    return PARA_ERROR;
}

/**
  * @功能:对应SUART接收字符 不阻塞方式(中断模式或查询模式)
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery ch 数据缓存地址
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_GetCharUnBlock(S_Uart_Device uartid, unsigned char *ch)
{
    Struct_UartInfo *info;

    if (uartid >= UARTID_MAX)
    {
    	return PARA_ERROR;
    }
    
    info = &(S_Uart_Table[uartid]);
    if (!(info->bopened))
    {
    	return PARA_ERROR;
    }

  	/*query mode*/
    if (info->brxquery)
    {
    	if (info->addr[S_UART_LSR] & LSR_DATA_READY)
    	{
          *ch = info->addr[S_UART_RBR];
          return SUCCESS;
        }
    }
    else
    { 
        /*irq mode*/
    	if(TRUE == S_CircleBuffer_Read(&(info->rxcirclebuffer), ch))
    	return SUCCESS;
    }
    return PARA_ERROR;
}
/**
  * @功能:对应SUART发送数据(中断模式或查询模式)
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery ch 发送的数据
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_PutChar_test(S_Uart_Device uartid, unsigned ch)
{
    Struct_UartInfo *info;
    unsigned char temp;

    if (uartid >= UARTID_MAX)
    {
    	return PARA_ERROR;
    }
    
    info = &(S_Uart_Table[uartid]);
    if (!(info->bopened))
    {
    	return PARA_ERROR;
    }
    /*query mode*/
    if (info->btxquery)
    {
    	while ((!(info->addr[S_UART_LSR] & S_LSR_TRANS_EMPTY)));
	    info->addr[S_UART_THR] = ch;
    
    	return SUCCESS;
    } 
    /*irq mode*/
    else
    {
        while (S_CircleBuffer_Write(&(info->txcirclebuffer), ch) == FALSE );    
     	if(info->addr[S_UART_LSR] & 0x20)
     	{
            S_CircleBuffer_Read(&(info->txcirclebuffer), &temp);
            info->addr[S_UART_THR] = temp;
     	}
    }
    return SUCCESS;
}
/**
  * @功能:对应SUART发送字符(中断模式或查询模式)
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 bQuery ch 发送的数据
  * @返回值:SUCCESS 配置成功 PARA_ERROR 参数错误
  */
int S_Uart_PutChar(S_Uart_Device uartid, unsigned char ch)
{
    Struct_UartInfo *info;
    unsigned char temp;
    uint32_t lsr_reg;

    if (uartid >= UARTID_MAX)
    {
    	return PARA_ERROR;
    }
    
    info = &(S_Uart_Table[uartid]);
    if (!(info->bopened))
    {
    	return PARA_ERROR;
    }
    /*query mode*/
    if (info->btxquery)
    {
    	while ((!(info->addr[S_UART_LSR] & S_LSR_TRANS_EMPTY)));
    	if(ch == '\n')
    	{
          info->addr[S_UART_THR] = '\r';
          mydelay(10);
    	}
    	info->addr[S_UART_THR] = ch;   
    	return SUCCESS;
    }  
    /*irq mode*/
    else
    {
        NVIC_DisableIRQ(UART2_IRQn);
            if (S_CircleBuffer_Write(&(info->txcirclebuffer), ch) == FALSE )
            {
                NVIC_EnableIRQ( UART2_IRQn);
                NVIC_SetPendingIRQ(UART2_IRQn);
                return ERROR;
            }
       NVIC_EnableIRQ( UART2_IRQn);
       NVIC_SetPendingIRQ(UART2_IRQn);
         #if 0
    	if(ch == '\n')
    	{
            while (S_CircleBuffer_Write(&(info->txcirclebuffer), '\r') == FALSE );
            while (S_CircleBuffer_Write(&(info->txcirclebuffer), ch) == FALSE );
     	}
     	else
     	{
     	    vPortEnterCritical();
            while (S_CircleBuffer_Write(&(info->txcirclebuffer), ch) == FALSE );
            vPortExitCritical();
     	} 
       #endif 

       lsr_reg = info->addr[S_UART_LSR] ;
       if(lsr_reg & 0x2)
        {
        mprintf("\n err uart n");
        }
       else if(lsr_reg & 0x40)
     	{
     	    NVIC_DisableIRQ(UART2_IRQn);
            S_CircleBuffer_Read(&(info->txcirclebuffer), &temp);
            info->addr[S_UART_THR] = temp;
            NVIC_EnableIRQ( UART2_IRQn);
            NVIC_SetPendingIRQ(UART2_IRQn);
     	}
    }
    return SUCCESS;
}

/**
  * @功能:中断处理函数
  * @注意:无    
  * @参数:uartid为SUART序号，0 和 1 
  * @返回值:无
  */
static void S_Uart_Interrupt(unsigned int irqid)
{
    Struct_UartInfo *info;
    char int_state;
    unsigned char txrxdata;
//  SEnum_Uart_Error error;
    char i;
  
    for(i = 0; i < UARTID_MAX; i++)
    {
      info = &(S_Uart_Table[i]);
      if(info->irq == irqid)
      {
        break;
      }
    }
/*    
    if(i >= UARTID_MAX)
    {
    
    }*/
    int_state = info->addr[S_UART_IIR] & 0xf;
    if((int_state == 0x2) && !(info->btxquery))
    {
      if(!S_CircleBuffer_IsEmpty(&(info->txcirclebuffer)))
      {
        S_CircleBuffer_Read(&(info->txcirclebuffer), &txrxdata);
        info->addr[S_UART_THR] = txrxdata;
      }
    }
    
    if((int_state == 0x4) && !(info->brxquery))
    {
      txrxdata = info->addr[S_UART_RBR];
      S_CircleBuffer_Write(&(info->rxcirclebuffer), txrxdata);
      UART3_REIEIVE++;
    }
}

void S_UART1_int(void)
{
    S_Uart_Interrupt(1);
}

void S_UART0_int(void)
{
    S_Uart_Interrupt(0);
}

/**
  * @功能:设置循环缓冲区的大小，初始化读写位置
  * @注意:无    
  * @参数:1.pcirclebuffer 需要初始化的循环缓冲区属性
  *       2.buffer   循环缓冲区
  *       3.buffersize  缓冲区大小
  * @返回值:无
  */
void S_CircleBuffer_Init(PStruct_CircleBuffer pcirclebuffer,unsigned char * buffer,int buffersize)
{
    pcirclebuffer->Buffer = buffer;
    pcirclebuffer->BufferSize = buffersize;
    pcirclebuffer->ReadPtr = pcirclebuffer->WritePtr = 0;
}

/**
  * @功能:清除读写缓冲区
  * @注意:无    
  * @参数:1.pcirclebuffer 需要初始化的循环缓冲区属性
  * @返回值:无
  */
void S_CircleBuffer_Clear( PStruct_CircleBuffer pcirclebuffer)
{
    pcirclebuffer->ReadPtr = pcirclebuffer->WritePtr = 0;
}

/**
  * @功能:判断循环缓冲区是否为空
  * @注意:无    
  * @参数:1.pcirclebuffer 需要初始化的循环缓冲区属性
  * @返回值:0 非空  1 空
  */
int S_CircleBuffer_IsEmpty(PStruct_CircleBuffer pcirclebuffer)
{
    return( pcirclebuffer->ReadPtr == pcirclebuffer->WritePtr);
}
/**
  * @功能:判断循环缓冲区是否满
  * @注意:无    
  * @参数:1.pcirclebuffer 需要初始化的循环缓冲区属性
  * @返回值:0 非满  1 满
  */
int S_CircleBuffer_IsFull(PStruct_CircleBuffer pcirclebuffer)
{
    return((pcirclebuffer->WritePtr + 1) % pcirclebuffer->BufferSize == pcirclebuffer->ReadPtr);
}
/**
  * @功能:从循环缓冲区读取数据
  * @注意:无    
  * @参数:1.pcirclebuffer 指向循环缓冲区  ch 读取的数据地址
  * @返回值:TRUE 读取成功  FALSE 读取失败
  */ 
int S_CircleBuffer_Read(PStruct_CircleBuffer pcirclebuffer,unsigned char * ch)
{
    if (S_CircleBuffer_IsEmpty(pcirclebuffer))
      return FALSE;
    *ch = pcirclebuffer->Buffer[pcirclebuffer->ReadPtr];
    pcirclebuffer->ReadPtr = (pcirclebuffer->ReadPtr + 1) % pcirclebuffer->BufferSize;
    return TRUE;
}

/*
 * if circlebuffer not full then write a char to circlebuffer
 *
 *pcirclebuffer: point to the circlebuffer
 *ch           : the char will write to circlebuffer
 *return value : write success then return TRUE else reture FALSE   
 */
/**
  * @功能:向循环缓冲区写入数据
  * @注意:无    
  * @参数:1.pcirclebuffer 指向循环缓冲区  ch 写入的数据
  * @返回值:TRUE 写入成功  FALSE 写入失败
  */ 
int S_CircleBuffer_Write(PStruct_CircleBuffer pcirclebuffer,unsigned char ch)
{
    if(S_CircleBuffer_IsFull(pcirclebuffer))
     return FALSE;
    pcirclebuffer->Buffer[pcirclebuffer->WritePtr] = ch;
    pcirclebuffer->WritePtr = (pcirclebuffer->WritePtr + 1) % pcirclebuffer->BufferSize;
    return TRUE;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/




