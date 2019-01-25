/**
  ******************************************************************************
  * @文件    ci100x_pwm.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的PWM模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __CI100X_PWM_H
#define __CI100X_PWM_H

#include "ci100x_timer.h"
#include <stdint.h>

#define PWM0                          PWM0_BASE
#define PWM1                          PWM1_BASE
#define PWM2                          PWM2_BASE
#define PWM3                          PWM3_BASE
#define PWM4                          PWM4_BASE
#define PWM5                          PWM5_BASE
#define PWM6                          PWM6_BASE
#define PWM7                          PWM7_BASE
#define PWM8                          TIMER3_BASE

#define IS_PWM_ALL_PERIPH(PERIPH) (((PERIPH) == PWM6) || \
                                   ((PERIPH) == PWM7) || \
                                   ((PERIPH) == PWM8)))

#define PWM_NUMBER(PWMx)    (PWMx == PWM8)?8:(PWMx - PWM0_BASE)/0x1000

//jason
#define PWM_MAX_CYCLE   1000
typedef struct
{
    unsigned int freq;  //pwm频率,如1K频率即设为1000.
    unsigned int duty;  //pwm占空比,取值区间0~100.
    int sourceClk;       //输入时钟源,TIMER_CLK_PCLK/TIMER_CLK_EXTCLK/TIMER_CLK_CASCADE
    int mode;            //计数模式: COUNTER_MODE_SINGLE，单周期模式
                          //          COUNTER_MODE_AUTO，自动重新计数模式
                          //          COUNTER_MODE_FREE，自由计数模式
                          //          COUNTER_MODE_EVENT，事件计数模式
                          //          COUNTER_MODE_WDT1，看门狗模式1
                          //          COUNTER_MODE_WDT2，看门狗模式2
                          //          COUNTER_MODE_WDT3，看门狗模式3
    int divFreq;         //时钟分频: TIMER_CLK_DIVIDED_NO，不分频
                         //          TIMER_CLK_DIVIDED_2， 2分频
                         //          TIMER_CLK_DIVIDED_4， 4分频
                         //          TIMER_CLK_DIVIDED_16，16分频
} PWM_InitTypeDef;

/****************************************************************************
** 函数  名：TIM_PWMIConfig
** 功    能：PWM初始化配置
** 入口参数：PWMx        - PWM通道(PWM6/PWM7/PWM8)
**           initStruct  - PWM初始化结构体
** 返回  值：无
**
****************************************************************************/
//void TIM_PWMIConfig(int PWMx, TIM_InitTypeDef* initStruct);
void TIM_PWMIConfig(int PWMx, PWM_InitTypeDef* initStruct);

/****************************************************************************
** 函数  名：pwm_stop
** 功    能：停止PWM输出
** 入口参数：PWMx  - PWM通道(PWM6/PWM7/PWM8)
** 返回  值：无
**
****************************************************************************/
void pwm_stop(int PWMx);

/****************************************************************************
** 函数  名：pwm_start
** 功    能：定时器计开始计数
** 入口参数：PWMx  - PWM通道(PWM6/PWM7/PWM8)
** 返回  值：无
**
****************************************************************************/
void pwm_start(int PWMx);

int32_t pwm_update_duty(int PWMx, uint32_t duty_cycle);
void TIM_PWM_Test(int PWMx,int duty,int freq);
#endif
