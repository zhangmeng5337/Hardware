#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "lcd.h"
#include "Protocal.h"
#include "gprs_app.h"
extern unsigned char wakeup_flag;
extern unsigned char RtcWakeUp;
uint32_t tick = 0;


// extern  unsigned int wakeupcount;
void main(void)
{
  
  
  HardwareInit();
  GSMInit();
  reapte_time();
  module_prams_init();
  wakeup_flag = 1;
  RtcWakeUp = 1;
  LCD_Congfig();
  
  while(1)
  {
    if(wakeup_flag == 1)
    {
//      GSM_HardwareInit(OFF);
//      EnterStopMode();
//      RTC_Config(30,ON);//10:9s 
//      enableInterrupts();
//      halt();	    
      parseGpsBuffer();
      module_process();  
    }
    
  }
}
