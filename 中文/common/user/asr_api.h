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

/*ʱ�䣺2017-05-19
�汾��v2.0.1
�������ݣ�
1���������������ò����Ľӿڣ�
   int asrSetBeam(void* handle, float beam);
   ˵����beam��Ĭ��ֵΪ10��beamֵԽ�������Ŀռ�Խ��ʶ���ʻ���ߣ������ٶȻ������
         beamֵ̫С�Ļ��ᵼ�������ռ��С��ʶ���ʻ��½���������С��ҪС��9
   
   int asrSetAmScale(void* handle, float scale);
   ˵����scale��Ĭ��ֵΪ0.08��scaleֵ̫���Ӱ��ʶ���ʣ����Ҫ����0.1��
         scaleֵ̫С�ᣬ��ʶ��ή�ͣ����ǻᵼ�ºܶ������ʶ�𲻳��������鲻ҪС��0.05
   
*/   

#endif  /*__ASR_API_H_*/
