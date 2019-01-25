#ifndef __CI100X_DMAC_H
#define __CI100X_DMAC_H

#include "smt.h"
#ifdef __cplusplus
extern "C"{ 
#endif 
  
#ifndef DMAC_BASE
#define DMAC_BASE (unsigned int)0x40011000
#endif 

#define GDMA_SDRAM_ADDR  (0x70000000UL)
#define GDMA_SDRAM_SIZE	 (16*1024*1024)
  
#define GDMA_CSRAM_ADDR  (0x1FFF8000UL)
#define GDMA_CSRAM_SIZE	 (32*1024)
  
#define GDMA_SRAM0_ADDR  (0x1FFE8000UL)
#define GDMA_SRAM0_SIZE  (64*1024) 
  
#define GDMA_SRAM1_ADDR  (0x20000000UL)
#define GDMA_SRAM1_SIZE  (64*1024)  

#define GDMA_PCMRAM_ADDR  (0x20020000UL)
#define GDMA_PCMRAM_SIZE  (16*1024)   

#define GDMA_FFTRAM_ADDR  (0x200FF800UL)
#define GDMA_FFTRAM_SIZE  (2*1024)      
  
typedef struct{
	volatile unsigned int DMACCxSrcAddr;
	volatile unsigned int DMACCxDestAddr;
	volatile unsigned int DMACCxLLI;
	volatile unsigned int DMACCxControl;
	volatile unsigned int DMACCxConfiguration;
	unsigned int reserved[3];
}DMACChanx_TypeDef;

typedef struct{
	volatile unsigned int DMACIntStatus;
	volatile unsigned int DMACIntTCStatus;
	volatile unsigned int DMACIntTCClear;
	volatile unsigned int DMACIntErrorStatus;
	volatile unsigned int DMACIntErrClr;
	volatile unsigned int DMACRawIntTCStatus;
	volatile unsigned int DMACRawIntErrorStatus;
	volatile unsigned int DMACEnbldChns;
	volatile unsigned int DMACSoftBReq;
	volatile unsigned int DMACSoftSReq;
	volatile unsigned int DMACSoftLBReq;
	volatile unsigned int DMACSoftLSReq;
	volatile unsigned int DMACConfiguration;
	volatile unsigned int DMACSync;
	unsigned int reserved1[50];
	DMACChanx_TypeDef DMACChannel[8];
	unsigned int reserved2[195];
	volatile unsigned int DMACITCR;
	volatile unsigned int DMACITOP[3];
	unsigned int reserved3[693];
	volatile unsigned int DMACPeriphID[4];
	volatile unsigned int DMACPCellID[4];
}DMAC_TypeDef;

#define DMAC  ((DMAC_TypeDef*)DMAC_BASE)


typedef enum{
	DMACChannel0 = 0,
	DMACChannel1 = 1,
	DMACChannel2 = 2,
	DMACChannel3 = 3,
	DMACChannel4 = 4,
	DMACChannel5 = 5,
	DMACChannel6 = 6,
	DMACChannel7 = 7,
	DMACChannelALL = 9,
}DMACChannelx;

typedef enum{
	DMAC_AHBMaster1 = 0,
	DMAC_AHBMaster2 = 1,
}DMAC_AHBMasterx;

typedef enum{
	LittleENDIANMODE = 0,
	BigENDIANMODE	= 1,
}ENDIANMODE;

typedef enum{
	INCREMENT = 1,
	NOINCREMENT = 0,
}INCREMENTx;

typedef enum{
	TRANSFERWIDTH_8b = 0,
	TRANSFERWIDTH_16b = 1,
	TRANSFERWIDTH_32b = 2,
}TRANSFERWIDTHx;

typedef enum{
	BURSTSIZE1 = 0,
	BURSTSIZE4 = 1,
	BURSTSIZE8 = 2,
	BURSTSIZE16 = 3,
	BURSTSIZE32 = 4,
	BURSTSIZE64 = 5,
	BURSTSIZE128 = 6,
	BURSTSIZE256 = 7,
}BURSTSIZEx;
/*Programmable DMA burst size. You can programme the DMA burst size to
transfer data more efficiently. The burst size is usually set to half the size of the
FIFO in the peripheral.
Busrts do not cross the 1KB address boundary*/

typedef enum{
	ACCESS_USERMODE = 0,
	ACCESS_PRIVILEGEDMODE = 1,
}ACCESS_MODE;

typedef enum{
	BUFFERABLE = 1,
	NONBUFFERABLE = 0,
}BUFFERABLEx;

typedef enum{
	CACHEABLE = 1,
	NONCACHEABLE = 0,
}CACHEABLEx;

typedef enum{
	CHANNELINTMASK_ITC = 15,
	CHANNELINTMASK_IE = 14,
}CHANNELINTMASKx;

typedef enum{
	M2M_DMA = 0,
	M2P_DMA = 1,
	P2M_DMA = 2,
	SP2DP_DMA = 3,
	SP2DP_DP = 4,
	M2P_P = 5,
	P2M_P = 6,
	SP2DP_SP = 7,
}DMAC_FLOWCTRL;

typedef enum{
	DMAC_Peripherals_SPI0 = 0,
	DMAC_Peripherals_SPI1_RX = 1,
	DMAC_Peripherals_SPI1_TX = 2,
	DMAC_Peripherals_UART0_RX = 5,
	DMAC_Peripherals_UART0_TX = 6,
	DMAC_Peripherals_UART1_RX = 3,
	DMAC_Peripherals_UART1_TX = 4,
}DMAC_Peripherals;

typedef struct{
	FunctionalState TCInt;
	CACHEABLEx		CacheAble;
	BUFFERABLEx		BufferAble;
	ACCESS_MODE		AccessMode;
	INCREMENTx 		DestInc;
	INCREMENTx		SrcInc;
	DMAC_AHBMasterx DestMaster;
	DMAC_AHBMasterx SrcMaster;
	TRANSFERWIDTHx	DestTransferWidth;
	TRANSFERWIDTHx	SrcTransferWidth;
	BURSTSIZEx		DestBurstSize;
	BURSTSIZEx		SrcBurstSize;
	unsigned short 	TransferSize;
}LLI_Control;

typedef struct{
	unsigned int SrcAddr;
	unsigned int DestAddr;
	unsigned int NextLLI;
	unsigned int Control;
}DMAC_LLI;


int DMAC_IntStatus(DMACChannelx dmachannel);
int DMAC_IntTCStatus(DMACChannelx dmachannel);
void DMAC_IntTCClear(DMACChannelx dmachannel);
int DMAC_IntErrorStatus(DMACChannelx dmachannel);
void DMAC_IntErrorClear(DMACChannelx dmachannel);
int DMAC_RawIntTCStatus(DMACChannelx dmachannel);
int DMAC_RawErrorIntStatus(DMACChannelx dmachannel);
int DMAC_ChannelEnableStatus(DMACChannelx dmachannel);
void DMAC_SoftwareBurstReq(DMACChannelx dmachannel);
void DMAC_SoftwareSingleReq(DMACChannelx dmachannel);
void DMAC_SoftwareLastBurstReq(DMACChannelx dmachannel);
void DMAC_SoftwareLastSingleReq(DMACChannelx dmachannel);
void DMAC_Config(DMAC_AHBMasterx dmamaster,ENDIANMODE endianmode);
void DMAC_EN(FunctionalState cmd);
void DMAC_ChannelSoureAddr(DMACChannelx dmachannel,unsigned int addr);
void DMAC_ChannelDestAddr(DMACChannelx dmachannel,unsigned int addr);
void DMAC_ChannelLLI(DMACChannelx dmachannel,unsigned int dmalli,DMAC_AHBMasterx dmamaster);
void DMAC_ChannelSourceConfig(DMACChannelx dmachannel,INCREMENTx inc,\
 	DMAC_AHBMasterx dmamaster,TRANSFERWIDTHx  transferwidth,BURSTSIZEx brustsize);
void DMAC_ChannelDestConfig(DMACChannelx dmachannel,INCREMENTx inc,\
 	DMAC_AHBMasterx dmamaster,TRANSFERWIDTHx  transferwidth,BURSTSIZEx brustsize);
void DMAC_ChannelTCInt(DMACChannelx dmachannel,FunctionalState cmd);
void DMAC_ChannelProtectionConfig(DMACChannelx dmachannel,ACCESS_MODE access_mode,\
	BUFFERABLEx bufferable,CACHEABLEx cacheable);
void DMAC_ChannelTransferSize(DMACChannelx dmachannel,unsigned short size);
void DMAC_ChannelDisable(DMACChannelx dmachannel);
void DMAC_ChannelEnable(DMACChannelx dmachannel);
void DMAC_ChannelInterruptMask(DMACChannelx dmachannel,CHANNELINTMASKx \
	channelintmask,FunctionalState cmd);
void DMAC_ChannelPowerDown(DMACChannelx dmachannel,FunctionalState cmd);
void DMAC_ChannelHalt(DMACChannelx dmachannel,FunctionalState cmd);
void DMAC_ChannelConfig(DMACChannelx dmachannel,char destperiph,char srcperiph ,DMAC_FLOWCTRL flowctrl);
void DMAC_ChannelLock(DMACChannelx dmachannel,FunctionalState cmd);
void DMAC_M2MConfig(DMACChannelx dmachannel,unsigned int srcaddr,unsigned int destaddr,unsigned int bytesize);
void DMAC_M2P_P2MConfig(DMACChannelx dmachannel,DMAC_Peripherals periph,DMAC_FLOWCTRL flowctrl,\
	unsigned int srcaddr,unsigned int destaddr,unsigned int bytesize);
void DMAC_P2PConfig(DMACChannelx dmachannel,DMAC_Peripherals srcperiph,DMAC_Peripherals destperiph,unsigned int bytesize);

#ifdef __cplusplus
}
#endif 

#endif /*__CI100X_DMA_H*/ 
