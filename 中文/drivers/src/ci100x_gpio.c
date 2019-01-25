/**
  ******************************************************************************
  * @文件    ci100x_gpio.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的GPIO模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  
#include "ci100x_gpio.h"
/**
  * @功能:读取GPIO某个引脚的数据寄存器 位带方式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  * 	  2.gpio_num,GPIO_Pin脚编号，即GPIO_Pin0~31。
  * @返回值:0，低电平；1，高电平；PARA_ERROR，参数错误
  */
 

/**
  * @功能:读取GPIO某个引脚的数据寄存器
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  * 	  2.gpio_num,GPIO_Pin脚编号，即GPIO_Pin0~31。
  * @返回值:0，低电平；1，高电平；PARA_ERROR，参数错误
  */
char GPIO_ReadDATA(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if(gpio_num < 32 && GPIOx)
    {
      return ((GPIOx->GPIODATA & (1 << gpio_num))?1:0);
    }
    
    return PARA_ERROR; 
}
/**
  * @功能:设置GPIO某个脚的数据寄存器
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31,
  *	      3.val:0,设置低电平；1(非零)，设置高电平。
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_DATA_Set(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num,char val)
{
    if((gpio_num < 32) && GPIOx)
    {
      if(!val)
      {
        GPIOx->GPIODATA &= ~(1 << gpio_num);
      }
      else
      {
        GPIOx->GPIODATA |= (1 << gpio_num);
      }
    }
    else
    {
      return PARA_ERROR;
    }
    return RETRUN_OK;
}
int GPIO_DATA_Inv(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if(gpio_num < 32 && GPIOx)
    {
        if(GPIO_ReadDATA(GPIOx,gpio_num))
            GPIO_DATA_Set(GPIOx,gpio_num,0);
        else
			GPIO_DATA_Set(GPIOx,gpio_num,1);		
        return RETRUN_OK;
	}
	else
		return PARA_ERROR;
}
/**
  * @功能:GPIO某个脚的中断标志状态
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31。
  * @返回值:0:无中断标志，非零：有中断标志
  */
int GPIO_INTStatus(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if((gpio_num < 32 )&& GPIOx)
    {
      return (GPIOx->GPIOMIS & (1 << gpio_num));
    }
    return PARA_ERROR;
}
/**
  * @功能:清除GPIO某个引脚的中断标志
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31。
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_INTClear(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if((gpio_num < 32 )&& GPIOx)
    {
      GPIOx->GPIOIC |= (1 << gpio_num);
    }
    else
    {
      return PARA_ERROR;
    }
    return RETRUN_OK;
}
/**
  * @功能:配置GPIO某个引脚的输出模式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31，
  *	      3.val:设置GPIO输出的高低电平，0：低电平，非零：高电平。
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_Output(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num,char val)
{
    if((gpio_num < 32) && GPIOx)
    {
      GPIOx->GPIOEN |= (1 << gpio_num);
      GPIOx->GPIODIR |= (1 << gpio_num);
      if(!val)
      {
        GPIOx->GPIODATA &= ~(1 << gpio_num);
      }
      else
      {
        GPIOx->GPIODATA |= (1 << gpio_num);
      }
    }
    else
    {
      return PARA_ERROR;
    }
    return RETRUN_OK;
}
/**
  * @功能:配置GPIO某引脚为输入模式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31。
  * @返回值:读回的引脚状态：0，低电平，1：高电平，PARA_ERROR：参数错误
  */
int GPIO_Input(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if(gpio_num < 32 && GPIOx)
    {
      GPIOx->GPIOEN |= (1 << gpio_num);
      GPIOx->GPIODIR &= ~(1 << gpio_num);
      return ((GPIOx->GPIODATA & (1 << gpio_num))?1:0);
    }
    else
    {
      return PARA_ERROR;
    }
}

/**
  * @功能:设置GPIO某个引脚的中断电平触发模式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31，
  *	      3.high_low:GPIO_LOW：低电平触发中断，GPIO_HIGH:高电平触发中断
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_Level_INT(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num,char high_low)
{
    if(gpio_num < 32 && GPIOx)
    {
      GPIOx->GPIOEN |= (1 << gpio_num);
      GPIOx->GPIODIR &= ~(1 << gpio_num);
      GPIOx->GPIOIS |= (1 << gpio_num);
      if(!high_low)
      {
        GPIOx->GPIOIEV &= ~(1 << gpio_num);
      }
      else
      {
        GPIOx->GPIOIEV |= ((high_low & 0X1) << gpio_num);
      }
      GPIOx->GPIOIC |= (1 << gpio_num);
      GPIOx->GPIOIE |= (1 << gpio_num);
      return RETRUN_OK;
    }
    return PARA_ERROR;
}
/**
  * @功能:设置GPIO某个引脚为双沿中断触发模式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31，
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_Bothedges_INT(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num)
{
    if(gpio_num < 32 && GPIOx)
    {
      GPIOx->GPIOEN |= (1 << gpio_num);
      GPIOx->GPIODIR &= ~(1 << gpio_num);
      GPIOx->GPIOIS &= ~(1 << gpio_num);
      GPIOx->GPIOIBE |= (1 << gpio_num);
      GPIOx->GPIOIC |= (1 << gpio_num);
      GPIOx->GPIOIE |= (1 << gpio_num);
      return RETRUN_OK;
    }
    return PARA_ERROR;
}
/**
  * @功能:设置GPIO某个引脚中断边沿触发模式
  * @注意:无       
  * @参数:1.GPIOx:GPIO组，即GPIO0、GPIO1,每组最多32个pin，
  *	      2.gpio_num:GPIO_Pin脚编号，即GPIO_Pin0~31，
  *	      3.rising_falling:GPIO_RISING:上沿触发,GPIO_FALLING:下沿触发
  * @返回值:RETRUN_OK、PARA_ERROR（正确/参数错误）
  */
int GPIO_Edges_INT(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num,char rising_falling)
{
    if(gpio_num < 32 && GPIOx)
    {
      GPIOx->GPIOEN |= (1 << gpio_num);
      GPIOx->GPIODIR &= ~(1 << gpio_num);
      GPIOx->GPIOIS &= ~(1 << gpio_num);
      GPIOx->GPIOIBE &= ~(1 << gpio_num);
      if(!rising_falling)
      {
        GPIOx->GPIOIEV &= ~(1 << gpio_num);
      }
      else
      {
        GPIOx->GPIOIEV |= ((rising_falling & 0X1) << gpio_num);
      }
      GPIOx->GPIOIC |= (1 << gpio_num);
      GPIOx->GPIOIE |= (1 << gpio_num);
      return RETRUN_OK;
    }
    return PARA_ERROR;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/