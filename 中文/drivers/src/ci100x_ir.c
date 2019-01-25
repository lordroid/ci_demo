/**
  ******************************************************************************
  * @文件    ci100x_ir.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的红外发送接收模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_ir.h"

/**
  * @功能:初始化红外模块
  * @注意:在使用前需调用SCU相关接口初始化对应管脚        
  * @参数:无
  * @返回值:无
  */
void IR_Init(void)
{
    SMT_IR->REM_CR = 0xc;
}

/**
  * @功能:使能或禁用红外模块
  * @注意:无       
  * @参数:state 0:禁用红外模块   1:使能红外模块
  * @返回值:无
  */
void IR_Cmd(unsigned int state)
{
    if(state==0)
    {
        SMT_IR->REM_CR &= ~(0x1 << 0);
    }
    else
    {
        SMT_IR->REM_CR |= (0x1 << 0);
    }
}
/**
  * @功能:配置红外模块发送单笔数据
  * @注意:无          
  * @参数:data0:8bit用户码data1:8bit用户码或反码，data2:8bit数据
  * @返回值:无
  */
void IR_SendData(unsigned char data0,unsigned char data1,unsigned char data2)
{
    SMT_IR->REM_TX_DATA = ((data0 & 0xff) << 16) | ((data1 & 0xff) << 8) | (data2 & 0xff);
}

/**
  * @功能:配置IR 接收 单笔数据
  * @注意:无          
  * @参数:无 
  * @返回值:接收到的数据
  */
unsigned char IR_ReceiveData(void)
{
    unsigned char data;
    data = (SMT_IR->REM_RX_DATA >> 7) & 0xff;
    return data;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/