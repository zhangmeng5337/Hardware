/*

*/
#ifndef __TASK_H
#define __TASK_H

#include "stm32f10x.h"


#define PARAMTER_Address               ((uint32_t)0x0800C000)

void  RunTickIntial(void);
void PreRunTick(void);
void  ReStartRunDevice(void);	

void TaskReadFlash(void *pdata);
unsigned char ReadDataFromSd(unsigned int mReadDataPoint);
void  SetRelay(unsigned char *pRelayData);
#endif
//$
