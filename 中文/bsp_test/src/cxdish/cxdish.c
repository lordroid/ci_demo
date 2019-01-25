/*
 * Conexant CxDish
 *
 * Copyright:   (C) 2013 Conexant Systems
 *
 *      
 *************************************************************************
 *  Modified Date:  6/25/2015
 *  File Version:   1.0.0.6 
 *************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "CxFlash.h"
#include "common.h"
#include "ff.h"

#if CX_20921_UPDATA

#define CXDISH_VERSION_STR 		"1.0.0.6"
#define HAS_GPIO_RESET_PIN 		1
#define DEF_MAX_I2C_WRITE_LEN 	32 /*max = 0x1000*/
#define DEF_MAX_I2C_READ_LEN 		16
#define MAX_DEV_NAME 				256
#define MAX_ARGS  					32
#define I2C_CHIP_ADDRESS    		0x41
#define GPIO_RESET_PIN      		102
#define SENDCMD_MSG_BEGIN      	0x98
#define SENDCMD_MSG_END        	0x9c
#define SENDCMD_MSG_TYPE_OK    	0Xa9
#define SENDCMD_MSG_TYPE_ERROR 	0xa2
#define SENDCMD_MSG_DATA_MASK  	0xd0

typedef void *HANDLE;

int debugflag = 0;
extern unsigned int        g_cbMaxI2cWrite;
extern unsigned int        g_cbMaxI2cRead;
extern unsigned char       g_bChipAddress;                   /*Specify the i2c chip address*/

static int golem_intf   = 0;

static int  g_max_i2c_write = DEF_MAX_I2C_WRITE_LEN;
static int  g_max_i2c_read = DEF_MAX_I2C_READ_LEN;
static unsigned char  g_ChipAddress = I2C_CHIP_ADDRESS;
static int g_flash_legacy = 0;
static int g_hot_flash = 0;
HANDLE device = NULL;
Command cmd;
void *__gxx_personality_v0;

extern void   SetDeviceToDownLoad(HANDLE I2c_dev,unsigned long enable);
extern HANDLE OpenI2cDevice();
extern void   CloseI2cDevice(HANDLE hI2cDevice);
extern int i2c_write_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr,unsigned long write_len,unsigned char* write_buf);
extern int i2c_read_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr, unsigned long rd_len, unsigned char*rd_buf);

int cxdish_sendcmd(unsigned int argc, char *argv[])
{
	int err = 0;
	int          num_32b_words = argc -2;
	unsigned int app_module_id;
	unsigned int command_id;
	
	if (argc < 2) {
		ALOGD( "Specify a register address\n");
		return -EINVAL;
	}

	if (sscanf(argv[0], "%x", &app_module_id) != 1)
		return -EINVAL;

	if (sscanf(argv[1], "%x", &command_id) != 1)
		return -EINVAL;

	cmd.num_32b_words =  num_32b_words;
	cmd.command_id    = command_id;
	cmd.reply         = 0;
	cmd.app_module_id = app_module_id;

	for (int n = 0 ; n < num_32b_words ; n++)
	{
		if (sscanf(argv[n+2], "%x", &cmd.data[n]) != 1){
			return -EINVAL;
		}
	}

	int result = SendCmdV(&cmd) ; 
	if( result >= 0 )
	{
		if(golem_intf){   // send to golem tool.
			//send_to_golem(false,result,(unsigned long*)cmd.data);
		}
		else{
			// print out the result to screen
			ALOGD("=> ");
			for(int i=0;i<cmd.num_32b_words;i++)
				ALOGD("0x%08x ",cmd.data[i]);
			ALOGD("\n");
		}
	}
	else
	{
		// error occured
		if(golem_intf){   // send to golem tool.
			//send_to_golem(true,1,(unsigned long*)&result);
		}
		else{
			// print out the result to screen
			ALOGD("ERROR: failed to call sendcmd : err no=%d \n",result);
		}
	}
	return err;
}

