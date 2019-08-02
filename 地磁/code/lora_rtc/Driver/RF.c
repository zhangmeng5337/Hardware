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


POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer




void RF_Initial( )
{

  POWER_MODE.ConfigureDone = 0;
  POWER_MODE.CurrentMode = PowerSavingMode;
  POWER_MODE.status = 0;
  POWER_MODE.wake_up_period = 384;
  
  SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
  SX1276_LoRa_SetDataRate(2);
  SX1276_SetFreq(0);				//配置信道为23，即433Hz
  SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
  SX1276_SetPreambleSize(65530);
  SX1276_SetRxMode(); 
}
POWER_MODE_Stru GetPowerMode()
{
  return POWER_MODE;
}

void lora_rx_process()

{
  if(GetPowerMode().status  == 1)
  {
    POWER_MODE.length = SX1276_ReceivePacket(recv_buffer);			 // 检测是否收到一个数据包   
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
void PowerSavingMode_setting()
{   
   char i = 0; 
  if(POWER_MODE.ConfigureDone == 1)
  {
    if(POWER_MODE.RtcWakeUp !=0)
    {
      
      if(POWER_MODE.RtcWakeUp == 1)
      {   
        POWER_MODE.RtcWakeUp = 2;
      //  POWER_MODE.RtcWakeUp_tmp = 2;
        POWER_MODE.status = 0;
        POWER_MODE.receive_timeout=0;             
        SX1276_SetPreambleSize(65530);//preamble 
        SX1276_CADMode();
        POWER_MODE.receive_timeout=0;     
      }
      
      while(POWER_MODE.CadDoneFlag==1)//preamble detect
      {      
        //if(POWER_MODE.CadDoneFlag == 1)
        //{
        //  POWER_MODE.RtcWakeUp_tmp = 0;
          POWER_MODE.receive_timeout=0;
          RTC_WakeUpCmd(DISABLE);
          SX1276_SetRxMode();
          POWER_MODE.CadDoneFlag = 0;         
        //}
        while(POWER_MODE.receive_timeout <= 200000)//effect data timeout calculate
        {
          POWER_MODE.receive_timeout = POWER_MODE.receive_timeout + 1;
          if(POWER_MODE.status==1)//effect data irq
          { 
            if(POWER_MODE.receive_timeout>=200000)
              POWER_MODE.receive_timeout = 500000;
            LED_TOG();
            lora_rx_process();    //data receive  
            for(i=0;i<POWER_MODE.length_tmp;i++)
            {
              USART_SendData8(USART1, recv_buffer[i]);
              while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            }
            POWER_MODE.length_tmp = 0;   
            POWER_MODE.receive_timeout=500000;
            disableInterrupts();
            RTC_Config(POWER_MODE.wake_up_period);
            enableInterrupts();          
            SX1276_LoRa_SetMode( LORA_MODE_SLEEP ); 
            POWER_MODE.status = 2;
            //RTC_WakeUpCmd(ENABLE);
          }                   
        }       
      }
        POWER_MODE.scan_timeout=POWER_MODE.scan_timeout+1;  //no cad timeout calculate    
        if(POWER_MODE.status!=2&&POWER_MODE.scan_timeout>=10000)
        {
          POWER_MODE.scan_timeout=0;
          POWER_MODE.status = 0;
          POWER_MODE.receive_timeout = 0; 
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
    RTC_Config(POWER_MODE.wake_up_period);
    enableInterrupts();
    POWER_MODE.ConfigureDone = 1; 
  }
  
}