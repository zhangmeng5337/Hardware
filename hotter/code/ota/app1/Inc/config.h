#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"
void delay_us(uint32_t nCount);
#include "main.h"


typedef struct
{
  unsigned char temp[24];//�¶ȶ�
  
  unsigned char mode;// 0---���أ�1----Զ��
  
  unsigned char ai_config[32];
  unsigned char ao_config[1];
  unsigned char di_config[8];
  unsigned char do_config[20];
 }CONFIG_stru;



#endif


