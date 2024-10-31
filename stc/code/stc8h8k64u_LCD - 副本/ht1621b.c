#include "main.h"
#include "lcd_driver.h"
#define uchar unsigned char
#define uint unsigned int
 
#define _Nop() _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()
#define BIAS 0x29 //0b1000 0101 0010 1/3duty 4com
 
#define SYSDIS 0X00 //0b1000 0000 0000 ����ϵͳ������LCDƫѹ������
#define SYSEN 0X01 //0b1000 0000 0010 ��ϵͳ����
 
#define LCDOFF 0X02 //0b1000 0000 0100 ��LCDƫѹ
#define LCDON 0X03 //0b1000 0000 0110 ��LCDƫѹ
#define XTAL 0x1c //0b1000 0010 1000 �ⲿ��ʱ��
#define RC256 0X18 //0b1000 0011 0000 �ڲ�ʱ��
#define TONEON 0X12 //0b1000 0001 0010 ���������
#define TONEOFF 0X10 //0b1000 0001 0000 �ر��������
#define WDTDIS 0X0A //0b1000 0000 1010 ��ֹ���Ź�
 




uchar code
Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 uchar code seg_tab[] =
{
    // ������ʾ0-9���κ���ʾ���ֵ����򣬶��ǵ������������
    0X5f, // 0
    0X06, // 1
    0X6b, // 2
    0X2f, // 3
    0X36, // 4
    0X3D, // 5
    0X7D, // 6
    0X07, // 7
    0X7F, // 8
    0X3F,  // 9
	  0x80   //dot
};
//delay us
void Delay(uchar us) //5,7,9
{
 while(--us);
}
 
 
//delay ms
void DelayMS(uint iMs)
{
 uint i,j;
 for(i=0;i<iMs;i++)
 for(j=0;j<65;j++) Delay(1);
}
 
/******************************************************
д���ݺ���,cntΪ��������λ��,���ݴ���Ϊ��λ��ǰ
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
 uchar i;
 for (i=0;i<cnt;i++)
      {
       HT1621_WR=0;
       Delay(1);
       HT1621_DAT=Data&0x80;
       Delay(1);
       HT1621_WR=1;
       Delay(1);
      Data<<=1;
     }
 
}
 
/********************************************************
�������ƣ�void Ht1621WrCmd(uchar Cmd)
��������: HT1621����д�뺯��
ȫ�ֱ�������
����˵����CmdΪд����������
����˵������
 
˵ ����д�������ʶλ100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
 HT1621_CS=0;
       Delay(1);
 Ht1621Wr_Data(0x80,3); //д�������־100
 Ht1621Wr_Data(Cmd,9); //д����������
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
�������ƣ�void Ht1621WrOneData(uchar Addr,uchar Data)
��������: HT1621��ָ����ַд�����ݺ���
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��DataΪд������
����˵������
˵ ������ΪHT1621������λ4λ������ʵ��д������Ϊ�����ĺ�4λ
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
 HT1621_CS=0;
 Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
 Ht1621Wr_Data(Addr<<2,6); //д���ַ����
 Ht1621Wr_Data(Data<<4,4); //д������
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
HT1621���Գ���2008-2-13, 22:41:43
�������ƣ�void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
��������: HT1621����д�뷽ʽ����
ȫ�ֱ�������
����˵����AddrΪд���ʼ��ַ��*pΪ����д������ָ�룬
                  cntΪд����������
����˵������
˵ ����HT1621������λ4λ���˴�ÿ������Ϊ8λ��д������
           ������8λ����
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
 uchar i;
 HT1621_CS=0;
 Ht1621Wr_Data(0xa0,3); //д�����ݱ�־101
 Ht1621Wr_Data(Addr<<2,6); //д���ַ����
 for (i=0;i<cnt;i++)
      {
        Ht1621Wr_Data(*p,8); //д������
        p++;
       }
 HT1621_CS=1;
       Delay(1);
}
/********************************************************
�������ƣ�void Ht1621_Init(void)
��������: HT1621��ʼ��
ȫ�ֱ�������
����˵������
����˵������
�� ����1.0
˵ ������ʼ����Һ���������ֶξ���ʾ
********************************************************/
void Ht1621_Init(void)
{
 HT1621_CS=1;
 HT1621_WR=1;
 HT1621_DAT=1;
 DelayMS(2000); //��ʱʹLCD������ѹ�ȶ�
 Ht1621WrCmd(BIAS);
 Ht1621WrCmd(RC256); //ʹ���ڲ�����
 Ht1621WrCmd(SYSDIS);
 Ht1621WrCmd(WDTDIS);
 Ht1621WrCmd(SYSEN);
 Ht1621WrCmd(LCDON);
	Ht1621WrAllData(0,Ht1621Tab,16);
}
 
/**********************************/
void test()
{
 uchar i,j,t;
 //Ht1621_Init(); //�ϵ��ʼ��LCD
 //DelayMS(1000); //��ʱһ��ʱ��
 while(1){
               //Ht1621WrAllData(0,Ht1621Tab,16); //���1621�Ĵ������ݣ�������
	             
	             Ht1621WrOneData(0,seg_tab[0]>>4);
	             Ht1621WrOneData(1,seg_tab[0]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[1]>>4);
	             Ht1621WrOneData(1,seg_tab[1]);
								DelayMS(5000);
		             Ht1621WrOneData(0,seg_tab[2]>>4);
	             Ht1621WrOneData(1,seg_tab[2]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[3]>>4);
	             Ht1621WrOneData(1,seg_tab[3]);
								DelayMS(5000); 
	 
	             Ht1621WrOneData(0,seg_tab[4]>>4);
	             Ht1621WrOneData(1,seg_tab[4]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[5]>>4);
	             Ht1621WrOneData(1,seg_tab[5]);
								DelayMS(5000);	
		             Ht1621WrOneData(0,seg_tab[6]>>4);
	             Ht1621WrOneData(1,seg_tab[6]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[7]>>4);
	             Ht1621WrOneData(1,seg_tab[7]);
								DelayMS(5000); 
	 
	             Ht1621WrOneData(0,seg_tab[8]>>4);
	             Ht1621WrOneData(1,seg_tab[8]);
								DelayMS(5000);
	             Ht1621WrOneData(0,seg_tab[9]>>4);
	             Ht1621WrOneData(1,seg_tab[9]);
								DelayMS(5000);	 								
  }
}