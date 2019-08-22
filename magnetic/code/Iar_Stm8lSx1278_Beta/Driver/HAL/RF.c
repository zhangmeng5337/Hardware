#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_flash.h"
#include "RF.h"
#include "stm8l15x_flash.h"
#include "uartParase.h"

Module_mode_stru Module_mode;
Module_Params_stru Module_Params;
unsigned int pream_long[6] = {7, 30, 61, 244, 488, 976};
POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer

/*参数4*/
extern unsigned char Channel ; //默认通道为23通道 433MHZ

POWER_MODE_Stru *GetPowerMode()
{
  POWER_MODE_Stru *s;
  return s;
}
Module_Params_stru *GetModuleParams()
{
  Module_Params_stru *s;
  return s;
}

Module_mode_stru *GetModuleMode()
{
  Module_mode_stru *s;
  return s;
}

void ModuleInit()
{
  Module_mode.AUX = 0;

  Module_mode.ConfigureDone = 0;
  Module_mode.CurrentMode = 0;
  Module_mode.LastMode = 0;
  Module_mode.LoraM0Flag = 0;
  Module_mode.LoraM1Flag = 0;
  Module_Params.CheckBit = 0;  
  Module_Params.ADDH = 0;
  Module_Params.ADDL = 0;
  Module_Params.AirRate = 2;
  Module_Params.Channel =0x17;
  //Module_Params.Flash_Data[] =
  Module_Params.Flash_Write_Done = 1;
  Module_Params.SerialRate = 0x02;
  Module_Params.WakeupTime = 250;
  
  
  
}
void RF_Initial( )
{
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);  //Flash 初始化
  
  if(FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 9)) == 1)  //判断Flash中是否有数据
  {
  
    Module_Params.Channel = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
    Module_Params.AirRate = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
    Module_Params.power = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
    Module_Params.SerialRate = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3));
    Module_Params.CheckBit = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4));
    Module_Params.WakeupTime = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5));	
	Module_Params.ADDH = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 6));	
    Module_Params.ADDL = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 7));	
    Module_Params.TranMode = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 8));	
  }  
 
  SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
  switch( Module_Params.AirRate)
  	{
	 case 0x00:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//0.3kbps,
	 case 0x01:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//1.2kbps
	 case 0x02:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//2.4kbps
	 case 0x03:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//4.8kbps
	 case 0x04:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//9.6kbps
	 case 0x05:  SX1276_LoRa_SetDataRate(Module_Params.AirRate);break;//19.2kbps
	 default:  SX1276_LoRa_SetDataRate(0x03);break;//4.8kbps
  }

  if(Module_Params.Channel<32)
  	SX1276_SetFreq(Module_Params.Channel);				//配置信道为23，即433Hz
  else
  	SX1276_SetFreq(23);				//配置信道为23，即433Hz 
  if( Module_Params.power*2<=14)
  	{
	  SX1276_SetPower(Module_Params.power*2, 1);		  //配置RFO PIN输出，功率20dBm
    }
  else
  	SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
    SX1276_SetRxMode();  
}


void lora_rx_process()

