#ifndef _SYSTEMDELAY_H_
#define _SYSTEMDELAY_H_
#include "main.h" 
 
void system_delay_init(void);			    
void system_delay_us(uint32_t nus);
void system_delay_ms(unsigned int nms);
void system_delay_ms_divide10(unsigned int times);
void system_mydelayms(unsigned int a);

#endif
