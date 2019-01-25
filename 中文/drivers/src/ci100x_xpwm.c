/**
  ******************************************************************************
  * @文件    ci100x_xpwm.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的电机控制xPWM模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 

#include "ci100x_xpwm.h"
#include "ci100x_scu.h"
#include "misc.h"

/**
  * @功能：xPWMx 的触发信号的触发时刻选择
  * @注意:无  
  * @参数：1.xPWMx：xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *        xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *	       2.socx：xPWM触发的PWM波选择，xPWM_SOCA , xPWM_SOCB
  *        3.socsel：触发时刻选择
  *           xPWM_SOCABSEL_ZERO：计数值等于0的时刻,
  *           xPWM_SOCABSEL_PRD ：计数值等于周期的时刻,
  *           xPWM_SOCABSEL_ZERO_PRD ：计数值等于0或周期的时刻,
  *           xPWM_SOCABSEL_CMPAINC  ：递增计数时计数值等于CMPA寄存器的值时刻,
  *           xPWM_SOCABSEL_CMPADEC  ：递减计数时计数值等于CMPA寄存器的值时刻,
  *           xPWM_SOCABSEL_CMPBINC  ：递增计数时计数值等于CMPB寄存器的值时刻,
  *           xPWM_SOCABSEL_CMPBDEC  ：递减计数时计数值等于CMPB寄存器的值时刻,
  *           xPWM_SOCABSEL_CPR1INC  ：递增计数时计数值等于CPR1寄存器的值时刻,
  *           xPWM_SOCABSEL_CPR1DEC  ：递减计数时计数值等于CPR1寄存器的值时刻,
  *           xPWM_SOCABSEL_CPR2INC  ：递增计数时计数值等于CPR2寄存器的值时刻,
  *           xPWM_SOCABSEL_CPR2DEC  ：递减计数时计数值等于CPR2寄存器的值时刻,
  * @返回值：无
  */
void xPWM_SOC(xPWM_TypeDef* xPWMx,xPWM_SOCx socx,xPWM_SOCABSELx socsel)
{
	xPWMx->ETSEL &= ~(0xF<<(6+5*socx));
	xPWMx->ETSEL |= (socsel<<(6+5*socx));
}
/**
  * @注意:无  
  * @功能：xPWM刹车中断使能控制
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2. xpwmtzint，刹车中断类型选择 
  *           xPWM_TZINT_CBC 连续刹车，刹车信号不会自动清除；
  *           xPWM_TZINT_OST 单点刹车，刹车信号保持一个周期，自动清除
  *        3. cmd ，ENABLE 中断使能；DISABLE 禁止中断
  * @返回：无
  */
void xPWM_TZEINT(xPWM_TypeDef* xPWMx,xPWM_TZINTx xpwmtzint,FunctionalState cmd)
{
  if(cmd!=ENABLE)
  {
     xPWMx->TZEINT &=~(1<<xpwmtzint);
  }else
  {
     xPWMx->TZEINT |=(1<<xpwmtzint);
  }
}
/**
  * @功能：xPWM刹车中断状态清除
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xpwmtzclr 中断状态选择
  *           xPWM_TZCLR_INT xPWM中断 
  *           xPWM_TZCLR_CBC 中断状态
  *           xPWM_TZCLR_OST 中断状态
  * @返回值：无
  */
void xPWM_TZCLR(xPWM_TypeDef* xPWMx,xPWM_TZCLRx xpwmtzclr)
{
    xPWMx->TZCLR |=(1<<xpwmtzclr);
}
/**
  * @功能：xPWM 控制寄存器配置
  * @注意:无  
  *@参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *       2. xPWM_TBCTLInit_Struct 初始化结构体指针
  * @返回值:无
  */
