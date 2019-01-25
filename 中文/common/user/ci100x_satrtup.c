/**
  ******************************************************************************
  * @文件    ci100x_startup.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件用于chipintelli公司的CI100X芯片的系统启动 向量表相关设置.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 

#include "ci100x_it.h"
typedef union {
    void(*__fun)(void);
    void* __ptr;
}intvec_elem;

void __ci100x_iar_program_start(void){
	SystemInit();
	__iar_program_start();
}
#pragma language = extended
#pragma segment = "CSTACK"

#pragma location = ".intvec"
__root const intvec_elem __vector_table[64]=
{
  	{.__ptr = __sfe("CSTACK")},
	__ci100x_iar_program_start,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	IWDG_IRQHandler,
	FPU_IRQHandler,
	xPWM_IRQHandler,
	xPWM_TZ_IRQHandler,
	ADC_IRQHandler,
	VAD_IRQHandler,
	ASR_IRQHandler,
	CAN_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
 	EXTI3_IRQHandler,
	DMA_IRQHandler,
	TIM0_IRQHandler,
	TIM1_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	GPIO0_IRQHandler,
	UART0_IRQHandler,                    
	UART1_IRQHandler,
	SUART0_IRQHandler,
	SUART1_IRQHandler,
	I2C0_IRQHandler,
	I2C1_IRQHandler,
	SCU_IRQHandler,  
	IISDMA_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler, 
	EXTI4_IRQHandler, 
	GPIO1_IRQHandler,  
	IR_IRQHandler,                   
};

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/