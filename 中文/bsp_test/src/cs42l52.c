#include "cs42l52.h"
#include "smt_cs42l52.h"
#include "ci100x_uart.h"
 
 
#define ENABLE_DEBUG_I2C

static int I2C_Write(unsigned char RegAddr,unsigned char *codec_data,unsigned count)
{
    int ret =-1;
    /*count must be 1*/
    write_smt_cs42l52(RegAddr,*codec_data);
    return ret;  	  				
}

static int I2C_Read(unsigned char RegAddr,unsigned char *codec_data,unsigned count)
{
    int ret = -1;
    /*count must be 1*/
    *codec_data=read_smt_cs42l52(RegAddr);
    return ret;
}

unsigned char Codec_ReadReg(unsigned char RegAddr)
{
//    int ret;
    unsigned char Codec_data = 0;
    //ret = I2C_Read(RegAddr, &Codec_data,1);
    I2C_Read(RegAddr, &Codec_data,1);
    return Codec_data;
}
unsigned char GetCodecVesion(void)
{
//    int ret;
    unsigned char Codec_data = 0;
    //ret = I2C_Read(0x01, &Codec_data,1);
    I2C_Read(0x01, &Codec_data,1);
    return Codec_data;

}
void CodecCommandSet(unsigned char RegAddr, unsigned uData)
{
    unsigned char   Codec_data;
    Codec_data = (unsigned char)uData;
    I2C_Write(RegAddr,  &Codec_data,1);
#ifdef ENABLE_DEBUG_I2C 
    Codec_data = 0;
    I2C_Read(RegAddr,&Codec_data,1);   
    //_printf("reg addr %x,value %x \n",RegAddr,Codec_data);
#endif    
}
/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_ADCMute
*  DESCRIPTION:   set codec ADC soft mute
*  INPUT:         NONE
*  RETURN:      NONE
*  NOTES:        this function only used when ADC working
*  -------------------------------------------------------------------------- */
void Codec_ADCMute(void)
{
    CodecCommandSet(CODEC_CS42L52_ADC_MISC_CTL, Codec_ReadReg(CODEC_CS42L52_ADC_MISC_CTL)&(~PWCTL1_PDN_CODEC)  );      // ADC soft mute
}

void Codec_ADCUnMute(void)
{
    
}
/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_DACMute
*  DESCRIPTION:   set codec DAC soft mute
*  INPUT:         NONE
*  RETURN:      NONE
*  NOTES:        this function only used when DAC working
*  -------------------------------------------------------------------------- */
void Codec_DACMute(void)
{
    CodecCommandSet(CODEC_CS42L52_PB_CTL2, ( Codec_ReadReg(CODEC_CS42L52_PB_CTL2)|0x01));      // DAC soft mute
}

/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_SetVolumet
*  DESCRIPTION:  set codec exit soft un MUTE
*  INPUT:         NONE
*  RETURN:      NONE
*  NOTES:        this function only used when DAC working
*  -------------------------------------------------------------------------- */
void Codec_DACUnMute(void)
{
    CodecCommandSet(CODEC_CS42L52_PB_CTL2, ( Codec_ReadReg(CODEC_CS42L52_PB_CTL2)&0xFE));      // DAC soft mute
}

/* --------------------------------------------------------------------------
*  FUNCTION:    Codec_PowOnInitial:
*
*  DESCRIPTION: Initial codec when power up
*
*  RETURN:      NONE
*
*  NOTES:
*  -------------------------------------------------------------------------- */

static int dec_gain = 20;

