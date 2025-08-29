#ifndef __MALLOC_H
#define __MALLOC_H
#include "main.h"
#include "stm32f4xx_hal.h"
#ifndef NULL
#define NULL 0
#endif

#define MEM_BLOCK_SIZE              32                                      //内存块大小为32字节
#define MEM_MAX_SIZE                16*1024                                  //最大管理内存 2K
#define MEM_ALLOC_TABLE_SIZE    MEM_MAX_SIZE/MEM_BLOCK_SIZE //内存表大小

//内存管理控制器
struct _m_mallco_dev
{
    void (*init)(void);                    //初始化
    unsigned char   (*perused)(void);                      //内存使用率
    unsigned char       *membase;                    //内存池
    unsigned int   *memmap;                     //内存管理状态表
    unsigned char     memrdy;                         //内存管理是否就绪
};

extern struct _m_mallco_dev mallco_dev;     //在mallco.c里面定义

void mymemset(void *s,unsigned char c,uint32_t count);     //设置内存
void mymemcpy(void *des,void *src,uint32_t n);//复制内存

void mem_init(void);                     //内存管理初始化函数
uint32_t mem_malloc(uint32_t size);         //内存分配
unsigned char mem_free(uint32_t offset);         //内存释放
unsigned char mem_perused(void);                 //获得内存使用率
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(void *ptr);              //内存释放
void *mymalloc(uint32_t size);            //内存分配
void *myrealloc(void *ptr,uint32_t size);//重新分配内存
#endif

