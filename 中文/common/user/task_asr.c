/**
  ******************************************************************************
  * @file    task_asr.c 
  * @author  Chipintelli Technology Co., Ltd.
  * @version V1.0.0
  * @date    2016.04
  * @brief   语音识别任务文件
  ******************************************************************************
  **/

#include "includes.h" 
#include "ci100x_asr.h"
#include "task_asr.h"
#include "user_main.h"
#include "user_config.h"
#include "ci100x_asrcmd.h"
#include "smt_cs42l52.h"

#define CHECK_ASR_MEMORY_LEAK  0
#ifdef CHECK_ASR_MEMORY_LEAK/*check asr memory leak used*/
#include <iar_dlmalloc.h>
//extern int g_continue_reconigze_flag;//yinengyong
extern void CheckStatus(void);

struct mallinfo alloc_meminfo;
void getMemStatus()
{
    alloc_meminfo = __iar_dlmallinfo();/*可以查看动态分配内存的总大小*/
    mprintf("alloc: %d free:%d\n",alloc_meminfo.uordblks,alloc_meminfo.fordblks);
}

int getHeapMem(void)
{
    alloc_meminfo = __iar_dlmallinfo();
    return alloc_meminfo.uordblks;
}

#endif

/**************************************************************************
                    function prototype
****************************************************************************/
void asrmge_getResult(void);
void asrmge_ReInitasr(void);
void asrmge_asrclear(void);
void noresult_just_reset(void);
void asrmge_state_reset(void);
float get_dynamic_threshold(float score);
extern void Reset_PlayVoice_EndTime(void);

void vad_state_lib(int flag);
extern int macro_init(int SKIP_FRAME_,int SKIP_FRAME_NUM_,int USE_ADAPTSKP_,int DECODE_DEBUG_,\
                            int MAX_FRAME_TIME_,int MAX_FRAME_TIME_NUMS_,int MIN_FRAME_NUMS_);
extern int  asr_frame_decode1(asr_datatype * rev_data);
extern int  asr_frame_decode2(asr_datatype * rev_data);
extern int get_threshold_adjustment_standard(void);
/**************************************************************************
                    type define
****************************************************************************/
#define VADSTART_2_DNN_TIMEOUT 500/*500ms*/

 

#define SKIP_FRAME 1
#if SKIP_FRAME

    #define SKIP_FRAME_NUM 2


    #define MAX_SPNOrSIL 250//75
    #define ASR_CNT_TIME_2  20 //??¡è??a?-¡ª??¨C¨¨€¡­??¡ë??a?-¡ª????¡®???¡è¨¨¡¥??¡ª??€????cnt?¡è???¡ã??????
    #define ASR_CNT_TIME 8//20  //?¡§?¡¥a€??£¤?????£¤???¡ã?£¤?¡è???£¤?¡ãa€??|?????¡§???¡è?¡è???o?¡§?¡¥a€??£¤?????£¤???¡ã
    #define BESTCNT 1
#else
    #define SKIP_FRAME_NUM 1
    
   
    #define MAX_SPNOrSIL 500
    
    #define ASR_CNT_TIME_2  40 //??¡è??a?-¡ª??¨C¨¨€¡­??¡ë??a?-¡ª????¡®???¡è¨¨¡¥??¡ª??€????cnt?¡è???¡ã??????
    #define ASR_CNT_TIME 16//20  //¨¨¡¥??????¡ã?¡è??¡ã¡®???¨¨?¡è??o¨¨¡¥??????¡ã
    #define BESTCNT 1
#endif
    #define VADSTART_2_DECODE_TIMEOUT 5000/*5s*/
#define ASR_BEAM 9.0
    #define VAD_START_OFF 30
#if USE_ADAPTSKP
    #define MAX_FRAME_TIME (21)
    #define MAX_FRAME_TIME_NUMS (5)
    #define EXIT_MAX_FRAME_TIME_NUMS (1)  
    #define MIN_FRAME_NUMS (20)

#endif