//static int init_dac_flag = 0;
//extern void audio_util_set_clk(unsigned freq, unsigned fs_config);
void Codec_PowerOnInitial(int arg)
{
    //if(init_dac_flag == 0)
    {
        CodecCommandSet(0x00,0x99);
        CodecCommandSet(0x3e,0xba);
        CodecCommandSet(0x47,0x80);
        CodecCommandSet(0x32,Codec_ReadReg(0x32)|(1<<7));
        CodecCommandSet(0x32,Codec_ReadReg(0x32)&(~(1<<7)));
        CodecCommandSet(0x00,00);
        CS42L52_DefaultConfig();
        CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x9f);  //close adc 0x02
        /*normally power both speaker and hp,let pin31 to configure the mode*/
        /*when pin31 high,power down the hp,power on the spk*/
        CodecCommandSet(CODEC_CS42L52_PWCTL3, 0x05);//control the hp and speaker 0x04;
        /** clock control,0x05**/
        //CodecCommandSet(CODEC_CS42L52_CLK_CTL,CLK_CTL_AUTODECT_ENABLE|CLK_CTL_MCLKDIV2);//0x05
        
        CodecCommandSet(CODEC_CS42L52_CLK_CTL,0xa0/*CLK_CTL_AUTODECT_ENABLE*/);//0x05
        /*i2s output ,16bit sample input--0x06*/
        CodecCommandSet(CODEC_CS42L52_IFACE_CTL1,IFACE_CTL1_ADC_FMT_LEFT_J|IFACE_CTL1_DAC_FMT_LEFT_J|IFACE_CTL1_WL_32BIT/*0x27*/); //0x06 slave mode(adc/dac i2s/left justiesd 
        /** power on adc/dac **/
        CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);// MIC Power Down  0x03            
        CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x00);//Disable the ADC->DAC digloop 0x07	
        CodecCommandSet(CODEC_CS42L52_SPKA_VOL, dec_gain);//volume configuration
        CodecCommandSet(CODEC_CS42L52_SPKB_VOL, dec_gain);//volume	
        CodecCommandSet(CODEC_CS42L52_HPA_VOL, dec_gain);
        CodecCommandSet(CODEC_CS42L52_HPB_VOL, dec_gain);	
        //CodecCommandSet(CODEC_CS42L52_PCMA_MIXER_VOL,0x18);//0x1a
        //CodecCommandSet(CODEC_CS42L52_PCMB_MIXER_VOL,0x18);//0x1b
        CodecCommandSet(0x1e,Codec_ReadReg(0x1e)&0x3f);//beep and tone configuration
        CodecCommandSet(0x0f,Codec_ReadReg(0x0f)&0x06);//
        CodecCommandSet(0x2f,Codec_ReadReg(0x2f)|0xc6);//cell compesation disable;
        //CodecCommandSet(CODEC_CS42L52_PWCTL1,0x1E);//Power ON Codec
    
        CodecCommandSet(CODEC_CS42L52_PB_CTL2, ( Codec_ReadReg(CODEC_CS42L52_PB_CTL2)|0x04));     
	CodecCommandSet(CODEC_CS42L52_PB_CTL1,0xa0);  // hpgain set to 0.8399
        
        /** used for battary monitor,since we use a audio dec,we should privide a i2s clock for it ,In case we have not set it before monitor*/
        /*******///audio_util_set_clk(5/*AUDIO_CLK_FREQ_441*/,0);
        //init_dac_flag = 1;
    }  
}

/* --------------------------------------------------------------------------
*  FUNCTION:    Codec_RampUp:
*
*  DESCRIPTION: Initial codec when power up
*
*  RETURN:      NONE
*
*  NOTES:
*  -------------------------------------------------------------------------- */
void Codec_RampUp(void)
{

}

 void CodecGainSet(unsigned char i)  //设置录音的音量
{
 

}

/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_DeIitial:
*  DESCRIPTION:   power off audio codec
*  INPUT:         NONE
*  RETURN:      NONE
*  NOTES:
*  -------------------------------------------------------------------------- */
void Codec_DeIitial(void)
{

    Codec_DACMute();
    Codec_ADCMute();
    CodecCommandSet(CODEC_CS42L52_PWCTL1,    0x00);
//	if(master_i2c_info.I2C_device_fd > 0)
//		close(master_i2c_info.I2C_device_fd);
}

