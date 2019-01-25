/**
  ******************************************************************************
  * @文件    ci100x_can.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的can总线模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __CI100X_CAN_H
#define __CI100X_CAN_H
#ifdef __cplusplus
 extern "C" {
#endif
#define CAN_TEST_ENABLE     1       //CAN测试程序编译开关，0表示关闭，1表示打开

typedef struct
{
    unsigned char RM    :1;         //复位模式
    unsigned char LOM   :1;         //仅侦听模式
    unsigned char STM   :1;         //自测模式
    unsigned char AFM   :1;         //容纳滤波器模式
    unsigned char SM    :1;         //睡眠模式
    unsigned char reserved   :3;  
}BF_CAN_MOD_REG;
typedef union 
{
    unsigned char   ui;
    BF_CAN_MOD_REG  bf;
}CAN_MOD_REG;

typedef struct
{
    unsigned char RI    :1;         //接收中断
    unsigned char TI    :1;         //传输中断
    unsigned char EI    :1;         //错误警告中断
    unsigned char DOI   :1;         //数据超限中断
    unsigned char WUI   :1;         //唤醒中断
    unsigned char EPI   :1;         //被动错误中断
    unsigned char ALI   :1;         //仲裁丢失中断
    unsigned char BEI   :1;         //总线错误中断
}BF_CAN_IR_REG;
typedef union 
{
    unsigned char   ui;
    BF_CAN_IR_REG   bf;
}CAN_IR_REG;

typedef struct
{
    unsigned char RBS   :1;         //接收缓存状态
    unsigned char DOS   :1;         //数据超限状态
    unsigned char TBS   :1;         //传输缓存状态
    unsigned char TCS   :1;         //传输完成状态
    unsigned char RS    :1;         //接收状态
    unsigned char TS    :1;         //传输状态
    unsigned char ES    :1;         //错误状态
    unsigned char BS    :1;         //总线状态
}BF_CAN_SR_REG;
typedef union 
{
    unsigned char   ui;
    BF_CAN_SR_REG   bf;
}CAN_SR_REG;

typedef struct
{
    unsigned char TR    :1;         //传输请求
    unsigned char AT    :1;         //终止传输
    unsigned char RRB   :1;         //释放接收Buffer
    unsigned char CDO   :1;         //清除数据超限
    unsigned char SRR   :1;         //自接收请求   
    unsigned char reserved    :3;
}BF_CAN_CMR_REG;
typedef union 
{
    unsigned char   ui;
    BF_CAN_CMR_REG  bf;
}CAN_CMR_REG;

typedef struct
{
    unsigned char RIE   :1;         //Receive Interrupt Enable
    unsigned char TIE   :1;         //Transmit Interrupt Enable
    unsigned char EIE   :1;         //Error Warning Interrupt Enable
    unsigned char DOIE  :1;         //Data Overrun Interrupt Enable
    unsigned char WUIE  :1;         //Wake-Up Interrupt Enable
    unsigned char EPIE  :1;         //Error Passive Interrupt Enable
    unsigned char ALIE  :1;         //Arbitration Lost Interrupt Enable
    unsigned char BEIE  :1;         //Bus Error Interrupt Enable
}BF_CAN_IER_REG;
typedef union 
{
    unsigned char   ui;
    BF_CAN_IER_REG  bf;
}CAN_IER_REG;

typedef struct
{
    
    volatile CAN_MOD_REG     MOD;            //mode
    volatile CAN_CMR_REG     CMR;            //command
    volatile CAN_SR_REG      SR;             //status
    volatile CAN_IR_REG      IR;             //intrrupt
    volatile CAN_IER_REG     IER;            //intrrupt Enable
    volatile unsigned char   reserved0;
    volatile unsigned char   BTR0;           //bus timing 0
    volatile unsigned char   BTR1;           //bus timing 1
    volatile unsigned char   OCR;            //output control register
    volatile unsigned char   reserved1;
    volatile unsigned char   reserved2;
    volatile unsigned char   ALC;            //Arbitration Lost Capture
    volatile unsigned char   ECC;            //Error Code Capture
    volatile unsigned char   EWLR;           //Error Warning Limit
    volatile unsigned char   RXERR;          //Receive Error Counter
    volatile unsigned char   TXERR;          //Transmit Error Counter
    volatile unsigned char   ACR[4];         //Acceptance Code Registers 0 – 3
    volatile unsigned char   AMR[4];         //Acceptance Mask Registers 0 – 3
    volatile unsigned char   reserved3[6];
    volatile unsigned char   RMC;            //Receive Message Counter
    volatile unsigned char   RBSA;           //Receive Buffer Start Address
    volatile unsigned char   CDR;            //Clock Divider
    volatile unsigned char   RX_FIFO[64];    //Receive FIFO
    volatile unsigned char   TX_BUFF[13];    //Transmit Buffer
}CAN_TypeDef;

#define	CAN	             ((CAN_TypeDef *)CAN_BASE)
#define CAN_TX_BUFFER    (( unsigned char *)(CAN_BASE + 0x10))
#define CAN_RX_BUFFER    (( unsigned char *)(CAN_BASE + 0x10))
    
enum
{
    CAN_Frame_Format_Standard=0,
    CAN_Frame_Format_Extend,
    CAN_Frame_Format_UNDEFINED
};
enum
{
    CAN_Frame_Data=0,
    CAN_Frame_Remote,
    CAN_Frame_UNDEFINED
};
typedef struct
{
    unsigned char length   :4; 
    unsigned char reserved :2; 
    unsigned char rtr      :1;              //REMOTE TRANSMISSION REQUEST 
    unsigned char format   :1;              //FRAME FORMAT
}BF_CAN_FRAME;
typedef union 
{
    unsigned char   ui;
    BF_CAN_FRAME    bf;
}CAN_FRAME;
typedef struct
{
    CAN_FRAME  frame;
    unsigned char  identifier1;
    unsigned char  identifier2;
    unsigned char  data[10];
}CAN_SFF_WINDOW;
typedef struct
{
    CAN_FRAME  frame;
    unsigned char  identifier1;
    unsigned char  identifier2;
    unsigned char  identifier3;
    unsigned char  identifier4;
    unsigned char  data[8];
}CAN_EFF_WINDOW;
    
enum
{
    CAN_BAUT_5K = 0,
    CAN_BAUT_10K,
    CAN_BAUT_20K,
    CAN_BAUT_50K,
    CAN_BAUT_100K,
    CAN_BAUT_125K,
    CAN_BAUT_250K,
    CAN_BAUT_500K,
    CAN_BAUT_800K,
    CAN_BAUT_1000K,
    CAN_BAUT_UNDEFINED
};

enum
{    
    CAN_FILTER_DUAL=0,
    CAN_FILTER_SINGLE,
    CAN_FILTER_UNDEFINED
};

typedef struct
{
    unsigned int identifier;
    unsigned int id_mask;
    unsigned int bautrate;
    unsigned int filterMode;
}CAN_InitTypeDef;

typedef struct
{
    unsigned int mask;
    unsigned int identifier;
    unsigned int id_mask;
    unsigned int format;
    unsigned int frame;
}CAN_TransmitTypeDef;
typedef struct
{
    unsigned char type;
    unsigned char BRT0;
    unsigned char BRT1;
}CAN_BAUT_LISTS;



void can_init(CAN_InitTypeDef *initStruct);

int can_transmit_dataframe( unsigned char *addr, \
                            unsigned int data_length, \
                            CAN_TransmitTypeDef *infor);


#ifdef __cplusplus
}
#endif
#endif
