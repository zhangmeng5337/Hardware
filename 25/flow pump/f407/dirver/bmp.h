#ifndef BMP_H
#define BMP_H
#include "main.h"
typedef struct
{
	unsigned int index;
	unsigned char width;
	unsigned char height;
	unsigned int bmpSize;
	uint16_t *bmp;
}bmp_stru;
#endif

