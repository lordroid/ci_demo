/**
  ******************************************************************************
  * @文件    ci100x_spiflash.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的spiflash驱动头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include "ci100x_dma.h"
#include "ci100x_global.h"
#ifdef __cplusplus
extern "C"{
#endif 

typedef struct{
	DMAC_FLOWCTRL flowctrl;
	BURSTSIZEx busrtsize;
	TRANSFERWIDTHx transferwidth;
  	unsigned int srcaddr;
	unsigned int destaddr;
	unsigned int bytesize;
}SPIDMAInit_Struct;

  
#ifndef SPIC0_BASE 
#define SPIC0_BASE 0x40040000
#endif

#ifndef SPI0FIFO_BASE 
#define SPI0FIFO_BASE 0x60000000
#endif

#include "smt.h"
#include "misc.h" 
  

typedef struct{
    volatile unsigned int SPIC_CSR_00;
    volatile unsigned int SPIC_CSR_01;
    volatile unsigned int SPIC_CSR_02;
    volatile unsigned int SPIC_CSR_03;
    volatile unsigned int SPIC_CSR_04;
    volatile unsigned int SPIC_CSR_05;
    volatile unsigned int SPIC_CSR_06;
    volatile unsigned int SPIC_CSR_07;
    volatile unsigned int SPIC_CSR_08;
    volatile unsigned int SPIC_CSR_09;
    volatile unsigned int SPIC_CSR_10;
    volatile unsigned int SPIC_CSR_11;
    volatile unsigned int SPIC_CSR_12;
    volatile unsigned int SPIC_CSR_13;
    volatile unsigned int SPIC_CSR_14;
    volatile unsigned int SPIC_CSR_15;
    volatile unsigned int SPIC_CSR_16;
}SPIC_TypeDef;   

#define SPIC0 ((SPIC_TypeDef* )SPIC0_BASE)

 
/*commandtype*/
#define Program 0x0
#define WriteStatReg 0x1
#define ReadStatReg 0x2
#define SectorErase 0x3
#define Block32Erase 0x4
#define Block64Erase 0x5
#define ChipErase 0x6
#define PowerDown 0x7
#define ReleasePowerDown 0x8
#define ReadSecurityReg 0xa
#define EraseSecurityReg 0xb
#define WriteSecurityReg 0xc
#define Read 0xd
#define ReadManufDevID 0xe
#define ReadJedecID 0XF
/*spi bus mode*/
#define STANDARDSPI 0X0
#define DUALSPI 0X1
#define QUADSPI 0X2

/*dma request tap*/
#define DmaReqTapWord 0x0
#define DmaReqTap4Word 0x1
#define DmaReqTap8Word 0x2
#define DmaReqTap16Word 0x3
#define DmaReqTap32Word 0x4
#define DmaReqTap64Word 0x5

/*FLASH CMD CODE HEX*/
#define RelePOWdownHPMDevID 0XAB
#define ManufDevID 0x90
#define UniqueID 0x4B
#define JEDEC_ID 0x9F
#define FlashEnableReset 0x66
#define FlashReset 0x99


#define ReadStatusReg1 0x05
#define ReadStatusReg2 0x35

#define WriteStatusReg 0x01

/*erase type */
#define BlockErase64 0XD8
#define BlockErase32 0x52
#define SectorErase4 0x20
#define ChipEraseAll 0xC7
/*program*/
#define QuadpageProgram 0x32
/*Quad read*/
#define FastReadQuadOutput 0x6B 

typedef struct {
    u8 SpiBusMode;
    u8 SpiClkMode;
    u8 FastReadIo;
    u8 FastReadEn;
    u8 CommandCode;
    u8 SpiDmaEn;
    u8 CommandType;
    u8 CommandValid;
    u8 DmaReqTap;
}SPIC_InitTypeDef;

typedef struct {
    u8 CommandCode;
    u8 SpiDmaEn;
    u8 CommandType;
    u8 CommandValid; 
}CMD_InitTypedef;



