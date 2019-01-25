
#if 0
#include <stdio.h>
#include <stdlib.h>
 
#include <stdint.h>
 
#include <string.h>
 
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
  
#endif

#include "includes.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
 
#include "FreeRTOS.h"
#include "task.h"

#include "ci100x_i2c.h"
#include "ci100x_scu.h"
#include "misc.h"
#include "ci100x_uart.h"

#include "CxFlash.h"
#include "command.h"
#include "common.h"

#if CX_20921_UPDATA

#define DEVICE_ADDR     (0x41)

typedef void *HANDLE;
typedef HANDLE *PHANDLE;

extern unsigned int        g_cbMaxI2cWrite;
extern unsigned int        g_cbMaxI2cRead;
extern unsigned char       g_bChipAddress;                   /*Specify the i2c chip address*/
extern unsigned char       g_bAddressOffset;
extern void *              g_pContextI2cWrite;
extern void *              g_pContextI2cWriteThenRead ;
extern void *              g_pContextSetResetpin ;

extern int debugflag ;


//int    file_length(FILE *f);
HANDLE OpenI2cDevice();
void   CloseI2cDevice(HANDLE hI2cDevice);
int    i2c_write_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr, unsigned long write_len,unsigned char* write_buf);
int    i2c_read_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr, unsigned long rd_len, unsigned char*rd_buf);

void   SetDeviceToDownLoad(HANDLE device);


/*
 * Convert a 4-byte number from a ByteOrder into another ByteOrder.
 */
unsigned long ByteOrderSwapULONG(unsigned long i)
{
	return((i&0xff)<<24)+((i&0xff00)<<8)+((i&0xff0000)>>8)+((i>>24)&0xff);
}

/*
 * Convert a 4-byte number from generic byte order into Big Endia
 */
unsigned long ToLittleEndiaULONG(unsigned long i)
{
#if (__BYTE_ORDER != __LITTLE_ENDIAN)
	return ByteOrderSwapULONG(i);
#else
	return i;
#endif
}

#if CX20921_IIC1
extern unsigned int iic1_client_flags;

static void i2c0_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct={0};
    I2C_InitStruct InitStruct={0};
    
	NVIC_InitStruct.NVIC_IRQChannel	=IIC1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  	Scu_SetDeviceGate((unsigned int)IIC1,ENABLE);
	Scu_SetIOReuse(I2C1_SCL_PAD,FIRST_FUNCTION);
	Scu_SetIOReuse(I2C1_SDA_PAD,FIRST_FUNCTION);
    
    InitStruct.I2C_IO_BASE      = (unsigned int)IIC1;
    InitStruct.I2C_CLOCK_SPEED  = 100;      //100KHz
    InitStruct.I2C_INPUT_CLK    = 50000000; //50MHz
    InitStruct.TIMEOUT  = 10000;
    i2c_init((I2C_TypeDef *)IIC1,&InitStruct);
}

#else
extern unsigned int iic0_client_flags;

static void i2c0_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct={0};
    I2C_InitStruct InitStruct={0};
    
	NVIC_InitStruct.NVIC_IRQChannel	=IIC0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd	=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  	Scu_SetDeviceGate((unsigned int)IIC0,ENABLE);
	Scu_SetIOReuse(I2C0_SCL_PAD,FIRST_FUNCTION);
	Scu_SetIOReuse(I2C0_SDA_PAD,FIRST_FUNCTION);
    
    InitStruct.I2C_IO_BASE      = (unsigned int)IIC0;
    InitStruct.I2C_CLOCK_SPEED  = 100;      //100KHz
    InitStruct.I2C_INPUT_CLK    = 50000000; //50MHz
    InitStruct.TIMEOUT  = 10000;
    i2c_init((I2C_TypeDef *)IIC0,&InitStruct);
}

#endif

void sys_mdelay(unsigned int ms_delay)
{
	volatile int i;
    while(ms_delay--)
    {
        for(i=0;i<0xfff;i++);
    } 
}

HANDLE OpenI2cDevice()
{
#if CX20921_IIC1
	return (HANDLE)IIC1;
#else
	return (HANDLE)IIC0;
#endif
}


void CloseI2cDevice(HANDLE hI2cDevice)
{

}

/* This is an I2C Write function. 
 * PARAMETERS
 *            i2c_dev          [in] - A pointer to a caller-supplied context area as specified in 
 *									  the CallbackContext parameter of SetupI2cWriteMemCallback. 
 *            slave_addr       [in] - The i2c chip address.
 *            sub_addr         [in] - The i2c data address.
 *            write_len        [in] - The size of the output buffer, in bytes.
 *            write_buf        [in] - A pointer to the putput buffer that contains the data required to perform the operation.
 * RETURN
 *    If the operation completes successfully, the return value is ERRNO_NOERR.
 *    Otherwise, return ERRON_* error code. 
 */
