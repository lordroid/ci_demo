#ifndef __CS42L52_H
#define __CS42L52_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 *CS42L52 internal registers
 *参见datasheet   page40 table 定义的该寄存器
 */
#define CODEC_TYPE  CS42L52_CODEC
//#define ENABLE_DEBUG_I2C
#define SUPPORT_ENCODE_AUDIO
#define CODEC_CS42L52_CHIP	0x01
#define CHIP_ID   		0xE0
#define CHIP_ID_MASK		0xF8
#define CHIP_REV_A0		0x00
#define CHIP_REV_A1		0x01
#define CHIP_REV_B0		0x02
#define CHIP_REV_MASK		0x03


#define CODEC_CS42L52_PWCTL1	0x02  //0x01
#define PWCTL1_PDN_CHRG		(1 << 7)
#define PWCTL1_PDN_PGAB		(1 << 4)
#define PWCTL1_PDN_PGAB_SHIFT	4
#define PWCTL1_PDN_PGAA   	(1 << 3)
#define PWCTL1_PDN_PGAA_SHIFT	3
#define PWCTL1_PDN_ADCB		(1 << 2)
#define PWCTL1_PDN_ADCB_SHIFT	2
#define PWCTL1_PDN_ADCA   	(1 << 1)
#define PWCTL1_PDN_ADCA_SHIFT	1
#define PWCTL1_PDN_CODEC	(1)



#define CODEC_CS42L52_PWCTL2	0x03 //0x07
#define PWCTL2_OVRDB		(1 << 4)
#define PWCTL2_OVRDA		(1 << 3)
#define	PWCTL2_PDN_MICB		(1 << 2)
#define PWCTL2_PDN_MICB_SHIFT	2
#define PWCTL2_PDN_MICA         (1 << 1)
#define PWCTL2_PDN_MICA_SHIFT	1
#define PWCTL2_PDN_MICBIAS	(1 << 0)
#define PWCTL2_PDN_MICBIAS_SHIFT	0


#define CODEC_CS42L52_PWCTL3	0x04
#define PWCTL3_HPB_PDN_SHIFT	6
#define PWCTL3_HPB_ON_LOW	0x00
#define PWCTL3_HPB_ON_HIGH	0x01
#define PWCTL3_HPB_ALWAYS_ON	0x02
#define PWCTL3_HPB_ALWAYS_OFF	0x03
#define PWCTL3_HPA_PDN_SHIFT    4
#define PWCTL3_HPA_ON_LOW       0x00
#define PWCTL3_HPA_ON_HIGH      0x01
#define PWCTL3_HPA_ALWAYS_ON    0x02
#define PWCTL3_HPA_ALWAYS_OFF   0x03
#define PWCTL3_SPKB_PDN_SHIFT   2
#define PWCTL3_SPKB_ON_LOW       0x00
#define PWCTL3_SPKB_ON_HIGH      0x01
#define PWCTL3_SPKB_ALWAYS_ON    0x02
#define PWCTL3_SPKB_ALWAYS_OFF   0x03
#define PWCTL3_SPKA_PDN_SHIFT   0
#define PWCTL3_SPKA_ON_LOW       0x00
#define PWCTL3_SPKA_ON_HIGH      0x01
#define PWCTL3_SPKA_ALWAYS_ON    0x02
#define PWCTL3_SPKA_ALWAYS_OFF   0x03
#define DEFAULT_OUTPUT_STATE	 0x05
#define PWCTL3_CONF_MASK	0x03




#define CODEC_CS42L52_CLK_CTL 	0x05 //0xa0
#define CLK_CTL_AUTODECT_ENABLE	(1 << 7)
#define CLK_CTL_SPEED_SHIFT	5
#define CLK_CTL_S_DS_MODE	0x00
#define CLK_CTL_S_SS_MODE	0x01
#define CLK_CTL_S_HS_MODE	0x02
#define CLK_CTL_S_QS_MODE 	0x03
#define CLK_CTL_32K_SR_SHIFT	4
#define CLK_CTL_32K_SR		1
#define CLK_CTL_NOT_32K		0
#define CLK_CTL_27M_MCLK_SHIFT	3
#define CLK_CTL_27M_MCLK	1
#define CLK_CTL_NOT_27M		0
#define CLK_CTL_RATIO_SHIFT	1
#define CLK_CTL_RATIO_128	0x00
#define CLK_CTL_RATIO_125	0x01
#define CLK_CTL_RATIO_132	0x02
#define CLK_CTL_RATIO_136	0x03
#define CLK_CTL_MCLKDIV2	1
#define CLK_CTL_NOT_MCLKDIV2	0



