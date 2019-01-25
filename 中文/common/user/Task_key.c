#include "includes.h"
#include "../../bsp_test/src/cxdish/cxdish.h"
#include "smt_adc.h"
#include "task_asr.h"
#include "user_main.h"


extern int user_code_updata_fromTF();
extern int cx2092x_upgrade(void);
extern void suart2_send(char *buffer,int lenth);
extern BaseType_t send_play_by_func_index(int32_t func_index);

extern void vol_up();
extern void  vol_dn();
void init_timer3_getresource(void);
void cpu_rate_print(void);
//extern void ASR_VAD_DNN_Init(void );
unsigned char MCUwork=0,powerup=0,key_holding_flag=0;
void start_updata()
{
    NVIC_DisableIRQ(VAD_IRQn);

    if((run_mode == 0)||(run_mode == 2))
    {
        xSemaphoreGive(xRecStopSemaphore);
    }

    IIS_EN(IIS0,DISABLE);
    //Scu_SetDeviceGate((unsigned int)IIS0,DISABLE);//有时语音采集可能会测试0x7050000~0x70600000 的ram，升级时关闭采音
    timer_stop(TIMER1);//
}


void end_updata()
{		
    ASR_VAD_DNN_Init(0);   

    IIS_EN(IIS0,ENABLE);
    //Scu_SetDeviceGate((unsigned int)IIS0,ENABLE);//有时语音采集可能会测试0x7050000~0x70600000 的ram，升级时关闭采音
    TIMx_us(TIMER1,SUSPEND_TIME);
}


void userapp_deal_key_msg(sys_key_msg_data_t  *key_msg)
{
    uint32_t key; 
	char dbgbuff[150];
    key = key_msg->key;
			sprintf(dbgbuff,"rcv keyvalue is %d \r\n",key);
			suart2_send(dbgbuff,strlen(dbgbuff));	
    switch(key)
    {
        case (key3evt|keyhold):
		    if(MCUwork==0)
            {
                MCUwork=1;
				mp3init();

				cmd2mp3(0xf2,3000);
                GPIO_Output(GPIO0,GPIO_Pin7,0);  
	            GPIO_Output(GPIO0,GPIO_Pin16,1);
//                key_to_exti(1, 0);
		    }
			else                     
			{
                MCUwork=0;
                GPIO_Output(GPIO0,GPIO_Pin7,1);  
	            GPIO_Output(GPIO0,GPIO_Pin16,0);
			}
            break;
        case key3evt|keyhold|keyrelease:
			key_holding_flag = 0;
			break;
		case key3evt|keyhold|keepholding :
			key_holding_flag = 1;
			break;	

        case key3evt|keyrelease :
            if(MCUwork==0)
            {                                    
                GPIO_Output(GPIO0,GPIO_Pin16,0);                   
            }
			else
			{
//               if(awaken==0)
//			   {
//			       awaken = 1;
//		           nvdata_save.voice_onoff=1;
//                   send_play_by_func_index(1);
//               }
			}
			break;
        default:
            break;
    }

}


#if CPU_RATE_PRINT
extern volatile unsigned int ulHighFrequencyTimerTicks;
char pbuf_cpu_print[400]={0};


void init_timer3_getresource(void)
{
    ulHighFrequencyTimerTicks = 0;
    Scu_SetDeviceGate((unsigned int)TIMER3 ,ENABLE);
    TIMx_us(TIMER3,200);

    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = TIMER3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void cpu_rate_print(void)
{
    mprintf("\n任务名\t任务状态\t优先级\t剩余栈\t任务序号\n");
    vTaskList((char*)pbuf_cpu_print);
    mprintf("%s\n",pbuf_cpu_print);
    mprintf("\n任务名\t\t运行计数\t使用率\n");
    vTaskGetRunTimeStats((char*)pbuf_cpu_print);
    mprintf("%s\n",pbuf_cpu_print);   
}
#endif    

