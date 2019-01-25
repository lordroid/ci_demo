#ifndef __SMT_H
#define __SMT_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define __CM4_REV                 0x0001  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1       /*!<  provides an MPU                     */
#define __NVIC_PRIO_BITS          3       /*!<  uses 3 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1       /*!< FPU present                                   */


typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*15 Cortex-M4 System Tick Interrupt                                */
/******  smt specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*timer WatchDog Interrupt   */
  IWDG_IRQn                   = 1,      /*arm watchdog Interrupt     */
  FPU_IRQn             		  = 2,      /*FPU STATES Interrupt             	*/
  xPWM_IRQn               	  = 3,      /*xpwm Interrupt             */
  XPWM_TZ_IRQn                = 4,      /*xpwm brake Interrupt      	*/
  ADC_IRQn                    = 5,      /*ADC  Interrupt       		*/
  VAD_IRQn                    = 6,      /*VAD  Interrupt             */
  ASR_IRQn                    = 7,      /*ASR  Interrupt             */
  CAN_IRQn                    = 8,      /*CAN  Interrupt             */
  EXTI0_IRQn                  = 9,      /*EXTI Line0 Interrupt       */
  EXTI1_IRQn                  = 10,     /*EXTI Line1 Interrupt       */
  EXTI2_IRQn           		  = 11,     /*EXTI Line2 Interrupt       */                                   
  EXTI3_IRQn                  = 12,     /*EXTI Line3 Interrupt       */
  DMA_IRQn           		  = 13,     /*DMA  Interrupt             */
  TIMER0_IRQn           	  = 14,     /*TIMER0 Interrupt           */
  TIMER1_IRQn           	  = 15,     /*TIMER1 Interrupt           */
  TIMER2_IRQn           	  = 16,     /*TIMER2 Interrupt           */
  TIMER3_IRQn           	  = 17,     /*TIMER3 Interrupt           */
  GPIO0_IRQn                  = 18,     /*GPIO0  Interrupt           */
  UART0_IRQn				  = 19,		/*UART0  Interrupt			 */
  UART1_IRQn 				  = 20, 	/*UART1  Interrupt			 */
  UART2_IRQn				  = 21,	    /*UART2  Interrupt 			 */
  UART3_IRQn				  = 22,     /*UART3  Interrupt			 */
  IIC0_IRQn					  = 23,     /*IIC0   Interrupt			 */
  IIC1_IRQn					  = 24,     /*IIC1	 Interrupt			 */
  SCU_IRQn					  = 25,     /*SCU 	 Interrupt           */
  IIS_DMA_IRQn				  = 26, 	/*IIS DMA Interrupt          */
  SPI1_IRQn					  = 27, 	/*SPI1 Interrupt			 */
  SPI2_IRQn					  = 28,		/*SPI2 Interrupt			 */
  EXTI4_IRQn				  = 29,		/*EXTI Line4 Interrupt		 */
  GPIO1_IRQn			      = 30,		/*GPIO1  Interrupt			 */
  REM_IRQn					  = 31,		/*REM 	 Interrupt			 */
} IRQn_Type;



#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */

#include <stdint.h>

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
#define TRUE 1
#define FALSE 0
#ifndef NULL
#define NULL 0
#endif
/*SPI REG define */
typedef struct
{
    __IO uint32_t SPICR0;       /*0x0 control 0*/
    __IO uint32_t SPICR1;       /*0x4 control 1*/
    __IO uint32_t SPIDR;        /*0x8  data reg*/
    __IO uint32_t SPISR;        /*0xc  status reg*/
    __IO uint32_t SPICPSR;    /*0x10  预分频寄存器*/
    __IO uint32_t SPIIER;       /*0x14  中断使能寄存器*/
    __IO uint32_t SPIRIS;       /*0x18    中断源状态寄存器*/
    __IO uint32_t SPIMIS;       /*0x1c   中断状态寄存器*/
    __IO uint32_t SPIICR;       /*0x20    中断清除寄存器*/
}SPI_TypeDef;

/*IR REG define*/
typedef struct
{
	__IO uint32_t res[15];
    __IO uint32_t REM_CR;   /*0x3c 红外控制寄存器*/
    __IO uint32_t REM_TX_DATA;  /*0x40 红外发送数据寄存器*/
    __IO uint32_t REM_RX_DATA;  /*0x44 红外接收数据寄存器*/
    __IO uint32_t REM_INT_CLR;   /*0x48 中断清除寄存器*/
    __IO uint32_t REM_INT_STATE;    /*0x4c 中断状态寄存器*/
}IR_TypeDef;

