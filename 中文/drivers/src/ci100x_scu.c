/**
  ******************************************************************************
  * @文件    ci100x_scu.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的系统控制单元模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
#include "ci100x_scu.h"

#define CKCFG_UNLOCK_MAGIC  0x51ac0ffe
#define RSTCFG_UNLOCK_MAGIC 0x51ac0ffe
#define SOFTWARE_RESET_KEY   0xdeadbeef

#define DIV_MASK1   0x1
#define DIV_MASK2   0x3
#define DIV_MASK3   0x7
#define DIV_MASK4   0xf
#define DIV_MASK5   0x1f
#define DIV_MASK6   0x3f
#define DIV_MASK7   0x7f
#define DIV_MASK8   0xff
#define DIV_MASK9   0x1ff
/**
  * @功能:延迟函数
  * @注意:无       
  * @参数:cnt 延迟时间 毫秒
  * @返回值:无
  */
static void _delay(unsigned int cnt)
{
    unsigned int i,j,sum = 0;
    for(i = 0; i < cnt; i++)
    {
    	for(j = 0; j < 0X3F; j++)
        {
        	sum += j % 10;
     	}
    }
}

/**
  * @功能:设置PLL
  * @注意: M= pll_dm; N= pll_dn; OD = 2^(pll_od[0] + 2Xpll_od[1]); CKOUT=CKIN*M/n/OD 
  *			1Mhz<=XIN/N<=25Mhz        200Mhz<=clk_outXOD <=600Mhz   M>=2   N>=2
  * @参数:pll_od 寄存器OD值，pll_dn寄存器DN值，pll_dm 寄存器DM值
  * @返回值:RETRUN_OK 返回成功   PARA_ERROR 参数错误
  */
