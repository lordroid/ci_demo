#include "ci100x_sd.h"
 
#include "ci100x_malloc.h"
#include "ci100x_uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if 0

/**
  * @功能:读取SD中的文件，把*.pcm 或者*.wav 保存到一个链表里面
  * @注意:无       
  * @参数:pcmlistfile 文件名
  * @返回值:链表
  */
FATFS pcmlst_fs;
DIR pcmlst_DirInf;
FIL PCMlst_file;
FIL SDPCMfile,PCM_voiceFile,VAD_logfile;
FATFS fs;
DIR DirInf;
#if 0
PCM_List GetPcmList(char *pcmlistfile)
{
    PCM_List ret;
    Note * pcur,*pnext,*prev = NULL,*phead;
    phead =(Note *)ci100x_malloc(sizeof(Note)) ;
    if((!pcmlistfile) ||(!phead))
    {
        ret.head =NULL;
        ret.notes =0;
        return ret;
    }
    memset(phead,0,sizeof(Note));
    phead->prv =NULL;
    strcpy(phead->name,"HEAD");
    prev = phead;
    ret.head  = phead;
    ret.notes = 0;
    
    FRESULT result;
    char fileName[50] ={"pcm.lst"};
    strcpy(fileName,pcmlistfile);
    result = f_mount(FS_SD, &pcmlst_fs);
    if (result != FR_OK)
    {
        mprintf("mount failed:");
    }
    result = f_opendir(&pcmlst_DirInf, "/");
    if (result != FR_OK)
    {
        mprintf("opendir failed:");
    }
    result = f_open(&PCMlst_file, fileName, FA_READ);
    if(result != FR_OK)
    {
       mprintf("open failed:");
    }else 
    {
       mprintf("open success:%s\r\n",fileName);
    }
    while(!f_eof(&PCMlst_file))
    {
        char line[50]={0};
        memset(line,0,50);
        f_gets(line,50,&PCMlst_file);
        if(strstr(line,".pcm")||strstr(line,".wav"))
        {
            for(int i =0;line[i];i++)
            {
                if((line[i]=='\n')||(line[i]=='\r'))
                {
                    line[i]=0;
                    break;
                }
            }
            pcur = (Note *)ci100x_malloc(sizeof(Note)) ;
            if(!pcur)
            {
                f_close(&PCMlst_file);
                f_mount(FS_SD, NULL);/*umount*/
                mprintf("malloc failed:\n");
                return ret;
            }
            memset(pcur,0,sizeof(Note));
            strcpy(pcur->name,line);
            prev->pnext = pcur;
            pcur->prv = prev;
            pcur->pnext = NULL;
            prev = pcur;
            ret.notes++;
        }
    }
    f_close(&PCMlst_file);
    result = f_mount(FS_SD, NULL);/*umount*/
    return ret;
}
#endif

extern void i2s0rx_recordSD(void);


#define FILE_MAX_2G 	2*1024*1024*1024 
#define FILE_MAX_1G 	1*1024*1024*1024 
#define FILE_MAX_256M 	256*1024*1024 
#define FILE_MAX_512M 	512*1024*1024 
#define FILE_MAX_1M 	1*1024*1024 
#define FILE_MAX_11M 	11*1024*1024 
#define FILE_MAX_512K 	512*1024 


#define ONE_FILE_SIZE    FILE_MAX_1M

void  WriteReadSDPCM(void*pvParameters)
{
	FRESULT result;
    unsigned int bw;
    
    char fileName[50] ={"cx20921.pcm"};
    result = f_mount(FS_SD, &fs);
    if (result != FR_OK)
    {
        mprintf("mount failed:");
    }
    result = f_opendir(&DirInf, "/");
    if (result != FR_OK)
    {
        mprintf("opendir failed:");
    }
   
    i2s0rx_recordSD();
	
    char flag = 0;
    int size = 0;
	int NUM;
    int i,cnt;
    for(i=0;i<512;i++)
    {		
        sprintf(fileName,"rec_%d.pcm",i);
        result =f_open(&SDPCMfile, fileName, FA_READ);
         if(result != FR_OK)
         {
            NUM = i;
            mprintf("open %s fail,so Fixnum %d ",fileName,i);
            break;
         }else
         {
             result = f_close(&SDPCMfile);
             if(result != FR_OK)
             {
                mprintf( "!!! close file somthing err:please reset!\r\n");
             }
         }
    }
	
    sprintf(fileName,"rec_%d.pcm",NUM);
    result = f_open(&SDPCMfile, fileName, FA_CREATE_ALWAYS|FA_WRITE  );
    if(result != FR_OK)
    {
       mprintf("open failed:");
    }else 
    {
       mprintf("open success:%s\r\n",fileName);
    }

    for(;;)
    {
        xSemaphoreTake(xVadpcmSemaphore,portMAX_DELAY);
        m_fwrite(&SDPCMfile,(void*)(0x70500000+ flag*16*1024),16*1024,&bw);
        size +=16*1024;
        flag =!flag;
     
        if(size%(128*1024) == 0)
        {
            result = f_close(&SDPCMfile);
            if(result != FR_OK)
            {
               mprintf("close failed:");
            }else 
            {
               mprintf("close success:%s\r\n",fileName);
            }
            
             if(size >= ONE_FILE_SIZE)
            {
                NUM++;
              
                sprintf(fileName,"rec_%d.pcm",NUM);
  
                
                result =f_open(&SDPCMfile, fileName, FA_CREATE_ALWAYS|FA_WRITE);
                if(result != FR_OK)
                {
                    mprintf("f_open failed:%s<><><><><><><>--reopen\r\n",fileName);
                    result =f_open(&SDPCMfile, fileName, FA_CREATE_ALWAYS|FA_WRITE);
                    if(result != FR_OK)
                    { 
                        mprintf("f_open failed:%s<><><><><><><>--reopen failed again,while(1)\r\n",fileName);
                        {
                            while(1)
                            {
                                //GPIO_DATA_Set(GPIO1,GPIO_Pin19,!GPIO_ReadDATA(GPIO1,GPIO_Pin19));
                                vTaskDelay(80);
                                mprintf("x");
                            }
                        }
                    }
                }else 
                {
                    mprintf("f_open success:%s\r\n",fileName);
                }      
                
                size =  0;
            }else
            {
            
                vTaskDelay(1);
                mprintf("re open %s",fileName);
                result = f_open(&SDPCMfile, fileName,  FA_WRITE  );
                if(result != FR_OK)
                {
                    mprintf("re open %s",fileName);
                }

                f_lseek(&SDPCMfile,size);
            }
        }
         
    }
 
}
#endif