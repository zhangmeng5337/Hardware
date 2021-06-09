#include "app.h"
#include "sensors.h"
#include "LteHal.h"
#include "LteHardware.h"
#include  "MX25_CMD.h"
#include "EEPROM.h"
#include "stdlib.h"
#include "crc.h"
#include "rtc.h"
#include "LowpowerTest.h"

extern ADC_HandleTypeDef hadc1;
systemParams_stru systemParams_usr;
void payloadpack(unsigned char *p,uint32_t size);
extern float FilterData[SN];//滤波后的imu数据
void HardwareInit()
{
    RTC_Init();
    LteUartConfig();
    if(DEBUG_MODE == 0)
    {
        if( ROLE == LTE_4G)

            LtePowerManage(LTE_4G,ON);
        else
        {
            LtePowerManage(LTE_NBIOT,ON);

            while(gps_powerON())
                ;
        }


    }
    FLASH_INIT();
    sensors_Init();
}

uint32_t CpuID[3];
uint32_t Lock_Code;
void GetLockCode(void)
{
//获取CPU唯一ID

    CpuID[0]=*(volatile uint32_t*)(0x1fff7590);
    CpuID[1]=*(volatile uint32_t*)(0x1fff7594);
    CpuID[2]=*(volatile uint32_t*)(0x1fff7598);
//加密算法,很简单的加密算法

    Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
}


void ParamsInit(void)
{

    uint32_t p;
    uint32_t addr;
    FLashData_stru *FLashData_usr2;
    addr = 0;
    FLashData_usr2 = GetFLashStatus();
    if(DEBUG_MODE == 1)
    {
        FLashData_usr2->LastWriteAddr = 0x0000;
    }

    flash_read(addr++,&p,1);
    if(p!=0x5a)
    {
        systemParams_usr.period = 2;
        p = 0x5a; //写入标志
        flash_init(0);
        addr = 0;
        flash_write(addr++, &p,1);
        flash_write( addr++,&(FLashData_usr2->LastReadAddr),1);
        flash_write( addr++,&(FLashData_usr2->LastWriteAddr),1);
        flash_write( addr++,&(FLashData_usr2->SumLen),1);

        if(DEBUG_MODE == 0)
        {
            systemParams_usr.DayPeriod = 2;
            systemParams_usr.NightPeriod = 2;
            systemParams_usr.Dhours = 6;
            systemParams_usr.Nhours = 18;
		  systemParams_usr.vbatT = 31;

        }
        if(DEBUG_MODE == 1)
        {
            systemParams_usr.DayPeriod = 2;
            systemParams_usr.NightPeriod = 2;
            systemParams_usr.Dhours = 6;
            systemParams_usr.Nhours = 18;
			systemParams_usr.vbatT = 31;
        }

        uint32_t tmp;

        tmp = systemParams_usr.DayPeriod;
        flash_write( addr++,&(tmp),1);
        tmp = systemParams_usr.NightPeriod;
        flash_write( addr++,(uint32_t *)&(tmp),1);
        tmp = systemParams_usr.Dhours;
        flash_write( addr++,(uint32_t *)&(tmp),1);
        tmp = systemParams_usr.Nhours;
        flash_write( addr++,(uint32_t *)&(tmp),1);
        tmp = systemParams_usr.vbatT;
        flash_write( addr++,(uint32_t *)&(tmp),1);		
        if(DEBUG_MODE == 0)
        {
            EraseChip();
        }
    }
    else
    {

        addr = 0;
        flash_read( addr++, &p,1);
        flash_read( addr++, &(FLashData_usr2->LastReadAddr),1);

        flash_read( addr++, &(FLashData_usr2->LastWriteAddr),1);
        flash_read( addr++,  &(FLashData_usr2->SumLen),1);
        uint32_t tmp;


        flash_read( addr++,&(tmp),1);
        systemParams_usr.DayPeriod=tmp;

        flash_read( addr++,(uint32_t *)&(tmp),1);
        systemParams_usr.NightPeriod=tmp ;

        flash_read( addr++,(uint32_t *)&(tmp),1);
        systemParams_usr.Dhours=tmp  ;

        flash_read( addr++,(uint32_t *)&(tmp),1);
        systemParams_usr.Nhours =tmp ;
	flash_read( addr++,(uint32_t *)&(tmp),1);
	systemParams_usr.vbatT =tmp ;

    }
    GetLockCode();

}
void systmeReconfig()
{
    uint32_t p;
    uint32_t addr;
    p = 0x5a;  //дɫҪ־
    flash_init(0);
    addr = 0;
    flash_write(addr++, &p,1);
    flash_write( addr++,&(GetFLashStatus()->LastReadAddr),1);

    flash_write( addr++,&(GetFLashStatus()->LastWriteAddr),1);
    flash_write( addr++,&(GetFLashStatus()->SumLen),1);

    uint32_t tmp;

    tmp = systemParams_usr.DayPeriod;
    flash_write( addr++,&(tmp),1);
    tmp = systemParams_usr.NightPeriod;
    flash_write( addr++,(uint32_t *)&(tmp),1);
    tmp = systemParams_usr.Dhours;
    flash_write( addr++,(uint32_t *)&(tmp),1);
    tmp = systemParams_usr.Nhours;
    flash_write( addr++,(uint32_t *)&(tmp),1);
    tmp = systemParams_usr.vbatT;
    flash_write( addr++,(uint32_t *)&(tmp),1);

}
void cmdResponse(unsigned char wr,unsigned char *params)
{
    uint8_t *pb;
    uint32_t len;
    if(wr == 1)//读
    {
        if(params[PARAMS_INDEX] == 0x31)
        { 
					  len = 0;
            pb = malloc(4);
            memcpy(pb+len,&systemParams_usr.Dhours,1);
            len++;
            memcpy(pb+len,&systemParams_usr.DayPeriod,1);
            len++;
            memcpy(pb+len,&systemParams_usr.Nhours,1);
            len++;
            memcpy(pb+len,&systemParams_usr.NightPeriod,1);
            len++;
            payloadpack(pb,len);
            free(pb);

        }
        else if(params[PARAMS_INDEX] == 0x32)
        {
					len = 0;
            pb = malloc(1);
            memcpy(pb+len,&systemParams_usr.vbat,1);
            len++;//放大10倍
            payloadpack(pb,len);
					   free(pb);
        }
        else if(params[PARAMS_INDEX] == 0x33)
        {
					  len = 0;
            pb = malloc(4);
            memcpy(pb+len,getGPS(),GPS_COUNT);
            len = len +GPS_COUNT;
            memcpy(pb+len,FilterData,SENSORS_COUNT);
            payloadpack(pb,SUM_COUNT);
					 free(pb);
        } 


    }
    else
    {
        if(params[PARAMS_INDEX] == 0x31)
        {
            systemParams_usr.Dhours= params[18];
            systemParams_usr.DayPeriod = params[19];
            systemParams_usr.Nhours= params[20];
            systemParams_usr.NightPeriod= params[21];
            systmeReconfig();
        }
        else if(params[PARAMS_INDEX] == 0x32)
        {
		systemParams_usr.vbatT= params[18];
		systmeReconfig();

        }
        else if(params[PARAMS_INDEX] == 0x33)
        {
        }
    }
    //free(pb);
}