int Scu_SetPLL(int pll_od,int pll_dn,int pll_dm)
{
   if((pll_dm > 0x3f) || (pll_dm < 0))
        return PARA_ERROR;
   if((pll_dn > 15) || (pll_dn < 0))
        return PARA_ERROR;
   if((pll_od > 3) || (pll_od < 0))
        return PARA_ERROR;
   SCU->SYS_PLL_REG |= ((pll_od << 12) | (pll_dn << 8) | (pll_dm << 0));
   return RETRUN_OK;
}
/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为384Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_384(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = 0x240;   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为320Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_320(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = (80 << 0) | (3 << 8) | (0 << 12);   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:外部晶振为12Mhz时
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_CKIN(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    _delay(2);
    Scu_AllPreipheralclkclose();
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为400Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_400(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = (100 << 0) | (3 << 8) | (0 << 12);   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为240Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_240(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = (60 << 0) | (3 << 8) | (0 << 12);   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为200Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_200(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = (50 << 0) | (3 << 8) | (0 << 12);   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:在外部晶振为12Mhz时，把PLL设置为160Mhz
  * @注意:无       
  * @参数:无
  * @返回值:RETRUN_OK 配置完成
  */
int Scu_SetPll_160(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
    SCU->SYS_CTRL |= PLL_CONFIG_BEFORE;
    SCU->SYS_PLL_REG = (80 << 0) | (3 << 8) | (1 << 12);   
    SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    _delay(2);
    SCU->SYS_CTRL &= ~(0x1 << 3);
    //SCU->CKCFG_LOCK = 0;
    return RETRUN_OK;
}

/**
  * @功能:解锁控制寄存器
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Unlock_Ckconfig(void)
{
    SCU->CKCFG_LOCK = CKCFG_UNLOCK_MAGIC;
    while(!(SCU->CKCFG_LOCK));
}

/**
  * @功能:解锁复位寄存器
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Unlock_Rstconfig(void)
{
    SCU->RSTCFG_LOCK = RSTCFG_UNLOCK_MAGIC;
    while(!(SCU->RSTCFG_LOCK));
}
/**
  * @功能:锁定配置寄存器
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Lock_Ckconfig(void)
{
    //SCU->CKCFG_LOCK = 0x0;
}
/**
  * @功能:锁定复位寄存器
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Lock_Rstconfig(void)
{
    //SCU->RSTCFG_LOCK = 0x0;
}
/**
  * @功能:设置外设时钟分频
  * @注意:无       
  * @参数:device_base，需要设置的外设基址 div_num，分频输
  * @返回值: PARA_ERROR: 参数错误 RETRUN_OK, 配置完成
  */
int Scu_Setdiv_Parameter(unsigned int device_base,unsigned int div_num)
{
    Unlock_Ckconfig();
    if(device_base == AHB_BASE)
    {
        SCU->CLKDIV_PAPAM0 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM0 |= ((div_num  & DIV_MASK6 ) << 0);
    }    
    else if(device_base == APB_BASE)
    {
        SCU->CLKDIV_PAPAM0 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM0 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == ADC_BASE)
    {
        SCU->CLKDIV_PAPAM1 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM1 |= ((div_num & DIV_MASK6) << 0);
    }
    else if(device_base == ASR_BASE)
    {
        SCU->CLKDIV_PAPAM1 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM1 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == IWDG_BASE)
    {
        SCU->CLKDIV_PAPAM1 &= ~(DIV_MASK6 << 16);
        SCU->CLKDIV_PAPAM1 |= ((div_num & DIV_MASK6) << 16);
    }
    else if(device_base == SD_DIV_BASE)
    {
        SCU->CLKDIV_PAPAM1 &= ~(DIV_MASK6 << 24);
        SCU->CLKDIV_PAPAM1 |= ((div_num & DIV_MASK6) << 24);
    }
    else if(device_base == TIMER0_BASE)
    {
        SCU->CLKDIV_PAPAM2 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM2 |= ((div_num & DIV_MASK6) << 0);
    }
    else if(device_base == TIMER1_BASE)
    {
        SCU->CLKDIV_PAPAM2 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM2 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == TIMER2_BASE)
    {
        SCU->CLKDIV_PAPAM2 &= ~(DIV_MASK6 << 16);
        SCU->CLKDIV_PAPAM2 |= ((div_num & DIV_MASK6) << 16);
    }
    else if(device_base == TIMER3_BASE)
    {
        SCU->CLKDIV_PAPAM2 &= ~(DIV_MASK6 << 24);
        SCU->CLKDIV_PAPAM2 |= ((div_num & DIV_MASK6) << 24);
    }
    else if(device_base == SPI0_BASE)
    {
        SCU->CLKDIV_PAPAM3 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM3 |= ((div_num & DIV_MASK6) << 0);
    }
    else if(device_base == DNN_DIV_BASE)
    {
        SCU->CLKDIV_PAPAM3 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM3 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == TARCE_CLOCK_DIV)
    {
        SCU->CLKDIV_PAPAM3 &= ~(DIV_MASK6 << 16);
        SCU->CLKDIV_PAPAM3 |= ((div_num & DIV_MASK6) << 16);
    }
    else if(device_base == UART0_BASE)
    {
        SCU->CLKDIV_PAPAM3 &= ~(DIV_MASK6 << 24);
        SCU->CLKDIV_PAPAM3 |= ((div_num & DIV_MASK6) << 24);
    }
    else if(device_base == UART1_BASE)
    {
        SCU->CLKDIV_PAPAM4 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM4 |= ((div_num & DIV_MASK6) << 0);
    }
    else if(device_base == UART2_BASE)
    {
        SCU->CLKDIV_PAPAM4 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM4 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == UART3_BASE)
    {
        SCU->CLKDIV_PAPAM4 &= ~(DIV_MASK6 << 16);
        SCU->CLKDIV_PAPAM4 |= ((div_num & DIV_MASK6) << 16);
    }
    else if(device_base == IIS0_BASE)
    {
        SCU->CLKDIV_PAPAM4 &= ~(DIV_MASK6 << 24);
        SCU->CLKDIV_PAPAM4 |= ((div_num & DIV_MASK6) << 24);
    }
    else if(device_base == IIS1_BASE)
    {
        SCU->CLKDIV_PAPAM5 &= ~(DIV_MASK6 << 0);
        SCU->CLKDIV_PAPAM5 |= ((div_num & DIV_MASK6) << 0);
    }
    else if(device_base == IIS2_BASE)
    {
        SCU->CLKDIV_PAPAM5 &= ~(DIV_MASK6 << 8);
        SCU->CLKDIV_PAPAM5 |= ((div_num & DIV_MASK6) << 8);
    }
    else if(device_base == IR_BASE)
    {
        SCU->CLKDIV_PAPAM5 &= ~(DIV_MASK9 << 16);
        SCU->CLKDIV_PAPAM5 |= ((div_num &DIV_MASK9) << 16);
    }
    else 
    {
        Lock_Ckconfig();
        return PARA_ERROR;
    }
    Lock_Ckconfig();
    return RETRUN_OK;
}
/**
  * @功能:设置时钟开关
  * @注意:无       
  * @参数:device_base，需要设置的外设基址 gate:  0 关闭    1 打开
  * @返回值: PARA_ERROR: 参数错误 RETRUN_OK, 配置完成
  */
int Scu_SetClkgate(unsigned int device_base,int gate)
{
    Unlock_Ckconfig();
    if(device_base == CSRAM_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 1);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 1);
    }
    else if(device_base == SRAM0_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 2);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 2);
    }
    else if(device_base == SRAM1_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 3);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 3);
    }
    else if(device_base == STCLK_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 5);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 5);
    }
    else if(device_base == CM4CORE_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 6);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 6);
    }
    else if(device_base == CM4_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 7);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 7);
    }
    else if(device_base == SLEEPING_MODE_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 8);
        SCU->SYS_CLKGATE |= ((0x1 & gate) << 8);
    }
    else if(device_base == SLEEPDEEP_MODE_GATE)
    {
        SCU->SYS_CLKGATE &= ~(0x1 << 9);
        SCU->SYS_CLKGATE |= ((0x1 << gate) << 9);
    }
    else
    {
        Lock_Ckconfig();
        return PARA_ERROR;
    }
    Lock_Ckconfig();
    return RETRUN_OK;
}
/**
  * @功能:设置时钟开关
  * @注意:无       
  * @参数:device_base，需要设置的外设基址 gate:  0 关闭    1 打开
  * @返回值: PARA_ERROR: 参数错误 RETRUN_OK, 配置完成
  */
