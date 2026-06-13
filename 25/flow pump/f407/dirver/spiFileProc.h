#ifndef SPIFILEPRO1_H
#define SPIFILEPRO1_H
#include "main.h"
#define BIN_SIZE  1044460
#define FILE_NAME_SIZE  32
#define MAX_FILE_COUNT 6





#define ASCII16_INDEX 	0
#define ASCII24_INDEX 	1
#define ASCII32_INDEX 	2
#define FONT16_INDEX 	3
#define FONT32_INDEX 	4
#define IMAG_INDEX 		5



// 假设 SPI Flash 中字库和图片的烧录地址
#define ASCII_FONT_BASE     0x00000000   // 8x16 ASCII 字库
#define HAN16_FONT_BASE     0x00008000   // 16x16 汉字字库
#define HAN32_FONT_BASE     0x00020000   // 32x32 汉字字库
#define IMAGE_BASE          0x00040000   // 一张 100x100 图片

typedef struct {
//    char name[32];
    uint32_t offset;
    uint32_t size;
    uint32_t reserved;
} file_index_t;

 typedef struct
{
    uint32_t fileCount;
	uint32_t offset;
	file_index_t fileIndex[MAX_FILE_COUNT];
	
}binStru;
int stest(void); 
uint32_t getLibBaseAddr(unsigned char type);

#endif

