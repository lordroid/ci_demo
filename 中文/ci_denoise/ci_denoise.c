#include "ci100x_timer.h" 
#include "ci100x_scu.h"

#include "user_config.h"

#include "ci100x_global.h"
#include <string.h>
#include "includes.h"
#include "ci_denoise.h"

#include "denoise_api.h"

static int denoise_task_started = 0;
 
static short pcm_out_buf[512];
static char  ver_buf[20];
 
extern  void apply_denoise(short * pcm_in, short * pcm_out);
extern int enter_denoise_api(float api_denoise_threshold, float api_decide_frame_len );
extern int denoise_parameter_api(float denoise_alpha_l,float denoise_alpha_m,float denoise_beta,float denoise_G_min );
extern int denoise_getver(char* ver_buf);

int debug_pcm_addr_flag = 0;
QueueHandle_t DENOISE_xQueue = NULL;

void vTask_Denoise(void *pvParameters)
{
    denoise_datatype receive_data;
 
    short * pcm_in ;
    
    denoise_init();
    
    denoise_getver(ver_buf);
    //mprintf("\n%s",ver_buf);
    enter_denoise_api(0.1, 25);
     
    denoise_parameter_api(0.85,0.85,0.5,0.07 );

    debug_pcm_addr_flag = 0;
    DENOISE_xQueue = xQueueCreate(50,sizeof(denoise_datatype));
    denoise_task_started = 1;
    for( ;; )
    {
         if(pdPASS==xQueueReceive(DENOISE_xQueue,&receive_data,pdMS_TO_TICKS(100)))
         {
            pcm_in = (short*)receive_data.data_address;
            if(pcm_in)
            {
              apply_denoise(pcm_in,pcm_out_buf);
            }
            memcpy((void *)0x20020000,pcm_out_buf,256*sizeof(short));
            //mprintf("denoise ..\n");
            denoise_task_started = 2;
         }else
         {
            //mprintf("denoise data in time out!\n");
         }
    }
}

extern int IISxDMA_TADDR0(IISxDMA iisxdma,unsigned int txaddr0);
extern int IISxDMA_TADDR1(IISxDMA iisxdma,unsigned int txaddr0);
           
void IIS1_denoise_receive_handler(void)
{
    static unsigned int count = 0;
    static unsigned int denoise_last_addr = DENOISE_IN_DATA_BASE_ADDR;
    denoise_datatype denoise_receive_data;
    BaseType_t xHigherPriorityTaskWoken;
    
    uint32_t addr;

    if( denoise_task_started)
    {
        count = count%DENOISE_IN_DATA_BUF_COUT;
        addr = DENOISE_IN_DATA_BASE_ADDR + 512*count;
        
        IISxDMA_RADDR(IIS1DMA, addr);
        ++count;
        denoise_receive_data.data_address = denoise_last_addr;
        denoise_last_addr = addr;
    	if(pdPASS!=xQueueSendToBackFromISR(DENOISE_xQueue,&denoise_receive_data,&xHigherPriorityTaskWoken ))
    	{
          //mprintf("AEC ERROR:aec deal too slow!!!\n");
        }

        if(xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(pdTRUE);
        }
    }     
}