int Scu_SetDeviceGate(unsigned int device_base, int gate)
{
    Unlock_Ckconfig();
    if(device_base == IIS_DMA_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 0);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 0);
    }
    else if(device_base == CAN_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 1);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 1);
    }
    else if(device_base == DMA_CTRL_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 2);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 2);
    }
    else if(device_base == xPWM_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 3);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 3);
    }
    else if(device_base == ADC_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 7);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 7);
    }
    else if(device_base == SPI0_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 12);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 12);
    }
    else if(device_base == SPI1_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 13);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 13);
    }
    else if(device_base == SPI2_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 14);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 14);
    }
    else if(device_base == GPIO0_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 20);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 20);
    }
    else if(device_base == GPIO1_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 21);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 21);
    }
    else if(device_base == IIC0_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 22);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 22);
    }
    else if(device_base == IIC1_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 23);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 23);
    }
    else if(device_base == WWDG_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 24);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 24);
    }
    else if(device_base == IWDG_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 25);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 25);
    }
    else if(device_base == ASR_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1f << 26);
        SCU->PRE_CLKGATE0 |= (((0x1 & gate) << 26) | ((0x1 & gate) << 27) | ((0x1 & gate) << 28) | ((0x1 & gate) << 29) \
                                               | ((0x1 & gate) << 30));
    }
    else if(device_base == IR_BASE)
    {
        SCU->PRE_CLKGATE0 &= ~(0x1 << 31);
        SCU->PRE_CLKGATE0 |= ((0x1 & gate) << 31);
    }
    else if(device_base == PWM0_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 0);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 0);
    }
    else if(device_base == PWM1_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 1);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 1);
    }
    else if(device_base == PWM2_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 2);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 2);
    }
    else if(device_base == PWM3_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 3);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 3);
    }
    else if(device_base == PWM4_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 4);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 4);
    }
    else if(device_base == PWM5_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 5);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 5);
    }
    else if(device_base == PWM6_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 6);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 6);
    }
    else if(device_base == PWM7_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 7);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 7);
    }
    else if(device_base == PWM8_GATE_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 8);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 8);
    }
    else if(device_base == TIMER3_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 8);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 8);
    }
    
    else if(device_base == TIMER0_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 12);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 12);
    }
    else if(device_base == TIMER1_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 13);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 13);
    }
    else if(device_base == TIMER2_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 14);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 14);
    }
	else if(device_base == IIS0_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 <<20);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 20);
    }
    else if(device_base == UART0_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 <<16);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 16);
    }    
	else if(device_base == UART1_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 <<17);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 17);
    }
	else if(device_base == UART2_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 <<18);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 18);
    }
	else if(device_base == UART3_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 <<19);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 19);
    }
    else if(device_base == IIS1_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 21);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 21);
    }
    else if(device_base == IIS2_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 22);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 22);
    }
    else if(device_base == SDRAM_CTRL_BASE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 25);
        SCU->PRE_CLKGATE1 |= ((0x1 & gate) << 25);
    }
    else if(device_base == TRACE_GATE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 26);
        SCU->PRE_CLKGATE1|= ((0x1 & gate) << 26);
    }
	else if(device_base == VAD_GATE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 27);
        SCU->PRE_CLKGATE1|= ((0x1 & gate) << 27);
    }
	else if(device_base == FE_GATE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 28);
        SCU->PRE_CLKGATE1|= ((0x1 & gate) << 28);
    }
	else if(device_base == DNN_GATE)
    {
        SCU->PRE_CLKGATE1 &= ~(0x1 << 29);
        SCU->PRE_CLKGATE1|= ((0x1 & gate) << 29);
    }
    else
    {
        Lock_Ckconfig();
        return PARA_ERROR;
    }
    Lock_Ckconfig();
    return RETRUN_OK;
}
/**
  * @功能:设置watchdog计数和复位受到CPU HALTED信号控制
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_SetWDG_Halt(void)
{
    Unlock_Ckconfig();
    SCU->PRE_CLKGATE0 |= (0x1 << 15);
    Lock_Ckconfig();
}

/**
  * @功能:设置watchdog计数和复位是不受CPU HALTED信号控制
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
  void Scu_CleanWDG_Halt(void)
{
    Unlock_Ckconfig();
    SCU->PRE_CLKGATE0 &= ~(0x1 << 15);
    Lock_Ckconfig();
}

/**
  * @功能:软件复位系统
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_SoftwareRst_System(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 12);
    SCU->SOFT_SYSRST_CTRL |= (0x2 << 12);
    SCU->SOFT_RST_PAPAM = SOFTWARE_RESET_KEY;
    Lock_Rstconfig();
}

/**
  * @功能:软件复位系统总线
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_SoftwareRst_SystemBus(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 12);
    SCU->SOFT_SYSRST_CTRL |= ( 0x3 << 12);
    SCU->SOFT_RST_PAPAM = SOFTWARE_RESET_KEY;
    Lock_Rstconfig();
}

/**
  * @功能:配置wwatchdog复位时复位系统
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_Wwdg_RstSys_Config(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 6);
    SCU->SOFT_SYSRST_CTRL |= (0x2 << 6);
    Lock_Rstconfig();
}

/**
  * @功能:配置wwatchdog复位时复位系统总线
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_Wwdg_RstSysBus_Config(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 6);
    SCU->SOFT_SYSRST_CTRL |= (0x3 << 6);
    Lock_Rstconfig();
}

/**
  * @功能:配置iwatchdog复位时复位系统
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_Iwdg_RstSys_Config(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 4);
    SCU->SOFT_SYSRST_CTRL |= (0x2 << 4);
    Lock_Rstconfig();
}

/**
  * @功能:配置wwatchdog复位时复位系统总线
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_Iwdg_RstSysBus_Config(void)
{
    Unlock_Rstconfig();
    SCU->SOFT_SYSRST_CTRL &= ~(0x3 << 4);
    SCU->SOFT_SYSRST_CTRL |= (0x3 << 4);
    Lock_Rstconfig();
}

/**
  * @功能:配置外设复位
  * @注意:配合Scu_Setdevice_ResetRelease  使用，先reset,然后release，外设复位完成     
  * @参数:device_base 设备基地址
  * @返回值:PARA_ERROR: 参数错误   RETRUN_OK: 配置完成
  */
