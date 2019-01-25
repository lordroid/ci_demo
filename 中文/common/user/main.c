/**
  ******************************************************************************
  * @文件    main.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件是chipintelli公司的CI100X芯片程序的主函数.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
      
#include "includes.h"
#include "user_main.h"
#include <stdint.h>

/*
**********************************************************************************************************
                                            函数声明
**********************************************************************************************************
*/
static void vTaskCreate (void);
static void vTask_LowPower(void*pvParameters);
static void vTaskVariablesInit(void);
extern void vSetupTaskInfoTest(void);
extern void noresult_just_reset(void);


static void welcome(void);
static void prvSetupHardware(void);
void os_printf(char* fmt,...);
extern void MainCallback(void *p);
extern void vTaskFileFATfsDealWith(void*pvParameters);
extern void WriteReadSDPCM(void*pvParameters);
extern void  UserTaskManageProcess(void *p_arg);
extern void  UserTaskKeyScan(void *p_arg);


extern "C" void vTask_Test(void *pvParameters);
extern void ADKey_Init(int adcx);
extern void reset_20921(); 
extern void iis1_play_init();
extern void IIS0_SlaveRxSDRAMpcm(void);
extern void GetUserPara(int dDNNId,int dASRId,int dLanguageId);
extern void MacroInit(int uLanguageId);
extern void iis1_cs42l52_play_cmd(char* WavAddr,unsigned int WavSize);
extern void  AppTaskKeyProcess (void *p_arg);
extern void  vTask_play (void *pvParameters);
extern SD_Error SD_Init(void);
extern void iis1_ES8388_play_cmd();
extern void iis1_ES8388_rec_test();
/*
**********************************************************************************************************
                                            变量定义
**********************************************************************************************************
*/

static TaskHandle_t xHandleTaskASRdecode = NULL;
static TaskHandle_t xHandleLowPower    =NULL;
extern int sysinit_index;

void InitCpuRstState(Reset_Type * p)
{
    if(!p)
    {
        mprintf("para error \n");
        return ;
    }
    memset(p,0,sizeof(Reset_Type));
    p->por = (SCU->SCU_STATE & (1<<0))?1:0;
    p->jtag = (SCU->SCU_STATE & (1<<3))?1:0;
    p->iwtd = (SCU->SCU_STATE & (1<<6))?1:0;
    p->wwtd = (SCU->SCU_STATE & (1<<7))?1:0;
    p->lockup = (SCU->SCU_STATE & (1<<8))?1:0;
    p->srstreq = (SCU->SCU_STATE & (1<<9))?1:0;
    p->softrst = (SCU->SCU_STATE & (1<<10))?1:0;
}
void key_to_exti(unsigned char exti_num,unsigned char swithmode)
{
    
    if(swithmode==1)//exit to key
    {
        if(exti_num==1)
        {
            NVIC_DisableIRQ(EXTI1_IRQn);
            Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION);//pin16-key3
            GPIO_Input(GPIO0,(GPIO_Pinx)6);
		}
		else
		{
           
           
		}
	}
	else    //key to exti 
	{
	    if(exti_num==1)
		{
		    Scu_SetIOReuse(UART2_RX_PAD,FORTH_FUNCTION);//pin16-key3 
		    GPIO_Edges_INT(GPIO0,GPIO_Pin6,GPIO_RISING);
			NVIC_EnableIRQ(EXTI1_IRQn);
	    }

	}
}
void _IO_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel	=EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);

    Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);

#if MASS_FACTOR      
    // UART0_RX                                       
    Scu_SetIOReuse( UART0_RX_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin0,0);      
  
    // UART1_RX                                       
    Scu_SetIOReuse( UART1_RX_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin2,0);    
    
#if (USE_USER_UART && !USER_UART_USE_UART1) 
     //use uart2 ,not set ouput low
#else    
        //UART2_RX
    Scu_SetIOReuse( UART2_RX_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin6,0);  
#endif 

