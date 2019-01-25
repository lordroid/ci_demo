#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"

#include "ci100x_i2c.h"
#include "ci100x_scu.h"
#include "misc.h"
#include "ci100x_uart.h"
#define DEVICE_ADDR     (0x41)
extern unsigned int iic0_client_flags;
extern unsigned int iic1_client_flags;

#define COMMAND_OF_SIZE(n)   \
struct {                     \
  unsigned int  num_32b_words:16;\
  unsigned int  command_id:15;   \
  unsigned int  reply:1;         \
  unsigned int  app_module_id;   \
  int  data[n] ;        \
}

// The maximum number of 32-bit data elements that a command can contain
#define MAX_COMMAND_SIZE 13
typedef COMMAND_OF_SIZE(MAX_COMMAND_SIZE) Command ;
//
typedef enum 
{
	MODE_ZSH2  = 1,//4a8cfa00    MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-90°，dsptuned for SmartHome applications 
	MODE_ZSN2  = 2,//4aecfa00	 MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-30° DSP is tuned for natural language speech recognition.
	MODE_ZSN4  = 3,//52ecfa00	MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-30°DSP is tuned for natural language speech recognition.
	MODE_ZSW2  = 4,//4b7cfa00    MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-90° DSP is tuned for natural language speech recognition.


	MODE_SNN2  = 5,//  4aebb300   采集正负10度角内的声音，定向窄角集束采集

	MODE_ZSW4  = 6,//537cfa00  MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-90°DSP is tuned for natural language speech recognition.

//	ZSBW //MIC spacing is set to 55mm, SSP center is set to 0°;width is set to +/-90°  DSP is tuned for natural language speech recognition in Setup Box

	MODE_ZMP6  = 7,//5b0b7a00    工厂测试模式，
    MODE_MP16  = 8,// 591c2d00    mic输入和回音信号不做任何处理
    MODE_R6CH  = 9,//a235b200     使用户用usb口记录6通道数据，2路mic加4路回声     
// 6.22 版本以下模式设定会failed
	MODE_WAKE  = 10,
	MODE_ZRH2  = 11,
}VpaMode ;

#define REPLY_POLL_INTERVAL_MSEC  1
#define REPLY_POLL_TIMEOUT_MSEC   500

#define CMD_SET(item)   ((item) & ~0x0100)
#define CMD_GET(item)   ((item) |  0x0100)
#define CMD_MASK        (~(CMD_SET(0)|CMD_GET(0)))
#define CMD_ITEM(cmd)   ((cmd) & CMD_MASK)

// Code MODULE_CMD_ACTIVE - command data is one ModuleActive.
#define MODULE_CMD_ACTIVE                0x0007
#define MODULE_CMD_SET_ACTIVE            CMD_SET(MODULE_CMD_ACTIVE       )

#define ID(a,b,c,d)  ((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26))
#define CHAR_FROM_CAPE_ID_A(id)  (((((unsigned int)(id))>>8) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_B(id)  (((((unsigned int)(id))>>14) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_C(id)  (((((unsigned int)(id))>>20) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_D(id)  (((((unsigned int)(id))>>26) & 0x3f) + 0x20)


#define APP_ID_SOS  ID('S','O','S',' ')
#define APP_ID_CTRL ID('C','T','R','L')
#define APP_ID_CAPT ID('C','A','P','T')
#define COUT        ID('C','O','U','T')
#define CPIN        ID('C','P','I','N')
//--szk add20161028
#define APP_ID_STRM ID('S','T','R','M')
#define APP_ID_CAPT30 (APP_ID_CAPT|30) 


typedef enum {
		CONTROL_APP_VERSION               =  3,
		CONTROL_APP_EXEC_FILE             =  4,

		SOS_RESOURCE                      = 47,
		//--szkadd 20161028
		CONTROL_APP_FW_UPGD               = 33,
		CONTROL_MGR_TUNED_MODES           = 87,

		CONTROL_APP_ELBA  = 0x30,               //左右mic boost gain 
		SPATIAL_SEP2_CMD_MIC_SPACING  =87,       //mic 距离
		SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH = 0x59,//角度
        
        SPATIAL_SEP2_CMD_GET_INTERNAL_STATE  = 0x42,//获取DOA角度
        
}ControlAppCommandCode;

