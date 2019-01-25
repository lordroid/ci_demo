#ifndef __SMT_AT24C08B_H
#define __SMT_AT24C08B_H

#ifdef __cplusplus
extern "C"{
#endif
  
#include "ci100x_uart.h"
#include "ci100x_scu.h"
#include "ci100x_it.h"
#include "misc.h"
#include "ci100x_i2c.h"  
 

  
#define SMT_AT24C08B_ADDR 0x50

void AT24C08B_ByteWrite(char wordaddr,char data);  
void AT24C08B_PageWrite(char wordaddr,char *buf,int cnt);  
char AT24C08B_CurrentAddressRead(void);
char AT24C08B_RandomRead(char wordaddr);
void AT24C08B_SequentailRead(char *pbuf,int cnt);

void SMT_AT24C08Btest(void);

#ifdef __cplusplus
}
#endif

#endif /*__SMT_AT24C08B*/