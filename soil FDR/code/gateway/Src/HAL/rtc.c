#include "rtc.h"
#include "stdlib.h"
#include "register.h"
#include "modbus.h"

extern RTC_HandleTypeDef hrtc;
static RTC_STRU rtc_usr;

RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure;


void RtcIT_ctrl(unsigned char flag)
{
	if(flag == 1)
	{
	__HAL_RTC_WAKEUPTIMER_DISABLE(&hrtc) ; 
	__HAL_RTC_WAKEUPTIMER_DISABLE_IT(&hrtc,RTC_IT_WUT);
	
	}
	else 
	{
	__HAL_RTC_WAKEUPTIMER_ENABLE(&hrtc) ; 
	__HAL_RTC_WAKEUPTIMER_ENABLE_IT(&hrtc,RTC_IT_WUT);		
	}

}
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ��
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
    RTC_TimeTypeDef RTC_TimeStructure;

    RTC_TimeStructure.Hours=hour;
    RTC_TimeStructure.Minutes=min;
    RTC_TimeStructure.Seconds=sec;
    RTC_TimeStructure.TimeFormat=ampm;
    RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
    return HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BCD);
}

//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ��
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
    RTC_DateTypeDef RTC_DateStructure;

    RTC_DateStructure.Date=date;
    RTC_DateStructure.Month=month;
    RTC_DateStructure.WeekDay=week;
    RTC_DateStructure.Year=year;
    return HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BCD);
}

//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       2,�����ʼ��ģʽʧ��;
u8 RTC_Init(void)
{
  
    HAL_RTC_MspDeInit(&hrtc);
    HAL_RTC_MspInit(&hrtc);
    __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);


    hrtc.Instance=RTC;
    hrtc.Init.HourFormat=RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ
    hrtc.Init.AsynchPrediv=0X7F;           //RTC�첽��Ƶϵ��(1~0X7F)
    hrtc.Init.SynchPrediv=0XFF;            //RTCͬ����Ƶϵ��(0~7FFF)
    hrtc.Init.OutPut=RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&hrtc)!=HAL_OK) return 2;

    // HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5051);
    if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5050)//�Ƿ��һ������
    {
        RTC_Set_Time(0x14,0x03,0,RTC_HOURFORMAT12_PM);	        //����ʱ�� ,����ʵ��ʱ���޸�
        RTC_Set_Date(0x20,0x12,0x03,0x04);		                    //��������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
        HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD);
        HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BCD);
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,stimestructure.Hours);//����Ѿ���ʼ������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR2,stimestructure.Minutes);//����Ѿ���ʼ������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR3,stimestructure.Seconds);//����Ѿ���ʼ������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR4,sdatestructure.Year);//����Ѿ���ʼ������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR5,sdatestructure.Month);//����Ѿ���ʼ������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR6,sdatestructure.Date);//����Ѿ���ʼ������
    }
//    unsigned char *p;
//    p = ReadRegister(0xf001);
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);




//    if(p[1] == 0x01)
//    {
//        p = ReadRegister(0xf002);
//        p[0] = p[0];
//        p[1] = p[1];
//        /*rtc_usr.wakeup_period = p[0];
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;*/

//        rtc_usr.wakeup_period = p[0];
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
//        // rtc_usr.wakeup_period = rtc_usr.wakeup_period*20;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;

//        //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,  rtc_usr.wakeup_period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

//        
//        SetWakeUp(p);


//    }
//    else if(p[1] == 0x02)
//    {
//        p = ReadRegister(0xf003);
//        //q[0] = p[0];
//        //q[1] = p[1];
//        rtc_usr.wakeup_period = p[0];
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
//        // rtc_usr.wakeup_period = rtc_usr.wakeup_period*20;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;
//        //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,  rtc_usr.wakeup_period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

//        SetWakeUp(p);



//    }


    return 0;
}
void RTC_WAKEUP_Init(void)
{

    unsigned char *p;
    p = ReadRegister(0xf001);
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    if(p[1] == 0x01)
    {
        p = ReadRegister(0xf002);
        p[0] = p[0];
        p[1] = p[1];
        /*rtc_usr.wakeup_period = p[0];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;*/

        rtc_usr.wakeup_period = p[0];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
        // rtc_usr.wakeup_period = rtc_usr.wakeup_period*20;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;

        //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,  rtc_usr.wakeup_period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

        
        SetWakeUp(p);


    }
    else if(p[1] == 0x02)
    {
        p = ReadRegister(0xf003);
        //q[0] = p[0];
        //q[1] = p[1];
        rtc_usr.wakeup_period = p[0];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
        // rtc_usr.wakeup_period = rtc_usr.wakeup_period*20;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;
        //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,  rtc_usr.wakeup_period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

        SetWakeUp(p);



    }
}


//RTC�ײ�������ʱ������
//�˺����ᱻHAL_RTC_Init()����
////hrtc:RTC���
//void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
//{
//    RCC_OscInitTypeDef RCC_OscInitStruct;
//    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

//    __HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
//	HAL_PWR_EnableBkUpAccess();//ȡ����������д����
//
//    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE����
//    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
//    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTCʹ��LSE
//    HAL_RCC_OscConfig(&RCC_OscInitStruct);

//    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//����ΪRTC
//    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
//    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
//
//    __HAL_RCC_RTC_ENABLE();//RTCʱ��ʹ��
//}

//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7) @ref  RTC_WeekDay_Definitions
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
    RTC_AlarmTypeDef RTC_AlarmSturuct;

    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //��
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;

    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//��ȷƥ�����ڣ�ʱ����
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //����
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //����A
    HAL_RTC_SetAlarm_IT(&hrtc,&RTC_AlarmSturuct,RTC_FORMAT_BIN);

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//�����Ի��Ѷ�ʱ������
/*wksel:  @ref RTCEx_Wakeup_Timer_Definitions
#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
*/
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־

    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,cnt,wksel);            //������װ��ֵ��ʱ��

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

//RTC�����жϷ�����
//void RTC_Alarm_IRQHandler(void)
//{
//    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
//}

//RTC����A�жϴ���ص�����
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

//RTC WAKE UP�жϷ�����
//void RTC_WKUP_IRQHandler(void)
//{
//    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler);
//}

RTC_STRU *getRtcStatus(void)
{
    return &rtc_usr;
}

void SetWakeUp(unsigned char *p)
{
    // RTC_HandleTypeDef hrtc;
    unsigned int WakeUpCounter;
    WakeUpCounter = ((u16)(p[0]))<<8;
    WakeUpCounter = WakeUpCounter +p[1];
    WakeUpCounter = WakeUpCounter*60;
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WakeUpCounter, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
    //HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
    //HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

}

void  setRtc(unsigned char* p,unsigned char item)
{
    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */
    if(item <=3)
    {
        //	RTC_HandleTypeDef hrtc;
        RTC_TimeTypeDef sTime = {0};
        RTC_DateTypeDef sDate = {0};
        sDate.Year = p[0];
        sDate.Year = p[1];
        sDate.Year = p[2];
        sTime.Hours = p[3];
        sTime.Minutes = p[4];
        sTime.Seconds = p[5];

        HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
        HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);


    }


}
void getRTC()
{
    unsigned char p[6],i;

    HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BCD);
    i = 0;
    p[i++] = sdatestructure.Year;
    p[i++] = sdatestructure.Month;
    p[i++] = sdatestructure.Date;
    p[i++] = stimestructure.Hours;
    p[i++] = stimestructure.Minutes;
    p[i++] = stimestructure.Seconds;
    WriteRegisteSet(0x1100,p,6);


}



