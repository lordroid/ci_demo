#include "includes.h"
#include "ci100x_uart.h"
#if ENABLE_TEST_STAMP_BOARD
//stamp_board  keyword

unsigned char stamp_uart2_receive_data;

unsigned int in_debug_mode = 0;
unsigned int in_debug_mode_too = 0;
unsigned int test_stamp_timeout = 0;
extern void iis1_play_init();
extern void iis1_ES8388_play_cmd();


#define USE_GPIO_I2C_SCL_DETECT
//#define USE_UART_DETECT

void Test_Wait_Timeout_Set(unsigned int us)
{
	NVIC_InitTypeDef NVIC_InitStructure={0};
    Scu_SetDeviceGate(TIMER0,ENABLE);                    
    NVIC_InitStructure.NVIC_IRQChannel = TIMER0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	static TIM_InitTypeDef initStruct = {0};
    initStruct.period = us;  
    initStruct.mode = COUNTER_MODE_AUTO;    
    initStruct.divFreq = TIMER_CLK_DIVIDED_16;
    initStruct.sourceClk = TIMER_CLK_PCLK;
    timer_start(TIMER0,&initStruct);

	
}


void Test_Stamp_BoardIo(void)
{
	
	unsigned char gpio_value[7];
	
	int debug_flag = 0;
	
	stamp_uart2_receive_data = 0;
	/*set uart1 tx query mode,rx interrupt mode*/
	Scu_SetDeviceGate((unsigned int)UART1,ENABLE);
    Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);




    Scu_SetIOReuse(UART1_TX_PAD,FIRST_FUNCTION );
    Scu_SetIOReuse(UART1_RX_PAD,FIRST_FUNCTION );

    
    //Flag_stamp_not_rec = 1;
    
    UART_EN(UART1,DISABLE);
    UART_CRConfig(UART1,UART_TXE,ENABLE);
    UART_CRConfig(UART1,UART_RXE,ENABLE);
    UART_BAUDRATEConfig(UART1,UART_BaudRate115200);
    
    UART_TXFIFOByteWordConfig(UART1,UART_Byte);
    UART_LCRConfig(UART1,UART_WordLength_8b,UART_StopBits_1,UART_Parity_No);
    UART_RXFIFOConfig(UART1,UART_FIFOLevel1_4);
    UART_TXFIFOConfig(UART1,UART_FIFOLevel1_8);
	
    UART_IntClear(UART1,UART_AllInt);
    UART_IntMaskConfig(UART1,UART_AllInt,ENABLE);
    UART_FIFOClear(UART1);
    UART_CRConfig(UART1,UART_NCED,DISABLE);
	//UART_IntMaskConfig(UART1,UART_RXInt,DISABLE);
    UART_EN(UART1,ENABLE);
    SCU->AD_IO_REUSE_CFG = 0;

    UartPollingSenddata(UART1, 't');
    UartPollingSenddata(UART1, 'e');
    UartPollingSenddata(UART1, 's');
    UartPollingSenddata(UART1, 't');
    UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
	
#ifdef USE_GPIO_I2C_SCL_DETECT
    Scu_SetIOReuse(I2C1_SCL_PAD,THIRD_FUNCTION );
	GPIO_Input(GPIO0,(GPIO_Pinx)15);// set SCL to low into test mode 
	if(GPIO_ReadDATA(GPIO0,(GPIO_Pinx)15) == 0)
	{
 	    int i=1000;
 	    while(i--);
		if(GPIO_ReadDATA(GPIO0,(GPIO_Pinx)15) == 0)
		{	
            UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'n');
            UartPollingSenddata(UART1, 't');
            UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			debug_flag = 1;
			in_debug_mode = 1;
			in_debug_mode_too = 1;                       
		}
	}
