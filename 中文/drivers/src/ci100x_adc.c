/**
  ******************************************************************************
  * @文件    ci100x_adc.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的ADC模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_adc.h"

/**
  * @功能:ADC电路使能
  * @注意:无       
  * @参数:1.cmd ENABLE ,ADC power on;DISABLE ,ADC power off
  * @返回值:无
  */
void ADC_PowerCtrl(FunctionalState cmd)
{
  	if(cmd != ENABLE)
        {
  		ADC->ADCCTRL &= ~(1<<0);
	}
        else
        {
		ADC->ADCCTRL |= (1<<0);
	}
}

/**
  * @功能:ADC连续转换使能
  * @注意:无       
  * @参数:1.cmd ENABLE ,连续转换;DISABLE ,单次转换
  * @返回值:无
  */
void ADC_ContinuousConvert(FunctionalState cmd)
{
  	if(cmd != ENABLE)
    {
  		ADC->ADCCTRL &= ~(1 << 1);
	}
    else
    {
		ADC->ADCCTRL |= (1 << 1);
	}	
}

/**
  * @功能:ADC校准使能
  * @注意:无       
  * @参数:1 cmd ENABLE 使能，DISABLE 关闭
  * @返回值:无
  */
void ADC_Calibrate(FunctionalState cmd)
{
  	if(cmd == ENABLE)
    {
		ADC->ADCCTRL |= (1 << 3);
	}
    else
    {
		ADC->ADCCTRL &= ~(1 << 3);
	}
}

/**
  * @功能:ADC通道周期监测使能(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 cmd ENABLE 使能，DISABLE 关闭
  * @返回值:无
  */
void ADC_PeriodMonitor(ADC_Channelx channel,FunctionalState cmd)
{
  	if(cmd != DISABLE)
    {
  		ADC->ADCCTRL |= (1 << (channel + 4));
	}
    else
    {
		ADC->ADCCTRL &= ~(1 << (channel + 4));
	}
}

/**
  * @功能:ADC周期监测使能(只能工作于单次采样模式下,(只支持ADC_Channel_0-ADC_Channel_3))
  * @注意:无       
  * @参数:1 cmd ENABLE 使能，DISABLE 关闭
  * @返回值:无
  */
void ADC_PeriodAble(FunctionalState cmd)
{
  	if(cmd != DISABLE)
    {
  		ADC->ADCCTRL |= (1 << 8);
	}
    else
    {
		ADC->ADCCTRL &= ~(1 << 8);
	}
}

/**
  * @功能:ADC采样值异常（超过阈值）中断使能(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 cmd ENABLE 使能(采样值超阀值时产生中断)，DISABLE 关闭(每次采样结束产生中断)
  * @返回值:无
  */
void ADC_IntSel(FunctionalState cmd)
{
  	if(cmd != DISABLE)
    {
  		ADC->ADCCTRL |= (1 << 12);
	}else{
		ADC->ADCCTRL &= ~(1 << 12);
	}
}

/**
  * @功能:ADC通道采样结果阀值下限中断使能(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 cmd ENABLE 使能，DISABLE 关闭
  * @返回值:无
  */
void ADC_ChannelMinvalueInt(ADC_Channelx channel,FunctionalState cmd)
{
  	if(cmd != DISABLE)
        {
  		ADC->ADCCTRL |= (1 << (channel + 16));
	}
        else
        {
		ADC->ADCCTRL &= ~(1 << (channel + 16));
	}
}

/**
  * @功能:ADC通道采样结果阀值上限中断使能(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 cmd ENABLE 使能，DISABLE 关闭
  * @返回值:无
  */
void ADC_ChannelMaxvalueInt(ADC_Channelx channel,FunctionalState cmd)
{
  	if(cmd != DISABLE)
        {
  		ADC->ADCCTRL |= (1 << (channel + 20));
	}
        else
        {
		ADC->ADCCTRL &= ~(1 << (channel + 20));
	}
}

/**
  * @功能:ADC中断屏蔽设置
  * @注意:无       
  * @参数:1 cmd ENABLE 屏蔽中断，DISABLE 不屏蔽中断
  * @返回值:无
  */
