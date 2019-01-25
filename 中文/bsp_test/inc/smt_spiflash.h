#ifndef __SMT_SPIFLASH_H
#define __SMT_SPIFLASH_H
#ifdef __cplusplus
extern "C"{
#endif 
typedef struct{
	unsigned int addr;
	unsigned int size;
}Voice_mp3;



extern Voice_mp3 voice_addr_size[75];

void smt_spiflashtest(void);

void flashtosdram(unsigned int sdramaddr,unsigned int flashaddr,\
  unsigned int size);
void QSPI_Init(void);
#ifdef __cplusplus
}
#endif 

#endif /*__SMT_SPIFLASH_H*/