void xPWM_TBCTLConfig(xPWM_TypeDef* xPWMx,\
  xPWM_TBCTLInit_TypeDef* xPWM_TBCTLInit_Struct)
{
	  xPWMx->TBCTL = (xPWM_TBCTLInit_Struct->xPWM_FREESOFT<<14)|\
      (xPWM_TBCTLInit_Struct->xPWM_PHSDIR<<13)|\
      (xPWM_TBCTLInit_Struct->xPWM_CLKDIV <<10)|\
      (xPWM_TBCTLInit_Struct->xPWM_CNTEN<<9)|\
      (xPWM_TBCTLInit_Struct->xPWM_SWFSNC<<6)|\
      (xPWM_TBCTLInit_Struct->xPWM_SYNCOSEL <<4)|\
      (xPWM_TBCTLInit_Struct->xPWM_PRDLD<<3)|\
      (xPWM_TBCTLInit_Struct->xPWM_PHSEN<<2)|\
      (xPWM_TBCTLInit_Struct->xPWM_CTRMODE<<0);        
}
/**
  * @功能：xpwm计数周期值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.tbprd  计数周期值 16位
  * @返回值：无
  */
void xPWM_TBPRDConfig(xPWM_TypeDef* xPWMx,unsigned short tbprd)
{
    xPWMx->TBPRD =  tbprd & 0xffff;
}
/**
  * @功能：xpwm比较控制寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_CMPCTLInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_CMPCTLConfig(xPWM_TypeDef* xPWMx,xPWM_CMPCTLInit_TypeDef* xPWM_CMPCTLInit_Struct)
{
    xPWMx->CMPCTL = (xPWM_CMPCTLInit_Struct->xPWM_SHDWBMODE>>6)|\
      (xPWM_CMPCTLInit_Struct->xPWM_SHDWAMODE>>4)|\
      (xPWM_CMPCTLInit_Struct->xPWM_LOADBMODE>>2)|\
      (xPWM_CMPCTLInit_Struct->xPWM_LOADAMODE>>0);
}
/**
  * @功能：xpwm比较寄存器CMPA的值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.cmpaval 比较值，16位
  *@返回值：无
  */
void xPWM_CMPAConfig(xPWM_TypeDef* xPWMx,unsigned short cmpaval)
{
    xPWMx->CMPA = cmpaval & 0xffff;
}
/**
  * @功能：xpwm比较寄存器CMPB的值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.cmpbval 比较值，16位
  * @返回值：无
  */
void xPWM_CMPBConfig(xPWM_TypeDef* xPWMx,unsigned short cmpbval)
{
    xPWMx->CMPB = cmpbval ;
}
/**
  * @功能：xpwm的cpr1寄存器的值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.cpr1val 比较值，16位
  * @返回值：无
  */
void xPWM_CPR1Config(xPWM_TypeDef* xPWMx,unsigned short cpr1val)
{
    xPWMx->CPR1 = cpr1val & 0xffff;
}
/**
  * @功能：xpwm的cpr2寄存器的值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.cpr2val 比较值，16位
  * @返回值：无
  */
void xPWM_CPR2Config(xPWM_TypeDef* xPWMx,unsigned short cpr2val)
{
    xPWMx->CPR2 = cpr2val & 0xffff;
}
/**
  * @功能：xpwm的AQCTLA寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_AQCTLAInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_AQCTLAConfig(xPWM_TypeDef* xPWMx,xPWM_AQCTLABInit_TypeDef* xPWM_AQCTLAInit_Struct)
{
    xPWMx->AQCTLA = (xPWM_AQCTLAInit_Struct->xPWM_CBD<<10)|\
      (xPWM_AQCTLAInit_Struct->xPWM_CBU <<8)|\
      (xPWM_AQCTLAInit_Struct->xPWM_CAD <<6)|\
      (xPWM_AQCTLAInit_Struct->xPWM_CAU <<4)|\
      (xPWM_AQCTLAInit_Struct->xPWM_PRD <<2)|\
      (xPWM_AQCTLAInit_Struct->xPWM_ZRO <<0);
}
/**
  * @功能：xpwm的AQCTLB寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_AQCTLBInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_AQCTLBConfig(xPWM_TypeDef* xPWMx,xPWM_AQCTLABInit_TypeDef* xPWM_AQCTLBInit_Struct)
{
    xPWMx->AQCTLB = (xPWM_AQCTLBInit_Struct->xPWM_CBD<<10)|\
      (xPWM_AQCTLBInit_Struct->xPWM_CBU <<8)|\
      (xPWM_AQCTLBInit_Struct->xPWM_CAD <<6)|\
      (xPWM_AQCTLBInit_Struct->xPWM_CAU <<4)|\
      (xPWM_AQCTLBInit_Struct->xPWM_PRD <<2)|\
      (xPWM_AQCTLBInit_Struct->xPWM_ZRO <<0);
}
/**
  * @功能：xpwm的AQSFRCC寄存器配置
  * @注意:无  
  *@参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *       2.xPWM_AQSFRCInit_Struct 初始化结构体指针
  *@返回值：无
  */
