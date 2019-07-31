#ifndef FILTER_H
#define FILTER_H
#include "stm32f0xx_hal.h"
#define N					20
#define BUFFERSIZE		80
#define THRES_X			100
#define THRES_Y			100
#define THRES_Z			100
#define ALPHA			0.05
#define SAMPLE_CYCLE	20
#define UPDATE_MAX_TIME	20
#define TD			15
typedef struct
{
        unsigned char sample_index;
	short int S[3][BUFFERSIZE];
	signed long int  A[3][BUFFERSIZE];	
	signed long int B[3][BUFFERSIZE];		
}MANETIC_TYPEDEF;
typedef struct
{

	unsigned int Ts;
	unsigned int Te;
	unsigned char UpdateFlag;
	unsigned char EventEndFalg;
	unsigned char CarFLag;
	unsigned int STimes;
	unsigned int Eimes;
	unsigned int  Tp[3][BUFFERSIZE/2];	
	unsigned int  Tv[3][BUFFERSIZE/2];
	signed long int  Mp_MAX;
	signed long int  Mv_MIN;
	signed long int  Mp[3][BUFFERSIZE/2];	
	signed long int  Mv[3][BUFFERSIZE/2];		
}MPV_Struct;
void SupvisorProcess(void);
void InitBaseLine(void);
#endif
