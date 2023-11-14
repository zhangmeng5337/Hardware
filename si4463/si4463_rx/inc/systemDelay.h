#ifndef _SYSTEMDELAY_H_
#define _SYSTEMDELAY_H_
#include "main.h" 
 
void system_delay_init(void);			    
void system_delay_us(uint32_t nus);
void system_delay_ms(unsigned int nms);
void system_delay_ms_divide10(unsigned int times);
void system_mydelayms(unsigned int a);
unsigned char two_hex_cmp(const unsigned char *str1,const unsigned char *str2,\
	const unsigned char afirstChar,const unsigned char bfirstChar,const unsigned char length);
#endif
