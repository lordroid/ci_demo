#include "includes.h"
#include "smt_uart.h"
#include "ci100x_suart.h"
#include "ci100x_scu.h"
#include "ci100x_uart.h"
#include "ci100x_scu.h"
#include "user_main.h"
#include "com2_task.h"

unsigned int gsUsartRxCount=0xff;
UART_USER_SETTING_Typedef gs_uar2_user;
extern int ReservedFunc(char *val);
extern BaseType_t send_play_by_func_index(int32_t func_index);

void Suart2_Init(void)
{
#if USER_UART_USE_UART1
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    Scu_SetDeviceGate((unsigned int)UART1,ENABLE);
    
    Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
    Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
   

    UART_EN(UART1,DISABLE);
    UART_BAUDRATEConfig(UART1,UART2_BAUDRATE_DEFAULT);
    UART_CRConfig(UART1,UART_TXE,ENABLE);
    UART_CRConfig(UART1,UART_RXE,ENABLE);
    UART_IntClear(UART1,UART_AllInt);
    UART_IntMaskConfig(UART1,UART_AllInt,ENABLE);
    UART_TXFIFOByteWordConfig(UART1,UART_Byte);
    UART_LCRConfig(UART1,UART_WordLength_8b,UART_StopBits_1,UART_Parity_No);
    UART_RXFIFOConfig(UART1,UART_FIFOLevel1);
    UART_TXFIFOConfig(UART1,UART_FIFOLevel1_8);
    UART_FIFOClear(UART1);
    UART_CRConfig(UART1,UART_NCED,ENABLE);
    UART_IntMaskConfig(UART1,UART_TXInt,ENABLE);/*if TX FIFO is empty,then TX int coming */
    UART_IntMaskConfig(UART1,UART_RXInt,DISABLE);
    UART_EN(UART1,ENABLE);
#else
    //------init
    //////when use uart 2,set rx output high ,then set uart function!
    Scu_SetDeviceGate(UART2_BASE,ENABLE);
    Scu_SetIOReuse(UART2_TX_PAD,THIRD_FUNCTION );
    Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION );
    GPIO_Output(GPIO0,GPIO_Pin6,1);  
    ////////////
    Scu_SetDeviceGate(UART2_BASE,ENABLE);
    Scu_SetIOReuse(UART2_TX_PAD,THIRD_FUNCTION );
    Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION );
    GPIO_Output(GPIO0,GPIO_Pin6,1);  
    Scu_SetIOReuse(UART2_RX_PAD,FIRST_FUNCTION );

    S_Uart_Open(SUART0,NULL);
    S_Uart_ChangeBaudrate(SUART0,UART2_BAUDRATE_DEFAULT);

    S_Uart_SetTXMode(SUART0,0);//uart2 used as 中断方式 
    S_Uart_SetRXMode(SUART0,0);//

    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    gsUsartRxCount = 0xff; 

    gs_uar2_user.chk_enable = 0;
    gs_uar2_user.echo = 0;
    gs_uar2_user.play_by_uart = 0;
#endif
}

void suart2_send(char *buffer,int lenth)
{
    char tc_sum;
    
    if(buffer==NULL)
    {
        mprintf("err com send buf\r\n");
        return;
    }        
    if((lenth<=4)||(lenth > sizeof(sys_com_msg_data_t)))
    {
        mprintf("err com send lenth %d \r\n",lenth);
        return;
    }else
    {
        lenth -= 2;//last 2 byte not send
    }
    
    tc_sum = 0;
    
#if USER_UART_USE_UART1
    while(lenth--)    
    {
        tc_sum += *buffer;
        UartPollingSenddata(UART1,*buffer++);
    }  
    UartPollingSenddata(UART1,tc_sum);//send sum
    UartPollingSenddata(UART1,UART_END);    
#else    
    while(lenth--)    
    {
        tc_sum += *buffer;
        S_Uart_PutChar(SUART0,*buffer++);
    }  
       S_Uart_PutChar(SUART0,tc_sum);//send sum
    S_Uart_PutChar(SUART0,UART_END);
#endif    
  
}

