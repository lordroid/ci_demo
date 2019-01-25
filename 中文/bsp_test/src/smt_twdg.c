

#if(TWDG_TEST_ENABLE == 1)
#include "ci100x_TimerWdt.h"
#include "misc.h"

#define TWDG_CLK 15000000

#ifndef SCU_CONFIG
static void TWDG_SCU_resetConfig()
{
    volatile unsigned int tmp;
    *(volatile unsigned int *)(SCU_BASE + 0x40) = 0x51AC0FFE;
    tmp = *(volatile unsigned int *)(SCU_BASE + 0x30);
    tmp &= ~(0x3 << 6);
    tmp |= (0x2 << 6);
    *(volatile unsigned int *)(SCU_BASE + 0x30) = tmp;
    *(volatile unsigned int *)(SCU_BASE + 0x40) =0;
}
#endif
/****************************************************************************
** 函数  名： TWDG_test
** 功    能： 初始化时间看门狗
** 入口参数： mode       -  看门狗工作模式(TWDG_PRE_WARNING / TWDG_NORMAL_MODE)
**            ms_windowLowper     - 窗口下边界，单位ms
**            ms_windowUpper      - 窗口上边界，单位ms
**
** 返回  值： 无
**
****************************************************************************/
void TWDG_test(int mode,int ms_windowLowper,int ms_windowUpper)
{
    TWDG_InitTypeDef initStruct;
    
    //initStruct.lower    = 0x500000;
    //initStruct.upper    = 0x500f00;
    initStruct.mode = mode; //TWDG_PRE_WARNING / TWDG_NORMAL_MODE
    initStruct.scale = 1;
    
    initStruct.scale = (initStruct.scale > 1)?initStruct.scale:1;
    initStruct.lower = TWDG_CLK/(initStruct.scale * 1000)*ms_windowLowper;
    initStruct.upper = TWDG_CLK/(initStruct.scale * 1000)*ms_windowUpper;
    
    TWDG_disable();
    TWDG_init( &initStruct );
    
    #ifndef SCU_CONFIG
    TWDG_SCU_resetConfig();     //要产生复位，必须配置SCU中，WWDG复位开关
    #endif
    irq_en(0);                  //要产生中断，必须使能TWDG中断向量
}
#endif