#endif    
    
    // i2s0_sck                                       
    Scu_SetIOReuse( I2S0_SCLK_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin18,0);      
    // i2s1_sck   
    Scu_SetIOReuse( I2S1_SCLK_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin21,0);      
    // i2s1_lrck                                       
    Scu_SetIOReuse( I2S1_LRCLK_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin22,0);      
          
    //I2S0_SDI
    Scu_SetIOReuse( I2S0_SDI_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin20,0); 
	
    Scu_SetIOReuse(JTAG_TDI_PAD,THIRD_FUNCTION);//pin23-ctrl0
    GPIO_Output(GPIO1,GPIO_Pin13,0); 
	
    Scu_SetIOReuse(TRST_PAD,THIRD_FUNCTION);//pin24-ctrl1
    GPIO_Output(GPIO1,GPIO_Pin17,0);   
	
    Scu_SetIOReuse(UART3_TX_PAD,THIRD_FUNCTION);//pin11-led1
    GPIO_Output(GPIO0,GPIO_Pin7,1);  
	
    Scu_SetIOReuse(I2C1_SDA_PAD,THIRD_FUNCTION);//pin10-powerlock
    GPIO_Output(GPIO0,GPIO_Pin16,0);  	
	
	Scu_SetIOReuse(I2C1_SCL_PAD,THIRD_FUNCTION);//pin9-powerinit
    GPIO_Input(GPIO0,(GPIO_Pinx)15);		
    
    Scu_SetIOReuse(UART2_TX_PAD,THIRD_FUNCTION);//pin15-key2
    GPIO_Input(GPIO0,(GPIO_Pinx)5);		
    
    Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION);//pin16-key3
    GPIO_Input(GPIO0,(GPIO_Pinx)6);		
//    GPIO_Edges_INT(GPIO0,GPIO_Pin6,GPIO_FALLING);
	
    Scu_SetIOReuse(CAN_TX_PAD,THIRD_FUNCTION);//pin17-key4
    GPIO_Input(GPIO0,(GPIO_Pinx)11);    

    Scu_SetIOReuse(SPI1_CLK_PAD,THIRD_FUNCTION);//pin38-mp3sclk
    GPIO_Output(GPIO1,GPIO_Pin8,0);  
	
    Scu_SetIOReuse(SPI1_CS_PAD,THIRD_FUNCTION);//pin39-mp3csn
    GPIO_Output(GPIO1,GPIO_Pin7,1);  
	
    Scu_SetIOReuse(SPI1_DIN_PAD,THIRD_FUNCTION);//pin40-mp3si
    GPIO_Input(GPIO1,(GPIO_Pinx)6);
	
    Scu_SetIOReuse(SPI1_DOUT_PAD,THIRD_FUNCTION);//pin41-mp3so
    GPIO_Output(GPIO1,GPIO_Pin5,1); 
	
    Scu_SetIOReuse(UART1_RTS_PAD,THIRD_FUNCTION);//pin42-mp3gdo
    GPIO_Output(GPIO0,GPIO_Pin4,1); 
	
    Scu_SetIOReuse(UART1_CTS_PAD,THIRD_FUNCTION);//pin43-mp3play
    GPIO_Input(GPIO0,(GPIO_Pinx)3);     

}

extern void GetNVUserData(void);
/**
  * @功能:FreeRTOS启动前的相关硬件的配置
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void prvSetupHardware(void)
{
    InitCpuRstState(&CPU_rstType);
           
    //uart init
#if USER_UART_USE_UART1
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    UARTPollingConfig(UART0);
#else    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    UARTPollingConfig(UART1);
#endif  
    _IO_init();
    
    run_mode = 1;//always = 1  
    //spi init
    QSPI_Init();
    GetNVUserData();
    GetUserPara(SUPPORT_CHINESE_LANGUAGE,SUPPORT_CHINESE_LANGUAGE,nvdata_save.uLanguageId);
    
    //8388 && 20921 init
    iis1_play_init(); 
//    reset_20921();
    MacroInit(SUPPORT_CHINESE_LANGUAGE);

    run_mode = 1;
    if((run_mode==0)||(run_mode==2))
    {
        if(0)
        {
            if(SD_Init() == SD_OK)
                sd_init_ok = 1;
            else
            {
                run_mode = 1;
                sd_init_ok = 0;
            }
        }else
        {
            sd_init_ok = 0;
            run_mode = 1;
        }
    }
#if RECTEST
    //直接录音到0x70500000, 在jlink.exe上使用savebin <文件名> 0x70500000 0x100000
    iis1_ES8388_rec_test();
    while(1);
#endif

#if USE_ASR8388 && !DECODE_DEBUG
    iis1_ES8388_play_cmd();
#endif 

#if !DECODE_DEBUG && !USE_ASR8388
    if(0 == run_mode) 
    {
        IIS0_SlaveRxSDRAMpcm();
    }else if(1 == run_mode) 
    {
        IIS0_SlaveRxpcm(); 
    }else if(2 == run_mode) 
    {   
        IIS0_SlaveRxSDRAMpcm();
    }
#endif
  
    userapp_initial();

    
#if USE_IWDG
   extern int  iwdg_rstInit(int sec);
   iwdg_rstInit(IWDG_TIME); 
#endif
}

extern void Test_Stamp_BoardIo(void);

/**
  * @功能:主函数，进入应用程序的入口
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
int main(void)
{
  	/*屏蔽所有中断,除内核异常,如需其他测试，请屏蔽此句*/
    //	__set_PRIMASK(1);  