unsigned char  LteAnaly(void)
{
    unsigned int recCRC,calCRC;
    uint32_t len;
    unsigned char result;
    unsigned char *ltebuf;
	 result = 1;
    ltebuf = &(GetLteStru()->lterxbuffer[1]);
    len = GetLteStru()->rxSize-HEADDER_LEN;
    
    if(GetLteStru()->lterxbuffer[0]==SERVER_HEADER)
    {
        len = 0;
        len = len +GetLteStru()->lterxbuffer[LENINDEX];
        len =len + ((GetLteStru()->lterxbuffer[LENINDEX+1])<<8);
        len =len +(( GetLteStru()->lterxbuffer[LENINDEX+2])<<16);
        len =len + ((GetLteStru()->lterxbuffer[LENINDEX+3])<<24);
        len = len +DEVID_LEN;
        len = len +PAYLOAD_LEN;
        calCRC=CRC_Compute(ltebuf,len-2);//计算所接收数据的CRC
        recCRC=ltebuf[len-1]|(((u16)ltebuf[len-2])<<8);//接收到的CRC
        if(calCRC!=recCRC||(calCRC == 0))//CRC校验错误
        {
            result = 1;
        }
        else
        {
            switch(ltebuf[CMD_INDEX])//读写解析
            {
            case 0x60://读
            {
                cmdResponse(1,ltebuf);
            }
            break;
            case 0x61://写
                cmdResponse(0,ltebuf);
                break;
            }
            result = 0;
        }
    }
    return result;
}