int Scu_Setdevice_Reset(unsigned int device_base)
{
    Unlock_Rstconfig();
    if(device_base == IIS_DMA_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 0);
    }
    else if(device_base == CAN_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 1);
    }
    else if(device_base == DMA_CTRL_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 2);
    }
    else if(device_base == xPWM_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 3);
    }
    else if(device_base == ADC_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 4);
    }
    else if(device_base == PWM0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 5);
    }
    else if(device_base == PWM1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 5);
    }
    else if(device_base == PWM2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 6);
    }
    else if(device_base == PWM3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 6);
    }
    else if(device_base == PWM4_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 7);
    }
    else if(device_base == PWM5_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 7);
    }
    else if(device_base == PWM6_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 8);
    }
    else if(device_base == PWM7_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 8);
    }
    else if(device_base == TIMER0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 10);
    }
    else if(device_base == TIMER1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 10);
    }
    else if(device_base == TIMER2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 11);
    }
    else if(device_base == TIMER3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 11);
    }
    else if(device_base == IR_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 12);
    }
    else if(device_base == SPI0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 13);
    }
    else if(device_base == SPI1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 14);
    }
    else if(device_base == SPI2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 15);
    }
    else if(device_base == UART0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 16);
    }
    else if(device_base == UART1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 17);
    }
    else if(device_base == UART2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 18);
    }
    else if(device_base == UART3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 19);
    }
    else if(device_base == GPIO0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 20);
    }
    else if(device_base == GPIO1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 21);
    }
    else if(device_base == IIC0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 22);
    }    
    else if(device_base == IIC1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 23);
    }
    else if(device_base ==WWDG_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 24);
    }
    else if(device_base == IWDG_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0x1 << 25);
    }
    else if(device_base == ASR_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 &= ~(0xf << 26);
	}else if(device_base ==ASR_AHBBASE){
		SCU->SOFT_PRERST_CTRL0 &=~(1<<30);
	}
    else if(device_base == IIS0_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 &= ~(0x1 << 0);
    }
    else if(device_base == IIS1_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 &= ~(0x1 << 1);
    }
    else if(device_base == IIS2_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 &= ~(0x1 << 2);
    }
    else if(device_base == SDRAM_CTRL_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 &= ~(0x1 << 3);
    }
    else if(device_base == PLL_RST_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 &= ~(0x1 << 4);
    }
    else
    {
        Lock_Rstconfig();
        return PARA_ERROR;
    }
    
    _delay(2);
    Lock_Rstconfig();
    return RETRUN_OK;
}
/**
  * @功能:配置外设复位释放
  * @注意:配合Scu_Setdevice_ResetRelease  使用，先reset,然后release，外设复位完成     
  * @参数:device_base 设备基地址
  * @返回值:PARA_ERROR: 参数错误   RETRUN_OK: 配置完成
  */