#if ENABLE_TEST_STAMP_BOARD    
	Test_Stamp_BoardIo();
#endif	
	/* 硬件初始化 */
	prvSetupHardware(); 
	
	/* 版本信息 */
	welcome();
    
    /* 任务的消息队列、信号量等的创建 */
    vTaskVariablesInit();
    
    /* 创建任务 */
    vTaskCreate();
    
       /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
    
    while(1);
}

/**
  * @功能:获取当前时间
  * @注意:无       
  * @参数:无
  * @返回值:当前的时间us数
  */
unsigned int GetCurrentMs(void)
{
    static int ustime_init = 0;
    unsigned int allus = 0;
    
    if(!ustime_init)
    {
        NVIC_InitTypeDef NVIC_InitStructure={0};
        Scu_SetDeviceGate(TIMER3,ENABLE);
        TIMx_us(TIMER3,8000);
        asrtimer_counter = 0;
        ustime_init = 1;
        NVIC_InitStructure.NVIC_IRQChannel = TIMER3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    allus =  asrtimer_counter;
    
    return  allus;
}
/**
  * @功能:低功耗任务，用于进入\退出低功耗控制
  * @注意:无       
  * @参数:pvParameters
  * @返回值:无
  */
extern void Vad_open_lowpower(void);
uint32_t vad_wakeup_flag =  0;
void vTask_LowPower(void*pvParameters)
{
 
    NVIC_InitTypeDef NVIC_InitStructure={0};
    unsigned int tmp;
    unsigned int times=0;
    uint32_t backup_int;
    
    Scu_SetDeviceGate(TIMER1,ENABLE);
    TIMx_us(TIMER1,SUSPEND_TIME);
    NVIC_InitStructure.NVIC_IRQChannel = TIMER1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    for(;;)
    {
        if(xSemaphoreTake(xLowPowerSemaphore,portMAX_DELAY) == pdTRUE)
        {

            
            NVIC_DisableIRQ(VAD_IRQn);
            #if USER_UART_USE_UART1
            NVIC_DisableIRQ(UART1_IRQn);
            #else
            NVIC_DisableIRQ(UART2_IRQn);
            #endif

            
            noresult_just_reset();
            scu_vadasrreset();


            //关闭FE和DNN gate CLK  
            *(volatile unsigned int*)(0x40010000+0x44) = 0x51AC0FFE;
            *(volatile unsigned int*)(0x40010000+0x40) = 0x51AC0FFE;
            while((*(volatile unsigned int *)(0x40010000 + 0x44)) != 1);
            while((*(volatile unsigned int *)(0x40010000 + 0x40)) != 1);
            *(volatile unsigned int*)(0x40010000 + 0x24) &= ~(0x3<<28); 
            *(volatile unsigned int*)(0x40010000 + 0x24) |= (0x3<<26);
            *(volatile unsigned int*)(0x40010000 + 0x28) &= ~(0x7<<27); 
            Vad_open_lowpower();
            
            NVIC_DisableIRQ(ASR_IRQn);

            mprintf("%d:进入低功耗模式...\n",times++);

            /*backup interrupt enable reg,and disable all*/
            backup_int = NVIC->ISER[0];
            NVIC->ICER[0] = 0xffffffff;
            
            #if USE_IWDG
            extern int  iwdg_rstInit(int sec);
            iwdg_rstInit(IWDG_TIME); 
            NVIC_EnableIRQ(IWDG_IRQn);/*IWDG run check iis work in lowpower mode*/
            #endif

            clkgate_state[0] =  *(volatile unsigned int *)(0x40010000 + 0x24);
            clkgate_state[1] =  *(volatile unsigned int *)(0x40010000 + 0x28);
            /*SDRAM自刷新*/
            *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc) |= (0x1 << 1);
            while(((*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc))&(0x1 << 11)) == 0);
            /*关闭SDRAM CLK*/          
            tmp = *(volatile unsigned int *)(0x40010000 + 0x6c);
            tmp = 0x0;
            tmp |= (0x1 << 23);
            #if USER_UART_USE_UART1
            tmp |= (0x1 << 13);
            #else
            tmp |= (0x1 << 14);
            #endif
            *(volatile unsigned int *)(0x40010000 + 0x6c) = tmp;

            *(volatile unsigned int *)(0x40010000 + 0x58) |= (0x1 << 24);
            *(volatile unsigned int *)(0x40010000 + 0x44) = 0x51AC0FFE;
            while((*(volatile unsigned int *)(0x40010000 + 0x44)) != 1);

            #if USE_IWDG
            *(volatile unsigned int *)(0x40010000 + 0x24) = (1<<0)|(3<<26)|(0x1<<30)|(1<<25);/*iwdg bit 25*/    
            #else
            *(volatile unsigned int *)(0x40010000 + 0x24) = (1<<0)|(3<<26)|(0x1<<30);    
            #endif

            #if USER_UART_USE_UART1
            *(volatile unsigned int *)(0x40010000 + 0x24) |= (1<<17);
            #else
            *(volatile unsigned int *)(0x40010000 + 0x24) |= (1<<18);
            #endif

            *(volatile unsigned int *)(0x40010000 + 0x28) = (0x3 << 23) | (0x3 << 20)|(0x3 << 16)|(1<<8);/*timer3 bit8*/          

            *(volatile unsigned int *)(0x40010000 + 0x20) = 0x3df;

            #if 0/*single mic add timer3 no div freq*/
            /*配置AHB分频*/
            *(volatile unsigned int*)(0x40010000+0x44) = 0x51AC0FFE;
            while((*(volatile unsigned int *)(0x40010000 + 0x44)) != 1);
            *(volatile unsigned int *)(0x40010000 + 0x08) &= ~(0x3f << 0);
            *(volatile unsigned int *)(0x40010000 + 0x08) |= (0x8 << 0);
            *(volatile unsigned int *)(0x40010000 + 0x3c) |= (0x1 << 0);
            #endif
//            key_to_exti(1,0);
			Scu_SetWakeup_Source(EXTI1_IRQn);

            NVIC_EnableIRQ(SCU_IRQn);
            NVIC_EnableIRQ(TIMER3_IRQn);
            for(volatile int delay = 0x1800;delay;delay--){__NOP();}
            *(volatile unsigned int *)(0xe000e010) &= ~(0x1 << 0);
            mprintf("sleep\n");
            vad_wakeup_flag = 0;
            while(1)
            {
                iwdg_watch = 1<<2;
                if(0 == vad_wakeup_flag)
                {
                    /*even vad irq on here,set vad_wakeup_flag , then asr irq will wakeup*/
                    asm("wfi"); /*timer 3 wakeup,and then lowpower*//*iwdg*/
                }
                else
                {
                    break;
                }
                iwdg_watch = 1<<2;
            }
            mprintf("退出低功耗模式...\n");
            
            /*restore interrupt enable reg*/
            NVIC->ISER[0] = backup_int;
            
        }
    }
}


