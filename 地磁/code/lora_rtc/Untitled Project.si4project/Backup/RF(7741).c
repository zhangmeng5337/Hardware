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
u8 CurrentMode = 0;
u8 ConfigureDone = 0;
u8 lora_mode_satus;//dio0 ira flag 
u8 length_tmp;
INT8U length = 0;  //every payload length 
unsigned char RtcWakeUp_tmp;
uint32_t receive_timeout; //
unsigned int scan_timeout;
u16 wake_up_period=384;//19------>500ms
POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer




void RF_Initial( )
{
  CurrentMode =PowerSavingMode ;//NormalMode  PowerSavingMode 
  lora_mode_satus = 0;
  SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
  SX1276_LoRa_SetDataRate(2);
  SX1276_SetFreq(0);				//配置信道为23，即433Hz
  SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
  SX1276_SetPreambleSize(65530);
  SX1276_SetRxMode(); 
}
u8 GetPowerMode()
{
  return CurrentMode;
}

void lora_rx_process()

{
  //if(lora_mode_satus == 1)
  {
    length = SX1276_ReceivePacket(recv_buffer);			 // 检测是否收到一个数据包   
    if( length )
    {
      #if debug == 1
        rx_length = rx_length + length +4;
      #endif
      length_tmp = length+length_tmp;
      length = 0; 
    }  
    
  }
  
}
void PowerSavingMode_setting()
{   
   char i = 0; 
  if(ConfigureDone == 1)
  {
    if(RtcWakeUp !=0)
    {
      
      if(RtcWakeUp == 1)
      {   
        RtcWakeUp = 2;
        RtcWakeUp_tmp = 2;
        lora_mode_satus = 0;
        receive_timeout=0;             
        SX1276_SetPreambleSize(65530);//preamble 
        SX1276_CADMode();
        receive_timeout=0;     
      }
      
      while(CadDoneFlag==1)//preamble detect
      {      
        if(CadDoneFlag == 1)
        {
          RtcWakeUp_tmp = 0;
          receive_timeout=0;
          RTC_WakeUpCmd(DISABLE);
          SX1276_SetRxMode();
          CadDoneFlag = 0;         
        }
        while(receive_timeout <= 200000)//effect data timeout calculate
        {
          receive_timeout = receive_timeout + 1;
          if(lora_mode_satus==1)//effect data irq
          { 
            if(receive_timeout>=200000)
              receive_timeout = 500000;
            LED_TOG();
            lora_rx_process();    //data receive  
            for(i=0;i<length_tmp;i++)
            {
              USART_SendData8(USART1, recv_buffer[i]);
              while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            }
            length_tmp = 0;   
            receive_timeout=500000;
            disableInterrupts();
            RTC_Config(wake_up_period);
            enableInterrupts();          
            SX1276_LoRa_SetMode( LORA_MODE_SLEEP ); 
            lora_mode_satus = 2;
            //RTC_WakeUpCmd(ENABLE);
          }                   
        }       
      }
        scan_timeout=scan_timeout+1;  //no cad timeout calculate    
        if(lora_mode_satus!=2&&scan_timeout>=10000)
        {
          scan_timeout=0;
          lora_mode_satus = 0;
          receive_timeout = 0; 
          SX1276_LoRa_SetMode( LORA_MODE_SLEEP );
          disableInterrupts();
          RTC_WakeUpCmd(ENABLE); 
          enableInterrupts();          
         EnterStopMode();//enter stop mode 
          HardwareInit(); // hardare init after wake up       
        }
      
    }
    
  } 
  else
  {
    disableInterrupts();
    GPIO_Init(PORT_SX127X_DIO3, PIN_SX127X_DIO3, GPIO_Mode_In_FL_IT);
    GPIO_Init(PORT_SX127X_DIO1, PIN_SX127X_DIO1, GPIO_Mode_In_FL_IT);  
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);    
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
    RTC_Config(wake_up_period);
    enableInterrupts();
    ConfigureDone = 1; 
  }
  
}