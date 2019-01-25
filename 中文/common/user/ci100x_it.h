#ifndef __CI100x_IT_H
#define __CI100x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "smt.h" 
#define LOOP_BUF_SPACE(head, tail, size) (((head) - ((tail)+1)) & ((size)))

void __ci100x_iar_program_start(void);
extern void __iar_program_start(void);

extern void SVC_Handler(void);
extern void PendSV_Handler(void);

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void WWDG_IRQHandler(void);
void IWDG_IRQHandler(void);
void FPU_IRQHandler(void);
void xPWM_IRQHandler(void);
void xPWM_TZ_IRQHandler(void);
void ADC_IRQHandler(void);

void CAN_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void DMA_IRQHandler(void);

void TIM0_IRQHandler(void);
void TIM1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void GPIO0_IRQHandler(void);
void UART0_IRQHandler(void);                    
void UART1_IRQHandler(void);
void SUART0_IRQHandler(void);
void SUART1_IRQHandler(void);
void I2C0_IRQHandler(void);
void I2C1_IRQHandler(void);
void SCU_IRQHandler(void);  
void IISDMA_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void); 
void EXTI4_IRQHandler(void); 
void GPIO1_IRQHandler(void);  
void IR_IRQHandler(void);

#ifdef __cplusplus
}
#endif 

extern void VAD_IRQHandler(void);
extern void ASR_IRQHandler(void);
extern void SystemInit(void);
extern void SysTick_Handler(void);

#endif /* __CI100x_IT_H */
