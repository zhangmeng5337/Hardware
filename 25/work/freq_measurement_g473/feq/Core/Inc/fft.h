#ifndef FFT_H_
#define FFT_H_
#include "main.h"
typedef struct
{
	float result;
	float vol;
	float f;	
}fft_stru;

float DFT_Calculation(unsigned char *Data, int num, int N,uint32_t samp_freq);

#endif

