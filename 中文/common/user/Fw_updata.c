#include "includes.h"


extern void SpiFlash_SequencePageProgram(SPICInit_Typedef* QSPICx,char* buf,unsigned int page_start_addr,unsigned int pages);
extern void Scu_SoftwareRst_System(void);
extern void sdramtoflash(unsigned int flashaddr,unsigned int sdramaddr,unsigned int size);
extern void AT_send(char *buffer,int lenth);
extern u32 checksum_char(u8 *data, u32 length, u32 init);

void disable_vadasr()
{

    NVIC_DisableIRQ(VAD_IRQn);

    NVIC_DisableIRQ(ASR_IRQn);
    
	IIS_EN(IIS0,DISABLE);
    IIS_EN(IIS1,DISABLE);
    //Scu_SetDeviceGate((unsigned int)IIS0,DISABLE);//有时语音采集可能会测试0x7050000~0x70600000 的ram，升级时关闭采音
	//Scu_SetDeviceGate((unsigned int)IIS1,DISABLE);
}


#define ADDR_FW_UPDATA_START 0x70580000
#define FW_SPIADDR	0x3000
#define FW_SIZE  100*1024
#define UPDATA_DBG 1
int user_code_updata()
{
#if 0
	char txbuf[32];
	int lenth,i;
	int updata_size;
	int cnt, timeout=0;
	unsigned int chksum;
	unsigned int *ptr;
	unsigned int partA_flag,partB_flag;
	unsigned int addr_spiupdata;

	int resetFlag;
	
	resetFlag = 0;
	vTaskSuspendAll();
	disable_vadasr();

	memset((void *)ADDR_FW_UPDATA_START,0,512*1024);	
	flag_updata = 1;
	
	lenth=sprintf(txbuf,"AT+OK\r\n");
	AT_send(txbuf,lenth);	
	
	mprintf("ready updata");

    while(1) //timout 
    {
    	DelayMs(10);
        if(flag_updata >= FW_SIZE)
        {           	
			ptr = (unsigned int *)(ADDR_FW_UPDATA_START+FW_SIZE-8);// 指向数据的标志和checksum位置
            chksum=checksum_char((unsigned char *)ADDR_FW_UPDATA_START,FW_SIZE-8,0);
mprintf("---<1>---chkcalc=%x,read chk=%x\r\n",chksum,*(ptr+1));			
			if(chksum == *(ptr+1))//待升级的文件的checksum 与待升级文件的checksum一致
			{	
				// 比较哪份code比较旧，更新旧版本的程序，擦除旧版本的code区域
				SpiFlash_ReadData(QSPIC,(FW_SPIADDR+FW_SIZE-8),(char *)(&partA_flag),sizeof(int));
				SpiFlash_ReadData(QSPIC,(FW_SPIADDR+FW_SIZE*2-8),(char *)(&partB_flag),sizeof(int));

			/*	SpiFlash_ReadData(QSPIC,(FW_SPIADDR+FW_SIZE-8),bufA,sizeof(int));
				SpiFlash_ReadData(QSPIC,(FW_SPIADDR+FW_SIZE*2-8),bufB,sizeof(int));
				partA_flag = *((unsigned int*)bufA);
				partB_flag = *((unsigned int*)bufB);*/
				
//----如果擦除后掉电未升级文件成功，则check和abflag都是错的，此处可以根据abflag来直接确定运行那份code 	
mprintf("---<2>---partA_flag=%x,partB_flag=%x\r\n",partA_flag,partB_flag); 		

				if(partB_flag<=partA_flag)
				{//b程序较旧，updata B程序
					addr_spiupdata = FW_SPIADDR+FW_SIZE;
					*ptr = partA_flag+1;//标记要更新的程序的flag
				}
				else
				{
					addr_spiupdata = FW_SPIADDR;	
					*ptr = partB_flag+1;//标记要更新的程序的flag
				}
mprintf("---<3>---partA_flag=%x,partB_flag=%x,newFlag=%x,addr=%x\r\n",partA_flag,partB_flag,*ptr,addr_spiupdata);		
				
				//擦除flash的指定区域

				updata_size = FW_SIZE;			
				cnt = updata_size/4096;
									
				for(i=0;i<cnt;i++)
				{
					SpiFlash_Erase(QSPIC,SpiFlash_CmdCode_SectorErase4k,addr_spiupdata+4096*i);
				}
mprintf("---<4>--- erase ok,write at addr%x,flag%x,crc%x\r\n",addr_spiupdata,*ptr,*(ptr+1));		

				//write to spi	
				sdramtoflash(addr_spiupdata,(unsigned int )ADDR_FW_UPDATA_START,FW_SIZE);				
				

				//read code and check
				memset((void *)ADDR_FW_UPDATA_START,0,512*1024);
				flashtosdram((unsigned int )ADDR_FW_UPDATA_START,addr_spiupdata,FW_SIZE);
				
				ptr = (unsigned int *)(0x70580000+FW_SIZE-8);// 指向数据的标志和checksum位置
	            chksum=checksum_char((unsigned char *)0x70580000,FW_SIZE-8,0);
mprintf("---<5>--- check ,read addr%x,flag%x,crc%x,check_calc=%x\r\n",addr_spiupdata,*ptr,*(ptr+1),chksum);						
				if(chksum == *(ptr+1))//升级后的文件的checksum 与待升级文件中的checksum一致
				{
mprintf("---<6>--- check ok ,then reset system\r\n");										
					lenth=sprintf(txbuf,"Updata sucessfull!");
					AT_send(txbuf,lenth);
					resetFlag = 1;
                    break;
				}else							
				{
					lenth=sprintf(txbuf,"spi write check err!");
					AT_send(txbuf,lenth);
					break;
				}
			}else
			{
				lenth=sprintf(txbuf,"Updata check err!");
				AT_send(txbuf,lenth);
				break;
			}
        }
		if(timeout++ > 60000)//超过60s timeout
		{
			lenth=sprintf(txbuf,"Updata timeout!");
			AT_send(txbuf,lenth);
			break;
		}		
    }
	if(resetFlag)
	{
		DelayMs(200);
		Scu_SoftwareRst_System();
        while(1)
        DelayMs(200); 
	}	
	
	if(!xTaskResumeAll())
	{
		taskYIELD();
	}
#endif  
    return 0;
}
/////////////////////////
static int flag_updata=0;
extern int tf_mounted_state;
int user_code_updata_fromTF()//
{
#if 1
//	char txbuf[32];
//	int lenth
    int i;
//	int updata_size;
//	int cnt, timeout=0;
	unsigned int chksum1,chksum2;
	char *ptr = NULL,*ptr_start=NULL;
//	unsigned int partA_flag,partB_flag;
//	unsigned int addr_spiupdata;

	volatile int resetFlag=0;
	//vTaskSuspendAll();
	//disable_vadasr();

///////////////
	FATFS fs;
	DIR DirInf;
	FRESULT result; 
	FIL    pUpFile;
	unsigned long     Upfilesize = 0;
	unsigned int br;

    
    if(tf_mounted_state==FR_OK)
        f_mount(FS_SD,NULL);
    
    //mprintf("start mount SD!\n");
    tf_mounted_state = f_mount(FS_SD, &fs);
    if (tf_mounted_state != FR_OK)
    {
        mprintf("mount failed:\r\n");
		return -1;
    } 
    mprintf("mount OK!\n");
    result = f_opendir(&DirInf, "/");
    if (result != FR_OK)
    {
        mprintf("opendir failed:");
		return -1;
    }else
    {
        mprintf("opendir OK");
    }    
      
/////////
	flag_updata = 0;
	

	for(i=0;i<5;i++)
	{
        
        
        result = f_open(&pUpFile, "CI_updata.bin", FA_READ);
        if (FR_OK!=result) 
        {
            mprintf("failed to open CI_updata.bin!\n");
			if(i==4)
				return -1;
			else
	            continue;
        }else 
        {
            mprintf("open ok at CI_updata.bin!,size=%x\n",pUpFile.fsize);
        }
        
        ptr_start = (char *)(SDRAM_REC_BUFFER_ADDR);
        
        
        ptr = ptr_start;
        mprintf("<1> read file to sdram\r\n");
        Upfilesize = 0;
        while(Upfilesize<pUpFile.fsize)
        {
            result = f_read(&pUpFile, (void *)ptr, 512, &br);
            if(FR_OK == result)
            {  
                ptr+=512;
                Upfilesize+=512;
            }else
            {
                mprintf("f_read err,Upfilesize=%d, retry \r\n",Upfilesize);
            }
        }
        
        mprintf("ptr=%x,pUpFile.fsize=%x\r\n",ptr,pUpFile.fsize);

        
        mprintf("<2> count chksum\r\n");
        ptr = ptr_start;
        chksum1=checksum_char((unsigned char *)(ptr),pUpFile.fsize,0);
        mprintf("            read file ok,code chksum=%x\r\n",chksum1);
      
      
        mprintf("<3>erase spi \r\n");
	    SpiFlash_Erase(QSPIC,SpiFlash_CmdCode_ChipErase,0);

        mprintf("<4>write to spi \r\n");
		sdramtoflash(0,(unsigned int )ptr,pUpFile.fsize);				
		
        mprintf("<4>read and check \r\n");
		//read code and check
		memset((void *)ptr,0,Upfilesize);
		flashtosdram((unsigned int )ptr,0,pUpFile.fsize);

		chksum2=checksum_char((unsigned char *)ptr,pUpFile.fsize,0);
        
        f_close(&pUpFile);
        mprintf("read file ok code chksum1=%x,chksum1=%x\r\n",chksum1,chksum2); 
       
		if(chksum1==chksum2)
		{		
			flag_updata = 1;
			break;
		}else
		{
			mprintf("sorry , updata err,please wait retry!");
		}
	}
	
	if(flag_updata)
		mprintf("updata OK GoodLuck!\r\n");
	else
		mprintf("updata err,So so so so sorry~!\r\n");		

	
	DelayMs(200);
    for(i=0;i<10;i++)
    {  
      LED_TEST_OFF;
      DelayMs(200); 
      LED_TEST_ON;
      DelayMs(200); 
	  LED_TEST_OFF;
      DelayMs(200); 
	  LED_TEST_ON;
      DelayMs(1200); 
		
	}
	Scu_SoftwareRst_System();
    while(1)
      DelayMs(200); 
	
#endif    
}

