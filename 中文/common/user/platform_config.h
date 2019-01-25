#ifndef _PLATFORM_CONFIG_H
#define _PLATFORM_CONFIG_H
//------------------SDRAM CONFIG
#define  SDRAM_HEAP_ADDR 0x70000000
#define  SDRAM_HEAP_SIZE 0x300000

#define  SDRAM_VOICE_PLAY_ADDR 0x70500000       //adpcm play buffer
#define  SDRAM_VOICE_PLAY_SIZE 0x120000

#define  SDRAM_REC_BUFFER_ADDR 0x70620000
#define  SDRAM_REC_BUFFER_SIZE 0x40000
                
#define  SDRAM_WAKECMD_ADDR 0x70660000
#define  SDRAM_WAKECMD_SIZE 0x40000    

#define  SDRAM_ASRCMD_ADDR 0x706A0000
#define  SDRAM_ASRCMD_SIZE 0x40000

#define  SDRAM_DNN_ADDR 0x70700000
#define  SDRAM_DNN_SIZE 1024*1024*9
#define SDRAM_DNN_BAK 0x70400000
//---------------0x70300000~0x70500000 can be used by user
#define NVDATA_BUF_ADDR 0x70300000
#define NVDATA_BUF_SIZE 0x10000

//----spi nvdata save
#define SPI_FLASH_SIZE              (8*1024*1024)
#define SPI_NVDATA_SIZE    		    0x10000
#define SPI_NVDATA_START_ADDR    	(SPI_FLASH_SIZE-SPI_NVDATA_SIZE)

//==================SDK VER============================
#define SDK_VERSION_MAIN_NO     1
#define SDK_VERSION_SUB_NO      5
#define SDK_VERSION_SUB2_NO     7
#define SDK_TYPE        "release"
//------------ customer Ver -----user define
#define USER_VERSION_MAIN_NO     1
#define USER_VERSION_SUB_NO      0
#define USER_TYPE        "CustomerAA"
//------------


#endif
