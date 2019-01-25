//---this is a demo code ,used at tf record pcm 
FATFS fs;
DIR DirInf;       
FIL PCM_voiceFile,VAD_logfile,CorpusFile,NoiseFile;   


#define FILE_MAX_2G     2*1024*1024*1024 
#define FILE_MAX_1G     1*1024*1024*1024 
#define FILE_MAX_256M     256*1024*1024 
#define FILE_MAX_512M     512*1024*1024 
#define FILE_MAX_100M     100*1024*1024 
#define FILE_MAX_50M     50*1024*1024 
#define FILE_MAX_1M     1*1024*1024 
#define FILE_MAX_5M     5*1024*1024 
#define FILE_MAX_11M     11*1024*1024 
#define FILE_MAX_20M     20*1024*1024 
#define FILE_MAX_512K     512*1024 


#define ONE_FILE_SIZE    FILE_MAX_100M

int Get_NextInitnum(char *filename_head)
{
    char filename[120]={0};
    int i,result;
    FIL filehandle;
    
    for(i=0;i<512;i++)
    {        
        sprintf(filename,"%s%d.pcm",filename_head,i);
        result =f_open(&filehandle, filename, FA_READ);
        if(result != FR_OK)
        {
            mprintf("open %s fail,so Fixnum %d ",filename,i);
            break;
        }else
        {
            result = f_close(&filehandle);
            if(result != FR_OK)
            {
                mprintf( "!!! close file somthing err:please reset!\r\n");
            }
        }
    }
    return i;
}

