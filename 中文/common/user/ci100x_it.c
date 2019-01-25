/**
  ******************************************************************************
  * @文件    ci100x_it.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件是chipintelli公司的CI100X芯片程序的各个外设中断服务函数.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
 
#include "ci100x_it.h"
#include "includes.h"
#include "user_config.h"

/******************************************************************************/
unsigned int test_wakeup = 0;
/*CAN中断服务程序用到的变量*/
extern volatile unsigned int g_can_transmix_complete;
extern unsigned char rx_data[13];
extern volatile unsigned int can_receive_number;

/*DMA中断服务程序用到的变量*/

extern SemaphoreHandle_t xLowPowerSemaphore;
/*I2C中断服务程序用到的变量*/
//extern unsigned int receive_data_count[2];
//extern unsigned int transfer_data_count[2];

extern unsigned int tx_count[2];
//extern unsigned int rx_count[2];
extern unsigned int iic0_expact_send_size;
extern unsigned int iic1_expact_send_size;
extern unsigned int iic1_client_flags;
extern unsigned int iic0_client_flags;
extern struct i2c_data i2c0_master;
extern struct i2c_data i2c1_master;
extern struct i2c_algorithm i2c0_algo; 
extern struct i2c_algorithm i2c1_algo;

static unsigned int master_send[2] = {0};

volatile unsigned int g_dma_translate_ok2 = 0;

extern void  suart0_rx_handle(unsigned char txrxdata);
extern Struct_UartInfo S_Uart_Table[];

#if USE_S_MIC_DENOISE
extern "C" void IIS1_denoise_receive_handler(void);
#endif 
         
/**
  * @功能:timer看门狗中断处理函数
  * @注意:在使用前需调用SCU相关接口初始化,复位系统使能       
  * @参数:无
  * @返回值:无
  */
void WWDG_IRQHandler(void)
{
	TWDG->WDT_LOCK  = 0x51acce55;
    TWDG->WDT_SRV   = 0xabadc0de;
    TWDG->WDT_LOCK  = 0; 
}

/**
  * @功能:IWDG看门狗中断服务函数
  * @注意:在使用前需调用SCU相关接口初始化,复位系统使能        
  * @参数:无
  * @返回值:无
  */
#if USE_ASR8388
/*es8388 iis1*/
#define ASR_PCM_IIS_DMA_ADDR (*(volatile unsigned int*)(0X40015000 + 0x60))/*iis 1 dma rx address*/
#define ASR_PCM_IIS_DMA_STATUS ((*(volatile unsigned int*)(0X40015000 + 0x0))&(1<<15)) /*iis0 rx en bit 15*/
#else
/*cx20921 iis0*/
#define ASR_PCM_IIS_DMA_ADDR (*(volatile unsigned int*)(0X40015000 + 0x5c))/*iis 0 dma rx address*/
#define ASR_PCM_IIS_DMA_STATUS ((*(volatile unsigned int*)(0X40015000 + 0x0))&(1<<13)) /*iis0 rx en bit 13*/
#endif

void IWDG_IRQHandler(void)
{
    uint32_t dmaadr = 0X0;
    static uint32_t  last_dma_addr = 0X0;
    int i = 0;
    int die_flag = 0;
    int iisdmastatus = 0;
    dmaadr = ASR_PCM_IIS_DMA_ADDR;

    if(iwdg_watch  )
    {
        //喂狗
        WDOGLOCK 			= 0x1ACCE551;		      
        IWDG->WdogIntClr 	= 0x1;
        WDOGLOCK 			= 0x0;
        #if USE_IWDG
        iwdg_watch = 0;
        mprintf("feed IWDG ...\n");
        #endif
        iisdmastatus = ASR_PCM_IIS_DMA_STATUS;
        if((dmaadr == last_dma_addr) && (iisdmastatus))
        {
            for(i=0;i<10;i++)
            {
                DelayMs(30);
                dmaadr = ASR_PCM_IIS_DMA_ADDR;
                if(dmaadr == last_dma_addr)
                {
                    continue;
                }
                else
                {
                    die_flag = 1;
                    break;
                }
            }
            if(die_flag == 0)
            {
                mprintf("iwtd  soft RST\n\r");
                Scu_SoftwareRst_System();
            } 
        }
        else
        {
            last_dma_addr = dmaadr;
        }
    }
    else
    {
        mprintf("iwtd  soft RST\n\r");
        Scu_SoftwareRst_System();
    }
}

