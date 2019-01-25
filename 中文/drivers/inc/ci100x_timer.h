/**
  ******************************************************************************
  * @文件    ci100x_timer.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的timer模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef _TIMER_H
#define _TIMER_H
#ifdef __cplusplus
 extern "C" {
#endif
#ifndef FPCLK
#define FPCLK   50000000L
#endif

#define TIMER_VALUE_NO_EFFECT           0

#define TIMER_ISR_WIDTH_CT              0
#define TIMER_ISR_WIDTH_2CLK            1
#define TIMER_ISR_WIDTH_4CLK            2
#define TIMER_ISR_WIDTH_8CLK            3

#define TIMER_ISR_HIGH                  0
#define TIMER_ISR_LOW                   1

#define TIMER_WDT_CLEAR                 1
#define TIMER_WDT_RESET                 1

#define TIMER_COUNTER_DECREASE_ONE      1
#define TIMER_COUNTER_RELOAD            1
#define TIMER_RU_ENABLE                 1
#define TIMER_RESET_ENABLE              1


typedef struct
{
    unsigned int ts:2;          //时钟分频系数
    unsigned int cm:3;          //计数模式
    unsigned int cs:1;          //计数时钟源
    unsigned int tp:1;          //定时器中断极性
    unsigned int tm:2;          //中断信号宽度
    unsigned int reserved:23;
}BF_TIMER_CFG;
typedef union 
{
    unsigned int    ui;
    BF_TIMER_CFG    bf;         //bit defined
}TIMER_CFG;

typedef struct
{
    unsigned int res:1;         //重新计数
    unsigned int pc:1;          //暂停计数
    unsigned int ct:1;          //清除定时器中断
    unsigned int ru:1;          //CC寄存器状态
    unsigned int bpen:1;        //端点使能
    unsigned int reserved:27;
}BF_TIMER_CFG1;
typedef union 
{
    unsigned int    ui;
    BF_TIMER_CFG1   bf;         //bit defined
}TIMER_CFG1;

typedef struct
{
    unsigned int ew:1;          //事件计数模式/看门狗重载模式
    unsigned int wdrsts:1;      //看门狗复位状态
    unsigned int reserved:30;
}BF_TIMER_EW;
typedef union 
{
    unsigned int    ui;
    BF_TIMER_EW     bf;         //bit defined
}TIMER_EW;

typedef struct
{
    unsigned int tsel_clk:1;    //计数时钟选择
    unsigned int trest_en:1;    //输入信号TIMER_RESET_IN使能
    unsigned int tru_en:1;      //输入信号TIMER_RU使能
    unsigned int reserved:29;
}BF_TIMER_CFG0;
typedef union 
{
    unsigned int    ui;
    BF_TIMER_CFG0   bf;         //bit defined
}TIMER_CFG0;

typedef struct
{
    TIMER_CFG       cfg;
    TIMER_CFG1      cfg1;
    TIMER_EW        ew;     //事件/看门狗寄存器
    unsigned int    sc;     //定时器周期寄存器
    unsigned int    cc;     //计数器寄存器
    unsigned int    spwmc;  //PWM周期寄存器
    TIMER_CFG0      cfg0;
}TIMER_PWM_TypeDef;

#define TIMER0                          TIMER0_BASE
#define TIMER1                          TIMER1_BASE
#define TIMER2                          TIMER2_BASE
#define TIMER3                          TIMER3_BASE

#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIMER0) || \
                                   ((PERIPH) == TIMER1) || \
                                   ((PERIPH) == TIMER2) || \
                                   ((PERIPH) == TIMER3)))
    
#define TIM_NUMBER(TIMx)    (TIMx - TIMER0)/0x1000
    
#define TIMER_CLK_DIVIDED_NO            0
#define TIMER_CLK_DIVIDED_2             1
#define TIMER_CLK_DIVIDED_4             2
#define TIMER_CLK_DIVIDED_16            3

#define IS_TIM_CKD_DIV(DIV) (((DIV) == TIMER_CLK_DIVIDED_NO) || \
                             ((DIV) == TIMER_CLK_DIVIDED_2) || \
                             ((DIV) == TIMER_CLK_DIVIDED_4) || \
                             ((DIV) == TIMER_CLK_DIVIDED_16))

#define COUNTER_MODE_SINGLE             0
#define COUNTER_MODE_AUTO               1
#define COUNTER_MODE_FREE               2
#define COUNTER_MODE_EVENT              3
#define COUNTER_MODE_WDT1               4
#define COUNTER_MODE_WDT2               5
#define COUNTER_MODE_WDT3               6
#define COUNTER_MODE_RESERVED           7

#define IS_TIM_OCM(MODE) (((MODE) == COUNTER_MODE_SINGLE) || \
                          ((MODE) == COUNTER_MODE_AUTO) || \
                          ((MODE) == COUNTER_MODE_FREE) || \
                          ((MODE) == COUNTER_MODE_EVENT)|| \
                          ((MODE) == COUNTER_MODE_WDT1) || \
                          ((MODE) == COUNTER_MODE_WDT2) ||	\
                          ((MODE) == COUNTER_MODE_WDT3))

#define TIMER_CLK_PCLK                  0
#define TIMER_CLK_EXTCLK                1
#define TIMER_CLK_CASCADE               2

#define IS_TIM_CLK_SOURCE(SOURCE) (((SOURCE) == TIMER_CLK_PCLK) || \
                          ((SOURCE) == TIMER_CLK_EXTCLK) || \
                          ((SOURCE) == TIMER_CLK_CASCADE))




typedef struct
{
    unsigned int period;//定时周期,最小定时周期为1毫秒
    unsigned int duty;  //pwm占空比,最小为1毫秒
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
} TIM_InitTypeDef;

void timer_start(int TIMx,TIM_InitTypeDef* initStruct);

void timer_stop(int TIMx);

void timer_restart(int TIMx);

unsigned int timer_elapse(int TIMx);

void timer_test_startOneTimer(int TIMx,unsigned int ms);
void timer_init(int TIMx,TIM_InitTypeDef* initStruct);
void TIMx_us(int TIMx,unsigned int us);
void TIMx_ms(int TIMx,unsigned int ms);
#ifdef __cplusplus
}
#endif
#endif
