

#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"



void delay_ms(u16 num)//不是很精确
{
  u8 i = 0;
  while(num--)
  {
    for (i=0; i<100; i++);
  }
}






void main(void)
{

  HardwareInit();
  while(1)
  {

  }
}