// Constants
#define AEC_NCH_CMD_SET_NLP_PARAM                          66
#define AMPLIFIER_CMD_VOLUME_LIST                          0x0040
#define CHANNEL_MIXER_CMD_CONFIG                           0x0040
#define CHANNEL_MIXER_CMD_SET_CONFIG                       64
#define CMD_PARAM                                          0x0200
#define CONTROL_APP_EXEC_FILE                              4
#define DRC_CMD_DYNAMICS                                   0x0042
#define DRC_CMD_GAIN_FUNC                                  0x0041
#define DRC_CMD_VOLUME_dB                                  0x0040
#define ELBA_ADC0_BOOST                                    14
#define ELBA_ADC0_GAIN                                     6
#define ELBA_ADC1_BOOST                                    15
#define ELBA_ADC1_GAIN                                     7

#define ELBA_ADC0_MUTE                                    0x0a
#define ELBA_ADC1_MUTE                                    0x0b

#define EQ_CMD_PARAM                                       0x0041
//#define MODULE_CMD_SET_ACTIVE                              7
#define SOS_SET_RESOURCE                                   47
#define SPATIAL_SEP2_CMD_BEAM_FOCUS_ANGLE                  0x0058
#define SPATIAL_SEP2_CMD_GAIN_CONTROLS                     0x0048
#define SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH                   0x0059
#define SPATIAL_SEP2_CMD_MIC_SPACING                       0x0057
#define STREAMER_APP_SET_CONFIG_ELBA                       48


VpaMode mode = MODE_WAKE;
                 
