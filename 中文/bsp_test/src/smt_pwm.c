//#include "smt_example_config.h"

#include "Includes.h"
#include "smt.h"
#include "Ci100x_scu.h"
#include "Ci100x_pwm.h"
#include "user_main.h"
#include "stdlib.h"
#include "smt_cs42l52.h"
color rgb_color;

#define DEFAULT_RGB_FREQ        1000
#define DEFAULT_RGB_STEP        100




void RGBDataInit(void)
{
    //rgb led init
    rgb_color.r.duty = 0;
    rgb_color.r.period = DEFAULT_RGB_FREQ;
    rgb_color.g.duty = 0;
    rgb_color.g.period = DEFAULT_RGB_FREQ;
    rgb_color.b.duty = 0;
    rgb_color.b.period = DEFAULT_RGB_FREQ;

    //pwm init
    if(g_ESIC_Type == IC_TYPE_8374)
    {
        Scu_SetIOReuse(XPWM5_PAD,FIRST_FUNCTION);
        Scu_SetDeviceGate(PWM5_BASE,ENABLE);
    }
    else
    {
        Scu_SetIOReuse(TRST_PAD,SECOND_FUNCTION);
        Scu_SetDeviceGate(PWM4_BASE,ENABLE);
    }

    SCU->PWM_REUSE_CFG = 0x05;
    
    Scu_SetIOReuse(JTAG_TDI_PAD,SECOND_FUNCTION);
    Scu_SetDeviceGate(PWM0_BASE,ENABLE);
    
    Scu_SetIOReuse(JTAG_TDO_PAD,SECOND_FUNCTION);
    Scu_SetDeviceGate(PWM1_BASE,ENABLE);

}

void RGBRed(int duty)
{
    //TIM_PWM_Test(PWM4_BASE,duty,freq);
    if(g_ESIC_Type == IC_TYPE_8374)
    {
        pwm_update_duty(PWM5_BASE, duty);
    }
    else
    {
        pwm_update_duty(PWM4_BASE, duty);
    }
}

void RGBGreen(int duty)
{
    //TIM_PWM_Test(PWM1_BASE,duty,freq);
    pwm_update_duty(PWM0_BASE, duty);
}

void RGBBlue(int duty)
{
    //TIM_PWM_Test(PWM0_BASE,duty,freq);
    pwm_update_duty(PWM1_BASE, duty);
}

void RGBRefresh(void)
{
    RGBRed(rgb_color.r.duty);
    RGBGreen(rgb_color.g.duty);
    RGBBlue(rgb_color.b.duty);
}

void RGBOpen(void)
{   
    if(g_ESIC_Type == IC_TYPE_8374)
    {
        TIM_PWM_Test(PWM5_BASE,500,DEFAULT_RGB_FREQ);
    }
    else
    {
        TIM_PWM_Test(PWM4_BASE,500,DEFAULT_RGB_FREQ);
    }
    
    TIM_PWM_Test(PWM1_BASE,500,DEFAULT_RGB_FREQ);
    TIM_PWM_Test(PWM0_BASE,500,DEFAULT_RGB_FREQ);

    rgb_color.r.duty = 500;
    rgb_color.g.duty = 500;
    rgb_color.b.duty = 500;

}

void RGBClose(void)
{
    rgb_color.r.duty = 0;
    rgb_color.g.duty = 0;
    rgb_color.b.duty = 0;
    RGBRefresh();
}

