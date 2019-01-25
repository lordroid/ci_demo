#ifndef __SMT_CS42L52_H
#define __SMT_CS42L52_H

#ifdef __cplusplus
extern "C"{
#endif
  
#include "ci100x_i2c.h"
#include "ci100x_iis.h"
#include "ci100x_iisdma.h"
  
typedef struct{
  	unsigned int Mclk;
	unsigned int Lrck;   
}CS42L52Init_TypeDef; 

typedef enum 
{
	IC_TYPE_8388=0,
	IC_TYPE_8374=1
}ES_IC_TYPE;

extern ES_IC_TYPE g_ESIC_Type;

void cs42l52_test(void);
int cs42l52_init(CS42L52Init_TypeDef* CS42L52Init_Struct);
void i2splay_16k_16bit_test(void);
void iis1_play_handler(void);
void iis1_record_handler(void);
void write_smt_cs42l52(unsigned int regaddr,char val);
int read_smt_cs42l52(unsigned int regaddr);
void i2s1_record_realtime_play_handler(void);
void i2s1_record_realtime_play(void);
void i2s_slave_record_test(void);
void i2s_cs42l52_record(unsigned int sramaddr,unsigned int bytesize);
void de_codeadpcm(char* src,unsigned int size,short* dst);
void de_codeadpcm_mono_to_stereo(char* src,unsigned int size,short* dst);
void iis1_cs42l52_play_cmd(char *addr,unsigned int size);
void iis1_cs42l52_play_cmd_handler(void);
void iis1_play_pcm_handler(void);
void iis1_cs42l52slave_aec(char* WavAddr,unsigned int WavSize);
void cs42l52slave_ace(void);
void os_iis1_cs42l52_adpcmplay(char* WavAddr,unsigned int WavSize);
void de_codeadpcm_mono_to_stereo(char* src,unsigned int size,short* dst);
void iis1_cs42l52_adpcmplay_cmd_handler(void);
void iis1_play_init(void);
void play_voice_index(int index);
void ES8374_CloseSpeaker(void);
void ES8374_OpenSpeaker(void);
#ifdef __cplusplus
}
#endif

#endif /*__SMT_CS42L52_H*/