//#define NARGS(...) (sizeof((int[]){0,##__VA_ARGS__})/sizeof(int)-1)
//#define SENDCMD(cmd,app_module_id, command_id, ...) \
//     SendCmd(cmd,app_module_id, command_id, NARGS (__VA_ARGS__), ##__VA_ARGS__)
#define SENDCMD(cmd,app_module_id, command_id,...) \
     SendCmd(cmd,app_module_id, command_id,1, ##__VA_ARGS__)
#if CX20921_IIC1
static void i2c0_init(void){
  	NVIC_InitTypeDef NVIC_InitStruct={0};
    I2C_InitStruct InitStruct={0};
    
	NVIC_InitStruct.NVIC_IRQChannel	=IIC1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  	Scu_SetDeviceGate((unsigned int)IIC1,ENABLE);
	Scu_SetIOReuse(I2C1_SCL_PAD,FIRST_FUNCTION);
	Scu_SetIOReuse(I2C1_SDA_PAD,FIRST_FUNCTION);
    
    InitStruct.I2C_IO_BASE      = (unsigned int)IIC1;
    InitStruct.I2C_CLOCK_SPEED  = 100;      //100KHz
    InitStruct.I2C_INPUT_CLK    = 50000000; //50MHz
    InitStruct.TIMEOUT  = 10000;
    i2c_init((I2C_TypeDef *)IIC1,&InitStruct);
}
#else
static void i2c0_init(void){
  	NVIC_InitTypeDef NVIC_InitStruct={0};
    I2C_InitStruct InitStruct={0};
    
	NVIC_InitStruct.NVIC_IRQChannel	=IIC0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  	Scu_SetDeviceGate((unsigned int)IIC0,ENABLE);
	Scu_SetIOReuse(I2C0_SCL_PAD,FIRST_FUNCTION);
	Scu_SetIOReuse(I2C0_SDA_PAD,FIRST_FUNCTION);
    
    InitStruct.I2C_IO_BASE      = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED  = 100;      //100KHz
    InitStruct.I2C_INPUT_CLK    = 50000000; //50MHz
    InitStruct.TIMEOUT  = 10000;
    i2c_init((I2C_TypeDef *)IIC0,&InitStruct);
}
#endif

static int i2c_master_recv_16bit(I2C_TypeDef* I2Cx,struct i2c_client *client,char *buf ,int count)
{
    struct i2c_data *adap;
    struct i2c_msg msgs[2];
    int     ret32;
#if CX20921_IIC1
    adap = &i2c1_master;
    iic1_client_flags   = client->flags;
#else
    adap = &i2c0_master;
    iic0_client_flags   = client->flags;
#endif
	msgs[0].addr    = client->addr;
	msgs[0].flags   = I2C_M_TEN;
	msgs[0].len     = 2;
	msgs[0].buf     = (unsigned char*)buf;
    
	msgs[1].addr    = client->addr;
	msgs[1].flags   = I2C_M_RD | I2C_M_TEN;
	msgs[1].len     = count*4;
	msgs[1].buf     = (unsigned char*)buf;
	ret32 = i2c_master_transfer(adap,msgs,2);
    
    return (ret32 == 1) ? count : ret32;
}

int i2c_read(int address, unsigned char *buffer, int number)
{
	struct i2c_client client;
    
    i2c0_init();
    
    number  = (number > 50)?50:number;
    client.flags    = I2C_M_RD;
    client.addr     = DEVICE_ADDR;
    buffer[0]       = address>>8;
    buffer[1]       = address&0xff;

#if CX20921_IIC1	
    i2c_master_recv_16bit((I2C_TypeDef *)IIC1,&client,(char *)buffer,number);
#else
    i2c_master_recv_16bit((I2C_TypeDef *)IIC0,&client,(char *)buffer,number);
#endif
    //dma_dump_buffer("iic read data buff:\r\n",buffer,number*4,sizeof(char));
    return 0;
}

int i2c_write(int address, unsigned char *buffer, int number)
{
    struct i2c_client client;
    unsigned char   pbuf[50]={0};
    
    i2c0_init();
    number  = (number > 50)?50:number;
    client.flags    = 0;
    client.addr     = DEVICE_ADDR;
    pbuf[0]         = address>>8;
    pbuf[1]         = address&0xff;
    memcpy(&pbuf[2], buffer, number) ; // 4 bytes per word.
    //memcpy(&pbuf[2],buffer,number);
#if CX20921_IIC1	    
    number = i2c_master_send((I2C_TypeDef *)IIC1, &client, (const char *)pbuf, number + 2);
#else
    number = i2c_master_send((I2C_TypeDef *)IIC0, &client, (const char *)pbuf, number + 2);
#endif
    return 0;
}


static void msleep(int ms)
{
    volatile int i;
    while(ms--)
    {
        for(i=0;i<0xfff;i++);
    }
}
int para_num_32b_words=0;
int SendCmd(Command *cmd, int  app_module_id, int  command_id, int num_32b_words, ...)
//int SendCmd(Command *cmd, int  app_module_id, int  command_id, int num_32b_words)
{
    unsigned int *i2c_data;
    va_list     args;
    int         n,size;

    num_32b_words = para_num_32b_words;
    va_start (args , num_32b_words) ;
    if (num_32b_words > MAX_COMMAND_SIZE)
        return -1;
    cmd->num_32b_words = (command_id&CMD_GET(0)) ? MAX_COMMAND_SIZE:num_32b_words;
    cmd->command_id    = command_id;
    cmd->reply         = 0;
    cmd->app_module_id = app_module_id;
    for (n = 0 ; n < num_32b_words ; n++)
    {
        cmd->data[n] = va_arg(args, int);
    }
    va_end (args) ;
    //mprintf("=== app_module_id: %d, command_id: %d, data[0]: %d, num: %d\n", cmd->app_module_id, cmd->command_id, cmd->data[0], num_32b_words);
    i2c_data = (unsigned int *)cmd;
    size = num_32b_words + 2;
    // write words 1 to N-1 , to addresses 4 to 4+4*N-1
    i2c_write (0x4, (unsigned char *)&i2c_data[1], (size-1)*4);
    
    // write word 0 to address 0
    i2c_write (0x0, (unsigned char *)&i2c_data[0], 4);
    msleep(200);
    
    int elapsed_ms = 0;
    while (elapsed_ms < REPLY_POLL_TIMEOUT_MSEC)
    {
        i2c_read (0x0, (unsigned char *)&i2c_data[0], 1);
        if(cmd->reply==1)
            break;
        msleep(REPLY_POLL_INTERVAL_MSEC);
        elapsed_ms += REPLY_POLL_INTERVAL_MSEC;
    }
    if( cmd->reply == 1 )
    {
        if (cmd->num_32b_words > MAX_COMMAND_SIZE)
            goto error;
        if (cmd->num_32b_words > 0)
            i2c_read (0x8, (unsigned char *)&i2c_data[2], cmd->num_32b_words);
        return(cmd->num_32b_words);
    }
    return 0;
error:
    mprintf("i2c_read too large,%d\n",cmd->num_32b_words);
    return -1;
}

int switch_to_mode (VpaMode new_mode)
{
    Command cmd = {0};
    int id = ID('0','0','0','0');
    int ret_val =  0;
    //taskENTER_CRITICAL();
    //vTaskSuspendAll();
    if(new_mode == MODE_WAKE)
        id = ID('Z','T','R','G');
        //id = ID('Z','W','V','1');
    else if(new_mode == MODE_SNN2)
        id = ID('S','N','N','2');
    else if(new_mode == MODE_MP16)  //bypass mode
        id = ID('M','P','1','6');
    else if(new_mode == MODE_R6CH)
        id = ID('R','6','C','H');    
    else if(new_mode == MODE_ZRH2)
        id = ID('Z','R','H','2');
    else if(new_mode == MODE_ZSH2)
        id = ID('Z','S','H','2');
    else if(new_mode == MODE_ZSW2)
        id = ID('Z','S','W','2');
    else if(new_mode == MODE_ZSN2)
        id = ID('Z','S','N','2');
    else if(new_mode == MODE_ZSN4)
        id = ID('Z','S','N','4');
    else if(new_mode == MODE_ZSW4)
        id = ID('Z','S','W','4');
    else if(new_mode == MODE_ZMP6)
        id = ID('Z','M','P','6');
    else if(new_mode == MODE_ZMP6)
        id = ID('Z','M','P','6');


    para_num_32b_words = 1;
	//mprintf("app_module_id: %d, command_id: %d, data[0]: %d\n", APP_ID_CTRL, CMD_SET(CONTROL_APP_EXEC_FILE), id);
    ret_val = SENDCMD(&cmd, APP_ID_CTRL, CMD_SET(CONTROL_APP_EXEC_FILE), id);
    if (ret_val==0)
    {
        mode = new_mode;
        para_num_32b_words = 1;
        ret_val = SENDCMD(&cmd, APP_ID_SOS, CMD_GET(SOS_RESOURCE), APP_ID_CTRL);
        if ((ret_val==1)&&((int)(cmd.data[0])==id))
        {
            mprintf ("switched to mode: %d = %x\n",mode,cmd.data[0]);
            ret_val = 2;
        }
        else
            mprintf ("failed to confirm switching to mode: %d %x %x\n",ret_val, id, cmd.data[0]);
    }
    else
        mprintf ("command to switch to mode: %d failed\n",new_mode);
    //taskEXIT_CRITICAL();
    //xTaskResumeAll();
    return ret_val;
}

int get_fw_version(void)
{
    Command cmd = {0};
    int ret_val;
    
    para_num_32b_words = 0;    
    ret_val = SENDCMD(&cmd, APP_ID_CTRL, CMD_GET(CONTROL_APP_VERSION));
    if (ret_val<0)
        mprintf("failed to get FW version\r\n");
    else
        mprintf("version number: %d.%d.%d.%d\n",cmd.data[0],
    	cmd.data[1], cmd.data[2], cmd.data[3]);
    return 0;
}
static void aec_26(int enable)
{
    Command cmd = {0};
    int ret_val;
    
    if(enable)
    {
        para_num_32b_words = 1;
        ret_val = SENDCMD(&cmd, CPIN|26, MODULE_CMD_SET_ACTIVE , 1);
        mprintf("enable AEC_26 %s!!!\r\n",(ret_val < 0)?"error":"ok");
    }else{
        ret_val = SENDCMD(&cmd, CPIN|26, MODULE_CMD_SET_ACTIVE , 0);
        mprintf("disabla AEC_26 %s!!!\r\n",(ret_val < 0)?"error":"ok");
    }
}
static void aec_27(int enable)
{
    Command cmd = {0};
    int ret_val;
    
    if(enable)
    {
        para_num_32b_words = 1;
        ret_val = SENDCMD(&cmd, CPIN|27, MODULE_CMD_SET_ACTIVE , 1);
        mprintf("enable AEC_27 %s!!!\r\n",(ret_val < 0)?"error":"ok");
    }else{
        ret_val = SENDCMD(&cmd, CPIN|27, MODULE_CMD_SET_ACTIVE , 0);
        mprintf("disabla AEC_27 %s!!!\r\n",(ret_val < 0)?"error":"ok");
    }
}
static void shell_iic_test_help()
{
    mprintf( "Usage: aec [para]\n"
                "\tpara     : -v get fw version\n"
                "\t           -1 aec 26 enable\n"
                "\t           -2 aec 26 disable\n"
                "\t           -3 aec 27 enable\n"
                "\t           -4 aec 27 disable\n"
               );
    
    mprintf( "\t           -5 switch to wake mode\n"
                "\t           -6 switch to conf mode\n"
               );
}

int shellAPP_aec(int argc, char *argv[])
{       
    if(argc < 2)
        goto shell_iic_test_error;
    
    if( strncmp(argv[1],"-v",2) == 0 )
    {
        get_fw_version();
        return 0;
    }
    if( strncmp(argv[1],"-1",2) == 0 )
        aec_26(1);  //enable
    else if( strncmp(argv[1],"-2",2) == 0 )
        aec_26(0);  //disable
    else if( strncmp(argv[1],"-3",2) == 0 )
        aec_27(1);  //enable
    else if( strncmp(argv[1],"-4",2) == 0 )
        aec_27(0);  //disable
    else if( strncmp(argv[1],"-5",2) == 0 )
        switch_to_mode(MODE_WAKE);
    else if( strncmp(argv[1],"-6",2) == 0 )
        switch_to_mode(MODE_ZSN2);
    else
        goto shell_iic_test_error;
    return 0;
shell_iic_test_error:
    shell_iic_test_help();
    return -1;
}
void Set_Mic0_Boost(int boost_db)//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;;

      
    para_num_32b_words = 2;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_SET(CONTROL_APP_ELBA),ELBA_ADC0_BOOST,boost_db);

    para_num_32b_words = 1;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC0_BOOST);
    if(cmd.data[0]<0xff)
      mprintf ("get Mic0 boost gain : %ddb ret_val=%d\n",cmd.data[0],ret_val);
    else
      mprintf ("get Mic0 boost gain	err!!: %d\n",cmd.data[0]);
  /////////////////////////  	
     para_num_32b_words = 2;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_SET(CONTROL_APP_ELBA),ELBA_ADC1_BOOST,boost_db);

    para_num_32b_words = 1;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC1_BOOST);
    if(cmd.data[0]<0xff)
      mprintf ("get Mic1 boost gain : %ddb ret_val=%d\n",cmd.data[0],ret_val);
    else
      mprintf ("get Mic1 boost gain	err!!: %d\n",cmd.data[0]);
    	
    
}
 void Get_Mic0_Boost(void)//0~48.12db step
 {
	Command cmd = {0};
	int ret_val = 0;;
	
	para_num_32b_words = 1;
	 ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC0_BOOST);
	if(cmd.data[0]<0xff)
	 mprintf ("get Mic0 boost gain : %ddb ret_val=%d\n",cmd.data[0],ret_val);
	else
	 mprintf ("get Mic0 boost gain	err!!: %d\n",cmd.data[0]);
	 
       	para_num_32b_words = 1;
	 ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC1_BOOST);
	if(cmd.data[0]<0xff)
	 mprintf ("get Mic1 boost gain : %ddb ret_val=%d\n",cmd.data[0],ret_val);
	else
	 mprintf ("get Mic1 boost gain	err!!: %d\n",cmd.data[0]);
	 
 }