#define CODEC_CS42L52_IFACE_CTL1 0x06 //0
#define IFACE_CTL1_MASTER		(1 << 7)
#define IFACE_CTL1_INV_SCLK		(1 << 6)
#define IFACE_CTL1_ADC_FMT_I2S		(1 << 5)
#define IFACE_CTL1_ADC_FMT_LEFT_J 	(0 << 5)
#define IFACE_CTL1_DSP_MODE_EN		(1 << 4)
#define IFACE_CTL1_DAC_FMT_LEFT_J 	(0 << 2)
#define IFACE_CTL1_DAC_FMT_I2S  	(1 << 2)
#define IFACE_CTL1_DAC_FMT_RIGHT_J 	(2 << 2)
#define IFACE_CTL1_WL_32BIT		(0x00)
#define IFACE_CTL1_WL_24BIT		(0x01)
#define IFACE_CTL1_WL_20BIT		(0x02)
#define IFACE_CTL1_WL_16BIT		(0x03)
#define IFACE_CTL1_WL_MASK		0xFFFD



#define CODEC_CS42L52_IFACE_CTL2 0x07 //0
#define IFACE_CTL2_SC_MC_EQ 		(1 << 6)
#define IFACE_CTL2_LOOPBACK		(1 << 5)
#define IFACE_CTL2_S_MODE_OUTPUT_EN 	(0 << 4)
#define IFACE_CTL2_S_MODE_OUTPUT_HIZ 	(1 << 4)
#define IFACE_CTL2_HP_SW_INV		(1 << 3)
#define IFACE_CTL2_MIC_BIAS_5X 	0x00
#define IFACE_CTL2_MIC_BIAS_6X  0x01
#define IFACE_CTL2_MIC_BIAS_7X	0x02
#define IFACE_CTL2_MIC_BIAS_8X	0x03
#define IFACE_CTL2_MIC_BIAS_83X	0x04
#define	IFACE_CTL2_MIC_BIAS_91X	0x05


#define CODEC_CS42L52_ADC_PGA_A	0x08 //0x81
#define CODEC_CS42L52_ADC_PGA_B 0x09 //0x81
#define ADC_SEL_SHIFT		5
#define PGA_SEL_SHIFT           0
#define ADC_SEL_AIN1		0x00
#define ADC_SEL_AIN2		0x01
#define ADC_SEL_AIN3		0x02
#define ADC_SEL_AIN4		0x03
#define ADC_SEL_PGA		0x04
#define PGA_SEL_NONE		0x00
#define PGA_SEL_AIN1		0x01
#define PGA_SEL_AIN2		0x02
#define PGA_SEL_AIN3		0x04
#define PGA_SEL_AIN4		0x08
#define PGA_SEL_MIC		0x10
#define PGA_SEL_MIC_AIN1	0x11
#define PGA_SEL_MIC_AIN1_AIN2	0x13

#define AIN1_INPUT  0x81
#define AIN2_INPUT  0x82
#define AIN3_INPUT  0x84
#define AIN4_INPUT  0x88
#define MIC_INPUT  0x90
#define MIC_INPUT_MIX_AIN1  0x91
#define MIC_INPUT_MIX_AIN1_2  0x93

#define CODEC_CS42L52_ANALOG_HPF_CTL	0x0A //0xa5
#define HPF_CTL_ANLGSFTB		(1 << 3)
#define HPF_CTL_ANLGSFTA                (1 << 0)


#define CODEC_CS42L52_ADC_HPF_FREQ	0x0B //0
#define CODEC_CS42L52_ADC_MISC_CTL	0x0C //0
#define ADC_MISC_CTL_SOURCE_DSP		(1 << 6)
#define ADC_MISC_CTL_SOURCE_DSP		(1 << 6)