unsigned int ReadSpiflashID(SPIC_TypeDef* SPICx,unsigned int ReadIDcmd);
unsigned int ReadSpiflashStatusRegister(SPIC_TypeDef* SPICx,unsigned char ReadStatusReg);
void WriteSpiflashStatusReg(SPIC_TypeDef* SPICx,unsigned int RegVal);
void SpiflashErase(SPIC_TypeDef* SPICx,unsigned int EraseType,unsigned int Addr);
void SpiflashQuadProgram(SPIC_TypeDef* SPICx,unsigned int Addr,unsigned char* Data,unsigned int size);
void SpiflashQuadRead(SPIC_TypeDef* SPICx,unsigned int Addr,unsigned char* Data,unsigned int size);
void WaitCommand_vail(SPIC_TypeDef* SPICx);
void SetSpiflashMode(SPIC_TypeDef* SPICx,unsigned int RegVal);

void SetSPICCSR07IntEn(SPIC_TypeDef* SPICx,FunctionalState NewState);
void Spiflash_Init(SPIC_TypeDef* SPICx,SPIC_InitTypeDef* SPIC_InitStruct);
void RsetSpiflash(SPIC_TypeDef* SPICx);
void SPIC_SetFlashDefault(SPIC_TypeDef* SPICx);
 

typedef struct{
  	volatile unsigned int SPIC_CSR_00;
	volatile unsigned int SPIC_CSR_01;
	volatile unsigned int SPIC_CSR_02;
	volatile unsigned int SPIC_CSR_03;
	volatile unsigned int SPIC_CSR_04;
	volatile unsigned int SPIC_CSR_05;
	volatile unsigned int SPIC_CSR_06;
	volatile unsigned int SPIC_CSR_07;
	volatile unsigned int SPIC_CSR_08;
	volatile unsigned int SPIC_CSR_09;
	volatile unsigned int SPIC_CSR_10;
	volatile unsigned int SPIC_CSR_11;
	volatile unsigned int SPIC_CSR_12;
	volatile unsigned int SPIC_CSR_13;
	volatile unsigned int SPIC_CSR_14;
	volatile unsigned int SPIC_CSR_15;
	volatile unsigned int SPIC_CSR_16;
}SPICInit_Typedef;  
  
#define QSPIC ((SPICInit_Typedef*)SPIC0_BASE) 

typedef enum{
	Spi_BusMode_StdSpi	=0,
	Spi_BusMode_DualSpi	=1,
	Spi_BusMode_QuadSpi	=2,
}Spi_BusMode;	

typedef enum{
	Spi_ClkMode0=0,
	Spi_ClkMode3=1,
}Spi_ClkMode;
typedef enum{
	Fast_ReadIO_readoutput	=0,
	Fast_ReadIO_addr_data	=1,
}Fast_ReadIO;	

typedef enum{
	SpiFlash_CmdCode_ReadmanufacturerIDDeviceID			=0x90,
	SpiFlash_CmdCode_ReadJedecID						=0x9F,
	SpiFlash_CmdCode_ReadStatusReg1						=0x05,
	SpiFlash_CmdCode_ReadStatusReg2						=0x35,
	SpiFlash_CmdCode_SectorErase4k						=0x20,
	SpiFlash_CmdCode_BlockErase32k						=0x52,
	SpiFlash_CmdCode_BlockErase64k						=0xd8,
	SpiFlash_CmdCode_ChipErase							=0xc7,//or 0x60
	SpiFlash_CmdCode_PageProgram						=0x02,
	SpiFlash_CmdCode_QuadInputPageProgram				=0x32,
	SpiFlash_CmdCode_WrStatusReg						=0x01,
	SpiFlash_CmdCode_ReadData							=0x03,
	SpiFlash_CmdCode_FastRead							=0x0b,
	SpiFlash_CmdCode_FastReadDualOutput					=0x3b,
	SpiFlash_CmdCode_FastReadQuadOutput					=0x6b,
    SpiFlash_CmdCode_FastReadQuadIO                     =0XEB,
	SpiFlash_CmdCode_Powerdown							=0xb9,
	SpiFlash_CmdCode_ReleasePowerdown					=0xab,
	SpiFlash_CmdCode_EnableReset						=0x66,
	SpiFlash_CmdCode_Reset								=0x99,
}SpiFlash_CmdCode;

