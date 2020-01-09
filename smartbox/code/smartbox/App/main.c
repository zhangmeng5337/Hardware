#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"
#include "Protocal.h"


void main(void)
{
 
  HardwareInit();
  module_prams_init();
   battery_quantity();
  while(1)
  {
    
  //delay_ms(3000);
    if(battery_status()!=1)
      SIMCOM_Register_Network();
    module_process(); 
  }
}