#else	
        int i= 0;
	Test_Wait_Timeout_Set(1000);
	do
	{
		while((UART1->UARTFlag & (0x1 << 6)) && (test_stamp_timeout == 0));
		receive_data[i] =  UART1->UARTRdDR & 0xff;
        if(test_stamp_timeout)
        {
            UartPollingSenddata(UART1, 'n');
            UartPollingSenddata(UART1, 'o');
            UartPollingSenddata(UART1, 't');
            break;
        }
		
		i++;
		if(i==22)
		{
			i = 0;
			for(t=0;t<11;t++)
			{
				if((receive_data[t] == 's') && (receive_data[t+1] == 't') && (receive_data[t+2] == 'a')
					&& (receive_data[t+3] == 'm') && (receive_data[t+4] == 'p'))
				{
					//mprintf("enter test mode\n");
                    UartPollingSenddata(UART1, 'e');
                    UartPollingSenddata(UART1, 'n');
                    UartPollingSenddata(UART1, 't');
                    UartPollingSenddata(UART1, 'e');
                    UartPollingSenddata(UART1, 'r');
                    UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
                    
					break_flag = 1;
					debug_flag = 1;
					in_debug_mode = 1;
					in_debug_mode_too = 1;
					timer_stop(TIMER0);
					break;
				}
			}
		}
		if(break_flag == 1)
		{
			break;
		}
		
		//mprintf("tt\n");
	}while(test_stamp_timeout==0);	
	timer_stop(TIMER0);