void xPWM_AQSFRCConfig(xPWM_TypeDef* xPWMx,xPWM_AQSFRCInit_TypeDef* xPWM_AQSFRCInit_Struct)
{
    xPWMx->AQCSFRC =(xPWM_AQSFRCInit_Struct->xPWM_RLDCSF<<6)|\
      (xPWM_AQSFRCInit_Struct->xPWM_OTSFB <<5)|\
      (xPWM_AQSFRCInit_Struct->xPWM_ACTSFB<<3)|\
      (xPWM_AQSFRCInit_Struct->xPWM_OTSFA <<2)|\
      (xPWM_AQSFRCInit_Struct->xPWM_ACTSFA<<0);
}
/**
  * @功能：xpwm的AQCSFRC寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_AQCSFRCInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_AQCSFRCConfig(xPWM_TypeDef* xPWMx,xPWM_AQCSFRCInit_TypeDef* xPWM_AQCSFRCInit_Struct)
{
    xPWMx->AQCSFRC =(xPWM_AQCSFRCInit_Struct->xPWM_CSFB<<2)|\
      (xPWM_AQCSFRCInit_Struct->xPWM_CSFA <<0);
}
/**
  * @功能：xpwm的DBCTL寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_DBCTLInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_DBCTLConfig(xPWM_TypeDef* xPWMx,xPWM_DBCTLInit_TypeDef* xPWM_DBCTLInit_Struct)
{
    xPWMx->DBCTL =(xPWM_DBCTLInit_Struct->xPWM_IN_MODE <<4)|\
      (xPWM_DBCTLInit_Struct->xPWM_POLSEL << 2)|\
      (xPWM_DBCTLInit_Struct->xPWM_OUT_MODE <<0);
}
/**
  * @功能：xpwm的上沿死区计数值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.dbred 死区计数值 16位
  * @返回值：无
  */
void xPWM_DBREDConfig(xPWM_TypeDef* xPWMx,unsigned short dbred)
{
    xPWMx->DBRED = dbred & 0XFFFF;
}
/**
  * @功能：xpwm的下沿死区计数值配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.dbred 死区计数值 16位
  * @返回值：无
  */
void xPWM_DBFEDConfig(xPWM_TypeDef* xPWMx,unsigned short dbfed)
{
    xPWMx->DBFED = dbfed & 0XFFFF;
}
/**
  * @功能：xpwm的刹车选择寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_TZSELInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_TZSELConfig(xPWM_TypeDef* xPWMx,xPWM_TZSELInit_TypeDef* xPWM_TZSELInit_Struct)
{
    char i=0;
    xPWMx->TZSEL  &= ~0xFFFF;
    for(i=1;i<7;i++)
    {
        xPWMx->TZSEL |= (xPWM_TZSELInit_Struct->xPWM_TZx_CBC[i] << (i-1));
        xPWMx->TZSEL |= (xPWM_TZSELInit_Struct->xPWM_TZx_OSHT[i] << (i+7));
    }
    xPWMx->TZSEL |= (xPWM_TZSELInit_Struct->xPWM_CBCMOD << 6)|\
      (xPWM_TZSELInit_Struct->xPWM_OSHTMOD << 14);
}
/**
  * @功能：xpwm的刹车控制寄存器器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_TZCTLInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_TZCTLConfig(xPWM_TypeDef* xPWMx,xPWM_TZCTLInit_TypeDef* xPWM_TZCTLInit_Struct)
{
    char i=0;
    xPWMx->TZCTL =0x0;
    for(i=1;i<7;i++)
    {
        if(i!=4) 
        {
            xPWMx->TZCTL |=(xPWM_TZCTLInit_Struct->xPWM_TZn_SEL[i]<<(i+3));
        }
        xPWMx->TZCTL |=(xPWM_TZCTLInit_Struct->xPWM_TZn_EN[i]<<(i+9));
    }
    xPWMx->TZCTL |= (xPWM_TZCTLInit_Struct->xPWM_TZB << 2)|\
      (xPWM_TZCTLInit_Struct->xPWM_TZA << 0);
}

/**
  * @功能：xpwm的软件触发寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.tz 刹车中断类型
  * @返回值：无
  */