/**
  * @功能:所有任务的创建 
  * @注意:无       
  * @参数:pvParameters
  * @返回值:无
  */
static void vTaskCreate (void)
{ 

#if PLAY_TASK_ENABLE
    xTaskCreate(vTask_play,"vTask_play",512,NULL,5,NULL);
#endif

    xTaskCreate( vTaskASRdecode,         /* 任务函数  */
        "vTaskASRdecode",       /* 任务名    */
        640,                    /* 任务栈大小，单位word，也就是4字节 */
        NULL,                   /* 任务参数  */
        4,                      /* 任务优先级*/
        &xHandleTaskASRdecode );   /* 任务句柄  */

    xTaskCreate( vTask_Test,         /* 任务函数  */
        "vTask_Test",       /* 任务名    */
        256,                    /* 任务栈大小，单位word，也就是4字节 */
        NULL,                   /* 任务参数  */
        2,                      /* 任务优先级*/
        NULL );   /* 任务句柄  */   
#if USE_S_MIC_DENOISE
     extern void vTask_Denoise(void *pvParameters);
     xTaskCreate( vTask_Denoise,"vTask_Denoise",512,NULL,4,NULL);
#endif
#if USE_S_MIC_DENOISE
#else 
    #if !DECODE_DEBUG
    xTaskCreate( vTask_LowPower,     /* 任务函数  */
        "vTask_LowPower",       /* 任务名    */
        256,                    /* 任务栈大小，单位word，也就是4字节 */
        NULL,                   /* 任务参数  */
        4,                      /* 任务优先级*/
        &xHandleLowPower );   /* 任务句柄  */
    #endif 
#endif 
    xTaskCreate(UserTaskManageProcess,"UserTaskManageProcess",512,NULL,4,NULL);
    xTaskCreate(UserTaskKeyScan,"UserTaskKeyScan",512,NULL,3,NULL);

}