typedef enum
{
    ASRSTATE_P_IDLE = 0,
    ASRSTATE_P_VADSTART,
    ASRSTATE_P_HASDNN_RELSULT1,
    ASRSTATE_P_VADEND_EARLY,
    ASRSTATE_P_DECODE_RESULT_QUICK=4,/*early vad end*/
    ASRSTATE_P_NEWVADSTART,
    ASRSTATE_P_DNNEND_DONE,
    ASRSTATE_P_HASDNN_RELSULT2,
    ASRSTATE_P_DECODE_RESULT_SLOW=8,/*after vad end*/
    ASRSTATE_P_DNNTIMEOUT,
    ASRSTATE_P_VADEND_TIMEOUT,
    ASRSTATE_P_CHECK_VAD_TIMEOUT,
}ASR_StateType;

 
extern void IIS0_SlaveRxSDRAMpcm(void);
extern void IIS0_SlaveRxSDRAMpcm_handler(void);

 
Para_Inof para_info = {0};
extern int InitData(void* p,unsigned int wake_addr,unsigned int asrcmd_addr,unsigned int flag);
extern int macro_asr_init(int asrcmd_maxsize,unsigned int asrcmd_addr,int wakecmd_addr,float beam,char* tag);
extern int macro_decode_init(int macro_use_separate_wakeup,int macro_use_adaptskp,\
            int macro_skip_frame,int macro_skip_frame_num ,int macro_max_frame_time,\
            int macro_max_frame_time_nums,int macro_min_frame_nums,int macro_use_combo_cmd);
extern int macro_cnt_init(int macro_suspend_time,int macro_asr_cnt_time_3,int macro_asr_cnt_time_2,\
            int macro_asr_cnt_time,int macro_max_spnorsil,int macro_bestcnt);
extern int macro_score_init(float macro_min_score,float macro_max_score,\
            float macro_score_th,float macro_wakeupscore_th,int macro_score_len,\
            char* tag);
extern int macro_vad_init(int macro_vad_max_sillen,unsigned int macro_vad_min_back_eng,\
            int macro_vad_min_sillen,unsigned int macro_vad_prsp,int macro_vad_start_offset, int macro_use_fft_softvad);
extern int macro_timeout_init(int macro_decode_debug,int macro_usesmic,int macro_use_6_156_2_16,\
            int macro_vadstart_2_dnn_timeout,int macro_vadstart_2_decode_timeout,\
            int macro_test_into_unwakeup_check_busy, int macro_use_8374);

extern int macro_vad_eng_low_init(int macro_sg_vad_eng_low_start,int macro_sg_vad_eng_low_end);
int sysinit_index = 0;
extern unsigned int vad_backengrg;

extern int asr_CheckAcSum(void);
extern int asr_GetArcsNotes(int *arc_nums, int *note_nums);
extern int asr_GetVersion(char* ver_buf); 
extern int asr_GetBeam(float *beam);
extern int asr_GetActive(int *min_active,int* max_active);
extern int set_start_no_use_frame_number(int number);
extern  void setFrameNode(int node1,int node2,int node3);
extern  void setFrameNodeActive(int active1,int active2,int active3);

void setandgetASRpara(void)
{
int arc_nums = 0;
int note_nums = 0;
int min_active = 0;
int max_active = 0;
char ver_buf[20] = {0};  
float beam = 0.0;

if(asr_CheckAcSum() != 0)
{
mprintf("warning:check ac_model fail!\r\n");
}
asr_GetArcsNotes(&arc_nums, &note_nums);
mprintf("arc_nums is :%d, note_nums is:%d\r\n",arc_nums, note_nums);
asr_GetVersion( ver_buf); 
mprintf("Version is :%s\r\n", ver_buf);
asr_GetBeam(&beam);
mprintf("beam is :%f\r\n", beam);
asr_GetActive(&min_active,&max_active);
mprintf("min_active is :%d ,max_active is :%d\r\n", min_active,max_active);
#if(!SKIP_FRAME)
setFrameNode(30,60,90);
setFrameNodeActive(300,500,300);
#endif
asrSetActiveByLevel(ACTIVE_LEVEL8);
set_start_no_use_frame_number(8);
}

