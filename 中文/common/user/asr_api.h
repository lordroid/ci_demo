#ifndef __ASR_API_H_
#define __ASR_API_H_
#include "user_config.h"
 
//-------------------------------------------
void* asrCreate(const char* fst0, const char* fst1);

int   asrStart(void* handle,int idx);

//int   asrStart(void* handle);

int   asrRecognize(void* handle, float* likelihoods, int likes_len);

int   asrStop(void* handle);

int   asrReachedFinal(void* handle);

int   asrReset(void* handle); //   asrRelease
 
int   asrGetResult(void* handle, char** result);
 
int   asrGetResult(void* handle, const char** result);
 
void  asrRelease(void* handle);  //   asrReset

int asrSetWordsTag(void* handle,const char* tag);

int asrGetNetInfo(void* handle,int* size,int* outnums,int*hidnums,int*layernums);

int asrSwitchWordsFst(void* handle,int idx);

int   asrSetBeam(void* handle, float beam);

int   asrSetAmScale(void* handle, float scale);

/*时间：2017-05-19
版本：v2.0.1
更新内容：
1、增加了两个设置参数的接口：
   int asrSetBeam(void* handle, float beam);
   说明：beam的默认值为10，beam值越大搜索的空间越大，识别率会提高，但是速度会变慢；
         beam值太小的话会导致搜索空间变小，识别率会下降，建议最小不要小于9
   
   int asrSetAmScale(void* handle, float scale);
   说明：scale的默认值为0.08，scale值太大会影响识别率，最大不要超过0.1，
         scale值太小会，误识别会降低，但是会导致很多命令词识别不出来，建议不要小于0.05
   
*/   

#endif  /*__ASR_API_H_*/