/**
  * @功能:任务间通信变量的初始化，例如,消息队列、信号量等的创建。
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
static void vTaskVariablesInit(void)
{
    vSemaphoreCreateBinary(xLowPowerSemaphore);
    if(!xLowPowerSemaphore)
    {
        mprintf("低功耗信号量创建失败！\n");
    }
    else
    {
        xSemaphoreTake(xLowPowerSemaphore,portMAX_DELAY);
        mprintf("低功耗信号量创建成功 :%x\n",xLowPowerSemaphore);
    }

    vSemaphoreCreateBinary(xVadpcmSemaphore);
    if(!xVadpcmSemaphore)
    {
        mprintf("VADPCM信号量创建失败！\n");
    }
    else
    {
        xSemaphoreTake(xVadpcmSemaphore,portMAX_DELAY);
        mprintf("VADPCM信号量创建成功 :%x\n",xVadpcmSemaphore);
    }
    
    xFileMutexlockSemaphore = xSemaphoreCreateMutex();
    if(!xFileMutexlockSemaphore)
    {
        mprintf("文件保护互斥信号量创建失败！\n");
    }
    else
    {
        mprintf("文件保护互斥信号量创建成功 :%x\n",xFileMutexlockSemaphore);
    }

    user_task_queue = xQueueCreate(32, sizeof(user_msg_t));
    if(user_task_queue==0)
    {
        mprintf("user_task_queue creat fail\r\n");    
    }
    
    ASR_xQueue = xQueueCreate(128,sizeof(asr_datatype));
    if(!ASR_xQueue)
    {
        mprintf("识别队列创建失败！\n");
    }
    else
    {
        mprintf("识别队列创建成功 :%x \n",ASR_xQueue);
    }
    
    Vadpcm_xQueue = xQueueCreate(5,sizeof(int));
    if(!Vadpcm_xQueue)
    {
        mprintf("VAD 语音队列创建失败！\n");
    }
    else
    {
        mprintf("VAD 语音队列创建成功 :%x \n",Vadpcm_xQueue);
    }
    WakeupMutex = xSemaphoreCreateMutex();
    if(!WakeupMutex)
    {
      mprintf("唤醒机制互斥锁！\n");
    }
    else
    {
      mprintf("唤醒机制互斥锁 :%x\n",WakeupMutex);
    }

    vSemaphoreCreateBinary(xRecSemaphore);
    if(!xRecSemaphore)
    {
        mprintf("tf REC复位信号量创建失败！\n");
    }
    else
    {
        xSemaphoreTake(xRecSemaphore,portMAX_DELAY);
        mprintf("tf REC复位信号量创建成功 :%x\n",xRecSemaphore);
    }
    
    vSemaphoreCreateBinary(xRecStopSemaphore);
    if(!xRecStopSemaphore)
    {
        mprintf("tf xRecStopSemaphore  creat err\n");
    }
    else
    {
        xSemaphoreTake(xRecStopSemaphore,portMAX_DELAY);
        mprintf("tf xRecStopSemaphore create success :%x\n",xRecStopSemaphore);
    }
    
    vSemaphoreCreateBinary(xRecSemaphore_4K);
    if(!xRecSemaphore_4K)
    {
        mprintf("tf REC 4k信号量创建失败！\n");
    }
    else
    {
        xSemaphoreTake(xRecSemaphore_4K,portMAX_DELAY);
        mprintf("tf REC 4k信号量创建成功 :%x\n",xRecSemaphore_4K);
    }
    SpiWrMutex = xSemaphoreCreateMutex();
    if(!SpiWrMutex)
    {
      mprintf("SpiWrMutex creat err!!\n");
    }
    I2CMutex = xSemaphoreCreateMutex();
    if(!I2CMutex)
    {
      mprintf("SpiWrMutex creat err!!\n");
    }
	
    #if PLAY_TASK_ENABLE    
    play_Q = xQueueCreate(5, sizeof(int));
    if(play_Q==0)
    {
        mprintf("play_Q creat fail\r\n");    
    }
    #endif    
}

/**
  * @功能:显示版本信息 
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
static void welcome(void)
{
    mprintf("\r\n");
    mprintf("\r\n");
    mprintf("ci100x_sdk_%s_%d.%d.%d Built-in\r\n",
               SDK_TYPE,
               SDK_VERSION_MAIN_NO,SDK_VERSION_SUB_NO,SDK_VERSION_SUB2_NO);    
    mprintf("UserVer_%s_%d.%d Built-in\r\n",
               USER_TYPE,
               USER_VERSION_MAIN_NO,USER_VERSION_SUB_NO);         
    mprintf("\033[1;32mWelcome to ci100x_sdk.\033[0;39m\r\n");  
#if (MODEL_DEFINE==MODEL_V1)
    mprintf("<<MODEL_V1>>\r\n");
#elif (MODEL_DEFINE==MODEL_STD)
    mprintf("<<MODEL_STD>>\r\n");
#endif
}

/**
  * @功能:操作系统串口打印函数 ,只能用于任务中调试打印，
  * @注意:打印的时候,暂停任务调度,波特率设置尽量高，以减少对OS的实时性       
  * @参数:可变参(最多一次性打印250字符)
  * @返回值:无
  */
