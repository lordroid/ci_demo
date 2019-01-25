#include "ci100x_mpu.h"
#include "ci100x_uart.h"
#include <string.h>
static char mpu_addr_test[256] = {0};
void ARMCM4_MPU_test(void)
{
    unsigned char number;
    number = mpu_get_DRegion_number(); 
    if(!number)
    {
        mprintf("Not Supported MPU");
        return ;
    }
    //unsigned int startAddr = 0x20010000;
    //unsigned int size = 0x1000;
    memset(mpu_addr_test,0,sizeof(mpu_addr_test));
    mpu_mem_setup(  MPU_REGION0,
                    (unsigned int)mpu_addr_test,
                    64,
                    MPU_RW_RONLY); 
    mprintf("Write pro only");
    for(int i =0 ;i<sizeof(mpu_addr_test)/sizeof(mpu_addr_test[0]);i++)
    {
        mpu_addr_test[i] = i;
    }
    mprintf("Read pro only data");
    for(int i =0 ;i<sizeof(mpu_addr_test)/sizeof(mpu_addr_test[0]);i++)
    {
       // mpu_addr_test[i] = i;
    }
    while(1);
}




#if( CONSOLE_ENABLE == 1)
#include <stdlib.h>
#include <string.h>
#include "ci100x_mpu.h"
#include "smt_assert.h"
#include "utils.h"

static int      cReadOnlyArray[64];

void get_control_reg(int *reg)
{
    __asm volatile
    (
        " MRS r1,control        \n"
        " str r1,[r0]           "
    );
};
void mpu_test(int startAddr,int size)
{
    unsigned char number;
    unsigned int  i;   
    int  control_reg=0;
    
    mprintf("mpu zone: start addr 0x%08x,size=%d bytes,readonly\n",startAddr,size);
    
    get_control_reg(&control_reg);
    //mprintf("control_reg=0x%08x\n",control_reg);

    number  = mpu_get_DRegion_number();    
    if(!number)
    {
        mprintf("Not supported!\r\n");
        return;
    }
    
    mpu_mem_setup(  MPU_REGION0,
                    startAddr,
                    size,
                    MPU_RONLY_RONLY); 
    mprintf("read from 0x%08x:\r\n",startAddr);
    dma_dump_buffer("",(void *)startAddr,size/sizeof(int),sizeof(int));
    
    mprintf("start write data to 0x%08x ...\r\n",startAddr);
    for(i=0;i<size/sizeof(int);i++)
    {
        *(int *)(startAddr + 4*i)   = 0xa5a5a5a5;
    }
    for(i=0;i<size/sizeof(int);i++)
    {
        if( *(int *)(startAddr + 4*i) != 0xa5a5a5a5 )
        {
            mprintf("error,cReadOnlyArray[%d]=0x%08x\r\n",i,*(int *)(startAddr + 4*i));
            break;
        }
    }
    mprintf("mpu test end!\n");
}

static void shell_mpu_test_help()
{
     mprintf( "Usage: mpu [startAddr] [size]\n" );                
     mprintf("\texample :  mpu 0x20010000 0x1000\n");  
}

static int shell_mpu_test(int argc,char *argv[])
{
    unsigned int      startAddr;
    unsigned int      size;
    if(argc == 1)
    {
        startAddr   = (unsigned int)&cReadOnlyArray[0];
        size        = 64*sizeof(int);
        mpu_test(startAddr,size);
        return 0;
    }
    if( argc < 3 )
        goto shell_mpu_test_error;
    StrToNumber(argv[1],&startAddr);
    //StrToNumber(argv[1],&size);
    size        = 64*sizeof(int);
    mpu_test(startAddr,size);
    return 0;   
shell_mpu_test_error:
    shell_mpu_test_help();
    return 0;
}

int shellApp_mpu(int argc, char *argv[])
{
    shell_mpu_test(argc,argv);
    return 0;
}
#endif