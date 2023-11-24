#ifndef SYS_H_
#define SYS_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"
#define TICK_SIZE  26
#define ADC_SAMP_TICK_NO  		0
#define MQTT_TX_TICK_NO  		1
#define MODBUS_FRAME_TICK_NO  	2
#define MODBUS_TX_TICK_NO  		3

typedef struct
{
	unsigned char TickNum[TICK_SIZE];
	uint32_t TickTime[TICK_SIZE];
	uint32_t TickTimeTmp[TICK_SIZE];	
	unsigned char TimeEnd[TICK_SIZE] ;
    unsigned char TickStart[TICK_SIZE] ;
}tick_stru;

unsigned char GetTickResult(unsigned char TickNo);
void TickPro(void);
unsigned char registerTick(unsigned char TickNo,uint32_t ms);
unsigned char GetTickStatus(unsigned char TickNo);
void delay_ms(uint32_t count);
uint32_t HAL_GetTick(void);
void systick_config(void);
void delay_us(uint32_t nCount);
unsigned char GetTickNum(unsigned char TickNo);
unsigned char setTickResult(unsigned char TickNo,unsigned char oper);
void reset_registerTick(unsigned char TickNo);

#endif