void os_printf(char* format,...)
{
/*    char printk_string[250];
    vTaskSuspendAll();
    va_list args;
    va_start(args, format);
    vsnprintf(printk_string, sizeof(printk_string)/sizeof(char) - 1, format,\
      args);
    _puts(printk_string);
    va_end(args);
    if(!xTaskResumeAll())
    {
        taskYIELD();
    }*/
}

/**
  * @功能:FREERTOS tickless 进入低功耗前的自定义函数处理,例如降低主频等
  * @注意:暂时不用此函数      
  * @参数:参数
  * @返回值:无
  */
void OS_PreSleepProcessing(uint32_t vParameters)
 {
     (void)vParameters;
    //vParameters = 0;
    static int cnt =0 ;
    mprintf("%d in sleep\n",cnt++);
 }

/**
  * @功能:FREERTOS tickless 退出低功耗后的自定义函数处理,例如设置正常主频等
  * @注意:暂时不用此函数      
  * @参数:参数
  * @返回值:无
  */
void OS_PostSleepProcessing(uint32_t vParameters)
{
    static int cnt = 0;
      mprintf("%d out sleep\n",cnt++);
}

/**
  * @功能:获取系统运行时间,调用FreeRTOS API实现
  * @注意:操作系统调度器启动后再调用该函数,不可再中断中使用。    
  * @参数:show_flag 非0 显示当前时\分\秒\毫秒; 0不显示
  * @返回值:当前的时间毫秒数
  */
unsigned int OS_GetCurrentMs(int show_flag)
{
    unsigned int time =0,MS;
    int h,m,s,ms;

    time=xTaskGetTickCount();

    time = time*portTICK_PERIOD_MS;
    MS = time;
    
   if(show_flag)
    {
        ms=time%1000;

        time=time/1000;
        s=time%60;

        time=time/60;    
        m=time%60;

        h=time/24;
        mprintf("[%d:%02d:%02d.%03d]",h,m,s,ms);
    }
    return MS;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/  
