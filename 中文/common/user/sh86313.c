#include "includes.h"
#include "user_main.h"

static void wait_ms(int ms)
{
    int i=0;
    while(ms--)
    {
        for(i=0;i<0x1A80;i++);
    }
}
static void delay(int counter)
{
    while(counter--);
}
static void mp3Sendbyte(unsigned char byte)
{

    uint8_t j=8;
    MP3CSNClr;
	delay(200);
	while(MP3SS==0);
	MP3CSNSet;
	
    MP3GDOSet;
    MP3TXClr;   
    wait_ms(3);
    while(j--)
    {
 
        if(byte&0x01)
            MP3TXSet;
        else MP3TXClr;
        MP3GDOInv;
        wait_ms(3);
        byte=byte>>1;
    }

    MP3TXSet;
    MP3GDOSet;
    wait_ms(3);	
}
static void snd2mp3(unsigned char *buffer,unsigned char dat_lenth)
{
    unsigned char temp_cnt,check_uart4;
	
//    MP3CSNClr;
//	delay(200);
//	MP3CSNSet;
	for(temp_cnt=0;temp_cnt<dat_lenth;)
	{
	    mp3Sendbyte(*(buffer+temp_cnt));
		if(temp_cnt==1)
		{
		    check_uart4 = 0xFF; 
		}
		else if(temp_cnt>1)
		{
		    check_uart4 = check_uart4^*(buffer+temp_cnt);
		}		  
		temp_cnt += 1;
		wait_ms(10);
	  }
//	MP3CSNClr;
//	delay(200);
//	MP3CSNSet;
    mp3Sendbyte(check_uart4);
}
void mp3init(void)
{
    unsigned char cmdbuf[6]={0xaa,0x55,0x00,0x00,0x00,0x00};
	cmdbuf[1] = 0xCC;
	cmdbuf[2] = (11<<4)|1;
	snd2mp3(cmdbuf,3);
	while(MP3SS)
	{
         snd2mp3(cmdbuf,3); 
		 vTaskDelay(50/portTICK_PERIOD_MS);
	}
}
void cmd2mp3(unsigned char playcmd,uint16_t filenum)
{
    unsigned char cmdbuf[6]={0xaa,0x55,0x00,0x00,0x00,0x00};
	cmdbuf[2]=playcmd;
	cmdbuf[3]=(uint8_t)(filenum>>8);
	cmdbuf[4]=(uint8_t)filenum;
	snd2mp3(cmdbuf,5);
	vTaskDelay(50/portTICK_PERIOD_MS);
	while(MP3SS)
	{
         snd2mp3(cmdbuf,5); 
		 vTaskDelay(50/portTICK_PERIOD_MS);
	}
}