#define IFLASH_ADDR	0x70580000
#define IFLASH_LEN	(20 * 1024)
#define SFS_ADDR (IFLASH_ADDR + IFLASH_LEN)
#define SFS_LEN		(400 * 1024)

#define USE_MALLOC
#define ONE_READ_SIZE	512
#if 1
FIL bootsave;
int cx2092x_upgrade(void)
{
	HANDLE            hDevice      = NULL;
	unsigned char    *pBootloader  =  NULL;
	unsigned char    *pImage       =  NULL;
	unsigned long     cbBootloader = 0;
	unsigned long     cbImage      = 0;
	int               ErrNo        = 0;
	void             *buf          = NULL;
	FATFS fs;
	FRESULT res;
	FIL fBootloader;
	FIL fImage;
	UINT br ;
	DIR DirInf;
	int read_len;
	int leave_len;
	int i;
	
	do{
		FRESULT result;
        
	    result = f_mount(FS_SD, &fs);
	    if (result != FR_OK)
	    {
	        mprintf("mount failed:");
			return -1;
	    }
		mprintf("mount OK!\n");
	    result = f_opendir(&DirInf, "/");
	    if (result != FR_OK)
	    {
	        mprintf("opendir failed:");
			return -1;
	    }
		/*---Attempt to open i2c device.---*/ 
		hDevice = OpenI2cDevice();
		if(hDevice == NULL ){
			// I2C device is not ready, return 1.
			ALOGD( "[Error]: I2c device is not present.\n");
			return -1;
		}
		//f_mount(0, &fs);
		/*res = f_open(&bootsave, "test.bin", FA_WRITE);
		if(res){
			ALOGD("open test.bin fail!\n");
			return -1;
		}*/
		res = f_open(&fBootloader, "iflash.bin", FA_READ);
		if (res) {
			ALOGD("failed to open iflash.bin!\n");
			return -1;
		};
		ALOGD("open iflash.bin ok!\n");
		res = f_open(&fImage, "20921firmware.sfs", FA_READ);
		if (res) {
			return -1;
		};
		ALOGD("open 20921firmware.sfs ok!\n");
		/*---Load BootLoader from file to memory.---*/
		// store the file size for later use.
		cbBootloader = fBootloader.fsize;
		leave_len = cbBootloader;
		//cbBootloader = 100;
		ALOGD("cbBootloader = %d\n", cbBootloader);
		// allocate the memory to store the bootlaod. 
		#ifdef USE_MALLOC
		pBootloader = (unsigned char*) malloc((cbBootloader/512+1)*512);
		if(NULL == pBootloader){
			ALOGD("no memory!\n");
			return -2;
		}
		#else
		pBootloader = (unsigned char *)IFLASH_ADDR;
		#endif
		i = 0;
		while(1){
			read_len = leave_len>ONE_READ_SIZE?ONE_READ_SIZE:leave_len;
			if(0 == read_len){
				break;
			}
			res = f_read(&fBootloader, pBootloader + cbBootloader - leave_len, read_len, &br);
			//f_write(&bootsave, pBootloader + cbBootloader - leave_len, read_len, &bw);
			//ALOGD("read bootloader %d %d\n", i++, read_len);
			if (res || br == 0){
				#ifdef USE_MALLOC
				if(pBootloader){
					free(pBootloader);
				}
				#endif
				ALOGD("read bootloader error!\n");
				f_close(&fBootloader);
				return -2;
			}
			leave_len -= read_len;
		}
		//f_close(&bootsave);
		f_close(&fBootloader);
		// save the file size for later use.
		cbImage = fImage.fsize;
		leave_len = cbImage;
		//cbImage = 100;
		ALOGD("cbImage = %d\n", cbImage);
		// allocate the memory to store the bootlaod.
		#ifdef USE_MALLOC
		pImage = (unsigned char*) malloc((cbImage/512+1)*512);
		if(NULL == pImage){
			ALOGD("no memory!\n");
			free(pBootloader);
			return -2;
		}
		#else
		pImage = (unsigned char *)SFS_ADDR;
		#endif
		i = 0;
		while(1){
			read_len = leave_len>ONE_READ_SIZE?ONE_READ_SIZE:leave_len;
			if(0 == read_len){
				break;
			}
			res = f_read(&fImage, pImage + cbImage - leave_len, read_len, &br);
			//ALOGD("read sfs %d %d\n", i++, read_len);
			if(1 == i)
				ALOGD("read sfs data: %c %c %c %c\n", pImage[0], pImage[1], pImage[2], pImage[3]);
			if (res || br == 0){
				#ifdef USE_MALLOC
				if(pImage){
					free(pImage);
				}
				#endif
				ALOGD("read sfs error!\n");
				f_close(&fImage);
				return -2;
			}
			leave_len -= read_len;
		}
		f_close(&fImage);
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) hDevice, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) hDevice, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

