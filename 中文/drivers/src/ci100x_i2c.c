/**
  ******************************************************************************
  * @文件    ci100x_i2c.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的I2C控制器模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#include "misc.h"
#include <string.h>
#include "ci100x_i2c.h"
#include "ci100x_global.h"

#define i2c_debug 3

#if 1
#define DEB1(fmt, args...) do{}while(0)
#define DEB2(fmt, args...) do{}while(0)
#define DEB3(fmt, args...) do{}while(0)
#else
#define DEB1(fmt, args...) do { if (i2c_debug>=1) mprintf(fmt, ## args); } while(0)
#define DEB2(fmt, args...) do { if (i2c_debug>=2) mprintf(fmt, ## args); } while(0)
#define DEB3(fmt, args...) do { if (i2c_debug>=3) mprintf(fmt, ## args); } while(0)
#endif

#define i2c_outb(adap, reg, val) adap->write_byte(adap->data, reg, val)
#define i2c_inb(adap, reg) adap->read_byte(adap->data, reg)

#define i2c_set_clk(adap, val) i2c_outb(adap, IIC_SCLDIV, val)
#define i2c_status(adap) i2c_inb(adap, IIC_STATUS)
#define i2c_set_cmd(adap,val)  i2c_outb(adap,IIC_CMD,val)
#define i2c_intclr_set(adap,val)  i2c_outb(adap,IIC_INTCLR,val)
#define i2c_wait(adap) adap->wait_for_completion(adap->data)
#define i2c_rx_byte(adap) i2c_inb(adap, IIC_RXDR)
#define i2c_resetd(adap) adap->reset(adap->data)
#define i2c_tx_byte(adap, val) i2c_outb(adap, IIC_TXDR, val)

#define _REG_R(reg) *(volatile unsigned int*)(reg)
#define _REG_W(val,reg)	*(volatile unsigned int*)(reg)=(val)

struct i2c_data i2c0_master;
struct i2c_data i2c1_master;
struct i2c_algorithm i2c0_algo; 
struct i2c_algorithm i2c1_algo;

//unsigned char receive_data[100];
//unsigned int receive_data_count[2];
//unsigned int transfer_data_count[2];

unsigned int tx_count[2];
unsigned int rx_count[2];
unsigned int iic0_expact_send_size;
unsigned int iic1_expact_send_size;
unsigned int iic0_client_flags;
unsigned int iic1_client_flags;

#define IIC_WAITEEVENT_TIMEOUT  10      //Normally 3 is ready
static volatile unsigned int i2c_timeout = 0;
/**
  * @功能:延迟函数
  * @注意:无        
  * @参数:time 延迟毫秒
  * @返回值:无
  */
static void delay_time(int time)
{
    int loop;
    while(time--)
    {
      loop = 100;
      while(loop--);
    }
}
/**
  * @功能:等待中断信号量
  * @注意:无        
  * @参数:wait 等待信号
  * @返回值:成功返回0
  */
int wait_event_interruptible(int *wait)
{
    int i = 0;
    i2c_timeout = 0;
    while(*wait != WAKE_UP)
    {
        delay_time(10);
        i++;
        if(i >= IIC_WAITEEVENT_TIMEOUT)
        {
            i2c_timeout = 1;
            break;
        }
    }
    *wait = SLEEP;
    return 0;
}
/**
  * @功能:读I2C寄存器值
  * @注意:无        
  * @参数:pd 传入指针，用来寻址寄存器  reg寄存器偏移
  * @返回值:读取的寄存器值
  */
static int i2c_readbyte(void *pd, int reg)
{
    struct i2c_data *i2c = (struct i2c_data *)pd;
    return _REG_R(i2c->io_base + reg);	//ioread8(i2c->io_base + reg);
}
/**
  * @功能:写寄存器值
  * @注意:无        
  * @参数:pd 传入指针   reg寄存器偏移  val 写入寄存器的值
  * @返回值:无
  */
static void i2c_writebyte(void *pd, int reg, int val)
{
    struct i2c_data *i2c = (struct i2c_data *)pd;
    _REG_W(val, i2c->io_base + reg);	//iowrite8(val, i2c->io_base + reg);
}
/**
  * @功能:等待I2C总线传输完成
  * @注意:无        
  * @参数:pd 传入指针，用来寻址寄存器
  * @返回值:成功返回0
  */
