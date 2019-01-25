#ifndef __CI_DENOISE_H_
#define __CI_DENOISE_H_


#ifdef __cplusplus
extern "C"{
#endif



typedef struct
{
    unsigned int data_address;
    unsigned int ostick;
}denoise_datatype;


#define DENOISE_IN_DATA_BASE_ADDR (0x70400000)
#define DENOISE_IN_DATA_BUF_COUT 20//512








extern void IIS1_denoise_receive_handler(void);

#ifdef __cplusplus
}
#endif

#endif //__CI_DENOISE_H