/**
  * @功能:xpwm中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void xPWM_IRQHandler(void)
{
	xPWM_INTClear(xPWM);
}

/**
  * @功能:低电压检测中断服务程序
  * @注意:暂时无此功能,此函数不可使用 
  * @参数:无
  * @返回值:无
  */
void FPU_IRQHandler(void)
{
}

/**
  * @功能:xpwm刹车中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void xPWM_TZ_IRQHandler(void)
{
	xPWM_TZCLR(xPWM,xPWM_TZCLR_OST);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_INT);
}

/**
  * @功能:模数转换器(ADC)中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
extern void adkey_handler(void );
void ADC_IRQHandler(void)
{
    /*ADC 共8个通道，添加对应的通道即可*/
    if(ADC_IntFlag(ADC_Channel_0))
    {
        adkey_handler();//
        ADC_IntClear(ADC_Channel_0);
    }
    if(ADC_IntFlag(ADC_Channel_2))    
    {
        ADC_IntClear(ADC_Channel_2);
    }
}

/**
  * @功能:CAN中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void CAN_IRQHandler(void)
{
	BF_CAN_FRAME *frame=NULL;
    volatile unsigned char tmp;
    int length;
    int i;
    tmp = CAN->IR.ui;  
    if(CAN->SR.bf.TBS)
    {        
        if( CAN->SR.bf.TCS )
        {
          	g_can_transmix_complete = 1;
        }
    }
    if(CAN->SR.bf.RBS)
    {  
        frame = (BF_CAN_FRAME *)CAN_RX_BUFFER;
        length = (frame->format == CAN_Frame_Format_Standard)?11:13;
        for(i=0;i<length;i++)
        {
            rx_data[i] = CAN_RX_BUFFER[i];
        }        
        can_receive_number  = length;
        CAN->CMR.bf.AT      = 1;    //终止传输
        CAN->CMR.bf.RRB     = 1;    //释放接收Buffer
    }
}

/**
  * @功能:EXIT0中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void EXTI0_IRQHandler(void)
{
}

/**
  * @功能:EXIT1中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void EXTI1_IRQHandler(void)
{
    GPIO_DATA_Inv(GPIO0, GPIO_Pin7);
	GPIO_INTClear(GPIO0,GPIO_Pin6);
}

/**
  * @功能:EXIT2中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void EXTI2_IRQHandler(void)
{
}

/**
  * @功能:EXIT3中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void EXTI3_IRQHandler(void)
{
}

/**
  * @功能:DMA中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void DMA_IRQHandler(void)
{
  	DMAC->DMACIntErrClr    = 0xff;
    //g_dma_translate_ok = 1;
	if(DMAC_IntTCStatus(DMACChannel0))
	{
		g_dma_translate_ok = 1;
	}else if(DMAC_IntTCStatus(DMACChannel1))
	{
		g_dma_translate_ok1 = 1;
	}else if(DMAC_IntTCStatus(DMACChannel2))
	{
	    g_dma_translate_ok2 = 1;
	}
    DMAC->DMACIntTCClear = 0xff;
}

/**
  * @功能:GPIO0组中断服务程序
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void GPIO0_IRQHandler(void)
{
	/*GPIO中断GPIO0 32个Pin脚(GPIO_Pin0~GPIO_Pin31),
	用到对应的Pin服务程序,请自行添加*/
	if( GPIO0->GPIOMIS & (1 << GPIO_Pin15))
	{
	    GPIO_DATA_Inv(GPIO0, GPIO_Pin7);	   
	}
	GPIO0->GPIOIC = 0xFFFFFFFF;
}

