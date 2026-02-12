/*
*/
#ifndef __FOOD_H
#define __FOOD_H
//
#include    <ucos_ii.h>
//
#define  FOOD_NUMBER                                     16 
#if  FOOD_NUMBER==8
#define  FOOD_NUMBER_SPI1
#endif
#if  FOOD_NUMBER==16
#define  FOOD_NUMBER_SPI1
#define  FOOD_NUMBER_SPI2
#endif
#if  FOOD_NUMBER==24
#define  FOOD_NUMBER_SPI1
#define  FOOD_NUMBER_SPI2
#define  FOOD_NUMBER_SPI3
#endif
//-----------------------------------------------------
#define  FOOD8_TASK_STK_SIZE                             128
#define  FOOD16_TASK_STK_SIZE                            128
#define  FOOD24_TASK_STK_SIZE                            128

#define  FOOD8_TASK_START_PRIO                           7
#define  FOOD16_TASK_START_PRIO                          8
#define  FOOD24_TASK_START_PRIO                          9
//-----------------------------------------------------


#ifdef FOOD_NUMBER_SPI1
void SetSpiOneADChannel(unsigned char mChannel);
#endif

#ifdef FOOD_NUMBER_SPI2
void SetSpiTwoADChannel(unsigned char mChannel);
#endif

#ifdef FOOD_NUMBER_SPI3
void SetSpiThreeADChannel(unsigned char mChannel);
#endif

#ifdef OS_VERSION
extern OS_EVENT      *pFood8Sem;   //
extern OS_EVENT      *pFood16Sem;   //
extern OS_EVENT      *pFood24Sem;   //

#endif

typedef struct
{
	  unsigned char mChannel;	
//..		    
    unsigned char mADReadFlag[3];		 
//..		     	
}CFoodData;
extern CFoodData   mFoodData;

void TaskFoodCreate(void);
void FoodIntal(void);
void SetLedValue(unsigned char mChannel,unsigned char mState);
#endif
//$