typedef enum{
	SpiFlash_CmdType_Program	 		=0,
	SpiFlash_CmdType_WrStatusReg 		=1,
	SpiFlash_CmdType_RdStatusReg 		=2,
	SpiFlash_CmdType_SectorErase4k		=3,
	SpiFlash_CmdType_BlockErase32k		=4,
	SpiFlash_CmdType_BlockErase64k		=5,
	SpiFlash_CmdType_ChipErase			=6,
	SpiFlash_CmdType_Powerdown			=7,
	SpiFlash_CmdType_Releasepowerdown	=8,
	SpiFlash_CmdType_ReadSecurityReg 	=10,
	SpiFlash_CmdType_EraseSecurityReg	=11,
	SpiFlash_CmdType_WrSecurityReg		=12,
	SpiFlash_CmdType_Read				=13,
	SpiFlash_CmdType_ReadManufacturerID	=14,
	SpiFlash_CmdType_ReadJedecID		=15,
}SpiFlash_CmdType;

typedef enum{
	DMARequestSize_1word	=0,
	DMARequestSize_4word	=1,
	DMARequestSize_8word	=2,
	DMARequestSize_16word	=3,  
	DMARequestSize_32word	=4,
	DMARequestSize_64word	=5,
}DMARequestSizeBytes;	

typedef enum{
	SpiFlash_SoftwareProtection		=0,
	SpiFlash_HardwareProtection		=1,
	SpiFlash_PowerSupplyLock_Down	=2,
	SpiFlash_OneTimeProgram			=3,
	SpiFlash_Resv					=-1,
}SpiFlash_StatusProtect;	
	
void SpiFlash_ReadManfacturerIDDeviceID(SPICInit_Typedef* QSPICx,unsigned char* manid,unsigned char* deviceid);
void SpiFlash_ReadJedecID(SPICInit_Typedef* QSPICx,unsigned char* jedecid);
char SpiFlash_ReadStatusReg(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode read_status_regx);
void SpiFlash_WrStatusReg(SPICInit_Typedef* QSPICx,char reg1,char reg2);
void SpiFlash_Powerdown(SPICInit_Typedef* QSPICx);
void SpiFlash_ReleasePowerdown(SPICInit_Typedef* QSPICx);
void SpiFlash_Erase(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode erase_type_code,unsigned int addr);
void SpiFlash_PageProgram(SPICInit_Typedef* QSPICx,char* buf,unsigned int page_start_addr);
void SpiFlash_QuadInputPageProgram(SPICInit_Typedef* QSPICx,char* buf,unsigned int page_start_addr);
void SpiFlash_ReadData(SPICInit_Typedef* QSPICx,unsigned int addr,char *buf,unsigned int size);
void SpiFlash_FastRead(SPICInit_Typedef* QSPICx,unsigned int addr,char* buf,unsigned int size);
void SpiFlash_FastReadDualOutput(SPICInit_Typedef* QSPICx,unsigned int addr,char* buf,unsigned int size);
void SpiFlash_FastReadSectorDualOutput(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors);
void SpiFlash_FastReadQuadOutput(SPICInit_Typedef* QSPICx,unsigned int addr,char *buf,unsigned int size);
void SpiFlash_FastReadSectorQuadIO(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors);
void SpiFlash_FastReadPageQuadOutput(SPICInit_Typedef* QSPICx,unsigned int addr,char *buf,unsigned int pages);
char SpiFlash_Protect(SPICInit_Typedef* QSPICx,FunctionalState cmd);
void SpiFlash_Reset_Set(SPICInit_Typedef* QSPICx);
void SpiFlash_QuadInputSequencePageProgram(SPICInit_Typedef* QSPICx,char* buf,\
  unsigned int page_start_addr,unsigned int pages);
void SpiFlash_FastReadSectorQuadOutput(SPICInit_Typedef* QSPICx,unsigned int addr,char *buf,unsigned int sectors);
void SMT_DMA_LLItest(void);
void QSPINoBootSet(void);
void SpiFlash_ReadDataSector(SPICInit_Typedef* QSPICx,unsigned \
  int addr,char *buf,unsigned int sectors);
  void SpiFlash_SequencePageProgram(SPICInit_Typedef* QSPICx,\
  char* buf,unsigned int page_start_addr,unsigned int pages);
#ifdef __cplusplus
}
#endif 

#endif /*__SPIFLASH_H*/