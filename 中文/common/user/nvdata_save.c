#include "includes.h"

#define SPI_WRNVDATA_DEBUGEN   	1//enable debug 信息

#define SPI_NVDATA_START    	SPI_NVDATA_START_ADDR//spi flash的最后64 0x800000-64*1024,每份NVDATA 256byte，供256份循环使用
#define SPI_NVDATA_PAGE_NUM     (1)
#define SPI_NVDATA_PERSIZE 		(256*SPI_NVDATA_PAGE_NUM)
#define SPI_NVDATA_NUMS    		((SPI_NVDATA_SIZE)/SPI_NVDATA_PERSIZE) //128k 空间做保存信息
#define SPI_NVDATA_WRITED_FLAG 	0xAAAA5555
#define CI_NVDATA_LEN           (SPI_NVDATA_PERSIZE-8)
///////////////////////
int nvdata_index;
char nvdata[SPI_NVDATA_PERSIZE] @ NVDATA_BUF_ADDR;
////////////////////////
extern void SpiFlash_PageProgram(SPICInit_Typedef* QSPICx,char* buf,unsigned int page_start_addr);
extern void SpiFlash_ReadData(SPICInit_Typedef* QSPICx,unsigned int addr,char *buf,unsigned int size);
extern void SpiFlash_Erase(SPICInit_Typedef* QSPICx,SpiFlash_CmdCode erase_type_code,unsigned int addr);
extern u32 checksum_char(u8 *data, u32 length, u32 init);

#if NVDATA_SAVEENABLE
void nvdata_init()
{
	int i;
	char readdata[4];
    
    if(SpiWrMutex!=NULL)
    xSemaphoreTake(SpiWrMutex, portMAX_DELAY);
    
	for(i=0;i<SPI_NVDATA_NUMS;i++)
	{
		SpiFlash_ReadData(QSPIC,SPI_NVDATA_START+SPI_NVDATA_PERSIZE*i,readdata,sizeof(int));
		if(*(unsigned int *)readdata !=0xAAAA5555)
		{
			break;
		}
	}

	if(i==0)
	{
		nvdata_index = SPI_NVDATA_NUMS - 1;
	}
	else 
	{
		nvdata_index = i - 1;
	}
	
#if SPI_WRNVDATA_DEBUGEN
	mprintf("@spi last wNVDATA index=%d,addr =%x@\r\n",nvdata_index,SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index);
#endif	
    memset(nvdata,0,SPI_NVDATA_PERSIZE);

    if(SpiWrMutex!=NULL)
    {
        xSemaphoreGive(SpiWrMutex);
    }
}


int nvdat_read(char* buf,unsigned char lenth)
{
    unsigned int *ptr;
	unsigned int crc_clac;
	int init_flag=1;	

    if(SpiWrMutex!=NULL)
    {
        xSemaphoreTake(SpiWrMutex, portMAX_DELAY);
    }

    ptr = nvdata;
	//flashtosdram((unsigned int)buf,SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index,256);//one page
	SpiFlash_ReadData(QSPIC,SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index,(char *)nvdata,SPI_NVDATA_PERSIZE);

	if(lenth>=(CI_NVDATA_LEN))
	{
		mprintf("lenth must < CI_NVDATA_LEN,err return\r\n");		
		return -2;
	}
	
	if(*ptr == 0xaaaa5555)      
	{
		crc_clac = checksum_char((unsigned char *)(&nvdata[8]),lenth,0);	
		if(crc_clac != *(ptr+1))
		{	
			mprintf("crc_clac=%x,nvdata_save.crc=%x\r\n",crc_clac,*(ptr+1));
			mprintf("spi readNVDATA crc err --^..^!!!，init all NVDATAs\r\n");
			//memcpy((void *)(&buf),nvdata,lenth);
			init_flag = -1;
		}else
		{
#if SPI_WRNVDATA_DEBUGEN
			mprintf("@read spi NVDATA%d  crc=%x\r\n",nvdata_index,*(ptr+1));
#endif
			memcpy(buf,&nvdata[8],lenth);
			init_flag = 0;
		}			
	}
    else
    {
        init_flag = -1;
    }
    
    if(SpiWrMutex!=NULL)
    {
        xSemaphoreGive(SpiWrMutex);    
    }
	return init_flag;
/*
	if(init_flag)
	{//first use spi flash,init nvdata_save,then write NVDATA
#if SPI_WRNVDATA_DEBUGEN		
		mprintf("first use spi,or err writeFlag, init all NVDATAs\r\n\r\n");
#endif
		NVDATA_default_setting();
	}
	*/
	
}
void nvdata_write(char* buf,unsigned char lenth)
{	
	int *ptr;
	ptr = nvdata;
    int crc;

 
	if(lenth>=CI_NVDATA_LEN)
	{
		mprintf("lenth must < CI_NVDATA_LEN ,err return\r\n");		
		return ;
	}
    
    if(SpiWrMutex!=NULL)
    {
        xSemaphoreTake(SpiWrMutex, portMAX_DELAY);
    }
    
	if(nvdata_index<(SPI_NVDATA_NUMS-1))
	{
		nvdata_index ++;
	}
	else
	{
#if SPI_WRNVDATA_DEBUGEN
		mprintf("@NVDATAindex=255,next earse64k spi\r\n");
#endif		
		SpiFlash_Erase(QSPIC,SpiFlash_CmdCode_BlockErase64k,SPI_NVDATA_START);
		nvdata_index = 0;
	}

    memset((char*)(nvdata+8),0,SPI_NVDATA_PERSIZE-8);
    memcpy((char*)(nvdata+8),buf,lenth);
    crc = checksum_char((unsigned char *)(&nvdata[8]),lenth,0);
    
	*ptr++ = 0xaaaa5555;
	*ptr++ = crc;
       
	
#if SPI_WRNVDATA_DEBUGEN
	mprintf("@write spi NVDATA%d crc=%x\r\n",nvdata_index,crc);
#endif	
	//sdramtoflash(SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index,(unsigned int)(&nvdata_save),256);//one page	
	//SpiFlash_PageProgram(QSPIC,	(char *)(&nvdata),SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index);//
	SpiFlash_SequencePageProgram(QSPIC,(char *)(&nvdata),SPI_NVDATA_START+SPI_NVDATA_PERSIZE*nvdata_index,SPI_NVDATA_PAGE_NUM);//

    if(SpiWrMutex!=NULL)
    {
	    xSemaphoreGive(SpiWrMutex);
    }
}
#else
void nvdata_write(char* buf,unsigned char lenth)
{
    (void)buf;
    (void)lenth;
}
int nvdat_read(char* buf,unsigned char lenth)
{
}
void nvdata_init(void)
{
}

#endif
