/**
  ******************************************************************************
  * @文件    ci100x_xpwm.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的xpwm模块驱动头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#ifndef _CI100X_xPWM_H
#define _CI100X_xPWM_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "smt.h"
   
#ifndef xPWM_BASE
#define xPWM_BASE (0X40025000)
#endif

typedef enum{
    xPWM_TZINT_CBC=1,
    xPWM_TZINT_OST=2,
}xPWM_TZINTx;
typedef enum{
    xPWM_TZCLR_INT =0,
    xPWM_TZCLR_CBC=1,
    xPWM_TZCLR_OST=2,
}xPWM_TZCLRx;

typedef enum{
		xPWM_FREESOFT_STOP =0,
		xPWM_FREESOFT_STOP_TBPRD_ZERO	=1,
		xPWM_FREESOFT_GO_ON = 2,
        xPWM_FREESOFT_CONTINUE =3,
}xPWM_FREESOFTx;

typedef enum{
		xPWM_PHSDIR_DEC=0,
		xPWM_PHSDIR_INC=1,
}xPWM_PHSDIRx;

typedef enum{
	   xPWM_CLKDIV1=0,
	   xPWM_CLKDIV2=1,
	   xPWM_CLKDIV4=2,
	   xPWM_CLKDIV8=3,
	   xPWM_CLKDIV16=4,
	   xPWM_CLKDIV32=5,
	   xPWM_CLKDIV64=6,
	   xPWM_CLKDIV128=7,
}xPWM_CLKDIVx;

typedef enum{
	xPWM_CNTEN_STOP=0,
	xPWM_CNTEN_ENABLE=1,
}xPWM_CNTENx;

typedef enum{
	xPWM_SWFSYNC_NOSYNC=0,
	xPWM_SWFSYNC_SYNC=1,
}xPWM_SWFSYNCx;

typedef enum{
		xPWM_SYNCOSEL_EPWMxSYNCI=0,
		xPWM_SYNCOSEL_ZERO=1,
		xPWM_SYNCOSEL_CMPB=2,
		xPWM_SYNCOSEL_NOSYN=3,
}xPWM_SYNCOSELx;

typedef enum{
	xPWM_PRDLD_ZERO =0,
	xPWM_PRDLD_FREE	=1,
}xPWM_PRDLDx;

typedef enum{
	 xPWM_PHSEN_INVALID     =0, 
	 xPWM_PHSEN_SYNCVALID   =1,
}xPWM_PHSENx;

typedef enum{
		xPWM_CTRMODE_INC    =0,
		xPWM_CTRMODE_DEC    =1,
		xPWM_CTRMODE_INC_DEC=2,
		xPWM_CTRMODE_STOP   =3,
}xPWM_CTRMODEx;

typedef struct{
	 xPWM_FREESOFTx xPWM_FREESOFT;
	 xPWM_PHSDIRx xPWM_PHSDIR;
	 xPWM_CLKDIVx xPWM_CLKDIV;
	 xPWM_CNTENx xPWM_CNTEN;
	 xPWM_SWFSYNCx xPWM_SWFSNC;
	 xPWM_SYNCOSELx xPWM_SYNCOSEL; 
	 xPWM_PRDLDx xPWM_PRDLD;
	 xPWM_PHSENx xPWM_PHSEN;
	 xPWM_CTRMODEx xPWM_CTRMODE;	
}xPWM_TBCTLInit_TypeDef;

typedef enum{
    xPWM_SHDWABMODE_DIRECT=0,
    xPWM_SHDWABMODE_INDIRECT=1,
}xPWM_SHDWABMODEx;

typedef enum{
    xPWM_LOADABMODE_ZERO =0,
    xPWM_LOADABMODE_PRD=1,
    xPWM_LOADABMODE_ZERO_PRD=2,
    xPWM_LOADABMODE_NONE = 3,
}xPWM_LOADABMODEx;

typedef struct{
    xPWM_SHDWABMODEx xPWM_SHDWBMODE;
    xPWM_SHDWABMODEx xPWM_SHDWAMODE;
    xPWM_LOADABMODEx xPWM_LOADBMODE;
    xPWM_LOADABMODEx xPWM_LOADAMODE;
}xPWM_CMPCTLInit_TypeDef;

typedef enum{
    xPWM_CBD_NONE   =0,
    xPWM_CBD_LOW    =1,
    xPWM_CBD_HIGH   =2,
    xPWM_CBD_EDG    =3,
}xPWM_CBDx;

typedef enum{
    xPWM_CBU_NONE   =0,
    xPWM_CBU_LOW    =1,
    xPWM_CBU_HIGH   =2,
    xPWM_CBU_EDG    =3,
}xPWM_CBUx;

typedef enum{
    xPWM_CAD_NONE   =0,
    xPWM_CAD_LOW    =1,
    xPWM_CAD_HIGH   =2,
    xPWM_CAD_EDG    =3,
}xPWM_CADx;

typedef enum{
    xPWM_CAU_NONE   =0,
    xPWM_CAU_LOW    =1,
    xPWM_CAU_HIGH   =2,
    xPWM_CAU_EDG    =3,
}xPWM_CAUx;

typedef enum{
    xPWM_PRD_NONE   =0,
    xPWM_PRD_LOW    =1,
    xPWM_PRD_HIGH   =2,
    xPWM_PRD_EDG    =3,
}xPWM_PRDx;

typedef enum{
    xPWM_ZRO_NONE   =0,
    xPWM_ZRO_LOW    =1,
    xPWM_ZRO_HIGH   =2,
    xPWM_ZRO_EDG    =3,
}xPWM_ZROx;

typedef struct{
    xPWM_CBDx xPWM_CBD;
    xPWM_CBUx xPWM_CBU;
    xPWM_CADx xPWM_CAD;
    xPWM_CAUx xPWM_CAU;
    xPWM_PRDx xPWM_PRD;
    xPWM_ZROx xPWM_ZRO;
}xPWM_AQCTLABInit_TypeDef;

typedef enum{
    xPWM_RLDCSF_ZERO        =0,
    xPWM_RLDCSF_PRD         =1,
    xPWM_RLDCSF_ZERO_PDR    =2,
    xPWM_RLDCSF_IMMEDIATELY =3,
}xPWM_RLDCSFx;

typedef enum{
    xPWM_OTSFAB_NONE =0,
    xPWM_OTSFAB_FORCE=1,
}xPWM_OTSFABx;

typedef enum{
    xPWM_ACTSFAB_NONE   =0,
    xPWM_ACTSFAB_LOW    =1,
    xPWM_ACTSFAB_HIGH   =2,
    xPWM_ACTSFAB_EDG    =3,    
}xPWM_ACTSFABx;

typedef struct{
    xPWM_RLDCSFx xPWM_RLDCSF;
    xPWM_OTSFABx xPWM_OTSFB;
    xPWM_ACTSFABx xPWM_ACTSFB;    
    xPWM_OTSFABx xPWM_OTSFA;
    xPWM_ACTSFABx xPWM_ACTSFA;
}xPWM_AQSFRCInit_TypeDef;

typedef enum{
    xPWM_CSFAB_NONE     =0,
    xPWM_CSFAB_LOW      =1,
    xPWM_CSFAB_HIGH     =2,
    xPWM_CSFAB_INVALID  =3,
}xPWM_CSFABx;

typedef struct{
    xPWM_CSFABx xPWM_CSFB;
    xPWM_CSFABx xPWM_CSFA;
}xPWM_AQCSFRCInit_TypeDef;

typedef enum{
    xPWM_IN_MODE_EPWMxA_FALLING_A_RISING=0,
    xPWM_IN_MODE_EPWMxA_FALLING_B_RISING=1,
    xPWM_IN_MODE_EPWMxB_FALLING_A_RISING=2,
    xPWM_IN_MODE_EPWMxB_FALLING_B_RISING=0,
}xPWM_IN_MODEx;

typedef enum{
    xPWM_POLSEL_NOOPPOSITEEPWMxAB=0,
    xPWM_POLSEL_OPPOSITEEPWMxA   =1,
    xPWM_POLSEL_OPPOSITEEPWMxB   =2,
    xPWM_POLSEL_OPPOSITEEPWMxAB  =3,
}xPWM_POLSELx;

typedef enum{
    xPWM_OUT_MODE_CLOSE         =0,
    xPWM_OUT_MODE_FALLING       =1,
    xPWM_OUT_MODE_RISING        =2,
    xPWM_OUT_MODE_FALLINGRISING =3,
}xPWM_OUT_MODEx;

typedef struct{
    xPWM_IN_MODEx xPWM_IN_MODE;
    xPWM_POLSELx xPWM_POLSEL;
    xPWM_OUT_MODEx xPWM_OUT_MODE;
}xPWM_DBCTLInit_TypeDef;

typedef enum{
    xPWM_OSHTMOD_IMMEDIATELY    =0,
    xPWM_OSHTMOD_CTRZERO        =1,
}xPWM_OSHTMODx;

typedef enum{
    xPWM_CBCMOD_IMMEDIATELY     =0,
    xPWM_CBCMOD_CTRZERO         =1,
}xPWM_CBCMODx;

typedef enum{
    xPWM_TZx_n_CLOSE            =0,
    xPWM_TZx_n_OPEN             =1,
}xPWM_TZx_n;

typedef struct{
    xPWM_TZx_n xPWM_TZx_CBC[7];
    xPWM_CBCMODx xPWM_CBCMOD;
    xPWM_TZx_n xPWM_TZx_OSHT[7];
    xPWM_OSHTMODx xPWM_OSHTMOD;
}xPWM_TZSELInit_TypeDef;

typedef enum{
    xPWM_TZn_EN_DISABLE =0,
    xPWM_TZn_EN_ENABLE  =1,
}xPWM_TZn_ENx;

typedef enum{
    xPWM_TZn_SEL_HIGH   =0,
    xPWM_TZn_SEL_LOW    =1,
}xPWM_TZn_SELx;

typedef enum{
    xPWM_TZx_HIGHZ  =0,
    xPWM_TZx_HIGH   =1,
    xPWM_TZx_LOW    =2,
    xPWM_TZx_NONE   =3,
}xPWM_TZABx;

typedef struct{
    xPWM_TZn_ENx xPWM_TZn_EN[7];
    xPWM_TZn_SELx xPWM_TZn_SEL[7]; 
    xPWM_TZABx xPWM_TZB;
    xPWM_TZABx xPWM_TZA;
}xPWM_TZCTLInit_TypeDef;

typedef enum{
    xPWM_SOCABEN_DISABLE    =0,
    xPWM_SOCABEN_ENABLE     =1,
}xPWM_SOCABENx;

typedef enum{
	xPWM_SOCA=0,
	xPWM_SOCB=1,
}xPWM_SOCx;

typedef enum{
    xPWM_SOCABSEL_ZERO      =1,
    xPWM_SOCABSEL_PRD       =2,
    xPWM_SOCABSEL_ZERO_PRD  =3,
    xPWM_SOCABSEL_CMPAINC   =4,
    xPWM_SOCABSEL_CMPADEC   =5,
    xPWM_SOCABSEL_CMPBINC   =6,
    xPWM_SOCABSEL_CMPBDEC   =7,
    xPWM_SOCABSEL_CPR1INC   =8,
    xPWM_SOCABSEL_CPR1DEC   =9,
    xPWM_SOCABSEL_CPR2INC   =0XA,
    xPWM_SOCABSEL_CPR2DEC   =0XB,
}xPWM_SOCABSELx;

typedef enum{
    xPWM_INTEN_DISABLE  =0,
    xPWM_INTEN_ENABLE   =1,
}xPWM_INTENx;

typedef enum{
    xPWM_INTSEL_ZERO    =1,
    xPWM_INTSEL_PRD     =2,
    xPWM_INTSEL_ZERO_PRD=3,
    xPWM_INTSEL_CMPAINC =4,
    xPWM_INTSEL_CMPADEC =5,
    xPWM_INTSEL_CMPBINC =6,
}xPWM_INTSELx;

typedef struct{
    xPWM_SOCABENx xPWM_SOCBEN;
    xPWM_SOCABSELx xPWM_SOCBSEL;  
    xPWM_SOCABENx xPWM_SOCAEN;
    xPWM_SOCABSELx xPWM_SOCASEL;
    xPWM_INTENx xPWM_INTEN;
    xPWM_INTSELx xPWM_INTSEL;
}xPWM_ETSELInit_TypeDef;


typedef enum{
    xPWM_INT_SOCABCNT_0=0,
    xPWM_INT_SOCABCNT_1=1,
    xPWM_INT_SOCABCNT_2=2,
    xPWM_INT_SOCABCNT_3=3,
}xPWM_INT_SOCABCNTx;

typedef enum{
    xPWM_INT_SOCABPRD_NONE      =0,
    xPWM_INT_SOCABPRD_SOCABCNT01=1,
    xPWM_INT_SOCABPRD_SOCABCNT10=2,
    xPWM_INT_SOCABPRD_SOCABCNT11=3,   
}xPWM_INT_SOCABPRDx;

typedef struct{
    xPWM_INT_SOCABCNTx xPWM_SOCBCNT;
    xPWM_INT_SOCABPRDx xPWM_SOCBPRD;
    xPWM_INT_SOCABCNTx xPWM_SOCACNT;
    xPWM_INT_SOCABPRDx xPWM_SOCAPRD;
    xPWM_INT_SOCABCNTx xPWM_INTCNT;
    xPWM_INT_SOCABPRDx xPWM_INTPRD;
}xPWM_ETPSInit_TypeDef;

typedef enum{
    xPWM_ETSTATUS_SOCB=3,
    xPWM_ETSTATUS_SOCA=2,
    xPWM_ETSTATUS_INT =0
}xPWM_ETSTATUSx;

typedef enum{
    xPWM_SOCAB_NONE=0,
    xPWM_SOCAB_FORCE=1,
}xPWM_SOCABx;

typedef struct{
    xPWM_SOCABx xPWM_SOCB;
    xPWM_SOCABx xPWM_SOCA;
}xPWM_ETFRCInit_TypeDef;

typedef struct{
    unsigned short xPWMX_TBPRD;  
    unsigned short xPWM_CMPA;
    unsigned short xPWM_CMPB;
    unsigned short xPWM_CPR1;
    unsigned short xPWM_CPR2;
    unsigned short xPWM_DBRED;
    unsigned short xPWM_DBFED;
    unsigned short xPWM_TBPHS;
    xPWM_TBCTLInit_TypeDef xPMW_TBCTL;
    
    xPWM_CMPCTLInit_TypeDef xPWM_CMPCTL;
    
    xPWM_AQCTLABInit_TypeDef xPWM_AQCTLA;
    xPWM_AQCTLABInit_TypeDef xPWM_AQCTLB;
    
    xPWM_AQSFRCInit_TypeDef xPWM_AQSFRC;
    xPWM_AQCSFRCInit_TypeDef xPWM_AQCSFRC;
    xPWM_DBCTLInit_TypeDef xPWM_DBCTL;
    xPWM_TZSELInit_TypeDef xPWM_TZSEL;
    xPWM_TZCTLInit_TypeDef xPWM_TZCTL;
    xPWM_ETSELInit_TypeDef xPWM_ETSEL;
    xPWM_ETPSInit_TypeDef xPWM_ETPS;
    xPWM_ETFRCInit_TypeDef xPWM_ETFRC;
}xPWMInit_TypeDef;


void xPWM_INTClear(xPWM_TypeDef* xPWMx);
void xPWM_TZEINT(xPWM_TypeDef* xPWMx,xPWM_TZINTx xpwmtzint,FunctionalState cmd);
void xPWM_ETCLRClear(xPWM_TypeDef* xPWMx,xPWM_ETSTATUSx xpwm_etstatus);
void xPWM_TZCLR(xPWM_TypeDef* xPWMx,xPWM_TZCLRx xpwmtzclr);
void xPWM_START(xPWM_TypeDef* xPWMx);
void xPWM_SOC(xPWM_TypeDef* xPWMx,xPWM_SOCx socx,xPWM_SOCABSELx socsel);
void xPWM_STOP(xPWM_TypeDef* xPWMx);
void xPWM_CMPAConfig(xPWM_TypeDef* xPWMx,unsigned short);
void xPWM_CPR1Config(xPWM_TypeDef* xPWMx,unsigned short);
void xPWM_Init(xPWM_TypeDef* xPWMx,xPWMInit_TypeDef* xPWMInit_Struct);


#ifdef __cplusplus
}
#endif


#endif /*_CI100X_xPWM_H*/