/*IIC REG define*/
typedef struct
{
    __IO uint32_t SCLDIV;   /*0x0  IIC SCL分频参数寄存器*/
    __IO uint32_t SRHLD;    /*0x4   IIC start条件保持时间*/
    __IO uint32_t DTHLD;    /*0x8   IIC SDA data time*/
    __IO uint32_t GLBCTRL;  /*0xc   IIC全局控制寄存器*/
    __IO uint32_t CMD;       /*0x10   IIC命令寄存器*/
    __IO uint32_t INTEN;    /*0x14  IIC中断使能寄存器*/
    __IO uint32_t INTCLR;   /*0x18  IIC中断清除寄存器*/
    __IO uint32_t SLAVDR;   /*0x1c IIC SLAVE地址寄存器*/
    __IO uint32_t TXDR;         /*0x20  IIC发送数据寄存器*/
    __IO uint32_t RXDR;     /*0x24  IIC接收数据寄存器*/
    __IO uint32_t TIMEOUT;  /*0x28  IIC超时寄存器*/
    __IO uint32_t STATUS;       /*0x2c  IIC状态寄存器*/
    __IO uint32_t BUSMON;       /*0x30  IIC总线信号监测寄存器*/
}I2C_TypeDef;


/*SCU REG define*/
typedef struct
{
    __IO uint32_t SYS_PLL_REG;              /*0x0, PLL configuration reg*/
    __IO uint32_t SYS_CTRL;                    /*0x4, sys ctrl*/
    __IO uint32_t CLKDIV_PAPAM0;         /*0x8, clock divider configuration 0*/
    __IO uint32_t CLKDIV_PAPAM1;         /*0xc, clock divider configuration 1*/
    __IO uint32_t CLKDIV_PAPAM2;         /*0x10, clock divider configuration 2*/
    __IO uint32_t CLKDIV_PAPAM3;         /*0x14, clock divider configuration 3*/
    __IO uint32_t CLKDIV_PAPAM4;         /*0x18, clock divider configuration 4*/
    __IO uint32_t CLKDIV_PAPAM5;         /*0x1c, clock divider configuration 5*/
    __IO uint32_t SYS_CLKGATE;             /*0x20, sys clock enable control*/
    __IO uint32_t PRE_CLKGATE0;           /*0x24, device clock enable control 0*/
    __IO uint32_t PRE_CLKGATE1;           /*0x28, device clock enable control 1*/
    __IO uint32_t SOFT_RST_PAPAM;      /*0x2c, software reset system papam set*/
    __IO uint32_t SOFT_SYSRST_CTRL;   /*0x30, sotware reset system control*/
    __IO uint32_t SOFT_PRERST_CTRL0; /*0x34, device reset control 0*/
    __IO uint32_t SOFT_PRERST_CTRL1; /*0x38, device reset control 1*/
    __IO uint32_t SYS_CLKDIV_EN;         /*0x3c, the clock divider parameter load enable register*/
    __IO uint32_t RSTCFG_LOCK;             /*0x40, reset  reg lock reg*/
    __IO uint32_t CKCFG_LOCK;               /*0x44, clock config lock reg*/
    __IO uint32_t IIS0_CLK_CFG;            /*0x48, iis0 clock config reg*/
    __IO uint32_t IIS1_CLK_CFG;            /*0x4c, iis1 clock config reg*/
    __IO uint32_t IIS2_CLK_CFG;            /*0x50, iis2 clock config reg*/
    __IO uint32_t SCU_INT_CLR;             /*0x54, SCU interrupt clear reg*/
    __IO uint32_t SCU_INT_MASK;          /*0x58, low power wake up cpu device interrupt enable*/
    __IO uint32_t INT_STATE;                 /*0x5c, low power interrupt state reg*/
    __IO uint32_t SCU_STATE;                 /*0x60, SCU system state*/
    __IO uint32_t M4_STCALIB;               /*0x64, M4 systick clock set*/
    __IO uint32_t M4_STATE;                  /*0x68, M4 state*/
    __IO uint32_t WAKE_UP_MASK;         /*0x6c, ext wake up mask*/
    __IO uint32_t KEY_FILTER;                /*0x70, key filter*/
    __IO uint32_t EXT0_FILTER;              /*0x74, ext0 filter*/
    __IO uint32_t EXT1_FILTER;              /*0x78, ext1 filter*/
    __IO uint32_t EXT2_FILTER;              /*0x7c, ext2 filter*/
    __IO uint32_t EXT3_FILTER;              /*0x80, ext3 filter*/
    __IO uint32_t EXT4_FILTER;              /*0x84, ext4 filter*/
    __IO uint32_t PWM_REUSE_CFG;       /*0x88, gpwm xpwm reuse config*/
    __IO uint32_t IO_REUSE_CFG;           /*0x8c, IO reuse reg 0*/
    __IO uint32_t IO_REUSE_CFG1;         /*0x90, IO reuse reg 1*/
    __IO uint32_t FUNC_TEST_SEL;         /*0x94, PAD test func select*/
    __IO uint32_t SDRAM_DLL_CTRL;       /*0x98, SDRAM delay line control*/  
	__IO uint32_t AD_IO_REUSE_CFG; 
}SCU_TypeDef;


