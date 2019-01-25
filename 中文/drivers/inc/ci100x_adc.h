/**
  ******************************************************************************
  * @文件    ci100x_adc.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的adc模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */

#ifndef __CI100X_ADC_H
#define __CI100X_ADC_H

#ifdef __cplusplus
extern "C"{
#endif

#include "smt.h"  
  
/*ADC寄存器定义*/
typedef struct
{
  volatile unsigned int ADCCTRL;//0x00
  volatile unsigned int ADCINTMASK;
  volatile unsigned int ADCINTFLG;
  volatile unsigned int ADCINTCLR;
  volatile unsigned int ADCSOFTSOC;//0x10
  volatile unsigned int ADCSOCCTRL;
  volatile unsigned int ADCRESULT[8];
  volatile unsigned int CHxPERIOD[4];
  volatile unsigned int CH0MINVALUE;
  volatile unsigned int CH0MAXVALUE;
  volatile unsigned int CH1MINVALUE;
  volatile unsigned int CH1MAXVALUE;
  volatile unsigned int CH2MINVALUE;
  volatile unsigned int CH2MAXVALUE;
  volatile unsigned int CH3MINVALUE;
  volatile unsigned int CH3MAXVALUE;
  volatile unsigned int ADCSTAT;
}ADC_TypeDef;  

#define ADC 					((ADC_TypeDef* )ADC_BASE)

typedef enum{
	ADC_Channel_0	=0,
	ADC_Channel_1	=1,
  	ADC_Channel_2	=2,
	ADC_Channel_3	=3,
	ADC_Channel_4	=4,
	ADC_Channel_5	=5,
	ADC_Channel_6	=6,
	ADC_Channel_7	=7,
	ADC_Channel_ALL	=-1,
}ADC_Channelx;

typedef enum{
	ADC_CLKcycle_1	=0,
	ADC_CLKcycle_2	=1,
	ADC_CLKcycle_3	=3,
	ADC_CLKcycle_4	=4,
	ADC_CLKcycle_5	=5,
}ADC_CLKcyclex;

void ADC_PowerCtrl(FunctionalState cmd);
void ADC_ContinuousConvert(FunctionalState cmd);
void ADC_Calibrate(FunctionalState cmd);
void ADC_PeriodMonitor(ADC_Channelx channel,FunctionalState cmd);
void ADC_PeriodAble(FunctionalState cmd);
void ADC_IntSel(FunctionalState cmd);
void ADC_ChannelMinvalueInt(ADC_Channelx channel,FunctionalState cmd);
void ADC_ChannelMaxvalueInt(ADC_Channelx channel,FunctionalState cmd);
void ADC_MaskInt(FunctionalState cmd);	
unsigned int ADC_IntFlag(ADC_Channelx channel);
void ADC_IntClear(ADC_Channelx channel);	
void ADC_SOCSoftCtrl(ADC_Channelx channel);
void ADC_CovertConfig(ADC_Channelx channel,ADC_CLKcyclex holdtime);
unsigned short ADC_GetResult(ADC_Channelx channel);
void ADC_CHxPeriod(ADC_Channelx channel,unsigned short period);
void ADC_ChannelMinvalue(ADC_Channelx channel,unsigned short val);
void ADC_ChannelMaxvalue(ADC_Channelx channel,unsigned short val);  
int ADC_Status(void);	
	
#ifdef __cplusplus
}
#endif

#endif /*__CI100X_ADC_H*/
