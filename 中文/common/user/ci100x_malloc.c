/**
  ******************************************************************************
  * @文件    ci100x_malloc.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件是chipintelli公司的CI100X芯片程序的内存管理函数.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 
#include "ci100x_malloc.h"

#ifndef USE_IAR_ALLOC

static char * ci100x_baseaddr=(char*)0x70100000;
static unsigned int ci100x_heapsize= 1*1024*1024;
static unsigned int ci100x_aligheapsize = ci100x_heapsize-8;
static char ci100x_xHeapHasBeenInit	=0;
static CI100X_BlockLink_t ci100x_xStart,ci100x_xEnd;
static const unsigned short ci100x_heapSTRUCT_SIZE	= ( ( sizeof ( CI100X_BlockLink_t ) + ( 8 - 1 ) ) & ~0x7 );
#define ci100x_heapMINIMUM_BLOCK_SIZE	( ( unsigned int ) ( ci100x_heapSTRUCT_SIZE * 2 ) )
static unsigned int ci100x_xFreeBytesRemaining =  ci100x_aligheapsize;

//for debug
static unsigned int ci100x_allocedsize=0,ci100x_allocedtimes=0,ci100x_freesize=0,ci100x_freetimes=0;
static void prvInsertBlockIntoFreeList( CI100X_BlockLink_t * pxBlockToInsert ){																					\
	CI100X_BlockLink_t *pxIterator;
	unsigned int xBlockSize;
	xBlockSize = pxBlockToInsert->xBlockSize;
	for( pxIterator = &ci100x_xStart; pxIterator->pxNextFreeBlock->xBlockSize < xBlockSize; pxIterator = pxIterator->pxNextFreeBlock )
	{																			
		/* There is nothing to do here - just iterate to the correct position. */
	}																																			\
	pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	pxIterator->pxNextFreeBlock = pxBlockToInsert;
}

static void ci100x_prvHeapInit(void){
	CI100X_BlockLink_t *pxFirstFreeBlock=0;
	unsigned char * pucAlignedHeap=0;	
	pucAlignedHeap=(unsigned char *)( ( ( unsigned int  ) (ci100x_baseaddr+8) ) & ( ~( ( unsigned int) 0x7 ) ) );
	ci100x_xStart.pxNextFreeBlock=( CI100X_BlockLink_t * ) pucAlignedHeap;
	ci100x_xStart.xBlockSize = ( unsigned int ) 0;
	
	ci100x_xEnd.xBlockSize = -8;
	ci100x_xEnd.pxNextFreeBlock = (CI100X_BlockLink_t *)0;
	
	pxFirstFreeBlock = ( CI100X_BlockLink_t * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = ci100x_aligheapsize;
	pxFirstFreeBlock->pxNextFreeBlock = &ci100x_xEnd;
}
#endif 

void ci100x_malloc_init(unsigned int baseaddr,unsigned int heapsize){
#ifndef USE_IAR_ALLOC
  	ci100x_baseaddr = (char*)baseaddr;
	ci100x_heapsize  = heapsize;
	ci100x_aligheapsize = ci100x_heapsize-8;
	ci100x_xFreeBytesRemaining =  ci100x_aligheapsize;
	ci100x_xHeapHasBeenInit=0;
	while(--heapsize)
    {
		*(char*)baseaddr=0;
		baseaddr++;
	}
#endif 
}

#include <stdlib.h>
void * ci100x_malloc(unsigned int size){
#ifdef USE_IAR_ALLOC
  return malloc(size);
#else
 	CI100X_BlockLink_t *pxBlock =0,*pxPreviousBlock=0,*pxNewBlockLink=0;
	void* pvReturn =0;
	if(!ci100x_xHeapHasBeenInit)
    {
		ci100x_prvHeapInit();
		ci100x_xHeapHasBeenInit=1;
	}
	if(size>0){
		size+=ci100x_heapSTRUCT_SIZE;	
		if(size & 0x7)
        {
			size +=(8-(size &0x7));	
		}
	}
	if((size >0) && (size <ci100x_aligheapsize)){
		pxPreviousBlock = &ci100x_xStart;
		pxBlock = ci100x_xStart.pxNextFreeBlock;
		while( ( pxBlock->xBlockSize < size ) &&\
		 ( pxBlock->pxNextFreeBlock != 0 ) ){
			pxPreviousBlock = pxBlock;
			pxBlock = pxBlock->pxNextFreeBlock;
		}
		if(pxBlock !=&ci100x_xEnd){
			pvReturn =(void*)( ( ( unsigned char * ) pxPreviousBlock->pxNextFreeBlock ) \
			+ ci100x_heapSTRUCT_SIZE );
			pxPreviousBlock->pxNextFreeBlock->xflag = CI100X_MALLOC_ID;

			ci100x_allocedsize +=size;
			ci100x_allocedtimes++;

			pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;
			if(( pxBlock->xBlockSize - size ) > ci100x_heapMINIMUM_BLOCK_SIZE ){
				pxNewBlockLink = ( CI100X_BlockLink_t * ) ( ( ( unsigned char * ) pxBlock ) + size );
				pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - size;
				pxBlock->xBlockSize = size;	
				prvInsertBlockIntoFreeList( ( pxNewBlockLink ) );
			}
			ci100x_xFreeBytesRemaining -= pxBlock->xBlockSize;
		}	
	}
	return pvReturn;
#endif
}

void ci100x_free(void* pv){
#ifdef USE_IAR_ALLOC
  	free(pv);
#else
 	unsigned char *puc = ( unsigned char * ) pv;
	CI100X_BlockLink_t *pxLink=0;
	if( pv != 0 ){
		puc -= ci100x_heapSTRUCT_SIZE;
		pxLink = ( CI100X_BlockLink_t * ) puc;
		if(pxLink->xflag==CI100X_MALLOC_ID)
		{
			pxLink->xflag =0;

			ci100x_freesize +=pxLink->xBlockSize;
			ci100x_freetimes++;

			prvInsertBlockIntoFreeList( ( ( CI100X_BlockLink_t * ) pxLink ) );
			ci100x_xFreeBytesRemaining += pxLink->xBlockSize;
		}
	}
#endif 
}

void ci100x_debug(CI100X_MALLOC_DEBUG* debug){
#ifndef USE_IAR_ALLOC
 	 CI100X_BlockLink_t *pBlock=0;
	//for(pBlock=ci100x_xStart.pxNextFreeBlock;pBlock!=&ci100x_xEnd;pBlock=pBlock->pxNextFreeBlock){
		
	//}
	debug->allocedsize	=ci100x_allocedsize; 
	debug->allocedtimes	=ci100x_allocedtimes;
	debug->freesize		=ci100x_freesize;
	debug->freetimes	=ci100x_freetimes;
	debug->remainfreesize	=ci100x_xFreeBytesRemaining;
#endif 
}
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ci100x_uart.h"
/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
 