/*---setup the reset pin callback function.---*/
#ifdef  HAS_GPIO_RESET_PIN
		SetupSetResetPin((void *) hDevice,(fn_SetResetPin) SetDeviceToDownLoad );
#endif
		buf = malloc(GetSizeOfBuffer());

		/*---Download FW.---*/
		//If the operation completes successfull,the return value is zero. Otherwise,
		//return EERON_* error code. For more information about return code, please refer
		//to cxpump.h file.
		ErrNo = DownloadFW(buf, pBootloader, cbBootloader, pImage, cbImage, g_ChipAddress, SFS_UPDATE_AUTO, g_hot_flash ? 0 : 1,g_flash_legacy);
		if(ErrNo){
			ALOGD("\nFailed! ERROR CODE = %d\n\n",ErrNo);
		}
		else{
			ALOGD("Firmware Downloaded Successfully\n"); 
		}
	}while(0);

	/*---Clean up.---*/
	if(pImage){
		#ifdef USE_MALLOC
		free(pImage);
		#endif
		pImage = NULL;
	}
	if(pBootloader){
		#ifdef USE_MALLOC
		free(pBootloader);
		#endif
		pBootloader = NULL;
	}
	CloseI2cDevice(hDevice);
	if(buf) 
		free(buf);
	return ErrNo;
}
#endif
int getfwver(int *ver)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(CONTROL_APP_VERSION);
		cmd.reply         = 0;
		cmd.app_module_id = APP_ID ( 'C', 'T', 'R', 'L');

		if( SendCmdV(&cmd) > 0){
			int i = 0;
			for(i = 0; i < 4; i++){
                ver[i] = cmd.data[i];
			}
			ALOGD("Firmware Version: %d.%d.%d.%d\n",cmd.data[0],cmd.data[1],cmd.data[2],cmd.data[3] );
		}
		else{
			ALOGD("ERROR: Failed to get firmware version!\n");
			ret_val =2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = 1;
	}
	return ret_val;
}

int get_mode()
{
	int ret ;
	char data[5];
	unsigned char index;
	HANDLE dev = OpenI2cDevice();

	if(dev == NULL){
		ALOGD("ERROR: Failed to open I2C device!\n");
		return -1;
	}

	SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
	SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

	cmd.command_id = CMD_GET(SOS_RESOURCE);
	cmd.reply = 0;
	cmd.app_module_id = APP_ID ( 'S', 'O', 'S', ' ');
	cmd.num_32b_words = 1;
	cmd.data[0] = APP_ID ( 'C', 'T', 'R', 'L');
	ret = SendCmdV(&cmd);
	if( ret <= 0 || ret > MAX_COMMAND_SIZE ) 
		ALOGD("Failed to get tv mode, sendcmd error = %d\n",ret);
	else {
		data[0]= CHAR_FROM_CAPE_ID_A(cmd.data[0]);
		data[1] = CHAR_FROM_CAPE_ID_B(cmd.data[0]);
		data[2] = CHAR_FROM_CAPE_ID_C(cmd.data[0]);
		data[3] = CHAR_FROM_CAPE_ID_D(cmd.data[0]);
		index   = (unsigned char)(cmd.data[0]&0xff);
		if( index == 0){
			ALOGD("Current tv mode = \"%c%c%c%c\" \n",	data[0],data[1],data[2],data[3]);
		}
		else{
			ALOGD("Current tv mode = \"%c%c%c%c|%d\" \n",data[0],data[1],data[2],data[3],index);
		}
		ret = 0;
	}

	CloseI2cDevice(dev);
	return ret;
}

