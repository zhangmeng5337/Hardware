#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "lcd.h"
#include "Protocal.h"
#include "gprs_app.h"
extern unsigned char RtcWakeUp;

uint32_t tick = 0;



void main(void)
{
#define SCROLL_SPEED  100 /* Low value gives higher speed */
#define SCROLL_NUM    5
  uint8_t LCD_String[7] = " STM8L ";
  HardwareInit();
  module_prams_init();
  RtcWakeUp = 1;
  LCD_Congfig();

  while(1)
  {
    uint8_t tt;
    tt=0xff;
//      LCD_WriteRAM(LCD_RAMRegister_0,0xff);  
//      LCD_WriteRAM(LCD_RAMRegister_1,0xff);
//      
//      LCD_WriteRAM(LCD_RAMRegister_2,tt);
//      LCD_WriteRAM(LCD_RAMRegister_3,tt);
//      LCD_WriteRAM(LCD_RAMRegister_4,tt);  
//      LCD_WriteRAM(LCD_RAMRegister_5,tt);
//      LCD_WriteRAM(LCD_RAMRegister_6,tt);
//      LCD_WriteRAM(LCD_RAMRegister_7,tt);
//      LCD_WriteRAM(LCD_RAMRegister_8,tt);  
//      LCD_WriteRAM(LCD_RAMRegister_9,tt);
//      LCD_WriteRAM(LCD_RAMRegister_10,tt);
//      LCD_WriteRAM(LCD_RAMRegister_11,tt);
//      LCD_WriteRAM(LCD_RAMRegister_12,00);  
//lcd_process();
    if(RtcWakeUp == 1)
    {
      //#if !DEGUG_SENSOR 
      SIMCOM_Register_Network();
      //parseGpsBuffer();
     // #endif
    //  module_process(); 
      
	  FLOW_Ctrl(ON);   LED_TOG();//delay_ms(2000);//不是很精确
	  FLOW_Ctrl(OFF);  LED_TOG();
    //GPIO_ResetBits( PORT_FLOW, PIN_FLOW );
   // GPIO_SetBits( PORT_FLOW, PIN_FLOW );  
	  
	  //delay_ms(2000);//不是很精确
    }

  }
}