/* --------------------------------------------------------------------------
*  FUNCTION:    Codec_SetMode:
*
*  DESCRIPTION:  setup codec input/output to application mode
*  INPUT:        CodecMode_en_t Codec_DACout/Codec_FMin/Codec_LineADC/Codec_MICAdc/Codec_Standby
*  RETURN:      NONE
*  NOTES:      if exit from application, like FM or MIC , please set codec to standby mode
*  -------------------------------------------------------------------------- */
//extern void audio_util_set_clk(unsigned freq, unsigned fs_config);

void Codec_Set_Music_Dac_Mode(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
    CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);// MIC Power Down
    CodecCommandSet(0x0f,0);
    //CodecCommandSet(0x0f,Codec_ReadReg(0x0f)&0x0f);//unmute hp&spk
    CodecCommandSet(0xe,Codec_ReadReg(0xe)&0x3f);//disable analog passthrough              
    CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x00);//Disable the ADC->DAC digloop
    /***power down adc,pga****/
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x1);//Power ON Codec		
}

void Codec_Set_Mic_Adc_Mode(int arg)
{
    //CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x9F); //0x02,power down device 
    CodecCommandSet(CODEC_CS42L52_PWCTL2, 0x00); //0x03 MIc power on
    //CodecCommandSet(0x0f,Codec_ReadReg(0x0f)|0xf0);//mute hp&spk
    CodecCommandSet( CODEC_CS42L52_ADC_PGA_A, MIC_INPUT/*0x90*/); //0x08 select MIC
    CodecCommandSet( CODEC_CS42L52_ADC_PGA_B, MIC_INPUT); //0x09
    //CodecCommandSet(0x0a,0xa0);//enable high pass filter
    CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x00); //Disable the ADC->DAC digloop
    
    CodecCommandSet(CODEC_CS42L52_MICA_CTL, Codec_ReadReg(CODEC_CS42L52_MICA_CTL)|0x30); //0x10,Mic1,diff input, 16dB gain
    CodecCommandSet( CODEC_CS42L52_MICB_CTL, Codec_ReadReg(CODEC_CS42L52_MICB_CTL)|0x30); //0x11
    /**enable ALC control here**/
    CodecCommandSet(0x2a,0xc0);//enable ALC, fastest attack rate
    CodecCommandSet(0x2b,0x3f);//lowest release rate
    CodecCommandSet(0x2c,0x1c);//max alc threshold == 0db,min == -30db
    CodecCommandSet(0x0a,0xaa);//enable HPF&zero cross
    CodecCommandSet(0x12,0x41);//PGA_A enable soft ramp,gain == 0.5db
    CodecCommandSet(0x13,0x41);//PGA_B 
    //the user can set the Tone, Volume at here
    
    CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x00); //Power up device, power down ADC
}

void Codec_Set_Fm_Adc_Mode(int)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
    CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);////MIc power down
    
    CodecCommandSet(CODEC_CS42L52_ADC_PGA_A,  AIN2_INPUT);//pga  select ani1
    CodecCommandSet(CODEC_CS42L52_ADC_PGA_B,  AIN2_INPUT);//pga select ani1
    
    CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x20);  //Enable the ADC->DAC digloop
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x00);   //Power up device, power down ADC
}

void Codec_Set_Fm_Bypass_Mode(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
    CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);// MIC Power Down 
	
    CodecCommandSet(0xe,Codec_ReadReg(0xe)|0xc0);//enable analog passthrough
	
    CodecCommandSet(CODEC_CS42L52_ADC_PGA_A,  AIN1_INPUT);//pga  select ani1
	
    CodecCommandSet(CODEC_CS42L52_ADC_PGA_B,  AIN1_INPUT);//pga select ani1
	
    CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x1E);//Power ON Codec,
}


void Codec_Set_Cell_Mon_Mode(int arg) 	
{
    CodecCommandSet(0x2f,Codec_ReadReg(0x2f)|0x4F);//battary monitor able and refence voltage 5v. 
   // CodecCommandSet(CODEC_CS42L52_PWCTL1,0x00);//power on ADCA&B 	
}

