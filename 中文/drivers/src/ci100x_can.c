/**
  ******************************************************************************
  * @文件    ci100x_can.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的CAN模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_can.h"
#include "misc.h"
#include "string.h"

#define CAN_CLK     50000000
#define SINGEL_FILTER_ID    ((0xab << 24) | (0x3 << 21) | (0x0 << 20) ) 
#define SINGEL_FILTER_MASK  0xffffffff
#define CAN_TIMEOUT		    200000

volatile unsigned int g_can_transmix_complete = 0;
unsigned char rx_data[13];
volatile unsigned int can_receive_number = 0;



#if( CAN_CLK == 50000000 )
static const CAN_BAUT_LISTS can_baut[]={
    { CAN_BAUT_5K,          0x27,   0x7f },
    { CAN_BAUT_10K,         0x13,   0x7f },
    { CAN_BAUT_20K,         0x09,   0x7f },
    { CAN_BAUT_50K,         0x03,   0x7f },
    { CAN_BAUT_100K,        0x18,   0x16 },
    { CAN_BAUT_125K,        0x01,   0x2f },
    { CAN_BAUT_250K,        0x00,   0x2f },
    { CAN_BAUT_500K,        0x00,   0x16 },
    { CAN_BAUT_800K,        NULL,   NULL },
    { CAN_BAUT_1000K,       0x00,   0x11 },
    { CAN_BAUT_UNDEFINED,   NULL,   NULL },
};
#endif
/**
  * @功能:根据给定的波特率，计算波特率寄存器值
  * @注意:无      
  * @参数:btr0     - 波特率寄存器0指针        
  *       btr1     - 波特率寄存器1 指针
  *       bautrate - 目标波特率
  * @返回值:int - 0表示成功，-1表示失败
  */
static int can_calculation_bautrate(volatile unsigned char *btr0,volatile unsigned char *btr1,int bautrate)
{
    int i = 0;

    while(1)
    {
        if(can_baut[i].type == CAN_BAUT_UNDEFINED)
        {
          return -1;
        }
        if(can_baut[i].type == bautrate)
        {
            if((can_baut[i].BRT0 == NULL) && (can_baut[i].BRT1 == NULL))
            {
              return -1;
            }
            CAN->reserved0 = can_baut[i].BRT0;
            CAN->BTR0 = can_baut[i].BRT1;
            break;
        }
        i++;
    }
    return 0;
}
/**
  * @功能:can初始化函数
  * @注意:无      
  * @参数:initStruct     - 初始化结构体数据指针   
  * @返回值:无
  */
void can_init(CAN_InitTypeDef *initStruct)
{         
    CAN->MOD.bf.AFM = initStruct->filterMode;
    CAN->MOD.bf.STM = 1;
    
    CAN->MOD.bf.RM = 1;                //进入复位模式
    CAN->CDR = 0xc0;
    CAN->ACR[0] = (initStruct->identifier & 0xff000000) >> 24;
    CAN->ACR[1] = (initStruct->identifier & 0xff0000) >> 16;
    CAN->ACR[2] = (initStruct->identifier & 0xff00) >> 8;
    CAN->ACR[3] = (initStruct->identifier & 0xff);
    CAN->AMR[0] = (initStruct->id_mask & 0xff000000) >> 24;
    CAN->AMR[1] = (initStruct->id_mask & 0xff0000) >> 16;
    CAN->AMR[2] = (initStruct->id_mask & 0xff00) >> 8;
    CAN->AMR[3] = (initStruct->id_mask & 0xff);    
    can_calculation_bautrate(&CAN->reserved0,&CAN->BTR0,initStruct->bautrate);
    CAN->BTR1 = 0x00;       
    CAN->MOD.bf.RM = 0;                //退出复位模式
    
    CAN->OCR = 0x02;             //Normal Output Mode
    CAN->IER.bf.RIE = 1;                //打开接收中断
    CAN->IER.bf.TIE = 1;                //打开发送中断
    CAN->IER.bf.EIE = 1;                //打开错误中断
}
/**
  * @功能:延迟函数
  * @注意:无      
  * @参数:counter  延迟毫秒
  * @返回值:无
  */

static void delay(int counter)
{
    while(counter--);
}
/**
  * @功能:can发送一帧数据
  * @注意:无      
  * @参数:addr           - 发送缓冲区指针
  *       data_length    - 发送数据长度
  *       infor          - 发送数据包信息
  * @返回值:无
  */
int can_transmit_dataframe( unsigned char *addr,unsigned int data_length,CAN_TransmitTypeDef *infor)
{
    BF_CAN_FRAME frame;
    int i;
    int index;
    
	g_can_transmix_complete = 0;
	data_length = (data_length > 8)?8:data_length;
    
    CAN->MOD.bf.RM = 0;            //Normal operation
    while( CAN->SR.bf.TBS == 0 );       //Transmit Buffer Locked

    index = 0;
    if(infor->format == CAN_Frame_Format_Standard)
    {
        frame.format = CAN_Frame_Format_Standard;
        frame.length = data_length;
        frame.rtr = infor->frame;
        CAN_TX_BUFFER[index++] = *(unsigned char *)(&frame);
        CAN_TX_BUFFER[index++] = (infor->identifier & 0xff000000) >> 24;
        CAN_TX_BUFFER[index++] = ((infor->identifier & 0xff0000) >> 16); 
    }
    else
    {
        frame.format = CAN_Frame_Format_Extend;
        frame.length = data_length;
        frame.rtr = infor->frame;
        CAN_TX_BUFFER[index++] = *(unsigned char *)(&frame);
        CAN_TX_BUFFER[index++] = (infor->identifier & 0xff000000) >> 24;
        CAN_TX_BUFFER[index++] = ((infor->identifier & 0xff0000) >> 16) | 0; 
        CAN_TX_BUFFER[index++] = (infor->identifier & 0xff000000) >> 24;
        CAN_TX_BUFFER[index++] = ((infor->identifier & 0xff0000) >> 16) | 0; 
    } 
    for(i = 0;i<data_length;i++)
	{
        CAN_TX_BUFFER[index + i] = *(addr + i);
	}
    while( CAN->SR.bf.TBS == 0 );       //Transmit Buffer Locked

    CAN->CMR.bf.AT = 0;            //终止传输位清零
    CAN->CMR.bf.SRR = 1;            //自接收请求
    CAN->CMR.bf.TR = 1;            //传输请求
    
    i = CAN_TIMEOUT;
    while(g_can_transmix_complete == 0)   //wait transmit complete
    {
      if(i--)
      {
        delay(5000);
      }
      else
      {
        return 0;
      }
    }
    
    return data_length;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
