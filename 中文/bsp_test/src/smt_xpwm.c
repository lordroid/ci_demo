#include "ci100x_xpwm.h"
#include "ci100x_uart.h"
#include "includes.h"

void xPWM_test(void)
{
    xPWMInit_TypeDef xPWMInit_Struct={0};
    NVIC_InitTypeDef NVIC_InitStruct={0};
    
    NVIC_InitStruct.NVIC_IRQChannel = XPWM_TZ_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
    
    NVIC_InitStruct.NVIC_IRQChannel = xPWM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
    
    Scu_SetIOReuse(XPWM0_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM1_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM2_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM3_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM4_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM5_PAD,FIRST_FUNCTION);
    
    /*TZ 脚   需要把TZ 脚改成对应的状态，TZ脚为低电平 有效
    ，，XPWM 就停止输出，停止方式有两种:one shot ,cbc*/
    Scu_SetIOReuse(I2S1_LRCLK_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(I2S1_SCLK_PAD,SECOND_FUNCTION);
    
    Scu_SetDeviceGate(xPWM_BASE,ENABLE);
    
    xPWMInit_Struct.xPMW_TBCTL.xPWM_CTRMODE = xPWM_CTRMODE_INC_DEC;/*增减计数，计数器先递增，再递减*/
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PRDLD = xPWM_PRDLD_FREE;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_SYNCOSEL = xPWM_SYNCOSEL_ZERO;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSDIR = xPWM_PHSDIR_INC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_FREESOFT = xPWM_FREESOFT_CONTINUE;

    xPWMInit_Struct.xPWMX_TBPRD =  12000;/*计数周期*/

    xPWMInit_Struct.xPWM_CMPCTL.xPWM_LOADAMODE =xPWM_LOADABMODE_ZERO;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_LOADBMODE =xPWM_LOADABMODE_ZERO;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_SHDWAMODE =xPWM_SHDWABMODE_DIRECT;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_SHDWBMODE =xPWM_SHDWABMODE_DIRECT;
 
    xPWMInit_Struct.xPWM_CMPA = 3000;
    xPWMInit_Struct.xPWM_CMPB = 4000;
    xPWMInit_Struct.xPWM_CPR1 = 4000;
    xPWMInit_Struct.xPWM_CPR2 = 4000;

    xPWMInit_Struct.xPWM_AQCTLA.xPWM_ZRO = xPWM_ZRO_HIGH;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_PRD = xPWM_PRD_LOW;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_CAU = xPWM_CAU_LOW;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_CAD = xPWM_CAD_HIGH;
 
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_ZRO = xPWM_ZRO_NONE;
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_CAU = xPWM_CAU_NONE;
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_CAD = xPWM_CAD_NONE;
  
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_ACTSFA = xPWM_ACTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_ACTSFB = xPWM_ACTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_OTSFA = xPWM_OTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_OTSFB =xPWM_OTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_RLDCSF = xPWM_RLDCSF_ZERO;
    
    xPWMInit_Struct.xPWM_AQCSFRC.xPWM_CSFA = xPWM_CSFAB_NONE;
    xPWMInit_Struct.xPWM_AQCSFRC.xPWM_CSFB = xPWM_CSFAB_NONE;
    
    xPWMInit_Struct.xPWM_DBCTL.xPWM_OUT_MODE = xPWM_OUT_MODE_FALLINGRISING;
    xPWMInit_Struct.xPWM_DBCTL.xPWM_POLSEL = xPWM_POLSEL_OPPOSITEEPWMxB;
    xPWMInit_Struct.xPWM_DBCTL.xPWM_IN_MODE =xPWM_IN_MODE_EPWMxA_FALLING_A_RISING;
    
    xPWMInit_Struct.xPWM_DBRED = 1000;
    xPWMInit_Struct.xPWM_DBFED = 1000;
    
    xPWMInit_Struct.xPWM_TZSEL.xPWM_TZx_OSHT[1]=xPWM_TZx_n_OPEN;
    xPWMInit_Struct.xPWM_TZSEL.xPWM_TZx_OSHT[2]=xPWM_TZx_n_OPEN;
 
    xPWMInit_Struct.xPWM_TZSEL.xPWM_OSHTMOD = xPWM_OSHTMOD_IMMEDIATELY;
    
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZn_SEL[1] = xPWM_TZn_SEL_LOW;
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZn_SEL[2] = xPWM_TZn_SEL_LOW;
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZA = xPWM_TZx_LOW;
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZB = xPWM_TZx_LOW;
    
    xPWMInit_Struct.xPWM_ETSEL.xPWM_INTSEL = xPWM_INTSEL_ZERO;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_INTEN = xPWM_INTEN_ENABLE;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCASEL = xPWM_SOCABSEL_CPR1INC;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCAEN = xPWM_SOCABEN_ENABLE;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCBSEL = xPWM_SOCABSEL_CPR2DEC;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCBEN = xPWM_SOCABEN_ENABLE;
    
    xPWMInit_Struct.xPWM_ETPS.xPWM_INTPRD  =xPWM_INT_SOCABPRD_SOCABCNT01;
    xPWMInit_Struct.xPWM_ETPS.xPWM_SOCAPRD =xPWM_INT_SOCABPRD_SOCABCNT01;
    xPWMInit_Struct.xPWM_ETPS.xPWM_SOCBPRD =xPWM_INT_SOCABPRD_SOCABCNT01; 
	xPWM_Init(xPWM1,&xPWMInit_Struct);
    xPWMInit_Struct.xPMW_TBCTL.xPWM_CTRMODE = xPWM_CTRMODE_INC_DEC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PRDLD = xPWM_PRDLD_FREE;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_SYNCOSEL = xPWM_SYNCOSEL_EPWMxSYNCI;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSDIR = xPWM_PHSDIR_INC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_FREESOFT = xPWM_FREESOFT_CONTINUE;
    
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSEN = xPWM_PHSEN_SYNCVALID;
    xPWMInit_Struct.xPWM_TBPHS = 0;
    
    xPWMInit_Struct.xPWM_CPR1 =0x0;
    xPWMInit_Struct.xPWM_CPR2 =0x0;
    
    xPWM_Init(xPWM2,&xPWMInit_Struct);    
    
    xPWM_Init(xPWM3,&xPWMInit_Struct);    

    xPWM_INTClear(xPWM);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_OST);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_CBC);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_OST);    
    xPWM_TZEINT(xPWM,xPWM_TZINT_CBC,ENABLE);
    xPWM_TZEINT(xPWM,xPWM_TZINT_OST,ENABLE);
    xPWM_START(xPWM);
}

