// file code :SYSTEM

 /******************************************************************************* 
����Ʒ�ͺš���BJ12864M-1
���� �� ������128X64
�����ͳߴ硿��93X70
������ߴ硿��72X40
����λ�ߴ硿��88X64
���� �� �ࡿ��0.52X0.52
���� �� С����0.48X0.48
�����������͡���ST7920
���򵥽��ܡ������ֿ⣬SPI�����ڣ�3.3V��5V

  2015-7-12 ���������!
���¼�¼��
1.������24*32��������ʾ���ܣ�
2.������16*32��������ʾ���ܣ�
3.�Ż��˶�LCDæ�Ĵ��룻
4.��������sprintf������λ������ֱ����ʾ��
5.������CGRAM��д�룬��������������
6.�Ż��˻�ͼGDRAM�����Ĳ�������ͼ�ٶ���ߣ�
 ������ : ST7920  
 
1��ST7920 �ڲ��̻���8192 ��16��16 ���������������CGROM�
2���̻���126 ��16��8 ����������ַ���HCGROM�
3���ṩ4��16��16���������ַ��Ĵ洢�ռ�CGRAM��
4���ṩ128��64�ĵ����ͼ��1024���ֽڵĴ洢�ռ�GDRAM��
5���ṩ1��16��15����ͼ��Ĵ洢�ռ�IRAM��ICON RAM����

DDRAM��ַ
0x80  0x81  0x82  0x83  0x84  0x85  0x86  0x87    //��һ�к���λ��
0x90  0x91  0x92  0x93  0x94  0x95  0x96  0x97    //�ڶ��к���λ��
0x88  0x89  0x8a  0x8b  0x8c  0x8d  0x8e  0x8f     //�����к���λ��
0x98  0x99  0x9a  0x9b  0x9c  0x9d  0x9e  0x9f     //�����к���λ��

*******************************************************************************/  
#include "lcd.h"
#include "sys.h"




/*  �Դ��з�ʽ����һ�ֽ����� */ 
static void SendByte(u8 sendData)
{
  u8 i;
  for (i = 0; i < 8; i++)
  {
    if (sendData&0x80)
      LCD_SID_HIGH;
    else
      LCD_SID_LOW;
    LCD_CLK_LOW;
     delay_us(10); //��ʱ10��us�ȴ���һ��ָ��ִ�����
    LCD_CLK_HIGH;
     sendData=sendData<<1;
   }
}


/* ���н���һ���ֽڵ�����  */
static u8 ReceiveByte(void)
{
   u8 i,d1,d2;
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
    delay_us(10);
    LCD_CLK_HIGH;
    
    if (LCD_SID) d1++;
    d1 = d1<<1;
   }
   
   for (i = 0; i < 8; i++)
   {
    LCD_CLK_LOW;
     delay_us(1);
    LCD_CLK_HIGH;   
    if (LCD_SID) d2++;
    d2 = d2<<1;
   }
   return (d1&0xf0 + d2&0x0f);
}

//������ݼĴ��������λ���ж�æ��־
 static u8 ReadBF(void)
{
  SendByte(READ_STATE);
  u8 data = ReceiveByte();
  if (data > 0x7f) return 1;
  else  return 0;
}

void LCD_WriteCMD(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writecommand); //���ʹ��ڿ��Ƹ�ʽ
  SendByte(data&0xf0);  //����λ
  SendByte((data&0x0f)<<4); //����λ
  while (ReadBF());
  LCD_CS_LOW;
}
      
void LCD_WriteDATA(u8 data)
{
  LCD_CS_HIGH;
  while (ReadBF());
  SendByte(writedata); //���ʹ��ڿ��Ƹ�ʽ
  SendByte(data&0xf0);   //���͸�4λ
  SendByte((data&0x0f)<<4); //���͵�4λ
  while (ReadBF());
  LCD_CS_LOW;
 }

