#ifndef SYS_H_
#define SYS_H_
#include "main.h"

#define TICK_SIZE  10

#define ADC_SAMP_TIME       100
#define LCD_REFRESH_TIME    200
#define FLASH_REFRESH_TIME  50
#define ADC_TICK_NO  		   0
#define LCD_TICK_NO  		   1
#define FLASH_TICK_NO  		   2


typedef struct
{
	unsigned char TickNum[TICK_SIZE];
	uint32_t TickTime[TICK_SIZE];
	uint32_t TickTimeTmp[TICK_SIZE];	
	unsigned char TimeEnd[TICK_SIZE] ;
    unsigned char TickStart[TICK_SIZE] ;
}tick_stru;


void TickPro(void);
void reset_registerTick(uint32_t TickNo);
unsigned char GetTickNum(uint32_t TickNo);
unsigned char GetTickResult(uint32_t TickNo);

#endif


