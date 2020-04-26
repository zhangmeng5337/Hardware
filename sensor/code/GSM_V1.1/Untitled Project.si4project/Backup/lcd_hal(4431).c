#include "lcd_hal.h"
#include "lcd.h"
#include "bsp.h"

const uint8_t LCD_Tab[10] = {
  
  a + b + c + d + e + f,                        // Displays "0"
  
  b + c,                                        // Displays "1"
  
  a + b + m + g + e + d,                        // Displays "2"
  
  a + b + m + g + c + d,                        // Displays "3"
  
  f + g + m + b + c,                        // Displays "4"
  
  a + f + g + m + c +d,                        // Displays "5"
  
  a + f + e + d + c + g + m ,                // Displays "6"
  
  a + b + c,                                // Displays "7"
  
  a + b + c + d + e + f + g + m,                // Displays "8"
  
  a + b + c + d + f + g + m                // Displays "9"
    
    
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void LCD_Congfig()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD,ENABLE);   //启用或禁用指定的外围时钟
    CLK_RTCClockConfig(CLK_RTCCLKSource_HSI,CLK_RTCCLKDiv_64);      //必须配置RTC时钟源,HSI选为RTC时钟
    /*lcd初始化，*/
    LCD_Init(LCD_Prescaler_8,LCD_Divider_16,LCD_Duty_1_4,LCD_Bias_1_3,LCD_VoltageSource_External );//LCD_VoltageSource_External,LCD_VoltageSource_Internal
    /*lcd端口的配置*/
    LCD_PortMaskConfig(LCD_PortMaskRegister_0,0Xff); // seg 0-7为     seg口
    LCD_PortMaskConfig(LCD_PortMaskRegister_1,0X3f); // seg 8,9 12-15为seg口
    LCD_PortMaskConfig(LCD_PortMaskRegister_2,0X00); // seg 16-21    为seg口
    LCD_PortMaskConfig(LCD_PortMaskRegister_3,0X00);
    
    LCD_ContrastConfig(LCD_Contrast_Level_5);        //配置LCD的对比度0---7
    LCD_DeadTimeConfig(LCD_DeadTime_1);              // 配置LCD液晶的死区时间0--7
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);// 配置LCD脉冲持续时间0--7    
    LCD_Cmd(ENABLE);                                 // 启用LCD控制器。
    
      LCD_WriteRAM(LCD_RAMRegister_0,00);  
      LCD_WriteRAM(LCD_RAMRegister_1,00);
      
      LCD_WriteRAM(LCD_RAMRegister_2,00);
      LCD_WriteRAM(LCD_RAMRegister_3,0);
      LCD_WriteRAM(LCD_RAMRegister_4,0);  
      LCD_WriteRAM(LCD_RAMRegister_5,0);
      LCD_WriteRAM(LCD_RAMRegister_6,0);
      LCD_WriteRAM(LCD_RAMRegister_7,0);
      LCD_WriteRAM(LCD_RAMRegister_8,0);  
      LCD_WriteRAM(LCD_RAMRegister_9,0);
      LCD_WriteRAM(LCD_RAMRegister_10,0);
      LCD_WriteRAM(LCD_RAMRegister_11,0);
      LCD_WriteRAM(LCD_RAMRegister_12,0);  
       LCD_WriteRAM(LCD_RAMRegister_13,0);    
    
}
void LCD_RAM_WRITE(unsigned char RegisterNum,unsigned char bitset,unsigned int dat)
{
      LCD_WriteRAM(RegisterNum,dat);   
}
/******************************************************************************************************

*  名          称：LCD_DisplayNum(uint8_t number)

*  功            能：控制段式液晶屏的数字显示部分

*  入口参数：number:要显示的数字

*  出口参数：无

*  说          明：根据数字的长度，判断要显示的长度,长度大于6位,只显示后六位

*  范          例：无

******************************************************************************************************/
uint8_t LCD_RAMX[14];
void LCD_DisplayNum(uint32_t number)
 
