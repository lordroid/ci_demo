#ifndef __TASK_ASR_H
#define __TASK_ASR_H

#ifdef __cplusplus
extern "C"{
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h" 
  
extern void * handle;
#define LOOP_BUF_CNT(head, tail, size) (((head) - (tail)) & ((size)-1))

#define FILECONFIG_SPIFLASH_START_ADDR      0x2000

typedef enum
{
    VAD_TYPE_HARD = 0,
    VAD_TYPE_SOFT,
    VAD_TYPE_MIXED,
}VAD_TYPE_ENUM;

typedef enum
{
    ACTIVE_LEVEL1,
    ACTIVE_LEVEL2,
    ACTIVE_LEVEL3,
    ACTIVE_LEVEL4,
    ACTIVE_LEVEL5,
    ACTIVE_LEVEL6,
    ACTIVE_LEVEL7,
    ACTIVE_LEVEL8,
    ACTIVE_LEVEL9,
    ACTIVE_LEVEL10,
    ACTIVE_LEVEL11,
}ACTIVE_LEVEL_ENUM;

typedef enum
{
    DYNAMIC_THRESHHOLD_SILENCE = 1,
    DYNAMIC_THRESHHOLD_NOISE,
    DYNAMIC_THRESHHOLD_MIDNOISE_PERSON,
    DYNAMIC_THRESHHOLD_MIDNOISE_OTHER,
}DYNAMIC_THRESHHOLD;


#define VAD_TYPE    VAD_TYPE_HARD
void vTask_Test(void *pvParameters);  
void vTask_Asrdealwith(void *pvParameters);  
void vTask_ttyS0(void *pvParameters);  
void ASR_VAD_DNN_Init(int CIRBMDL_flag);
int asrSetActiveByLevel(ACTIVE_LEVEL_ENUM active_level);
extern int switch_language_req(int dDNNId,int dASRId,int dLanguageId);

#ifdef __cplusplus
}
#endif

#endif /*__TASK_ASR_H*/
