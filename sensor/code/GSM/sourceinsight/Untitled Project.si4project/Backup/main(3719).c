

#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"






uint32_t tick = 0;

void main(void)
{

  HardwareInit();
  //test();
  while(1)
  {
    
    delay_ms(1000);
    tick ++;
  SIMCOM_Register_Network();
	//  adcGet(ADC_BAT_CHANNEL);

  }
}
