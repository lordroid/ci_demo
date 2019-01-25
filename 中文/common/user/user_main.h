/**
  ******************************************************************************
  * @file    task_asr.c 
  * @author  Chipintelli Technology Co., Ltd.
  * @version V1.0.0
  * @date    2017.05
  * @brief  
  ******************************************************************************
  **/

#ifndef _USER_MAIN_H_
#define _USER_MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define key1evt 1
#define key2evt 2
#define key3evt 4

#define key12evt (key1evt|key2evt)
#define key13evt (key1evt|key3evt)
#define key23evt (key2evt|key3evt)

#define keyhold (1<<6)
#define keyrelease (1<<5)
#define keepholding (1<<4)

#define MP3GDOSet      GPIO_Output(GPIO0,GPIO_Pin4,1)
#define MP3GDOClr      GPIO_Output(GPIO0,GPIO_Pin4,0) 
#define MP3GDOInv      GPIO_DATA_Inv(GPIO0,GPIO_Pin4)
#define MP3CSNSet      GPIO_Output(GPIO1,GPIO_Pin7,1)
#define MP3CSNClr      GPIO_Output(GPIO1,GPIO_Pin7,0)
#define MP3TXSet       GPIO_Output(GPIO1,GPIO_Pin5,1) 
#define MP3TXClr       GPIO_Output(GPIO1,GPIO_Pin5,0)
#define MP3TXINV       GPIO_DATA_Inv(GPIO1,GPIO_Pin5)
#define MP3SS          GPIO_ReadDATA(GPIO1,GPIO_Pin6)

typedef enum
{
    MSG_TYPE_SYS_ASR = 0,
    MSG_TYPE_SYS_KEY,
    MSG_TYPE_SYS_COM,
}user_msg_type_t;

typedef enum
{
    MSG_ASR_STATUS_GOOD_RESULT = 0,
}user_asr_msg_status_t;

typedef struct
{
    uint32_t asr_status;
    uint32_t asr_index;
    float asr_score;
}sys_asr_msg_data_t;

typedef struct
{
    uint32_t key;
}sys_key_msg_data_t;

typedef struct
{
    unsigned char header0;//header
    unsigned char header1;
    unsigned char id;
    unsigned char cmd;
    unsigned char data0;
    unsigned char data1;
    unsigned char chksum;//check sum= add "header0~dta1"
    unsigned char end;	
}sys_com_msg_data_t;


typedef struct
{
    uint32_t msg_type;/*here will be modify use union*/
    union
    {
        sys_asr_msg_data_t asr_data;
        sys_key_msg_data_t key_data;
        sys_com_msg_data_t com_data;
    }msg_data;
}user_msg_t;

typedef struct
{
    int duty;
    int period;
}light;

typedef struct
{
    light r;
    light g;
    light b;
}color;
void cmd2mp3(unsigned char playcmd,uint16_t filenum);

void mp3init(void);
void key_to_exti(unsigned char exti_num,unsigned char swithmode);

void userapp_initial(void);

#ifdef __cplusplus
}
#endif

#endif
