
#include "misc.h"

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  	uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
    
  	if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  	{  
		  tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    	tmppre = (0x4 - tmppriority);
    	tmpsub = tmpsub >> tmppriority;

    	tmppriority = NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    	tmppriority |=  (uint8_t)(NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub);
        
    	tmppriority = tmppriority << 0x04;
        
    	NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
    
    	NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      	(uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  	}
  	else
	{
	  	NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
			(uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
	}
}
void irq_en(unsigned long nv_bit)
{
  if(nv_bit < 32)
    NVIC->ISER[0] |= (unsigned long)(0x1<<nv_bit);
  else
    ;//NVIC->ISER[1] |= 0x1<<(nv_bit - 31);
}

void irq_cl(unsigned long nv_bit)
{
  if(nv_bit <32)
    NVIC->ICER[0] = (unsigned long)(0x1<<nv_bit);
  else
    ;//NVIC->ICER[0] |= 1<<(nv_bit - 31);
}

void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  	SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}


void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
 
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
}

 
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
	/* Check the parameters */
 
	if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
	{
	  SysTick->CTRL |= SysTick_CLKSource_HCLK;
	}
	else
	{
	  SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
	}
 }