void xPWM_TZFRC(xPWM_TypeDef* xPWMx,xPWM_TZCLRx tz)
{
	xPWMx->TZFRC |=(1<<tz);
}


/**
  * @功能：xpwm的事件选择寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.tz 刹车中断类型
  * @返回值：无
  */
int xPWM_TZFlag(xPWM_TypeDef* xPWMx,xPWM_TZCLRx tz)
{
	return   xPWMx->TZFLG & (1<<tz);
}

/**
  * @功能：xpwm的事件选择寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_ETSELInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_ETSELConfig(xPWM_TypeDef* xPWMx,xPWM_ETSELInit_TypeDef* xPWM_ETSELInit_Struct)
{
    xPWMx->ETSEL =(xPWM_ETSELInit_Struct->xPWM_SOCBEN << 15)|\
      (xPWM_ETSELInit_Struct->xPWM_SOCBSEL <<11)|\
      (xPWM_ETSELInit_Struct->xPWM_SOCAEN << 10)|\
      (xPWM_ETSELInit_Struct->xPWM_SOCASEL << 6)|\
      (xPWM_ETSELInit_Struct->xPWM_INTEN << 3  )|\
      (xPWM_ETSELInit_Struct->xPWM_INTSEL << 0);
}

/**
  * @功能：xpwm的中断寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.cmd ENABLE 使能，DISABLE 关闭
  * @返回值：无
  */
void xPWM_ETSELInterruptConfig(xPWM_TypeDef* xPWMx,FunctionalState cmd)
{
  	if(cmd==ENABLE)
    {
  		xPWMx->ETSEL |=(1<<3);
	}else
    {
		xPWMx->ETSEL &=~(1<<3);
	}
}

/**
  * @功能：xpwm的事件预分频寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_ETPSInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_ETPSConfig(xPWM_TypeDef* xPWMx,xPWM_ETPSInit_TypeDef* xPWM_ETPSInit_Struct)
{
    xPWMx->ETPS = (xPWM_ETPSInit_Struct->xPWM_SOCBCNT<<14)|\
      (xPWM_ETPSInit_Struct->xPWM_SOCBPRD << 12)|\
      (xPWM_ETPSInit_Struct->xPWM_SOCACNT << 10)|\
      (xPWM_ETPSInit_Struct->xPWM_SOCAPRD <<  8)|\
      (xPWM_ETPSInit_Struct->xPWM_INTCNT  <<  2)|\
      (xPWM_ETPSInit_Struct->xPWM_INTPRD  <<  0);
}
/**
  * @功能：xpwm事件中断清除寄存器
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xpwm_etstatus 事件选择
  *           xPWM_ETSTATUS_SOCB b路触发事件
  *           xPWM_ETSTATUS_SOCA a路触发事件
  *           xPWM_ETSTATUS_INT 中断
  * @返回值：无
  */
void xPWM_ETCLRClear(xPWM_TypeDef* xPWMx,xPWM_ETSTATUSx xpwm_etstatus)
{
    xPWMx->ETCLR |= (1<<(xpwm_etstatus &0x3));
}
/**
  * @功能：xpwm所有中断清除
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  * @返回值：无   
  */