void RGBChangeColor(int index)
{
    switch(index){
        case FUNC_ID_LAMP_OPEN://打开台灯
            RGBOpen();
            break;
        case FUNC_ID_LAMP_CLOSE://关闭台灯
            RGBClose();
            break;
        case FUNC_ID_LAMP_BRTNSS_MAX://最高亮度
            if(rgb_color.r.duty != 0)   rgb_color.r.duty = 900;
            if(rgb_color.g.duty != 0)   rgb_color.g.duty = 900;
            if(rgb_color.b.duty != 0)   rgb_color.b.duty = 900;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_BRTNSS_MID://中等亮度
            if(rgb_color.r.duty != 0)   rgb_color.r.duty = 500;
            if(rgb_color.g.duty != 0)   rgb_color.g.duty = 500;
            if(rgb_color.b.duty != 0)   rgb_color.b.duty = 500;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_BRTNSS_MIN://最低亮度
            if(rgb_color.r.duty != 0)   rgb_color.r.duty = 100;
            if(rgb_color.g.duty != 0)   rgb_color.g.duty = 100;
            if(rgb_color.b.duty != 0)   rgb_color.b.duty = 100;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_BRTNSS_UP://调亮一点
            if(rgb_color.r.duty != 0) 
                if(rgb_color.r.duty < 900)
                    rgb_color.r.duty += DEFAULT_RGB_STEP;
            if(rgb_color.g.duty != 0) 
                if(rgb_color.g.duty < 900)
                    rgb_color.g.duty += DEFAULT_RGB_STEP;
            if(rgb_color.b.duty != 0) 
                if(rgb_color.b.duty < 900)
                    rgb_color.b.duty += DEFAULT_RGB_STEP;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_BRTNSS_DOWN://调暗一点
            if(rgb_color.r.duty != 0) 
                if(rgb_color.r.duty > 100)
                    rgb_color.r.duty -= DEFAULT_RGB_STEP;
            if(rgb_color.g.duty != 0) 
                if(rgb_color.g.duty > 100)
                    rgb_color.g.duty -= DEFAULT_RGB_STEP;
            if(rgb_color.b.duty != 0) 
                if(rgb_color.b.duty > 100)
                    rgb_color.b.duty -= DEFAULT_RGB_STEP;
            RGBRefresh();
            break;          
        case FUNC_ID_LAMP_READING://阅读模式
            rgb_color.r.duty = 500;
            rgb_color.g.duty = 500;
            rgb_color.b.duty = 500;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_NIGHTLIGHT://夜灯模式
            rgb_color.r.duty = 50;
            rgb_color.g.duty = 50;
            rgb_color.b.duty = 0;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_NORMAL://照明模式
            rgb_color.r.duty = 700;
            rgb_color.g.duty = 700;
            rgb_color.b.duty = 700;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_RED://红色模式
            rgb_color.r.duty = 900;
            rgb_color.g.duty = 0;
            rgb_color.b.duty = 0;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_GREEN://绿色模式
            rgb_color.r.duty = 0;
            rgb_color.g.duty = 900;
            rgb_color.b.duty = 0;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_BLUE://蓝色模式
            rgb_color.r.duty = 0;
            rgb_color.g.duty = 0;
            rgb_color.b.duty = 900;
            RGBRefresh();
            break;
        case FUNC_ID_LAMP_RANDOM://彩色模式
            rgb_color.r.duty = rand()%100;
            rgb_color.g.duty = rand()%100;
            rgb_color.b.duty = rand()%100;
            RGBRefresh();
            break;
        default:
            break;
        };
}

#if(PWM_TEST_ENABLE == 1)
#include "ci100x_timer.h"
#include "ci100x_pwm.h"
#include "smt_assert.h"	
#include "misc.h"
/****************************************************************************
** 函数  名： hmc32m_pwm_stop
** 功    能： PWM测试程序
** 入口参数： PWMx   - PWM通道(PWM6/PWM7/PWM8)
**           duty   - 占空比，单位：ms
**           period - PWM周期，单位：ms
** 返回  值：无
**
****************************************************************************/
void TIM_PWM_Test(int PWMx,int duty,int period)
{
    TIM_InitTypeDef initStruct;
    int pinNum;
    int bitOffset;
    
    assert_param( IS_PWM_ALL_PERIPH(PWMx) );
    
    hmc32m_pwm_stop(PWM6);
    hmc32m_pwm_stop(PWM7);
    hmc32m_pwm_stop(PWM8);
    
    initStruct.period       = period;
    initStruct.duty         = duty;    
    initStruct.mode         = COUNTER_MODE_AUTO;    
    initStruct.divFreq      = TIMER_CLK_DIVIDED_16;
    initStruct.sourceClk    = TIMER_CLK_PCLK;
    
    pinNum      = PWM_NUMBER(PWMx);
    bitOffset   = (pinNum < 6)?(((pinNum >> 1) << 1) + 24):((pinNum - 6)<< 1 );
    
    /* 设置PIN为PWM功能 */
    SCU->RSTCFG_LOCK    = 0x51AC0FFE;
    if( pinNum < 6 )
        SCU->IO_REUSE_CFG   = SCU->IO_REUSE_CFG & ~(3 << bitOffset);
    else
        SCU->IO_REUSE_CFG1  = SCU->IO_REUSE_CFG1 & ~(3 << bitOffset);
    
    TIM_PWMIConfig(PWMx,&initStruct);
    hmc32m_pwm_start(PWMx);
}
#endif