#define CODEC_CS42L52_PB_CTL1	0x0D //0x60
#define PB_CTL1_HP_GAIN_SHIFT	5
#define PB_CTL1_HP_GAIN_03959	0x00
#define PB_CTL1_HP_GAIN_04571	0x01
#define PB_CTL1_HP_GAIN_05111	0x02
#define PB_CTL1_HP_GAIN_06047	0x03
#define PB_CTL1_HP_GAIN_07099	0x04
#define PB_CTL1_HP_GAIN_08399	0x05
#define PB_CTL1_HP_GAIN_10000 	0x06
#define PB_CTL1_HP_GAIN_11430	0x07
#define PB_CTL1_INV_PCMB	(1 << 3)
#define PB_CTL1_INV_PCMA	(1 << 2)
#define PB_CTL1_MSTB_MUTE	(1 << 1)
#define PB_CTL1_MSTA_MUTE	(1 << 0)
#define PB_CTL1_MUTE_MASK	0xFFFD

#define CODEC_CS42L52_MISC_CTL	0x0E //0x02
#define MISC_CTL_DEEMPH		(1 << 2)
#define MISC_CTL_DIGSFT		(1 << 1)
#define MISC_CTL_DIGZC		(1 << 0)


#define CODEC_CS42L52_PB_CTL2	0x0F //0

#define PB_CTL2_HPB_MUTE	(1 << 7)
#define PB_CTL2_HPA_MUTE	(1 << 6)
#define PB_CTL2_SPKB_MUTE	(1 << 5)
#define PB_CTL2_SPKA_MUTE	(1 << 4)
#define PB_CTL2_SPK_SWAP	(1 << 2)
#define PB_CTL2_SPK_MONO	(1 << 1)
#define PB_CTL2_SPK_MUTE50	(1 << 0)

#define	CODEC_CS42L52_MICA_CTL	0x10 //0
#define CODEC_CS42L52_MICB_CTL  0x11 //0

#define	MIC_CTL_SEL_MIC1	(0 << 6)
#define MIC_CTL_SEL_MIC2	(1 << 6)
#define MIC_CTL_SEL_DIFF	(1 << 5)

#define CODEC_CS42L52_PGAA_CTL	0x12 //0
#define CODEC_CS42L52_PGAB_CTL 	0x13 //0

#define PGAX_CTL_VOL_12DB	24
#define PGAX_CTL_VOL_6DB	12 /*step size 0.5db*/

#define CODEC_CS42L52_PASSTHRUA_VOL	0x14 //0
#define CODEC_CS42L52_PASSTHRUB_VOL     0x15 //0

#define CODEC_CS42L52_ADCA_VOL	0x16 //0
#define CODEC_CS42L52_ADCB_VOL	0x17 //0
#define ADCX_VOL_24DB		24               /*step size 1db*/
#define ADCX_VOL_12DB		12
#define ADCX_VOL_6DB		6

#define CODEC_CS42L52_ADCA_MIXER_VOL	0x18 // 0x80
#define CODEC_CS42L52_ADCB_MIXER_VOL	0x19 //0x80
#define ADC_MIXER_VOL_12DB	0x18

#define CODEC_CS42L52_PCMA_MIXER_VOL	0x1A //0
#define CODEC_CS42L52_PCMB_MIXER_VOL	0x1B //0

#define CODEC_CS42L52_BEEP_FREQ	0x1C //0
#define CODEC_CS42L52_BEEP_VOL	0x1D //0
#define BEEP_VOL_12DB	0x06


#define CODEC_CS42L52_BEEP_TONE_CTL	0x1E //0

#define CODEC_CS42L52_TONE_CTL	0x1F //0x88

#define CODEC_CS42L52_MASTERA_VOL	0x20 //0
#define CODEC_CS42L52_MASTERB_VOL	0x21 //0

#define CODEC_CS42L52_HPA_VOL	0x22 //0
#define CODEC_CS42L52_HPB_VOL	0x23 //0
#define DEFAULT_HP_VOL	0xF0
//#define HP_VOL_STARTVAL	0xF0

#define CODEC_CS42L52_SPKA_VOL	0x24 //0
#define CODEC_CS42L52_SPKB_VOL	0x25 //0
#define DEFAULT_SPK_VOL	0xF0

#define CODEC_CS42L52_ADC_PCM_MIXER	0x26 //0

#define CODEC_CS42L52_LIMITER_CTL1	0x27 //0
#define CODEC_CS42L52_LIMITER_CTL2	0x28 //0x7f
#define CODEC_CS42L52_LIMITER_AT_RATE	0x29 //0xc0

