#include "ci100x_wdt.h"
#include "misc.h"
#include "ci100x_uart.h"
#include "includes.h"

void IWDG_feed(void)
{
        WDOGLOCK 			= 0x1ACCE551;		      
        IWDG->WdogIntClr 	= 0x1;
        WDOGLOCK 			= 0x0;
}
/**
  * @功能:IWDG初始化配置函数
  * @注意:无      
  * @参数:sec 看门狗的定时周期，单位秒。
  * @返回值:无
  */
int  iwdg_rstInit(int sec)
{
    int load_value;
    if(sec>300 || sec<=0)
    {
        mprintf("iwdg sec error [%d]\n",sec);
        sec = 300;
    }
    mprintf("IWDG TIME: %d sec\n",sec);
    load_value = 12500000*sec;//(FPCLK/1000L) * 1000;     //load_value = Fpclk * period/1000 
    Scu_SetDeviceGate((unsigned int)IWDG_BASE,ENABLE);
    IWDG_disable();
    IWDG_init(load_value,WDT_IRQ_ENABLE,WDT_RESET_ENABLE);//WDT_IRQ_ENABLE
    Scu_Iwdg_RstSys_Config();   
  
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = IWDG_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
   
    //IWDG_getPeriphID();
    return 0;
}

#if(IWDG_TEST_ENABLE == 1)
#include "ci100x_wdt.h"
#include "smt_assert.h"	
#include "misc.h"

#ifndef FPCLK
#define FPCLK   50000000L
#endif

/****************************************************************************
** 函数  名： IWDG_getPeriphID
** 功    能： 获取看门狗外设标识符
** 入口参数： 无
**           
** 返回  值： int - 外设标识符
**
****************************************************************************/
static int IWDG_getPeriphID()
{   
    IWDG_PID_TypeDef    pid,*mPID = 0;
    
    mPID = &pid;
    mPID->bf.PartNumber = IWDG_PID->WdogPeriphID0.bf.PartNumber0 | 
                              (IWDG_PID->WdogPeriphID1.bf.PartNumber1<<7);
    mPID->bf.Designer = IWDG_PID->WdogPeriphID1.bf.Designer0 |
                              IWDG_PID->WdogPeriphID2.bf.Designer1<<4;
    mPID->bf.Revision = IWDG_PID->WdogPeriphID2.bf.reserved;
    mPID->bf.Configuration = IWDG_PID->WdogPeriphID3.bf.Configuration;
    
    
    mprintf("WatchDog PeriphID:\t0x%08x "
               "\n\t\tPartNumber:\t0x%03x"
               "\n\t\tDesigner:\t0x%02x"
               "\n\t\tRevision:\t%d "
               "\n\t\tConfiguration:\t%d\n",
                mPID->ui,
                mPID->bf.PartNumber,
                mPID->bf.Designer,
                mPID->bf.Revision,
                mPID->bf.Configuration);
    
    return mPID->ui;
}

/****************************************************************************
** 函数  名： IWDG_Test
** 功    能： 看门狗测试程序
** 入口参数： 看门时间，单位:毫秒
**           
** 返回  值： 无
**
****************************************************************************/
void IWDG_Test(int period)
{
    int load_value;
    
    load_value = FPCLK / 1000L * period;     //load_value=Fpclk * period/1000 
    IWDG_disable();
    IWDG_init(load_value,WDT_IRQ_ENABLE,WDT_RESET_ENABLE);
    irq_en(1);
   
    IWDG_getPeriphID();
}
#endif