/**
  * @功能:UART0中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void UART0_IRQHandler(void)                    
{
	if(UART0->UARTMIS & (1UL << UART_TXInt))
	{
		 
	}
	if(UART0->UARTMIS & (1UL << UART_RXInt))
	{ 
		//here FIFO DATA must be read out  
	}
	UART0->UARTICR = 0xFFF;
}

/**
  * @功能:UART1中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
extern char  UartPollingReceiveData(UART_TypeDef* UARTx);
void UART1_IRQHandler(void)
{
	if(UART1->UARTMIS & (1UL << UART_TXInt))
	{
		 
	}
	if(UART1->UARTMIS & (1UL << UART_RXInt))
	{
#if USER_UART_USE_UART1        
        suart0_rx_handle(UartPollingReceiveData(UART1));
#endif        
		//here FIFO DATA must be read out  
	}
	UART1->UARTICR = 0xFFF;
}

/**
  * @功能:SUART0中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void SUART0_IRQHandler(void)
{
#if USE_USER_UART
#if !USER_UART_USE_UART1    
    
	Struct_UartInfo *info;
    char int_state = 0;
    unsigned char txrxdata;
     
NVIC_DisableIRQ(UART2_IRQn);
    info = &(S_Uart_Table[0]);
uint32_t lsr_reg;
do
{
    lsr_reg = info->addr[S_UART_LSR] ;
    int_state = info->addr[S_UART_IIR] & 0xf;

    if(lsr_reg&0x2)
    {
        mprintf("lsr is %x\n",info->addr[S_UART_LSR]);
    }
    
    if(((lsr_reg&0x20)||(int_state == 0x2)) && !(info->btxquery))
    {
      if(!S_CircleBuffer_IsEmpty(&(info->txcirclebuffer)))
      {
        S_CircleBuffer_Read(&(info->txcirclebuffer), &txrxdata);
        info->addr[S_UART_THR] = txrxdata;
      }
    }
    
    if((int_state == 0x4)&& !(info->brxquery))
    {
      txrxdata = info->addr[S_UART_RBR];
      //S_CircleBuffer_Write(&(info->rxcirclebuffer), txrxdata);
      suart0_rx_handle(txrxdata);
    }

    //int_state = info->addr[S_UART_IIR] & 0xf;
  //  lsr_reg = info->addr[S_UART_LSR] ;
}while(1 != int_state);

NVIC_EnableIRQ(UART2_IRQn);    
#endif
#else

	S_UART0_int();	
#endif	
}

/**
  * @功能:SUART1中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void SUART1_IRQHandler(void)
{
	S_UART1_int();
}

/**
  * @功能:I2C0中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void I2C0_IRQHandler(void)
{
	struct i2c_data *i2c = &i2c0_master;
    volatile unsigned int temp;
    temp = IIC0->STATUS;
    IIC0->INTCLR = 0x7f;   

    if ((i2c->algo_data.read_byte(i2c, IIC_STATUS) & SR_IRQ_FLAG) == 0)
    {
        i2c->algo_data.i2c_errors = i2c->algo_data.read_byte(i2c, IIC_STATUS);
    }
    if(i2c->algo_data.i2c_over == I2C_TRANS_OVER)
    {
        i2c->algo_data.i2c_errors &= ~SR_RXACK;
        i2c->algo_data.i2c_over = I2C_TRANS_ON;
    }
    /*master */
    	if(temp & (0x1 << 11))
		{
			master_send[0] = 1;
			if(temp & (0x1 << 2))
			{
				//mprintf("send 1B\n");
				tx_count[0]++;
			}
		}
		else
		{
			master_send[0] = 0;
		}
		if(master_send[0] == 1)
		{
			if(temp & (0x1 << 14))
			{
				tx_count[0] = 0;
				IIC0->CMD = (0x1 << 3) | 0x1;
			//mprintf("master stop\n");
			}
			else
			{
				
				//mprintf("receive ack\n");
			}
		}
		else
		{
			if(temp & (0x1 << 14))
			{
				tx_count[0] = 0;
			
				//mprintf("master receive stop\n");
			}
			else
			{
			
				//mprintf("receive receive ack\n");
			}
		}
		wake_up_interruptible(&i2c->wait);
    IIC0->INTCLR = CR_IRQ_CLEAR;
}

