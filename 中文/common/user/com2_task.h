#ifndef _COM_TASK_
#define _COM_TASK_

//--defined 
#define UART_HEADER0    0xA5
#define UART_HEADER1    0xFA
#define UART_END        0xFB
#if USER_UART_USE_UART1
    #define UART2_BAUDRATE_DEFAULT UART_BaudRate9600
#else
    #define UART2_BAUDRATE_DEFAULT B9600
#endif
//--define use by rx irq
#define SOP_STATE1      0x00
#define SOP_STATE2      0x01
#define DATA_STATE     0x02

//--CI send cmd.
typedef enum
{
    CI_TX_START =   0x80, //start   
	CI_TX_ASR	    ,  
    CI_TX_UNWAKEN	    , 
    CI_TX_VER,
    CI_TX_REV,
    CI_TX_END       ,      //--end
}CMD_CI_SEND;
//--CI recive cmd.
typedef enum
{
    CI_RX_START	    =   0,//--start
	CI_RX_ECHO_EN	    ,
    CI_RX_CHK_EN        ,
    CI_RX_PLAY_INDEX    ,
    CI_RX_RESET    ,
    CI_RX_UNWAKE_CMD    ,
    CI_RX_MUTE,
    CI_RX_VOLSET ,

    CI_RX_GETVER,
    CI_RX_REV,
    CI_RX_ENTER_AWAKE,
    CI_RX_END   ,//--end
}CMD_CI_RECIVED;

////////////////////////////////////
typedef struct{
  //  char Baudrat;//Ŀǰ��baudrate  1:9600  2:14400 3:19200 4:56000 5:38400 6:57600 7:115200 ������ֵ��115200
    char echo;//�Ƿ�ظ��յ������� 
    char vol_set;//��λ��֪ͨ��������
    char chk_enable;//ʹ��У��
    char play_by_uart; //�Ƿ��Ǵ��ڷ��͵Ĳ���, 1�Ļ����ֱ�Ӳ���
}UART_USER_SETTING_Typedef;

extern UART_USER_SETTING_Typedef gs_uar2_user;
#endif