static int i2c_waitforcompletion(void *pd)
{
    struct i2c_data *i2c = (struct i2c_data*)pd;
    int ret = 0;

    if (i2c->irq)
    {
      ret = wait_event_interruptible(&i2c->wait);
    }
    else
    {
      /*
      * Do polling...
      * XXX: Could get stuck in extreme cases!
      *      Maybe add timeout, but using irqs is preferred anyhow.
      */
      while ((i2c->algo_data.read_byte(i2c, IIC_STATUS)
      & SR_IRQ_FLAG) == 0)
      delay_time(200);

      i2c->algo_data.i2c_errors = i2c->algo_data.read_byte(i2c, IIC_STATUS);
      if(i2c->algo_data.i2c_over == I2C_TRANS_OVER)
      {
        i2c->algo_data.i2c_errors &= ~SR_RXACK;
        i2c->algo_data.i2c_over = I2C_TRANS_ON;
      }  
    }

    i2c->algo_data.i2c_errors &= (~SR_IRQ_FLAG);
    return ret;
}

/**
  * @功能:复位I2C总线
  * @注意:无        
  * @参数:pd 传入指针，用来寻址寄存器
  * @返回值:无
  */
static void i2c_reset(void *pd)
{
    struct i2c_data *i2c = (struct i2c_data*)pd;

    i2c->algo_data.write_byte(i2c, IIC_CMD, CR_STOP);		//:TODO: send stop

}
/**
  * @功能:I2C总线发送start位
  * @注意:无        
  * @参数:adap 传入指针，用来寻址寄存器
  * @返回值:无
  */
static void i2c_start(struct i2c_algo_data *adap)
{
    unsigned int value;

    value = (0x1 << 4);
    i2c_intclr_set(adap,value);
    value = CR_START | CR_TB;
    i2c_set_cmd(adap,value);
    i2c_wait(adap);    
}
/**
  * @功能:I2C总线发送stop位
  * @注意:无        
  * @参数:adap 传入指针，用来寻址寄存器
  * @返回值:无
  */
static void i2c_stop(struct i2c_algo_data *adap)
{
    unsigned int value;
    DEB2("=== STOP\n");
    
    adap->i2c_over = I2C_TRANS_OVER;
    value = CR_STOP | CR_TB;
    i2c_set_cmd(adap,value);
    value = CR_IRQ_CLEAR;
    i2c_intclr_set(adap,value);
    i2c_wait(adap);
}
/**
  * @功能:配置发送数据地址
  * @注意:无        
  * @参数:adap 传入指针，用来寻址寄存器，msg I2C传递地址等消息指针
  * @返回值:无
  */
static void i2c_address(struct i2c_algo_data * adap, struct i2c_msg *msg)
{
    unsigned short flags = msg->flags;
    unsigned char addr;

    addr = (unsigned char) ( (0x7f & msg->addr) << 1 );

    if (flags & I2C_M_RD )
    {
      addr |= 1;
    }
	
    i2c_outb(adap, IIC_TXDR, addr);
}

/**
  * @功能:I2C发送数据
  * @注意:无        
  * @参数:1.adap 传入指针，用来寻址寄存器，
  *       2.lastbyte 1 是最后一个字节 0 不是最后一个字节
  *       3.receive  0 接收数据    1 发送数据
  *       4.midbyte  1是中间数据   0 不是中间数据
  * @返回值:无
  */
static void i2c_transfer( struct i2c_algo_data *adap, int lastbyte, int receive, int midbyte)
{
    unsigned char value;

    if(lastbyte)
    {
      adap->i2c_over = I2C_TRANS_OVER;
      if( receive==I2C_RECEIVE)
      {
        //value = CR_READ | CR_IRQ_CLEAR | CR_NACK | CR_STOP ;
        value = CR_NACK | CR_TB;
        i2c_set_cmd(adap,value);
        value = CR_IRQ_CLEAR;
        i2c_intclr_set(adap,value);
      }
      else
      {
        //value = CR_WRITE | CR_IRQ_CLEAR  | CR_NACK | CR_STOP ;
        value = CR_WRITE | CR_TB;
        i2c_set_cmd(adap,value);
        value = CR_IRQ_CLEAR;
        i2c_intclr_set(adap,value);
      }

      DEB2("=== LAST\n");
    }
    else if(midbyte)
    {
      if(receive==I2C_RECEIVE)
      {
        //value = CR_READ | CR_IRQ_CLEAR  ;
        value = CR_READ | CR_TB;
        i2c_set_cmd(adap,value);
        value = CR_IRQ_CLEAR;
        i2c_intclr_set(adap,value);
      }
      else
      {
        //value = CR_WRITE | CR_IRQ_CLEAR | CR_NACK ;
        value = CR_WRITE | CR_NACK | CR_TB;
        i2c_set_cmd(adap,value);
        value = CR_IRQ_CLEAR;
        i2c_intclr_set(adap,value);
      }
      DEB2("=== MID\n");
    }

    i2c_wait(adap);
}
/**
  * @功能:I2C发送多个字节数据
  * @注意:无        
  * @参数:1.adap 传入指针，用来寻址寄存器
  *       2.buf  发送数据缓冲指针
  *       3.count 传输字节个数
  *       4.last  当前传输计数
  * @返回值:传输字节数
  */