/**
  * @功能:I2C1中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void I2C1_IRQHandler(void)
{
	struct i2c_data *i2c = &i2c1_master;
    volatile unsigned int temp;
    temp = IIC1->STATUS;
    IIC1->INTCLR = 0x7f;   
    if ((i2c->algo_data.read_byte(i2c, IIC_STATUS) & SR_IRQ_FLAG) == 0)
    {
        i2c->algo_data.i2c_errors = i2c->algo_data.read_byte(i2c, IIC_STATUS);
    }
    if(i2c->algo_data.i2c_over == I2C_TRANS_OVER)
    {
        i2c->algo_data.i2c_errors &= ~SR_RXACK;
        i2c->algo_data.i2c_over = I2C_TRANS_ON;
    }
    /*master */
    if(temp &(0x1 << 2))
	{
		tx_count[1]++;
	}
    if( (tx_count[1] == (iic1_expact_send_size + 1)) && 
        ((iic1_client_flags & I2C_M_RD)!=I2C_M_RD))
    {
        IIC1->CMD |= (0x1 << 3) | 0x1;
        tx_count[1] = 0;
    }
    wake_up_interruptible(&i2c->wait);
    IIC1->INTCLR = CR_IRQ_CLEAR;
}

extern uint32_t vad_wakeup_flag;

/**
  * @功能:SCU中断处理函数
  * @注意:无      
  * @参数:无
  * @返回值:无
  */

void SCU_IRQHandler(void) 
{
  
    volatile unsigned int tmp;
   
     vad_wakeup_flag = 1;
    *(volatile unsigned int *)(0x40010000 + 0x40) = 0x51AC0FFE;
    while((*(volatile unsigned int *)(0x40010000 + 0x40))!=1);
    *(volatile unsigned int *)(0x40010000 + 0x44) = 0x51AC0FFE; 
    while((*(volatile unsigned int *)(0x40010000 + 0x44))!=1);
    *(volatile unsigned int *)(0x40014000 + 0x08) |= (0x1 << 0);
    *(volatile unsigned int *)(0x40010000 + 0x08) &= ~(0x3f << 0);   
    *(volatile unsigned int *)(0x40010000 + 0x08) |= (0x2 << 0);
    *(volatile unsigned int *)(0x40010000 + 0x3c) |= (0x1 << 0);
   
    for(volatile unsigned int i=0;i<0x6000;i++){__NOP();    }
    
    /*打开DNN ，ASR， FE，SDRAM时钟*/
    *(volatile unsigned int *)(0x40010000 + 0x44) = 0x51AC0FFE; 
    while((*(volatile unsigned int *)(0x40010000 + 0x44))!=1);  
    *(volatile unsigned int *)(0x40010000 + 0x24) |= clkgate_state[0];
    *(volatile unsigned int *)(0x40010000 + 0x28) |= clkgate_state[1];    
    *(volatile unsigned int *)(0x40010000 + 0x44) = 0x0;     
    for(volatile unsigned int i=0;i<0x300;i++){__NOP();    }
          
    /*SDRAM退出自刷新*/
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc) &= ~(0x1 << 1);
   
    while(((*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc))&(0x1 << 11)) != 0);
    for(volatile unsigned int i=0;i<300;i++){__NOP();    }
    
    *(volatile unsigned int *)(0xe000e010) |= (0x1 << 0);    
    tmp = *(volatile unsigned int *)(0x40010000 + 0x5c);
    *(volatile unsigned int *)(0x40010000 + 0x5c) = tmp;
      
    //开启 FE和DNN gate CLK
    *(volatile unsigned int*)(0x40010000+0x44) = 0x51AC0FFE;
    *(volatile unsigned int*)(0x40010000+0x40) = 0x51AC0FFE;
    while((*(volatile unsigned int *)(0x40010000 + 0x44)) != 1);
    while((*(volatile unsigned int *)(0x40010000 + 0x40)) != 1);
    *(volatile unsigned int*)(0x40010000 + 0x24) |= (0x3<<28);  

    Asr_open();

    /*disable vad int and enable asr int*/
    if(tmp & (0x1 << 23) )
    {
        NVIC_DisableIRQ(VAD_IRQn);
        NVIC_EnableIRQ(ASR_IRQn);
    }
    else
    {
        NVIC_EnableIRQ(VAD_IRQn);
        NVIC_DisableIRQ(ASR_IRQn);
    }
	NVIC_DisableIRQ(GPIO0_IRQn);