/*XPWM寄存器定义*/
typedef struct{
	volatile unsigned int TBCTL;//0x00
  	volatile unsigned int TBSTS;
  	volatile unsigned int TBPHS;
  	volatile unsigned int TBCTR;
  	volatile unsigned int TBPRD;//0x10
  	volatile unsigned int CMPCTL;
  	volatile unsigned int CMPA;
  	volatile unsigned int CMPB;
  	volatile unsigned int CPR1;//0x20
  	volatile unsigned int CPR2;
  	unsigned int   resver1[2];
  	volatile unsigned int AQCTLA;//0x30
  	volatile unsigned int AQCTLB;
  	volatile unsigned int AQSFRC;
  	volatile unsigned int AQCSFRC;
  	volatile unsigned int DBCTL;//0x40
  	volatile unsigned int DBRED;//up dead time
  	volatile unsigned int DBFED;//down dead time
  	unsigned int  RESERVED24;
  	volatile unsigned int PCCTL;//0x50
  	volatile unsigned int PCDUTY;
  	unsigned int  resver2[2];
  	volatile unsigned int TZSEL;//0x60
  	volatile unsigned int TZCTL;
  	volatile unsigned int TZEINT;
  	volatile unsigned int TZFLG;
  	volatile unsigned int TZCLR;//0x70
  	volatile unsigned int TZFRC;
  	unsigned int  resver3[2];
  	volatile unsigned int ETSEL;//0x80
  	volatile unsigned int ETPS;
  	volatile unsigned int ETFLG;
  	volatile unsigned int ETCLR;
  	volatile unsigned int ETFRC;//0x90
}xPWM_TypeDef;


#define CSRAM_BASE           ((uint32_t)0x1fff0000) /*16KB SRAM ,address up to 0x1fff3fff*/
#define SYSRAM0_BASE       ((uint32_t)0x20000000) /*64KB SRAM ,address up to 0x2000ffff*/
#define SYSRAM1_BASE       ((uint32_t)0x20010000) /*64KB SRAM ,address up to 0x2001ffff*/
#define PERIPH_BASE          ((uint32_t)0x40000000) /*Peripheral base address in the alias region */
#define SDRAM_BASE           ((uint32_t)0x70000000) /*16MB SDRAM ,address up to 0x70ff_ffff*/

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE

#define VAD_GATE 0X12ED45
#define FE_GATE	0x12e563
#define DNN_GATE 0x9829e

/* APB peripherals*/
#define AHB_BASE                    (0xa5a5a5a5)
#define APB_BASE                    (0x5a5a5a5a)
#define SD_DIV_BASE             (0x33333333)
#define DNN_DIV_BASE            (0x22222222)
#define TARCE_CLOCK_DIV        (0x55555555)

#define SCU_BASE                    (0x40010000)
#define DMA_CTRL_BASE           (0x40011000)
#define xPWM_BASE                   (0x40012000)
#define ADC_BASE                        (0x40013000)
#define ASR_BASE                     (0x40014000)
#define ASR_AHBBASE              (0x51F738DF)
#define IIS_DMA_BASE            (0x40015000)
#define SDRAM_CTRL_BASE     	(0x40016000)
#define CAN_BASE                    (0x40030000)
#define PWM0_BASE                 (0x40031000)
#define PWM1_BASE                  (0x40032000)
#define PWM2_BASE                   (0x40033000)
#define PWM3_BASE                   (0x40034000)
#define PWM4_BASE                   (0x40035000)
#define PWM5_BASE                   (0x40036000)
#define PWM6_BASE                   (0x40037000)
#define PWM7_BASE                   (0x40038000)
#define PWM8_BASE                   (0x4003c000)
#define TIMER0_BASE                 (0x40039000)
#define TIMER1_BASE                 (0x4003a000)
#define TIMER2_BASE                 (0x4003b000)
#define TIMER3_BASE                 (0x4003c000)
#define IWDG_BASE                   (0x4003d000)
#define WWDG_BASE                   (0x4003e000)
#define SD_BASE                     (0x40041000)
#define SPI0_BASE                   (0x40040000)
#define SPI1_BASE                   (0x40041000)
#define SPI2_BASE                   (0x40042000)
#define UART0_BASE                (0x40043000)
#define UART1_BASE                (0x40044000)
#define UART2_BASE                (0x40045000)
#define UART3_BASE                (0x40046000)
#define GPIO0_BASE                (0x40047000)
#define GPIO1_BASE                (0x40048000)
#define IR_BASE                      (0x40049000)
#define IIC0_BASE                   (0x4004a000)
#define IIC1_BASE                   (0x4004b000)
#define IIS0_BASE                   (0x4004c000)
#define IIS1_BASE                   (0x4004d000)
#define IIS2_BASE                   (0x4004e000)


