
/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/
#include "canbus.h"


/********************************** �������� ***********************************/
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

/********************************** ȫ�ֱ��� ***********************************/
CANBUSUNIT   CanData;
//https://blog.csdn.net/weixin_44536482/article/details/89030152

/********************************** �������� ***********************************/
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


/********************************** ȫ�ֺ��� ***********************************/
/*CAN����λʱ��������趨�����
1��	ȷ��ʱ��ݶ�
		ʱ��ݶ�����Ϊ8~25
		λʱ��=1/������           ( 250K           4us)
		ʱ��ݶ�=λʱ��/ʱ��ݶ�����        
		BRP=ʱ��ݶ�/(2*tclk)=  ʱ��ݶ�/2*FOSC=206*200/2000=40      
2��	����ʱ��κͲ�����
		ʱ��ݶ�����=1+T1+T2
		������80%���
		(1+T1)/(1+T1+T2)=0.8
		�Ϳ���ȷ��T1  T2
		����ʱ��κ���λ��������1  =T1
		��λ��������2              =T2
3��	ȷ��ͬ����ת��ȺͲ�������
		ͬ����ת���1~4  ������

*/

//����T5LӦ�ÿ���ָ��3.8��CAN�ӿڽ��г�ʼ��
void CanBusInit(u16 bdtid)//���ʹ��CAN����ôsys.h  �����������º궨��  #define INTVPACTION
{
	u16 i;

	SetPinOut(0,2);
	SetPinIn(0,3);
	PinOutput(0,2,1);
	MUX_SEL |= 0x80;		//��CAN�ӿ�������P0.2,P0.3	
	ADR_H = 0xFF;
	ADR_M = 0x00;
	ADR_L = 0x60;
	ADR_INC = 1;
	RAMMODE = 0x8F;		//д����
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
	DATA0 = 0;	 		//�������ռĴ���ACR
	APP_EN = 1;	  
	while(APP_EN);//0061
	DATA3 = 0xFF;
	DATA2 = 0xFF;
	DATA1 = 0xFF;
	DATA0 = 0xFF;	 	//����AMR
	APP_EN = 1;	
	while(APP_EN);//0062
	RAMMODE = 0;
	CAN_CR = 0xA0;
	while(CAN_CR&0x20);	//ִ������FF0060-FF0062����
	ECAN = 1;			//��CAN�ж�	
//	CanRTRFlag = 1;	
	CanData.CanRxTail = 0;
	CanData.CanRxHead = 0;
}



/**************************************************************

D3  1  CAN_RX_BUFFER  [7] IDE ��[6]RTR�� [3:0]��DLC��֡���ݳ��ȡ�
0xFF:0068
D2:D0  3  δ���� 
ID  ID����չ֡ʱ 29bit ��Ч����׼֡ʱ 11bit ��Ч��
D3  1  ID ��һ���ֽڣ���׼֡����չ֡��
D2  1  ID �ڶ����ֽڣ�[7:5]Ϊ��׼֡�ĸ� 3bit����չ֡�� 2 �ֽڡ�
D1  1  ID �������ֽڣ���׼֡��Ч����չ֡�� 3 �ֽڡ�
0xFF:0069
D0  1  ID ���ĸ��ֽڣ���׼֡��Ч��[7:3]-��չ֡�ĸ� 5bit��
0xFF:006A  D3:D0  4  ����  �������ݣ�DATA1-DATA4��
0xFF:006B  D3:D0  4  ����  �������ݣ�DATA5-DATA8��


******************************************************************/
void CanErrorReset(void)//����T5L1��T5L2������main������while(1)�е���
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
		CAN_IR &= 0x3F;	//���Զ��֡���λ			
	}
	if((CAN_IR&0x40) == 0x40)
	{
		CAN_IR &= 0xBF;	//�������֡���λ
		ADR_H = 0xFF;
		ADR_M = 0x00;
		ADR_L = 0x68;
		ADR_INC = 1;
		RAMMODE = 0xAF;		//������
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
			if((tmp & 0xc0)==0xc0)//Զ��֡
			{

			}

			if(0==(tmp&0x80))//��׼֡ID����Ҫ����18λ
			{
				CanData.BusRXbuf[CanData.CanRxHead].ID >>= 18;
			}
		if(0==(tmp&0x40))//����֡����Ҫ��ȡ����
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
		CAN_IR &= ~(0x20);	//��շ���֡���λ
	}
	if((CAN_IR&0x10) == 0x10)
	{
		CAN_IR &= 0xEF;	//��ս���������λ
	}
	if((CAN_IR&0x08) == 0x08)
	{
		CAN_IR &= 0xF7;	//��մ�����λ
	}
	if((CAN_IR&0x04) == 0x04)
	{
		CAN_IR &= 0xFB;	//����ٲ�ʧ�ܱ��λ
		CAN_CR |= 0x04;	//������������	
	}
	CAN_ET=0;
	EA = 1;  
}

void CanRx(void)//���������--��������
{
	u8 i=0,j;
	u8 buffer[8];
	if(CanData.CanRxHead != CanData.CanRxTail)
	{
//		for(i=0;i<CAN_frame_NUM;i++)//��ѯ,����Ŀ��ID
//		{
//			if(CanData.BusRXbuf[CanData.CanRxTail].ID==CanBusAddr[i])
//			{

//				for(j=0;j<8;j++)
//				{
//					buffer[j] = CanData.BusRXbuf[CanData.CanRxTail].candata[j];
//				}
//				//Uart_Send_Data(UART5,8,buffer);// ���ڵ��Դ�ӡ
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
	RAMMODE = 0x8F;		//д����
	while(!APP_ACK);
	DATA3 = 0x88;			//֡�೤�����Լ�����
	DATA2 = 0;
	DATA1 = 0;
	DATA0 = 0;	 		
	APP_EN = 1;
	while(APP_EN);		//д��RTR,IDE,DLC������
	DATA3 = (u8)(ID>>24);
	DATA2 = (u8)(ID>>16);
	DATA1 = (u8)(ID>>8);
	DATA0 = (u8)(ID>>0);	 		
	APP_EN = 1;
	while(APP_EN);		//д��ID����
	DATA3 = str[0];
	DATA2 = str[1];
	DATA1 = str[2];
	DATA0 = str[3];	 		
	APP_EN = 1;
	while(APP_EN);		//д�뷢������ǰ4�ֽ�
	DATA3 = str[4];
	DATA2 = str[5];
	DATA1 = str[6];
	DATA0 = str[7];	 		
	APP_EN = 1;
	while(APP_EN);		//д�뷢�����ݺ�4�ֽ�
	EA = 1;
	RAMMODE = 0;
	CAN_CR |= 0x04;		//��������			

	delay_ms(2);//


}