int Scu_Setdevice_ResetRelease(unsigned int device_base)
{
    Unlock_Rstconfig();
    if(device_base == IIS_DMA_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 0);
    }
    else if(device_base == CAN_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 1);
    }
    else if(device_base == DMA_CTRL_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 2);
    }
    else if(device_base == xPWM_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 3);
    }
    else if(device_base == ADC_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 4);
    }
    else if(device_base == PWM0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 5);
    }
    else if(device_base == PWM1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 5);
    }
    else if(device_base == PWM2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 6);
    }
    else if(device_base == PWM3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 6);
    }
    else if(device_base == PWM4_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 7);
    }
    else if(device_base == PWM5_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 7);
    }
    else if(device_base == PWM6_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 8);
    }
    else if(device_base == PWM7_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 8);
    }
    else if(device_base == TIMER0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 10);
    }
    else if(device_base == TIMER1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 10);
    }
    else if(device_base == TIMER2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 11);
    }
    else if(device_base == TIMER3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 11);
    }
    else if(device_base == IR_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 12);
    }
    else if(device_base == SPI0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 13);
    }
    else if(device_base == SPI1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 14);
    }
    else if(device_base == SPI2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 15);
    }
    else if(device_base == UART0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 16);
    }
    else if(device_base == UART1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 17);
    }
    else if(device_base == UART2_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 18);
    }
    else if(device_base == UART3_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 19);
    }
    else if(device_base == GPIO0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 20);
    }
    else if(device_base == GPIO1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 21);
    }
    else if(device_base == IIC0_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 22);
    }    
    else if(device_base == IIC1_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 23);
    }
    else if(device_base ==WWDG_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 24);
    }
    else if(device_base == IWDG_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0x1 << 25);
    }
    else if(device_base == ASR_BASE)
    {
        SCU->SOFT_PRERST_CTRL0 |= (0xf << 26);
	}else if(device_base == ASR_AHBBASE){
		SCU->SOFT_PRERST_CTRL0 |=(1<<30);
	}
    else if(device_base == IIS0_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 |= (0x1 << 0);
    }
    else if(device_base == IIS1_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 |= (0x1 << 1);
    }
    else if(device_base == IIS2_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 |= (0x1 << 2);
    }
    else if(device_base == SDRAM_CTRL_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 |= (0x1 << 3);
    }
    else if(device_base == PLL_RST_BASE)
    {
        SCU->SOFT_PRERST_CTRL1 |= (0x1 << 4);
    }
    else
    {
        Lock_Rstconfig();
        return PARA_ERROR;
    }
    
    _delay(2);
    Lock_Rstconfig();
    return RETRUN_OK;
}

