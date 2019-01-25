/**
  ******************************************************************************
  * @文件    ci100x_wdt.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的watchdog模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 

#include "ci100x_wdt.h"
#include "misc.h"
	

#ifndef FPCLK
#define FPCLK   50000000L
#endif
/**
  * @功能:IWDG初始化配置函数
  * @注意:无      
  * @参数:    load_value - 看门狗计数器初始值
  *           irq_en     - 中断使能标志
  *                         WDT_IRQ_ENABLE  :   中断使能
  *                         WDT_IRQ_DISABLE :   不产生中断
  *           res_en     - 复位信号输出使能标志
  *                         WDT_RESET_ENABLE : 复位信号输出使能
  *                         WDT_RESET_DISABLE: 复位信号输出关闭
  * @返回值:无
  */
void IWDG_init(unsigned int load_value,int irq_en,int res_en)
{    
    WDOGLOCK = 0x1ACCE551;                      //enable write-access all registers 
    IWDG->WdogLoad = load_value;				//set value of counter, if Zero interrupt generate immediately    
    IWDG->WdogControl.bf.int_en    = irq_en;
    IWDG->WdogControl.bf.res_en    = res_en;
    IWDG->WdogIntClr = 0x1;
    WDOGLOCK = 0x0;                             //disable write-access all registers   
}
/**
  * @功能:看门狗初喂狗函数
  * @注意:无      
  * @参数:无
  * @返回值:无
  */
void IWDG_feed_dog()
{
    WDOGLOCK = 0x1ACCE551;
    IWDG->WdogControl.bf.int_en = WDT_IRQ_DISABLE;
    IWDG->WdogControl.bf.int_en = WDT_IRQ_ENABLE;
    WDOGLOCK = 0x0;
}

/**
  * @功能:关闭IWDG
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void IWDG_disable()
{
    WDOGLOCK = 0x1ACCE551;
    IWDG->WdogControl.bf.int_en = WDT_IRQ_DISABLE;
    IWDG->WdogControl.bf.res_en = WDT_RESET_DISABLE;
    WDOGLOCK = 0x0;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/