int set_mode(char *argv)
{
	int ret;
	int len;
	unsigned int index = 0;
	HANDLE dev ;

	len = strlen(argv);

	if( len < 4 || len == 5) {
		ALOGD( "ERROR: set-mode failed, mode id is not correct\n");
		return -EINVAL;
	}

	if( len >5 && argv[4]!='|'){
		ALOGD( "ERROR: set-mode failed, mode id is not correct\n");
		return -EINVAL;
	}

	if ( len >5  ){
		sscanf(&(argv[5]),"%d",&index );
	}

	dev = OpenI2cDevice();
	if(dev == NULL){
		ALOGD("ERROR: Failed to open I2C device!\n");
		return -1;
	}

	SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
	SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

	cmd.command_id = CMD_SET(CONTROL_APP_EXEC_FILE);//4;/*CMD_SET(CONTROL_APP_EXEC_FILE)*/
	cmd.reply = 0;
	cmd.app_module_id = APP_ID ( 'C', 'T', 'R', 'L');
	cmd.num_32b_words = 1;
	cmd.data[0] = APP_ID(argv[0], argv[1], argv[2], argv[3]) + (index&0xff);
	mprintf("start set mode !\n");
	ret = SendCmdV(&cmd);
	if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
		ALOGD( "ERROR: failed to set mode %s, sendcmd error = %d \n",argv,ret );
		ret = -1;
	} else {
		ALOGD( "set mode to \"%s\"\n",argv );
		ret = 0 ;
	}
	mprintf("end set mode !\n");
	CloseI2cDevice(dev);
	return ret;
}

void cxdish_init(void)
{
	g_bChipAddress = g_ChipAddress;
	g_cbMaxI2cWrite = g_max_i2c_write;
	g_cbMaxI2cRead = g_max_i2c_read;
}

int get_mic_boost(int index)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(STREAMER_APP_CONFIG_ELBA);
		cmd.reply         = 0;
		cmd.app_module_id = STRM;
		cmd.num_32b_words = 1;
		cmd.data[0] = index?ELBA_ADC0_BOOST:ELBA_ADC1_BOOST;

		if( SendCmdV(&cmd) > 0){
			ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret_val = cmd.data[0];
		}
		else{
			ALOGD("ERROR: Failed to get MicBoost!\n");
			ret_val = -2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = -1;
	}
	return ret_val;
}

int set_mic_space(int space)
{
    int ret;
	HANDLE dev = OpenI2cDevice();
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(SPATIAL_SEP2_CMD_MIC_SPACING);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 1;
		cmd.data[0] = space;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set MicBoost!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

int get_mic_space(void)
{
    int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(SPATIAL_SEP2_CMD_MIC_SPACING);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			ALOGD("Micspace: %d\n", cmd.data[0]);
			ret_val = cmd.data[0];
		}
		else{
			ALOGD("ERROR: Failed to get MicSpace!\n");
			ret_val = -2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = -1;
	}
	return ret_val;
}

