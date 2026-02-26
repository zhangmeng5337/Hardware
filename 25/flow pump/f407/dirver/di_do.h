
#ifndef DI_DO_H
#define DI_DO_H
#include "main.h"
#define DO_ON 		0
#define DO_OFF 		1
typedef enum
{
	di0 = 0x01,
	di1 = 0x02,
	di2 = 0x04,
	di3 = 0x08,
	di4 = 0x10,
	di5 = 0x20,
	di6 = 0x40
}diInEnum;

typedef struct
{
	unsigned int diStatus; //0:no input
	unsigned int diLevel;
	unsigned char dolevel;
}di_do_stru;
void setDo(unsigned char doNum,unsigned char level);
void getDiStatus(void);
void readDi(void);
void doInit(void);
void diDoProc(void);
#endif