/**
  * @功能:设置外设时钟分频参数生效
  * @注意:无       
  * @参数:device_base 外设基地址
  * @返回值:PARA_ERROR: 参数错误  RETRUN_OK: 配置成功
  */
int Scu_SetClkDiv_Enable(unsigned int device_base)
{
    Unlock_Ckconfig();
    if(device_base == AHB_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 0);
    }
    else if(device_base == APB_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 1);
    }
    else if(device_base == STCLK_DIV_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 2);
    }
    else if(device_base == ADC_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 3);
    }
    else if(device_base == ASR_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 4);
    }
    else if(device_base == IWDG_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 5);
    }
    else if(device_base == TIMER0_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 6);
    }
    else if(device_base == TIMER1_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 7);
    }
    else if(device_base == TIMER2_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 8);
    }
    else if(device_base == TIMER3_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 9);
    }
    else if(device_base == SPI0_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 10);
    }
    else if(device_base == SPI1_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 11);
    }
    else if(device_base == SPI2_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 12);
    }
    else if(device_base == UART0_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 13);
    }
    else if(device_base == UART1_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 14);
    }
    else if(device_base == UART2_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 15);
    }
    else if(device_base == UART3_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 16);
    }
    else if(device_base == IIS0_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 17);
    }
    else if(device_base == IIS1_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 18);
    }
    else if(device_base == IIS2_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 19);
    }
    else if(device_base == IR_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 20);
    }
    else if(device_base == PLL_DIV_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 21);
    }
    else if(device_base == TARCE_CLOCK_DIV)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 22);
    }
    else if(device_base == DNN_DIV_BASE)
    {
        SCU->SYS_CLKDIV_EN |= (0x1 << 23);
    }
    else
    {
        Lock_Ckconfig();
        return PARA_ERROR;
    }

    Lock_Ckconfig();
    return RETRUN_OK;
    
}

/**
  * @功能:设置IIS时钟参数
  * @注意:无       
  * @参数:device_base IIS基地址
  * @返回值:PARA_ERROR: 参数错误  RETRUN_OK: 配置成功
  */
