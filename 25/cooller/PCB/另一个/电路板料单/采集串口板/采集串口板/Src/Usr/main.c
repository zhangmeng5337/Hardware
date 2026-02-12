/**
  ******************************************************************************
  * @file    main.c   STM32F10X_CL  STM32F10X_LD
  * @author
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Custom HID demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  W5500-STM32F105-CAN V1.10
  */
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CSystemData   mSystemData;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

//Coordinate  mCoordinatePoint;
int main(void)
{
  /* Add your application code here
     */   
  CPU_IntDis();
  OSInit();
  SystemTaskCreat();
  OSStart();
}
/*=================================================
【Function  】:OS所有的任务建立函数：
               包含任务建立、
	           包含任务所有的信号量...初始化			  
【WriteDate 】:2011.11.21
【EditDate  】:2011.11.21
【Autor     】:wuqingsong
【Descrption】:
=================================================*/
void SystemTaskCreat(void)
{
   INT8U os_err;
//...............................   	
	
//...............................   
	 //启动任务
   os_err = OSTaskCreateExt( (void (*)(void *)) TaskStartApp,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
  #if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
  #endif  
//...................................	
    TaskUartCreate();
//..
	  TaskFoodCreate();
//..
    TaskAptCreate();														   
}
/*=====================================================================
函数功能：任务建立，包括硬件、软件初始化，任务时钟初始化
编写时间: 2012.01.05
修改时间: 2012.01.05
作    者：吴青松
说    明：//
=====================================================================*/
void TaskStartApp(void *pdata)
{   
//.........................................   	 	
	 OS_CPU_SysTickInit();//os移植必须:时钟初始化
   OSStatInit(); 		    //os移植必须:OS初始化			
	 mSystemData.mSystemInitlFlag = 0;//系统初始化开始
	 HardwareInital();    //系统硬件初始化	 
	 SoftInital();        //系统软件初始化  
	 mSystemData.mSystemInitlFlag = 1;//系统初始化结束	
//..........................................
   while(1)
   {	 			
        OSTimeDly(40);  	 
   }
}
/*******************************************************************************
* Function Name  : 
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None	  *** error 65: access violation at 0x64000000 : no 'write' permission
*******************************************************************************/
void HardwareInital(void)
{
	  ReadChipID();
//..
    mSystemData.mSPIFlag = 0;	
//..
    FoodIntal();	
//..
	  OSTimeDly(100); 
//...	  
	  ADSPI3Intial();
//..
    ADSPI2Intial();	
//
    
//..
	  ADSPI1Intial();
	
    AptMeasureTimeCountIntial();	
	  IVC102Inital();
//..
    UartInital();		
}
/*******************************************************************************
* Function Name  : SoftInital
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : BRIGHTBLUE  BLACK
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SoftInital(void)
{
   
}
/*******************************************************************************
* Function Name  : ReadChipID
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReadChipID(void)
{	
    unsigned int *pDateObject,*pDateSouce;
	  unsigned char i;
//..
	  pDateObject =  (unsigned int *)mSystemData.mChipIDStr;
    pDateSouce  =  (unsigned int *)((uint32_t)0x1ffff7e8);
//..读取FLASH数据
    for(i = 0; i < 3 ; i++)
    {
        *pDateObject++ = *pDateSouce++;
    }	

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
