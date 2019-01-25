#ifndef _GPIO_CONFIG_H
#define _GPIO_CONFIG_H

#if (MODEL_DEFINE==MODEL_V1)
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MODEL_V1
    #if BOARD_TYPE
        #define LED_VAD_INIT {Scu_SetIOReuse(XPWM5_PAD,THIRD_FUNCTION);\
        Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin1,1);}
        #define LED_VAD_ON	 {GPIO_DATA_Set(GPIO1,GPIO_Pin1,0);}
        #define LED_VAD_OFF  {GPIO_DATA_Set(GPIO1,GPIO_Pin1,1);}
    #else
        #define LED_VAD_INIT {Scu_SetIOReuse(PWM6_PAD,THIRD_FUNCTION);\
        Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin2,1);}
        #define LED_VAD_ON	 { GPIO_DATA_Set(GPIO1,GPIO_Pin2,0);}	
        #define LED_VAD_OFF  { GPIO_DATA_Set(GPIO1,GPIO_Pin2,1);}

    #endif

    #define MUTE_INIT 	{Scu_SetIOReuse(AIN1_PAD,THIRD_FUNCTION);\
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin19,0);}
    #define MUTE_ON		{GPIO_DATA_Set(GPIO1,GPIO_Pin19,0);}	
    #define MUTE_OFF 	{GPIO_DATA_Set(GPIO1,GPIO_Pin19,1);}	

    #define RST_20921_INIT {Scu_SetIOReuse(AIN2_PAD,THIRD_FUNCTION);\
    Scu_SetDeviceGate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin20,0);}
    #define RST_20921_HIGH	{GPIO_DATA_Set(GPIO1,GPIO_Pin20,1);}		
    #define RST_20921_LOW	{GPIO_DATA_Set(GPIO1,GPIO_Pin20,0);}	

    #define LED_TEST_INIT ; 
    #define LED_TEST_ON   ; 
    #define LED_TEST_OFF  ;
    #define LED_TEST_STATE   0

    #define TF_DETECT_INIT 
    #define TF_DETECT    (0)


#elif (MODEL_DEFINE==MODEL_STD)
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MODEL_STD
    #define ON  1
    #define OFF 0

    #define LED_VAD_INIT {Scu_SetIOReuse(PWM6_PAD,THIRD_FUNCTION);Scu_SetDeviceGate\
    ((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin2,1);}
    #define LED_VAD_ON	 {GPIO_DATA_Set(GPIO1,GPIO_Pin2,ON);}
    #define LED_VAD_OFF  {GPIO_DATA_Set(GPIO1,GPIO_Pin2,OFF);}

    #define LED_ASR_INIT 	{Scu_SetIOReuse(AIN2_PAD,THIRD_FUNCTION);Scu_SetDevice\
    Gate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin20,1);}
    #define LED_ASR_ON	 	{GPIO_DATA_Set(GPIO1,GPIO_Pin20,0);}	
    #define LED_ASR_OFF  	{GPIO_DATA_Set(GPIO1,GPIO_Pin20,1);	}

    #define MUTE_INIT 	  ;//not ctrl mute 
    #define MUTE_ON		  ;
    #define MUTE_OFF 	  ;

    #define RST_20921_INIT {Scu_SetIOReuse(CAN_TX_PAD,THIRD_FUNCTION);Scu_SetDevice\
    Gate((unsigned int)GPIO0,ENABLE);GPIO_Output(GPIO0,GPIO_Pin11,0);}
    #define RST_20921_HIGH	{GPIO_DATA_Set(GPIO0,GPIO_Pin11,1);}		
    #define RST_20921_LOW	{GPIO_DATA_Set(GPIO0,GPIO_Pin11,0);	}

    #define LED_TEST_INIT 	{Scu_SetIOReuse(AIN3_PAD,THIRD_FUNCTION);Scu_SetDeviceG\
    ate((unsigned int)GPIO1,ENABLE);GPIO_Output(GPIO1,GPIO_Pin21,1);}
    #define LED_TEST_ON   	{GPIO_DATA_Set(GPIO1,GPIO_Pin21,ON);}	
    #define LED_TEST_OFF  	{GPIO_DATA_Set(GPIO1,GPIO_Pin21,OFF);}	 
    #define LED_TEST_STATE	(!GPIO_ReadDATA(GPIO1,GPIO_Pin21))

    #define TF_DETECT_INIT {Scu_SetIOReuse(XPWM5_PAD,THIRD_FUNCTION);Scu_SetDeviceG\
    ate((unsigned int)GPIO1,ENABLE);GPIO_Input(GPIO1,GPIO_Pin1);}
    #define TF_DETECT    (!GPIO_ReadDATA(GPIO1,GPIO_Pin1))


#else
    Error[Ci001]:---please set a defined MODEL_DEFINE ---
#endif

#endif
