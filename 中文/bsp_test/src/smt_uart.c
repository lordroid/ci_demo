#include "smt_uart.h"
#include "ci100x_dma.h"
/*UART  Polling(查询)模式*/
void UARTPollingConfig(UART_TypeDef* UARTx)
{
    Scu_SetDeviceGate((unsigned int)UARTx,ENABLE);
    if(UARTx == UART0)
    {
      Scu_SetIOReuse(UART0_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART0_TX_PAD,FIRST_FUNCTION );
    }
    else if(UARTx==UART1)
    {
      Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
      //Scu_SetIOReuse(UART1_CTS_PAD,FIRST_FUNCTION );
      //Scu_SetIOReuse(UART1_RTS_PAD,FIRST_FUNCTION );
    }
    
    UART_EN(UARTx,DISABLE);
    UART_CRConfig(UARTx,UART_TXE,ENABLE);
    UART_CRConfig(UARTx,UART_RXE,ENABLE);
    UART_BAUDRATEConfig(UARTx,UART_BaudRate115200);
    
    UART_TXFIFOByteWordConfig(UARTx,UART_Byte);
    UART_LCRConfig(UARTx,UART_WordLength_8b,UART_StopBits_1,UART_Parity_No);
    UART_RXFIFOConfig(UARTx,UART_FIFOLevel1_4);
    UART_TXFIFOConfig(UARTx,UART_FIFOLevel1_8);
	
    UART_IntClear(UARTx,UART_AllInt);
    UART_IntMaskConfig(UARTx,UART_AllInt,ENABLE);
    UART_FIFOClear(UARTx);
    UART_CRConfig(UARTx,UART_NCED,DISABLE);
    UART_EN(UARTx,ENABLE);
}

/*UART Interrupt (中断)模式*/

void UARTInterruptConfig(UART_TypeDef* UARTx)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = (UARTx == UART0)?UART0_IRQn:UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    Scu_SetDeviceGate((unsigned int)UARTx,ENABLE);
    if(UARTx==UART0)
    {
      Scu_SetIOReuse(UART0_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART0_TX_PAD,FIRST_FUNCTION );
    }
    else if(UARTx==UART1)
    {
      Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_CTS_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_RTS_PAD,FIRST_FUNCTION );
    }
 
    UART_EN(UARTx,DISABLE);
    UART_BAUDRATEConfig(UARTx,UART_BaudRate38400);
    UART_CRConfig(UARTx,UART_TXE,ENABLE);
    UART_CRConfig(UARTx,UART_RXE,ENABLE);
    UART_IntClear(UARTx,UART_AllInt);
    UART_IntMaskConfig(UARTx,UART_AllInt,ENABLE);
    UART_TXFIFOByteWordConfig(UARTx,UART_Byte);
    UART_LCRConfig(UARTx,UART_WordLength_8b,UART_StopBits_1,UART_Parity_No);
    UART_RXFIFOConfig(UARTx,UART_FIFOLevel1);
    UART_TXFIFOConfig(UARTx,UART_FIFOLevel1_8);
    UART_FIFOClear(UARTx);
    UART_CRConfig(UARTx,UART_NCED,ENABLE);
    UART_IntMaskConfig(UARTx,UART_TXInt,ENABLE);/*if TX FIFO is empty,then TX int coming */
    UART_IntMaskConfig(UARTx,UART_RXInt,DISABLE);
    UART_EN(UARTx,ENABLE);
}

/*UART DMA 模式*/
void UARTDMAConfig(UART_TypeDef* UARTx)
{
  	char tx[]={"UART DMA TEST DATE:16/4/19,@chipintelli\n"};
//	char rx[30]={0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};
	
    NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    Scu_SetDeviceGate((unsigned int)DMA_CTRL_BASE,ENABLE);
    Scu_SetDeviceGate((unsigned int)UARTx,ENABLE);
    
    if(UARTx == UART0)
    {
      Scu_SetIOReuse(UART0_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART0_RX_PAD,FIRST_FUNCTION );
    }
    else if(UARTx == UART1)
    {
      Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_RX_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_CTS_PAD,FIRST_FUNCTION );
      Scu_SetIOReuse(UART1_RTS_PAD,FIRST_FUNCTION );
    }

    UART_EN(UARTx,DISABLE);
	
    UART_CRConfig(UARTx,UART_TXE,ENABLE);
    UART_CRConfig(UARTx,UART_RXE,ENABLE);
    UART_BAUDRATEConfig(UARTx,UART_BaudRate38400);
    UART_TXFIFOByteWordConfig(UARTx,UART_Word);
    UART_LCRConfig(UARTx,UART_WordLength_8b,UART_StopBits_1,UART_Parity_No);
    UART_RXFIFOConfig(UARTx,UART_FIFOLevel1);
    UART_TXFIFOConfig(UARTx,UART_FIFOLevel1_2);
	
    UART_IntClear(UARTx,UART_AllInt);
    UART_IntMaskConfig(UARTx,UART_AllInt,ENABLE);
    UART_FIFOClear(UARTx);
    UART_CRConfig(UARTx,UART_NCED,ENABLE);
    UART_TXRXDMAConfig(UARTx,UART_RXDMA);
    UART_TXRXDMAConfig(UARTx,UART_TXDMA);
    UART_EN(UARTx,ENABLE);
    DMAC_M2P_P2MConfig(DMACChannel1,DMAC_Peripherals_UART1_TX,M2P_DMA,\
       (unsigned int)tx,UART1_DMA_ADDR,sizeof(tx)/sizeof(*tx));
    //DMAC_M2P_P2MConfig(DMACChannel1,DMAC_Peripherals_UART1_RX,P2M_DMA,\
    //UART1_DMA_ADDR,(unsigned int)UartInterruptReceiveDataBuf,32);
}
	