static int i2c_sendbytes(struct i2c_algo_data *adap, const char *buf,int count, int last)
{
    int wrcount;
    for (wrcount=0; wrcount<count; ++wrcount)
    {
      i2c_tx_byte(adap,buf[wrcount]);
      if ((wrcount==(count-1)) && last)
      {
        i2c_transfer( adap, last, I2C_TRANSMIT, 0);
        i2c_stop(adap);
      }
      else
      {
        i2c_transfer( adap, 0, I2C_TRANSMIT, 1);
      }
      
      if(adap->i2c_errors & SR_ABITOR_LOST)
      {
        i2c_stop(adap);
        break;
      }
      
      if(adap->i2c_errors & SR_TRANS_ERR)
      {
        i2c_stop(adap);
        break;
      }

      if(adap->i2c_errors & SR_RXACK)
      {
        DEB2("NOT ACK received after SLA+R\n");
        i2c_stop(adap);
        break;
      }
    }
    
    return (wrcount);
}
/**
  * @功能:I2C读取多个字节数据
  * @注意:无        
  * @参数:1.adap 传入指针，用来寻址寄存器
  *       2.buf  发送数据缓冲指针
  *       3.count 传输字节个数
  *       4.last  当前传输计数
  * @返回值:传输字节数
  */

static unsigned int  i2c_readbytes(struct i2c_algo_data *adap, char *buf,int count, int last)
{
    int i;

    /* increment number of bytes to read by one -- read dummy byte */
    for (i = 0; i <= count; i++)
    {
      if (i!=0)
      {
        /* set ACK to NAK for last received byte ICR[ACKNAK] = 1
        only if not a repeated start */
			
        if ((i == count) && last) 
        {
          i2c_transfer( adap, last, I2C_RECEIVE, 0);
        }
        else
        {
          i2c_transfer( adap,0, I2C_RECEIVE, 1);
        }

        if(adap->i2c_errors & SR_ABITOR_LOST)
        {			
          i2c_stop(adap);
          break;
        }
        
        if(adap->i2c_errors & SR_TRANS_ERR)
        {		
          i2c_stop(adap);
          break;
        }

        if(adap->i2c_errors & SR_RXACK)
        {		
          i2c_stop(adap);
          break;
        }

      }

      if (i) 
      {
        buf[i - 1] = i2c_rx_byte(adap);
      }
      else
      {
        i2c_rx_byte(adap); /*! dummy read */
      }
    }
	i2c_stop(adap);
    return (i - 1);
}
/**
  * @功能:I2C使用消息传输数据
  * @注意:无        
  * @参数:1.adap 传入指针，用来寻址寄存器
  *       2.msgs  消息指针
  *       3.count 传输消息个数
  * @返回值:传输消息个数
  */
static int i2c_xfer(struct i2c_data *i2c_adap, struct i2c_msg *msgs, int num)
{
    struct i2c_algo_data *adap = &i2c_adap->algo_data;
    struct i2c_msg *msg = NULL;
    int curmsg;
    int state;
    int ret,retval;
    int index;
    int timeout = i2c_adap->timeout;
    
    while (((state = i2c_status(adap)) & SR_BUSY) && timeout--)
    {
      delay_time(10);
    }
    if (state & SR_BUSY)
    {
      return -ERET;
    }
    ret = -ERET;
    for (curmsg = 0;curmsg < num; curmsg++)
    {
      int last = curmsg + 1 == num;
      msg = &msgs[curmsg];
      i2c_address(adap,msg);
      if(i2c_adap->io_base==IIC0_BASE)
      {
        index = 0;
      }
      else if(i2c_adap->io_base==IIC1_BASE)
      {
        index = 1;
      }
      tx_count[index] = 0;
      //rx_count[index] = 0;
      i2c_start(adap);

      if(adap->i2c_errors & SR_ABITOR_LOST)
      {
        i2c_stop(adap);
        goto out;
      }

      if(adap->i2c_errors & SR_TRANS_ERR)
      {
        i2c_stop(adap);
        goto out;
      }

      if(adap->i2c_errors & SR_RXACK)
      {
        i2c_stop(adap);
        goto out;
      }
      /*! Read */
      if (msg->flags & I2C_M_RD)
      {
        /*! read bytes into buffer*/
        retval = i2c_readbytes(adap, (char*)msg->buf, msg->len, last);
        if (retval != msg->len)
        {
          goto out;
        }
        else
        {
        }
      }
      else 
      {
        /*! Write */
        retval = i2c_sendbytes(adap, (char *)msg->buf, msg->len, last);

        if (retval != msg->len)
        {
          goto out;
        }
        else
        {
        }
      }
    }
    ret = curmsg;
out:	
    return ret;
}