void xPWM_INTClear(xPWM_TypeDef* xPWMx)
{
    xPWMx->ETCLR =0xF;
}
/**
  * @功能：xpwm事件触发寄存器配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *        2.xPWM_ETFRCInit_Struct 初始化结构体
  * @返回值：无
  */
void xPWM_ETFRCConfig(xPWM_TypeDef* xPWMx,xPWM_ETFRCInit_TypeDef* xPWM_ETFRCInit_Struct)
{
    xPWMx->ETFRC = (xPWM_ETFRCInit_Struct->xPWM_SOCA << 2)|\
      (xPWM_ETFRCInit_Struct->xPWM_SOCB << 3);
}
/**
  * @功能：xpwm计数启动
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  * @返回值：无
  */
void xPWM_START(xPWM_TypeDef* xPWMx)
{
    xPWMx->TBCTL |= (1<<9);
}
/**
  * @功能：xpwm计数停止
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  * @返回值：无
  */
void xPWM_STOP(xPWM_TypeDef* xPWMx)
{
    xPWMx->TBCTL &= ~(1<<9);
}
/**
  * @功能：xpwm初始化配置
  * @注意:无  
  * @参数：1.xPWMx:xPWM组，xPWM1,xPWM2,xPWM3,xPWM(
  *         xPWM同时映射到xPWM1,xPWM2,xPWM3)
  *       2.xPWMInit_Struct 初始化结构体指针
  * @返回值：无
  */
void xPWM_Init(xPWM_TypeDef* xPWMx,xPWMInit_TypeDef* xPWMInit_Struct)
{
    xPWM_TBCTLConfig(xPWMx,&(xPWMInit_Struct->xPMW_TBCTL));
    xPWM_TBPRDConfig(xPWMx,xPWMInit_Struct->xPWMX_TBPRD);
    xPWM_CMPCTLConfig(xPWMx,&(xPWMInit_Struct->xPWM_CMPCTL));
    xPWM_CMPAConfig(xPWMx,xPWMInit_Struct->xPWM_CMPA);
    xPWM_CMPBConfig(xPWMx,xPWMInit_Struct->xPWM_CMPB);
    xPWM_CPR1Config(xPWMx,xPWMInit_Struct->xPWM_CPR1);
    xPWM_CPR2Config(xPWMx,xPWMInit_Struct->xPWM_CPR2);
    xPWM_AQCTLAConfig(xPWMx,&(xPWMInit_Struct->xPWM_AQCTLA));
    xPWM_AQCTLBConfig(xPWMx,&(xPWMInit_Struct->xPWM_AQCTLB));
    xPWM_AQSFRCConfig(xPWMx,&(xPWMInit_Struct->xPWM_AQSFRC));
    xPWM_AQCSFRCConfig(xPWMx,&(xPWMInit_Struct->xPWM_AQCSFRC));
    xPWM_DBCTLConfig(xPWMx,&(xPWMInit_Struct->xPWM_DBCTL));
    xPWM_DBREDConfig(xPWMx,xPWMInit_Struct->xPWM_DBRED);
    xPWM_DBFEDConfig(xPWMx,xPWMInit_Struct->xPWM_DBFED);
	xPWM_TZCTLConfig(xPWMx,&(xPWMInit_Struct->xPWM_TZCTL));
    xPWM_TZSELConfig(xPWMx,&(xPWMInit_Struct->xPWM_TZSEL));
    xPWM_ETSELConfig(xPWMx,&(xPWMInit_Struct->xPWM_ETSEL));
    xPWM_ETPSConfig(xPWMx,&(xPWMInit_Struct->xPWM_ETPS));
    xPWM_ETCLRClear(xPWMx,xPWM_ETSTATUS_INT);
    xPWM_ETCLRClear(xPWMx,xPWM_ETSTATUS_SOCA);
    xPWM_ETCLRClear(xPWMx,xPWM_ETSTATUS_SOCB);
    xPWM_ETFRCConfig(xPWMx,&(xPWMInit_Struct->xPWM_ETFRC));
}


/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/