void Set_Mic_Spacing(int gc_mm)//0~48.12db step
{
  Command cmd = {0};
  int ret_val = 0;

  para_num_32b_words = 1;
  ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_SET(SPATIAL_SEP2_CMD_MIC_SPACING),gc_mm);
cmd.data[0] = 0xFF;
  para_num_32b_words = 1;
  ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_GET(SPATIAL_SEP2_CMD_MIC_SPACING));

  mprintf ("%s=  : %d mm ret_val=%d\n",__func__,cmd.data[0],ret_val);  	
}


void Get_Mic_Spacing(void)//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;

    para_num_32b_words = 1;
    ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_GET(SPATIAL_SEP2_CMD_MIC_SPACING));
 
    mprintf ("get Mic spacing = : %d mm ret_val=%d\n",cmd.data[0],ret_val);  	
}

void Set_Mic_AngleDomain(int tc_angle)
{
    Command cmd = {0};
    int ret_val = 0;

    para_num_32b_words = 1;
       ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_SET(SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH),tc_angle);
 
    mprintf ("%s = : %d  ret_val=%d\n",__func__,cmd.data[0],ret_val);  	
}
void Get_Mic_AngleDomain()//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;

    para_num_32b_words = 1;
       ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_GET(SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH));
 
    mprintf ("%s = : %d  ret_val=%d\n",__func__,cmd.data[0],ret_val);  	
}
//////////////////drc
void Set_DRC_en(int en)
{
  Command cmd = {0};
  int ret_val = 0;

  para_num_32b_words = 1;
  ret_val = SENDCMD(&cmd, APP_ID_CAPT|41, CMD_SET(MODULE_CMD_ACTIVE),en);
cmd.data[0] = 0xFF;

  para_num_32b_words = 1;
  ret_val = SENDCMD(&cmd, APP_ID_CAPT|41, CMD_GET(MODULE_CMD_ACTIVE));

  mprintf ("%s=  : %d  ret_val=%d\n",__func__,cmd.data[0],ret_val);  	
}


