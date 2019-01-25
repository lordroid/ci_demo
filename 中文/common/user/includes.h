/*
*********************************************************************************************************
*
*	模块名称 : 头文件汇总
*	文件名称 : includes.h
*	版    本 : V1.0
*	说    明 : 当前使用头文件汇总
*
*	修改记录 :
*		版本号    日期        作者    			 说明
*		V1.0    2016-08-22  chipintelli软件团队   首次发布
*
*	版权归chipintelli公司所有，未经允许不得使用或修改
*
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include <string.h>
#include <stdint.h>

/*
*********************************************************************************************************
*                                         其它库
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           OS
*********************************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"
/*
*********************************************************************************************************
*                                           Fatfs
*********************************************************************************************************
*/
#include "ff.h"

/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        Driver
*********************************************************************************************************
*/
#include "ci100x_adc.h"
#include "ci100x_xpwm.h"
#include "ci100x_suart.h"
#include "ci100x_can.h"
#include "ci100x_wdt.h"
#include "ci100x_dma.h"
#include "ci100x_uart.h"
#include "ci100x_gpio.h"
#include "ci100x_timer.h"
#include "ci100x_i2c.h"
#include "ci100x_iisdma.h"
#include "ci100x_TimerWdt.h"
#include "ci100x_uart.h"
#include "ci100x_sdram.h"
#include "ci100x_asr.h"
#include "ci100x_spiflash.h"
#include "ci100x_scu.h"
#include "ci100x_asrcmd.h"
#include "ci100x_sd.h"

/*
*********************************************************************************************************
*                                        APP / BSP
*********************************************************************************************************
*/
#include "smt_uart.h"
#include "asr_api.h"
#include "smt_iis.h"
#include "smt_cs42l52.h"
#include "ci100x_global.h"    
#include "smt_spiflash.h"
#include "asr_api.h"
#include "smt_gpio.h"
#include "ci100x_malloc.h"
#include "smt.h"
#include "user_config.h"

/* 在主函数中调用 */
extern AsrCmd_TypeDef AsrCmd_Struct[]; 
extern void vSetupTaskInfoTest(void);
void vTaskASRdecode(void*p);
void vTask_ResetASR(void *p);
extern void os_printf(char* fmt,...);

int FindCmdIndex(char* input,double score);
extern void MainCallback(void *p);
extern unsigned int GetCurrentMs(void);
extern unsigned int OS_GetCurrentMs(int show_flag);
int get_fw_version(void);

//---nvdata  
extern void nvdata_init();
extern int nvdat_read(char* buf,unsigned char lenth);
extern void nvdata_write(char* buf,unsigned char lenth);
extern void vol_set(char vol);
extern void DelayMs(int ms);
////////////////////////////////////////////
#endif /*__INCLUDES_H__*/

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/ 