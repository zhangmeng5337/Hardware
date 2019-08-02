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











void EnterStopMode(void) ;
void HardwareInit(void);

void DelayMs(INT8U x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)  
    for(i=1000;i>0;i--)
      ;
}



void LED_Init(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}

void delay_ms(u16 num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<1266; i++);
  }
}


 







void main(void)
{	
  
  
  HardwareInit();
  LED_Init();  //led init
  GPIO_SetBits(GPIOD, GPIO_Pin_0);//led off
  while(1)
  {
    PowerSavingMode_setting();
  }
}