void payloadpack(unsigned char *p,uint32_t size)
{
    unsigned char *pb;
    unsigned int calCRC;
    uint32_t len,payload_len;
    pb = malloc(size+HEADDER_LEN+DEVID_LEN+PAYLOAD_LEN+CRC_LEN);//id:12   len:4  crc: 2 header:1
    pb[0] = NODE_HEADER;
    len = 1;//
    memcpy(&(pb[DEVID_INDEX]),CpuID,DEVID_LEN);//dev id
    len = len +DEVID_LEN;
    len = len +PAYLOAD_LEN;//paylaod len length
    memcpy(pb+len,p,size);//gps+imu
    len = len +size;
    len = len +CRC_LEN;//crc
	  payload_len = size + 2;
    pb[LENINDEX] = (unsigned char)(payload_len);
    pb[LENINDEX+1] = (unsigned char)(payload_len>>8);
    pb[LENINDEX+2] = (unsigned char)(payload_len>>16);
    pb[LENINDEX+3] = (unsigned char)(payload_len>>24);
    calCRC=CRC_Compute(pb+HEADDER_LEN,len-3);//计算所接收数据的CRC
    pb[len-1] = (unsigned char)calCRC;
    pb[len-2] = (unsigned char)(calCRC>>8);

    LteUart_SendByte(LTE_4G,pb,len);
		HAL_Delay(1000);
    free(pb);

}
unsigned char powerManage()
{
    uint32_t adc_value;
    float vbat_value;
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
    HAL_Delay(20);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,0xffff);
    adc_value = HAL_ADC_GetValue(&hadc1);
    vbat_value = adc_value*2.5/2048;
    systemParams_usr.vbat = (unsigned char)(vbat_value*10);
    if( systemParams_usr.vbat>VBAT_MAX_VALU|| systemParams_usr.vbat<systemParams_usr.vbatT)
        return 1;
    else
        return 0;

}
extern RTC_HandleTypeDef hrtc;
static void RTC_Config(uint32_t p)
{
    /*##-1- Configure the RTC peripheral #######################################*/
    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follows:
    - Hour Format    = Format 24
    - Asynch Prediv  = Value according to source clock
    - Synch Prediv   = Value according to source clock
    - OutPut         = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType     = Open Drain */
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0xF9
#define WAKEUP_TIMER_ENABLE 0x32F2
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV; /* 0x7F */
    hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV; /* (32 kHz / 128) - 1 = 0xF9 */
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    if(HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Calendar ultra-low power mode */
    if (HAL_RTCEx_SetLowPowerCalib(&hrtc, RTC_LPCAL_SET) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-2- Check if data stored in BackUp register1: Wakeup timer enable #######*/
    /* Read the Back Up Register 1 Data */
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) == WAKEUP_TIMER_ENABLE)
    {
        /* if the wakeup timer is enabled then deactivate it to disable the wakeup timer interrupt */
        if(HAL_RTCEx_DeactivateWakeUpTimer(&hrtc) != HAL_OK)
        {
            /* Initialization Error */
            Error_Handler();
        }
    }

    /*##-3- Configure the RTC Wakeup peripheral #################################*/
    /* Setting the Wakeup time to 1 s
    If RTC_WAKEUPCLOCK_CK_SPRE_16BITS is selected, the frequency is 1Hz,
    this allows to get a wakeup time equal to 1 s if the counter is 0x0 */
    SetWakeUp(p);
// HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0);

    /*##-4- Write 'wakeup timer enabled' tag in RTC Backup data Register 1 #######*/
//  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, WAKEUP_TIMER_ENABLE);

}



void powersleep(void)
{


    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    SensorsPowerDown();
    PowerDown();
//	 HAL_GPIO_WritePin(GPIOA, WP_Pin, GPIO_PIN_SET);
//   HAL_GPIO_WritePin(GPIOB, SPI_RESET_Pin, GPIO_PIN_SET);
//	 HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    gpioLowpowerTest(GPIOC, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|
                     GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|
                     GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
                     GPIO_PIN_14|GPIO_PIN_15, GPIO_MODE_INPUT,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOB,  GPIO_PIN_3|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_MODE_OUTPUT_PP,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOC,  GPIO_PIN_All, GPIO_MODE_INPUT,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOH,  GPIO_PIN_All, GPIO_MODE_INPUT,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOA,	GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|
                     GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|
                     GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_MODE_INPUT,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOA,	GPIO_PIN_3, GPIO_MODE_INPUT,GPIO_PULLDOWN);
    gpioLowpowerTest(GPIOA,	GPIO_PIN_3, GPIO_MODE_INPUT,GPIO_PULLUP);
    /* GPIO_InitStruct.Pin =GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);*/


//HAL_GPIO_WritePin(GPIOC, EN_3_3V_Pin, GPIO_PIN_RESET);
//HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_RESET);



    /* __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_DMA1_CLK_DISABLE();
    __HAL_RCC_DMA2_CLK_DISABLE();
    __HAL_RCC_USART1_CLK_DISABLE();
    __HAL_RCC_USART2_CLK_DISABLE();
    __HAL_RCC_USART3_CLK_DISABLE();*/

    __HAL_RCC_PWR_CLK_ENABLE();
    RTC_WAKEUP_Init(systemParams_usr.period);


    HAL_PWREx_EnableBORPVD_ULP();
//   HAL_SuspendTick();
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    HAL_PWREx_EnterSHUTDOWNMode();
//HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
// // HAL_PWR_EnterSTANDBYMode();
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

