#include<CS1237.h>     //20bit ADC
//��ַ��WWW.JIXIN.PRO


#define ADC_Bit  20 //ADC��Чλ����������λ ���24λ
#define SCK_1  SCLK = 1
#define SCK_0  SCLK = 0
#define DAT_1  DOUT = 1
#define DAT_0  DOUT = 0
#define	NOP_5()		_nop_();_nop_()
#define	NOP30()		NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define	NOP40()		NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();NOP_5();
#define One_CLK  SCK_1;NOP40();SCK_0;NOP40();
#define CS_CON  0X0C   //оƬ������ �ڲ�REF ���40HZ PGA=128 ͨ��A 0X1C   

sbit DOUT = P3^7;//���ݶ�ӦIO��
sbit SCLK = P3^5;//ʱ�Ӷ�ӦIO��

//��ʱ500US 25MHZ
void delay_500us(unsigned char a)
{	
	unsigned char i,j,b;
	for(b=0;b<a;b++)
	{
		i = 13;
		j = 37;
		do
		{
			while (--j);
		} while (--i);
	}
}
//����CS1237оƬ
void Con_CS1237(void)
{
	unsigned char i;
	unsigned char dat;
	unsigned char count_i=0;//�����ʱ��
	dat = CS_CON;// 0100 1000
	SCK_0;//ʱ������
	while(DOUT)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237ȫ������Ϊ0���㶼׼����
	{
		delay_500us(10);
		count_i++;
		if(count_i > 150)
		{
			SCK_1;
			DAT_1;
			return;//��ʱ����ֱ���˳�����
		}
	}
	for(i=0;i<29;i++)// 1 - 29
	{
		One_CLK;
	}
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//30
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//31
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//32
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//33
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//34
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//35
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//36
	One_CLK;//37     д����0x65
	for(i=0;i<8;i++)// 38 - 45�������ˣ�д8λ����
	{
		SCK_1;NOP40();
		if(dat&0x80)
			DAT_1;
		else
			DAT_0;
		dat <<= 1;
		SCK_0;NOP40();
	}
	One_CLK;//46������������������
}
//��ȡоƬ����������
unsigned char Read_CON(void)
{
	unsigned char i;
	unsigned char dat=0;//��ȡ��������
	unsigned char count_i=0;//�����ʱ��
	unsigned char k=0,j=0;//�м����
	SCK_0;//ʱ������
	while(DOUT)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237ȫ������Ϊ0���㶼׼����
	{
		delay_500us(10);
		count_i++;
		if(count_i > 150)
		{
			SCK_1;
			DAT_1;
			return 1;//��ʱ����ֱ���˳�����
		}
	}
	for(i=0;i<29;i++)// 1 - 29
	{
		One_CLK;
	}
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//30
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//31
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//32
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//33
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//34
	SCK_1;NOP30();DAT_1;SCK_0;NOP30();//35
	SCK_1;NOP30();DAT_0;SCK_0;NOP30();//36
	DAT_1;
	One_CLK;//37     д����0x56
	dat=0;
	for(i=0;i<8;i++)// 38 - 45�������ˣ���ȡ����
	{
		One_CLK;
		dat <<= 1;
		if(DOUT)
			dat++;
	}
	One_CLK;//46������������������
	return dat;
}
//��ȡADC���ݣ����ص���һ���з�������
long Read_CS1237(void)
{
	unsigned char i;
	long dat=0;//��ȡ��������
	unsigned char count_i=0;//�����ʱ��
	DOUT = 1;//�˿�����1��51�ر�
	SCK_0;//ʱ������
	while(DOUT)//оƬ׼�����������  ʱ���Ѿ�Ϊ0������Ҳ��Ҫ��CS1237����Ϊ0���㶼׼����
	{
		delay_500us(10);
		count_i++;
		if(count_i > 300)
		{
			SCK_1;
			DAT_1;
			return 0;//��ʱ����ֱ���˳�����
		}
	}
	DOUT = 1;//�˿�����1��51�ر�
	dat=0;
	for(i=0;i<24;i++)//��ȡ24λ��Чת��
	{
		SCK_1;
		NOP40();
		dat <<= 1;
		if(DOUT)
			dat ++;
		SCK_0;
		NOP40();	
	}
	for(i=0;i<3;i++)//һ������27������
	{
		One_CLK;
	}
	DAT_1;
	//�ȸ��ݺ궨���������Чλ������һЩ����
	i = 24 - ADC_Bit;//i��ʾ��Ҫ������λ��
	dat >>= i;//���������λ��
	return dat;
}
//
