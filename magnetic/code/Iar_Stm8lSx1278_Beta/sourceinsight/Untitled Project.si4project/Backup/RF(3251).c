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
#include "uart1.h"

//unsigned char VersionNumber[5] = {'1', '.', '0', '.', '0'} ;
INT16U SingleWait = 0;
unsigned char InitFlag = 0;
static INT8U recv_buffer[BUFFERSIZE]={ 0 }; //receive data buffer
volatile Module_mode_stru Module_mode;
volatile Module_Params_stru Module_Params;
unsigned int pream_long[6] = {7, 30, 61, 244, 488, 976};
//POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode


/*参数4*/
//extern unsigned char Channel ; //默认通道为23通道 433MHZ
unsigned char xorCheck(unsigned char *pbuffer,unsigned char len)
{
  unsigned char result;
  unsigned char i;
  result = 0;
  for(i=0;i<len;i++)
  {
    result = result^pbuffer[i];
  }
  return result;
}


//Module_mode_stru *GetModuleMode()
//{
//  Module_mode_stru *s;
//  s = &Module_mode;
//  return s;
//}
//
//Module_Params_stru *GetModuleParams()
//{
//  Module_Params_stru *s;
//  s = &Module_Params;
//  return s;
//}

void flash_operation(uint32_t addr,unsigned char *p,unsigned char size)
{
  unsigned char i;
  i =0;
  FLASH_Unlock(FLASH_MemType_Data);	 
  for(i=0;i<size;i++)
  {
    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i));
  }
  for(i=0;i<size;i++)
  {
    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+i, p[i]);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
  }
  FLASH_Lock(FLASH_MemType_Data);   
  
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
  Module_Params.ADDH = 0x01;
  Module_Params.ADDM = 0x04;
  
  Module_Params.ADDL = 0x03;
  
  Module_Params.Flash_Write_Done = 1;  
  Module_Params.AirRate = 2;
  Module_Params.Channel =0x17;
  Module_Params.SerialRate = 0x07;
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
  else
    ModuleInit();
  
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
  if( Module_Params.power*2<=14&&Module_Params.power>0)
  {
    SX1276_SetPower(Module_Params.power*2, 1);		  //配置RFO PIN输出，功率20dBm
  }
  else
    SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
  SX1276_SetRxMode();  
}



void moduleconfig()
{
  /*?ж?M0 M1 ????????????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 0 ??????
  *M0 = 0  M1 = 1 ?????
  *M0 = 1  M1 = 1 ??????
  */
  Module_mode.LoraM1Flag = GPIO_ReadInputDataBit(PORT_SX127X_M1, PIN_SX127X_M1);
  Module_mode.LoraM0Flag = GPIO_ReadInputDataBit(PORT_SX127X_M0, PIN_SX127X_M0);
  
  if((Module_mode.LoraM0Flag == 0) && (Module_mode.LoraM1Flag == 0))
    Module_mode.CurrentMode = NormalMode;
  else if((Module_mode.LoraM0Flag != 0) && (Module_mode.LoraM1Flag == 0))
    Module_mode.CurrentMode = WakeMode;
  else if((Module_mode.LoraM0Flag == 0) && (Module_mode.LoraM1Flag != 0))			  //??0 ??0x02
    Module_mode.CurrentMode = PowerSavingMode;
  else Module_mode.CurrentMode = SleepMode;
  
  if(Module_mode.LastMode != Module_mode.CurrentMode) { 							  //?????????仯???
    AUX_CONFIGURING();
    Module_mode.ConfigureDone = 0;
    RtcWakeUp = 0;
    RTC_DeInit();											//?????????? 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //???RTC??? 
    RTC_WakeUpCmd(DISABLE); 								//???RTC????????
    RTC_ITConfig(RTC_IT_WUT, DISABLE);						//???RTC?ж?
    Module_mode.LastMode = Module_mode.CurrentMode;
  }
  
}

