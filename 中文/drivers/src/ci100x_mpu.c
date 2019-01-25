/**
  ******************************************************************************
  * @文件    ci100x_mpu.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件提供接口函数来控制chipintelli公司的CI100X芯片的MPU单元.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */  

#include "ci100x_mpu.h"


static unsigned int mpu_region_base_address(int start)
{
    return start >> 5;
}

static unsigned long prvGetMPURegionSizeSetting( unsigned long ulActualSizeInBytes )
{
    unsigned long ulRegionSize, ulReturnValue = 4;

	/* 32 is the smallest region size, 31 is the largest valid value for
	ulReturnValue. */
	for( ulRegionSize = 32UL; ulReturnValue < 31UL; ( ulRegionSize <<= 1UL ) )
	{
		if( ulActualSizeInBytes <= ulRegionSize )
		{
			break;
		}
		else
		{
			ulReturnValue++;
		}
	}

	return ulReturnValue;
}

/**
  * @功能: mpu 读region number
  * @注意: 用来判断mcu是否支持MPU
  * @参数: 无
  * @返回值:region number
  */
unsigned char mpu_get_DRegion_number()
{
    return MPU->type.d_region;
}

/**
  * @功能:mpu mem配置
  * @注意:无        
  * @参数: region:区号(MPU_REGION0~MPU_REGION7);baseAddress:区域基址;
  *        size: MPU保护区大小;access:访问权限
  * @返回值:无
  */
void mpu_mem_setup(unsigned int region,int baseAddress,int size,int access)
{
    if(MPU->type.d_region != 8) return;
    if(region >= 8) return;
    
    MPU->ctrl.enable                = MPU_DISABLE;
    MPU->region_base_address.addr   = mpu_region_base_address(baseAddress);
    MPU->region_base_address.region = region;
    MPU->region_base_address.valid  = 1;
    
    MPU->region_attribute.size          = prvGetMPURegionSizeSetting(size);
    MPU->region_attribute.sub_region    = 0;    
    MPU->region_attribute.buff_enable   = MPU_ENABLE;
    MPU->region_attribute.cach_enable   = MPU_ENABLE;
    MPU->region_attribute.share_enable  = MPU_ENABLE;
    MPU->region_attribute.type_extend   = 0;
    MPU->region_attribute.access_permit = access;
    MPU->region_attribute.xn            = MPU_XN_ENABLE;
    MPU->region_attribute.enable        = MPU_ENABLE;
    
    MPU->ctrl.privdef_enable        = MPU_ENABLE;
    MPU->ctrl.hf_nmi_enable         = MPU_DISABLE;
    MPU->ctrl.enable                = MPU_ENABLE;
}

/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/