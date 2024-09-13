
//***************************************************************************
//���߱�: CPU=89C52  SystemClock=12Mhz                                      *
//CS=P1.0            SCLK=P1.2          SID=P1.1       Reset=RC in Board    *
//***************************************************************************


#include "lcd.h"
#include "lcd_string.h"


//sbit Key=P1^4;



//���ڷ���һ���ֽ�
void SendByte(unsigned char Dbyte)
{
     unsigned char i,tmp;
	 tmp = Dbyte;
     for(i=0;i<8;i++)
     {
           LCD_CLK_L
		   	if(tmp&0x80)
				LCD_SID_H
			else
				LCD_SID_L
           tmp=tmp<<1;      //����һλ
           LCD_CLK_H
           LCD_CLK_L
     }
}

//���ڽ���һ���ֽ�
//���ڶ�ȡ���ݵ�ʱ���õ�
//��������������һ��ֻ�ܶ���4bit��
unsigned char ReceiveByte(void)
{
     unsigned char i,temp1,temp2;
     temp1=temp2=0;
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
            LCD_CLK_L 
		  	LCD_CLK_H
            LCD_CLK_L
           if(LCD_SID_R)
		   	 temp1++;
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
		   LCD_CLK_L 
		   LCD_CLK_H
		   LCD_CLK_L
           if(LCD_SID_R) temp2++;
     }
     return ((0xf0&temp1)+(0x0f&temp2));
}

void CheckBusy( void )
{
     do   SendByte(0xfc);      //11111,RW(1),RS(0),0
     while(0x80&ReceiveByte());      //BF(.7)=1 Busy
}

void WriteCommand( unsigned char Cbyte )
{
     LCD_CS_H
     CheckBusy();
     SendByte(0xf8);            //11111,RW(0),RS(0),0
     SendByte(0xf0&Cbyte);      //����λ
     SendByte(0xf0&Cbyte<<4);//����λ(��ִ��<<)
     LCD_CS_L
}

void WriteData( unsigned char Dbyte )
{
     LCD_CS_H
     CheckBusy();
     SendByte(0xfa);            //11111,RW(0),RS(1),0
     SendByte(0xf0&Dbyte);      //����λ
     SendByte(0xf0&Dbyte<<4);//����λ(��ִ��<<)
    LCD_CS_L
}

unsigned char ReadData( void )
{
     CheckBusy();
     SendByte(0xfe);            //11111,RW(1),RS(1),0
     return ReceiveByte();
}

void Delay(unsigned int MS)
{
     unsigned char us,usn;
     while(MS!=0)            //for 12M
           { usn = 2;
                 while(usn!=0)
                       {
                             us=0xf5;
                             while (us!=0){us--;};
                             usn--;
                       }
                 MS--;
           }
}

//ά�ݵǲ��Լ�ר����ʱ����
void DelayKey(unsigned int Second , unsigned int MS100)
{                              //���뾫ȷ��0.1S,����","
     unsigned int i;
     for(i=0;i<Second*100+MS100*10;i++)
     {
          // if(Key==0)
          // {
                 Delay(20);
          //       while(Key==0) {Delay(20);}
           //      break;
          // }
          // else Delay(10);
     }
}

void LcmInit( void )
{
     WriteCommand(0x30);      //8BitMCU,����ָ���
     WriteCommand(0x03);      //AC��0,���ı�DDRAM����
     WriteCommand(0x0C);      //��ʾON,�α�OFF,�α�λ����OFF
     WriteCommand(0x01);      //����,AC��0
     WriteCommand(0x06);      //д��ʱ,�α����ƶ�
}

//�ı�����RAM����
void LcmClearTXT( void )
{
     unsigned char i;
     WriteCommand(0x30);      //8BitMCU,����ָ���
     WriteCommand(0x80);      //AC����ʼλ
     for(i=0;i<64;i++)
        WriteData(0x20);
}

//ͼ�������ı�����ʾ��������ͬ��RAM��
//ͼ������RAM����
void LcmClearBMP( void )
{
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     WriteCommand(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           WriteCommand(0x80|i);      //��λ��
           WriteCommand(0x80);      //��λ��
           for(j=0;j<32;j++)            //256/8=32 byte
                WriteData(0);
     }
}

void PutStr(unsigned char row,unsigned char col,unsigned char *puts)
{
     WriteCommand(0x30);      //8BitMCU,����ָ���
     WriteCommand(AC_TABLE[8*row+col]);      //��ʼλ��
     while(*puts != '\0')      //�ж��ַ����Ƿ���ʾ���
     {
           if(col==8)            //�жϻ���
           {            //�����ж�,���Զ��ӵ�һ�е�������
                 col=0;
                 row++;
           }
           if(row==4) row=0;      //һ����ʾ��,�ص������Ͻ�
           WriteCommand(AC_TABLE[8*row+col]);
           WriteData(*puts);      //һ������Ҫд����
           puts++;
           WriteData(*puts);
           puts++;
           col++;
     }
}

void PutBMP(unsigned char *puts)
{
     unsigned int x=0;
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     WriteCommand(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           WriteCommand(0x80|i);      //��λ��
           WriteCommand(0x80);      //��λ��
           for(j=0;j<32;j++)      //256/8=32 byte
           {            //��λ��ÿ���Զ�����
                 WriteData(puts[x]);
                 x++;
           }
     }
}

//ά�ݵǲ����õ�����ʾ
void DisplayDots(unsigned char DotByte)
{
     unsigned char i,j;
     WriteCommand(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     WriteCommand(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           WriteCommand(0x80|i);      //��λ��
           WriteCommand(0x80);      //��λ��
           for(j=0;j<32;j++)      //256/8=32 byte
           {            //��λ��ÿ���Զ�����
                 WriteData(DotByte);                  
           }
           DotByte=~DotByte;
     }
}

void lcd_proc_test( void )
{
     Delay(100);      //�ȴ���λ
     LcmInit();
     LcmClearTXT();
     LcmClearBMP();
     while(1)
     {
           LcmClearTXT();
           PutBMP(bmp1);
           DelayKey(1,5);
           
           DisplayDots(0x55);
           DelayKey(1,5);
           
           LcmClearBMP();
           PutStr(0,0,str1);
           DelayKey(1,5);
                       
           LcmClearTXT();
           DisplayDots(0xaa);
           DelayKey(1,5);
     }
}



