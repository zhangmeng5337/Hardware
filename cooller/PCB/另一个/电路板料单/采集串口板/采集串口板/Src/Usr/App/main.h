/******************** (C) COPYRIGHT 2012 WQS************** ********************
* File Name          : main.h
* Author             : wqs
* Version            : V1.01
* Date               : 05/28/2012
* Description        : 
********************************************************************************
* 
*******************************************************************************/


#ifndef __MAIN_H
#define __MAIN_H

//--------------------------------------------------------------------------//
#include    "ucos_ii.h"

//--------------------------------------------------------------------------//
//#define  APP_TASK_START_STK_SIZE                         128
#define  TASK_READ_FLASH_STK_SIZE                         128



//#define  APP_TASK_START_PRIO                           3
#define  TASK_READ_FLASH_PRIO                            10

extern OS_EVENT    *pReadFlashSem;

//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//

typedef struct
{
	  unsigned  char    mChipIDStr[12]; 
	  unsigned  char    mSystemInitlFlag;
//..
	  unsigned  char    mSPIFlag;
//..
	  unsigned  char    mAptMeasureFlag; 	
    unsigned  int  mAptMeasureTime; 	
	  unsigned  char mAptMeasureLable;
	  unsigned  int  mAptMeasureAdList[20];
	  unsigned  int  mAptMeasureTimeList[20];
//..		     
}CSystemData;
extern CSystemData   mSystemData;
//--------------------------------------------------------------------------//
void HardwareInital(void);
void SoftInital(void);
void ReadChipID(void);
//
//
void TaskStartApp(void *pdata);
void SystemTaskCreat(void);

void TaskNetShow(void *pdata);
void TaskCanShow(void *pdata);
#endif
//$
