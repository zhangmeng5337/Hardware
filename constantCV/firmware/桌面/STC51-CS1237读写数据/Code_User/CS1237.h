#ifndef __CS1237_H__
#define __CS1237_H__
#include<STC15W.h>
#include<intrins.h>

//配置CS1237芯片
void Con_CS1237(void);
//读取芯片的配置数据
unsigned char Read_CON(void);
//读取ADC数据
long Read_CS1237(void);

#endif