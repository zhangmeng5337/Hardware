#include "app.h"
#include "sensors.h"
#include "LteHal.h"
#include "LteHardware.h"
#include  "MX25_CMD.h"
#include "EEPROM.h"
#include "stdlib.h"
#include "crc.h"
#include "rtc.h"
systemParams_stru systemParams_usr;

extern float FilterData[SN];//滤波后的imu数据
void HardwareInit()
{
    LteUartConfig();
#if ROLE == 0
    LtePowerManage(LTE_4G,ON);
#else
    LtePowerManage(LTE_NBIOT,ON);
#endif
    sensors_Init();
}

 uint32_t CpuID[3];
 uint32_t Lock_Code;
void GetLockCode(void)
{
//获取CPU唯一ID
CpuID[0]=*(volatile uint32_t*)(0x1ffff7e8);
CpuID[1]=*(volatile uint32_t*)(0x1ffff7ec);
CpuID[2]=*(volatile uint32_t*)(0x1ffff7f0);
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
    flash_read(addr++,&p,1);
    if(p!=0x5a)
    {
        systemParams_usr.period = 120000;
        p = 0x5a;  //写入标志
        flash_init(0);
        addr = 0;
        flash_write(addr++, &p,1);
        flash_write( addr++,&(FLashData_usr2->LastWriteAddr),1);
        flash_write( addr++,&(FLashData_usr2->SumLen),1);

        systemParams_usr.DayPeriod = 1;
        systemParams_usr.NightPeriod = 10;
        systemParams_usr.Dhours = 6;
        systemParams_usr.Nhours = 18;
        flash_write( addr++,(uint32_t *)&(systemParams_usr.DayPeriod),1);
        flash_write( addr++,(uint32_t *)&(systemParams_usr.NightPeriod),1);
        flash_write( addr++,(uint32_t *)&(systemParams_usr.Dhours),1);
        flash_write( addr++,(uint32_t *)&(systemParams_usr.Nhours),1);




    }
    else
    {

        addr = 0;
        flash_read( addr++, &p,1);
        flash_read( addr++, &(FLashData_usr2->LastWriteAddr),1);
        flash_read( addr++,  &(FLashData_usr2->SumLen),1);

        flash_read( addr++,  (uint32_t *)&(systemParams_usr.DayPeriod),1);
        flash_read( addr++, (uint32_t *) &(systemParams_usr.NightPeriod),1);
        flash_read( addr++,  (uint32_t *)&(systemParams_usr.Dhours),1);
        flash_read( addr++,  (uint32_t *)&(systemParams_usr.Nhours),1);

    }
	GetLockCode();

}
void systmeReconfig()
{
    uint32_t p;
    uint32_t addr;
	p = 0x5a;  //写入标志
	flash_init(0);
	addr = 0;
	flash_write(addr++, &p,1);
	flash_write( addr++,&(GetFLashStatus()->LastWriteAddr),1);
	flash_write( addr++,&(GetFLashStatus()->SumLen),1);
	
	flash_write( addr++,(uint32_t *)&(systemParams_usr.DayPeriod),1);
	flash_write( addr++,(uint32_t *)&(systemParams_usr.NightPeriod),1);
	flash_write( addr++,(uint32_t *)&(systemParams_usr.Dhours),1);
	flash_write( addr++,(uint32_t *)&(systemParams_usr.Nhours),1);


}
unsigned char  LteAnaly(void)
{
    unsigned int recCRC,calCRC;
    uint32_t len;
    unsigned char result;
    uint8_t *pb;
    pb = malloc(GetLteStru()->rxSize);
    result = 0;
    if(GetLteStru()->lterxbuffer[0]==SERVER_HEADER)
    {
        memcpy(pb,&(GetLteStru()->lterxbuffer[1]),GetLteStru()->rxSize-HEADDER_LEN);
        len = *(uint32_t *)GetLteStru()->lterxbuffer[LENINDEX]+DEVID_LEN+PAYLOAD_LEN;
        calCRC=CRC_Compute(pb,len-2);//计算所接收数据的CRC
        recCRC=pb[len-1]|(((u16)pb[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
        if(calCRC!=recCRC||(calCRC == 0))//CRC校验错误
        {
            result = 1;
        }
        else
        {
            switch(pb[CMD_INDEX])
            {
            case 0x60:

                break;
            case 0x61:
                if(pb[PARAMS_INDEX] == 0x31)
                {
					systemParams_usr.Dhours= pb[20];
					systemParams_usr.DayPeriod = pb[21];
					systemParams_usr.Nhours= pb[23];
					systemParams_usr.NightPeriod= pb[24];
                    systmeReconfig();
                }
                break;
            }
            result = 0;
        }
    }
    free(pb);
		return result;
}


void payloadpack(unsigned char *p,uint32_t size)
{
    unsigned char *pb;
    unsigned int calCRC;
    uint32_t len;
    pb = malloc(size+HEADDER_LEN+DEVID_LEN+PAYLOAD_LEN+CRC_LEN);//id:12   len:4  crc: 2 header:1
    pb[0] = NODE_HEADER;
    len = 0;//header length is not included
    memcpy(&(pb[DEVID_INDEX]),CpuID,DEVID_LEN);//dev id
    len = len +DEVID_LEN;
    len = len +PAYLOAD_LEN;//paylaod len length
    memcpy(pb+len,p,size);//gps+imu
    len = len +size;
    len = len +CRC_LEN;//crc
    pb[LENINDEX] = (unsigned char)len;
    pb[LENINDEX+1] = (unsigned char)(len>>8);
    pb[LENINDEX+2] = (unsigned char)(len>>16);
    pb[LENINDEX+3] = (unsigned char)(len>>24);
    calCRC=CRC_Compute(pb+HEADDER_LEN,len);//计算所接收数据的CRC
    pb[len] = (unsigned char)calCRC;
    pb[len-1] = (unsigned char)(calCRC>>8);
	
    LteUart_SendByte(LTE_4G,pb,len+HEADDER_LEN);
    free(pb);

}
void powersleep(void)
{


		GPIO_InitTypeDef GPIO_InitStruct = {0};
	
		/* GPIO Ports Clock Enable */
	
	
	
		/*Configure GPIO pins : DTR_Pin EN_3_3V_Pin SIM_PWR_Pin CTRL3__Pin
								 CTRL1__Pin */
		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
							  GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|
							  GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
		GPIO_InitStruct.Pin =GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
		GPIO_InitStruct.Pin = GPIO_PIN_3;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	
		GPIO_InitStruct.Pin =GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	
		GPIO_InitStruct.Pin = GPIO_PIN_All;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	
	
	
	
	
	
	
		GPIO_InitStruct.Pin =GPIO_PIN_All;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	
		//HAL_GPIO_WritePin(GPIOC, EN_3_3V_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_RESET);
	
	
	
		__HAL_RCC_GPIOC_CLK_DISABLE();
		__HAL_RCC_GPIOH_CLK_DISABLE();
		__HAL_RCC_GPIOA_CLK_DISABLE();
		__HAL_RCC_GPIOB_CLK_DISABLE();
		__HAL_RCC_GPIOD_CLK_DISABLE();
		__HAL_RCC_DMA1_CLK_DISABLE();
		__HAL_RCC_DMA2_CLK_DISABLE();
		__HAL_RCC_USART1_CLK_DISABLE();
		__HAL_RCC_USART2_CLK_DISABLE();
		__HAL_RCC_USART3_CLK_DISABLE();

	
		 RTC_WAKEUP_Init(systemParams_usr.period);
		__HAL_RCC_PWR_CLK_ENABLE();
		HAL_SuspendTick();
		__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		//	 HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
		HAL_PWR_EnterSTANDBYMode();
		__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		HAL_ResumeTick();

}
void DataUploadPeriod()
{
    unsigned char *p;
    p = getRTC();
    if(p[3] < systemParams_usr.Nhours&&p[3]>=systemParams_usr.Dhours)  //23.00-18.0
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
    snesors_process();//imu参数采集
}
void app_main()
{

    uint32_t tick;
    unsigned char *pb;
    uint32_t len;
    SIMCOM_Register_Network();
    DataUploadPeriod();//数据上传周期控制

    if(GetLteStru()->NetStatus == SIMCOM_NET_OK)//解析服务器指令
    {

        RTC_Calibration(GetLteTime());//时间校准
        if(GetLteStru()->LteReceivedFlag)
        {
            GetLteStru()->LteReceivedFlag = 0;
            LteAnaly();//解析服务端命令
        }
    }

	/***********************数据上报机制*********************************/
    if((HAL_GetTick()-tick)>=(systemParams_usr.period*60000))//周期上报数据
    {
        tick = HAL_GetTick();
        snesors_process();//imu参数采集
        if(GetLteStru()->RetryConnectCount>MAX_CONNECT_COUNT )//断网数据存储
        {

            FlashDataStore(GetFLashStatus()->LastWriteAddr,(uint32_t *)FilterData,12);
            powersleep();
        }

        if(GetLteStru()->NetStatus == SIMCOM_NET_OK)
        {
            if(GetFLashStatus()->SumLen !=0)//数据断网续传
            {
                uint32_t i,len,len2,k;
                uint8_t *pb;
                pb = malloc(1400);
                len = GetFLashStatus()->SumLen;
                if(len<=1400)//数据分包
                {
                   // for(i=0; i<GetFLashStatus()->SumLen; i++)
                    {
                        FlashRead4bytes((GetFLashStatus()->LastWriteAddr),pb,GetFLashStatus()->SumLen);

                    }
                    payloadpack(pb,GetFLashStatus()->SumLen);
				    GetFLashStatus()->LastWriteAddr = GetFLashStatus()->SumLen + len;
					if(GetFLashStatus()->LastWriteAddr>DATA_MAX_ADDR)
					{
						GetFLashStatus()->LastWriteAddr= GetFLashStatus()->LastWriteAddr-DATA_MAX_ADDR;
					}
				    GetFLashStatus()->SumLen = 0;
                }
                else  //分包续传
                {
                    k = len/1400;
                    len2 = k *1400;
                    for(i=0; i<k; i++)
                    {
                        FlashRead4bytes((GetFLashStatus()->LastWriteAddr+i),pb,1400);
                        payloadpack(pb,1400);
                    }

                    FlashRead4bytes((GetFLashStatus()->LastWriteAddr+i),pb,len-len2);
                    payloadpack(pb,len-len2);
				    GetFLashStatus()->LastWriteAddr = GetFLashStatus()->SumLen + len;

					if(GetFLashStatus()->LastWriteAddr>DATA_MAX_ADDR)
					{
						GetFLashStatus()->LastWriteAddr= GetFLashStatus()->LastWriteAddr-DATA_MAX_ADDR;
					}
				    GetFLashStatus()->SumLen = 0;
                }
                free(pb);
				systmeReconfig();
            }
            else //定期上传
            {
                pb = malloc(60+12);//每组数据包含60个gps数据，12个imu数据
                memcpy(pb+len,getGPS(),60);
                len = len +60;
                memcpy(pb+len,FilterData,12);
                payloadpack(pb,72);
                free(pb);
            }
            powersleep();
        }
    }
  /***************************数据上报机制结束***********************************/

}