{
  if(GetPowerMode()->status  == 1)
  {
    POWER_MODE.length = SX1276_ReceivePacket(recv_buffer);			 // 妫�娴嬫槸鍚︽敹鍒颁竴涓暟鎹寘   
    if( POWER_MODE.length )
    {
#if debug == 1
      POWER_MODE.RxLengthSum =  POWER_MODE.RxLengthSum +  POWER_MODE.length +4;
#endif
      POWER_MODE.length_tmp = POWER_MODE.length+POWER_MODE.length_tmp;
      POWER_MODE.length = 0; 
    }  
    
  }
  
}
void moduleconfig()
{
  /*?ж?M0 M1 ????????????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 0 ??????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 1 ??????
  */
  GetModuleMode()->LoraM1Flag = GPIO_ReadInputDataBit(PORT_SX127X_M1, PIN_SX127X_M1);
  GetModuleMode()->LoraM0Flag = GPIO_ReadInputDataBit(PORT_SX127X_M0, PIN_SX127X_M0);
  
  if((GetModuleMode()->LoraM0Flag == 0) && (GetModuleMode()->LoraM1Flag == 0))
    GetModuleMode()->CurrentMode = NormalMode;
  else if((GetModuleMode()->LoraM0Flag != 0) && (GetModuleMode()->LoraM1Flag == 0))
    GetModuleMode()->CurrentMode = WakeMode;
  else if((GetModuleMode()->LoraM0Flag == 0) && (GetModuleMode()->LoraM1Flag != 0))			  //??0 ??0x02
    GetModuleMode()->CurrentMode = PowerSavingMode;
  else GetModuleMode()->CurrentMode = SleepMode;
  
  if(GetModuleMode()->LastMode != GetModuleMode()->CurrentMode) { 							  //?????????仯???
    AUX_CONFIGURING();
    GetModuleMode()->ConfigureDone = 0;
    RtcWakeUp = 0;
    RTC_DeInit();											//?????????? 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //???RTC??? 
    RTC_WakeUpCmd(DISABLE); 								//???RTC????????
    RTC_ITConfig(RTC_IT_WUT, DISABLE);						//???RTC?ж?
    GetModuleMode()->LastMode = GetModuleMode()->CurrentMode;
  }
  
}

