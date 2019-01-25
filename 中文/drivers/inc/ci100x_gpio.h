/**
  ******************************************************************************
  * @文件    ci100x_gpio.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的GPIO模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */

#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "smt.h"
#include "misc.h"
#include "ci100x_scu.h"

#ifndef GPIO0_BASE
#define GPIO0_BASE (unsigned int )0x40047000
#endif
#ifndef GPIO1_BASE
#define GPIO1_BASE (unsigned int )0x40048000
#endif
typedef struct{
	volatile unsigned int  GPIODATA; 
	unsigned int resved[255];
	volatile unsigned int  GPIODIR; 
	volatile unsigned int  GPIOIS; 
    volatile unsigned int  GPIOIBE; 
    volatile unsigned int  GPIOIEV; 
    volatile unsigned int  GPIOIE; 
    volatile unsigned int  GPIORIS; 
    volatile unsigned int  GPIOMIS; 
    volatile unsigned int  GPIOIC; 
    volatile unsigned int  GPIOAFSEL; 
	volatile unsigned int  GPIOEN; 
}GPIO_TypeDef;

#define GPIO0 ((GPIO_TypeDef*)GPIO0_BASE)
#define GPIO1 ((GPIO_TypeDef*)GPIO1_BASE)


typedef enum{
	GPIO_Pin0=0,
	GPIO_Pin1=1,
	GPIO_Pin2=2,
	GPIO_Pin3=3,
	GPIO_Pin4=4,
	GPIO_Pin5=5,
	GPIO_Pin6=6,
	GPIO_Pin7=7,
	GPIO_Pin8=8,
	GPIO_Pin9=9,
	GPIO_Pin10=10,
	GPIO_Pin11=11,
	GPIO_Pin12=12,
	GPIO_Pin13=13,
	GPIO_Pin14=14,
	GPIO_Pin15=15,
	GPIO_Pin16=16,
	GPIO_Pin17=17,
	GPIO_Pin18=18,
	GPIO_Pin19=19,
	GPIO_Pin20=20,
	GPIO_Pin21=21,
	GPIO_Pin22=22,
	GPIO_Pin23=23,
	GPIO_Pin24=24,
	GPIO_Pin25=25,
	GPIO_Pin26=26,
	GPIO_Pin27=27,
	GPIO_Pin28=28,
	GPIO_Pin29=29,
	GPIO_Pin30=30,
	GPIO_Pin31=31,
}GPIO_Pinx;	

typedef enum{
	GPIO_HIGH=1,
	GPIO_LOW=0,
  	GPIO_FALLING=0,
	GPIO_RISING=1,
}GPIO_STATE;	
	
int GPIO_Output(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num,char val);
int GPIO_Input(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num);
int GPIO_Level_INT(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num,char high_low);
int GPIO_Bothedges_INT(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num);
int GPIO_Edges_INT(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num,char rising_falling);
int GPIO_DATA_Set(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num,char val);
int GPIO_DATA_Inv(GPIO_TypeDef* GPIOx,GPIO_Pinx gpio_num);
char GPIO_ReadDATA(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num);
int GPIO_INTStatus(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num);
int GPIO_INTClear(GPIO_TypeDef*GPIOx,GPIO_Pinx gpio_num);
#define GPIO_PinBitband(addr,bit) (*(volatile unsigned int*)(0x42000000UL+(((addr) &0xFFFFF)<<5)+((bit)<<2)))    


#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H*/