 /*****************************************************************************
 *版权信息：深圳天微电子有限公司
 *文 件 名：TM1650-V2.0
 *当前版本：V2.0
 *MCU 型号：STC12C5608AD
 *开发环境：Keil uVision4
 *晶震频率：11.0592MHZ       
 *免责声明：1.此程序为TM1650驱动LED数码管演示程序，仅作参考之用。
            2.如有直接使用本例程程序造成经济损失的，本公司不承担任何责任             
********************************************************************************/
#include<intrins.h>				  			//包含nop指令头文件
#define nop _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();  //宏定义

//每一段显示编码
code unsigned  char  perseg[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; 
//共阴驱动显示编码，0-F-灭
code unsigned char  DispCode[17]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00}; 
//辉度调节指令,1-8级
code unsigned char INTENS[8]={0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x01}; 
 
//通讯端口定义,客户可以根据实际平台进行相应的配置;
sbit SCL_1650=P3^4;                       //时钟线
sbit SDA_1650=P3^3;                       //数据线

//读取按键值存储
uchar keya;	                     //定义读出按键返回值
bit Key_kk;
/*************1ms延时*晶振11.0592M********************
void delay(uint n)
{
  uint i;
  while(n--)
	 for(i=0;i<550;i++);
}
/************ START信号*******************************/
void TM1650_START()
{
	SCL_1650=1;
	SDA_1650=1;
	nop;
	SDA_1650=0;
	nop;
	SCL_1650=0;
}

/******************** STOP信号************************/
void TM1650_STOP()
{
	SDA_1650=0;
	nop;
	SCL_1650=1;
	nop;
	SDA_1650=1;
	nop;
	SCL_1650=0;
	SDA_1650=0;
}


/****************写1个字节给TM1650********************/
void write_8bit( uchar dat)
{
 	uchar i;
	SCL_1650=0;
	for(i=0;i<8;i++)
		{
		if(dat&0x80)
		{
			SDA_1650=1;
			nop;
			nop;
			SCL_1650=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL_1650=0;	 
		}
		else
		{
			SDA_1650=0;
			nop;
			nop;
			SCL_1650=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL_1650=0;
		}	
			dat<<=1;	 
		}
		SDA_1650=1;			//ACK信号
		nop;
		nop;
		nop;
		nop;
		SCL_1650=1;
		nop;
		nop;
		nop;
		nop;
		nop;
		SCL_1650=0;
		nop;
		nop;	 
}


/****读8bit
注意：本程序采用双向IO口，对于需要配置端口特性的MCU需要注意
在发送指令时配置为输出模式;
接受ACK为输入模式；
读数据位输入模式；
**************************/
uchar read_8bit()
{
	uchar dat,i;
	SDA_1650=1;
	dat=0;
	for(i=0;i<8;i++)
	{
	SCL_1650=1;                        //时钟上沿
	nop;
	nop;
	nop;
	dat<<=1;
	if(SDA_1650)
	 dat++;
	SCL_1650=0;
	nop;
	nop;
	nop;
	nop;
	}
	SDA_1650=0;			    //ACK信号
	nop;
	nop;
	nop;
	SCL_1650=1;
	nop;
	nop;
	nop;
	nop;
	SCL_1650=0;
	nop;
	
	return dat ;
} 

/********发送显示数据*******/
void TM1650_send(uchar saddr,uchar sdate)
{
 	TM1650_START();
	write_8bit(saddr);
	write_8bit(sdate);
	TM1650_STOP();
}

/**系统设置命令
light -- 亮度级别(00H 8级亮度、10H为1级亮度。。。。。。70H为7级亮度)
segMod -- 段模式设置(00H为8段模式，08H为7段模式)
WorkMod -- 工作模式(00H为正常模式，04H为低功耗模式)
Onoff -- 开关(00H为关显示，01H为开显示)
例如：1级亮度，7段模式，正常工作模式，开显示则函数为
TM1650_SystemCmd(0x10, 0x08, 0x01, 0x01 );
***/
void TM1650_SystemCmd(uchar light,uchar segMod, uchar WorkMod, uchar Onoff)
{
 	TM1650_START();
	write_8bit(0x48);
	write_8bit(light | segMod | WorkMod | Onoff );
	TM1650_STOP();
}