{
 
  uint8_t cnts=0,tmp=0;
  
  
 
  if(number<10)cnts=1;
  else if(number<100)cnts=2;
  else if(number<1000)cnts=3;
  else if(number<10000)cnts=4;
  else if(number<100000)cnts=5;
  else if(number<(uint32_t) 1000000)cnts=6;
  else cnts=6;
  
  //判断需要显示数字的长度，确定在LCD屏上需要的位数
  switch(cnts)
  {
    /*case 6:
           tmp = LCD_Tab[number%1000000/100000];
         //  ((tmp&m)==0)?(LCD_RAM0&=~0x02):(LCD_RAM0 |=0x02) ;
           ((tmp&e)==0)?(LCD_RAM0&=~0x01):(LCD_RAM0 |=0x01) ;
           ((tmp&g)==0)?(LCD_RAM2&=~0x80):(LCD_RAM2 |=0x80) ;
           ((tmp&b)==0)?(LCD_RAM2&=~0x40):(LCD_RAM2 |=0x40) ;
           ((tmp&f)==0)?(LCD_RAM6&=~0x08):(LCD_RAM6 |=0x08) ;
           ((tmp&a)==0)?(LCD_RAM6&=~0x04):(LCD_RAM6 |=0x04) ;
           ((tmp&c)==0)?(LCD_RAM3&=~0x20):(LCD_RAM3 |=0x20) ;
           ((tmp&d)==0)?(LCD_RAM3&=~0x10):(LCD_RAM3 |=0x10) ;  */                    
//    case 5:
//           tmp = LCD_Tab[number%100000/10000];
//           LCD_RAM_WRITE(unsigned char RegisterNum,unsigned int dat)
//          // ((tmp&m)==0)?(LCD_RAM0&=~0x08):(LCD_RAM0 |=0x08) ;
//           ((tmp&e)==0)?(LCD_RAMX[]&=~0x04):(LCD_RAM0 |=0x04) ;
//           ((tmp&g)==0)?(LCD_RAM2&=~0x20):(LCD_RAM2 |=0x20) ;
//           ((tmp&b)==0)?(LCD_RAM2&=~0x10):(LCD_RAM2 |=0x10) ;
//           ((tmp&f)==0)?(LCD_RAM6&=~0x02):(LCD_RAM6 |=0x02) ;
//           ((tmp&a)==0)?(LCD_RAM6&=~0x01):(LCD_RAM6 |=0x01) ;
//           ((tmp&c)==0)?(LCD_RAM3&=~0x80):(LCD_RAM3 |=0x80) ;
//           ((tmp&d)==0)?(LCD_RAM3&=~0x40):(LCD_RAM3 |=0x40) ;           
//           
//    case 4:
//           tmp = LCD_Tab[number%10000/1000];
//           ((tmp&m)==0)?(LCD_RAM0&=~0x20):(LCD_RAM0 |=0x20) ;
//           ((tmp&e)==0)?(LCD_RAM0&=~0x10):(LCD_RAM0 |=0x10) ;
//           ((tmp&g)==0)?(LCD_RAM2&=~0x08):(LCD_RAM2 |=0x08) ;
//           ((tmp&b)==0)?(LCD_RAM2&=~0x04):(LCD_RAM2 |=0x04) ;
//           ((tmp&f)==0)?(LCD_RAM5&=~0x80):(LCD_RAM5 |=0x80) ;
//           ((tmp&a)==0)?(LCD_RAM5&=~0x40):(LCD_RAM5 |=0x40) ;
//           ((tmp&c)==0)?(LCD_RAM4&=~0x02):(LCD_RAM4 |=0x02) ;
//           ((tmp&d)==0)?(LCD_RAM4&=~0x01):(LCD_RAM4 |=0x01) ;           
//    case 3:
//           tmp = LCD_Tab[number%1000/100];
//           ((tmp&m)==0)?(LCD_RAM0&=~0x80):(LCD_RAM0 |=0x80) ;
//           ((tmp&e)==0)?(LCD_RAM0&=~0x40):(LCD_RAM0 |=0x40) ;
//           ((tmp&g)==0)?(LCD_RAM2&=~0x02):(LCD_RAM2 |=0x02) ;
//           ((tmp&b)==0)?(LCD_RAM2&=~0x01):(LCD_RAM2 |=0x01) ;
//           ((tmp&f)==0)?(LCD_RAM5&=~0x20):(LCD_RAM5 |=0x20) ;
//           ((tmp&a)==0)?(LCD_RAM5&=~0x10):(LCD_RAM5 |=0x10) ;
//           ((tmp&c)==0)?(LCD_RAM4&=~0x08):(LCD_RAM4 |=0x08) ;
//           ((tmp&d)==0)?(LCD_RAM4&=~0x04):(LCD_RAM4 |=0x04) ;
   case 2:
           tmp = LCD_Tab[number%10];
           //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
           ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x80):(LCD_RAMX[LCD_RAMRegister_7] |=0x80) ; 
             LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//e
             
           ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x04):(LCD_RAMX[LCD_RAMRegister_4] |=0x04) ; 
             LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g 
             
           ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x04):(LCD_RAMX[LCD_RAMRegister_4] |=0x04) ;
           LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b  
           
           ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x80):(LCD_RAMX[LCD_RAMRegister_0] |=0x80) ; 
           LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//f 
           
           ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x40):(LCD_RAMX[LCD_RAMRegister_0] |=0x40) ;
           LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//a
           
           ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x40):(LCD_RAMX[LCD_RAMRegister_7] |=0x40) ; 
           LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//c
           
           ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x02):(LCD_RAMX[LCD_RAMRegister_11] |=0x02) ;
             LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
   case 1: 
           tmp = LCD_Tab[number%10];
           //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
           ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x20):(LCD_RAMX[LCD_RAMRegister_7] |=0x20) ; 
             LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//e
             
           ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x02):(LCD_RAMX[LCD_RAMRegister_4] |=0x02) ; 
             LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g 
             
           ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x01):(LCD_RAMX[LCD_RAMRegister_4] |=0x01) ;
           LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b  
           
           ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x20):(LCD_RAMX[LCD_RAMRegister_0] |=0x20) ; 
           LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//f 
           
           ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x10):(LCD_RAMX[LCD_RAMRegister_0] |=0x10) ;
           LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//a
           
           ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x10):(LCD_RAMX[LCD_RAMRegister_7] |=0x10) ; 
           LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//c
           
           ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x01):(LCD_RAMX[LCD_RAMRegister_11] |=0x01) ;
             LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    break;
    default:break;
  }
}
void lcd_process()
{
LCD_DisplayNum(0);
}