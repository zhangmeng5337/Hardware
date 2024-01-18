#include "lcd_hal.h"
#include "lcd.h"
#include "bsp.h"
#include "Protocal.h"
extern Data_Stru Data_usr;
const uint8_t LCD_Tab[10] =
{
  
  a + b + c + d + e + f,                        // Displays "0"
  
  b + c,                                        // Displays "1"
  
  a + b + m + g + e + d,                        // Displays "2"
  
  a + b + m + g + c + d,                        // Displays "3"
  
  f + g + m + b + c,                        // Displays "4"
  
  a + f + g + m + c +d,                        // Displays "5"
  
  a + f + e + d + c + g + m,                 // Displays "6"
  
  a + b + c,                                // Displays "7"
  
  a + b + c + d + e + f + g + m,                // Displays "8"
  
  a + b + c + d + f + g + m                // Displays "9"
    
    
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void LCD_Congfig()
{
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI,CLK_RTCCLKDiv_1);      //��������RTCʱ��Դ,HSIѡΪRTCʱ��
   CLK_PeripheralClockConfig(CLK_Peripheral_LCD,ENABLE);   //���û����ָ������Χʱ�� 
  /*lcd��ʼ����*/
  LCD_Init(LCD_Prescaler_8,LCD_Divider_16,LCD_Duty_1_4,LCD_Bias_1_3,LCD_VoltageSource_External );//LCD_VoltageSource_External,LCD_VoltageSource_Internal
  /*lcd�˿ڵ�����*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0,0Xff); // seg 0-7Ϊ     seg��
  LCD_PortMaskConfig(LCD_PortMaskRegister_1,0X3f); // seg 8,9 12-15Ϊseg��
  LCD_PortMaskConfig(LCD_PortMaskRegister_2,0X00); // seg 16-21    Ϊseg��
  LCD_PortMaskConfig(LCD_PortMaskRegister_3,0X00);
  
  LCD_ContrastConfig(LCD_Contrast_Level_5);        //����LCD�ĶԱȶ�0---7
  LCD_DeadTimeConfig(LCD_DeadTime_1);              // ����LCDҺ��������ʱ��0--7
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);// ����LCD�������ʱ��0--7
  LCD_Cmd(ENABLE);                                 // ����LCD��������
  
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
uint8_t LCD_RAMX[14];
void digit_ctrl(unsigned char digitnum,unsigned char digit_sw)
{
  if(digit_sw == 0)
  {
    switch(digitnum)
    {
    case 5:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit4
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] |=0x80;//digit 5
      break;
    case 4:
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3
      break;
    case 3:
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit 4
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      break;
    case 2:
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      //LCD_RAMX[LCD_RAMRegister_11]&=~0x10;//digit 4
      LCD_RAMX[LCD_RAMRegister_11]&=~0x08;//digit 3
      break;
    default:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit4
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      break;
    }
  }
  else
  {
    switch(digitnum)
    {
    case 5:
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit 5
      break;
    case 4:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit 4
      break;
    case 3:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x08;//digit 3
      break;
    case 2:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      break;
    default:
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit4
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      break;
    }
  }
  
}
/******************************************************************************************************

*  ��          �ƣ�LCD_DisplayNum(uint8_t number)

*  ��            �ܣ����ƶ�ʽҺ������������ʾ����

*  ��ڲ�����number:Ҫ��ʾ������

*  ���ڲ�������

*  ˵          �����������ֵĳ��ȣ��ж�Ҫ��ʾ�ĳ���,���ȴ���6λ,ֻ��ʾ����λ

*  ��          ������

******************************************************************************************************/

void LCD_DisplayNum(float number2)

{
  
  uint8_t cnts=0,tmp=0;
  uint8_t digit_flag;
  static float number3;
  uint32_t number;
  //number2 = 11.111;
//  if(number3 == 11.111)
//    number3 = 55.555;
//  else
//   number3= 11.111;  
   if(number2<0)
  {
    number2 = 0;

  } 
  if(number2<10)
  {
    number = (uint32_t)(number2*10000.0);
    cnts=5;
    digit_flag = 5;
  }
  else if(number2<100)
  {
    number = (uint32_t)(number2*1000.0);
    cnts=5;
    digit_flag = 4;
  }
  else if(number2<1000)
  {
    number = (uint32_t)(number2*100.0);
    cnts=5;
    digit_flag = 3;
  }
  else if(number2<10000)
  {
    number = (uint32_t)(number2*10.0);
    cnts=5;
    digit_flag = 2;
  }
  else if(number2<100000)
  {
    number = (uint32_t)(number2*1.0);
    cnts=5;
    digit_flag = 1;
  }
  else if(number2<(uint32_t) 1000000)
  {
    cnts=5;
    number = (uint32_t)(number2*1.0);
    digit_flag = 1;
  }
  
  //�ж���Ҫ��ʾ���ֵĳ��ȣ�ȷ����LCD������Ҫ��λ��
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
  case 5:
    tmp = LCD_Tab[number%100000/10000];
    //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
    
    ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x10):(LCD_RAMX[LCD_RAMRegister_1] |=0x10) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//a
    
    ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_5]&=~0x01):(LCD_RAMX[LCD_RAMRegister_5] |=0x01) ;
    LCD_WriteRAM(LCD_RAMRegister_5,LCD_RAMX[LCD_RAMRegister_5]); //b
    
    ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x10):(LCD_RAMX[LCD_RAMRegister_8] |=0x10) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//c
    
    ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_12]&=~0x01):(LCD_RAMX[LCD_RAMRegister_12] |=0x01) ;
    LCD_WriteRAM(LCD_RAMRegister_12,LCD_RAMX[LCD_RAMRegister_12]);//d
    
    ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x20):(LCD_RAMX[LCD_RAMRegister_8] |=0x20) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//e
    
    ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x20):(LCD_RAMX[LCD_RAMRegister_1] |=0x20) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//f
    
    ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_5]&=~0x02):(LCD_RAMX[LCD_RAMRegister_5] |=0x02) ;
    LCD_WriteRAM(LCD_RAMRegister_5,LCD_RAMX[LCD_RAMRegister_5]); //g
    
    if(digit_flag == 5)
    {
      /*LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit4
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3
      LCD_RAMX[LCD_RAMRegister_11]&=0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] |=0x80;//digit 5*/
      digit_ctrl(5,0) ;
    }
    else
    {
      //LCD_RAMX[LCD_RAMRegister_11] &=~0x80;
      digit_ctrl(5,1) ;
    }
    LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
  case 4:
    tmp = LCD_Tab[number%10000/1000];
    //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
    ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x04):(LCD_RAMX[LCD_RAMRegister_1] |=0x04) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//a
    
    ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x40):(LCD_RAMX[LCD_RAMRegister_4] |=0x40) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b
    
    ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x04):(LCD_RAMX[LCD_RAMRegister_8] |=0x04) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//c
    
    if(digit_flag == 4)
    {
      /*LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2
      LCD_RAMX[LCD_RAMRegister_11] &=~0x08;//digit3*/
      digit_ctrl(4,0) ;
      if((tmp&d)==0)
      {
        
        LCD_RAMX[LCD_RAMRegister_11]|=0x20;//digit4
        LCD_RAMX[LCD_RAMRegister_11]&=0xbf;//digit4
        
      }
      else
      {
        LCD_RAMX[LCD_RAMRegister_11]|=0x60;
        
      }
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    else
    {
      digit_ctrl(4,1) ;
      ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x40):(LCD_RAMX[LCD_RAMRegister_11] |=0x40) ;
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x08):(LCD_RAMX[LCD_RAMRegister_8] |=0x08) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//e
    
    ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x08):(LCD_RAMX[LCD_RAMRegister_1] |=0x08) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//f
    
    ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x80):(LCD_RAMX[LCD_RAMRegister_4] |=0x80) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g
    
  case 3:
    tmp = LCD_Tab[number%1000/100];
    //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
    
    ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x01):(LCD_RAMX[LCD_RAMRegister_1] |=0x01) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//a
    
    ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x10):(LCD_RAMX[LCD_RAMRegister_4] |=0x10) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b
    
    ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x01):(LCD_RAMX[LCD_RAMRegister_8] |=0x01) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//c
    
    if(digit_flag == 3)
    {
      
      /*LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      LCD_RAMX[LCD_RAMRegister_11]&=~0x20;//digit 4
      LCD_RAMX[LCD_RAMRegister_11]&=~0x02;//digit2*/
      digit_ctrl(3,0) ;
      if((tmp&d)==0)
      {
        LCD_RAMX[LCD_RAMRegister_11]&=~0x10;//
        LCD_RAMX[LCD_RAMRegister_11]|=0x08;//digit3
        
        
      }
      else
      {
        LCD_RAMX[LCD_RAMRegister_11]|=0x18;
        
      }
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    else
    {
      digit_ctrl(3,1) ;
      ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x10):(LCD_RAMX[LCD_RAMRegister_11] |=0x10) ;
      
      
      
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_8]&=~0x02):(LCD_RAMX[LCD_RAMRegister_8] |=0x02) ;
    LCD_WriteRAM(LCD_RAMRegister_8,LCD_RAMX[LCD_RAMRegister_8]);//e
    
    ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_1]&=~0x02):(LCD_RAMX[LCD_RAMRegister_1] |=0x02) ;
    LCD_WriteRAM(LCD_RAMRegister_1,LCD_RAMX[LCD_RAMRegister_1]);//f
    
    ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x20):(LCD_RAMX[LCD_RAMRegister_4] |=0x20) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g
    LCD_WriteRAM(LCD_RAMRegister_11,0x08);
  case 2:
    tmp = LCD_Tab[number%100/10];
    //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
    
    ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x40):(LCD_RAMX[LCD_RAMRegister_0] |=0x40) ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//a
    
    ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x04):(LCD_RAMX[LCD_RAMRegister_4] |=0x04) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b
    
    ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x40):(LCD_RAMX[LCD_RAMRegister_7] |=0x40) ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//c
    
    
    if(digit_flag == 2)
    {
      /*LCD_RAMX[LCD_RAMRegister_11] &=~0x80;//digit5
      //LCD_RAMX[LCD_RAMRegister_11]&=~0x10;//digit 4
      LCD_RAMX[LCD_RAMRegister_11]&=~0x08;//digit 3*/
      digit_ctrl(2,0) ;
      if((tmp&d)==0)
      {
        
        LCD_RAMX[LCD_RAMRegister_11]|=0x02;//digit2
      }
      else
      {
        LCD_RAMX[LCD_RAMRegister_11]|=0x06;
        
      }
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    else
    {
      digit_ctrl(2,1) ;
      ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x04):(LCD_RAMX[LCD_RAMRegister_11] |=0x04) ;
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x80):(LCD_RAMX[LCD_RAMRegister_7] |=0x80) ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//e
    
    ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x80):(LCD_RAMX[LCD_RAMRegister_0] |=0x80) ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//f
    
    ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x08):(LCD_RAMX[LCD_RAMRegister_4] |=0x08) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g
  case 1:
    tmp = LCD_Tab[number%10];
    //((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
    
    ((tmp&a)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x10):(LCD_RAMX[LCD_RAMRegister_0] |=0x10) ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//a
    
    ((tmp&b)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x01):(LCD_RAMX[LCD_RAMRegister_4] |=0x01) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //b
    
    ((tmp&c)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x10):(LCD_RAMX[LCD_RAMRegister_7] |=0x10) ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//c
    /*if(digit_flag == 1)
    {
    ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x81):(LCD_RAMX[LCD_RAMRegister_11] |=0x81) ;
    LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
  }*/
    // else
    {
      ((tmp&d)==0)?(LCD_RAMX[LCD_RAMRegister_11]&=~0x01):(LCD_RAMX[LCD_RAMRegister_11] |=0x01) ;
      LCD_WriteRAM(LCD_RAMRegister_11,LCD_RAMX[LCD_RAMRegister_11]);//d
    }
    ((tmp&e)==0)?(LCD_RAMX[LCD_RAMRegister_7]&=~0x20):(LCD_RAMX[LCD_RAMRegister_7] |=0x20) ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]);//e
    
    ((tmp&f)==0)?(LCD_RAMX[LCD_RAMRegister_0]&=~0x20):(LCD_RAMX[LCD_RAMRegister_0] |=0x20) ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]);//f
    
    ((tmp&g)==0)?(LCD_RAMX[LCD_RAMRegister_4]&=~0x02):(LCD_RAMX[LCD_RAMRegister_4] |=0x02) ;
    LCD_WriteRAM(LCD_RAMRegister_4,LCD_RAMX[LCD_RAMRegister_4]); //g
    break;
  default:
    break;
    
  }
  
  if(Data_usr.vbatf/3700000>=1.0)
  {
    LCD_RAMX[LCD_RAMRegister_0] |=0x03 ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]); //p5 p6
    LCD_RAMX[LCD_RAMRegister_3] |=0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //p7
    LCD_RAMX[LCD_RAMRegister_7] |=0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //p8  
    LCD_RAMX[LCD_RAMRegister_10] |=0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_10,LCD_RAMX[LCD_RAMRegister_10]); //p9 
  }
  else if(Data_usr.vbatf/3700000>=0.55)
  {
    LCD_RAMX[LCD_RAMRegister_0] |=0x03 ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]); //p5 p6
    LCD_RAMX[LCD_RAMRegister_3] |=0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //p7
    LCD_RAMX[LCD_RAMRegister_7] |=0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //p8  
    LCD_RAMX[LCD_RAMRegister_10] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_10,LCD_RAMX[LCD_RAMRegister_10]); //p9 
  }
  else if(Data_usr.vbatf/3700000>0.3)
  {
    LCD_RAMX[LCD_RAMRegister_0] |=0x03 ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]); //p5 p6
    LCD_RAMX[LCD_RAMRegister_3] |=0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //p7
    LCD_RAMX[LCD_RAMRegister_7] &=~0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //p8  
    LCD_RAMX[LCD_RAMRegister_10] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_10,LCD_RAMX[LCD_RAMRegister_10]); //p9 
  }
  else if(Data_usr.vbatf/3700000>0.1)
  {
    LCD_RAMX[LCD_RAMRegister_0] |=0x03 ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]); //p5 p6
    LCD_RAMX[LCD_RAMRegister_3] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //p7
    LCD_RAMX[LCD_RAMRegister_7] &=~0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //p8  
    LCD_RAMX[LCD_RAMRegister_10] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_10,LCD_RAMX[LCD_RAMRegister_10]); //p9 
  }
  else 
  {
    LCD_RAMX[LCD_RAMRegister_0] &=~0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_0,LCD_RAMX[LCD_RAMRegister_0]); //p5 p6
    LCD_RAMX[LCD_RAMRegister_3] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //p7
    LCD_RAMX[LCD_RAMRegister_7] &=~0x01 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //p8  
    LCD_RAMX[LCD_RAMRegister_10] &=~0x10 ;
    LCD_WriteRAM(LCD_RAMRegister_10,LCD_RAMX[LCD_RAMRegister_10]); //p9 
  }
}
void lcd_process(unsigned char flag)
{
  if(flag==1) 
    LCD_DisplayNum(Data_usr.deep_f);
  else if(flag == 3)
  { 
    LCD_RAMX[LCD_RAMRegister_7] &=~0x08 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //T2
    LCD_RAMX[LCD_RAMRegister_3] |=0x80 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //T3 
  }
  else if(flag == 4)
  {  
    LCD_RAMX[LCD_RAMRegister_7] |=0x08 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //T2
    LCD_RAMX[LCD_RAMRegister_3] &=~0x80 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //T3   
  }
  else if(flag == 5)
  {  
    LCD_RAMX[LCD_RAMRegister_7] &=~0x08 ;
    LCD_WriteRAM(LCD_RAMRegister_7,LCD_RAMX[LCD_RAMRegister_7]); //T2
    LCD_RAMX[LCD_RAMRegister_3] &=~0x80 ;
    LCD_WriteRAM(LCD_RAMRegister_3,LCD_RAMX[LCD_RAMRegister_3]); //T3   
  }
  else if(flag == 0)
  {
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
  
}