/*******************************************************************************
�������������������������ʾ���ǻ�ͼ
���ڲ���: ��
********************************************************************************/
void LCD_Clear_DDRAM(void)
{
  LCD_WriteCMD(0x01);  //����ָ�
  HAL_Delay(2);        //datasheet >=1.4ms 
}

//һ���㷨������OK xianhuitu test OK
// ���Է�������ִ��������Ҫ2~3S
 void LCD_Clear_GDRAM(void)  //test OK
{    
  u8 j, i;
  LCD_WriteCMD(0x36);
  for(j=0;j<32;j++) //�ϰ���32����
  {
    LCD_WriteCMD(0x80+j);
    LCD_WriteCMD(0x80);//X����
    for(i=0;i<32;i++)//32��ʾ��1��д��,�Զ�д��3��
    {
      LCD_WriteDATA(0x00);  
    }
  }
  LCD_WriteCMD(0x30);     //����ָ�� 
}

void LCD_Init(void)
{  
  //������LCD�����ӵ�GPIO����
  
// PA6--MISO--(not used) // you omit this IO
// PB13--SCK--CLK--E
// PB14--NSS--CS--RS
// PB15--MOSI--SDIO--R/W

// PB1--BLK
  

  //  1>оƬ�ϵ磻
  //2>��ʱ40ms���ϣ�
  //3>��λ������RST����һ�������أ�RST=1;RST=0;RST=1;��
     HAL_Delay(50);  // this delay is necessary !
     LCD_WriteCMD(0x30);//�������ã�һ����8λ���ݣ�����ָ�
     LCD_WriteCMD(0x0C);//0000,1100  ��ʾ״̬���أ�������ʾON�������ʾOFF�������ʾ����OFF
     //LCD_WriteCMD(0x0f);//������ʾ ON C=1�� �α�ON(������˸) B=1�� �α�λ��(�α��»���)ON
     //����а����������֣��򿪹�꣬�����ƶ���LCD_WriteCMD(0x10); LCD_WriteCMD(0x10);  
     LCD_WriteCMD(0x01);//0000,0001 �����ʾDDRAM
     LCD_WriteCMD(0x02);//0000,0010 DDRAM��ַ��λ
//   LCD_WriteCMD(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC
//   LCD_WriteCMD(0x04);//���趨����ʾ�ַ�/����������λ��DDRAM��ַ��1
     LCD_WriteCMD(0x06); //���趨�����治�ƶ�,�α����ƣ�DDRAMλ��ַ��1
     HAL_Delay(10);  // this delay is necessary !
}

/******************************************************************************* 
* ���� :  LCD_SetPos
* ���� : �趨��ʾλ��,ֻ�����ڱ�׼ָ� 
* ���� :  x (0~3)��
          y (0~127)��
* ��� : �� 
*******************************************************************************/  
void LCD_SetPos(u8 row, u8 col)  
{  
    u8 pos;  
    if( row == 0)  
            row = 0x80;  
    else if(row == 1)  
            row = 0x90;  
    else if(row == 2)  
            row = 0x88;  
    else if(row == 3)  
            row = 0x98;     
    pos = row + col;  
    LCD_WriteCMD(pos);//��д������ǰ��ָ����ʾ��ַ 
    delay_us(8);
}  


/******************************************************************** 
* ���� : ShowCharPos 
* ���� : �ڵ�ǰ����λ����ʾ�����ַ�,col=0,1,2,ÿ��colռ16dot
* ���� : u8 row, u8 col,u8 dat
* ��� : �� 
***********************************************************************/ 
void ShowCharPos(u8 row, u8 col,u8 dat)
{    
  LCD_SetPos(row, col);
  LCD_WriteDATA(dat);
}

/******************************************************************************* 
* ���� : ShowEnString(u8 *s)  
* ���� : ��ʾӢ���ַ���  col=0,1,2,1 col=16dot
* ���� : *s 
* ��� : �� 
*******************************************************************************/  
void ShowString(u8 *s)  
{    
    u8  i = 0;  
    while(s[i] != '\0')  
    {   
      LCD_WriteDATA(s[i++]);      
    }  
}  

