
/********************************** 非标准库头文件 ***********************************/
#include "canbus.h"


/********************************** 常量定义 ***********************************/
// 125k,250k,500k,1M
u8 code can_baud[4][4]={{0x3F,0x40,0x72,0x00},{0x1F,0x40,0x72,0x00},{0x0F,0x40,0x72,0x00},{0x07,0x40,0x72,0x00}};

u32 code CanBustxaddr[27]={
    0x1C1F1830<<3,0x1C1F1831<<3,0x1C1F1832<<3,0x1C1F1833<<3,0x1C1F1834<<3,
    0x1C1F1835<<3,0x1C1F1836<<3,0x1C1F183D<<3,0x1C1F183E<<3,0x1C1F183F<<3,
    0x1C1F1840<<3,0x1C1F1841<<3,0x1C1F1842<<3,0x1C1F1843<<3,0x1C1F1850<<3,
    0x1C1F1851<<3,0x1C1F1852<<3,0x1C1F1853<<3,0x1C1F1854<<3,0x1C1F1855<<3,
	0x1C1F1800<<3,0x1C1F1822<<3,0x1C1F1825<<3,0x1C1F1860<<3,0x1C1F1722<<3,
	0x1C1F2010<<3,0x1C1F1826<<3,
};
u32 code CanBusAddr[35]={
	0x1C1F1715,0x1C1F1719,0x1C1F1714,0x1C1F1717,0x1C1F1716,
	0x1C1F1718,0x1C1F1203,0x1C1F1701,0x1C1F1823,0x1C1F2011,
	0x1C1F1827,0x1C1F1722,0x1C1F1723,0x1C1F1825,0x1C1F1870,//P8-P4
	0x1C1F1730,0x1C1F1731,0x1C1F1732,0x1C1F1733,0x1C1F1734,
	0x1C1F1735,0x1C1F1736,0x1C1F173D,0x1C1F173E,0x1C1F173F,
	0x1C1F1740,0x1C1F1741,0x1C1F1742,0x1C1F1743,0x1C1F1750,
	0x1C1F1751,0x1C1F1752,0x1C1F1753,0x1C1F1754,0x1C1F1755,
};

/********************************** 全局变量 ***********************************/
CANBUSUNIT   CanData;
//https://blog.csdn.net/weixin_44536482/article/details/89030152

/********************************** 常量定义 ***********************************/
// 125k,250k,500k,1M

static void delay_us(unsigned int t)
{ 
	u8 i,m;
	while(t)
	{
	 for(i=0;i<5;i++) //5-6 7 8-9
	 {}
			m++;
			m++;
	 t--;}
}
static void PinOutput(u8 Port,u8 Pin,u8 value)
{
	switch(Port)
	  {
			case 0: P0MDOUT|=(1<<Pin);
						break;
			  case 1: P1MDOUT|=(1<<Pin);
						break;
			  case 2: P2MDOUT|=(1<<Pin);
						break;
			  case 3: P3MDOUT|=(1<<Pin);
						break;	  
	  default:break;			  
	  }

	  switch(Port)
		{
			  case 0: if(value) P0|=(1<<Pin);
						  else		P0&=~(1<<Pin);
						  break;
				case 1: if(value) P1|=(1<<Pin);
						  else		P1&=~(1<<Pin);
						  break;
				case 2: if(value) P2|=(1<<Pin);
						  else		P2&=~(1<<Pin);
						  break;
				case 3: if(value) P3|=(1<<Pin);
						  else		P3&=~(1<<Pin);
						  break;	
		default:break;				
		}
}


/********************************** 全局函数 ***********************************/
/*CAN总线位时间参数的设定与调整
1、	确定时间份额
		时间份额数量为8~25
		位时间=1/波特率           ( 250K           4us)
		时间份额=位时间/时间份额数量        
		BRP=时间份额/(2*tclk)=  时间份额/2*FOSC=206*200/2000=40      
2、	设置时间段和采样点
		时间份额数量=1+T1+T2
		采样点80%最佳
		(1+T1)/(1+T1+T2)=0.8
		就可以确定T1  T2
		传播时间段和相位缓冲器段1  =T1
		相位缓冲器段2              =T2
3、	确定同步跳转宽度和采样次数
		同步跳转宽度1~4  尽量大

*/