//#define VersionNumber 1.1.1
unsigned char VersionNumber[5] = {'1', '.', '0', '.', '0'} ;
INT16U SingleWait = 0;
unsigned char InitFlag = 0;
void lora_process()
{
  INT8U length = 0, recv_buffer[128]={ 0 };  //接收数据的长度以及数据所需存储的数组
  INT8U tmp_76 = 0, tmp_543 = 0;             //分别代表串口校验位、波特率
  char i = 0;
  switch (GetModuleMode()->CurrentMode) {
  case NormalMode:
    if(GetModuleMode()->ConfigureDone) {
      SX1276_SetRxMode();
     // if( UsartReceiveFlag == 1 )
      {
        //??????????	??????????鷢??????????ó????????飬??????????????????鷢???????????????????飬?????????
        SX1276_SetPreambleSize(8);								//?????????????????????Щ
        SX1276_SetRxMode();
        AUX_CONFIGURING();
        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
        //USART_SendData8(USART1, (usart_i-1));
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        for(i = 0; i < (usart_i-1); i++) {
          USART_SendData8(USART1, UsartReceiveData[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        AUX_CONFIGURED();
        SX1276_SetPreambleSize(65530);							//???????????????????????????????????????????
        usart_i = 0;
        LED_TOG();
        UsartReceiveFlag = 0;
      }
      //AUX_CONFIGURED();
      if( ExitInterFlag) {
        ExitInterFlag = 0;
        length = SX1276_ReceivePacket(recv_buffer); 			// ?????????????????
        if( length )
        {
          AUX_CONFIGURING();
          delay_ms(50);
          for(i = 0; i < length; i++) {
            USART_SendData8(USART1, recv_buffer[i]);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
          }
          AUX_CONFIGURED();
          length = 0;
          LED_TOG();
        }
      }
    } else {
      AUX_CONFIGURING();
      Uart1_Init(GetModuleParams()->SerialRate, GetModuleParams()->CheckBit);							//????????????
      USART_SendStr("???+???????????\r\n");
      SX1276_SetPreambleSize(65530);
      GetModuleMode()->ConfigureDone = 1;
      AUX_CONFIGURED();
    }
    break;
  case WakeMode:
    if(GetModuleMode()->ConfigureDone) {
      SX1276_SetRxMode();
      if( UsartReceiveFlag == 1 )
      {
        SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));	//??????972 BW 500kHZ SF?7 ????д??????250ms
        SX1276_SetRxMode();
        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
        USART_SendData8(USART1, (usart_i-1));
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        for(i = 0; i < (usart_i-1); i++) {
          USART_SendData8(USART1, UsartReceiveData[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        SX1276_SetPreambleSize(65530);							//???????????????????????????????????????????
        usart_i = 0;
        LED_TOG();
        UsartReceiveFlag = 0;
      }
      if( ExitInterFlag) {
        ExitInterFlag = 0;
        length = SX1276_ReceivePacket(recv_buffer); 			// ?????????????????
        if( length )
        {
          AUX_CONFIGURING();
          delay_ms(50);
          //USART_SendData8(USART1, length);
          //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
          for(i = 0; i < length; i++) {
            USART_SendData8(USART1, recv_buffer[i]);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
          }
          AUX_CONFIGURED();
          length = 0;
          LED_TOG();
        }
      }
    } else {
      AUX_CONFIGURING();
      Uart1_Init(GetModuleParams()->SerialRate, GetModuleParams()->CheckBit);							//????????????
      USART_SendStr("???+???????????\r\n");
      SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));	//??????972 BW 500kHZ SF?7 ????д??????250ms
      GetModuleMode()->ConfigureDone = 1;
      AUX_CONFIGURED();
    }
    break;
  case PowerSavingMode:
    if(GetModuleMode()->ConfigureDone) {
      if(RtcWakeUp == 1)
      {
        RtcWakeUp = 0;
        SX1276_CADMode();
        while (SingleWait <= 20000) {
          SingleWait ++;
          if(CadDoneFlag == 1) {
            RTC_WakeUpCmd(DISABLE);
            RTC_ITConfig(RTC_IT_WUT, DISABLE);
            SX1276_SetRxMode();
            if( ExitInterFlag) {
              ExitInterFlag = 0;
              length = SX1276_ReceivePacket(recv_buffer); 		  // ?????????????????
              if( length )
              {
                AUX_CONFIGURING();
                delay_ms(50);
                for(i = 0; i < length; i++) {
                  USART_SendData8(USART1, recv_buffer[i]);
                  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                }
                AUX_CONFIGURED();
                length = 0;
                SingleWait = 20001;
                CadDoneFlag = 0;
              }
            }
          }
        }
        SingleWait = 0;
        RTC_ITConfig(RTC_IT_WUT, ENABLE);
        RTC_WakeUpCmd(ENABLE);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==0);
        EnterStopMode();
	
        SystemClock_Init(); 	// ?????????
        GPIO_Initial(); 		// ?????GPIO
        Uart1_Init(GetModuleParams()->SerialRate, GetModuleParams()->CheckBit);							//????????????
        SPI_Initial();			//SPI?????
      }
    } else {
      AUX_CONFIGURING();
      Uart1_Init(GetModuleParams()->SerialRate, GetModuleParams()->CheckBit); //????????????
      USART_SendStr("???+???????????\r\n");
      /*SX1276??????????ж?
      DIO3???CADDone??????ж?
      ??RTC???ж??
      */
      SX1276_SetPreambleSize(65530);
      RTC_Config(GetModuleParams()->WakeupTime);
      enableInterrupts();
      GetModuleMode()->ConfigureDone = 1;
      AUX_CONFIGURED();
    }
    break;
  case SleepMode:
    if(GetModuleMode()->ConfigureDone) {
      if ((SleepModeFlag == 1) && (InitFlag == 0))
      {
        AUX_CONFIGURING();
        SystemClock_Init(); 	// ?????????
        GPIO_Initial(); 		// ?????GPIO
        SPI_Initial();			//SPI?????
        LED_Init(); 			//????LED?????
        RF_Initial( );
        Uart1_Init(9600, 0);
        InitFlag = 1;
        AUX_CONFIGURED();
      } else if(InitFlag == 1) {
          switch (uartParase()) {
          case LORA_WRITE://频率，空中速率，发射功率，串口速率，串口效验，唤醒时间	   
            INT32U freq_tmp;
		    freq_tmp = UsartReceiveData[INDEX_FREQ]<<16+ UsartReceiveData[INDEX_FREQ+1]<<8+
		                UsartReceiveData[INDEX_FREQ+2];
		    GetModuleParams()->Channel =	freq_tmp%433;		    
			GetModuleParams()->AirRate = UsartReceiveData[INDEX_AIRATE];  
			GetModuleParams()->power =   UsartReceiveData[INDEX_POWER]; 
		    GetModuleParams()->SerialRate = UsartReceiveData[INDEX_BURDRATE];
		    GetModuleParams()->CheckBit = UsartReceiveData[INDEX_CHECK];
            GetModuleParams()->WakeupTime = UsartReceiveData[INDEX_WAKETIME];
		    GetModuleParams()->ADDH = UsartReceiveData[INDEX_ADDH];
		    GetModuleParams()->ADDL = UsartReceiveData[INDEX_ADDL];    
		    GetModuleParams()->TranMode = UsartReceiveData[INDEX_MODE]; 

            GetModuleParams()->Flash_Write_Done = 1;

            FLASH_Unlock(FLASH_MemType_Data);      
            FLASH_EraseByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
            FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, GetModuleParams()->Channel);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);
         
            
            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1), GetModuleParams()->AirRate);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);

            
            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2), GetModuleParams()->power);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);

            
            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+3));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3), GetModuleParams()->SerialRate);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);

            
            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+4));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4), GetModuleParams()->CheckBit);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);


            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+5));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5), GetModuleParams()->WakeupTime);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);

            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+6));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 6), GetModuleParams()->ADDH);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);


            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+7));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 7), GetModuleParams()->ADDL);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);


            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+8));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 8), GetModuleParams()->TranMode);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);
            FLASH_Lock(FLASH_MemType_Data);

            FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+9));
            FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 9), GetModuleParams()->Flash_Write_Done);
            FLASH_WaitForLastOperation(FLASH_MemType_Data);
            FLASH_Lock(FLASH_MemType_Data);            
            LED_TOG();
            break;
          case LORA_READ: //
            USART_SendData8(USART1,  GetModuleParams()->ADDH);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            USART_SendData8(USART1,  GetModuleParams()->ADDL);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            
            if(GetModuleParams()->CheckBit == 0x00)
              tmp_76 = 0x00;
            else if (GetModuleParams()->CheckBit == 0x06)
              tmp_76 = 0x01;
            else tmp_76 = 0x02;
            tmp_543 = (unsigned char) (ceil(log2(GetModuleParams()->SerialRate/1200)));
            USART_SendData8(USART1, (tmp_76 << 6 | tmp_543 << 3 | GetModuleParams()->AirRate)); 
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            
            USART_SendData8(USART1,  GetModuleParams()->Channel);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            
           // USART_SendData8(USART1, (( GetModuleParams()->TranMode<<7) | (( GetModuleParams()->WakeupTime/250-1)<<3)));
           // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            LED_TOG();
            break;
          case 0xC3:
            if((UsartReceiveData[1] == 0xC3) && (UsartReceiveData[2] == 0xC3))
            {
              for(i = 0; i < 5; i++) {
                USART_SendData8(USART1, VersionNumber[i]);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
              }
            }
            break; 
          case 0xC4:
            if((UsartReceiveData[1] == 0xC4) && (UsartReceiveData[2] == 0xC4))
            {
              WWDG->CR = 0x80;
            }
            break;
          }
          UsartReceiveFlag = 0;
      }
    } else {
      GetModuleMode()->ConfigureDone = 1;
      SleepModeFlag = 0;	//?????????????????????????????ж??????????л??????SleepModeFlag??1
      InitFlag = 0;
      EnterStopMode();
    }
    break;
  default:
    break;
  }
  
}