void Get_DRC_Sta(void)//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;

    para_num_32b_words = 1;
  ret_val = SENDCMD(&cmd, APP_ID_CAPT|41, CMD_GET(MODULE_CMD_ACTIVE));
 
    mprintf ("get drc enable = : %d mm ret_val=%d\n",cmd.data[0],ret_val);  	
}
/////////////////////////////

void Get_DOA()//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;

    para_num_32b_words = 1;
       ret_val = SENDCMD(&cmd, APP_ID_CAPT30, CMD_GET(SPATIAL_SEP2_CMD_GET_INTERNAL_STATE));
 
       mprintf ("%s = [0]: %x [1]: %x [2]: %x ret_val=%d\n",__func__,cmd.data[0],cmd.data[1],cmd.data[2],ret_val);  	
       //
       if(cmd.data[2]&0x80000000)
         mprintf ("-----------------------------------------------cal DOA  -%d,conver last: -%d 度\n",(cmd.data[2]&0x7FFFFFFF)/0x800000,360-((cmd.data[2]&0x7FFFFFFF)/0x800000)*90/64);  
       else 
         mprintf ("-----------------------------------------------cal DOA  +%d,conver last: +%d 度\n",(cmd.data[2]&0x7FFFFFFF)/0x800000,((cmd.data[2]&0x7FFFFFFF)/0x800000)*90/64);  
       
}
//-------20170907 add set adc mute szk
void Set_Mic_MUTE(int mute)//0~48.12db step
{
    Command cmd = {0};
    int ret_val = 0;;

    //////set adc0  
    para_num_32b_words = 2;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_SET(CONTROL_APP_ELBA),ELBA_ADC0_MUTE,mute);

    para_num_32b_words = 1;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC0_MUTE);
    if(cmd.data[0]<0xff)
      mprintf ("get Mic0 mute: %d ret_val=%d\n",cmd.data[0],ret_val);
    else
      mprintf ("get Mic0 mute:	err!!: %d\n",cmd.data[0]);
	//////set adc1
	    para_num_32b_words = 2;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_SET(CONTROL_APP_ELBA),ELBA_ADC1_MUTE,mute);

    para_num_32b_words = 1;
    ret_val = SENDCMD(&cmd, APP_ID_STRM, CMD_GET(CONTROL_APP_ELBA), ELBA_ADC1_MUTE);
    if(cmd.data[0]<0xff)
      mprintf ("get Mic1 mute: %d ret_val=%d\n",cmd.data[0],ret_val);
    else
      mprintf ("get Mic1 mute:	err!!: %d\n",cmd.data[0]);
	
    
}

