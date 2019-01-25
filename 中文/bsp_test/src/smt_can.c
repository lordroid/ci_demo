#include "ci100x_can.h"
#include "ci100x_uart.h"
#include "ci100x_scu.h"
#include "misc.h"

#define CAN_TXBUFF_SIZE 64
#define SINGEL_FILTER_ID    ((0xab << 24) | (0x3 << 21) | (0x0 << 20) ) 
#define SINGEL_FILTER_MASK  0xffffffff

static void can_transmit_test()
{
    CAN_TransmitTypeDef infor;
    int     size;
    int     number=0;
    int     i;
    unsigned char tx_buf[CAN_TXBUFF_SIZE];
    
    for(i = 0;i<CAN_TXBUFF_SIZE;i++)
        tx_buf[i]   = i;
    
    infor.format    = CAN_Frame_Format_Standard;
    infor.frame     = CAN_Frame_Data;
    infor.identifier= ('c'<<24) | ('i' << 16) | ('1' << 8) | '0';
    
    size    = CAN_TXBUFF_SIZE;
    while(size)
    {
        number  = can_transmit_dataframe((unsigned char *)(tx_buf + number),size,&infor);
        size    -= number;
        if(number == 0)break;           //transmit failed
    }
}

void can_test(void)
{
    CAN_InitTypeDef initStruct;
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = CAN_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    Scu_SetDeviceGate(CAN_BASE,ENABLE);
    /* can第1组 pad切换到can功能*/
    //Scu_SetIOReuse(CAN_TX_PAD,FIRST_FUNCTION);
    //Scu_SetIOReuse(CAN_RX_PAD,FIRST_FUNCTION);
    
    
        /* can第1组 pad切换到can功能*/
    Scu_SetIOReuse(CAN_TX_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(CAN_RX_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(UART2_TX_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(UART2_RX_PAD,SECOND_FUNCTION);
    
    int baut = 100;    
    if(baut == 5)           baut = CAN_BAUT_5K;
    else if(baut == 10)     baut = CAN_BAUT_10K;
    else if(baut == 20)     baut = CAN_BAUT_20K;
    else if(baut == 50)     baut = CAN_BAUT_50K;
    else if(baut == 100)    baut = CAN_BAUT_100K;
    else if(baut == 125)    baut = CAN_BAUT_125K;
    else if(baut == 250)    baut = CAN_BAUT_250K;
    else if(baut == 500)    baut = CAN_BAUT_500K;
    else if(baut == 800)    baut = CAN_BAUT_800K;
    else if(baut == 1000)   baut = CAN_BAUT_1000K;
    else
    {
        baut = CAN_BAUT_100K;
    }
    baut = CAN_BAUT_100K;
    initStruct.identifier   = SINGEL_FILTER_ID;
    initStruct.id_mask      = 0xffffffff;
    initStruct.bautrate     = baut;
    initStruct.filterMode   = CAN_FILTER_SINGLE;
    can_init(&initStruct);  

    can_transmit_test();
}

#if 0
#if(CAN_TEST_ENABLE == 1)
#include "ci100x_can.h"
 
#include "misc.h"
#include "string.h"

#define SINGEL_FILTER_ID    ((0xab << 24) | (0x3 << 21) | (0x0 << 20) ) 
#define SINGEL_FILTER_MASK  0xffffffff

extern unsigned char rx_data[13];
extern volatile unsigned int can_receive_number;
/****************************************************************************
** 函数  名：can_test_display
** 功    能：can主动发送数据，用于测试
** 入口参数：str  - 发送缓冲区指针
**           
** 返回  值：无
**
****************************************************************************/
static void can_test_display(const char *str)
{
    CAN_TransmitTypeDef infor;
    int size;
    int number = 0;    
    
    infor.format = CAN_Frame_Format_Standard;
    infor.frame = CAN_Frame_Data;
    infor.identifier = ('c'<<24) | ('i' << 16) | ('1' << 8) | '0';
    
    size = strlen(str);
    while(size)
    {
        number = can_transmit_dataframe((unsigned char *)(str + number),size,&infor);
        size -= number;
        if(number == 0)
          break;           //transmit failed
    }
}

/****************************************************************************
** 函数  名：can_test_receiveLoop
** 功    能：can数据接收环回程序，用于测试
** 入口参数：rxData  - 接收缓冲区指针
**           
** 返回  值：无
**
****************************************************************************/
static void can_test_receiveLoop(unsigned char *rxData)
{
    CAN_TransmitTypeDef infor;
    BF_CAN_FRAME *frame = (BF_CAN_FRAME *)rxData;
    CAN_SFF_WINDOW *SFFrame = NULL;
    CAN_EFF_WINDOW *EFFrame = NULL;
    int i;
        
    //for(i=0;i<can_receive_number;i++)
    //    mprintf("0x%02x ",rx_data[i]);
    mprintf("\n");
    
    if(frame->format == CAN_Frame_Format_Standard)
    {    
        SFFrame = (CAN_SFF_WINDOW *)rxData;
    
        mprintf("Frame format: \tStandard\n");
        mprintf("Frame type  : \t%s\n",(frame->rtr == CAN_Frame_Data)?"Data Frame":"Remote Frame");
        mprintf("Frame length: \t%d\n",frame->length);        
        mprintf("Frame ID    : \t0x%02x%02x\n",SFFrame->identifier1,SFFrame->identifier2);
        mprintf("Frame Data  : \t");
        for(i=0;i<frame->length;i++)
            mprintf("0x%02x ",SFFrame->data[i]);
        mprintf("\n");
    
        infor.format    = CAN_Frame_Format_Standard;
        infor.frame     = frame->rtr;
        infor.identifier= EFFrame->identifier1 | (EFFrame->identifier2 << 8);
        can_transmit_dataframe(SFFrame->data,frame->length,&infor);
    }
    else
    {        
        EFFrame = (CAN_EFF_WINDOW *)rxData;
    
        mprintf("Frame format: \tExtended\n");
        mprintf("Frame type  : \t%s\n",(frame->rtr == CAN_Frame_Data)?"Data Frame":"Remote Frame");
        mprintf("Frame length: \t%d\n",frame->length);        
        mprintf("Frame ID    : \t0x%02x%02x%02x%02x\n",EFFrame->identifier1,
                                                          EFFrame->identifier2,
                                                          EFFrame->identifier3,
                                                          EFFrame->identifier4);
        mprintf("Frame Data  : \t");
        for(i = 0; i < frame->length; i++)
            mprintf("0x%02x ",EFFrame->data[i]);
        mprintf("\n");
        
        infor.format    = CAN_Frame_Format_Extend;
        infor.frame     = frame->rtr;
        infor.identifier = EFFrame->identifier1 | \
                          (EFFrame->identifier2 << 8) | \
                          (EFFrame->identifier3 << 16) | \
                          (EFFrame->identifier4 << 24);
        can_transmit_dataframe(EFFrame->data,frame->length,&infor);
    }    
}

/****************************************************************************
** 函数  名：can_test
** 功    能：can测试程序
** 入口参数：无
**           
** 返回  值：无
**
****************************************************************************/
void can_test_(void)
{
    CAN_InitTypeDef initStruct;   
    
    mprintf("***********************************************************\n");
    mprintf("**                      CAN test                           \n");
    mprintf("***********************************************************\n");
    
    initStruct.identifier = SINGEL_FILTER_ID;
    initStruct.id_mask = 0xffffffff;
    initStruct.bautrate = CAN_BAUT_500K;
    initStruct.filterMode = CAN_FILTER_SINGLE;
    can_init(&initStruct);    
    //irq_en(8);
      
    can_test_display("start test can...\n");
    while(1)
    {
        if(can_receive_number)
        {
            can_test_receiveLoop(rx_data);
            can_receive_number  = 0;
        }
    }  
}
#endif

#endif 