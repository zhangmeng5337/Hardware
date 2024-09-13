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
��Function  ��:OS���е�������������
               ������������
	           �����������е��ź���...��ʼ��			  
��WriteDate ��:2011.11.21
��EditDate  ��:2011.11.21
��Autor     ��:wuqingsong
��Descrption��:
=================================================*/
void SystemTaskCreat(void)
{
   INT8U os_err;
//...............................   	
	
//...............................   
	 //��������
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
�������ܣ�������������Ӳ���������ʼ��������ʱ�ӳ�ʼ��
��дʱ��: 2012.01.05
�޸�ʱ��: 2012.01.05
��    �ߣ�������
˵    ����//
=====================================================================*/
void TaskStartApp(void *pdata)
{   
//.........................................   	 	
	 OS_CPU_SysTickInit();//os��ֲ����:ʱ�ӳ�ʼ��
   OSStatInit(); 		    //os��ֲ����:OS��ʼ��			
	 mSystemData.mSystemInitlFlag = 0;//ϵͳ��ʼ����ʼ
	 HardwareInital();    //ϵͳӲ����ʼ��	 
	 SoftInital();        //ϵͳ�����ʼ��  
	 mSystemData.mSystemInitlFlag = 1;//ϵͳ��ʼ������	
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
//..��ȡFLASH����
    for(i = 0; i < 3 ; i++)
    {
        *pDateObject++ = *pDateSouce++;
    }	

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