#if USER_UART_USE_UART1
    NVIC_EnableIRQ(UART1_IRQn);
#else
    NVIC_EnableIRQ(UART2_IRQn);
#endif

}

/**
  * @功能:IIS_DMA中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void i2s0rx_recordSD_handler(void)
{
    static unsigned int addr = 0;
    addr += 16*1024;
    if(addr >= 32 * 1024 )
    {
        addr = 0;
    }
    IISxDMA_RADDR(IIS0DMA,0x70500000 + addr);
    xSemaphoreGiveFromISR(xVadpcmSemaphore,NULL);
}

void i2s1rx_IIS_handler(void)
{
    static unsigned int addr = 0;
    addr += 16*1024;
    if(addr >= 1024 * 1024 )
    {
        addr = 0;
    }
    IISxDMA_RADDR(IIS1DMA,0x70500000 + addr);
    //xSemaphoreGiveFromISR(xVadpcmSemaphore,NULL);
}

extern void IIS0_SlaveRxSDRAMpcm(void);
unsigned int vadlog_pcmsize = 0;
void IIS0_SlaveRxSDRAMpcm_handler(void)
{
	static BaseType_t xHigherPriorityTaskWoken;

    static unsigned int addr = SDRAM_REC_BUFFER_ADDR;
    
    static int writepcmflag=0;// 标志是第一次start_readpcm = 1,这时进行文件buffer的指针初始化及同步
    	
    if(addr == SDRAM_REC_BUFFER_ADDR+(SDRAM_REC_BUFFER_SIZE-1024))//设置下一次iis dma抓取数据的保存位置
    {
        IISxDMA_RADDR(IIS0DMA,SDRAM_REC_BUFFER_ADDR); //  
    }
    else
    {
        IISxDMA_RADDR(IIS0DMA,addr+1024); //  
    }  
    
    if(run_mode != 2) 
    {
        memcpy((void*)0x20020000,(void*)addr,1*1024);//
    }
	if(start_readpcm == 1)
	{ 
		pcm_wptr =(addr-SDRAM_REC_BUFFER_ADDR);
		if(!writepcmflag)
		{//这里发出一次write  pcm file，然后task那边就开始循环写数据到tf卡，当buffer的space数据大于1K即开始写数据
			writepcmflag = 1;
			if(pcm_wptr == 0)
			{
				pcm_rptr = SDRAM_REC_BUFFER_SIZE-1024;
			}else
                pcm_rptr = pcm_wptr-1024;
			
            vadlog_pcmsize = 1024;
			//if(pdPASS!=xQueueSendToBackFromISR(RWFILE_xQueue,&data,&xHigherPriorityTaskWoken1))
		    xSemaphoreGiveFromISR(xRecSemaphore,&xHigherPriorityTaskWoken);
		}else
        {
            vadlog_pcmsize +=1024;
        }
        
        {
            int space = LOOP_BUF_SPACE(pcm_rptr,pcm_wptr,SDRAM_REC_BUFFER_SIZE);
            if(space<=4*1024)
            {
                mprintf("err?,PCM SPACE = %dk\r\n",space/1024);
            }           
            else if(space<=95*1024)
            {
              mprintf("#%dk ",space/1024);
            }
           /* if(space < 120 )
            {//当数据填充 （PCMREC_SIZE - 120 ）*1024byte时，发写文件指令
                xSemaphoreGiveFromISR(xRecSemaphore_4K,&xHigherPriorityTaskWoken);
            }*/
        }
       // sdfile_cursize += 1024;
	}else
    {
      writepcmflag = 0;
    }
	addr +=1*1024;
	if(addr >=SDRAM_REC_BUFFER_ADDR + SDRAM_REC_BUFFER_SIZE)
	{
		addr = SDRAM_REC_BUFFER_ADDR;
	}  
	
}
#if RECTEST
void IIS1_SlaveRxSDRAMtest_handler(void)
{
    static unsigned addr = 0x70500000;

    IISxDMA_RADDR(IIS1DMA,addr + 1024); //  
	addr += 1*1024;
	
	if(addr >= 0x70500000 + 1024*1024)
	{
		memset((void *)0x70500000,0xff,1024*1024);
		addr = 0x70500000;
	}
}
#endif

