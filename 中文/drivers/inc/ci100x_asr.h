/**
  ******************************************************************************
  * @文件    ci100x_asr.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的语音识别模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */

#ifndef __SMT_ASR_H_
#define __SMT_ASR_H_
#ifdef __cplusplus
    extern "C"{
#endif


/**************************************************************************
                    debug macro
****************************************************************************/
enum ASRLOG_LEVEL
{
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_DEBUG = 2,
    LOG_INFO,
};

#define ASR_DEBUG_LEVEL LOG_WARN

/*need open SecureCRT ansi color*/
#define asr_loginfo(fmt, args...) do { if (ASR_DEBUG_LEVEL>=LOG_INFO) mprintf(fmt, ## args); } while(0)
#define asr_logdebug(fmt, args...) do { if (ASR_DEBUG_LEVEL>=LOG_DEBUG) mprintf(fmt, ## args); } while(0)
#define asr_logwar(fmt, args...) do { if (ASR_DEBUG_LEVEL>=LOG_WARN) mprintf("\x1b[35m"fmt"\x1b[0m", ## args); } while(0)
#define asr_logerr(fmt, args...) do { if (ASR_DEBUG_LEVEL>=LOG_ERROR) mprintf("\x1b[31m"fmt"\x1b[0m", ## args); } while(0)


#define TEST_INTO_UNWAKEUP_CHECK_BUSY 1
#if TEST_INTO_UNWAKEUP_CHECK_BUSY
typedef enum 
{
    ASR_DECODE_RESULT_STATE_CLEAR = 0,
    ASR_DECODE_RESULT_STATE_BAD_RESULT = 1,
    ASR_DECODE_RESULT_STATE_GOOD_RESULT = 2,
    ASR_DECODE_RESULT_STATE_TIMEOUT = 3,
}asr_decode_have_result_t;
extern unsigned int asr_decode_already_have_result;/*0:no result,and clear state,1:bad result,2:good result,3:no result and timeout*/
extern unsigned int asr_decode_busy;/*0:asr not busy,1:asr busy*/
#define unwakeup_check_busy_long(fmt,args...) do{if(0) mprintf(fmt,##args);} while(0)
#endif

typedef struct
{
    /*ASR_INT_EN*/
    unsigned int    EXTFIFO_ALMOSTFULL_EN;
    unsigned int    DNN_VOC_DONE_EN;
    unsigned int    DNN_RST_DONE_EN;
    unsigned int    DNN_BUS_REQ_EN;
    unsigned int    VAD_DATA_READY_EN;
    unsigned int    MFCC_DATA_READY_EN;
    unsigned int    PITCH_DATA_READY_EN;
    unsigned int    VAD_DONE_EN;
    unsigned int    VAD_END_EN;
    unsigned int    VAD_START_EN;
    /*ASR_VAD_CFG*/
    unsigned int    VAD_START_OFFSET;
    unsigned int    VAD_MAX_SILLEN;
    unsigned int    VAD_MIN_SILLEN;
    /*ASR_VAD_MIN_BACK_ENG*/
    unsigned int    VAD_MIN_BACK_ENG;
    /*ASR_VAD_PRSP*/
    unsigned int    VAD_PRSP;
}ASR_InitStruct;

extern unsigned int ASR_MODEL_LENGTH;
extern unsigned int ASR_DNN_PARA_LENGTH;

unsigned int asr_Preprocessing(void);
void asr_Cancelprocessing(void);
void asr_init(ASR_InitStruct *InitStruct,int CIRBMDL_flag);

void scu_vadasrreset(void);
void Asr_open(void);
void Vad_open(void);
extern int ReservedFunc(char *val);
extern int SwitchVad(int val);
extern void ASR_VAD_DNN_Init(int CIRBMDL_flag);

#define VAD_STATE_START     0 // vad start
#define VAD_STATE_VADEND    1 // vad hw end
#define VAD_STATE_DNNEND    2// vad DNN end 
#define VAD_STATE_RECEND    3 // vad recognize end
#define VAD_STATE_RSTEND    4 // RST END
#define DNN_STATE_START     5 //DNN start
#define ASR_DECODE_START    6 //decode start
#define VAD_STATE_RETEND    7

#ifdef __cplusplus
}
#endif

#endif
