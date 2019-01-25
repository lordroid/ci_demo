#include "ci100x_adc.h"
#include "ci100x_xpwm.h"
#include "ci100x_scu.h"
#include "misc.h"
#include "smt_adc.h"
#include "smt_uart.h"
#include "ci100x_gpio.h"
#include "includes.h"
#include "user_main.h"

void ADC_xPWMFOC_INTHandler(void);

unsigned short adc0ina0[300] = {0},adc1ina0[300] = {0},adc0ina1[300] = {0};
/*
void printadcdata(unsigned short a[],unsigned int size,char* s)
{
    int i=0;
    //debug("%s\n",s);
    for(i=0;i<size;i++){
        //debug("%d\t",a[i]);
    }
}

void ADC_xPWMFOC_INTHandler(void)
{
 
}
*/
void testADC_xPWMFOC(void)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    xPWMInit_TypeDef xPWMInit_Struct = {0};
    unsigned int tmp = 0xfffff;
   
    NVIC_InitStruct.NVIC_IRQChannel = XPWM_TZ_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
    
    NVIC_InitStruct.NVIC_IRQChannel = xPWM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);	
    
#if 1
    Scu_SetIOReuse(XPWM0_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM1_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM2_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM3_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM4_PAD,FIRST_FUNCTION);
    Scu_SetIOReuse(XPWM5_PAD,FIRST_FUNCTION);
