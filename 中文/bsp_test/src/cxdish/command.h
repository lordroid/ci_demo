/*------------------------------------------------------------------------------
  Copyright (C) 2013 Conexant Systems Inc.
  All rights reserved.

  CONEXANT SYSTEMS, INC. CONFIDENTIAL AND PROPRIETARY

  The information contained in this source code file
  is strictly confidential and proprietary to Conexant Systems, Inc.
  ("Conexant")

  No part of this file may be possessed, reproduced or distributed, in
  any form or by any means for any purpose, without the express written
  permission of Conexant Systems Inc.

  Except as otherwise specifically provided through an express agreement
  with Conexant that governs the confidentiality, possession, use
  and distribution of the information contained in this file, CONEXANT
  PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
  WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
  WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
  TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
  AND REPRESENTATIONS.  IN NO EVENT WILL CONEXANT BE LIABLE FOR ANY DAMAGES
  ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
  --------------------------------------------------------------------------------
  File Name: command.h
Description: The Command structure and related definitions
Created Jun 27, 2012
------------------------------------------------------------------------------*/

#ifndef COMMAND_H_
#define COMMAND_H_
#include "includes.h"

//typedef int int32_t;
//typedef unsigned int uint32_t;
//typedef unsigned char uint8_t;


#define COMMAND_OF_SIZE(n)   \
struct {                      \
	int32_t   num_32b_words:16;\
	uint32_t  command_id:15;   \
	uint32_t  reply:1;         \
	uint32_t  app_module_id;   \
	uint32_t  data[n] ;        \
}

// The maximum number of 32-bit data elements that a command can contain
#define MAX_COMMAND_SIZE 				13

#define CMD_REPLY 						1

#define CMD_SET(item)   				((item) & ~0x0100)
#define CMD_GET(item)   				((item) |  0x0100)
#define CMD_MASK        				(~(CMD_SET(0)|CMD_GET(0)))
#define CMD_ITEM(cmd)   				((cmd) & CMD_MASK)


#define APP_ID(a,b,c,d) 				((((a)-0x20)<<8)|(((b)-0x20)<<14)|(((c)-0x20)<<20)|(((d)-0x20)<<26))
#define CHAR_FROM_CAPE_ID_A(id)  		(((((unsigned int)(id))>>8) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_B(id)  		(((((unsigned int)(id))>>14) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_C(id)  		(((((unsigned int)(id))>>20) & 0x3f) + 0x20)
#define CHAR_FROM_CAPE_ID_D(id)  		(((((unsigned int)(id))>>26) & 0x3f) + 0x20)

#define CONTROL_APP_GET_VERSION     	CMD_GET(CONTROL_APP_VERSION)

// Retrieve the app and module id from an app_module_id
#define GET_APP_ID(app_module_id)    	((app_module_id)&~0xFF)
#define GET_MODULE_ID(app_module_id) 	((app_module_id)& 0xFF)

// Reserved App IDs
#define APP_ID_BROADCAST     			0xFFFFFF00 // to broadcast commands to all apps

// Reserved module IDs
#define MODULE_ID_APP        			0    // to send commands to the app
#define MODULE_ID_BROADCAST  			0xFF // to broadcast commands to all modules


// The Command type may be used to point to commands of arbitrary sizes, for example:
// COMMAND_OF_SIZE(5) cmd
// Command *ptr = (Command *)&cmd;
typedef COMMAND_OF_SIZE(MAX_COMMAND_SIZE) Command ;

#define ELBA_ADC0_GAIN	6
#define ELBA_ADC1_GAIN	7
#define ELBA_ADC0_BOOST	14
#define ELBA_ADC1_BOOST	15
/* control_ex.h */
typedef enum {
	MCLK_DISABLE						= 0,
	MCLK_FREQ_12_2880					= 2,
	CONTROL_APP_VERSION              	= 3,
	CONTROL_APP_EXEC_FILE            	= 4,
	MODULE_CMD_SET_ACTIVE				= 7,
	PAR_INDEX_OUTPUT_MCLK_FREQ			= 24,
	CONTROL_APP_FW_UPGD              	= 33,
	SPATIAL_SEP2_CMD_INTERNAL_STATE		= 42,
	SOS_RESOURCE                      	= 47,
	STREAMER_APP_CONFIG_ELBA			= 48,
	CHANNEL_MIXER_CMD_CONFIG			= 64,
	AEC_NCH_CMD_NLP_PARAM				= 66,
	CONTROL_MGR_TUNED_MODES          	= 85,
	SPATIAL_SEP2_CMD_MIC_SPACING		= 87,
	SPATIAL_SEP2_CMD_BEAM_FOCUS_ANGLE	= 0x58,
	SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH	= 0x59,
	CONTROL_MGR_PARAMETER_VALUE			= 90,
	MODULE_CMD_GET_INPUT_RATE           = 265,
	SPATIAL_SEP2_CMD_GET_INTERNAL_STATE	= 322,
} ControlAppCommandCode;

#define CAPT APP_ID('C','A','P','T')
#define COUT APP_ID('C','O','U','T')
#define CPTR APP_ID('C','P','T','R')
#define CTRL APP_ID('C','T','R','L')
#define SOS  APP_ID('S','O','S',' ')
#define STRM APP_ID('S','T','R','M')


#endif // COMMAND_H_
