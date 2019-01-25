/**
  ******************************************************************************
  * @文件    ci100x_mpu.h
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的MPU模块.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#ifndef __CI100X_MPU_H
#define __CI100X_MPU_H

    #ifdef __cplusplus
     extern "C" {
    #endif
     
typedef struct
{
    unsigned int  separate      :1;                 //固定为0
    unsigned int  reserved      :7;
    unsigned int  d_region      :8;                 //MPU支持的数据区数量
    unsigned int  i_region      :8;                 //MPU支持的指令区数量
    unsigned int  reserved1     :8;
}MPU_REG_TYPE;

typedef struct
{
    unsigned int  enable        :1;                 //使能MPU
    unsigned int  hf_nmi_enable :1; 
    unsigned int  privdef_enable:1; 
    unsigned int  reserved      :29;
}MPU_REG_CTRL;

typedef struct
{
    unsigned int  enable        :1;                 //使能region
    unsigned int  size          :5;
    unsigned int  reserved      :2;
    unsigned int  sub_region    :8;
    unsigned int  buff_enable   :1;
    unsigned int  cach_enable   :1;
    unsigned int  share_enable  :1;
    unsigned int  type_extend   :3;
    unsigned int  reserved1     :2;
    unsigned int  access_permit :3;
    unsigned int  reserved2     :1;
    unsigned int  xn            :1;
    unsigned int  reserved3     :3;
}MPU_REG_ATTR;

typedef struct
{
    unsigned int  region        :4;
    unsigned int  valid         :1;
    unsigned int  addr          :27;
}MPU_REG_BASE;

typedef struct
{
    MPU_REG_TYPE   type;                            //MPU类型寄存器,0xe000ed90
    MPU_REG_CTRL   ctrl;                            //MPU控制寄存器,0xe000ed94
    unsigned int   region_num;                      //MPU region number寄存器,0xe000ed98
    MPU_REG_BASE   region_base_address;             //MPU region基址寄存器,0xe000ed9c
    MPU_REG_ATTR   region_attribute;                //MPU region属性寄存器,0xe000eda0
}MPU_TypeDef;

#define MPU_BASE        0xe000ed90
#define	MPU             ((MPU_TypeDef *)MPU_BASE)

enum
{
    MPU_DISABLE=0,
    MPU_ENABLE   
};

enum
{
    MPU_XN_ENABLE=0,
    MPU_XN_DISABLE       
};

enum
{
    MPU_REGION0=0,
    MPU_REGION1,
    MPU_REGION2,
    MPU_REGION3,
    MPU_REGION4,
    MPU_REGION5,
    MPU_REGION6,
    MPU_REGION7
};

#define IS_MPU_ALL_REGION(REGION) (((REGION) == MPU_REGION0) || \
                                   ((REGION) == MPU_REGION1) || \
                                   ((REGION) == MPU_REGION2) || \
                                   ((REGION) == MPU_REGION3) || \
                                   ((REGION) == MPU_REGION4) || \
                                   ((REGION) == MPU_REGION5) || \
                                   ((REGION) == MPU_REGION6) || \
                                   ((REGION) == MPU_REGION7)))

enum
{
    MPU_UNACCESS_UNACCESS=0,
    MPU_RW_UNACCESS,
    MPU_RW_RONLY,
    MPU_RW_RW,
    MPU_RESERVED,
    MPU_RONLY_UNACCESS,
    MPU_RONLY_RONLY
};


unsigned char mpu_get_DRegion_number(void);

void mpu_mem_setup(unsigned int region,int baseAddress,int size,int access);

    #ifdef __cplusplus
    }
    #endif
#endif

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/