//根据T5L应用开发指南3.8对CAN接口进行初始化
void CanBusInit(u16 bdtid)//如果使用CAN，那么sys.h  必须增加如下宏定义  #define INTVPACTION
{
	u16 i;

	SetPinOut(0,2);
	SetPinIn(0,3);
	PinOutput(0,2,1);
	MUX_SEL |= 0x80;		//将CAN接口引出到P0.2,P0.3	
	ADR_H = 0xFF;
	ADR_M = 0x00;
	ADR_L = 0x60;
	ADR_INC = 1;
	RAMMODE = 0x8F;		//写操作
	while(!APP_ACK);
	#if 0
	DATA3 = 17;
	DATA2 = 0x4c;
	DATA1 = 0x1f;
	DATA0 = 0x00;	
	#else
	i=bdtid;
	DATA3 = can_baud[i][0];
	DATA2 = can_baud[i][1];
	DATA1 = can_baud[i][2];
	DATA0 = can_baud[i][3];	 		
	#endif		
	APP_EN = 1;
	while(APP_EN);//0060
	DATA3 = 0;
	DATA2 = 0;
	DATA1 = 0;
	DATA0 = 0;	 		//配置验收寄存器ACR
	APP_EN = 1;	  
	while(APP_EN);//0061
	DATA3 = 0xFF;
	DATA2 = 0xFF;
	DATA1 = 0xFF;
	DATA0 = 0xFF;	 	//配置AMR
	APP_EN = 1;	
	while(APP_EN);//0062
	RAMMODE = 0;
	CAN_CR = 0xA0;
	while(CAN_CR&0x20);	//执行配置FF0060-FF0062动作
	ECAN = 1;			//打开CAN中断	
//	CanRTRFlag = 1;	
	CanData.CanRxTail = 0;
	CanData.CanRxHead = 0;
}



/**************************************************************

D3  1  CAN_RX_BUFFER  [7] IDE ，[6]RTR， [3:0]—DLC，帧数据长度。
0xFF:0068
D2:D0  3  未定义 
ID  ID，扩展帧时 29bit 有效，标准帧时 11bit 有效。
D3  1  ID 第一个字节，标准帧与扩展帧。
D2  1  ID 第二个字节，[7:5]为标准帧的高 3bit，扩展帧第 2 字节。
D1  1  ID 第三个字节，标准帧无效，扩展帧第 3 字节。
0xFF:0069
D0  1  ID 第四个字节，标准帧无效，[7:3]-扩展帧的高 5bit。
0xFF:006A  D3:D0  4  数据  接收数据，DATA1-DATA4。
0xFF:006B  D3:D0  4  数据  接收数据，DATA5-DATA8。


******************************************************************/
void CanErrorReset(void)//对于T5L1和T5L2必须在main函数，while(1)中调用
{
	// EA=0;
	if(CAN_ET&0X20)
	{
		CAN_ET &= 0XDF;
		CAN_CR |= 0X40;
		delay_us(1000);
		CAN_CR &= 0XBF;  
		CanData.CanTxFlag = 0;
	}
	// EA=1;
}