void MacroInit(int uLanguageId)
{
    int esic_type = g_ESIC_Type;
    
    //最后一个参数是词组之间的字符标记
    if(uLanguageId  == SUPPORT_CHINESE_LANGUAGE)
   {
   	 macro_asr_init(SDRAM_ASRCMD_SIZE,SDRAM_ASRCMD_ADDR,SDRAM_WAKECMD_ADDR,ASR_BEAM,"");
   }
   else  if(uLanguageId  == SUPPORT_ENGLISH_LANGUAGE)
   {
      	 macro_asr_init(SDRAM_ASRCMD_SIZE,SDRAM_ASRCMD_ADDR,SDRAM_WAKECMD_ADDR,ASR_BEAM,"-");
   }
    
    //
    macro_decode_init(USE_SEPARATE_WAKEUP,USE_ADAPTSKP,\
        SKIP_FRAME,SKIP_FRAME_NUM ,MAX_FRAME_TIME,\
        MAX_FRAME_TIME_NUMS,MIN_FRAME_NUMS,USE_COMBO_CMD);
    
    macro_cnt_init(SUSPEND_TIME ,ASR_CNT_TIME_2,ASR_CNT_TIME_2,\
        ASR_CNT_TIME,MAX_SPNOrSIL,BESTCNT);
    
    macro_score_init(MIN_SCORE,MAX_SCORE,\
        SCORE_THRESHOLD,WAKEUP_SCORE_THRESHOLD,SCORE_LEN,\
            SCORE_TAG);
    // VAD_MAX_SILLEN  VAD_MIN_BACK_ENG VAD_MIN_SILLEN VAD_PRSP VAD_START_OFFSET
    #if USE_ASR8388
    macro_vad_init(25,vad_backengrg,10,0x3F000000,40,VAD_TYPE);
    #else

    //只使用8374作为播放的时候, 不能修改
    esic_type = 0;

    #if USE_6_156_2_16
    macro_vad_init(25,40000000,10,0x3E4CCCCD,35,VAD_TYPE);
    #else
    macro_vad_init(25,5000000,10,0x3E4CCCCD,35,VAD_TYPE);
    #endif
    #endif
    
    macro_timeout_init(DECODE_DEBUG,USE_ASR8388,USE_6_156_2_16,\
        VADSTART_2_DNN_TIMEOUT,VADSTART_2_DECODE_TIMEOUT,\
          TEST_INTO_UNWAKEUP_CHECK_BUSY,esic_type);
}
#ifdef NEW_MERGE_TOOL 
VoiceInfo vc_info = {0};
void GetUserPara(int dDNNId,int dASRId,int dLanguageId)
{
    volatile unsigned int tmp = 0;
    ConfigParameterInit_Typedef codeinfo = {0};
    nvdata_save.uLanguageId = dLanguageId;
    unsigned int duserdateaddr =SUPPORT_CHIESE_ASR_ADDRESS;
    int dPlayWordidoff= 1;
    
    FileConfig_Struct FileConfigInfo = {0};
    unsigned int FileConfigInfoLen;
    FileConfigInfoLen = sizeof(FileConfig_Struct);

	if(SPI_FLASH_SIZE>(8*1024*1024))
	{
		if(dLanguageId  == SUPPORT_CHINESE_LANGUAGE)
		{
			duserdateaddr  = SUPPORT_CHIESE_ASR_ADDRESS;
			dPlayWordidoff= 1;

		}
		else if(dLanguageId  == SUPPORT_ENGLISH_LANGUAGE)
		{
			dPlayWordidoff =50;//此值为中文的命令词个数asrcmd.c中AsrCmd_Struct列表中
			duserdateaddr  = SUPPORT_ENGLISH_ASR_ADDRESS;
		}
	}
	else
	{
		duserdateaddr  = SUPPORT_CHIESE_ASR_ADDRESS;
	}
    SpiFlash_ReadData(QSPIC,FILECONFIG_SPIFLASH_START_ADDR,(char*)&FileConfigInfo,FileConfigInfoLen);
	flashtosdram(SDRAM_REC_BUFFER_ADDR,FileConfigInfo.VoicePlayingStartAddr,8 * 1024); 
    
	tmp = (unsigned int)(SDRAM_REC_BUFFER_ADDR) ;
	tmp +=4;
	vc_info.voicenums= *(volatile unsigned int*)(tmp);
	tmp +=4;
    
	vc_info.arsaddr = FileConfigInfo.ASRCMDModelStartAddr;
	vc_info.asrsize = FileConfigInfo.ASRCMDModelSize;
	vc_info.dnnaddr = FileConfigInfo.DNNModelStartAddr;
	vc_info.dnnsize = FileConfigInfo.DNNModelSize;


    if(vc_info.voicenums>SUPPORT_MAX_VOICE)
    {
          vc_info.voicenums = SUPPORT_MAX_VOICE;
    }
    
   int last_index = -10000,current_index = -1;
    AsrCmd_Struct[0].pVoiceAddr = *(volatile unsigned int*)(tmp);
    AsrCmd_Struct[0].VoiceSize = *(volatile unsigned int*)(tmp + 4);

    for(int i = dPlayWordidoff,j = 2; (AsrCmd_Struct[i].id != -1); i++,j += 2)
    {
           
        current_index = AsrCmd_Struct[i].id;
         if(current_index==last_index)
        {
            last_index = current_index;
            AsrCmd_Struct[i].pVoiceAddr = AsrCmd_Struct[i-1].pVoiceAddr;
            AsrCmd_Struct[i].VoiceSize = AsrCmd_Struct[i-1].VoiceSize;
            j -= 2;
            continue;
        }
        last_index = current_index;
        AsrCmd_Struct[i].pVoiceAddr = *(volatile unsigned int*)(tmp + j * 4);
        AsrCmd_Struct[i].VoiceSize = *(volatile unsigned int*)(tmp + j * 4 + 4);
    }
    sysinit_index -= 4;
    para_info.asrcmdaddr = vc_info.arsaddr; 
    para_info.asrcmdsize = vc_info.asrsize;
    para_info.wakeupaddr = vc_info.wakeupaddr; 
    para_info.wakeupsize = vc_info.wakeupsize;
    g_decord_ver = 1;   
   InitData(&vc_info,SDRAM_WAKECMD_ADDR,SDRAM_ASRCMD_ADDR,1);   
}