void ShowStringPos(u8 row, u8 col,u8 *s)     //col is full char wide 
{  
     u8  i = 0;  
     LCD_SetPos(row, col);    
    while(s[i] != '\0')  
    {       
      LCD_WriteDATA(s[i++]);        
      if((2*col+i)%(16)==0)     //must 16, is OK
      {          
        LCD_SetPos(++row,0);     //display start at next row.
      }
   
    }  
}  


/******************************************************************************* 
* ���� : ShowGB 
* ���� : ��ʾ��������
* ���� : i 
* ��� : �� 
********************************************************************************/  
void ShowGB( unsigned char *HZ)
{
   LCD_WriteDATA(HZ[0]);                //д�뺺�ֵĸ߰�λ����
   LCD_WriteDATA(HZ[1]);                //д�뺺�ֵĵͰ�λ���� 
}

void ShowGBPos(u8 row, u8 col, u8 *HZ)
{
   LCD_SetPos(row, col);
   ShowGB(HZ);
}


/******************************************************************** 
* ���� : ShowGBStringPos 
* ���� : ��ʾ�������
* ���� : u8 row, u8 col :���ִ�����ʼ��ַ
* ��� : �� 
* ˵�� : �Զ�����:lcmĬ�ϻ���˳�����ҵģ�0--2--1--3--0
***********************************************************************/  
void ShowGBStringPos(u8 row, u8 col, u8 *s)
{
  u8 i = 0;
  LCD_SetPos(row, col);
  while (s[i] != '\0')
  {
    ShowGB(s+i);
    i+=2;   
    if((2*col+i)%(16)==0)                 //�����һ��
    {            
       LCD_SetPos(++row,0);            //����������ʾ����ʼ��ַ
    }     
  } 
}



/******************************************************************** 
* ���� : ShowBlankPos 
* ���� : 
* ���� : col = 0~7 1num =1����ַ�
* ���� : �ڰ�����ܣ���������ʾ������
***********************************************************************/ 
void ShowBlankPos(u8 row, u8 col, u8 num)
{
  u8 i ;
  LCD_SetPos(row,col);   
  for (i = 0; i < num;i++) 
    LCD_WriteDATA(0x20); //д�ո�
}


/******************************************************************** 
* ���� : ShowNumPos
* ���� : 
* ���� : u8 row, u8 col ��ʼ��ַ col=0,1,2,ÿ��colռ16dot
* ��� : �� 
* ˵�� : �Զ�����:lcmĬ�ϻ���˳�����ҵģ�0--2--1--3--0
***********************************************************************/ 
void ShowNumPos(u8 row, u8 col, u16 num,u8 DecOrHex)//col is full char wide 
{     
    char buf[16]; 
    if(DecOrHex==10)  // dex 
    sprintf(buf, "%d", num);
    else
    sprintf(buf,"%X",num);  //"0x%X"
    ShowStringPos(row,col,(u8 *)(buf));
}


/******************************************************************** 
* ���� : Show2NumPos
* ���� : ��ʾ2λ����,ʮλ���㲹��,0~99
* ���� : u8 row, u8 col ��ʼ��ַ col=0,1,2,ÿ��colռ16dot
* ��� : �� 
* ˵�� : �Զ�����:lcmĬ�ϻ���˳�����ҵģ�0--2--1--3--0
***********************************************************************/ 
void Show2NumPos(u8 row, u8 col, u8 num)//col is full char wide 
{     
    char buf[1]; 
	char buf1[2]="0";
    
	if(num <10)
	 {
		sprintf(buf, "%d", num);  	 
		strcat (buf1, buf);	//buf link to the buf1
		ShowStringPos(row,col,(u8 *)buf1);	//06
	 }
	else
	{
        sprintf(buf1, "%d", num);  
		ShowStringPos(row,col,(u8 *)buf1);	//06
	}
   
}