/* Debug MCU registers base address */
#define DBGMCU_BASE           ((uint32_t )0xE0042000)

/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */ 

#define  SCU                    ((SCU_TypeDef*)SCU_BASE)
#define SPI1                    ((SPI_TypeDef*)SPI1_BASE)
#define SPI2                    ((SPI_TypeDef*)SPI2_BASE)
#define SMT_IR                         ((IR_TypeDef*)IR_BASE)
#define IIC0                    ((I2C_TypeDef*)IIC0_BASE)
#define IIC1                    ((I2C_TypeDef*)IIC1_BASE)
#define ADC0 					((ADC_TypeDef* )ADC_BASE)
#define xPWM    ((xPWM_TypeDef* )xPWM_BASE)
#define xPWM1   ((xPWM_TypeDef* )(xPWM_BASE+0x400)) 
#define xPWM2	((xPWM_TypeDef* )(xPWM_BASE+0x800))  
#define xPWM3   ((xPWM_TypeDef* )(xPWM_BASE+0xC00))
/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/
/*SCU  SYS_CTRL*/
#define PLL_CONFIG_BEFORE   (0x1 << 3)
#define PLL_CONFIG_AFTER    (~(0x1 <<3))

/*sys clock gate define*/
#define SLEEPDEEP_MODE_GATE 0x11
#define SLEEPING_MODE_GATE   0x22
#define CM4_GATE                       0x33
#define CM4CORE_GATE              0x44
#define STCLK_GATE                       0x55
#define SRAM1_GATE                  0x66
#define SRAM0_GATE                  0x77
#define CSRAM_GATE                  0x88
#define PWM8_GATE_BASE          0x99
#define TRACE_GATE                  0xaa
/*sys and device reset define*/
#define PLL_RST_BASE            0xbb
/*sys and device clokc set enable define*/
#define CLOCK_ENABLE_MAGIC  0x12340000
#define AHB_DIV_ENABLE          (CLOCK_ENABLE_MAGIC | 0x0)
#define APB_DIV_ENABLE          (CLOCK_ENABLE_MAGIC | 0x1)
#define STCLK_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0x2)
#define ADC_DIV_ENALBE          (CLOCK_ENABLE_MAGIC | 0x3)
#define ASR_DIV_ENABLE          (CLOCK_ENABLE_MAGIC | 0x4)
#define IWDG_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0x5)
#define TIMER0_DIV_ENABLE   (CLOCK_ENABLE_MAGIC | 0x6)
#define TIMER1_DIV_ENABLE   (CLOCK_ENABLE_MAGIC | 0x7)
#define TIMER2_DIV_ENABLE   (CLOCK_ENABLE_MAGIC | 0x8)
#define TIMER3_DIV_ENABLE   (CLOCK_ENABLE_MAGIC | 0x9)
#define SPI0_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0xa)
#define SPI1_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0xb)
#define SPI2_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0xc)
#define UART0_DIV_ENABLE    (CLOCK_ENABLE_MAGIC | 0xd)
#define UART1_DIV_ENABLE    (CLOCK_ENABLE_MAGIC | 0xe)
#define UART2_DIV_ENABLE    (CLOCK_ENABLE_MAGIC | 0xf)
#define UART3_DIV_ENABLE    (CLOCK_ENABLE_MAGIC | 0x10)
#define IIS0_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0x11)
#define IIS1_DIV_ENABLE       (CLOCK_ENABLE_MAGIC | 0x12)
#define IIS2_DIV_ENABLE      (CLOCK_ENABLE_MAGIC | 0x13)
#define IR_DIV_ENABLE           (CLOCK_ENABLE_MAGIC | 0x14)
#define PLL_LOAD_ENABLE     (CLOCK_ENABLE_MAGIC | 0x15)
#define TRACECLK_DIV_ENABLE     (CLOCK_ENABLE_MAGIC | 0x16)
#define DNN_DIV_ENABLE          (CLOCK_ENABLE_MAGIC | 0x17)
#define DMIC_DIV_ENABLE         (CLOCK_ENABLE_MAGIC | 0x18)

/** @addtogroup Exported_macro
  * @{
  */

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/*******function return defines******/
#define RETRUN_OK   0   /*return ok*/
#define PARA_ERROR   -1  /*parameters error*/
#define RETRUN_ERR  -2

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 

/************************ (C) COPYRIGHT chipintelli *****END OF FILE****/
