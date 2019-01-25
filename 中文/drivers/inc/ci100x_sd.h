
#ifndef __CI100X_SD_H
#define __CI100X_SD_H

#include "ci100x_scu.h"
#include "ci100x_dma.h"
#include "ci100x_global.h"
#ifdef __cplusplus
	extern "C" {
#endif

#define SD_BASE		(0x40041000)	
	  
#define TIMEOUT_CMD  50000//
#define TIMEOUT_W    200000    
#define TIMEOUT_R    100000
      
typedef struct 
{
	volatile unsigned int SPI_CR0;	
	volatile unsigned int SPI_CR1;
	volatile unsigned int SPI_DR;
	volatile unsigned int SPI_SR;
	volatile unsigned int SPI_CPSR;
	volatile unsigned int SPI_IER;
	volatile unsigned int SPI_RIS;
	volatile unsigned int SPI_MIS;
	volatile unsigned int SPI_ICR;
    volatile unsigned int SDCREG;
    volatile unsigned int SDCMDREG;
    volatile unsigned int SDCMDREG1;
    volatile unsigned int SDSTATE;
    volatile unsigned int SDCRREG1;
    volatile unsigned int SDCRREG2;
    volatile unsigned int SDCRREG3;
}SD_TypeDef;

#define SD ((SD_TypeDef*)SD_BASE)

typedef struct{
	DMAC_FLOWCTRL flowctrl;
	BURSTSIZEx busrtsize;
	TRANSFERWIDTHx transferwidth;
  	unsigned int srcaddr;
	unsigned int destaddr;
	unsigned int bytesize;
}SDDMAInit_Struct;
typedef enum
{
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
	SD_INTERNAL_ERROR,
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING,
	SD_REQUEST_NOT_APPLICABLE,
	SD_INVALID_PARAMETER,
	SD_UNSUPPORTED_FEATURE,
	SD_UNSUPPORTED_HW,
	SD_ERROR,
	SD_OK = 0
} SD_Error;

typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

typedef struct
{
  volatile uint8_t  CSDStruct;            /*!< CSD structure */
  volatile uint8_t  SysSpecVersion;       /*!< System specification version */
  volatile uint8_t  Reserved1;            /*!< Reserved */
  volatile uint8_t  TAAC;                 /*!< Data read access-time 1 */
  volatile uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  volatile uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  volatile uint16_t CardComdClasses;      /*!< Card command classes */
  volatile uint8_t  RdBlockLen;           /*!< Max. read data block length */
  volatile uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  volatile uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  volatile uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  volatile uint8_t  DSRImpl;              /*!< DSR implemented */
  volatile uint8_t  Reserved2;            /*!< Reserved */
  volatile uint32_t DeviceSize;           /*!< Device Size */
  volatile uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  volatile uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  volatile uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  volatile uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  volatile uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  volatile uint8_t  EraseGrSize;          /*!< Erase group size */
  volatile uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  volatile uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  volatile uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  volatile uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  volatile uint8_t  WrSpeedFact;          /*!< Write speed factor */
  volatile uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  volatile uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  volatile uint8_t  Reserved3;            /*!< Reserded */
  volatile uint8_t  ContentProtectAppli;  /*!< Content protection application */
  volatile uint8_t  FileFormatGrouop;     /*!< File format group */
  volatile uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  volatile uint8_t  PermWrProtect;        /*!< Permanent write protection */
  volatile uint8_t  TempWrProtect;        /*!< Temporary write protection */
  volatile uint8_t  FileFormat;           /*!< File Format */
  volatile uint8_t  ECC;                  /*!< ECC code */
  volatile uint8_t  CSD_CRC;              /*!< CSD CRC */
  volatile uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

typedef struct
{
  volatile uint8_t  ManufacturerID;       /*!< ManufacturerID */
  volatile uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  volatile uint32_t ProdName1;            /*!< Product Name part1 */
  volatile uint8_t  ProdName2;            /*!< Product Name part2*/
  volatile uint8_t  ProdRev;              /*!< Product Revision */
  volatile uint32_t ProdSN;               /*!< Product Serial Number */
  volatile uint8_t  Reserved1;            /*!< Reserved1 */
  volatile uint16_t ManufactDate;         /*!< Manufacturing Date */
  volatile uint8_t  CID_CRC;              /*!< CID CRC */
  volatile uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

typedef struct
{
    SD_CSD SD_csd;
    SD_CID SD_cid;
    uint64_t CardCapacity;  /*!< Card Capacity */
    uint32_t CardBlockSize; /*!< Card Block Size */
    uint16_t RCA;
    uint8_t CardType;
} SD_CardInfo;

SD_Error SD_Init(void);

SD_Error SD_WriteBlock(unsigned char *writebuff, 
                       unsigned int WriteBlockAddr);

SD_Error SD_WriteMultiBlocks(unsigned char *writebuff, 
                             unsigned int WriteBlockAddr,
                             unsigned int NumberOfBlocks);

SD_Error SD_ReadBlock(unsigned char *readbuff, 
                      unsigned int ReadBlockAddr);

SD_Error SD_ReadMultiBlocks(unsigned char *readbuff, 
                            unsigned int ReadBlockAddr, 
                            unsigned int NumberOfBlocks);

#include "ff.h"		
#include <stdlib.h>
#include <string.h>

FRESULT m_fread ( 
	FIL *fp, 		/* Pointer to the file object */
	void *buff,		/* Pointer to data buffer */
	UINT btr,		/* Number of bytes to read */
	UINT *br		/* Pointer to number of bytes read */
	  );
	
FRESULT m_fwrite (
	FIL *fp,			/* Pointer to the file object */
	const void *buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT *bw			/* Pointer to number of bytes written */
	  );
#ifdef __cplusplus
	}
#endif


#endif /*__CI100X_SD_H*/