#else
VoiceInfo vc_info = {0};
void GetUserPara(int dDNNId,int dASRId,int dLanguageId)
{
    volatile unsigned int tmp = 0,voicenums = 0;
    ConfigParameterInit_Typedef codeinfo = {0};
    nvdata_save.uLanguageId = dLanguageId;
    unsigned int duserdateaddr =SUPPORT_CHIESE_ASR_ADDRESS;

     int dPlayWordidoff= 1;

	if(SPI_FLASH_SIZE>(8*1024*1024))
	{
		if(dLanguageId  == SUPPORT_CHINESE_LANGUAGE)
		{
			duserdateaddr  = SUPPORT_CHIESE_ASR_ADDRESS;
			dPlayWordidoff= 1;

		}
		else if(dLanguageId  == SUPPORT_ENGLISH_LANGUAGE)
		{
			dPlayWordidoff =50;//此值为中文的命令词个数asrcmd.c中AsrCmd_Struct列表中
			duserdateaddr  = SUPPORT_ENGLISH_ASR_ADDRESS;
		}
	}
	else
	{
		duserdateaddr  = SUPPORT_CHIESE_ASR_ADDRESS;
	}
    flashtosdram(SDRAM_REC_BUFFER_ADDR,duserdateaddr,4 * 1024);

    memcpy(&codeinfo,(void*)SDRAM_REC_BUFFER_ADDR,sizeof(codeinfo));
    memcpy(&vc_info,(void*)(SDRAM_REC_BUFFER_ADDR+sizeof(codeinfo) + 4 * \
        codeinfo.UserCodeBackupNums),sizeof(vc_info));
    tmp = (unsigned int)(SDRAM_REC_BUFFER_ADDR + sizeof(codeinfo) + 4 * \
        codeinfo.UserCodeBackupNums) + sizeof(vc_info);
    
    if(vc_info.voicenums>SUPPORT_MAX_VOICE)
    {
          voicenums = SUPPORT_MAX_VOICE;
    }
    else
    {
          voicenums = vc_info.voicenums;
    }
    
   int last_index = -10000,current_index = -1;
    AsrCmd_Struct[0].pVoiceAddr = *(volatile unsigned int*)(tmp);
    AsrCmd_Struct[0].VoiceSize = *(volatile unsigned int*)(tmp + 4);

    for(int i = dPlayWordidoff,j = 2; (AsrCmd_Struct[i].id != -1); i++,j += 2)
    {
           
        current_index = AsrCmd_Struct[i].id;
         if(current_index==last_index)
        {
            last_index = current_index;
            AsrCmd_Struct[i].pVoiceAddr = AsrCmd_Struct[i-1].pVoiceAddr;
            AsrCmd_Struct[i].VoiceSize = AsrCmd_Struct[i-1].VoiceSize;
            j -= 2;
            continue;
        }
        last_index = current_index;
        AsrCmd_Struct[i].pVoiceAddr = *(volatile unsigned int*)(tmp + j * 4);
        AsrCmd_Struct[i].VoiceSize = *(volatile unsigned int*)(tmp + j * 4 + 4);
    }
    sysinit_index -= 4;
    para_info.asrcmdaddr = vc_info.arsaddr;
    para_info.asrcmdsize = vc_info.asrsize;
    para_info.wakeupaddr = vc_info.wakeupaddr;
    para_info.wakeupsize = vc_info.wakeupsize;
    g_decord_ver = 1;   
   InitData(&vc_info,SDRAM_WAKECMD_ADDR,SDRAM_ASRCMD_ADDR,1);
}
#endif


