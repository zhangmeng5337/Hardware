#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"
#include "Protocal.h"


void main(void)
{
 
  HardwareInit();
  module_prams_init();
  
  while(1)
  {
<<<<<<< HEAD
    
    SIMCOM_Register_Network();
    lock_ctrl_process();
=======
  //delay_ms(5000);
    SIMCOM_Register_Network();
    module_process(); 
>>>>>>> 51c400f7820177b55d954b6f6682b2acd4551e24
  }
}
