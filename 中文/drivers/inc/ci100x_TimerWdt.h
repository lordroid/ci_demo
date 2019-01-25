/**
  ******************************************************************************
  * @文件    ci100x_TimerWdt.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的timer_watchdog模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __TIMERWDT_H
#define __TIMERWDT_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "smt.h"

enum
{
    TWDG_DISABLE = 0,
    TWDG_ENABLE
};

enum
{
    TWDG_PCLK = 0,
    TWDG_EXT_CLK
};

enum
{
    TWDG_NORMAL_MODE = 0,
    TWDG_PRE_WARNING
};

typedef struct
{
    unsigned int glb_en     :1;         //watchdog timer enable
    unsigned int clk_sel    :1;         //时钟选择
    unsigned int mode       :1;         //工作模式选择
    unsigned int halt_en    :1;         //halt功能使能
    unsigned int srvWin_en  :1;         //window 功能开关，高有效
    unsigned int rest_en    :1;         //复位产生使能，高有效
    unsigned int int_en     :1;         //INT产生使能，高有效
    unsigned int reserved   :25;
}BF_TWDG_CONTROL;
typedef union 
{
    unsigned int      ui;
    BF_TWDG_CONTROL   bf;
}_TWDG_CONTROL;

typedef struct
{
    unsigned int scale      :16;        //分频系数，(0/1:不分频)
    unsigned int reserved   :16;
}BF_TWDG_SCALE;
typedef union 
{
    unsigned int      ui;
    BF_TWDG_SCALE     bf;
}_TWDG_SCALE;

typedef struct
{
    unsigned int cnt_ext    :1;         //counter计数越界
    unsigned int reserved   :31;
}BF_TWDG_STATUS;
typedef union 
{
    unsigned int      ui;
    BF_TWDG_STATUS   bf;
}_TWDG_STATUS;

typedef struct
{
    _TWDG_CONTROL   WDT_CTR;
    _TWDG_SCALE 	WDT_SCALE;
    unsigned int 	WDT_SRV;
    unsigned int 	WDT_WLB;
    unsigned int 	WDT_WUB;
    unsigned int 	WDT_LOCK;
    unsigned int 	WDT_COUNT;
    _TWDG_STATUS 	WDT_STATUS;
}TWDG_TypeDef;

#define	TWDG	((TWDG_TypeDef *)WWDG_BASE)


typedef struct
{
    unsigned int scale;//分频系数，0和1表示不分频
    unsigned int lower;
    unsigned int upper;
    unsigned int mode;
}TWDG_InitTypeDef;

void WWDG_IRQHandler(void);

void TWDG_init( TWDG_InitTypeDef *initStruct );

void TWDG_disable(void);

unsigned int TWDG_counter(void);

void TWDG_service(void);

//void TWDG_test(int mode,int ms_windowLowper,int ms_windowUpper);
#ifdef __cplusplus
}
#endif

#endif
