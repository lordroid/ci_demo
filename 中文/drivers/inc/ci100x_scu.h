/**
  ******************************************************************************
  * @文件    ci100x_scu.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    chipintelli公司的CI100X芯片的SCU模块头文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */
#ifndef _SCU_H_
#define _SCU_H_

#ifdef __cplusplus
    extern "C"{
#endif

#include "smt.h"
typedef struct
{
    unsigned int device_select; //0: iis0 1:iis1 2: iis2
    unsigned int model_sel;
    unsigned int over_sample;
    unsigned int sck_wid;
    unsigned int mclk_sel;
    unsigned int mclkout_sel;
    unsigned int sck_ext_inv_sel;
}IIS_CLK_CONFIGTypeDef;
/*scu iis clk config define*/
/*model sel*/
#define IIS_MASTER 0x0
#define IIS_SLAVE   0x2
/*over_sample define*/
#define IIS_128FS   0x0
#define IIS_192FS   0x1
#define IIS_256FS   0x2
#define IIS_384FS   0x3
/*sck wid define*/
#define SCK_LRCK_64 0x1
#define SCK_LRCK_32 0x0
/*mclk sel*/
#define SCU_OUTPUT_CLK  0x0
#define NO_OUTPUT_CLK   0x2
/*mclkout sel define*/
#define IIS1_MCLK   0x0
#define IIS2_MCLK   0x1
/*sck_ext_inv_sel define*/
#define SCK_EXT 0x0
#define SCK_EXT_INV 0x1

typedef enum
{
    UART0_TX_PAD,
    UART0_RX_PAD,
    UART1_TX_PAD,
    UART1_RX_PAD,
    UART1_CTS_PAD,
    UART1_RTS_PAD,
    UART2_TX_PAD,
    UART2_RX_PAD,
    UART3_TX_PAD,
    UART3_RX_PAD,
    IR_TX_PAD,
    IR_RX_PAD,
    CAN_TX_PAD,
    CAN_RX_PAD,
    I2C0_SCL_PAD,
    I2C0_SDA_PAD,
    I2C1_SCL_PAD,
    I2C1_SDA_PAD,
    I2S_MCLK_PAD,
    I2S0_SCLK_PAD,
    I2S0_LRCLK_PAD,
    I2S0_SDI_PAD,
    I2S1_SCLK_PAD,
    I2S1_LRCLK_PAD,
    I2S1_SDI_PAD,
    I2S1_SDO_PAD,
    I2S2_SDI_PAD,
    I2S2_SDO_PAD,
    I2S2_LRCLK_PAD,
    I2S2_SCLK_PAD,
    XPWM0_PAD,
    XPWM1_PAD,
    XPWM2_PAD,
    XPWM3_PAD,
    XPWM4_PAD,
    XPWM5_PAD,
    PWM6_PAD,
    PWM7_PAD,
    PWM8_PAD,
    SPI1_DOUT_PAD,
    SPI1_DIN_PAD,
    SPI1_CS_PAD,
    SPI1_CLK_PAD,
    SPI0_CLK_PAD,
    SPI0_CS_PAD,
    SPI0_D0_PAD,
    SPI0_D1_PAD,
    SPI0_D2_PAD,
    SPI0_D3_PAD,
    SPI2_DIN_PAD,
    SPI2_DOUT_PAD,
    SPI2_CS_PAD,
    SPI2_CLK_PAD,
    JTAG_TDI_PAD,
    JTAG_TDO_PAD,
    JTAG_TMS_PAD,
    JTAG_TCK_PAD,
    TRST_PAD,
    TEST_PAD,
    AIN0_PAD,
    AIN1_PAD,
    AIN2_PAD,
    AIN3_PAD,
    AIN4_PAD,
    AIN5_PAD,
    AIN6_PAD,
    AIN7_PAD,
}PinPad_Name;

typedef enum
{
    FIRST_FUNCTION = 0,
    SECOND_FUNCTION = 1,
    THIRD_FUNCTION = 2,
    FORTH_FUNCTION = 3,
}IOResue_FUNCTION;
#define PLL_DIV_BASE            (0x66666666)
#define STCLK_DIV_BASE          (0x44444444)


/*Scu function defines*/
int Scu_SetPLL(int pll_od,int pll_dn,int pll_dm);
int Scu_SetPll_384(void);
int Scu_SetPll_320(void);
int Scu_SetPll_400(void);
int Scu_SetPll_240(void);
int Scu_SetPll_200(void);
int Scu_SetPll_160(void);
int Scu_SetPll_CKIN(void);
void Unlock_Ckconfig(void);
void Unlock_Ckconfig(void);
void Lock_Ckconfig(void);
void Lock_Rstconfig(void);
int Scu_Setdiv_Parameter(unsigned int device_base,unsigned int div_num);
int Scu_SetClkgate(unsigned int device_base,int gate);
int Scu_SetDeviceGate(unsigned int device_base, int gate);
void Scu_SetWDG_Halt(void);
void Scu_CleanWDG_Halt(void);
void Scu_SoftwareRst_System(void);
void Scu_SoftwareRst_SystemBus(void);
void Scu_Wwdg_RstSys_Config(void);
void Scu_Wwdg_RstSysBus_Config(void);
void Scu_Iwdg_RstSys_Config(void);
void Scu_Iwdg_RstSysBus_Config(void);
int Scu_SetClkDiv_Enable(unsigned int device_base);
int Scu_Setdevice_Reset(unsigned int device_base);
int Scu_SetIISCLK_Config(IIS_CLK_CONFIGTypeDef *piis_config);
int Scu_SetIISCLK_Config(IIS_CLK_CONFIGTypeDef *piis_config);
void Scu_MaskScuInterrupt(void);
void Scu_SetWakeup_Source(int irq);
void Scu_SetIOReuse(PinPad_Name pin,IOResue_FUNCTION io_function);
void Scu_AllPreipheralRst(void);
int Scu_Setdevice_ResetRelease(unsigned int device_base);
void Scu_AllPreipheralclkclose(void);
#ifdef __cplusplus
}
#endif
#endif  /*_SCU_H_ endif*/