extern unsigned int result_count;
extern volatile unsigned int asr_trigger_event ;

extern  void reset_20921(void);
extern void iis1_ES8388_play_cmd();

#if 0
int IsWordsFinish(char *result)
{
    char words[120]={0};
    char dflag =0;
    int res_length=0;
    sscanf(result,"%s",words);
    res_length = strlen(words);
    for(int i=0;(AsrCmd_Struct[i].id != -1);i++)
    {
         if(!strncmp(words,AsrCmd_Struct[i].pCmd,res_length))
         {
             dflag++;
         }
	  if(dflag>1)
  	{
	  	return 1;
  	}
    }
    return 0;
}
#else
int IsWordsFinish(char *result)
{
    return 0;
}
#endif
int IsSpecialWords(char *result)
{
    char words[120]={0};
    int res_length=0;
    sscanf(result,"%s",words);
    res_length = strlen(words);
    for(int i=0;(specialwords_lst[i].dtimecnt != -1);i++)
    { 
      	 if(res_length != strlen(specialwords_lst[i].pCmd))
        {
            continue;
        }
    
         if(!strncmp(words,specialwords_lst[i].pCmd,res_length))
         {
            return specialwords_lst[i].dtimecnt;
         }	
    }
    return 0;
}

int IsWakeupWords(char* preuslt)
{
    if(!preuslt)return 0;
    int i=0;
    char words[120];

    sscanf(preuslt,"%s",words);
    for(i=0;i<SUPPORT_MAX_VOICE;i++)
    {
        if(!strcmp(words,wakewords_lst[i]))
        {
            return 1;                                                     
        }
        if(!strcmp("END",wakewords_lst[i]))
        {
            return 0;
        }
    }
    return 0;
}

 

void send_result_to_usertask(char *presult)
{
    if(NULL == presult)
    {
        return;
    }
    char* pscore = strstr(presult,SCORE_TAG);
    //mprintf("send result:%s\n",presult);
    if(pscore)
    {
        pscore += SCORE_LEN;
        float score = atof(pscore);
        asr_loginfo("%0.2f\n",score);

        int index = FindCmdIndex(presult,score);
        if(-1 != index)
        {
            Reset_PlayVoice_EndTime();
            #if TEST_INTO_UNWAKEUP_CHECK_BUSY
            asr_decode_already_have_result = ASR_DECODE_RESULT_STATE_GOOD_RESULT;/*set state before send*/
            unwakeup_check_busy_long("asr_decode_already_have_result = %d\n",asr_decode_already_have_result);
            #endif
            
            mprintf("send result:%s\n",presult);
            
            user_msg_t send_msg;
            send_msg.msg_type = MSG_TYPE_SYS_ASR;
            send_msg.msg_data.asr_data.asr_status = MSG_ASR_STATUS_GOOD_RESULT;
            send_msg.msg_data.asr_data.asr_index = index;
            send_msg.msg_data.asr_data.asr_score  = score;
            xQueueSend(user_task_queue,&send_msg,200);
            
            #if TEST_INTO_UNWAKEUP_CHECK_BUSY
            return;/*ASR_DECODE_RESULT_STATE_GOOD_RESULT*/
            #endif
        }
    }
    #if TEST_INTO_UNWAKEUP_CHECK_BUSY
    asr_decode_already_have_result = ASR_DECODE_RESULT_STATE_BAD_RESULT;
    unwakeup_check_busy_long("asr_decode_already_have_result = %d\n",asr_decode_already_have_result);
    #endif
}

