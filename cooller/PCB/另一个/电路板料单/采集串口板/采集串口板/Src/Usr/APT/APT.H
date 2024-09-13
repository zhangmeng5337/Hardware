/*
*/
#ifndef __APT_H
#define __APT_H
#include    <ucos_ii.h>


#ifdef OS_VERSION
//..............................................................
#define  APT_TASK_STK_SIZE                            256
#define  APT_TASK_START_PRIO                          10
//..............................................................
extern OS_EVENT      *pAptSem;   //
//..............................................................
#endif

//..............................................................
void TaskAptCreate(void);
void AptMeasureTimeCountIntial(void);
void IVC102Inital(void);


unsigned short AptMeasure(unsigned char *pErro);
unsigned short AptAdjustMeasure(unsigned char *pErro);
#endif
//$
