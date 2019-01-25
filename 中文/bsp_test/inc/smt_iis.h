#ifndef __SMT_IIS_H
#define __SMT_IIS_H

#ifdef __cplusplus
extern "C"{
#endif 


#include "ci100x_iis.h"
#include "ci100x_iisdma.h"

void i2stosram(unsigned int sramaddr,unsigned int bytesize);
void i2s0_record_tosram(unsigned int sramaddr,unsigned int bytesize);
void iis0_rxrecord_handler(void);
void i2s0rx_recordvoice(void);
void iis1_rxrecord_handler(void);
void i2s1_record_tosram(void);
void IIS0_SlaveRxpcm(void);
 
#ifdef __cplusplus
}
#endif 

#endif /*__SMT_IIS_H*/