void IISDMA_IRQHandler(void)
{
	volatile unsigned int tmp;
	tmp=IISDMA->IISDMASTATE;
	if(tmp & (1<<(7+IIS0DMA+IISxDMA_RX_EN*3)))
    {
	  	//iis0_rxrecord_handler();
        //i2s0rx_recordSD_handler();
		if(0 == run_mode)        
		{
	        IIS0_SlaveRxSDRAMpcm_handler();
		}else if(2 == run_mode)        
		{
	        IIS0_SlaveRxSDRAMpcm_handler();
		}
	}
	if(tmp & (1<<(7+IIS1DMA+IISxDMA_RX_EN*3)))
  	{
#if RECTEST
         IIS1_SlaveRxSDRAMtest_handler();
#endif
#if USE_S_MIC_DENOISE
         IIS1_denoise_receive_handler();
#endif
    }
	if(tmp & (1<<(7+IIS2DMA+IISxDMA_RX_EN*3)))
    {
    
    }
	if(tmp & (1<<(7+IIS0DMA+IISxDMA_TX_EN*3)))
    {
    
    }
	if(tmp & (1<<(7+IIS1DMA+IISxDMA_TX_EN*3)))
    {
    	//iis1_cs42l52_play_cmd_handler();
#if USE_ORIGIN_PLAY_FLOW
        iis1_cs42l52_adpcmplay_cmd_handler();
#else
        iis1_play_pcm_handler();
#endif
    }
	if(tmp & (1<<(7+IIS2DMA+IISxDMA_TX_EN*3)))
    {
        
    }
    IISDMA->IISDMACLR = 0x1;
}

/**
  * @功能:SPI1中断服务函数
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SPI1_IRQHandler(void)
{
	volatile unsigned int tmp=0;
  	SPI1->SPIICR = 0x3;
	tmp=SD->SDSTATE;
	if(tmp & (1UL << 1))
	{
		SD->SDSTATE |= (1UL<<3); 
		sd_transend	= 1;
	}
	
}

/**
  * @功能:SPI2中断服务函数
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void SPI2_IRQHandler(void) 
{
	SPI2->SPIICR = 0x3;
}

/**
  * @功能:EXIT4中断服务程序
  * @注意:无  
  * @参数:无
  * @返回值:无
  */
void EXTI4_IRQHandler(void)
{
}

/**
  * @功能:GPIO1组中断服务程序
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void GPIO1_IRQHandler(void)  
{
	/*GPIO中断GPIO1 25个Pin脚(GPIO_Pin0~GPIO_Pin24),
	用到对应的Pin服务程序,请自行添加*/
	if( GPIO1->GPIOMIS &(1<<GPIO_Pin0))
	{
	}
	GPIO1->GPIOIC=0xFFFFFFFF;
}

/**
  * @功能:红外(IR)中断服务函数
  * @注意:无       
  * @参数:无
  * @返回值:无
  */
void IR_IRQHandler(void)
{
	volatile unsigned int tmp;
    tmp = SMT_IR->REM_INT_STATE;
    SMT_IR->REM_INT_CLR = tmp;
}