#else
    Scu_SetIOReuse(JTAG_TDI_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(JTAG_TDO_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(JTAG_TMS_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(JTAG_TCK_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(TRST_PAD,SECOND_FUNCTION);
    Scu_SetIOReuse(AIN0_PAD,SECOND_FUNCTION);
#endif 
    Scu_SetDeviceGate(xPWM_BASE,ENABLE);
    
    while(tmp--);
    xPWMInit_Struct.xPMW_TBCTL.xPWM_CTRMODE = xPWM_CTRMODE_INC_DEC;/*增减计数，计数器先递增，再递减*/
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PRDLD = xPWM_PRDLD_FREE;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_SYNCOSEL = xPWM_SYNCOSEL_ZERO;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSDIR = xPWM_PHSDIR_INC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_FREESOFT = xPWM_FREESOFT_CONTINUE;

    xPWMInit_Struct.xPWMX_TBPRD =  12000;/*计数周期*/

    xPWMInit_Struct.xPWM_CMPCTL.xPWM_LOADAMODE =xPWM_LOADABMODE_ZERO;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_LOADBMODE =xPWM_LOADABMODE_ZERO;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_SHDWAMODE =xPWM_SHDWABMODE_DIRECT;
    xPWMInit_Struct.xPWM_CMPCTL.xPWM_SHDWBMODE =xPWM_SHDWABMODE_DIRECT;
 
    xPWMInit_Struct.xPWM_CMPA = 3000;
    xPWMInit_Struct.xPWM_CMPB = 4000;
    xPWMInit_Struct.xPWM_CPR1 = 4000;
    xPWMInit_Struct.xPWM_CPR2 = 4000;

    xPWMInit_Struct.xPWM_AQCTLA.xPWM_ZRO = xPWM_ZRO_HIGH;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_PRD = xPWM_PRD_LOW;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_CAU = xPWM_CAU_LOW;
    xPWMInit_Struct.xPWM_AQCTLA.xPWM_CAD = xPWM_CAD_HIGH;
 
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_ZRO = xPWM_ZRO_NONE;
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_CAU = xPWM_CAU_NONE;
    xPWMInit_Struct.xPWM_AQCTLB.xPWM_CAD = xPWM_CAD_NONE;
  
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_ACTSFA = xPWM_ACTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_ACTSFB = xPWM_ACTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_OTSFA = xPWM_OTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_OTSFB =xPWM_OTSFAB_NONE;
    xPWMInit_Struct.xPWM_AQSFRC.xPWM_RLDCSF = xPWM_RLDCSF_ZERO;
    
    xPWMInit_Struct.xPWM_AQCSFRC.xPWM_CSFA = xPWM_CSFAB_NONE;
    xPWMInit_Struct.xPWM_AQCSFRC.xPWM_CSFB = xPWM_CSFAB_NONE;
    
    xPWMInit_Struct.xPWM_DBCTL.xPWM_OUT_MODE = xPWM_OUT_MODE_FALLINGRISING;
    xPWMInit_Struct.xPWM_DBCTL.xPWM_POLSEL = xPWM_POLSEL_OPPOSITEEPWMxB;
    xPWMInit_Struct.xPWM_DBCTL.xPWM_IN_MODE =xPWM_IN_MODE_EPWMxA_FALLING_A_RISING;
    
    xPWMInit_Struct.xPWM_DBRED = 1000;
    xPWMInit_Struct.xPWM_DBFED = 1000;
    
    xPWMInit_Struct.xPWM_TZSEL.xPWM_TZx_OSHT[5]=xPWM_TZx_n_OPEN;
    xPWMInit_Struct.xPWM_TZSEL.xPWM_OSHTMOD = xPWM_OSHTMOD_IMMEDIATELY;
    
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZA = xPWM_TZx_LOW;
    xPWMInit_Struct.xPWM_TZCTL.xPWM_TZB = xPWM_TZx_LOW;
    
    xPWMInit_Struct.xPWM_ETSEL.xPWM_INTSEL = xPWM_INTSEL_ZERO;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_INTEN = xPWM_INTEN_ENABLE;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCASEL = xPWM_SOCABSEL_CPR1INC;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCAEN = xPWM_SOCABEN_ENABLE;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCBSEL = xPWM_SOCABSEL_CPR2DEC;
    xPWMInit_Struct.xPWM_ETSEL.xPWM_SOCBEN = xPWM_SOCABEN_ENABLE;
    
    xPWMInit_Struct.xPWM_ETPS.xPWM_INTPRD  =xPWM_INT_SOCABPRD_SOCABCNT01;
    xPWMInit_Struct.xPWM_ETPS.xPWM_SOCAPRD =xPWM_INT_SOCABPRD_SOCABCNT01;
    xPWMInit_Struct.xPWM_ETPS.xPWM_SOCBPRD =xPWM_INT_SOCABPRD_SOCABCNT01; 
    
    xPWM_Init(xPWM1,&xPWMInit_Struct);
    
    xPWMInit_Struct.xPMW_TBCTL.xPWM_CTRMODE = xPWM_CTRMODE_INC_DEC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PRDLD = xPWM_PRDLD_FREE;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_SYNCOSEL = xPWM_SYNCOSEL_EPWMxSYNCI;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSDIR = xPWM_PHSDIR_INC;
    xPWMInit_Struct.xPMW_TBCTL.xPWM_FREESOFT = xPWM_FREESOFT_CONTINUE;
    
    xPWMInit_Struct.xPMW_TBCTL.xPWM_PHSEN = xPWM_PHSEN_SYNCVALID;
    xPWMInit_Struct.xPWM_TBPHS = 0;
    
    xPWMInit_Struct.xPWM_CPR1 =0x0;
    xPWMInit_Struct.xPWM_CPR2 =0x0;
    
    xPWM_Init(xPWM2,&xPWMInit_Struct);    
    
    xPWM_Init(xPWM3,&xPWMInit_Struct);    
    xPWM_TZEINT(xPWM,xPWM_TZINT_CBC,ENABLE);
    //xPWM_TZEINT(xPWM,xPWM_TZINT_OST,DISABLE);
    xPWM_INTClear(xPWM);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_OST);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_CBC);
    xPWM_TZCLR(xPWM,xPWM_TZCLR_OST);
    xPWM_START(xPWM);
}

void ADC_normalTest(void)
{
    unsigned int tmp = 0xfffff;
	
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
	
    /*设置模拟IO成模拟功能*/
	SCU->AD_IO_REUSE_CFG = 0xFFFFFFFF;
    
    //Scu_Setdiv_Parameter((unsigned int)ADC,63);
    //Scu_SetClkDiv_Enable((unsigned int )ADC);
	
    Scu_SetDeviceGate(ADC_BASE,ENABLE);
    ADC_ContinuousConvert(DISABLE);
    //ADC_Calibrate(DISABLE);
    ADC_Calibrate(ENABLE);
   //ADC_PeriodAble(DISABLE);
    ADC_PeriodAble(ENABLE);
    ADC_IntSel(DISABLE);
    ADC_MaskInt(DISABLE);
    ADC_CovertConfig(ADC_Channel_2,ADC_CLKcycle_5);
    //*(volatile unsigned int*)(0x40010000+0x94) = 0x2;
    //Scu_SetIOReuse(UART1_CTS_PAD,FORTH_FUNCTION);
    //Scu_SetIOReuse(UART1_RTS_PAD,FORTH_FUNCTION);
    //Scu_SetIOReuse(I2S0_SDI_PAD,FORTH_FUNCTION);
    ADC_PowerCtrl(ENABLE);
    while(tmp--);
    ADC_SOCSoftCtrl(ADC_Channel_2);
}

void adc_normal_handler(void)
{
    static unsigned int addr = 0x70000000,i = 0;
    //mprintf("result:%d\n",ADC_GetResult(ADC_Channel_0));
    //ADC_SOCSoftCtrl(ADC_Channel_1);

    *(unsigned short*)(addr+i*2)=ADC_GetResult(ADC_Channel_2);
    ADC_SOCSoftCtrl(ADC_Channel_2);
    i++;
    if(i>=20000)
    {
      ADC_IntClear(ADC_Channel_ALL);
      ADC_ContinuousConvert(DISABLE);
      ADC_PowerCtrl(DISABLE);
      ADC_MaskInt(ENABLE);
      mprintf("adc done\n");
      for(unsigned int cnt=0;cnt<20000;cnt++)
      {
        mprintf("%x\t",*(unsigned short*)(0x70000000+cnt*2));
      }
      mprintf("adc test\n");
    }
}


void por_test(void)
{
    Scu_SetIOReuse(I2S0_SCLK_PAD,THIRD_FUNCTION);
    Scu_SetIOReuse(I2S0_SDI_PAD,THIRD_FUNCTION);
    Scu_SetDeviceGate((unsigned int)GPIO0,ENABLE);
    GPIO_Output(GPIO0,GPIO_Pin18,1);
    GPIO_Output(GPIO0,GPIO_Pin19,0);
    while(1)
    {
      unsigned int tmp = 0xffff;
      while(tmp--);
      GPIO_DATA_Set(GPIO0,GPIO_Pin18,0);
      tmp=0xffff;
      while(tmp--);
      GPIO_DATA_Set(GPIO0,GPIO_Pin18,1);
    }
}

////////////////--------adc0 for AD key
////////////////--------adc2 for Bat Detected

#define KEY_SHORTPRESS_TIMER 5//100*10ms = 1s
#define KEY_LONGPRESS_TIMER 150//100*10ms = 1s
#define KEY_LONGRELEASE_TIMER 300//100*10ms = 1s

/**
  * @功能:set ADC IO resue as adc 状态	
  * @注意:无       
  * @参数: channel
  */
void ADC_SetIOReuse(int channel)
{
    SCU->AD_IO_REUSE_CFG |= (1 << channel);
}

const int KEYADC[]={3582,3051, 2459, 1864, 1293, 777,350};// for evb
//=======================================================
void ADKey_Init(int adcx)
{
    NVIC_InitTypeDef NVIC_InitStruct = {0};
    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    Scu_Setdiv_Parameter(ADC_BASE,63);
    Scu_SetClkDiv_Enable(ADC_BASE);
    
    Scu_SetDeviceGate(ADC_BASE,1);
    //set AIN1 as ad0
    Scu_SetIOReuse(AIN0_PAD,FIRST_FUNCTION);     
    //SCU->AD_IO_REUSE_CFG =0Xff;
    ADC_SetIOReuse(ADC_Channel_0);//

    ADC_PowerCtrl(ENABLE);	
#if 0//连续采样		 
    ADC_ContinuousConvert(ENABLE);//DISABLE
    ADC_Calibrate(DISABLE);//ENABLE
    ADC_PeriodAble(DISABLE);
    ADC_IntSel(DISABLE);
#else//周期采样
    ADC_ContinuousConvert(DISABLE);//关闭连续采样，这时才支持单次周期采样
    ADC_PeriodAble(ENABLE);//ENABLE 周期采样
    ADC_PeriodMonitor(ADC_Channel_0,ENABLE);//enable ch0的周期采样
    ADC_CHxPeriod(ADC_Channel_0,62);//设定ch0的采样间隔
        
    ADC_Calibrate(DISABLE);
    ADC_IntSel(DISABLE);//DISABLE 每次采样均产生中断
#endif
    ADC_MaskInt(DISABLE);//不屏蔽中断
    ADC_CovertConfig(ADC_Channel_0,ADC_CLKcycle_1);//ADC_CLKcycle_5

    
    ADC_SOCSoftCtrl(ADC_Channel_0);
    
    //AIN1 used as IO key    
    Scu_SetIOReuse(AIN1_PAD,THIRD_FUNCTION );//GPIO51
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE); 
    GPIO_Input(GPIO1,GPIO_Pin19);//AIN1 
}

