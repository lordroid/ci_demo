/**
  ******************************************************************************
  * @文件    ci100x_timer.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的timer模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_timer.h"
#include "misc.h"
#include "ci100x_uart.h"


/**
  * @功能:启动定时器
  * @注意:无          
  * @参数:TIMx        - 定时器(TIMER0/TIMER1/TIMER2/TIMER3)
  *       initStruct  - 定时器初始化结构体
  * @返回值:无
  */
void timer_init(int TIMx,TIM_InitTypeDef* initStruct)
{
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    timer->ew.ui        = 0;
    timer->cfg0.bf.trest_en = 0;
    timer->cfg0.bf.tru_en   = 0;
    timer->cfg0.bf.tsel_clk = (initStruct->sourceClk == TIMER_CLK_CASCADE)?1:0;
    
    timer->cfg.bf.tm    = TIMER_ISR_WIDTH_4CLK;
    timer->cfg.bf.cm    = initStruct->mode;
    timer->cfg.bf.cs    = (initStruct->sourceClk == TIMER_CLK_PCLK)?TIMER_CLK_PCLK:TIMER_CLK_EXTCLK;
    timer->cfg.bf.tp    = TIMER_ISR_HIGH;    
    timer->cfg.bf.ts    =  initStruct->divFreq;
          
    timer->sc    = initStruct->period;//FPCLK/(div*1000L)*initStruct->period;    //N=Fpclk * period/(div * 1000) 
    
    timer->cfg1.bf.bpen = 0;
    timer->cfg1.bf.ct   = 0;
    timer->cfg1.bf.pc   = 0;
    timer->cfg1.bf.res  = 1;
    timer->cfg1.bf.ru   = 1;      
}

/**
  * @功能:启动定时器
  * @注意:无          
  * @参数:TIMx        - 定时器(TIMER0/TIMER1/TIMER2/TIMER3)
  *       initStruct  - 定时器初始化结构体
  * @返回值:无
  */
void timer_start(int TIMx,TIM_InitTypeDef* initStruct)
{
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    unsigned char div;
   
    timer->ew.ui        = 0;
    timer->cfg0.bf.trest_en = 0;
    timer->cfg0.bf.tru_en   = 0;
    timer->cfg0.bf.tsel_clk = (initStruct->sourceClk == TIMER_CLK_CASCADE)?1:0;
    
    timer->cfg.bf.tm    = TIMER_ISR_WIDTH_4CLK;
    timer->cfg.bf.cm    = initStruct->mode;
    timer->cfg.bf.cs    = (initStruct->sourceClk == TIMER_CLK_PCLK)?TIMER_CLK_PCLK:TIMER_CLK_EXTCLK;
    timer->cfg.bf.tp    = TIMER_ISR_HIGH;    
    timer->cfg.bf.ts    =  initStruct->divFreq;
        
    if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_NO )      div = 1;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_2 ) div = 2;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_4 ) div = 4;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_16 )div = 16;  
    timer->sc    = FPCLK/(div*1000L)*initStruct->period;    //N=Fpclk * period/(div * 1000) 
    
    timer->cfg1.bf.bpen = 0;
    timer->cfg1.bf.ct   = 0;
    timer->cfg1.bf.pc   = 0;
    timer->cfg1.bf.res  = 1;
    timer->cfg1.bf.ru   = 1;      
}
/**
  * @功能:停止定时器计时
  * @注意:无          
  * @参数:TIMx  - 定时器(TIMER1/TIMER2/TIMER3/TIMER4)
  * @返回值:无
  */
void timer_stop(int TIMx)
{
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    
    timer->cfg1.bf.ct    = 1;   //清除定时器中断
    timer->cfg1.bf.pc    = 1;   //暂停计数*/
    //mprintf("stop timer\n");
}
/**
  * @功能:定时器计重新开始计数
  * @注意:无          
  * @参数:TIMx  - 定时器(TIMER1/TIMER2/TIMER3/TIMER4)
  * @返回值:无
  */
void timer_restart(int TIMx)
{
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMx;
    timer->cfg1.bf.res      = 1;
    timer->cfg1.bf.pc       = 0;
}

/**
  * @功能:读取定时器已经消耗的时间
  * @注意:无          
  * @参数:TIMx  - 定时器(TIMER1/TIMER2/TIMER3/TIMER4)
  * @返回值:时间，单位：毫秒
  */
unsigned int timer_elapse(int TIMx)
{
    TIMER_PWM_TypeDef       *timer=(TIMER_PWM_TypeDef *)TIMx;
    unsigned int    ms;
    unsigned char   div;
        
    timer->cfg0.bf.tru_en = 1;
    if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_NO )      div = 1;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_2 ) div = 2;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_4 ) div = 4;
    else if( timer->cfg.bf.ts == TIMER_CLK_DIVIDED_16 )div = 16;

    ms  = div * timer->cc / (FPCLK / 1000); //ms = div*1000*cc/Fpclk   
    
    return ms;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