void Codec_PowerOn_Adc(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,Codec_ReadReg(CODEC_CS42L52_PWCTL1)&0xf9);
    CodecCommandSet(CODEC_CS42L52_PWCTL2,0x00);
}
void  Codec_PowerDown_Adc(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,Codec_ReadReg(CODEC_CS42L52_PWCTL1)|0x06);
    CodecCommandSet(CODEC_CS42L52_PWCTL2,0x7);//power down mic pre_amp and bia circuit		
}
/** power down all the codec chip**/
void Codec_PowerDown_Entile_Codec(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,0x9f);
    	
}

void Codec_PowerUp_Entile_Codec(int arg)
{
    CodecCommandSet(CODEC_CS42L52_PWCTL1,0x9e);		
}

/*
void Codec_SetMode(CodecMode_en_t Codecmode)
{

    switch (Codecmode)
    {
        case Codec_DACMode:
            {
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
            CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);// MIC Power Down
            CodecCommandSet(0x0f,Codec_ReadReg(0x0f)&0x0f);//unmute hp&spk
            CodecCommandSet(0xe,Codec_ReadReg(0xe)&0x3f);//disable analog passthrough              
            CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x00);//Disable the ADC->DAC digloop
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x1E);//Power ON Codec
            }
            break;
        
        case Codec_FMMode:
            {
            	CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
            	CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);// MIC Power Down
            	CodecCommandSet(0xe,Codec_ReadReg(0xe)|0xc0);//enable analog passthrough
            	CodecCommandSet(CODEC_CS42L52_ADC_PGA_A,  AIN1_INPUT);//pga  select ani1
                CodecCommandSet(CODEC_CS42L52_ADC_PGA_B,  AIN1_INPUT);//pga select ani1
            	CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x1E);//Power ON Codec	
            
            }
            break;
        
        case Codec_FMADCMode:
            {
            		
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
            CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);////MIc power down
            
            CodecCommandSet(CODEC_CS42L52_ADC_PGA_A,  AIN2_INPUT);//pga  select ani1
            CodecCommandSet(CODEC_CS42L52_ADC_PGA_B,  AIN2_INPUT);//pga select ani1
            
            CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x20);  //Enable the ADC->DAC digloop
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x00);   //Power up device, power down ADC
            
            
            }
            break;
            
        case Codec_LineInMode:
            {
            
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x9F);//Power Down Codec
            CodecCommandSet(CODEC_CS42L52_PWCTL2,     0x07);////MIc power down
            
            CodecCommandSet(CODEC_CS42L52_ADC_PGA_A,  AIN1_INPUT);//pga  select ani1
            CodecCommandSet(CODEC_CS42L52_ADC_PGA_B,  AIN1_INPUT);//pga select ani1
            
            CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x20);  //Enable the ADC->DAC digloop
            CodecCommandSet(CODEC_CS42L52_PWCTL1,     0x00);   //Power up device, power down ADC
            
            
            }
            break;
        
        case Codec_MICMode:
            {
            CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x9F); //0x02,power down device 
            CodecCommandSet(CODEC_CS42L52_PWCTL2, 0x00); //0x03 MIc power on
            CodecCommandSet(0x0f,Codec_ReadReg(0x0f)|0xf0);//mute hp&spk
            CodecCommandSet( CODEC_CS42L52_ADC_PGA_A, MIC_INPUT); //0x08 select MIC
            CodecCommandSet( CODEC_CS42L52_ADC_PGA_B, MIC_INPUT); //0x09
            //CodecCommandSet(0x0a,0xa0);//enable high pass filter
            CodecCommandSet(CODEC_CS42L52_IFACE_CTL2, 0x00); //Disable the ADC->DAC digloop
            
            CodecCommandSet(CODEC_CS42L52_MICA_CTL, Codec_ReadReg(CODEC_CS42L52_MICA_CTL)|0x30); //0x10,Mic1,diff input, 16dB gain
            CodecCommandSet( CODEC_CS42L52_MICB_CTL, Codec_ReadReg(CODEC_CS42L52_MICB_CTL)|0x30); //0x11
            //enable ALC control here
            CodecCommandSet(0x2a,0xc0);//enable ALC, fastest attack rate
            CodecCommandSet(0x2b,0x3f);//lowest release rate
            CodecCommandSet(0x2c,0x1c);//max alc threshold == 0db,min == -30db
            CodecCommandSet(0x0a,0xaa);//enable HPF&zero cross
            CodecCommandSet(0x12,0x41);//PGA_A enable soft ramp,gain == 0.5db
            CodecCommandSet(0x13,0x41);//PGA_B 
            //the user can set the Tone, Volume at here
            
            CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x00); //Power up device, power down ADC
            
            
            }
            break;
        case Codec_CellMode:
        {   	
            CodecCommandSet(0x2f,Codec_ReadReg(0x2f)|0x4F);//battary monitor able and refence voltage 5v. 
            CodecCommandSet(CODEC_CS42L52_PWCTL1,0x00);//power on ADCA&B 
        }	
            break;	    
        //case Codec_Cell_Monitor_Disable:
            CodecCommandSet(0x2f,Codec_ReadReg(0x2f)&0xbf);
            break;
        case Codec_Standby:
            {
            
            
            }
            break;
    
    }

}
*/

