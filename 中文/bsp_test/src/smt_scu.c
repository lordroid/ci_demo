#include "ci100x_scu.h"
#include "ci100x_uart.h"
#include "includes.h"
extern "C" void ADC_normalTest(void);
void scuint_wakeup(void)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};   

    NVIC_InitStructure.NVIC_IRQChannel = SCU_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
    Unlock_Ckconfig();
    SCU->SCU_INT_MASK |= (1<<24); 
    SCU->WAKE_UP_MASK = 0xFFFFFF;
    Lock_Ckconfig();
    /*1.配置唤醒源*/
    /*2.进入低功耗前的准备，eg:关闭外设时钟、降低频率、SDRAM进入自刷新等*/
    /*3.执行 WFI 指令*/
    /*4.唤醒源唤醒CPU,退出WFI,做退出低功耗的准备，eg:配置正常频率、SDRAM进入正常状态*/
 
    asm("WFI");
    /*1.WFI(Wait for interrupt)、WFE(Wait for event)是 
    两个让ARM核进入low-power standby模式的指令;
    2.对WFI来说，执行WFI指令后，ARM core会立即进
    入low-power standby state，直到有WFI Wakeup events发生;
    3.而WFE则稍微不同，执行WFE指令后，根据Event Register（
    一个单bit的寄存器，每个PE一个）的状态，有两种情况：如
    果Event Register为1，该指令会把它清零，然后执行完成（
    不会standby）；如果Event Register为0，和WFI类似，进入
    low-power standby state，直到有WFE Wakeup events发生;
    */
    
    
}