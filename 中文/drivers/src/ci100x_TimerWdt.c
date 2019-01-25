/**
  ******************************************************************************
  * @文件    ci100x_TimerWdt.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的窗口看门狗模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#include "ci100x_TimerWdt.h"

#include "misc.h"
#include "ci100x_scu.h"


#ifndef TWDG_CLK
#define TWDG_CLK   15000000L
#endif

/**
  * @功能:初始化配置时间看门狗
  * @注意:无      
  * @参数:initStruct     - 初始化配置结构体
  * @返回值:无
  */
void TWDG_init(TWDG_InitTypeDef *initStruct)
{
    TWDG->WDT_LOCK              = 0x51acce55;
    TWDG->WDT_SCALE.bf.scale    = initStruct->scale;
    TWDG->WDT_WLB               = initStruct->lower;
    TWDG->WDT_WUB               = initStruct->upper;
    TWDG->WDT_CTR.bf.glb_en     = TWDG_ENABLE;
    TWDG->WDT_CTR.bf.clk_sel    = TWDG_PCLK;
    TWDG->WDT_CTR.bf.mode       = initStruct->mode;
    TWDG->WDT_CTR.bf.halt_en    = TWDG_ENABLE;
    TWDG->WDT_CTR.bf.srvWin_en  = TWDG_ENABLE;
    TWDG->WDT_CTR.bf.rest_en    = TWDG_ENABLE;
    TWDG->WDT_CTR.bf.int_en     = TWDG_ENABLE;
    TWDG->WDT_LOCK  = 0;    
}

/**
  * @功能:关闭时间看门狗
  * @注意:无      
  * @参数:initStruct     - 初始化配置结构体
  * @返回值:无
  */
void TWDG_disable(void)
{
    TWDG->WDT_LOCK          = 0x51acce55;
    TWDG->WDT_CTR.bf.glb_en = 0;
    TWDG->WDT_LOCK          = 0;
}

/**
  * @功能:获取counter值
  * @注意:无      
  * @参数:initStruct     - 初始化配置结构体
  * @返回值:counter值
  */
unsigned int TWDG_counter(void)
{
    int value;
    
    TWDG->WDT_LOCK          = 0x51acce55;
    value   = TWDG->WDT_COUNT;
    TWDG->WDT_LOCK          = 0;    
    
    return value;
}

/**
  * @功能:看门狗service
  * @注意:无      
  * @参数:无
  * @返回值:无
  */
void TWDG_service()
{
    TWDG->WDT_LOCK  = 0x51acce55;
    TWDG->WDT_SRV   = 0xabadc0de;
    TWDG->WDT_LOCK  = 0;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/