/**
  * @功能:定时器0中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void TIM0_IRQHandler(void)
{
	TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)0x40039000;
    timer->cfg1.bf.ct = 1;   //清除定时器中断 
}
/**
  * @功能:定时器1中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void TIM1_IRQHandler(void)
{	
	static BaseType_t xHigherPriorityTaskWoken;
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)TIMER1;
    timer->cfg1.bf.ct = 1;   //清除定时器中断
    timer_stop(TIMER1);
    test_wakeup = 1;
    xSemaphoreGiveFromISR(xLowPowerSemaphore,&xHigherPriorityTaskWoken);
}

extern char CheckPlayFlag(void);
extern void iis1_ES8388_play_cmd();
void Check_iis1dmastate(void)
{
    static uint32_t  last_dma_addr = 0X0;
    uint32_t dmaadr = 0X0;
    dmaadr = ASR_PCM_IIS_DMA_ADDR;
    int iisdmastatus = 0;
    int die_flag = 0;
    iisdmastatus = ASR_PCM_IIS_DMA_STATUS;
    if((dmaadr == last_dma_addr) && (iisdmastatus))
    {
        for(int i=0;i<10;i++)
        {
            if(CheckPlayFlag() == 1)
            {
                last_dma_addr = dmaadr;
                die_flag = 1;
                break;
            }else
            {
                
                dmaadr = ASR_PCM_IIS_DMA_ADDR;
                if(dmaadr == last_dma_addr)
                {
                    DelayMs(30);
                    continue;
                }
                else
                {
                    last_dma_addr = dmaadr;
                    die_flag = 1;
                    break;
                }
            }
        }
        if(die_flag == 0)
        {
            mprintf("iis-dma check failed \n\r");
            iis1_ES8388_play_cmd();
        } 
    }
    else
    {
        last_dma_addr = dmaadr;
    }
}

/**
  * @功能:定时器2中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
extern   struct mallinfo alloc_meminfo;
#include <iar_dlmalloc.h>
unsigned int pcm_addr = 0x70200000;
extern void CheckStatus(void);
void TIM2_IRQHandler(void)
{	
    TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)0x4003B000;
    timer->cfg1.bf.ct = 1;   //清除定时器中断
    CheckStatus();
    Check_iis1dmastate();
#if VAD_ONLINE
    if(vad_cnt_timer!=0xffffffff) 
    {
        vad_cnt_timer--;
        if(vad_cnt_timer == (C_VAD_END_MAX-C_VAD_SHORT_LOST))
        {
             if(vad_end_timer == 0xffffffff)
             {//vadend 尚未开始计数，说明开始的时间超过C_VAD_SHORT_LOST ms才开始
                LED_VAD_ON;
             }else
             {//vad 开始后的短时间内查到vad 已经有结束信号，这时表明该次vad数据非常短，则忽略掉（vad 不开始，直接结束计数）
                vad_end_timer = 0xffffffff;
                vad_cnt_timer = 0xffffffff;
             }  
        }
        if(!vad_cnt_timer)
        {//vad start 超过 C_VAD_END_MAX，则强制online vad stop
            vad_cnt_timer = 0xffffffff;
            vad_end_timer = 0xffffffff;
            LED_VAD_OFF;
        }else
        {
            if(vad_end_timer!=0xffffffff)
            {//vad end信号，当超过 C_VAD_END_DELAY ，认为产生一次完整的的online vad
                vad_end_timer--;
                
                if(!vad_end_timer)
                {
                    vad_cnt_timer = 0xffffffff;
                    vad_end_timer = 0xffffffff;
                    LED_VAD_OFF;
                }
            }
        }
    }
#endif
}



#define NODNN_TIMEOUT 100 //10 ms
#define DECODE_TIMEOUT 500//10 ms
#define VADEND_TIMEOUT 500//10 ms
#define VADSTART_TIMEOUT 100// 10 ms

int asrtime_out(unsigned int time)
{
    asr_datatype asr_queue_data;
    asr_queue_data.data_address =0x0;
    asr_queue_data.data_length =0;
    asr_queue_data.data_type =0xaa;
    switch(timeout_type)
    {
        case clear_timeout:
          clear =0;
          timeout_type = idl_timeout;
        break;
        case nodnn_timeout:
          nodnn_start = time;
          clear =1;
          timeout_type = decode_timeout;
        break;
        case decode_timeout:
          decode_start = time;
          clear =1;
          timeout_type = vadend_timeout;
        break;
        case vadend_timeout:
          vadend_start = time;
          clear =1;
          timeout_type = idl_timeout;
        break;
        case vadstart_timeout:
          vadstart_start = time;
          clear =1;
          timeout_type = idl_timeout;
        break;
    }
    
    if(!clear)
    {
        vadstart_start =0;
        vadend_start =0;
        decode_start =0;
        nodnn_start =0;
        return 0;
    }
    if(vadstart_start)
    {
        if(time-vadstart_start > VADSTART_TIMEOUT)
        {
            mprintf("#########################VADSTART_TIMEOUT\n");
            if(pdPASS!=xQueueSendToBackFromISR(ASR_xQueue,&asr_queue_data,\
            NULL))
            {
                mprintf("timeout send failed\n");
            }
            vadstart_start =0;
        }
    }
    if(vadend_start)
    {
        if(time-vadend_start > VADEND_TIMEOUT)
        {
            mprintf("#########################VADEND_TIMEOUT\n");
            if(pdPASS!=xQueueSendToBackFromISR(ASR_xQueue,&asr_queue_data,\
            NULL))
            {
                mprintf("timeout send failed\n");
            }
            vadend_start =0;
        }   
        nodnn_start =0;
    }
    if(decode_start)
    {
        if(time-decode_start > DECODE_TIMEOUT)
        {
            mprintf("#########################DECODE_TIMEOUT\n");
            if(pdPASS!=xQueueSendToBackFromISR(ASR_xQueue,&asr_queue_data,\
            NULL))
            {
                mprintf("timeout send failed\n");
            }
            decode_start =0;
        }   
    }
    if(nodnn_start)
    {
        if(time-nodnn_start > NODNN_TIMEOUT)
        {
            mprintf("###################NODNN_TIMEOUT\n");
            if(pdPASS!=xQueueSendToBackFromISR(ASR_xQueue,&asr_queue_data,\
            NULL))
            {
                mprintf("timeout send failed\n");
            }
            nodnn_start =0;
        }   
    }
    return 1;
}

/**
  * @功能:定时器3中断服务函数
  * @注意:无    
  * @参数:无
  * @返回值:无
  */
