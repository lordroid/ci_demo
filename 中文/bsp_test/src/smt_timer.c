#include "ci100x_gpio.h"
#include "ci100x_scu.h"
#include "ci100x_timer.h"

#include "misc.h"

#include "ci100x_uart.h"



/**
  * @功能:定时器初始化
  * @注意:无          
  * @参数:TIMx        - 定时器(TIMER0/TIMER1/TIMER2/TIMER3)
  *       定时时间    - 单位ms (定时周期范围 0.4 ms - 1,717,986,918 ms)
  * @返回值:无
  */
void TIMx_ms(int TIMx,unsigned int ms)
{
	TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    timer->ew.ui        = 0;
    timer->cfg0.bf.trest_en = 0;
    timer->cfg0.bf.tru_en   = 0;
    timer->cfg0.bf.tsel_clk = 0;
    
    timer->cfg.bf.tm    = TIMER_ISR_WIDTH_4CLK;
    timer->cfg.bf.cm    = COUNTER_MODE_AUTO;
    timer->cfg.bf.cs    = TIMER_CLK_PCLK;
    timer->cfg.bf.tp    = TIMER_ISR_HIGH;    
    timer->cfg.bf.ts    =  TIMER_CLK_DIVIDED_16;
          
    timer->sc    =  ms;//10*us;
    
    timer->cfg1.bf.bpen = 0;
    timer->cfg1.bf.ct   = 0;
    timer->cfg1.bf.pc   = 0;
    timer->cfg1.bf.res  = 1;
    timer->cfg1.bf.ru   = 1;   
}

/**
  * @功能:定时器初始化
  * @注意:无          
  * @参数:TIMx        - 定时器(TIMER0/TIMER1/TIMER2/TIMER3)
  *       定时时间    - 单位us (定时周期范围 10 us - 1718 s)
  * @返回值:无
  */
void TIMx_us(int TIMx,unsigned int us)
{
	TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    timer->ew.ui        = 0;
    timer->cfg0.bf.trest_en = 0;
    timer->cfg0.bf.tru_en   = 0;
    timer->cfg0.bf.tsel_clk = 0;
    
    timer->cfg.bf.tm    = TIMER_ISR_WIDTH_4CLK;
    timer->cfg.bf.cm    = COUNTER_MODE_AUTO;
    timer->cfg.bf.cs    = TIMER_CLK_PCLK;
    timer->cfg.bf.tp    = TIMER_ISR_HIGH;    
    timer->cfg.bf.ts    =  TIMER_CLK_DIVIDED_4;
          
    timer->sc    =  (unsigned int)(12.5*us);
    
    timer->cfg1.bf.bpen = 0;
    timer->cfg1.bf.ct   = 0;
    timer->cfg1.bf.pc   = 0;
    timer->cfg1.bf.res  = 1;
    timer->cfg1.bf.ru   = 1;   
    //mprintf("start timer\n");
}

/****************************************************************************
** 函数  timer_test
** 功    能：定时器测试程序，只启动一个定时器
** 入口参数：TIMx  - 定时器(TIMER1/TIMER2/TIMER3/TIMER4)
**           ms    - 要定时的周期，单位：毫秒
** 返回  值：无
**
****************************************************************************/
void timer_test(int TIMx,unsigned int ms)
{
    TIM_InitTypeDef initStruct;
    
    timer_stop(TIMER0);
    timer_stop(TIMER1);
    timer_stop(TIMER2);
    timer_stop(TIMER3);
    
    initStruct.period = ms;  
    initStruct.mode = COUNTER_MODE_AUTO;    
    initStruct.divFreq = TIMER_CLK_DIVIDED_16;
    initStruct.sourceClk = TIMER_CLK_PCLK;
    
    timer_start(TIMx,&initStruct);
    //irq_en(14 + TIM_NUMBER(TIMx));
}


void timer_start_test(void)
{
    TIM_InitTypeDef initStruct = {0};
  
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = TIMER1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    Scu_SetDeviceGate(TIMER1_BASE,ENABLE);
    timer_stop(TIMER1);
    initStruct.period = 2000;  
    initStruct.mode = COUNTER_MODE_AUTO;    
    initStruct.divFreq = TIMER_CLK_DIVIDED_16;
    initStruct.sourceClk = TIMER_CLK_PCLK;
    timer_start(TIMER1,&initStruct);
}

void timer_caleb(void)
{
    Scu_SetDeviceGate(GPIO0_BASE,ENABLE);
    Scu_SetIOReuse(XPWM1_PAD,THIRD_FUNCTION);
    GPIO_Output(GPIO0,GPIO_Pin29,1);
    Scu_SetDeviceGate(TIMER2_BASE,ENABLE);
    timer_test(TIMER2,1);
}