void suart0_rx_handle(unsigned char rxdata)
{    
    static BaseType_t xHigherPriorityTaskWoken;
    static unsigned char rx_buf[sizeof(sys_com_msg_data_t)]; 
    static int tc_state=0;
    static int tc_cnt=0;

    switch(tc_state)
    {
        case SOP_STATE1:
            if(UART_HEADER0 == rxdata)
            {
                rx_buf[tc_cnt++] = rxdata;
                tc_state = SOP_STATE2;
            }            
        break;
        case SOP_STATE2:
            if(UART_HEADER1 == rxdata)
            {
                rx_buf[tc_cnt++] = rxdata;
                tc_state = DATA_STATE;
            }else
            {
                tc_state = SOP_STATE1;
                tc_cnt = 0;
            }
        break;
        case DATA_STATE:
            if(tc_cnt < sizeof(sys_com_msg_data_t))
            {
                rx_buf[tc_cnt] = rxdata; 

                if(tc_cnt == (sizeof(sys_com_msg_data_t)-1))
                {
                    if(rx_buf[tc_cnt] == UART_END)
                    {
                        user_msg_t send_msg;
                        send_msg.msg_type = MSG_TYPE_SYS_COM;                             
                        memcpy((unsigned char*)(&send_msg.msg_data.com_data.header0),rx_buf,sizeof(sys_com_msg_data_t));
                        xQueueSendFromISR(user_task_queue,&send_msg,&xHigherPriorityTaskWoken);
                    }
                    else
                    {
                        mprintf("get com data err_end \r\n");
                    }
                    memset(rx_buf,0,sizeof(sys_com_msg_data_t));
                    tc_state = SOP_STATE1;
                    tc_cnt = 0;
                }
                else
                {
                    tc_cnt++;                
                }               
            }
            else
            {
                mprintf("com2 too many data,reset recive\r\n");
                tc_state = SOP_STATE1;
                tc_cnt = 0;
            }
       break;        
    }
}

               
void uart2_send_AsrResult(unsigned int index,float score)
{
#if 1 //not send score
    user_msg_t send_msg;
    memset((void *)(&send_msg.msg_data.com_data.header0),0,sizeof(send_msg.msg_data.com_data));
    send_msg.msg_type = MSG_TYPE_SYS_COM;
    send_msg.msg_data.com_data.header0 = UART_HEADER0;
    send_msg.msg_data.com_data.header1 = UART_HEADER1;
    send_msg.msg_data.com_data.end= UART_END;
    send_msg.msg_data.com_data.id = 0;
    send_msg.msg_data.com_data.cmd= CI_TX_ASR;
    send_msg.msg_data.com_data.data0= (char)index;  
    send_msg.msg_data.com_data.data1= index>>8;//0;//score_conver;
    
    xQueueSend(user_task_queue,&send_msg,200);
#else    
    short score_conver =(short)(fabs(score)*20);//2.5对应 50分,0对应100分，+-5对应0分
    if(score_conver>=100)
    {
       score_conver =0;
    }else
    {
       score_conver = (100-score_conver);
    }    
     if(score_conver>30)
     {
        user_msg_t send_msg;
        memset((void *)send_msg.msg_data.com_data.buf,0,sizeof(send_msg.msg_data.com_data.buf));
        send_msg.msg_type = MSG_TYPE_SYS_COM;
        send_msg.msg_data.com_data.id= 0x80;
        send_msg.msg_data.com_data.data0 = index;  
        send_msg.msg_data.com_data.data1 = score_conver;
        xQueueSend(user_task_queue,&send_msg,200);
     }
#endif    
}                
                  