int i2c_write_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr, unsigned long write_len,unsigned char* write_buf)
{
	struct i2c_client client;
    unsigned char   pbuf[203]={0};
    
    i2c0_init();
    write_len  = (write_len > 200)?200:write_len;
    client.flags    = 0;
    client.addr     = DEVICE_ADDR;
    pbuf[0]         = sub_addr>>8;
    pbuf[1]         = sub_addr&0xff;
    memcpy(&pbuf[2], write_buf, write_len) ; // 4 bytes per word.
#if CX20921_IIC1    
    write_len = i2c_master_send((I2C_TypeDef *)IIC1, &client, (const char *)pbuf, write_len + 2);
#else
	write_len = i2c_master_send((I2C_TypeDef *)IIC0, &client, (const char *)pbuf, write_len + 2);
#endif
	return ERRNO_NOERR;
}


/* 
   in case the syste has no htobe32 function we re-define it
   */
#ifndef htobe32
uint32_t htobe32( uint32_t x)
{
	union { 
		uint32_t u32; 
		uint8_t  v[4]; 
		} ret;
	
	ret.v[0] = (uint8_t) (x>>24);
	ret.v[1] = (uint8_t) (x>>16);
	ret.v[2] = (uint8_t) (x>> 8);
	ret.v[3] = (uint8_t) (x);
	return ret.u32;
}
#endif


/* This is a call back routine for doing I2C READ. 
 * Before eading data from the salve device, you must tell slave device what sub-address number is. 
 * So a read of the salve actually starts off by writing sub-address to it.
 * PARAMETERS
 *          i2c_dev            [in]  - A pointer to a caller-supplied context area as specified in 
 *									   the CallbackContext parameter of SetupI2cWriteMemCallback. 
 *          slave_addr         [in]  - The i2c chip address.
 *          sub_addr           [in]  - slave addr.
 *          rd_len             [in]  - Specify the read data size.
 *          rd_buf             [out] - A pointer to the input buffer 
 * RETURN
 *    If the operation completes successfully, the return value is ERRNO_NOERR.
 *    Otherwise, return ERRON_* error code. 
 */
static int i2c_master_recv_8bit(I2C_TypeDef* I2Cx,struct i2c_client *client,char *buf ,int count)
{
	struct i2c_data *adap;
    struct i2c_msg msgs[2];
    int     ret32;
#if CX20921_IIC1 
    adap = &i2c1_master;
    iic1_client_flags   = client->flags;
#else
	adap = &i2c0_master;
    iic0_client_flags   = client->flags;
#endif


	msgs[0].addr    = client->addr;
	msgs[0].flags   = I2C_M_TEN;
	msgs[0].len     = 2;
	msgs[0].buf     = (unsigned char*)buf;
    
	msgs[1].addr    = client->addr;
	msgs[1].flags   = I2C_M_RD | I2C_M_TEN;
	msgs[1].len     = count;
	msgs[1].buf     = (unsigned char*)buf;
	ret32 = i2c_master_transfer(adap,msgs,2);
    
    return (ret32 == 1) ? count : ret32;
}

int i2c_read_imp(HANDLE i2c_dev, unsigned char slave_addr, unsigned long sub_addr, unsigned long rd_len, unsigned char*rd_buf)
{
	struct i2c_client client;
    
    i2c0_init();
    
    rd_len  = (rd_len > 50)?50:rd_len;
    client.flags    = I2C_M_RD;
    client.addr     = DEVICE_ADDR;
	//client.addr     = slave_addr;
    rd_buf[0]       = sub_addr>>8;
    rd_buf[1]       = sub_addr&0xff;
#if CX20921_IIC1 
	i2c_master_recv_8bit((I2C_TypeDef *)IIC1, &client, (char *)rd_buf, rd_len);
#else
	i2c_master_recv_8bit((I2C_TypeDef *)IIC0, &client, (char *)rd_buf, rd_len);
#endif
	//i2c_master_recv((I2C_TypeDef *)IIC0, &client, (char *)rd_buf, rd_len);
	return ERRNO_NOERR;
}

#if defined(_MSC_VER) 
extern "C" __declspec(dllimport) void __stdcall  Sleep(unsigned long dwMilliseconds);
int usleep (__useconds_t __useconds)
{
	Sleep(__useconds /1000);
	return 0;
}
#endif



/*----------------GPIO functions----------------*/
/*----------------------------------------------*/
/* export the gpio to user mode space */
extern void reset_20921(); 
void SetDeviceToDownLoad(HANDLE I2c_dev,unsigned long enable)
{
	reset_20921();
}
/*--------------------end-----------------------*/

#endif
