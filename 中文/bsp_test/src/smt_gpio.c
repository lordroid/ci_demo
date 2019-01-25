#include "smt_gpio.h"
#include "ci100x_scu.h"


void gpio_output_test(void)
{
  	Scu_Setdevice_Reset((unsigned int)GPIO0);
	Scu_Setdevice_Reset((unsigned int)GPIO1);
	Scu_Setdevice_ResetRelease((unsigned int)GPIO0);
	Scu_Setdevice_ResetRelease((unsigned int)GPIO1);
  	Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
	Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);
	Scu_SetIOReuse(UART0_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART1_TX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART1_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART1_CTS_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART1_RTS_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART2_TX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART3_TX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(UART3_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(IR_TX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(IR_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(CAN_TX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(CAN_RX_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2C0_SCL_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2C0_SDA_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2C1_SCL_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2C1_SDA_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S_MCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S0_SCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S0_LRCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S0_SDI_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S1_SCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S1_LRCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S1_SDI_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S2_SDI_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S2_SDO_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S2_LRCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(I2S2_SCLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM0_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM1_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM2_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM3_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM4_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(XPWM5_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(PWM6_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(PWM7_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(PWM8_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI1_DOUT_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI1_DIN_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI1_CS_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI1_CLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI2_DIN_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI2_DOUT_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI2_CS_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(SPI2_CLK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(JTAG_TDI_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(JTAG_TDO_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(JTAG_TMS_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(JTAG_TCK_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(TRST_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN0_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN1_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN2_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN3_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN4_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN5_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN6_PAD,THIRD_FUNCTION);
	Scu_SetIOReuse(AIN7_PAD,THIRD_FUNCTION);
        
	for(unsigned int i = 0; i < 32; i++)
        {
		GPIO_Output(GPIO0,(GPIO_Pinx)i,1);
	}
	
	for(unsigned int i = 0;i < 57-32; i++)
        {
		GPIO_Output(GPIO1,(GPIO_Pinx)i,1);
	}
	
	while(1)
        {
		unsigned int tmp = 0x5ffff;
		while(tmp--);
		for(int i = 0; i < 32; i++){
		  	GPIO_DATA_Set(GPIO0,(GPIO_Pinx)i,0);
			if(i < 57-32)
                        {
			  GPIO_DATA_Set(GPIO1,(GPIO_Pinx)i,0);
			}
		}
		tmp = 0x5ffff;
		while(tmp--);
		for(int i = 0;i < 32; i++)
                {
		  	GPIO_DATA_Set(GPIO0,(GPIO_Pinx)i,1);
			if(i < 57-32)
                        {
			  GPIO_DATA_Set(GPIO1,(GPIO_Pinx)i,1);
			}
		}
	}
	
}

#if 0
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);
    Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
 
    PinPad_Name IO_PAD[] = 
    {
        UART0_RX_PAD,
        UART1_CTS_PAD,
        UART1_RTS_PAD,
        UART2_TX_PAD,
        UART2_RX_PAD,
        UART3_TX_PAD,
        UART3_RX_PAD,
        IR_TX_PAD,
        IR_RX_PAD,
        CAN_TX_PAD,
        CAN_RX_PAD,
        I2C0_SCL_PAD,
        I2C0_SDA_PAD,
        SPI1_DOUT_PAD,
        SPI1_DIN_PAD,
        SPI1_CS_PAD,
        SPI1_CLK_PAD,
        CAN_TX_PAD,
        CAN_RX_PAD,           
#if 0
        JTAG_TDI_PAD,
        JTAG_TDO_PAD,
        JTAG_TMS_PAD,
        JTAG_TCK_PAD,
        TRST_PAD,
#endif 
        XPWM5_PAD,
        PWM6_PAD,
        AIN0_PAD,
        AIN1_PAD,
        AIN2_PAD,
        AIN3_PAD,
        AIN4_PAD,
        AIN5_PAD,
        AIN6_PAD,
        AIN7_PAD,
    };
    for(int i=0;i< sizeof(IO_PAD)/sizeof(IO_PAD[0]);i++)
    {
        Scu_SetIOReuse(IO_PAD[i],THIRD_FUNCTION);
    }
 
    struct TEST_IO{
        GPIO_TypeDef* GPIOX;
        GPIO_Pinx padpin;
    };
    struct TEST_IO test_io[] =
    {
      {GPIO0,GPIO_Pin0},
 
      {GPIO0,GPIO_Pin13},
      {GPIO0,GPIO_Pin14},
      
      {GPIO0,GPIO_Pin7},
      {GPIO0,GPIO_Pin8},
      
      {GPIO0,GPIO_Pin5},
      {GPIO0,GPIO_Pin6},
    
      {GPIO0,GPIO_Pin11},
      {GPIO0,GPIO_Pin12},

#if 0 
      {GPIO1,GPIO_Pin14},//TDO
      {GPIO1,GPIO_Pin16},//TCK
      {GPIO1,GPIO_Pin15},//TMS
      {GPIO1,GPIO_Pin13},//TDI 
      {GPIO1,GPIO_Pin17},//TRST
#endif
      {GPIO1,GPIO_Pin1},/*XPWM*/
      {GPIO1,GPIO_Pin2},/*PWM6*/
      
      {GPIO1,GPIO_Pin18},
      {GPIO1,GPIO_Pin19},
      {GPIO1,GPIO_Pin20},
      {GPIO1,GPIO_Pin21},
 
      {GPIO0,GPIO_Pin3},
      {GPIO0,GPIO_Pin4}, 
      
      {GPIO1,GPIO_Pin5},/*out*/
      {GPIO1,GPIO_Pin6},/*IN*/
      {GPIO1,GPIO_Pin7},//cs
      {GPIO1,GPIO_Pin8},//clk
    };
    for(int i=0;i< sizeof(test_io)/sizeof(test_io[0]);i++)
    {
        GPIO_Output(test_io[i].GPIOX,test_io[i].padpin,0);
    }
    int index =0;
    for(;;)
    {
      mprintf("%x:RTOS\r\n",times++);   
      GPIO_DATA_Set(test_io[index].GPIOX,test_io[index].padpin,1);
      vTaskDelay(1000); 
      //GPIO_DATA_Set(GPIO1,GPIO_Pin19,!GPIO_ReadDATA(GPIO1,GPIO_Pin19));
      GPIO_DATA_Set(test_io[index].GPIOX,test_io[index].padpin,0);

      index++;
      if(index >sizeof(test_io)/sizeof(test_io[0]))
      {
           index =0; 
      }
    }
#endif