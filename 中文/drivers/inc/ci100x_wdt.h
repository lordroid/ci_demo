/**
  ******************************************************************************
  * @文件    ci100x_wdt.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的watchdog模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __WDT_H
#define __WDT_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "smt.h"
enum
{
    WDT_IRQ_DISABLE=0,
    WDT_IRQ_ENABLE    
};

enum
{
    WDT_RESET_DISABLE=0,
    WDT_RESET_ENABLE    
};

typedef struct
{
    unsigned int int_en:1;          //中断使能
    unsigned int res_en:1;          //看门狗服务输出使能
    unsigned int reserved:30;
}BF_WDOG_CONTROL;
typedef union 
{
    unsigned int      ui;
    BF_WDOG_CONTROL     bf;
}_WDOG_CONTROL;

typedef struct
{
    unsigned int    WdogLoad;
    unsigned int    WdogValue;
    _WDOG_CONTROL     WdogControl;
    unsigned int    WdogIntClr;
    unsigned int    WdogRIS;
    unsigned int    WdogMIS;
}IWDG_TypeDef;

/*******************************************************************************
**  看门狗外设标识寄存器定义(Peripheral identification registers)
*******************************************************************************/
typedef struct
{
    unsigned int PartNumber:12;
    unsigned int Designer:8;
    unsigned int Revision:4;
    unsigned int Configuration:8;
}BF_IWDGID;
typedef union 
{
    unsigned int    ui;
    BF_IWDGID         bf;
}IWDG_PID_TypeDef;

typedef struct
{
    unsigned int PartNumber0    :8;
    unsigned int reserved       :24;
}BF_IWDG_PID0;
typedef union 
{
    unsigned int       ui;
    BF_IWDG_PID0         bf;
}IWDG_PID0;

typedef struct
{
    unsigned int PartNumber1    :4;
    unsigned int Designer0      :4;
    unsigned int reserved       :24;
}BF_IWDG_PID1;
typedef union 
{
    unsigned int       ui;
    BF_IWDG_PID1         bf;
}IWDG_PID1;

typedef struct
{    
    unsigned int Designer1      :4;
    unsigned int Revision       :4;
    unsigned int reserved       :24;
}BF_IWDG_PID2;
typedef union 
{
    unsigned int       ui;
    BF_IWDG_PID2         bf;
}IWDG_PID2;

typedef struct
{    
    unsigned int Configuration  :4;
    unsigned int reserved       :24;
}BF_IWDG_PID3;
typedef union 
{
    unsigned int       ui;
    BF_IWDG_PID3         bf;
}IWDG_PID3;

typedef struct
{
    IWDG_PID0   WdogPeriphID0;
    IWDG_PID1   WdogPeriphID1;
    IWDG_PID2   WdogPeriphID2;
    IWDG_PID3   WdogPeriphID3;
    unsigned int WdogPCellID0;
    unsigned int WdogPCellID1;
    unsigned int WdogPCellID2;
    unsigned int WdogPCellID3;
}IWDG_ID_TypeDef;

#define IWDG        ((IWDG_TypeDef *)IWDG_BASE)
#define IWDG_PID    ((IWDG_ID_TypeDef *)(IWDG_BASE  + 0x0fe0))
#define WDOGLOCK    *(volatile unsigned int *)(IWDG_BASE + 0x0c00)



     





void IWDG_init(unsigned int val,int irq_en,int res_en);


void IWDG_feed_dog(void);

void IWDG_disable(void);



#ifdef __cplusplus
}
#endif

#endif