/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_SetSampleRate:
*  DESCRIPTION:  setup codec sample rate both for ADC and DAC application
*  INPUT:         CodecFS_en_t  8kHz ~  48KHz
*  RETURN:      NONE
*  NOTES:
*  -------------------------------------------------------------------------- */
void Codec_SetSampleRate(CodecFS_en_t CodecFS)
{
   
    if (CodecFS < FSSTOP)
    {
         CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x9F);   
		 
        switch (CodecFS)
        {
            case FS_8000Hz:
				
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_QS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_32K_SR << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);												 
                break;
            case FS_11025Hz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_QS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_136 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);   //12MHZ,11.025K
                break;
            case FS_12KHz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_QS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,12K
                break;
            case FS_16KHz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_HS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_32K_SR << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,16K
                break;
            case FS_22050Hz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_HS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_136 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,22.05K
                break;
            case FS_24KHz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_HS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,24K
                break;
            case FS_32KHz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_SS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_32K_SR << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,32K
                break;
            case FS_48KHz:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_SS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_125 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,48K
                break;
            case FS_44KHz:
            default:
                CodecCommandSet(CODEC_CS42L52_CLK_CTL, (CLK_CTL_S_SS_MODE << CLK_CTL_SPEED_SHIFT)|\
                                (CLK_CTL_NOT_32K << CLK_CTL_32K_SR_SHIFT)|\
                                (CLK_CTL_NOT_27M << CLK_CTL_27M_MCLK_SHIFT)|\
                                (CLK_CTL_RATIO_136 << CLK_CTL_RATIO_SHIFT)|\
                                CLK_CTL_NOT_MCLKDIV2);    //12MHZ,44.1KHZ
                break;
        }
        CodecCommandSet(CODEC_CS42L52_PWCTL1, 0x00);//Power up device
    }
}


/* --------------------------------------------------------------------------
*  FUNCTION:     Codec_SetVolumet:
*  DESCRIPTION:  set  codec output volume
*  INPUT:         volume
*  INPUT:		  dev_type: 0  speaker; 1:hp;2:microphone
*  RETURN:      NONE
*  NOTES:        
*	 globle volume configuration range:-64db~12db 
*	 mapping to the register setting(0->255->128);-64db<->128;0db<->0	
*  -------------------------------------------------------------------------- */
#define MAX_VOLUME 255
#define MIN_VOLUME 128
void Codec_SetVolume(unsigned char dev_type,unsigned int Volume)
{

//    unsigned char Vol ;
 
    if (Volume > MAX_VOLUME)
        Volume = MAX_VOLUME;
    else if( Volume< MIN_VOLUME)
        Volume = MIN_VOLUME;

    CodecCommandSet(CODEC_CS42L52_MASTERA_VOL, Volume);
    CodecCommandSet(CODEC_CS42L52_MASTERB_VOL, Volume); 	                     

}