void TIM3_IRQHandler(void)
{	
    extern volatile unsigned int ulHighFrequencyTimerTicks;
	TIMER_PWM_TypeDef *timer=(TIMER_PWM_TypeDef *)0x4003C000;
    timer->cfg1.bf.ct = 1;   //清除定时器中断
    asrtimer_counter++;
    //asrtime_out(asrtimer_counter);
    extern void record_eng(void);
    record_eng();
    //mprintf("#\n");
#if DECODE_DEBUG
    extern int debug_work;
    if(debug_work)
    {
        static unsigned int pcm_start = 0x70500000;
        if(pcm_start>= 0x70500000+ 1024*1024)
        {
            //pcm_start = 0x70500000;
            mprintf("");
        }else
        {
            memcpy((void*)0x20020000,(void*)pcm_start,320);
            pcm_start+=320;
        }
    }
#endif 
#if CPU_RATE_PRINT
    ulHighFrequencyTimerTicks++;
#endif  
}

/*******************************ARM CM4 Core异常ISR***************************************/

void NMI_Handler(void)
{
    mprintf("%s\n",__func__);
  	while(1)
	{
	}
}

void HardFault_Handler(void)
{
	mprintf("%s\n",__func__);
	/* 当硬件失效异常发生时进入死循环 */
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	mprintf("%s\n",__func__);
    /* 当内存管理异常发生时进入死循环 */
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	mprintf("%s\n",__func__);
    /* 当总线异常时进入死循环 */
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	mprintf("%s\n",__func__);
    /* 当用法异常时进入死循环 */
	while (1)
	{
	}
}

void DebugMon_Handler(void)
{
  	mprintf("%s\n",__func__);
    while(1)
	{
	}
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
