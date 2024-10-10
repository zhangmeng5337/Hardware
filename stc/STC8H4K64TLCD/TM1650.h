 /*****************************************************************************
 *��Ȩ��Ϣ��������΢�������޹�˾
 *�� �� ����TM1650-V2.0
 *��ǰ�汾��V2.0
 *MCU �ͺţ�STC12C5608AD
 *����������Keil uVision4
 *����Ƶ�ʣ�11.0592MHZ       
 *����������1.�˳���ΪTM1650����LED�������ʾ���򣬽����ο�֮�á�
            2.����ֱ��ʹ�ñ����̳�����ɾ�����ʧ�ģ�����˾���е��κ�����             
********************************************************************************/
#include<reg52.h>				  				//MCUͷ�ļ�
#include<intrins.h>				  			//����nopָ��ͷ�ļ�
#define uint unsigned int			  	//�������ͺ궨��
#define uchar unsigned char			  //�������ͺ궨��

#define nop _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();  //�궨��

//ÿһ����ʾ����
code unsigned  char  perseg[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; 
//����������ʾ���룬0-F-��
code unsigned char  DispCode[17]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00}; 
//�Զȵ���ָ��,1-8��
code unsigned char INTENS[8]={0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x01}; 
 
//ͨѶ�˿ڶ���,�ͻ����Ը���ʵ��ƽ̨������Ӧ������;
sbit SCL=P3^2;                       //ʱ����
sbit SDA=P3^4;                       //������

//��ȡ����ֵ�洢
uchar keya;	                     //���������������ֵ

/*************1ms��ʱ*����11.0592M********************/
void delay(uint n)
{
  uint i;
  while(n--)
	 for(i=0;i<550;i++);
}



/************ START�ź�*******************************/
void TM1650_START()
{
	SCL=1;
	SDA=1;
	nop;
	SDA=0;
	nop;
	SCL=0;
}

/******************** STOP�ź�************************/
void TM1650_STOP()
{
	SDA=0;
	nop;
	SCL=1;
	nop;
	SDA=1;
	nop;
	SCL=0;
	SDA=0;
}


/****************д1���ֽڸ�TM1650********************/
void write_8bit( uchar dat)
{
 	uchar i;
	SCL=0;
	for(i=0;i<8;i++)
		{
		if(dat&0x80)
		{
			SDA=1;
			nop;
			nop;
			SCL=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL=0;	 
		}
		else
		{
			SDA=0;
			nop;
			nop;
			SCL=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL=0;
		}	
			dat<<=1;	 
		}
		SDA=1;			//ACK�ź�
		nop;
		nop;
		nop;
		nop;
		SCL=1;
		nop;
		nop;
		nop;
		nop;
		nop;
		SCL=0;
		nop;
		nop;	 
}


/****��8bit
ע�⣺���������˫��IO�ڣ�������Ҫ���ö˿����Ե�MCU��Ҫע��
�ڷ���ָ��ʱ����Ϊ���ģʽ;
����ACKΪ����ģʽ��
������λ����ģʽ��
**************************/
uchar read_8bit()
{
	uchar dat,i;
	SDA=1;
	dat=0;
	for(i=0;i<8;i++)
	{
	SCL=1;                        //ʱ������
	nop;
	nop;
	nop;
	dat<<=1;
	if(SDA)
	 dat++;
	SCL=0;
	nop;
	nop;
	nop;
	nop;
	}
	SDA=0;			    //ACK�ź�
	nop;
	nop;
	nop;
	SCL=1;
	nop;
	nop;
	nop;
	nop;
	SCL=0;
	nop;
	
	return dat ;
} 

/********������ʾ����*******/
void TM1650_send(uchar saddr,uchar sdate)
{
 	TM1650_START();
	write_8bit(saddr);
	write_8bit(sdate);
	TM1650_STOP();
}

/**ϵͳ��������
light -- ���ȼ���(00H 8�����ȡ�10HΪ1�����ȡ�����������70HΪ7������)
segMod -- ��ģʽ����(00HΪ8��ģʽ��08HΪ7��ģʽ)
WorkMod -- ����ģʽ(00HΪ����ģʽ��04HΪ�͹���ģʽ)
Onoff -- ����(00HΪ����ʾ��01HΪ����ʾ)
���磺1�����ȣ�7��ģʽ����������ģʽ������ʾ����Ϊ
TM1650_SystemCmd(0x10, 0x08, 0x01, 0x01 );
***/
void TM1650_SystemCmd(uchar light,uchar segMod, uchar WorkMod, uchar Onoff)
{
 	TM1650_START();
	write_8bit(0x48);
	write_8bit(light | segMod | WorkMod | Onoff );
	TM1650_STOP();
}

/**4λ��ʾ����
0x68 ��ӦDIG1
0x6A ��ӦDIG2
0x6C ��ӦDIG3
0x6E ��ӦDIG4
���ҪDIG1-4��ʾ0-3 ����ΪTM1650Disp(DispCode[0],DispCode[1],DispCode[2],DispCode[3]);
***/
void TM1650Disp(uchar sdate1, uchar sdate2, uchar sdate3, uchar sdate4)
{
		TM1650_send(0X68,sdate1);  //GID1-sdate1
		TM1650_send(0X6A,sdate2);  //GID2-sdate2
		TM1650_send(0X6C,sdate3);  //GID3-sdate3
		TM1650_send(0X6E,sdate4);  //GID4-sdate4
}