int Codec_Monitor_Voltage()
{
   /* Vp equals Va*VPLEVEL/63.3, not really true :) */	
    return Codec_ReadReg(0x30); 
	
}

/*
	* reg: 0x1e.beep[4:3] 
	* pop : bass 7.9db -3db@100hz .treble -2.2db -3db@>>16hz
	* jazz: bass  
	* classic: 0db  -3db@>>3k .treble  -8db  -3db@5khz
	* rock :bass 8db -3db@>100hz  treble 11db  -3db@6khz
**/
void Codec_Set_EQ_Mode(Codec_EQ_Mode_t mode)
{
	switch(mode)
	{
		case EQ_Mode_Classic:
			CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,0x01);//0x1e
			CodecCommandSet(CODEC_CS42L52_TONE_CTL,0xa8);//0x1f
			break;
		case EQ_Mode_JAZZ:
			CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,0x0b);//0x1e
			CodecCommandSet(CODEC_CS42L52_TONE_CTL,0x21);//0x1e
			break;
		case EQ_Mode_POP:
			CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,0x1f);//0x1e
			CodecCommandSet(CODEC_CS42L52_TONE_CTL,0x83);//0x1e
			break;
		case EQ_Mode_Rock:
			CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,0x03);//0x1e
			CodecCommandSet(CODEC_CS42L52_TONE_CTL,0x13);//0x1e
			break;
	    case EQ_Mode_Normal:
	        CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,Codec_ReadReg(CODEC_CS42L52_BEEP_TONE_CTL)&0xfe);
			break;
		default:
		    CodecCommandSet(CODEC_CS42L52_BEEP_TONE_CTL,Codec_ReadReg(CODEC_CS42L52_BEEP_TONE_CTL)&0xfe);
			break;
	}
}

void CS42L52_DefaultConfig(void)
{
    const unsigned char _default[]={
                  0x00, 0xE0, 0x01, 0x07, 0x05, /*4*/
                  0xa0, 0x00, 0x00, 0x81, /*8*/
                  0x81, 0xa5, 0x00, 0x00, /*12*/
                  0x60, 0x02, 0x00, 0x00, /*16*/
                  0x00, 0x00, 0x00, 0x00, /*20*/
                  0x00, 0x00, 0x00, 0x80, /*24*/
                  0x80, 0x00, 0x00, 0x00, /*28*/
                  0x00, 0x00, 0x88, 0x00, /*32*/
                  0x00, 0x00, 0x00, 0x00, /*36*/
                  0x00, 0x00, 0x00, 0x7f, /*40*/
                  0xc0, 0x00, 0x3f, 0x00, /*44*/
                  0x00, 0x00, 0x00, 0x00, /*48*/
                  0x00, 0x3b, 0x00, 0x5f, /*52*/
    };
    for(int i=2;i<sizeof(_default)/sizeof(_default[0]);i++){
          CodecCommandSet(i,_default[i]);	
    }
}

const unsigned char tmp[]={
/*0x0 */ 0x0,
/*0x1 */ 0xE3,
/*0x2 */ 0x0,
/*0x3 */ 0x0,
/*0x4 */ 0xaa,
/*0x5 */ 0x80,
/*0x6 */ 0x0,
/*0x7 */ 0x0, 
/*0x8 */ 0x0,
/*0x9 */ 0x0,
/*0xA */ 0x0,
/*0xb */ 0x0,
/*0xc */ 0x0,
/*0xd */ 0xc0,
/*0xe */ 0x2,
/*0xf */ 0x0,
/*0x10 */ 0x0,
/*0x11 */ 0x0,
/*0x12 */ 0x0,
/*0x13 */ 0x0,
/*0x14 */ 0x0,
/*0x15 */ 0x0,
/*0x16 */ 0x0,
/*0x17 */ 0x0,
/*0x18 */ 0x80,
/*0x19 */ 0x80,
/*0x1A */ 0x0,
/*0x1B */ 0x0,
/*0x1C */ 0x0,
/*0x1D */ 0x0,
/*0x1E */ 0x0,
/*0x1F */ 0x88,
/*0x20 */ 0x0,
/*0x21 */ 0x0,
/*0x22 */ 0x0,
/*0x23 */ 0x0,
/*0x24 */ 0x0,
/*0x25 */ 0x0,
/*0x26 */ 0x0,
/*0x27 */ 0x0,
/*0x28 */ 0x7f,
/*0x29 */ 0xc0,
/*0x2A */ 0x0,
/*0x2B */ 0x3f,
/*0x2C */ 0x0,
/*0x2D */ 0x0,
/*0x2E*/ 0x0,
/*0x2F */ 0x0,
/*0x30 */ 0x0,
/*0x31 */ 0x0,
/*0x32 */ 0x3b,
/*0x33 */ 0x0,
/*0x34 */ 0x5f,
};