int Scu_SetIISCLK_Config(IIS_CLK_CONFIGTypeDef *piis_config)
{
    if(piis_config->device_select == 0)
    {
        SCU->IIS0_CLK_CFG = (piis_config->mclkout_sel << 11) | (piis_config->mclk_sel << 9) \
                                            | (piis_config->sck_wid << 8) | (piis_config->over_sample << 2) \
                                            | (piis_config->model_sel << 0) | (piis_config->sck_ext_inv_sel << 12);
    }
    else if(piis_config->device_select == 1)
    {
        SCU->IIS1_CLK_CFG = (piis_config->mclk_sel << 9) \
                                            | (piis_config->sck_wid << 8) | (piis_config->over_sample << 2) \
                                            | (piis_config->model_sel << 0) | (piis_config->sck_ext_inv_sel << 12);
    }
    else if(piis_config->device_select == 2)
    {
        SCU->IIS2_CLK_CFG = (piis_config->mclk_sel << 9) \
                                            | (piis_config->sck_wid << 8) | (piis_config->over_sample << 2) \
                                            | (piis_config->model_sel << 0) | (piis_config->sck_ext_inv_sel << 12);
    }
    else
    {
        return PARA_ERROR;
    }
    return RETRUN_OK;
}

/**
  * @功能:屏蔽SCU中断
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_MaskScuInterrupt(void)
{
    SCU->SCU_INT_MASK = (0x1 << 24);
}

/**
  * @功能:配置在低功耗模式下的中断唤醒源
  * @注意:无       
  * @参数:irq    外设中断号
  * @返回值:无
  */
void Scu_SetWakeup_Source(int irq)
{
    if(irq == EXTI0_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 0);
    }
    else if(irq == EXTI1_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 1);
    }
    else if(irq == EXTI2_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 2);
    }
    else if(irq == EXTI3_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 3);
    }
    else if(irq == EXTI4_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 4);
    }
    else if(irq == TIMER0_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 8);
    }
    else if(irq == TIMER1_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 9);
    }
    else if(irq == TIMER2_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 10);
    }
    else if(irq == TIMER3_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 11);
    }
    else if(irq == UART0_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 12);
    }
    else if(irq == UART1_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 13);
    }
    else if(irq == UART2_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 14);
    }
    else if(irq == UART3_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 15);
    }
    else if(irq == ADC_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 16);
    }
    else if(irq == IIS_DMA_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 17);
    }
    else if(irq == CAN_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 20);
    }
    else if(irq == ASR_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 21);
    }
    else if(irq == REM_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 22);
    }
    else if(irq == VAD_IRQn)
    {
        SCU->WAKE_UP_MASK |= (0x1 << 23);
    }
}

/**
  * @功能:配置管脚复用对应功能
  * @注意:无       
  * @参数:pin : 管脚名;  io_function: 第X功能选择 select
  * @返回值:无
  */
