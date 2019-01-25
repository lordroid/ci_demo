/**
  ******************************************************************************
  * @文件    ci100x_i2c.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的I2C模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef I2C_H_
#define I2C_H_
#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"
#define I2C_NAME_SIZE 20
#ifndef NULL
#define NULL 0
#endif
struct i2c_algo_data
{
	void 				*data;	/* private low level data */
	void (*write_byte)		(void *data, int reg, int val);
	int  (*read_byte)		(void *data, int reg);
	int  (*wait_for_completion)	(void *data);
	void (*reset)		(void *data);
	/* i2c_errors values -- error types */
	unsigned int			i2c_errors;
	unsigned int			i2c_over;
};

struct i2c_algorithm
{
	/* master_xfer should return the number of messages successfully
	   processed, or a negative value on error */
	//int (*master_xfer)(struct i2c_data *adap,struct i2c_msg *msgs,
	//                   int num);
        int (*master_xfer)(struct i2c_data *,struct i2c_msg *,
	                   int);
	/* To determine what the adapter supports */
	unsigned int (*functionality) (struct i2c_data *);
};

struct i2c_data 
{
	int				irq;	/* if 0, use polling */
	int				wait;
	const struct i2c_algorithm *algo; /* the algorithm to access the bus */
	struct i2c_algo_data	algo_data;
	unsigned int			io_base;
	int					timeout;
};

struct i2c_msg 
{
	unsigned short addr;	/* slave address			*/
	unsigned short flags;
#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
	unsigned short len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

struct i2c_platform_data
{
	unsigned int base;		/* base address of IP register */
	int irq;	/* irq number */
	unsigned int 			sys_clk;		/* bus frequence */
	unsigned int i2c_clock_speed;	/* values are defined in l */
	int timeout;		/* timeout = this value * 10us */
};

struct i2c_client
{
	unsigned short flags;		/* div., see below		*/
	unsigned short addr;		/* chip address - NOTE: 7bit	*/
					/* addresses are stored in the	*/
					/* _LOWER_ 7 bits		*/
	char name[I2C_NAME_SIZE];
};


#define I2C_TRANSMIT		0x1
#define I2C_RECEIVE			0x0
#define I2C_TRANS_OVER		0x1
#define I2C_TRANS_ON		0x0

#define DEF_TIMEOUT             32
#define BUS_ERROR               (-ERET)
#define ACK_DELAY               0       /* time to delay before checking bus error */
#define MAX_MESSAGES            65536   /* maximum number of messages to send */

#define I2C_SLEEP_TIMEOUT       2       /* time to sleep for on i2c transactions */
#define I2C_RETRY               (-2000) /* an error has occurred retry transmit */

#define I2C_ISR_INIT            0xFF  /* status register init */

#define IIC_SCLDIV	0x0
#define IIC_SRHLD 	0x4
#define IIC_DTHLD 	0x8
#define IIC_GLBCTRL	0xc
#define IIC_CMD		0x10
#define IIC_INTEN	0x14
#define IIC_INTCLR	0x18
#define IIC_SLVADR	0x1c
#define IIC_TXDR	0x20
#define IIC_RXDR	0x24
#define IIC_TIMEOUT	0x28
#define IIC_STATUS	0x2c
#define IIC_BUSMON	0x30

#define CR_STOP		(0x1 << 3)
#define CR_START	(0x1 << 4)
#define CR_IRQ_CLEAR	(0x7f)
#define SR_BUSY	(0x1 << 15)
#define SR_ABITOR_LOST	(0x1 << 5)
#define SR_TRANS_ERR	(0x1 << 3)
#define SR_RXACK	(0x1 << 14)
#define SR_IRQ_FLAG	(0x1 << 12)
#define CR_NACK		(0x1 << 2)
#define CR_TB		(0x1 << 0)

#define CR_WRITE	0
#define CR_READ		0

#define WAKE_UP		0xacced
#define SLEEP		0x0

#define ENOMEM		0x1
#define ERET		0x2
#define IRQ_HANDLED	0x0
#define IRQ_NONE	0x1

#define I2C_FUNC_I2C	0x12c

//#define I2C_BASE       0x40040000

//#define I2C1_BASE	0x40041000
#define I2C_IRQ		24	//20
//#define I2C1_IRQ                25
#define SYS_CLK		72000000
#define I2C_SCL	  100	//:TODO: must be <= 400KHz

typedef struct
{
    unsigned int I2C_IO_BASE;
    unsigned int I2C_CLOCK_SPEED;
    unsigned int I2C_INPUT_CLK;
    unsigned int TIMEOUT;
}I2C_InitStruct;

#define RXDUFLIE    (0x1 << 1)
#define TXDEPTIE    (0x1 << 2)
#define SSTOPIE      (0x1 << 4)
#define SLVADIE      (0x1 << 6)

#define IIC0_SLAVEADDR  0x60
#define IIC1_SLAVEADDR  0x61
#define wake_up_interruptible(x)	*x=WAKE_UP;

extern struct i2c_data i2c1_master,i2c0_master;

int i2c_init(I2C_TypeDef* I2Cx,I2C_InitStruct* InitStruct);
int i2c_master_send(I2C_TypeDef* I2Cx,struct i2c_client *client,const char *buf ,int count);
int i2c_master_recv(I2C_TypeDef* I2Cx,struct i2c_client *client,char *buf ,int count);
int wait_event_interruptible(int *wait);
int i2c_master_transfer(struct i2c_data * adap, struct i2c_msg *msgs, int num);

#ifdef __cplusplus
}
#endif
#endif /*I2C_H_*/