#endif
	//mprintf("next\n");
    UartPollingSenddata(UART1, 'n');
    UartPollingSenddata(UART1, 'e');
    UartPollingSenddata(UART1, 'x');
    UartPollingSenddata(UART1, 't');
    UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
	
	if(debug_flag ==1)
	{
		//mprintf("stamp enter debug\n");
		#if 1
		Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
		Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);

		Scu_SetIOReuse(AIN2_PAD,THIRD_FUNCTION);		//GPIO[52]
		//Scu_SetIOReuse(UART0_TX_PAD,THIRD_FUNCTION);	
		Scu_SetIOReuse(I2C1_SCL_PAD,THIRD_FUNCTION);	//GPIO[15]
		Scu_SetIOReuse(I2C1_SDA_PAD,THIRD_FUNCTION);	//GPIO[16]
		Scu_SetIOReuse(UART2_TX_PAD,THIRD_FUNCTION);	//GPIO[5]
		//Scu_SetIOReuse(UART2_RX_PAD,THIRD_FUNCTION);	//GPIO[6]
		Scu_SetIOReuse(JTAG_TDI_PAD,THIRD_FUNCTION);	//GPIO[45]
		Scu_SetIOReuse(JTAG_TDO_PAD,THIRD_FUNCTION);	//GPIO[46]
		Scu_SetIOReuse(JTAG_TMS_PAD,THIRD_FUNCTION);	//GPIO[47]
		Scu_SetIOReuse(JTAG_TCK_PAD,THIRD_FUNCTION);	//GPIO[48]
		Scu_SetIOReuse(XPWM5_PAD,THIRD_FUNCTION);		//GPIO[33]
		Scu_SetIOReuse(SPI1_DOUT_PAD,THIRD_FUNCTION);	//GPIO[37]
		Scu_SetIOReuse(SPI1_DIN_PAD,THIRD_FUNCTION);	//GPIO[38]
		Scu_SetIOReuse(SPI1_CS_PAD,THIRD_FUNCTION);		//GPIO[39]
		Scu_SetIOReuse(SPI1_CLK_PAD,THIRD_FUNCTION);	//GPIO[40]

		GPIO_Output(GPIO1,(GPIO_Pinx)20,0);	//AIN2 output
		GPIO_Output(GPIO0,(GPIO_Pinx)16,0);	//I2C1_SDA
		//GPIO_Output(GPIO0,(GPIO_Pinx)6,0);	//UART2_RX
        #ifndef USE_GPIO_I2C_SCL_DETECT
		GPIO_Output(GPIO1,(GPIO_Pinx)16,0);	//TCK
        #endif
		GPIO_Output(GPIO1,(GPIO_Pinx)15,0);	//TMS
		GPIO_Output(GPIO1,(GPIO_Pinx)5,0);	//spi1_out
		GPIO_Output(GPIO1,(GPIO_Pinx)7,0);	//SPI1_cs

		

		
        #ifndef USE_GPIO_I2C_SCL_DETECT
		GPIO_Input(GPIO0,(GPIO_Pinx)15);	//I2C1_SCL
        #else
        GPIO_Input(GPIO0,(GPIO_Pinx)16);	//I2C1_SDA
        #endif
		GPIO_Input(GPIO0,(GPIO_Pinx)5);		//UART2_TX
		GPIO_Input(GPIO1,(GPIO_Pinx)14);	//TD0
		GPIO_Input(GPIO1,(GPIO_Pinx)1);		//pwm5
		GPIO_Input(GPIO1,(GPIO_Pinx)13);	//TDI
		GPIO_Input(GPIO1,(GPIO_Pinx)6);		//SPI1_IN
		GPIO_Input(GPIO1,(GPIO_Pinx)8);		//spi1_clk
		
		GPIO_Output(GPIO1,(GPIO_Pinx)20,1);	//AIN2 output
        #ifndef USE_GPIO_I2C_SCL_DETECT
		GPIO_Output(GPIO0,(GPIO_Pinx)16,1);	//I2C1_SDA
        #endif
		//GPIO_Output(GPIO0,(GPIO_Pinx)6,1);	//UART2_RX
		GPIO_Output(GPIO1,(GPIO_Pinx)16,1);	//TCK
		GPIO_Output(GPIO1,(GPIO_Pinx)15,1);	//TMS
		GPIO_Output(GPIO1,(GPIO_Pinx)5,1);	//spi1_out
		GPIO_Output(GPIO1,(GPIO_Pinx)7,1);	//SPI1_cs

		gpio_value[0] = GPIO_ReadDATA(GPIO0,(GPIO_Pinx)15);		//I2C1_SCL
		gpio_value[1] = GPIO_ReadDATA(GPIO0,(GPIO_Pinx)5);		//UART2_TX
		gpio_value[2] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)14);		//TD0
		gpio_value[3] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)1);		//pwm5
		gpio_value[4] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)13);		//TDI
		gpio_value[5] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)6);		//SPI1_IN
		gpio_value[6] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)8);		//SPI1_CLK

        #ifndef USE_GPIO_I2C_SCL_DETECT
		if(gpio_value[0] != 1)
		{
			//mprintf("PIN I2C1_SCL INPUT OR I2C1_SDA OUTPUT ERROR\n");
            UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '1');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		#endif

                
		if(gpio_value[1] != 0)
		{
			//mprintf("PIN  UART2_TX INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '2');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[2] != 1)
		{
			//mprintf("PIN AIN2 OUTPUT OR TDO INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '3');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[3] != 1)
		{
			//mprintf("PIN TCK OUTPUT OR PWM5 INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '4');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[4] != 1)
		{
			///mprintf("PIN TMS OUTPUT OR TDI INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '5');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[5] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '6');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN SPI1OUT OUTPUT OR SPI1IN INPUT ERROR\n");	
			while(1);
		}
		if(gpio_value[6] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '7');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN SPI1CS OUTPUT OR SPI1CLK INPUT ERROR\n");
			while(1);
		}

		GPIO_Output(GPIO1,(GPIO_Pinx)20,0);	//AIN2 output
		GPIO_Output(GPIO0,(GPIO_Pinx)16,0);	//I2C1_SDA
		GPIO_Output(GPIO1,(GPIO_Pinx)16,0);	//TCK
		GPIO_Output(GPIO1,(GPIO_Pinx)15,0);	//TMS
		GPIO_Output(GPIO1,(GPIO_Pinx)5,0);	//spi1_out
		GPIO_Output(GPIO1,(GPIO_Pinx)7,0);	//SPI1_cs

		gpio_value[0] = GPIO_ReadDATA(GPIO0,(GPIO_Pinx)15);		//I2C1_SCL
		gpio_value[2] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)14);		//TD0
		gpio_value[3] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)1);		//pwm5
		gpio_value[4] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)13);		//TDI
		gpio_value[5] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)6);		//SPI1_IN
		gpio_value[6] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)8);		//SPI1_clk

		if(gpio_value[0] != 0)
		{
			//mprintf("--PIN I2C1_SDA OUTPUT OR I2C1_SCL INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '8');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[2] != 0)
		{
			//mprintf("--PIN AIN2 OUTPUT OR TD0 INPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, '9');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[3] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'A');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN TCK OUTPUT OR PWM5 INPUT ERROR\n");
			while(1);
		}
		if(gpio_value[4] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'B');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN TMS OUTPUT OR TDI INPUT ERROR\N");
			while(1);
		}
		if(gpio_value[5] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'c');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN SPI1OUT OUTPUT OR SPI1IN INPUT ERROR\n");
			while(1);
		}
		if(gpio_value[6] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'd');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN SPI1CS OUTPUT OR SPI1CLK INPUT ERROR\n");
			while(1);
		}

		gpio_value[0] = 0;
		gpio_value[1] = 0;
		gpio_value[2] = 0;
		gpio_value[3] = 0;
		gpio_value[4] = 0;
		gpio_value[5] = 0;
		gpio_value[6] = 0;
			
		GPIO_Input(GPIO1,(GPIO_Pinx)20);	//AIN2

        #ifndef USE_GPIO_I2C_SCL_DETECT
		GPIO_Input(GPIO0,(GPIO_Pinx)16);	//I2C1_SDA
		#endif
		GPIO_Input(GPIO1,(GPIO_Pinx)16);	//TCK
		GPIO_Input(GPIO1,(GPIO_Pinx)15);	//TMS
		GPIO_Input(GPIO1,(GPIO_Pinx)5);	//spi1_out
		GPIO_Input(GPIO1,(GPIO_Pinx)7);	//SPI1_cs
		
		GPIO_Output(GPIO0,(GPIO_Pinx)15,1);		//I2C1_SCL
		GPIO_Output(GPIO1,(GPIO_Pinx)14,1);	//TD0
		GPIO_Output(GPIO1,(GPIO_Pinx)1,1);		//pwm5
		GPIO_Output(GPIO1,(GPIO_Pinx)13,1);	//TDI
		GPIO_Output(GPIO1,(GPIO_Pinx)6,1);		//SPI1_IN
		GPIO_Output(GPIO1,(GPIO_Pinx)8,1);		//spi1_clk

		
		
		gpio_value[0] = GPIO_ReadDATA(GPIO0,(GPIO_Pinx)16);	 //I2C1_SDA
		gpio_value[1] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)20);	//AIN2
		gpio_value[2] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)16);	//TCK
		gpio_value[3] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)15);	//TMS
		gpio_value[4] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)5);	//spi1_out
		gpio_value[5] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)7);	//SPI1_cs

		
        #ifndef USE_GPIO_I2C_SCL_DETECT
		if(gpio_value[0] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN I2C1_SDA INPUT OR I2C1_SCL OUTPUT ERROR\n");
			while(1);
		}
		#endif
		if(gpio_value[1] != 1)
		{
			//mprintf("PIN AIN2 INPUT OR TDO OUTPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'f');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[2] != 1)
		{
			//mprintf("PIN TCK INPUT OR PWM5 OUTPUT ERROR\n");
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'g');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			while(1);
		}
		if(gpio_value[3] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'h');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN TMS INPUT OR TDI OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[4] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'I');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN SPI1OUT INPUT OR SPI1IN OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[5] != 1)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'J');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN SPI1CS INPUT OR SPI1CLK OUTPUT ERROR\n");
			while(1);
		}

		GPIO_Output(GPIO0,(GPIO_Pinx)15,0);		//I2C1_SCL
		GPIO_Output(GPIO1,(GPIO_Pinx)14,0);	//TD0
		GPIO_Output(GPIO1,(GPIO_Pinx)1,0);		//pwm5
		GPIO_Output(GPIO1,(GPIO_Pinx)13,0);	//TDI
		GPIO_Output(GPIO1,(GPIO_Pinx)6,0);		//SPI1_IN
		GPIO_Output(GPIO1,(GPIO_Pinx)8,0);		//spi1_clk

		gpio_value[0] = GPIO_ReadDATA(GPIO0,(GPIO_Pinx)16);	 //I2C1_SDA
		gpio_value[1] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)20);	//AIN2
		gpio_value[2] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)16);	//TCK
		gpio_value[3] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)15);	//TMS
		gpio_value[4] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)5);	//spi1_out
		gpio_value[5] = GPIO_ReadDATA(GPIO1,(GPIO_Pinx)7);	//SPI1_cs

		if(gpio_value[0] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'K');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN I2C1_SDA INPUT OR I2C1_SCL OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[1] != 0)
		{      UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'L');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN AIN2 INPUT OR TDO OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[2] != 0)
		{      UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'M');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN TCK INPUT OR PWM5 OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[3] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'N');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN TMS INPUT OR TDI OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[4] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'o');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN SPI1OUT INPUT OR SPI1IN OUTPUT ERROR\n");
			while(1);
		}
		if(gpio_value[5] != 0)
		{
             UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'p');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("--PIN SPI1CS INPUT OR SPI1CLK OUTPUT ERROR\n");
			while(1);
		}
		
		#endif
                
        //uart0 -> UART2
        UartPollingSenddata(UART1, 't');
        UartPollingSenddata(UART1, 'e');
        UartPollingSenddata(UART1, 's');
        UartPollingSenddata(UART1, 't');
		UartPollingSenddata(UART1, ' ');
        UartPollingSenddata(UART1, 'S');
        UartPollingSenddata(UART1, 'U');
        UartPollingSenddata(UART1, 'C');
        UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
		//mprintf("test uart\n");