void CheckStatusTimerSet(void)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};
    Scu_SetDeviceGate(TIMER2,ENABLE);
    TIMx_us(TIMER2,1000000);
    NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
 
/**
  * @??: FreeRTos ?????
  * @??: ?      
  * @??:?
  * @???:?
  */
//uint8_t pcWriteBuffer[500]={0};
void vTask_Test(void *pvParameters)
{
    unsigned int times = 0;
    CheckStatusTimerSet();
    for(;;)
    {
        mprintf("%#x:RTOS\n",times++);
        #if USE_IWDG
        iwdg_watch |= (1<<0);
        #endif

        vTaskDelay(1000); 
    }
}

/*****************************************
  * @功能: 
  * @注意: 无      
  * @参数:无
  * @返回值:1-0.25;2-0.2;3-0.25;4-0.2
  * @作者:csy 20181106
*****************************************/
float get_dynamic_threshold(float score)
{
    float threshold = 0.0;
    int threshold_flag = 0;
    
    threshold_flag = get_threshold_adjustment_standard();
    switch(threshold_flag)
    {
      case DYNAMIC_THRESHHOLD_SILENCE:     
          threshold = score + 0.05;
          break;
      case DYNAMIC_THRESHHOLD_NOISE:
          threshold = score;
          break;
      case DYNAMIC_THRESHHOLD_MIDNOISE_PERSON:
          threshold = score + 0.05;
          break;
      case DYNAMIC_THRESHHOLD_MIDNOISE_OTHER:
          threshold = score;
          break;
      default:
          threshold = score;
          break;				 
	}
    return threshold;
}

/**
  * @??: ????
  * @??: ?      
  * @??:1.result ????
  *       2.score ?????
  * @???:???index 
  */
int FindCmdIndex(char* result,double score)
{
    int ret = -1;
    int res_length=0;
    char* pscore;
    float threshold = 0.0;
    
    if(!result)
    {
        return -1; 
    }
    
    pscore = strstr(result,SCORE_TAG);
    res_length = pscore - result;
    
    for(int i=0;(AsrCmd_Struct[i].id != -1) && (i < SUPPORT_MAX_VOICE);i++)
    {
        if(res_length != strlen(AsrCmd_Struct[i].pCmd))
        {
            continue;
        }
        if(!strncmp(result,AsrCmd_Struct[i].pCmd,res_length))
        {
#if DYNAMIC_THRESHOLD
            threshold = get_dynamic_threshold(AsrCmd_Struct[i].Score);
#else
            threshold = AsrCmd_Struct[i].Score;
#endif			
            if(score - PERCISION > threshold)
            {
                ret = i;
            }
            break;
        }
    }

    return ret;
}

void vad_time2_init(void)
{
    static int ustime_init = 0;
    if(!ustime_init)
    {
	vad_cnt_timer = 0xffffffff;
	vad_end_timer = 0xffffffff;
	ustime_init = 1;        
	//delay unit 10ms
	NVIC_InitTypeDef NVIC_InitStructure={0};
	Scu_SetDeviceGate(TIMER2,ENABLE);
	TIMx_us(TIMER2,10000);
	NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    } 
}
void vad_time0_init(void)
{
    static int ustime_init = 0;
    if(!ustime_init)
    {
        vad_cnt_timer = 0xffffffff;
        vad_end_timer = 0xffffffff;
        ustime_init = 1;        
        //delay unit 5s
        NVIC_InitTypeDef NVIC_InitStructure={0};
        Scu_SetDeviceGate(TIMER0,ENABLE);
        TIMx_us(TIMER0,5000000);
        timer_stop(TIMER0);
        NVIC_InitStructure.NVIC_IRQChannel = TIMER0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    } 
}