void vTaskFileFATfsDealWith(void*pvParameters)
{
    FRESULT result;
    //char vadlog[50]={0};
    char record[120]={0};
    
        int LED_Flash_cnt;
    int LED_Flash_INITNUM;

    LED_Flash_cnt = LED_Flash_INITNUM = 32;//每写32k文件，闪灯变化一下

    
    tf_mounted_state = f_mount(FS_SD, &fs);
    if (tf_mounted_state != FR_OK)
    {
        mprintf("mount failed:");
    }
    result = f_opendir(&DirInf, "/");
    if (result != FR_OK)
    {
        mprintf("opendir failed:");
    }
    
    start_readpcm = 0;
    unsigned int bw;
    
    int pcm_opened_flag = 0;
    int nums;
    
    sprintf(record,"record");
    nums = Get_NextInitnum(record);
    
        
    for(;;)
    {
        {  
            mprintf(" #---"); 
                     
            while(1)
            {
                
                if(!pcm_opened_flag)
                {
                    mprintf("\r\n------<file %d Now>-----\r\n",nums);
                    ////////////////////////////////preset record pcm file
                    sprintf(record,"record%d.pcm",nums);
                    result = f_open(&PCM_voiceFile, record, FA_CREATE_ALWAYS|FA_WRITE);
                    if(result!=FR_OK)
                    {                      
                        mprintf("%s create failed!\n",record);
                        while(1)
                        {
                             if(LED_TEST_STATE)
                            {
                                LED_TEST_OFF;
                            }else
                                LED_TEST_ON;
                             vTaskDelay(50);
                        }
                    }else
                    {
                        f_lseek(&PCM_voiceFile,ONE_FILE_SIZE);//512M
                    }
                    if(FR_OK !=f_close(&PCM_voiceFile))
                    {
                        mprintf("%s close failed\n",record);
                    }
                   ////////////////////////////////preset record log file
                                        
                  /*  sprintf(vadlog,"record%d.txt",nums);
                    result = f_open(&VAD_logfile, vadlog, FA_CREATE_ALWAYS|FA_WRITE);
                    if(result != FR_OK)
                    {
                        mprintf("record.txt create failed!\n");
                    }else
                    {
                        f_lseek(&VAD_logfile,ONE_FILE_SIZE/1024);//
                    }
                   
                    if(FR_OK !=f_close(&VAD_logfile))
                    {
                        mprintf("%s close failed\n",vadlog);
                    }*/
                    //-----open 文件等待写
                    if(FR_OK != f_open(&PCM_voiceFile,record,FA_WRITE))
                    {
                        mprintf("%s reopen failed\n",record);
                    }else
                    {
                        f_lseek(&PCM_voiceFile,0);
                    }
                                        
                /*    if(FR_OK != f_open(&VAD_logfile,vadlog,FA_WRITE))
                    {
                        mprintf("%s reopen failed\n",vadlog);
                    }else
                    {
                        f_lseek(&VAD_logfile,0);
                    }*/
                                         
                    pcm_opened_flag = 1;// open file 
                    start_readpcm  = 1; 
                   // sdfile_cursize = 0;//restart log file

                    
                    if(xSemaphoreTake(xRecSemaphore,portMAX_DELAY) == pdTRUE)
                    {//这里等待中断发个消息出来,以便后面数据的同步
                    }
                }  
           //=========================
                //write pcm file
                if(LOOP_BUF_CNT(pcm_wptr,pcm_rptr,SDRAM_REC_BUFFER_SIZE)> 1024*4)                  
            //    if(xSemaphoreTake(xRecSemaphore_4K,portMAX_DELAY) == pdTRUE) 
                {//    write
                    while(LOOP_BUF_CNT(pcm_wptr,pcm_rptr,SDRAM_REC_BUFFER_SIZE)>2048)
                    {
                        if(FR_OK !=m_fwrite(&PCM_voiceFile,(void *)(SDRAM_REC_BUFFER_ADDR+pcm_rptr),1*1024,&bw))
                        {
                            mprintf("%s write failed\n",record);
                        }
                        pcm_rptr += 1024;
                        if(pcm_rptr >= SDRAM_REC_BUFFER_SIZE)
                        {
                            pcm_rptr = 0;
                        }
                        ///////led flash
                        LED_Flash_cnt--;
                        if(!LED_Flash_cnt)
                        {
                            if(LED_TEST_STATE)
                            {
                                LED_TEST_OFF;
                            }else
                                LED_TEST_ON;
                            LED_Flash_cnt = LED_Flash_INITNUM;
                        } 
                        ////////////////////////
                    }
                    
                    if((&PCM_voiceFile)->fptr>=ONE_FILE_SIZE)
                    {
                        //---- 
                        mprintf("pcm file saved now\r\n");
                        //-------
                        if(FR_OK !=f_close(&PCM_voiceFile))
                        {
                            mprintf("%s close failed\n",record);
                        }  
                       // pcm_opened_flag = 2;//当前的log也存到tf中    
                        pcm_opened_flag = 0;//当前的log也存到tf中   
                        start_readpcm =0;// 这时候采音数据不计入
                        
                        nums++;
                    }                    
                }

                if(xSemaphoreTake(xRecStopSemaphore,2000) == pdTRUE)//
                {//这里等待中断发个消息出来,以便后面数据的同步
                    mprintf("xRecStopSemaphore -> pcm file saved \r\n");
                    if(FR_OK !=f_close(&PCM_voiceFile))
                    {
                        mprintf("%s close failed\n",record);
                    }  
                   // pcm_opened_flag = 2;//当前的log也存到tf中    
                    pcm_opened_flag = 0;//当前的log也存到tf中   
                    start_readpcm =0;// 这时候采音数据不计入                   
                    nums++;
                    
                    while(1)
                    {
                        mprintf("something close rec,please wait ok and restart system!!\r\n");
                        vTaskDelay(5000);
                    }
                }
                //vTaskDelay(2);
                //依据实际测试的写文件速度，vtask delay
#if 0
                //write log file
                if((LOOP_BUF_CNT(wptr_vadlog,rptr_vadlog,VADLOG_SIZE) > 512)  || (pcm_opened_flag==2) )
                { 
                /*    if(FR_OK != f_open(&VAD_logfile,vadlog,FA_WRITE))
                    {
                        mprintf("%s open failed\n",vadlog);
                    }else
                    {
                        f_lseek(&VAD_logfile,(&VAD_logfile)->fsize);
                    }*/
                    if(pcm_opened_flag==2)
                    {
                        int len;
                        len = LOOP_BUF_CNT(wptr_vadlog,rptr_vadlog,VADLOG_SIZE);
                        if(FR_OK !=m_fwrite(&VAD_logfile,(void *)(&vadlog_buf[rptr_vadlog]),len,&bw))
                        {
                            mprintf("%s write failed\n",vadlog);
                        }
                        rptr_vadlog += len;                       
                        wptr_vadlog = rptr_vadlog = 0;    // vadlog buffer 重新初始化
                        pcm_opened_flag = 0; //------本次两个文件保存完成
                        start_readpcm  = 0;// 这时候采音数据不计入
                        
                        if(FR_OK !=f_close(&VAD_logfile))
                        {
                            mprintf("%s close failed\n",vadlog);
                        } 
                    }
                    else
                    {
                        if(FR_OK !=m_fwrite(&VAD_logfile,(void *)(&vadlog_buf[rptr_vadlog]),512,&bw))
                        {
                            mprintf("%s write failed\n",vadlog);
                        }
                        rptr_vadlog += 512;
                        if(rptr_vadlog>=VADLOG_SIZE)                        
                            rptr_vadlog = 0;
                    }            
                }
#endif                

            }
        }
    }
}
////////////////////////////////////////////////采集模式
void WriteReadSDPCM(void*pvParameters)
{
    FRESULT result;
    char record[120]={0};
   
    tf_mounted_state = f_mount(FS_SD, &fs);
    if (tf_mounted_state != FR_OK)
    {
        mprintf("mount failed:");
    }
    result = f_opendir(&DirInf, "/");
    if (result != FR_OK)
    {
        mprintf("opendir failed:");
    }
    start_readpcm = 0;
    unsigned int bw;
    
    int pcm_opened_flag = 0;
    int nums;
    
    sprintf(record,"Corpus");
    nums = Get_NextInitnum(record);

    int LED_Flash_cnt;
    int LED_Flash_INITNUM;

    LED_Flash_cnt = LED_Flash_INITNUM = 32;//每写32k文件，闪灯变化一下
        
    for(;;)
    {
        {  
            mprintf(" #---"); 
                     
            while(1)
            {
                if(!pcm_opened_flag)
                {
                    mprintf("\r\n------<file %d Now>-----\r\n",nums);
                    ////////////////////////////////preset record pcm file
                    sprintf(record,"Corpus%d.pcm",nums);
                    result = f_open(&PCM_voiceFile, record, FA_CREATE_ALWAYS|FA_WRITE);
                    if(result!=FR_OK)
                    {
                        //mprintf("record.pcm create failed!\n");
                        while(1)
                        {
                            if(LED_TEST_STATE)
                            {
                                LED_TEST_OFF;
                            }else
                                LED_TEST_ON;
                            vTaskDelay(50);
                        }
                    }else
                    {
                        f_lseek(&PCM_voiceFile,ONE_FILE_SIZE);//512M
                    }
                    if(FR_OK !=f_close(&PCM_voiceFile))
                    {
                        mprintf("%s close failed\n",record);
                    }
                          
                    //-----open 文件等待写
                    if(FR_OK != f_open(&PCM_voiceFile,record,FA_WRITE))
                    {
                        mprintf("%s reopen failed\n",record);
                    }else
                    {
                        f_lseek(&PCM_voiceFile,0);
                    }
              
                                         
                    pcm_opened_flag = 1;// open file 
                    start_readpcm  = 1; 
                   // sdfile_cursize = 0;//restart log file

                    
                    if(xSemaphoreTake(xRecSemaphore,portMAX_DELAY) == pdTRUE)
                    {//这里等待中断发个消息出来,以便后面数据的同步
                    }
                }  
           //=========================
                //write pcm file
                if(LOOP_BUF_CNT(pcm_wptr,pcm_rptr,SDRAM_REC_BUFFER_SIZE)> 1024*4)                  
            //    if(xSemaphoreTake(xRecSemaphore_4K,portMAX_DELAY) == pdTRUE) 
                {//    write
                    while(LOOP_BUF_CNT(pcm_wptr,pcm_rptr,SDRAM_REC_BUFFER_SIZE)>2048)
                    {
                        if(FR_OK !=m_fwrite(&PCM_voiceFile,(void *)(SDRAM_REC_BUFFER_ADDR+pcm_rptr),1*1024,&bw))
                        {
                            mprintf("%s write failed\n",record);
                        }
                        pcm_rptr += 1024;
                        if(pcm_rptr >= SDRAM_REC_BUFFER_SIZE)
                        {
                            pcm_rptr = 0;
                        }
                        ///////led flash
                        LED_Flash_cnt--;
                        if(!LED_Flash_cnt)
                        {
                            if(LED_TEST_STATE)
                            {
                                LED_TEST_OFF;
                            }else
                                LED_TEST_ON;
                            
                            LED_Flash_cnt = LED_Flash_INITNUM;
                        }  
                        //////////////////////
                    }
                    
                    if((&PCM_voiceFile)->fptr>=ONE_FILE_SIZE)
                    {
                        //---- 
                        mprintf("pcm file saved now\r\n");
                        //-------
                        if(FR_OK !=f_close(&PCM_voiceFile))
                        {
                            mprintf("%s close failed\n",record);
                        }  
                       // pcm_opened_flag = 2;//当前的log也存到tf中    
                        pcm_opened_flag = 0;//当前的log也存到tf中   
                        start_readpcm =0;// 这时候采音数据不计入
                        
                        nums++;
                    }                    
                }
                if(xSemaphoreTake(xRecStopSemaphore,2000) == pdTRUE)//
                {//这里等待中断发个消息出来,以便后面数据的同步
                    mprintf("xRecStopSemaphore -> pcm file saved \r\n");
                    if(FR_OK !=f_close(&PCM_voiceFile))
                    {
                        mprintf("%s close failed\n",record);
                    }  
                   // pcm_opened_flag = 2;//当前的log也存到tf中    
                    pcm_opened_flag = 0;//当前的log也存到tf中   
                    start_readpcm =0;// 这时候采音数据不计入                   
                    nums++;
                    
                    while(1)
                    {
                        mprintf("something close rec,please wait ok and restart system!!\r\n");
                        vTaskDelay(5000);
                    }
                }
               // vTaskDelay(1);
            }
        }
    }
}