#define CODEC_CS42L52_ALC_CTL	0x2A //0
#define ALC_CTL_ALCB_ENABLE	(1 << 7)
#define ALC_CTL_ALCA_ENABLE	(1 << 6)
#define ALC_CTL_FASTEST_ATTACK	0

#define CODEC_CS42L52_ALC_RATE	0x2B //0x3f
#define ALC_SLOWEST_RELEASE	0x3F

#define CODEC_CS42L52_ALC_THRESHOLD	0x2C //0
#define ALC_MAX_RATE_SHIFT	5
#define ALC_MIN_RATE_SHIFT	2
#define ALC_RATE_0DB	0
#define ALC_RATE_3DB	1
#define ALC_RATE_6DB	2

#define CODEC_CS42L52_NOISE_GATE_CTL	0x2D //0
#define NG_ENABLE	(1 << 6)
#define NG_THRESHOLD_SHIFT	2
#define NG_MIN_70DB	2
#define NG_DELAY_SHIFT	0
#define NG_DELAY_100MS	1

#define CODEC_CS42L52_CLK_STATUS	0x2E //0
#define CODEC_CS42L52_BATT_COMPEN	0x2F //0

#define CODEC_CS42L52_BATT_LEVEL	0x30 //0
#define CODEC_CS42L52_SPK_STATUS	0x31 //0
#define SPK_STATUS_PIN_SHIFT		3
#define SPK_STATUS_PIN_HIGH		1

#define CODEC_CS42L52_TEM_CTL	0x32 //0x3b
#define CODEC_CS42L52_THE_FOLDBACK	0x33 //0
#define CODEC_CS42L52_CHARGE_PUMP	0x34 //0x5f


#define SOC_CS42L52_REG_NUM	56

typedef enum {
 Codec_Init = 0,
 Codec_DACMode,
 Codec_FMMode,
 Codec_FMADCMode,
 Codec_LineInMode,
 Codec_MICMode,
 Codec_CellMode,
 
 Codec_DAC_MUTE,
 Codec_DAC_UNMUTE,
 Codec_FM_MUTE,
 Codec_FM_UNMUTE,
 Codec_FMADC_Disable,
 Codec_FMADC_Enable,
 Codec_MIC_Disable,
 Codec_MIC_Enable,
 Codec_Cell_Disable,
 Codec_Cell_Enable,
 Codec_DAC_PowerUp,
 Codec_DAC_PowerDown,
 
 Codec_Standby,
}CodecMode_en_t;

typedef enum{
	FS_8000Hz = 0,
	FS_11025Hz,
	FS_12KHz,
	FS_16KHz,
	FS_22050Hz,
	FS_24KHz,
	FS_32KHz,
	FS_48KHz,
	FS_44KHz,
	FSSTOP,
}CodecFS_en_t;

typedef enum {
EQ_Mode_Classic,
EQ_Mode_POP,
EQ_Mode_JAZZ,
EQ_Mode_Rock,
EQ_Mode_Normal,
EQ_Mode_,
	
}Codec_EQ_Mode_t;

void Codec_PowerOnInitial(int arg);
void Codec_Set_Music_Dac_Mode(int arg);
void Codec_Set_Mic_Adc_Mode(int arg);
void Codec_Set_Fm_Adc_Mode(int);
void Codec_Set_Fm_Bypass_Mode(int arg);
void Codec_Set_Cell_Mon_Mode(int arg); 	
void Codec_PowerOn_Adc(int arg);
void  Codec_PowerDown_Adc(int arg);
void Codec_PowerDown_Entile_Codec(int arg);
void Codec_PowerUp_Entile_Codec(int arg);

void Codec_SetVolume(unsigned char dev_type,unsigned int Volume);
//void Codec_SetMode(CodecMode_en_t Codecmode);
//void Codec_PowerDown(unsigned flag);
int Codec_Monitor_Voltage();
void Codec_Set_EQ_Mode(Codec_EQ_Mode_t mode);  
void Codec_PowerOnInitial(int arg);  
void Codec_SetSampleRate(CodecFS_en_t CodecFS);
void CS42L52_DefaultConfig(void);
void head_tail_regs_test(void);
void cs42l52_hp_play(void);
void cs42l52slave_hp_spk_play(void);
void cs42l52_mute(char val);
void mic_spk_cfg(void);
#ifdef __cplusplus
}
#endif

#endif