#if 0       
		UARTPollingConfig(UART0);


		Scu_SetDeviceGate(UART2_BASE,ENABLE);
		Scu_SetIOReuse(UART2_TX_PAD,FIRST_FUNCTION );
		Scu_SetIOReuse(UART2_RX_PAD,FIRST_FUNCTION );

		S_Uart_Open(SUART0,NULL);
		S_Uart_ChangeBaudrate(SUART0,B115200);

		S_Uart_SetTXMode(SUART0,1);//uart2 used as 中断方式 
		S_Uart_SetRXMode(SUART0,0);//

		NVIC_InitTypeDef NVIC_InitStruct = {0};
		NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		
		UartPollingSenddata(UART0,0xab);                                     
                
		Test_Wait_Timeout_Set(3000);
		do
		{
			DelayMs(100);
		}while((stamp_uart2_receive_data!=0xab)&&(test_stamp_timeout==0));
		if(test_stamp_timeout)
		{
            UartPollingSenddata(UART1, 'e');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'r');
            UartPollingSenddata(UART1, 'q');
            UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
			//mprintf("PIN UART0_TX OR UART2_RX ERROR\n");
			while(1);
		}
		timer_stop(TIMER0);
                
		//QSPI_Init();
    	//unsigned int asraddr,asrsize;
		//GetUserPara(&asraddr,&asrsize);

		//iis1_play_init(); 
		//iis1_ES8388_play_cmd();
		//play_voice_index(1);
     #endif 
	}
//	mprintf("stamp next\n");
        
        

    UartPollingSenddata(UART1, 'e');
    UartPollingSenddata(UART1, 'n');
    UartPollingSenddata(UART1, 'd');
    UartPollingSenddata(UART1, 't');
    UartPollingSenddata(UART1, 'e');
    UartPollingSenddata(UART1, 's'); 
	UartPollingSenddata(UART1, 't'); 
	UartPollingSenddata(UART1, ' ');UartPollingSenddata(UART1, ' ');
	in_debug_mode = 0;

	
	
}
#endif