void userapp_deal_com_msg(sys_com_msg_data_t *com_data)
{    
    int tmp_data;
    char tc_Chksum;
        
    if((CI_TX_START < com_data->cmd) &&(CI_TX_END > com_data->cmd))
    {
        switch(com_data->cmd)
        {
            case CI_TX_UNWAKEN:
            case CI_TX_ASR:
            case CI_TX_VER:
            case CI_TX_REV:
                suart2_send((char*)com_data,sizeof(sys_com_msg_data_t));
            break;
        }
    } 
    else 
    {   
        if(gs_uar2_user.echo)//回显打开
        {
            suart2_send((char*)com_data,sizeof(sys_com_msg_data_t));
        }
        if(gs_uar2_user.chk_enable)//check sum
        {
            tc_Chksum = com_data->header0+com_data->header1+com_data->id + com_data->cmd + com_data->data0+com_data->data1;
            if(com_data->chksum != tc_Chksum)
            {
                mprintf("get chk=%x,cal chk=%x,err!\r\n",com_data->chksum,tc_Chksum);
                return;
            }
        }
        if((CI_RX_START < com_data->cmd) &&(CI_RX_END > com_data->cmd))
        {
            switch(com_data->cmd)
            {
                case CI_RX_CHK_EN://checksum enable
                    gs_uar2_user.chk_enable = com_data->data0;
                    mprintf("rx cmd = %d,data=%d\r\n",com_data->data0,com_data->data1);
                break;        
                case CI_RX_ECHO_EN://echo enable
                    gs_uar2_user.echo = com_data->data0;
                    mprintf("rx cmd = %d,data=%d\r\n",com_data->data0,com_data->data1);
                break;
                case CI_RX_PLAY_INDEX://play wav index (com_data->data0)
                    tmp_data = com_data->data0;
                    mprintf("tmp_data=%d\r\n",tmp_data);
                    gs_uar2_user.play_by_uart = 1;
                    send_play_by_func_index(tmp_data); 
                break;
                case CI_RX_RESET://play wav index (com_data->data0)
                     if(com_data->data0==1)
                     {
                         mprintf("CI_RX_RESET\r\n");
                          DelayMs(100);        
                        Scu_SoftwareRst_System();
                        while(1)
                          DelayMs(200); 
                     }
                break;
                case CI_RX_UNWAKE_CMD:
                     if(com_data->data0==1)
                     {
                        xSemaphoreTake(WakeupMutex, portMAX_DELAY);  
                        if(awaken)
                        {
                            awaken = 0;
                            mprintf("recive rx ,awake = 0,请唤醒\"...\n");
                          //  play_prompt();                                   
                            TIMx_us(TIMER1,SUSPEND_TIME);
                        }
                        
                        awaken = 0;
                        
                        xSemaphoreGive(WakeupMutex);
                     }
                break;
                case CI_RX_MUTE://
                     if(com_data->data0==0)
                     {
                         nvdata_save.voice_onoff = 0; 
                         nvdata_write((char* )&nvdata_save,sizeof(nvdata_save));   
                     }else
                     {
                         nvdata_save.voice_onoff = 1; 
                         nvdata_write((char* )&nvdata_save,sizeof(nvdata_save));   
                     }
                break;
                case CI_RX_VOLSET://play wav index (com_data->data0)
                     if(com_data->data0 <= 0x20)
                     {
                        nvdata_save.volset = com_data->data0;
                        vol_set(nvdata_save.volset);
                        nvdata_write((char* )&nvdata_save,sizeof(nvdata_save));                           
                     }else
                     {
                        mprintf("volset set %d,too large fail set volset\r\n",com_data->data0);
                     }
                break;
                case CI_RX_GETVER:
                    if(com_data->data0==1)
                    {
                        user_msg_t send_msg;
                        memset((void *)(&send_msg.msg_data.com_data.header0),0,sizeof(send_msg.msg_data.com_data));
                        send_msg.msg_type = MSG_TYPE_SYS_COM;
                        send_msg.msg_data.com_data.header0 = UART_HEADER0;
                        send_msg.msg_data.com_data.header1 = UART_HEADER1;    
                        send_msg.msg_data.com_data.id = 0;
                        send_msg.msg_data.com_data.cmd= CI_TX_VER;
                        send_msg.msg_data.com_data.data0= USER_VERSION_MAIN_NO;  
                        send_msg.msg_data.com_data.data1= USER_VERSION_SUB_NO;//score_conver;
                        send_msg.msg_data.com_data.end= UART_END;

                        xQueueSend(user_task_queue,&send_msg,200);
                    }
                break;
                case CI_RX_REV:
                    tmp_data = ReservedFunc((char *)com_data);
                    user_msg_t send_msg;
                    memset((void *)(&send_msg.msg_data.com_data.header0),0,sizeof(send_msg.msg_data.com_data));
                    send_msg.msg_type = MSG_TYPE_SYS_COM;
                    send_msg.msg_data.com_data.header0 = UART_HEADER0;
                    send_msg.msg_data.com_data.header1 = UART_HEADER1;    
                    send_msg.msg_data.com_data.id = (tmp_data>>16) & 0xff;
                    send_msg.msg_data.com_data.cmd= CI_TX_REV;//(tmp_data>>16);
                    send_msg.msg_data.com_data.data0= (tmp_data>>8) & 0xff;  
                    send_msg.msg_data.com_data.data1= (tmp_data & 0xff);//score_conver;
                    send_msg.msg_data.com_data.end= UART_END;
                    xQueueSend(user_task_queue,&send_msg,200);
                break;
				case CI_RX_ENTER_AWAKE: awaken = 1;
										mprintf("enter the awaken state");
				break;

            }
        }
        else
        {
            mprintf("com2 may recived an undefined cmd\r\n",com_data->cmd);
        }
    }        
}
////////example


