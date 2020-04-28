#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "lcd.h"
#include "Protocal.h"
#include "gprs_app.h"
extern unsigned char RtcWakeUp;

uint32_t tick = 0;

void LCD_GLASS_DisplaySTM(void)
{
  /*Set Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_3);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_5);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_7);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_8);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_11);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_12);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_17);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_18);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_1, PIXELCOLUMN_19);

  /*Set Row2 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_3);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_16);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_19);

  /*Set Row3 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_2);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_5);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_13);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_15);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_19);

  /*Set Row4 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_2);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_5);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_13);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_15);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_19);

  /*Set Row5 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_3);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_14);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_19);

  /*Set Row6 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_3);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_5);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_19);

  /*Set Row7 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_3);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_5);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_19);

  /*Set Row8 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_3);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_5);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_19);

  /*Set All Row9 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_1);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_7);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_11);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_17);

  LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_19);

  /*Set All Row10 Pixels On */
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_1);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_2);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_3);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_4);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_5);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_6);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_7);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_8);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_9);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_10);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_11);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_12);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_13);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_14);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_15);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_16);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_17);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_18);
  LCD_GLASS_WriteMatrixPixel(PIXELROW_10, PIXELCOLUMN_19);

}
void main(void)
{
#define SCROLL_SPEED  100 /* Low value gives higher speed */
#define SCROLL_NUM    5
  uint8_t LCD_String[7] = " STM8L ";
  HardwareInit();
  module_prams_init();
  RtcWakeUp = 1;
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

  /* Configures the RTC */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  LCD_GLASS_Init();
 LCD_GLASS_DisplaySTM();
  /* Enable the dispaly for the ST Logo */
  LCD_GLASS_DisplayLogo(ENABLE);
  /* Configure the LCD Battery Level High */
  LCD_GLASS_BatteryLevelConfig(BATTERYLEVEL_FULL);
  /* Configure the LCD ArrowDirection */
  LCD_GLASS_ArrowConfig(ARROWDIRECTION_UP);
  LCD_GLASS_ArrowConfig(ARROWDIRECTION_DOWN);
  LCD_GLASS_ArrowConfig(ARROWDIRECTION_LEFT);
  LCD_GLASS_ArrowConfig(ARROWDIRECTION_RIGHT);
  /* Configure the LCD Temperature level */
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_1);
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_2);
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_3);
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_4);
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_5);
  LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_6);
  /* Configure the LCD ValueUnit */
  LCD_GLASS_ValueUnitConfig(VALUEUNIT_MILLIAMPERE);
  LCD_GLASS_ValueUnitConfig(VALUEUNIT_MICROAMPERE);
  LCD_GLASS_ValueUnitConfig(VALUEUNIT_NANOAMPERE);
  /* Configure the LCD sign */
  LCD_GLASS_SignCmd(SIGN_POSITIVE, ENABLE);
  LCD_GLASS_SignCmd(SIGN_NEGATIVE, ENABLE);
  while(1)
  {
 /*Display " STM8L " string on LCD glass in scrolling mode*/
    LCD_GLASS_ScrollString(LCD_String, SCROLL_NUM, SCROLL_SPEED);

    /* Configures the LCD Blink mode*/
    LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div64);

    /* Display "  HALT " string on LCD glass before entering in Halt mode*/
    LCD_GLASS_DisplayString("LP MODE");

    /* Waiting Delay */
    delay_ms(0x7FFF);

    /* Configures the LCD Blink mode*/
    LCD_BlinkConfig(LCD_BlinkMode_Off, LCD_BlinkFrequency_Div64);

    /* RTC will wake-up from halt every 20second*/
    RTC_SetWakeUpCounter(10);
    RTC_WakeUpCmd(ENABLE);

    /* Display "LP MODE" string on LCD glass before entering in Halt mode*/
    LCD_GLASS_DisplayString(" HALT  ");

    /* Enter Halt mode*/
    halt();

    RTC_WakeUpCmd(DISABLE);

    if(RtcWakeUp == 1)
    {
      //#if !DEGUG_SENSOR 
     // SIMCOM_Register_Network();
      //parseGpsBuffer();
     // #endif
    //  module_process(); 
	 // FLOW_Ctrl(ON);
	//  FLOW_Ctrl(OFF);
    GPIO_ResetBits( PORT_FLOW, PIN_FLOW );
    GPIO_SetBits( PORT_FLOW, PIN_FLOW );  
	  LED_TOG();
	  delay_ms(2000);//不是很精确
    }

  }
}