int set_mic_boost(int index, int value)
{
	int ret;
	HANDLE dev = OpenI2cDevice();
	ALOGD("%s %d %d\n", __func__, index, value);
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(STREAMER_APP_CONFIG_ELBA);
		cmd.reply         = 0;
		cmd.app_module_id = STRM;
		cmd.num_32b_words = 2;
		cmd.data[0] = index?ELBA_ADC0_BOOST:ELBA_ADC1_BOOST;
		cmd.data[1] = value;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set MicBoost!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

static float float_from_q8_23_t(int val)
{
    float flt = 1.0/(1<<23);
	return flt * val;
}

static float float_from_q31_t(int val)
{
    float flt = 0.5/(1<<30);
	return flt * val;
}

int get_direction(int *dir, int *prob)
{
    //char *command[] = {"0xD308631E", "0x00000142", "0x0"};
    int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  0x00000142;
		cmd.reply         = 0;
		cmd.app_module_id = 0xD308631E;
		cmd.num_32b_words = 1;
		cmd.data[0] = 0;

		if( SendCmdV(&cmd) > 0){
			mprintf("dir: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", cmd.data[0], cmd.data[1], cmd.data[2], cmd.data[3], cmd.data[4], cmd.data[5], cmd.data[6]);
            *dir = (int)(float_from_q8_23_t(cmd.data[2]));
			*prob = (int)(float_from_q31_t(cmd.data[4])*100.0);
			mprintf("dir : %d, prob: %d\n", *dir, *prob);
		}
		else{
			ALOGD("ERROR: Failed to get direction!\n");
			ret_val =2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = 1;
	}
	return ret_val;
}

int get_foucus_angle(void)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(SPATIAL_SEP2_CMD_BEAM_FOCUS_ANGLE);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			ALOGD("Focus Angle: %d\n", cmd.data[0]);
			ret_val = cmd.data[0];
		}
		else{
			ALOGD("ERROR: Failed to get focus angle!\n");
			ret_val = -2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = -1;
	}
	return ret_val;
}

int set_foucus_angle(int angle)
{
    int ret;
	HANDLE dev = OpenI2cDevice();
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(SPATIAL_SEP2_CMD_BEAM_FOCUS_ANGLE);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 1;
		cmd.data[0] = angle;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set MicBoost!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

int get_foucus_half_width(void)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			ALOGD("Focus half width: %d\n", cmd.data[0]);
			ret_val = cmd.data[0];
		}
		else{
			ALOGD("ERROR: Failed to get focus half width!\n");
			ret_val = -2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = -1;
	}
	return ret_val;

}

int set_foucus_half_width(int value)
{
    int ret;
	HANDLE dev = OpenI2cDevice();
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(SPATIAL_SEP2_CMD_HALF_BEAM_WIDTH);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 1;
		cmd.data[0] = value;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set MicBoost!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

int get_enable_ssp(void)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(MODULE_CMD_SET_ACTIVE);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			ALOGD("get_enable_ssp: %d\n", cmd.data[0]);
			ret_val = cmd.data[0];
		}
		else{
			ALOGD("ERROR: Failed to get_enable_ssp!\n");
			ret_val = -2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = -1;
	}
	return ret_val;
}

int set_enable_ssp(int enable)
{
	int ret;
	HANDLE dev = OpenI2cDevice();
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(MODULE_CMD_SET_ACTIVE);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 30;
		cmd.num_32b_words = 1;
		cmd.data[0] = enable?1:0;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set MicBoost!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

static int mach_read_cmd(char *APP, int cmdid, int value, int cmd_ch)
{
	int ret_val = 0;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  cmdid;
		cmd.reply         = 0;
		cmd.app_module_id = APP_ID(APP[0], APP[1], APP[2], APP[3]) | cmd_ch;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			mprintf("mach_read_cmd read: %d, cmd_ch = %d\n", cmd.data[0], cmd_ch);
			if(cmd.data[0] == value){
				ret_val = 0;
			}else{
				ret_val = -1;
			}
		}
		else{
			//ALOGD("ERROR: Failed to get AEC_Enable!\n");
			ret_val =2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = 1;
	}
	return ret_val;
}