void head_tail_regs_test(void)
{
    for(int i=2;i<sizeof(tmp)/sizeof(tmp[0]);i++)
    {
      CodecCommandSet(i,tmp[i]);	
    }
	Codec_PowerOn_Adc(0);
	Codec_Set_Mic_Adc_Mode(0);
}

void cs42l52_hp_play(void)
{
    const unsigned char _tmp[]={
    /*0x0 */ 0x0,
    /*0x1 */ 0xE3,
    /*0x2 */ 0x0,
    /*0x3 */ 0x0,
    /*0x4 */ 0xaa,
    /*0x5 */ 0x80,
    /*0x6 */ 0x0,
    /*0x7 */ 0x0, 
    /*0x8 */ 0x0,
    /*0x9 */ 0x0,
    /*0xA */ 0x0,
    /*0xb */ 0x0,
    /*0xc */ 0x0,
    /*0xd */ 0xc0,
    /*0xe */ 0x2,
    /*0xf */ 0x0,
    /*0x10 */ 0x0,
    /*0x11 */ 0x0,
    /*0x12 */ 0x0,
    /*0x13 */ 0x0,
    /*0x14 */ 0x0,
    /*0x15 */ 0x0,
    /*0x16 */ 0x0,
    /*0x17 */ 0x0,
    /*0x18 */ 0x80,
    /*0x19 */ 0x80,
    /*0x1A */ 0x0,
    /*0x1B */ 0x0,
    /*0x1C */ 0x0,
    /*0x1D */ 0x0,
    /*0x1E */ 0x0,
    /*0x1F */ 0x88,
    /*0x20 */ 0x0,
    /*0x21 */ 0x0,
    /*0x22 */ 0xea,
    /*0x23 */ 0xea,
    /*0x24 */ 0xea,
    /*0x25 */ 0xea,
    /*0x26 */ 0x0,
    /*0x27 */ 0x0,
    /*0x28 */ 0x7f,
    /*0x29 */ 0xc0,
    /*0x2A */ 0x0,
    /*0x2B */ 0x3f,
    /*0x2C */ 0x0,
    /*0x2D */ 0x0,
    /*0x2E*/ 0x0,
    /*0x2F */ 0x0,
    /*0x30 */ 0x0,
    /*0x31 */ 0x0,
    /*0x32 */ 0x3b,
    /*0x33 */ 0x0,
    /*0x34 */ 0x5f,
    };
    
    for(int i=2;i<sizeof(_tmp)/sizeof(_tmp[0]);i++)
    {
      CodecCommandSet(i,_tmp[i]);	
    }
}