void Scu_SetIOReuse(PinPad_Name pin,IOResue_FUNCTION io_function)
{
    switch(pin)
    {
        case UART0_TX_PAD:
        case UART0_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 0);
            SCU->IO_REUSE_CFG |= (io_function << 0);
            break;
        case UART1_TX_PAD:
        case UART1_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 2);
            SCU->IO_REUSE_CFG |= (io_function << 2);
            break;
        case UART1_CTS_PAD:
        case UART1_RTS_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 4);
            SCU->IO_REUSE_CFG |= (io_function << 4);
            break;
        case UART2_TX_PAD:
        case UART2_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 6);
            SCU->IO_REUSE_CFG |= (io_function << 6);
            break;
        case UART3_TX_PAD:
        case UART3_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 8);
            SCU->IO_REUSE_CFG |= (io_function << 8);
            break;
        case IR_TX_PAD:
        case IR_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 10);
            SCU->IO_REUSE_CFG |= (io_function << 10);
            break;
        case CAN_TX_PAD:
        case CAN_RX_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 12);
            SCU->IO_REUSE_CFG |= (io_function << 12);
            break;
        case I2C0_SCL_PAD:
        case I2C0_SDA_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 14);
            SCU->IO_REUSE_CFG |= (io_function << 14);
            break;
        case I2C1_SCL_PAD:
        case I2C1_SDA_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 16);
            SCU->IO_REUSE_CFG |= (io_function << 16);
            break;
        case I2S_MCLK_PAD:
        case I2S0_SCLK_PAD:
        case I2S0_LRCLK_PAD:
        case I2S0_SDI_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 18);
            SCU->IO_REUSE_CFG |= (io_function << 18);
            break;
        case I2S1_SCLK_PAD:
        case I2S1_LRCLK_PAD:
        case I2S1_SDI_PAD:
        case I2S1_SDO_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 20);
            SCU->IO_REUSE_CFG |= (io_function << 20);
            break;
        case I2S2_SDI_PAD:
        case I2S2_SDO_PAD:
        case I2S2_LRCLK_PAD:
        case I2S2_SCLK_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 22);
            SCU->IO_REUSE_CFG |= (io_function << 22);
            break;
        case XPWM0_PAD:
        case XPWM1_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 24);
            SCU->IO_REUSE_CFG |= (io_function << 24);
            break;
        case XPWM2_PAD:
        case XPWM3_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 26);
            SCU->IO_REUSE_CFG |= (io_function << 26);
            break;
        case XPWM4_PAD:
        case XPWM5_PAD:
            SCU->IO_REUSE_CFG &= ~(0x3 << 28);
            SCU->IO_REUSE_CFG |= (io_function << 28);
            break;
        case PWM6_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 0);
            SCU->IO_REUSE_CFG1 |= (io_function << 0);
            break;
        case PWM7_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 2);
            SCU->IO_REUSE_CFG1 |= (io_function << 2);
            break;
        case PWM8_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 4);
            SCU->IO_REUSE_CFG1 |= (io_function << 4);
            break;
        case SPI1_DOUT_PAD:
        case SPI1_DIN_PAD:
        case SPI1_CS_PAD:
        case SPI1_CLK_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 6);
            SCU->IO_REUSE_CFG1 |= (io_function << 6);
            break;
        case SPI2_DIN_PAD:
        case SPI2_DOUT_PAD:
        case SPI2_CS_PAD:
        case SPI2_CLK_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 8);
            SCU->IO_REUSE_CFG1 |= (io_function << 8);
            break;
        case JTAG_TDI_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 10);
            SCU->IO_REUSE_CFG1 |= (io_function << 10);
            break;
        case JTAG_TDO_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 12);
            SCU->IO_REUSE_CFG1 |= (io_function << 12);
            break;
        case JTAG_TMS_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 14);
            SCU->IO_REUSE_CFG1 |= (io_function << 14);
            break;
        case JTAG_TCK_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 16);
            SCU->IO_REUSE_CFG1 |= (io_function << 16);
            break;
        case TRST_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 18);
            SCU->IO_REUSE_CFG1 |= (io_function << 18);
            break;
        case AIN0_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 20);
            SCU->IO_REUSE_CFG1 |= (io_function << 20);
            break;
        case AIN1_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 22);
            SCU->IO_REUSE_CFG1 |= (io_function << 22);
            break;
        case AIN2_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 24);
            SCU->IO_REUSE_CFG1 |= (io_function << 24);
            break;
        case AIN3_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 26);
            SCU->IO_REUSE_CFG1 |= (io_function << 26);
            break;
        case AIN4_PAD:
        case AIN5_PAD:
        case AIN6_PAD:
        case AIN7_PAD:
            SCU->IO_REUSE_CFG1 &= ~(0x3 << 28);
            SCU->IO_REUSE_CFG1 |= (io_function << 28);
            break;
        default:
            break;
    }
}
/**
  * @功能:对所有SCU控制的外设复位，并释放复位
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_AllPreipheralRst(void)
{
	Unlock_Rstconfig();
	SCU->SOFT_PRERST_CTRL0 = 0x0;
	SCU->SOFT_PRERST_CTRL1 = 0x0;
	for(volatile unsigned int i=0xFFFF;i;i--);
	SCU->SOFT_PRERST_CTRL0 = 0xFFFFFFFF;
	SCU->SOFT_PRERST_CTRL1 = 0xFFFFFFFF;
	Lock_Rstconfig();
}

/**
  * @功能:关闭SCU控制的部分外设(除wdg、sdram)时钟
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void Scu_AllPreipheralclkclose(void)
{
	Unlock_Ckconfig();
	SCU->PRE_CLKGATE0 &= ~((0xFF<<0)|(0xF<<11)|(0xF<<20)\
	  |(0x3F<<26)); 
	SCU->PRE_CLKGATE1 &= ~((0x1FF<<0)|(0x7<<12)|(0xF<<16)\
	  |(0x7<<20)); 
	Lock_Ckconfig();
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/






