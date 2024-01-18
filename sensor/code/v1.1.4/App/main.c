#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "lcd.h"
#include "Protocal.h"
#include "gprs_app.h"
extern unsigned char wakeup_flag;
void main(void)
{
  
  
  HardwareInit();
  GSMInit();
  reapte_time();
  module_prams_init();
  LCD_Congfig();
  
  while(1)
  {
    if(wakeup_flag == 1)
    {
   
      parseGpsBuffer();
      module_process();  
    }
    
  }
}
