#ifndef SYS_H_
#define SYS_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"
#define SCHEDU_POLL_TIME		300000
#define PID_TICK_TIME			18000
#define MODBUS_POLL_TIME		3000
#define MODBUS_TX_TIME  		300


#define TICK_SIZE  26
#define ADC_SAMP_TICK_NO  		0
#define MQTT_TX_TICK_NO  		1
#define MODBUS_FRAME_TICK_NO  	2
#define MODBUS_TX_TICK_NO  		3
#define MODBUS_MQTT_PID_TICK_NO   4
#define MODBUS_POLL_TICK_NO  5
#define SCHEDU_TICK_NO  6


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

#endif

