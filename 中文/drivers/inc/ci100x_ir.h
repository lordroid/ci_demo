/**
  ******************************************************************************
  * @文件    ci100x_ir.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的红外发送接收模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
#ifndef __IR_H_
#define __IR_H_

#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"

void IR_Init(void);
void IR_Cmd(unsigned int state);
void IR_SendData(unsigned char data0,unsigned char data1,unsigned char data2);
unsigned char IR_ReceiveData(void);

#ifdef __cplusplus
}
#endif

#endif