void ADC_MaskInt(FunctionalState cmd)
{
  	if(cmd != DISABLE)
        {
  		ADC->ADCINTMASK |= (1 << 0);
	}
        else
        {
		ADC->ADCINTMASK &= ~(1 << 0);
	}
}

/**
  * @功能:ADC中断标志
  * @注意:无       
  * @参数:1 channel,adc通道
  * @返回值:0,该通道无中断，非0该通道有中断
  */
unsigned int ADC_IntFlag(ADC_Channelx channel)
{
	return ADC->ADCINTFLG & (1 << channel);
}

/**
  * @功能:ADC中断标志
  * @注意:无       
  * @参数:1 channel,adc通道
  * @返回值:无
  */
void ADC_IntClear(ADC_Channelx channel)
{
  	if(ADC_Channel_ALL == channel)
        {
  		ADC->ADCINTCLR |= (1 << 8);
	}
        else
        {
  		ADC->ADCINTCLR |= (1 << channel);
	}
}

/**
  * @功能:ADC软件触发转换
  * @注意:无       
  * @参数:1 channel,adc通道
  * @返回值:无
  */
void ADC_SOCSoftCtrl(ADC_Channelx channel)
{
	ADC->ADCSOFTSOC = 1;//(1<<channel);
}

/**
  * @功能:ADC 转换通道和采用保持时配置
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 holdtime 采样保持时间
  * @返回值:无
  */
void ADC_CovertConfig(ADC_Channelx channel,ADC_CLKcyclex holdtime)
{
	ADC->ADCSOCCTRL = (channel << 5)|(holdtime << 12);
}

/**
  * @功能:ADC 通道的转换结果
  * @注意:无       
  * @参数:1 channel,adc通道
  * @返回值:转换结果
  */
unsigned short ADC_GetResult(ADC_Channelx channel)
{
	return ADC->ADCRESULT[channel] & 0xFFF;
}

/**
  * @功能:ADC 通道采样周期配置(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 period 采样周期值 通道采样周期：(period+1) * 512  
  * @返回值:无
  */
void ADC_CHxPeriod(ADC_Channelx channel,unsigned short period)
{
	ADC->CHxPERIOD[channel] = period & 0XFFFF;
}

/**
  * @功能:ADC 通道下限阀值配置(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 val下限阀值 
  * @返回值:无
  */
void ADC_ChannelMinvalue(ADC_Channelx channel,unsigned short val)
{
  	if(ADC_Channel_0 == channel)
        {
  		ADC->CH0MINVALUE = val & 0xFFFF;
  	}
        else if(ADC_Channel_1 == channel)
        {
  		ADC->CH1MINVALUE = val & 0xFFFF;
  	}else if(ADC_Channel_2 == channel)
        {
  		ADC->CH2MINVALUE = val & 0xFFFF;
  	}else if(ADC_Channel_3 == channel)
        {
  		ADC->CH3MINVALUE = val & 0xFFFF;
	}
        else
        {
		return ;
	}
}

/**
  * @功能:ADC 通道上限阀值配置(只支持ADC_Channel_0-ADC_Channel_3)
  * @注意:无       
  * @参数:1 channel,adc通道
  *       2 val下限阀值 
  * @返回值:无
  */
void ADC_ChannelMaxvalue(ADC_Channelx channel,unsigned short val)
{
  	if(ADC_Channel_0 == channel)
        {
  		ADC->CH0MAXVALUE = val & 0xFFFF;
  	}else if(ADC_Channel_1 == channel)
        {
  		ADC->CH1MAXVALUE = val & 0xFFFF;
  	}else if(ADC_Channel_2 == channel)
        {
  		ADC->CH2MAXVALUE = val & 0xFFFF;
  	}else if(ADC_Channel_3 == channel)
        {
  		ADC->CH3MAXVALUE = val & 0xFFFF;
	}
        else
        {
		return ;
	}
}

/**
  * @功能:ADC 状态	
  * @注意:无       
  * @参数:无
  * @返回值:0 空闲状态，非0 busy状态
  */
int ADC_Status(void)
{
	return ADC->ADCSTAT & (1 << 0);
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/