/**
  ******************************************************************************
  * @文件    ci100x_pwm.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的PWM模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#include "ci100x_timer.h"
#include "ci100x_pwm.h"
#include "misc.h"
/**
  * @功能:PWM初始化配置
  * @注意:在使用前需调用SCU相关接口初始化对应管脚        
  * @参数:PWMx PWM通道(PWM6/PWM7/PWM8)   initStruct  PWM初始化结构体
  * @返回值:无
  */

void TIM_PWMIConfig(int PWMx, PWM_InitTypeDef* initStruct)
{
    TIMER_PWM_TypeDef *pwm=(TIMER_PWM_TypeDef *)PWMx;
    unsigned char div=0;    
    
    pwm->ew.ui            = 0;
    pwm->cfg0.bf.trest_en = 0;
    pwm->cfg0.bf.tru_en   = 0;
    pwm->cfg0.bf.tsel_clk = (initStruct->sourceClk == TIMER_CLK_CASCADE)?1:0;
    
    pwm->cfg.bf.tm    = TIMER_ISR_WIDTH_4CLK;
    pwm->cfg.bf.cm    = initStruct->mode;
    pwm->cfg.bf.cs    = (initStruct->sourceClk == TIMER_CLK_PCLK)?TIMER_CLK_PCLK:TIMER_CLK_EXTCLK;
    pwm->cfg.bf.tp    = TIMER_ISR_HIGH;    
    pwm->cfg.bf.ts    = initStruct->divFreq;

    switch(pwm->cfg.bf.ts)
    {
        case TIMER_CLK_DIVIDED_NO:
            div = 1;
            break;
        case TIMER_CLK_DIVIDED_2:
            div = 2;
            break;
        case TIMER_CLK_DIVIDED_4:
            div = 4;
            break;
        case TIMER_CLK_DIVIDED_16:
            div = 16;
            break;
        default:
            div = 1;
            break;
    }

    pwm->sc    = FPCLK/div/initStruct->freq;
    pwm->spwmc = pwm->sc*initStruct->duty/PWM_MAX_CYCLE;
    
    pwm->cfg1.bf.bpen = 0;
    pwm->cfg1.bf.ct   = 1;
    pwm->cfg1.bf.pc   = 1;
    pwm->cfg1.bf.res  = 1;
    pwm->cfg1.bf.ru   = 1;     
}

/**
  * @功能:停止PWM输出
  * @注意:无       
  * @参数:PWMx PWM通道(PWM6/PWM7/PWM8)
  * @返回值:无
  */
void pwm_stop(int PWMx)
{
    TIMER_PWM_TypeDef *pwm=(TIMER_PWM_TypeDef *)PWMx;
   
    pwm->cfg1.bf.ct    = 1;   //清除定时器中断
    pwm->cfg1.bf.pc    = 1;   //暂停计数*/
}


/**
  * @功能:定时器计开始计数
  * @注意:无       
  * @参数:PWMx PWM通道(PWM6/PWM7/PWM8)
  * @返回值:无
  */
void pwm_start(int PWMx)
{
    TIMER_PWM_TypeDef *pwm=(TIMER_PWM_TypeDef *)PWMx;
    pwm->cfg1.bf.pc       = 0;
}


/*duty_cyle 0~1000*/
int32_t pwm_update_duty(int PWMx, uint32_t duty_cycle)
{
    TIMER_PWM_TypeDef *pwm=(TIMER_PWM_TypeDef *)PWMx;
    if(duty_cycle>PWM_MAX_CYCLE)
    {
        return -1;
    }

    pwm->spwmc = pwm->sc*duty_cycle/PWM_MAX_CYCLE;
    pwm->cfg1.bf.res  = 1;/*update*/
    return 0;
}

void TIM_PWM_Test(int PWMx,int duty,int freq)
{
    PWM_InitTypeDef initStruct;
    int pinNum;
    int bitOffset;
    
    pwm_stop(PWMx);
    
    initStruct.freq         = freq;
    initStruct.duty         = duty;    
    initStruct.mode         = COUNTER_MODE_AUTO;    
    initStruct.divFreq      = TIMER_CLK_DIVIDED_NO;//TIMER_CLK_DIVIDED_16;
    initStruct.sourceClk    = TIMER_CLK_PCLK;
    
    pinNum      = PWM_NUMBER(PWMx);
    bitOffset   = (pinNum < 6)?(((pinNum >> 1) << 1) + 24):((pinNum - 6)<< 1 );
    
    /* 设置PIN为PWM功能 */
    SCU->RSTCFG_LOCK    = 0x51AC0FFE;
    if( pinNum < 6 )
        SCU->IO_REUSE_CFG   = SCU->IO_REUSE_CFG & ~(1 << bitOffset);    /*此处之前是"3<<bitOffset" 会导致LED0长亮，改成“1<<bitOffset”问题解决。jason*/
    else
        SCU->IO_REUSE_CFG1  = SCU->IO_REUSE_CFG1 & ~(3 << bitOffset);
    
    TIM_PWMIConfig(PWMx,&initStruct);
    pwm_start(PWMx);
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/