void moduleNormalOperation()
{//uint32_t tx_cnt=0;uint32_t count_recv;
  INT8U length = 0;
  char i = 0;
 
  if(Module_mode.ConfigureDone) 
  {
   
   
    
    if( UsartReceiveFlag == 1 )
    {
      //SX1276_SetPreambleSize(8);
      // SX1276_SetRxMode();
      //?????????????????????Щ
      AUX_CONFIGURING();
      SX1276_SendPacket(UsartReceiveData, (usart_i));
      SX1276_SetRxMode();
     // tx_cnt = tx_cnt +usart_i;
      AUX_CONFIGURED();					
      usart_i = 0;
      LED_TOG();
      UsartReceiveFlag = 0;
     // SX1276_SetPreambleSize(2000);
     //  SX1276_SetPreambleSize(65530);
    }
    if( ExitInterFlag) 
    {
      ExitInterFlag = 0;
      length = SX1276_ReceivePacket(recv_buffer); 			// ?????????????????
       SX1276_SetRxMode();
      if(((Module_Params.ADDH == recv_buffer[2]) && (Module_Params.ADDM ==  recv_buffer[3])//id is native node id
         && (Module_Params.ADDL ==  recv_buffer[4]))||
         ((Module_Params.ADDH == recv_buffer[10]) && (Module_Params.ADDM ==  recv_buffer[11])//id is native node id
         && (Module_Params.ADDL ==  recv_buffer[12]))
         )
      {
        if(xorCheck(&recv_buffer[2],length-2)==recv_buffer[7]||xorCheck(&recv_buffer[2],length-2)==recv_buffer[8])
        //if(xorCheck(&recv_buffer[2],length-2)^==recv_buffer[7])//data check ok
        //  recv_buffer[7]
          length=length;
        else
          length=0;
      } 
      else 
      {
        length = 0;
      }
      if( length )
      {
        
        //count_recv = count_recv+length;
        AUX_CONFIGURING();
        DelayMs(50);
        for(i = 0; i < length; i++) 
        {
          USART_SendData8(USART1, recv_buffer[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        AUX_CONFIGURED();
        length = 0;
        LED_TOG();
        
      }
    }
  } 
  else 
  {
    AUX_CONFIGURING();
    Uart1_Init(Module_Params.SerialRate, Module_Params.CheckBit);							//????????????
    USART_SendStr("???+???????????\r\n");
    SX1276_SetPreambleSize(65530);
    Module_mode.ConfigureDone = 1;
    AUX_CONFIGURED();
  }
  free(recv_buffer);
}
void moduleWakeUpOperation()
{
  INT8U length = 0;
  char i = 0;
  INT8U recv_buffer[BUFFERSIZE]={ 0 }; //receive data buffer
  if(Module_mode.ConfigureDone) {
    SX1276_SetRxMode();
    if( UsartReceiveFlag == 1 )
    {
      SX1276_SetPreambleSize((pream_long[Module_Params.AirRate]*(Module_Params.WakeupTime/250)));	//??????972 BW 500kHZ SF?7 ????д??????250ms
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
        DelayMs(50);
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
    Uart1_Init(Module_Params.SerialRate, Module_Params.CheckBit);							//????????????
    USART_SendStr("???+???????????\r\n");
    SX1276_SetPreambleSize((pream_long[Module_Params.AirRate]*(Module_Params.WakeupTime/250))); //??????972 BW 500kHZ SF?7 ????д??????250ms
    Module_mode.ConfigureDone = 1;
    AUX_CONFIGURED();
  }
  free(recv_buffer);
}
void modulePowerSavingModeOperation()
{
  char i = 0;
   INT8U recv_buffer[BUFFERSIZE]={ 0 }; //receive data buffer
  INT8U length = 0;
  if(Module_mode.ConfigureDone) {
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
            length = SX1276_ReceivePacket(recv_buffer);		  // ?????????????????
            if( length )
            {
              AUX_CONFIGURING();
              DelayMs(50);
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
      Uart1_Init(Module_Params.SerialRate, Module_Params.CheckBit); 						//????????????
      SPI_Initial();			//SPI?????
    }
  } else {
    AUX_CONFIGURING();
    Uart1_Init(Module_Params.SerialRate, Module_Params.CheckBit); //????????????
    USART_SendStr("???+???????????\r\n");
    /*SX1276??????????ж?
    DIO3???CADDone??????ж?
    ??RTC???ж??
    */
    SX1276_SetPreambleSize(65530);
    RTC_Config(Module_Params.WakeupTime);
    enableInterrupts();
    Module_mode.ConfigureDone = 1;
    AUX_CONFIGURED();
  }
  free(recv_buffer);
}
void ModuleSleepModeOperation()
{
  unsigned char p[12];
  //INT8U recv_buffer[128]={ 0 };  //接收数据的长度以及数据所需存储的数组
  INT32U freq_tmp;
  INT8U tmp_76 = 0, tmp_543 = 0;			   //分别代表串口校验位、波特率
  
  if(Module_mode.ConfigureDone) {
    if ((SleepModeFlag == 1) && (InitFlag == 0))
    {
      AUX_CONFIGURING();
      SystemClock_Init();	   // ?????????
      GPIO_Initial();		   // ?????GPIO
      SPI_Initial();		   //SPI?????
      LED_Init();			   //????LED?????
      RF_Initial( );
      Uart1_Init(9600, 0);
      InitFlag = 1;
      AUX_CONFIGURED();
    } else if(InitFlag == 1) {
      switch (uartParase()) {
      case LORA_WRITE://频率，空中速率，发射功率，串口速率，串口效验，唤醒时间	  
        
        freq_tmp = UsartReceiveData[INDEX_FREQ]<<8+ UsartReceiveData[INDEX_FREQ+1];//<<8+
          //UsartReceiveData[INDEX_FREQ+2];
        Module_Params.Channel =  freq_tmp%433;		   
        Module_Params.AirRate = UsartReceiveData[INDEX_AIRATE];  
//        Module_Params.power =   UsartReceiveData[INDEX_POWER]; 
//        Module_Params.SerialRate = UsartReceiveData[INDEX_BURDRATE];
//        Module_Params.CheckBit = UsartReceiveData[INDEX_CHECK];
//        Module_Params.WakeupTime = UsartReceiveData[INDEX_WAKETIME];
//        Module_Params.ADDH = UsartReceiveData[INDEX_ADDH];
//        Module_Params.ADDM = UsartReceiveData[INDEX_ADDL];	
//        Module_Params.ADDL = UsartReceiveData[INDEX_ADDL+1];		
//        Module_Params.TranMode = UsartReceiveData[INDEX_MODE]; 
        Module_Params.Flash_Write_Done = 1;
        
        
       // memcpy(p,GetModuleParams(),10);
        flash_operation(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS,p,10);		  
        LED_TOG();
        module_process(LORA_WRITE);
        break;
      case LORA_READ: //
        USART_SendData8(USART1,  Module_Params.ADDH);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        USART_SendData8(USART1,  Module_Params.ADDL);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        
        if(Module_Params.CheckBit == 0x00)
          tmp_76 = 0x00;
        else if (Module_Params.CheckBit == 0x06)
          tmp_76 = 0x01;
        else tmp_76 = 0x02;
        tmp_543 = (unsigned char) (ceil(log2(Module_Params.SerialRate/1200)));
        USART_SendData8(USART1, (tmp_76 << 6 | tmp_543 << 3 | Module_Params.AirRate)); 
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        
        USART_SendData8(USART1,  Module_Params.Channel);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        
        // USART_SendData8(USART1, (( Module_Params.TranMode<<7) | (( Module_Params.WakeupTime/250-1)<<3)));
        // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        LED_TOG();
        module_process(LORA_READ);
        break;
      case NORMA_OPERATION:
        {
          module_process(NORMA_OPERATION);
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
    Module_mode.ConfigureDone = 1;
    SleepModeFlag = 0;    //?????????????????????????????ж??????????л??????SleepModeFlag??1
    InitFlag = 0;
    EnterStopMode();
  }
  free(p);
}
//#define VersionNumber 1.1.1

void lora_process()
{
  
  switch (Module_mode.CurrentMode) {
  case NormalMode:
    moduleNormalOperation();
    break;
  case WakeMode:
    moduleWakeUpOperation();
    break;
  case PowerSavingMode:
    modulePowerSavingModeOperation();
    break;
  case SleepMode:
    ModuleSleepModeOperation();
    break;
  default:
    break;
  }
  
}
