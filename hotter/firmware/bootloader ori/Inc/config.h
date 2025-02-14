#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"


typedef struct
{
  unsigned char temp[24];//温度段
  
  unsigned char mode;// 0---本地；1----远程
  
  unsigned char ai_config[32];
  unsigned char ao_config[1];
  unsigned char di_config[8];
  unsigned char do_config[20];
 }CONFIG_stru;



#endif


