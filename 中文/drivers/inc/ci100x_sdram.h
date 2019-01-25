/**
  ******************************************************************************
  * @文件    ci100x_sdram.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的SDRAM控制器模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef _SDRAM_H_
#define _SDRAM_H_

#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"
#define SDRAM_CLK	200	//MHz

void sdram_init(void);
#ifdef __cplusplus
}
#endif
#endif  /*SDRAM_H END*/