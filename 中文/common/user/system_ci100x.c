/**
  ******************************************************************************
  * @文件    system_ci100x.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件用于chipintelli公司的CI100X芯片的系统时钟初始化配置.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
#include "smt.h"
extern "C" void sdram_init(void);
#define CI100x_SDRAM_CLK    (200UL)
#define CI100x_SRAM_BASE     (0x1FFE8000)
#define VECT_TAB_OFFSET        (0x00)
void SystemInit(void)
{
    volatile unsigned int delay=0x3FFF;
      /*NVIC关闭外部中断*/
    NVIC->ICER[0]=0xFFFFFFFF;
    for(int i = 0;i<= 0x1C;i += 4)
    {
        /*所有IRQ优先级配置为最低*/
        *(volatile unsigned  int*)(0xe000e400+i) = 0xFFFFFFFF;
    }
    /*重定向中断向量表偏移*/
    SCB->VTOR = CI100x_SRAM_BASE | VECT_TAB_OFFSET;
    
    /*SCU中设置外设默认时钟分频*/
    SCU->CKCFG_LOCK = 0x51ac0ffe;
    SCU->RSTCFG_LOCK = 0x51ac0ffe;
    SCU->CLKDIV_PAPAM0 = (0xc0 << 22)|(0x18 << 16)|(0x2 << 8)|(0x2 << 0);
    SCU->CLKDIV_PAPAM1 = (0x4 << 24)|(0x10 << 16)|(0xF << 8)|(0xc << 0);
    SCU->CLKDIV_PAPAM2 = 0x10101010;
    SCU->CLKDIV_PAPAM3 = 0x08080504;//change spi clk from 66.6Mhz(0x08080503;) to 50Mhz (?0x08080504)
    SCU->CLKDIV_PAPAM4 = 0x18080808;
    SCU->CLKDIV_PAPAM5 = 0x01A01818;
    
    for(volatile unsigned int i=0x3FF;i;i--);
    SCU->SYS_CLKDIV_EN = 0xFFFFFF;

    /*设置复位模式的默认值*/
    SCU->SOFT_SYSRST_CTRL = 0x0;
    
    /*设置I2S 时钟默认值*/
    SCU->IIS0_CLK_CFG = 0x10A;
    SCU->IIS1_CLK_CFG = 0x10A;
    SCU->IIS2_CLK_CFG = 0x10A;
    
    /*设置SCU 中断屏蔽寄存器默认值*/
    SCU->SCU_INT_MASK = 0x0;
    /*设置低功耗唤醒状态寄存器默认值*/
    SCU->INT_STATE = 0x0;
    /*设置滴答时钟分频系数默认值*/
    SCU->M4_STCALIB = 0x1387F;
    /*设置中断屏蔽寄存器默认值*/
    SCU->WAKE_UP_MASK = 0x0;
    /*设置滤波参数默认值*/
    SCU->KEY_FILTER  = 0xFFFF;
    SCU->EXT0_FILTER = 0xFFFF;
    SCU->EXT1_FILTER = 0xFFFF;
    SCU->EXT2_FILTER = 0xFFFF;
    SCU->EXT3_FILTER = 0xFFFF;
    SCU->EXT4_FILTER = 0xFFFF;

    /*PWM 复用默认值*/
    SCU->PWM_REUSE_CFG = 0x0;
    SCU->IO_REUSE_CFG = 0X0;
    SCU->IO_REUSE_CFG1 = 0X0;
    SCU->FUNC_TEST_SEL = 0X0;
    SCU->SDRAM_DLL_CTRL = 0X300001;
    //SCU->CKCFG_LOCK = 0;
    //SCU->RSTCFG_LOCK = 0;

    /*复位外设*/
    SCU->RSTCFG_LOCK = 0x51ac0ffe;
    while(!(SCU->RSTCFG_LOCK));
    SCU->SOFT_PRERST_CTRL0 = 0x0;
    SCU->SOFT_PRERST_CTRL1 = 0x0;
    for(volatile unsigned int i=0x3FFF;i;i--);
    SCU->SOFT_PRERST_CTRL0 = 0xFFFFFFFF;
    SCU->SOFT_PRERST_CTRL1 = 0xFFFFFFFF;
    //SCU->RSTCFG_LOCK=0;
    
    /* 浮点(FPU) 配置,set CP10 and CP11 Full Access*/
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    #endif
    
    /*设置模拟IO成数字功能*/
    SCU->AD_IO_REUSE_CFG = 0;

    /*关闭外设时钟,*/    
    SCU->CKCFG_LOCK = 0x51ac0ffe;
    while(!(SCU->CKCFG_LOCK));    
    SCU->PRE_CLKGATE0 = 0x0;
    SCU->PRE_CLKGATE1 &=~(0xFFFFFFU<<0);
    
    /*开TRACECKEN、SDRAM时钟,设置系统默认时钟门控*/
    SCU->PRE_CLKGATE1  |=(0x3 << 25);
    SCU->SYS_CLKGATE = 0x380;
    
    /* 时钟配置,EXT_CLKIN=12Mhz,PLL = 400 Mhz*/  
    SCU->CKCFG_LOCK = 0x51ac0ffe;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= (1UL << 3);
    SCU->SYS_PLL_REG = (100 << 0) | (3 << 8) | (0 << 12);//(100 << 0) | (3 << 8) | (0 << 12); 
    SCU->SYS_CLKDIV_EN |= (1UL << 21);
    while(delay--);
    SCU->SYS_CTRL &= ~(1UL << 3);
    //SCU->CKCFG_LOCK = 0;
    delay=0xFFF;
    while(delay--);
 
    /* SDRAM 初始化*/
    sdram_init();
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/