extern void set_VAD_flag_status(int flag);
void vad_state_lib(int flag)
{
    int VAD_status = 0;//0:VAD off ;VAD on
#if VAD_ONLINE 
    vad_time2_init();
    vad_time0_init();
#endif
    if(flag != 5)
    {
        mprintf("VAD[%d]\n",flag);
    }
#if 1 
    static int vad_decode_timeflag=0;
    switch(flag)
    {
        case ASR_DECODE_START:
            mprintf("vad_decode_timeflag %d\n",vad_decode_timeflag);
            if(0x0==vad_decode_timeflag)
            {
                hardvad_timeout = 0X0;
                //TIMx_us(TIMER0,5000000);
                //timer_restart(TIMER0);
                vad_decode_timeflag = 1;
            }
            break;
        case VAD_STATE_VADEND:
            hardvad_timeout = 0X0;
            vad_decode_timeflag =0;
            //timer_stop(TIMER0);
            break;
        case VAD_STATE_RSTEND:
            hardvad_timeout = 0X0;
            vad_decode_timeflag =0x0;
            //timer_stop(TIMER0);
            break;
        case VAD_STATE_DNNEND:
            hardvad_timeout = 0X0;
            vad_decode_timeflag =0x0;
            //timer_stop(TIMER0);
            break;
        case VAD_STATE_RECEND:
            hardvad_timeout = 0X0;
            vad_decode_timeflag =0x0;
            //timer_stop(TIMER0);
            break;
    }
#endif
    switch(flag)
    {
        case VAD_STATE_START:
//            LED_TEST_ON;
#if VAD_ONLINE                
        if(vad_cnt_timer == 0xffffffff)//first vad_start
        {
            vad_cnt_timer = C_VAD_END_MAX;
        //      LED_VAD_ON;
        }else
        {
            vad_end_timer = 0xffffffff;
        }
#else
        LED_VAD_ON;
    VAD_status = 1;
    set_VAD_flag_status(VAD_status);
#endif
        break;
        case VAD_STATE_VADEND:
        case VAD_STATE_DNNEND:
        case VAD_STATE_RECEND:
        case VAD_STATE_RSTEND:
#if VAD_ONLINE
            if(vad_cnt_timer!=0xffffffff)
            {//vad start ??????
                vad_end_timer = C_VAD_END_DELAY;
            }
            else
            {
                mprintf("no vad start,just end, not care========\r\n");
            }
#else                
            LED_VAD_OFF;
    VAD_status = 0;
    set_VAD_flag_status(VAD_status);
#endif                
            break;
    }
    static int dnn_start=0,dnn_end=0,asr_start=0,asr_end=0;
    switch (flag)
    {
        case DNN_STATE_START:
              if(!dnn_start)
              {
                  dnn_start = GetCurrentMs();
              }
              break;
        case VAD_STATE_DNNEND:
              dnn_end = GetCurrentMs(); 
              mprintf("DNN[%u:ms]\n",(dnn_end-dnn_start)*10);
              dnn_start =0;
              break;
        case ASR_DECODE_START:
            if(!asr_start)
            {
                asr_start = GetCurrentMs(); 
            }
          break;
        case VAD_STATE_RECEND:
        case VAD_STATE_RETEND:
            asr_end = GetCurrentMs(); 
            mprintf("DEC[%u:ms]\n",(asr_end - asr_start)*10);
            asr_start=0;
            dnn_start =0;
          break;
    }
}
#if USE_COMBO_CMD
/********************
Is_wakeword
组合识别之唤醒词判定
cailu
2018.1.10
***********************/
int  Is_COMBO_CMD(char* result)
{
    static int cnt = 0;
    int i=0;
    char words[120];
    char* pf;
    float sc=-10.0;
    static char g_last_result[50]={0};
    
    sscanf(result,"%s",words);
    pf = strstr(result, " ");
    sc = atof(pf+1);
    
    for(i=0;strcmp((const char*)combo_cmd_first_word_list[i],"END");i++)
    {
        if(!strcmp(words,(const char*)combo_cmd_first_word_list[i]))
        {
            if(sc>WAKEUP_SCORE_THRESHOLD)
            {
                if(strlen(g_last_result)!=0)
                {
                    if(!strcmp(g_last_result,words))
                    {
                        cnt++;
                        if(cnt>COMBO_CMD_FIRST_WORD_CNT)
                        {
                            combo_cmd_result.first_word_valid = COMBO_CMD_FIRST_WORD_VALID;
                            combo_cmd_result.first_word_valid_tick = xTaskGetTickCount();
                            
                            cnt=0;
                            memset(g_last_result, 0, sizeof(g_last_result));
                        }
                    }else
                    {
                        strcpy(g_last_result, words);
                        cnt=1;
                    }
                }else
                {
                    strcpy(g_last_result, words);
                    cnt++;
                }
            }
            return 1;
        }
    }
    return 0;
}
#else
int  Is_COMBO_CMD(char* result)
{
    return 0;
}
#endif
/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