/**
  * @功能:I2C初始化
  * @注意:无        
  * @参数:1.I2Cx I2C基地址
  *       2.InitStruct 初始化相关
  * @返回值:传输消息个数
  */
int ptr_i2c0_master =  (int)&i2c0_master;
int i2c_init(I2C_TypeDef* I2Cx,I2C_InitStruct* InitStruct)
{
    if(I2CMutex!=NULL)
	{
        xSemaphoreTake(I2CMutex, portMAX_DELAY);
    }

    struct i2c_data *i2c;
//    struct i2c_algo_data *i2c_conf;
//    int ret = 0;
    unsigned int clock;
    unsigned int scldiv_l;
    unsigned int scldiv_h;
    unsigned int scldiv;
//    int i;
    if(I2Cx == IIC0)
    {
        i2c = &i2c0_master;
        i2c->io_base = InitStruct->I2C_IO_BASE;
        i2c->wait =SLEEP;
        i2c->timeout = InitStruct->TIMEOUT;
        i2c->irq = IIC0_IRQn;
        tx_count[0] = 0;
        //rx_count[0] = 0;
        //receive_data_count[0] = 0;
        //transfer_data_count[0] = 0;
        clock = InitStruct->I2C_INPUT_CLK;
        scldiv_l = (clock / (InitStruct->I2C_CLOCK_SPEED *1000)) / 2;
        scldiv_h = scldiv_l;
        scldiv = scldiv_l | (scldiv_h << 16);
        i2c->algo_data.data = i2c;
        i2c->algo_data.write_byte = i2c_writebyte;
        i2c->algo_data.read_byte = i2c_readbyte;
        i2c->algo_data.wait_for_completion = i2c_waitforcompletion;
        i2c->algo_data.reset = i2c_reset;
        i2c->algo_data.i2c_errors = 0;
        i2c->algo_data.i2c_over = 0;
        i2c->algo = &i2c0_algo;
        ptr_i2c0_master = (int)&i2c->wait;
//	 	i2c_conf = &(i2c->algo_data);

        I2Cx->INTCLR = 0xffffffff;
        I2Cx->GLBCTRL = 0x00040080;
        I2Cx->SCLDIV = scldiv;
        I2Cx->SRHLD = scldiv;
		unsigned tmp = scldiv/2;
		if(tmp)
		{
			I2Cx->DTHLD = tmp;
		}
		else
		{
			I2Cx->DTHLD = 0x00040004;
		}
        
        I2Cx->INTCLR = 0x7f;
        I2Cx->INTEN = (RXDUFLIE | TXDEPTIE | SSTOPIE | SLVADIE);
        I2Cx->SLAVDR = IIC0_SLAVEADDR;
        I2Cx->GLBCTRL |= 0x3;
		
    }
    else if(I2Cx == IIC1)
    {
        i2c = &i2c1_master;
        i2c->io_base = InitStruct->I2C_IO_BASE;
        i2c->wait =SLEEP;
        i2c->timeout = InitStruct->TIMEOUT;
        i2c->irq = IIC1_IRQn;
        tx_count[1] = 0;
        //rx_count[1] = 0;
        //receive_data_count[1] = 0;
        //transfer_data_count[1] = 0;
        clock = InitStruct->I2C_INPUT_CLK;
        scldiv_l = (clock / (InitStruct->I2C_CLOCK_SPEED *1000)) / 2;
        scldiv_h = scldiv_l;
        scldiv = scldiv_l | (scldiv_h << 16);
        i2c->algo_data.data = i2c;
        i2c->algo_data.write_byte = i2c_writebyte;
        i2c->algo_data.read_byte = i2c_readbyte;
        i2c->algo_data.wait_for_completion = i2c_waitforcompletion;
        i2c->algo_data.reset = i2c_reset;
        i2c->algo_data.i2c_errors = 0;
        i2c->algo_data.i2c_over = 0;
        i2c->algo = &i2c1_algo;

//	 	i2c_conf = &(i2c->algo_data);

        I2Cx->INTCLR = 0xffffffff;
        I2Cx->GLBCTRL = 0x00040080;
        I2Cx->SCLDIV = scldiv;
        I2Cx->SRHLD = scldiv;
		unsigned int tmp = scldiv/2;
        if(tmp )
		{
			I2Cx->DTHLD = tmp;
		}
		else
		{
			I2Cx->DTHLD = 0x00040004;
		}
        I2Cx->INTCLR = 0x7f;
        I2Cx->INTEN = (RXDUFLIE | TXDEPTIE | SSTOPIE | SLVADIE);
        I2Cx->SLAVDR = IIC1_SLAVEADDR;
        I2Cx->GLBCTRL |= 0x3;	
    }
    if(I2CMutex!=NULL)
    {
        xSemaphoreGive(I2CMutex);
    }

    return 0;

}

