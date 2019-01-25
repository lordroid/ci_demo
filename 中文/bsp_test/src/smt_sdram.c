#include "smt_sdram.h"
#include <stdlib.h>
#include <string.h>
#include "ci100x_scu.h"

extern unsigned int GetCurrentUs(void);
void sdram_algo(void)
{
     srand(2);
     GetCurrentUs( );
     int i=0;
     for(i =0;i<4*1024*1024;i+=4)
     {
        *(volatile unsigned int*)(0x70000000+i) = rand();
     }
     mprintf("start:\n");
     GetCurrentUs();
     for(i =0;i<4*1024*1024;i+=1)
     {
        *(volatile unsigned char*)(0x70000000+i) += 0x69;
     }
     mprintf("\nend\n");
     GetCurrentUs();
}

void sdram_test(void)
{
    unsigned int tmp = 0,i;
    for(i = 0; i < 16 * 1024 * 1024; i += 4)
    {
      *(volatile unsigned int*)(0x70000000+i) = i;
    }
    for(i = 0; i < 16 * 1024 * 1024; i += 4)
    {
      tmp=*(volatile unsigned int*)(0x70000000+i);
      if(i != *(volatile unsigned int*)(0x70000000 + i))
      {
        mprintf("i=%x  rd:%x %x sdram failed\n",i,tmp,0x70000000+i);
        //break;
      }
    }
    mprintf("cpu word 访问完成！\n");
    for(i = 0; i < 16 * 1024 * 1024; i += 2)
    {
      *(volatile unsigned short*)(0x70000000+i) = (unsigned short)i;
    }
    for(i = 0; i < 16 * 1024 * 1024; i += 2)
    {
      tmp=*(volatile unsigned short*)(0x70000000+i);
      if((unsigned short)i != *(volatile unsigned short*)(0x70000000 + i))
      {
        mprintf("i=%x  rd:%x %x sdram failed\n",(unsigned short)i,tmp,0x70000000+i);
        //break;
      }
    }
    mprintf("cpu half word 访问完成！\n");
        for(i = 0; i < 16 * 1024 * 1024; i += 1)
    {
      *(volatile unsigned char*)(0x70000000+i) = (unsigned char)i;
    }
    for(i = 0; i < 16 * 1024 * 1024; i += 2)
    {
      tmp=*(volatile unsigned char*)(0x70000000+i);
      if((unsigned char)i != *(volatile unsigned char*)(0x70000000 + i))
      {
          mprintf("i=%x  rd:%x %x sdram failed\n",(unsigned char)i,tmp,0x70000000+i);
            //break;
      }
    }
    mprintf("cpu  byte 访问完成！\n");
    
    
/*
    for(unsigned int i = 0; i < 8 * 1024 * 1024; i += 4)
    {
      *(volatile unsigned int*)(0x70000000 + i + 8 * 1024 * 1024) = i;
    }
    for(unsigned int i = 0; i < 8 * 1024 * 1024; i += 4)
    {
      if(i != *(volatile unsigned int*)(0x70000000 + i + 8 * 1024 * 1024))
      {
        mprintf("%x sdram failed\n",0x70000000+i+8*1024*1024);
        //break;
      }
    }
*/
}

void sdram_model2(void)
{
    Unlock_Ckconfig();
    SCU->SYS_CTRL &=  ~(1<<12);
    SCU->SYS_CTRL |= (1<<18);
    
    Lock_Ckconfig();
    
    /**/
    sdram_init();
    sdram_test();
}

void sdram_model3(void)
{
    Unlock_Ckconfig();
    SCU->SYS_CTRL &=  ~(1<<12);
    SCU->SYS_CTRL &= ~(1<<18);
    
    Lock_Ckconfig();
    
    Scu_Setdiv_Parameter(AHB_BASE,0X1);
    Scu_SetClkDiv_Enable(AHB_BASE);
    /*96Mhz*/
    /**/
    sdram_init();
    sdram_test();
}

void sdram_selfrefresh(void)
{
    /*SDRAM自刷新*/
    *(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc) |= (0x1 << 1);
    while(((*(volatile unsigned int *)(SDRAM_CTRL_BASE + 0xc))&(0x1 << 11)) == 0);
    /*关闭SDRAM CLK*/
    *(volatile unsigned int *)(0x40010000 + 0x44) = 0x51AC0FFE;
    while((*(volatile unsigned int *)(0x40010000 + 0x44))!=1);
    *(volatile unsigned int *)(0x40010000 + 0x28) &= ~(0x1 << 25);
    *(volatile unsigned int *)(0x40010000 + 0x44) = 0;
}