int testcmd(char *APP, int value, int cmdid, int cnt)
{
	int i = 0;

	mprintf("test APP:%s, cmdid:%d, value:%d\n", APP, cmdid, value);
	for(i = 0; i < cnt; i++){
		if(!mach_read_cmd(APP, cmdid, value, i)){
             mprintf("find moduleid: %d\n", i);
		}
	}
	return 0;
}

int setcmd(char *APP, int cmdid, int cmd_ch, int value)
{
	mprintf("set APP:%s, cmdid:%d, cmd_ch:%d, value:%d\n", APP, cmdid, cmd_ch, value);
	int ret;
	HANDLE dev = OpenI2cDevice(); 
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(cmdid);
		cmd.reply         = 0;
		cmd.app_module_id = APP_ID(APP[0], APP[1], APP[2], APP[3]) | cmd_ch;
		cmd.num_32b_words = 1;
		cmd.data[0] = value;

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			ALOGD("set cmd fail!\n");
			ret = -1;
		}
		else{
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

int set_output_type(int left_type, int right_type)
{
	int ret;
	int i = 0;
	ALOGD("%s %d %d", __func__, left_type, right_type);
	HANDLE dev = OpenI2cDevice();
	const int parm[8] = {0x000000c0, 0x000000c0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
	const int out_type_or[] = {
		0x000,	//processed
		0x100,	//mic left
		0x200,	//ref left
		0x300,	//eq left
		0x101,	//mic right
		0x201,	//ref right
		0x301	//eq right
	};
	if(left_type > 6 || right_type > 6){
		ALOGD("ERROR: not support output type");
		return -1;
	}
	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_SET(CHANNEL_MIXER_CMD_CONFIG);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 44;
		cmd.num_32b_words = 8;
		for(i = 0; i < 8; i++){
			cmd.data[i] = parm[i];
		}
		cmd.data[0] |= out_type_or[left_type];
		cmd.data[1] |= out_type_or[right_type];

		ret = SendCmdV(&cmd);
		if( ret < 0 || ret > MAX_COMMAND_SIZE ) {
			//ALOGD("Mic%dBoost: %d\n",index, cmd.data[0]);
			ret = -1;
		}
		else{
			//ALOGD("ERROR: Failed to set_output_type!\n");
			ret =0;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret = 1;
	}
	return ret;
}

int get_output_type(int *left_type, int *right_type)
{
	int ret_val;
	int i;
	HANDLE dev = OpenI2cDevice();
	const int out_type[] = {
		0x0c0,	//processed
		0x1c0,	//mic left
		0x2c0,	//ref left
		0x3c0,	//eq left
		0x1c1,	//mic right
		0x2c1,	//ref right
		0x3c1	//eq right
	};

	if(dev){
		/*---setup the i2c callback functions.---*/
		SetupI2cWriteMemCallback( (void *) dev, (fn_I2cWriteMem) i2c_write_imp,g_max_i2c_write);
		SetupI2cReadMemCallback( (void *) dev, (fn_I2cReadMem) i2c_read_imp, g_max_i2c_read);

		cmd.command_id    =  CMD_GET(CHANNEL_MIXER_CMD_CONFIG);
		cmd.reply         = 0;
		cmd.app_module_id = CAPT | 44;
		cmd.num_32b_words = 0;

		if( SendCmdV(&cmd) > 0){
			ALOGD("Output type: %x, %x\n", cmd.data[0], cmd.data[1]);
			for(i = 0; i < 7; i++){
				if(cmd.data[0] == out_type[i]){
					*left_type = i;
				}
				if(cmd.data[1] == out_type[i]){
					*right_type = i;
				}
			}
		}
		else{
			ALOGD("ERROR: Failed to get Output type!\n");
			ret_val =2;
		}
		CloseI2cDevice(dev);
	}
	else{
		ALOGD("ERROR: Failed to open I2C device!\n");
		ret_val = 1;
	}
	return ret_val;
}

#endif