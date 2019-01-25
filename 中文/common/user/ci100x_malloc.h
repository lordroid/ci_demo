#ifndef __CI100X_MALLOC_H
#define __CI100X_MALLOC_H

#ifdef __cplusplus
extern "C"{
#endif

#define USE_IAR_ALLOC

#ifndef USE_IAR_ALLOC

#define CI100X_MALLOC_ID 0x5

typedef struct CI100X_BLOCK_LINK{
	struct CI100X_BLOCK_LINK* pxNextFreeBlock;
	unsigned int xBlockSize:28;
	unsigned int xflag:4;
}CI100X_BlockLink_t;

#endif
typedef struct{
	unsigned int allocedsize;
	unsigned int allocedtimes;
	unsigned int freesize;
	unsigned int freetimes;
	unsigned int remainfreesize;
	void* debug_printf;
	char debug_ver;
}CI100X_MALLOC_DEBUG;

void ci100x_malloc_init(unsigned int baseaddr,unsigned int heapsize);
void ci100x_debug(CI100X_MALLOC_DEBUG* debug);

#ifdef __cplusplus
}
#endif

extern  void* ci100x_malloc(unsigned int size);
extern  void  ci100x_free(void* p);

#endif /*__CI100X_MALLOC_H*/