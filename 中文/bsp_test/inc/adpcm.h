#ifndef __ADPCM_H
#define __ADPCM_H

#include "smt.h"
unsigned char ADPCM_Encode(  int sample);
short ADPCM_Decode(unsigned char code);
extern   short  Decodeindex  ;
extern   int Decodepredsample  ;
extern   short  Encodeindex ;
extern   int Encodepredsample ;

#endif /* __ADPCM_H*/
