#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"
#include "Protocal.h"

extern Uart_Types uart_str;
uint32_t tick = 0;

void main(void)
{
  
  HardwareInit();
  params_init();
  //test();
  uart_str.receive_flag =0;
  while(1)
  {
    // delay_ms(1000);
    tick ++;
    SIMCOM_Register_Network();
    module_process(); 
  }
}