/***TM1650�ϵ��������
ע�⣺���ڹص���ʾʱ��������ö������Դ���������������4λ�Դ淢��0x00����
***/
void TM1650Init(uchar light,uchar segMod, uchar DispMod, uchar Onoff)
{
	TM1650_SystemCmd(light, segMod,DispMod, Onoff);  //8������+8��ģʽ+��������ģʽ+����ʾ
	//��������
	TM1650Disp(0x00,0x00,0x00,0x00);
}


/***
	�����ʾ
	lightΪ���ȵȼ�(00H\10H\...70H)
***/
void TM1650PersegDisp(uchar light)
{
	unsigned char i=0,j=0,Addr=0x68,fdate=0x01;
	TM1650_SystemCmd(light, 0x00, 0x00, 0x01);  //light����+8��ģʽ+��������ģʽ+����ʾ
	for(j=0; j<4; j++)
	{
			for(i=0; i<9; i++)
			{
					TM1650_send(Addr, fdate);
					fdate |= perseg[i];
					delay(100);
			}
			Addr+=2;
			fdate=perseg[0];
	}
}

/***��ʾ1-99����***/
void TM1650Disp2Num(unsigned char snum)
{
	 unsigned char i,j;
	 i=snum%10;
	 j=snum/10;
	
		if(snum<10)
		{
			TM1650_send(0x68, DispCode[j]);
			TM1650_send(0x6a, 0x00);
		}else{
			TM1650_send(0x68, DispCode[j]);
			TM1650_send(0x6a, DispCode[i]);
		}
}

/*******************����������************************/
uchar TM1650_read()
{
	uchar key;
	TM1650_START();
    write_8bit(0x4F);   //������ָ��	
	key=read_8bit();
	TM1650_STOP();
	return key;
}


/***����������
    ע�⣺�ó���δ�������ɿ�ʱ�����
***/
void TM1650ReadKey(void)
{
		unsigned char KeyDate=0x00;
	  KeyDate = TM1650_read();
		switch(KeyDate)
		{
			/**��������**/
			case 0x44:TM1650Disp2Num(1);break;    //KI1+DIG1
			case 0x45:TM1650Disp2Num(2);break;		//KI1+DIG2
			case 0x46:TM1650Disp2Num(3);break;		//KI1+DIG3
			case 0x47:TM1650Disp2Num(4);break;		//KI1+DIG4
			
			case 0x4C:TM1650Disp2Num(5);break;		//KI2+DIG1
			case 0x4d:TM1650Disp2Num(6);break;		//KI2+DIG2
			case 0x4e:TM1650Disp2Num(7);break;		//KI2+DIG3
			case 0x4f:TM1650Disp2Num(8);break;		//KI2+DIG4
			
			case 0x54:TM1650Disp2Num(9);break;		//KI3+DIG1
			case 0x55:TM1650Disp2Num(10);break;		//KI3+DIG2
			case 0x56:TM1650Disp2Num(11);break;		//KI3+DIG3
			case 0x57:TM1650Disp2Num(12);break;		//KI3+DIG4
			
			case 0x5C:TM1650Disp2Num(13);break;		//KI4+DIG1
			case 0x5d:TM1650Disp2Num(14);break;		//KI4+DIG2
			case 0x5e:TM1650Disp2Num(15);break;		//KI4+DIG3
			case 0x5f:TM1650Disp2Num(16);break;		//KI4+DIG4
			
			case 0x64:TM1650Disp2Num(17);break;		//KI5+DIG1
			case 0x65:TM1650Disp2Num(18);break;		//KI5+DIG2
			case 0x66:TM1650Disp2Num(19);break;		//KI5+DIG3
			case 0x67:TM1650Disp2Num(20);break;		//KI5+DIG4
			
			case 0x6C:TM1650Disp2Num(21);break;		//KI6+DIG1
			case 0x6d:TM1650Disp2Num(22);break;		//KI6+DIG2
			case 0x6e:TM1650Disp2Num(23);break;		//KI6+DIG3
			case 0x6f:TM1650Disp2Num(24);break;		//KI6+DIG4
			
			case 0x74:TM1650Disp2Num(25);break;		//KI7+DIG1
			case 0x75:TM1650Disp2Num(26);break;		//KI7+DIG2
			case 0x76:TM1650Disp2Num(27);break;		//KI7+DIG3
			case 0x77:TM1650Disp2Num(28);break;		//KI7+DIG4
			
			/**��ϰ���**/
			case 0x7C:TM1650Disp2Num(29);break;		//KI1_KI2+DIG1
			case 0x7d:TM1650Disp2Num(30);break;		//KI1_KI2+DIG2
			case 0x7e:TM1650Disp2Num(31);break;		//KI1_KI2+DIG3
			case 0x7f:TM1650Disp2Num(32);break;  	//KI1_KI2+DIG4
			
			default: break;
		}
}



/**************������**************************/
void main(void)
{ 
	
	TM1650_SystemCmd(0x00,0x00,0x00,0x01);    //1������+8��ģʽ+��������ģʽ+����ʾ
	TM1650Disp(0x00,0x00,0x00,0x00);          //�ϵ��������
	
  /**������ʾ���ݿͻ��ɸ���ʵ���������ʾ***/
	TM1650Disp(DispCode[1],DispCode[2],DispCode[3],DispCode[4]);   //��ʾ1��2��3��4
                               
	while(1)
	{
		 TM1650ReadKey();  //200ms��һ�ΰ���ֵ
		 delay(200);
	} 
} 






 