void Can_Isr() interrupt 9
{
	u8 tmp;

	EA = 0;
	if((CAN_IR&0x80) == 0x80)
	{
		CAN_IR &= 0x3F;	//清空远程帧标记位			
	}
	if((CAN_IR&0x40) == 0x40)
	{
		CAN_IR &= 0xBF;	//清空数据帧标记位
		ADR_H = 0xFF;
		ADR_M = 0x00;
		ADR_L = 0x68;
		ADR_INC = 1;
		RAMMODE = 0xAF;		//读操作
		while(!APP_ACK);
		APP_EN = 1;
		while(APP_EN);
		tmp = DATA3 ;
		CanData.BusRXbuf[CanData.CanRxHead].status=tmp;
			APP_EN = 1;
			while(APP_EN);			
			CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
			CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA3;
			CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
			CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA2;
			CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
			CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA1;
			CanData.BusRXbuf[CanData.CanRxHead].ID <<= 8;
			CanData.BusRXbuf[CanData.CanRxHead].ID |= DATA0;
				CanData.BusRXbuf[CanData.CanRxHead].ID=CanData.BusRXbuf[CanData.CanRxHead].ID>>3;		
			if((tmp & 0xc0)==0xc0)//远程帧
			{

			}

			if(0==(tmp&0x80))//标准帧ID还需要右移18位
			{
				CanData.BusRXbuf[CanData.CanRxHead].ID >>= 18;
			}
		if(0==(tmp&0x40))//数据帧才需要读取数据
		{
			APP_EN = 1;
			while(APP_EN);
			CanData.BusRXbuf[CanData.CanRxHead].candata[0] = DATA3;
			CanData.BusRXbuf[CanData.CanRxHead].candata[1] = DATA2;
			CanData.BusRXbuf[CanData.CanRxHead].candata[2] = DATA1;
			CanData.BusRXbuf[CanData.CanRxHead].candata[3] = DATA0;
			APP_EN = 1;
			while(APP_EN);
			CanData.BusRXbuf[CanData.CanRxHead].candata[4] = DATA3;
			CanData.BusRXbuf[CanData.CanRxHead].candata[5] = DATA2;
			CanData.BusRXbuf[CanData.CanRxHead].candata[6] = DATA1;
			CanData.BusRXbuf[CanData.CanRxHead].candata[7] = DATA0;		
			}

			RAMMODE = 0;
			CanData.CanRxHead++;

	}
	if((CAN_IR&0x20) == 0x20)
	{
		CAN_IR &= ~(0x20);	//清空发送帧标记位
	}
	if((CAN_IR&0x10) == 0x10)
	{
		CAN_IR &= 0xEF;	//清空接收溢出标记位
	}
	if((CAN_IR&0x08) == 0x08)
	{
		CAN_IR &= 0xF7;	//清空错误标记位
	}
	if((CAN_IR&0x04) == 0x04)
	{
		CAN_IR &= 0xFB;	//清空仲裁失败标记位
		CAN_CR |= 0x04;	//重新启动发送	
	}
	CAN_ET=0;
	EA = 1;  
}

void CanRx(void)//主程序调用--接收数据
{
	u8 i=0,j;
	u8 buffer[8];
	if(CanData.CanRxHead != CanData.CanRxTail)
	{
//		for(i=0;i<CAN_frame_NUM;i++)//轮询,接收目标ID
//		{
//			if(CanData.BusRXbuf[CanData.CanRxTail].ID==CanBusAddr[i])
//			{

//				for(j=0;j<8;j++)
//				{
//					buffer[j] = CanData.BusRXbuf[CanData.CanRxTail].candata[j];
//				}
//				//Uart_Send_Data(UART5,8,buffer);// 串口调试打印
//				break;
//			}
//		}
////////////
				for(j=0;j<8;j++)
				{
					buffer[j] = CanData.BusRXbuf[CanData.CanRxTail].candata[j];
				}		
				write_dgus_vp(0x1000,buffer,4);//test				
		CanData.CanRxTail++;

	}
}


void CAN_tx(u32 ID,  u8 *str)
{

	EA = 0;

	ADR_H = 0xFF;
	ADR_M = 0x00;
	ADR_L = 0x64;
	ADR_INC = 1;
	RAMMODE = 0x8F;		//写操作
	while(!APP_ACK);
	DATA3 = 0x88;			//帧类长度型以及数据
	DATA2 = 0;
	DATA1 = 0;
	DATA0 = 0;	 		
	APP_EN = 1;
	while(APP_EN);		//写入RTR,IDE,DLC等数据
	DATA3 = (u8)(ID>>24);
	DATA2 = (u8)(ID>>16);
	DATA1 = (u8)(ID>>8);
	DATA0 = (u8)(ID>>0);	 		
	APP_EN = 1;
	while(APP_EN);		//写入ID数据
	DATA3 = str[0];
	DATA2 = str[1];
	DATA1 = str[2];
	DATA0 = str[3];	 		
	APP_EN = 1;
	while(APP_EN);		//写入发送数据前4字节
	DATA3 = str[4];
	DATA2 = str[5];
	DATA1 = str[6];
	DATA0 = str[7];	 		
	APP_EN = 1;
	while(APP_EN);		//写入发送数据后4字节
	EA = 1;
	RAMMODE = 0;
	CAN_CR |= 0x04;		//启动发送			

	delay_ms(2);//


}