void cs42l52_mute(char val)
{
    if(val)
    {
      CodecCommandSet(0xf,0xFF);
    }
    else
    {
      CodecCommandSet(0xf,0x00);
    }
}
void cs42l52slave_hp_spk_play(void){
  	const unsigned char cfg[]=
	{/*0x0 */ 0x0,
	/*0x1 */ 0xE3,
	/*0x2 */ 0x0,
	/*0x3 */ 0x0,
	/*0x4 */ 0xaa,
	/*0x5 */ 0x80,
	/*0x6 */ 0x0,
	/*0x7 */ 0x0, 
	/*0x8 */ 0x0,
	/*0x9 */ 0x0,
	/*0xA */ 0x0,
	/*0xb */ 0x0,
	/*0xc */ 0x0,
	/*0xd */ 0xc0,
	/*0xe */ 0x32,
	/*0xf */ 0x0,
	/*0x10 */ 0x0,
	/*0x11 */ 0x0,
	/*0x12 */ 0x20,
	/*0x13 */ 0x20,
	/*0x14 */ 0x80,
	/*0x15 */ 0x80,
	/*0x16 */ 0x80,
	/*0x17 */ 0x80,
	/*0x18 */ 0x80,
	/*0x19 */ 0x80,
	/*0x1A */ 0x0,
	/*0x1B */ 0x0,
	/*0x1C */ 0x0,
	/*0x1D */ 0x0,
	/*0x1E */ 0x0,
	/*0x1F */ 0x88,
	/*0x20 */ 0x0,
	/*0x21 */ 0x0,
	/*0x22 */ 0xea,
	/*0x23 */ 0xea,
	/*0x24 */ 0xea,
	/*0x25 */ 0xea,
	/*0x26 */ 0x0,
	/*0x27 */ 0x1f,
	/*0x28 */ 0xff,
	/*0x29 */ 0x00,
	/*0x2A */ 0x0,
	/*0x2B */ 0x3f,
	/*0x2C */ 0x0,
	/*0x2D */ 0xc0,
	/*0x2E*/ 0x0,
	/*0x2F */ 0x0,
	/*0x30 */ 0x0,
	/*0x31 */ 0x0,
	/*0x32 */ 0x3b,
	/*0x33 */ 0x0,
	/*0x34 */ 0x5f,
	};
	for(int i=1;i<sizeof(cfg)/sizeof(*cfg);i++)
	{
		CodecCommandSet(i,cfg[i]);	
	}
}

void mic_spk_cfg(void){
	unsigned char mic_spk[]={
/*0x0 */ 0x0,
	/*0x1 */ 0xE3,
	/*0x2 */ 0x0,
	/*0x3 */ 0x0,
	/*0x4 */ 0xaa,
	/*0x5 */ 0x80,
	/*0x6 */ 0x0,
	/*0x7 */ 0x0, 
	/*0x8 */ 0x90,
	/*0x9 */ 0x90,
	/*0xA */ 0xaa,
	/*0xb */ 0x0,
	/*0xc */ 0x0,
	/*0xd */ 0xc0,
	/*0xe */ 0x2,
	/*0xf */ 0x0,
	/*0x10 */ 0xd0,
	/*0x11 */ 0xd0,
	/*0x12 */ 0x43,
	/*0x13 */ 0x43,
	/*0x14 */ 0x0,
	/*0x15 */ 0x0,
	/*0x16 */ 0x0,
	/*0x17 */ 0x0,
	/*0x18 */ 0x80,
	/*0x19 */ 0x80,
	/*0x1A */ 0x0,
	/*0x1B */ 0x0,
	/*0x1C */ 0x0,
	/*0x1D */ 0x0,
	/*0x1E */ 0x0,
	/*0x1F */ 0x88,
	/*0x20 */ 0x0,
	/*0x21 */ 0x0,
	/*0x22 */ 0xf0,
	/*0x23 */ 0xf0,
	/*0x24 */ 0xec,
	/*0x25 */ 0xec,
	/*0x26 */ 0x0,
	/*0x27 */ 0x0,
	/*0x28 */ 0x7f,
	/*0x29 */ 0xc0,
	/*0x2A */ 0xC0,
	/*0x2B */ 0x3f,
	/*0x2C */ 0x08,
	/*0x2D */ 0x40,
	/*0x2E*/  0x03,
	/*0x2F */ 0x0,
	/*0x30 */ 0x0,
	/*0x31 */ 0xc0,
	/*0x32 */ 0x3b,
	/*0x33 */ 0x0,
	/*0x34 */ 0x5f
	};
    for(int i=2;i<sizeof(mic_spk)/sizeof(mic_spk[0]);i++)
    {
      CodecCommandSet(i,mic_spk[i]);	
    }
}