//   HAL_ResumeTick();
}
void DataUploadPeriod()
{

    if(getRTCTime()->Hours < systemParams_usr.Nhours&&getRTCTime()->Hours>=systemParams_usr.Dhours)  //23.00-18.0
    {
        systemParams_usr.period = systemParams_usr.DayPeriod;
    }
    else
    {
        systemParams_usr.period = systemParams_usr.NightPeriod;
    }

}
void test()
{
// SIMCOM_Register_Network();
//DataUploadPeriod();//数据上传周期控制
    HAL_GPIO_TogglePin(GPIOB, led_Pin);
    HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, EN_5V1_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, EN_5V2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, EN_3V8_Pin, GPIO_PIN_RESET);
    DataUploadPeriod();//数据上传周期控制
//  HAL_Delay(1000);
    snesors_process();//imu参数采集

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, EN_5V1_Pin|EN_3V8_Pin, GPIO_PIN_SET);
// HAL_Delay(1000);

    flash_process();
}
uint32_t ledtick;
void ledBLink()
{


    HAL_GPIO_TogglePin(GPIOB, led_Pin);
    HAL_Delay(500);


}
void app_main()
{

    static uint32_t tick;
    unsigned char pb[SUM_COUNT*PAYLOAD_COUNT];
    uint32_t len;
    static unsigned char SleepFLag;

    DataUploadPeriod();//数据上传周期控制
    ledBLink();
#if DEBUG_MODE == 0
    snesors_process();//imu参数采集
    SIMCOM_Register_Network();
#endif

//GetLteStru()->RetryConnectCount = MAX_CONNECT_COUNT + 1;
#if DEBUG_MODE == 0

    if(powerManage() == 1)
    {
        systemParams_usr.period = 120;
        powersleep();
    }
#endif
    if(GetLteStru()->NetStatus == SIMCOM_NET_OK)//解析服务器指�?
    {


        if(GetLteStru()->LteReceivedFlag)
        {

		   tick = HAL_GetTick();
            GetLteStru()->LteReceivedFlag = 0;
            if(LteAnaly()==0)//解析服务端命�?
                SleepFLag = 2;
        }
    }

    /***********************数据上报机制*********************************/
    if(GetLteStru()->NetStatus == SIMCOM_NET_OK&&SleepFLag ==0)
    {
        if(GetFLashStatus()->SumLen !=0)//数据断网续传
        {
            uint32_t i,len,len2,k;
            len = GetFLashStatus()->SumLen;
            if(len<=SUM_COUNT*PAYLOAD_COUNT)//数据分包
            {
                FlashReadOnebytes((GetFLashStatus()->LastReadAddr ),pb,GetFLashStatus()->SumLen,1);
                payloadpack(pb,len);

            }
            else  //分包续传
            {
                k = len/(SUM_COUNT*PAYLOAD_COUNT);
                len2 = k *(SUM_COUNT*PAYLOAD_COUNT);
                for(i=0; i<k; i++)
                {
                    FlashReadOnebytes((GetFLashStatus()->LastReadAddr),pb,SUM_COUNT*PAYLOAD_COUNT,1);
                    payloadpack(pb,SUM_COUNT*PAYLOAD_COUNT);

                }

                FlashReadOnebytes((GetFLashStatus()->LastReadAddr),pb,len-len2,1);
                payloadpack(pb,len-len2);
                GetFLashStatus()->SumLen = 0;
            }
            systmeReconfig();
        }
        else //定期上传
        {
            extern unsigned char gpsBuf[GPS_COUNT];
            getGPS();
            len = 0;
            memcpy(pb+len,gpsBuf,GPS_COUNT);
            len = len +GPS_COUNT;
            memcpy(pb+len,FilterData,SENSORS_COUNT*4);
            payloadpack(pb,SUM_COUNT);

        }
        SleepFLag = 1;
        // HAL_Delay(3000);
        // powersleep();
    }
    else
    {


        if(GetLteStru()->RetryConnectCount>MAX_CONNECT_COUNT )//断网数据存储
        {
            FlashDataStore(GetFLashStatus()->LastWriteAddr,(uint32_t *)getGPS(),GPS_COUNT/4);
            FlashDataStore(GetFLashStatus()->LastWriteAddr,(uint32_t *)FilterData,SENSORS_COUNT);
            SleepFLag = 1;
            if(DEBUG_MODE == 1)
            {
                ;//FlashReadOnebytes((GetFLashStatus()->LastReadAddr ),pb,GetFLashStatus()->SumLen,1);
            }
            else
            {
                ;//powersleep();

            }
        }

    }
#if DEBUG_MODE == 1
    HAL_Delay(3000);
    powersleep();
#endif
    if(SleepFLag == 0)
        tick = HAL_GetTick();
    if((HAL_GetTick()-tick)>=60000)
    {
        SleepFLag = 0;
        powersleep();
    }

    /***************************数据上报机制结束***********************************/


}