/**4位显示数据
0x68 对应DIG1
0x6A 对应DIG2
0x6C 对应DIG3
0x6E 对应DIG4
如果要DIG1-4显示0-3 则函数为TM1650Disp(DispCode[0],DispCode[1],DispCode[2],DispCode[3]);
***/
void TM1650Disp(uchar sdate1, uchar sdate2, uchar sdate3, uchar sdate4)
{
		TM1650_send(0X68,sdate1);  //GID1-sdate1
		TM1650_send(0X6A,sdate2);  //GID2-sdate2
		TM1650_send(0X6C,sdate3);  //GID3-sdate3
		TM1650_send(0X6E,sdate4);  //GID4-sdate4
}

/***TM1650上电清零操作
注意：对于关掉显示时，建议采用对所有显存进行清零操作，即4位显存发送0x00数据
***/
void TM1650Init(uchar light,uchar segMod, uchar DispMod, uchar Onoff)
{
	TM1650_SystemCmd(light, segMod,DispMod, Onoff);  //8级亮度+8段模式+正常工作模式+开显示
	//数据清零
	TM1650Disp(0x00,0x00,0x00,0x00);
}


/***
	逐段显示
	light为亮度等级(00H\10H\...70H)
***/
void TM1650PersegDisp(uchar light)
{
	unsigned char i=0,j=0,Addr=0x68,fdate=0x01;
	TM1650_SystemCmd(light, 0x00, 0x00, 0x01);  //light亮度+8段模式+正常工作模式+开显示
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

/***显示1-99数据***/
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

/*******************读按键命令************************/
uchar TM1650_read()
{
	uchar key;
	TM1650_START();
    write_8bit(0x4F);   //读按键指令	
	key=read_8bit();
	TM1650_STOP();
	return key;
}


/***按键处理函数
    注意：该程序未处理按键松开时的情况
***/
void TM1650ReadKey(void)
{		
		switch(KeyDate)
		{
			/**单个按键**/
			case 0x44:TM1650Disp2Num(1);break;    //KI1+DIG1
			case 0x45:TM1650Disp2Num(2);break;		//KI1+DIG2
			case 0x46:if(Key_kk==0){Key_kk=1;schc=~schc;kzhi_com();}break;		//KI1+DIG3
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
			
			/**组合按键**/
			case 0x7C:TM1650Disp2Num(29);break;		//KI1_KI2+DIG1
			case 0x7d:TM1650Disp2Num(30);break;		//KI1_KI2+DIG2
			case 0x7e:TM1650Disp2Num(31);break;		//KI1_KI2+DIG3
			case 0x7f:TM1650Disp2Num(32);break;  	//KI1_KI2+DIG4
			
			default: break;
		}
}
void key_mun()
{
		uchar Key_A;
		Key_A=KeyDate;
		if(Key_A!=0xFF)
		{
				delaya(20);
				if(Key_A==KeyDate)
				{
						if((Key_A!=0xFF)&(Key_A!=0x00))
							 TM1650ReadKey();
				}
		}else
		{
				delaya(20); 
			 // if(Key_A==0)
					Key_kk=0;
		}
	
}


/**************主函数**************************
void main(void)
{ 
	
	TM1650_SystemCmd(0x00,0x00,0x00,0x01);    //1级亮度+8段模式+正常工作模式+开显示
	TM1650Disp(0x00,0x00,0x00,0x00);          //上电清零操作
	
  /**具体显示数据客户可根据实际情况来显示***
	TM1650Disp(DispCode[1],DispCode[2],DispCode[3],DispCode[4]);   //显示1、2、3、4
                               
	while(1)
	{
		 TM1650ReadKey();  //200ms读一次按键值
		 delay(200);
	} 
} 

*/




 