/**
  * @功能:I2C使用消息发送数据
  * @注意:无        
  * @参数:1.adap  指针用来寻址对应I2C
  *       2.msgs 需要发送的消息
  *       3.num  传输消息个数
  * @返回值:传输正确；传输消息个数  ；传输错误：0
  */
int i2c_master_transfer(struct i2c_data * adap, struct i2c_msg *msgs, int num)
{
    int ret = 0;

    if(I2CMutex!=NULL)
    {
        xSemaphoreTake(I2CMutex, portMAX_DELAY);
    }
    ret = i2c_xfer((struct i2c_data *)adap,msgs,num);
    if(I2CMutex!=NULL)
    {
        xSemaphoreGive(I2CMutex);
    }
    if(i2c_timeout == 1)
    {
        ret = RETRUN_ERR;
    }
    return ret;
	
}
/**
  * @功能:I2C使用消息发送数据
  * @注意:无        
  * @参数:1.I2Cx  I2C基地址
  *       2.client 对应的I2C传输数据的SLAVE属性
  *       3.buf   传输数据缓存指针
  *       4.count 传输数据个数
  * @返回值:传输消息个数, 0 as error;
  */

int i2c_master_send(I2C_TypeDef* I2Cx,struct i2c_client *client,const char *buf ,int count)
{
    int ret = 1;
    struct i2c_data *adap;
    struct i2c_msg msg;

    if(I2Cx == IIC0)
    {
        adap = &i2c0_master;
        iic0_expact_send_size   = count;
        iic0_client_flags   = client->flags;
    }
    else if(I2Cx == IIC1)
    {
        adap = &i2c1_master;
        iic1_expact_send_size   = count;
        iic1_client_flags   = client->flags;
    }
    msg.addr = client->addr;
    msg.flags = I2C_M_TEN;
    msg.len = count;
    msg.buf = (unsigned char *)buf;

    ret = i2c_master_transfer(adap, &msg, 1);
    if(i2c_timeout == 0)
    {
        ret = count;
    }
    else if(i2c_timeout == 1)
    {
        ret = RETRUN_ERR;
    }
	/* If everything went ok (i.e. 1 msg transmitted), return #bytes
	   transmitted, else error code. */
    return ret;
}

int i2c_master_recv(I2C_TypeDef* I2Cx,struct i2c_client *client,char *buf ,int count)
{
    struct i2c_data *adap;
    struct i2c_msg msgs[2];
    int ret32;
    
    if(I2Cx == IIC0)
    {
        adap = &i2c0_master;
        iic0_client_flags   = client->flags;
    }else
    {
        adap = &i2c1_master;
        iic1_client_flags   = client->flags;
    }
	msgs[0].addr    = client->addr;
	msgs[0].flags   = I2C_M_TEN;
	msgs[0].len     = 1;
	msgs[0].buf     = (unsigned char*)buf;
    
	msgs[1].addr    = client->addr;
	msgs[1].flags   = I2C_M_RD | I2C_M_TEN;
	msgs[1].len     = count;
	msgs[1].buf     = (unsigned char*)buf;
	ret32 = i2c_master_transfer(adap,msgs,2);
    
    return (ret32 == 1) ? count : ret32;
}

 /***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/