int keyMapping(int value)
{
    int keyid;
    if(!GPIO_ReadDATA(GPIO1,GPIO_Pin19))
    {
         keyid = KEY_SW1;
    }
    else
    {
        if(value>KEYADC[0])
        {
            keyid = KEY_NULL;
        }
        else if(value>KEYADC[1])
        {
            keyid = KEY_SW2;
        }
        else if(value>KEYADC[2])
        {
            keyid = KEY_SW3;
        }
        else if(value>KEYADC[3])
        {
            keyid = KEY_SW4;
        }
        else if(value>KEYADC[4])
        {
            keyid = KEY_SW5;
        }
        else if(value>KEYADC[5])
        {
            keyid = KEY_SW6;
        }
        else if(value>KEYADC[6])
        {
            keyid = KEY_SW7;
        }
        else 
        {
            keyid = KEY_NULL;
        }
    }
    return keyid;
}


static int tkey_Prevpresse=KEY_NULL;
static int tkey_presseed=KEY_NULL;
static int tkey_Longpress;

// 10ms 进入一次 
void adkey_handler(void)
{
    static int keyvalue=KEY_NULL;
    BaseType_t xHigherPriorityTaskWoken;

    tkey_presseed=keyMapping(ADC_GetResult(ADC_Channel_0));   
    if(tkey_presseed!=KEY_NULL)
    {
        if(tkey_presseed == tkey_Prevpresse)
        {
            tkey_Longpress ++;		
        }
        else
        {
            tkey_Prevpresse = tkey_presseed ;
            tkey_Longpress = 1;
        }

        if(tkey_Longpress%KEY_LONGPRESS_TIMER == 0)	
        {
            keyvalue = tkey_Prevpresse|C_KEY_HOLD;
        }
        else if(tkey_Longpress==KEY_SHORTPRESS_TIMER)  //100ms+
        {
            keyvalue = tkey_Prevpresse;
        }
    }
    else
    {
        if(tkey_Prevpresse!=KEY_NULL)
        {
            if(tkey_Longpress > KEY_LONGRELEASE_TIMER)
            {  
                keyvalue = tkey_Prevpresse|C_KEY_RELEASE;
            }
            tkey_presseed=tkey_Prevpresse = tkey_Longpress=KEY_NULL;
        }
    }
    
    if(keyvalue!=KEY_NULL)
    {
        //mprintf("key-->>>>(0x%x)\r\n",keyvalue);	
        user_msg_t send_msg;
        send_msg.msg_type = MSG_TYPE_SYS_KEY;
        send_msg.msg_data.key_data.key = keyvalue;
        xQueueSendFromISR(user_task_queue, &send_msg, &xHigherPriorityTaskWoken);
        keyvalue =  KEY_NULL;
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);  // TODO:in exit interrupt more better
    }
}