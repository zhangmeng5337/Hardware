#ifndef SYS_H_
#define SYS_H_
#include "main.h"

#define ADC_POLL_TIME  		10




#define TICK_SIZE  10
#define ADC_POLL_TICK  		0
#define LCD_FUCN_SW_TICK    1
#define LCD_BLINK_TICK      2



typedef struct
{
	unsigned char TickNum[TICK_SIZE];
	uint32_t TickTime[TICK_SIZE];
	uint32_t TickTimeTmp[TICK_SIZE];	
	unsigned char TimeEnd[TICK_SIZE] ;
    unsigned char TickStart[TICK_SIZE] ;
}tick_stru;

unsigned char GetTickResult(uint32_t TickNo);
void TickPro(void);
unsigned char registerTick(uint32_t TickNo,uint32_t ms);
unsigned char GetTickStatus(uint32_t TickNo);
void delay_ms(uint32_t count);
uint32_t HAL_GetTick(void);
void systick_config(void);
void delay_us(uint32_t nCount);
unsigned char GetTickNum(uint32_t TickNo);
unsigned char setTickResult(uint32_t TickNo,unsigned char oper);
void reset_registerTick(uint32_t TickNo);
int strrindex(char s[], char t[]);
void delay_us(uint32_t nCount);

#endif


