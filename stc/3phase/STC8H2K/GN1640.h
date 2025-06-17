/*������ƶ˿�*/
sbit Din=P2^5;	//����DIN���ݶ˿�
sbit CLK=P2^4;	//����CLK���ݶ˿�
void delay(){;}
#define uchar  unsigned char
#define bchar bdata unsigned char
#define uint   unsigned int
#define ulong  unsigned long
#define xchar  xdata unsigned char
#define xint   xdata unsigned int
#define xlong  unsigned long

uchar Disp_liangdu=7;  //��ʾ����
xchar  cp[16];	
uchar code shu[]=
{
	0x3F,0x06,0x5B,0x4F,0x66,
	0x6D,0x7D,0x07,0x7F,0x6F,
	0x77,0x7c,0x39,0x5e,0x79,0x71,
};
/*Start����*/
void I2CStart()
{
    Din=1;
    CLK=1;
	delay();delay();
	delay();delay();
	Din=1; 
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
	delay();delay();
	delay();delay();
	CLK=0;
	delay();delay();
	delay();delay();
}
/*Stop����*/
void I2CStop()
{
	CLK=1;
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
	Din=1;
	delay();delay();
	delay();delay();
	CLK=0;
	Din=0;

}
/*����8bit����,�ӵ�λ��ʼ*/
void I2CWritebyte(uchar oneByte)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(oneByte&0x01)Din=1;
		else Din=0;
		delay();delay();
		delay();delay();
		delay();delay();
		CLK=1;
		oneByte=oneByte>>1;
	}
	CLK=0;
	delay();delay();
	delay();delay();
	delay();delay();
	Din=0;
	delay();delay();
	delay();delay();
}
//�ͳ�8λ��������
void out_disp(void)
{
	uchar i;
	I2CStart();
	I2CWritebyte(0x40);	//������������:��ͨģʽ,��ַ�Զ���һ
	I2CStop();
	
	I2CStart();
	I2CWritebyte(0xc0);	//��ַ��������:��ʼ��ַ00H
	for(i=0;i<16;i++)	//����16λ��ʾ����
	{
		I2CWritebyte(cp[i]);
	}
	I2CStop();
	I2CStart();	
	switch(Disp_liangdu)
	{
		case 0:	I2CWritebyte(0x88);break;
		case 1:	I2CWritebyte(0x89);break;
		case 2:	I2CWritebyte(0x8A);break;
		case 3:	I2CWritebyte(0x8b);break;  //��ʾ����:��ʾ��,��������Ϊ12/16
		case 4:	I2CWritebyte(0x8c);break;
		case 5:	I2CWritebyte(0x8d);break;
		case 6:	I2CWritebyte(0x8e);break;
		case 7:	I2CWritebyte(0x8f);break;
		default:I2CWritebyte(0x8b);break;
	}  
	I2CStop();
}
void DP_disp(uchar x,uint dds) //x ��ʾ�� 0-2 ������ 
{
	cp[x*4+0]=shu[dds/1000];
	cp[x*4+1]=shu[dds/100%10];
	cp[x*4+2]=shu[dds%100/10];
	cp[x*4+3]=shu[dds%10];
}
void led_dd(uchar led)
{  	
	switch(led)
	{
		case 0: cp[12]=0x7b;cp[13]=0x21;cp[14]=0x20;break;	//��ѹ
		case 1: cp[12]=0x29;cp[13]=0x12;cp[14]=0x10;break;	//����
		case 2: cp[12]=0x00;cp[13]=0x00;cp[14]=0x04;break;	//�й�����,�޹�����,��������,
		case 3: cp[12]=0x00;cp[13]=0x40;cp[14]=0x34;break;  //Ƶ��	Hz
		case 4: cp[12]=0x00;cp[13]=0x00;cp[14]=0x84;break;  //kh  �й�����
		case 5: cp[12]=0x00;cp[13]=0x08;cp[14]=0x04;break;  //  �޹�����
		case 6: cp[12]=0x29;cp[13]=0x00;cp[14]=0x04;break; 	// A B C���й�����
		case 7: cp[12]=0x29;cp[13]=0x00;cp[14]=0x04;break;  //var  	// A B C���޹�����
		case 8: cp[12]=0x29;cp[13]=0x00;cp[14]=0x00;break;  //cos  	// A B C�๦������
		case 9: cp[12]=0x29;cp[13]=0x33;cp[14]=0x34;break;  //VA	// ABC�����ع���
		case 10: cp[12]=0x00;cp[13]=0x00;cp[14]=0x00; break; 
		case 11: cp[14]=cp[14]|0x02;cp[14]=cp[14]|0x01;cp[12]=cp[12]|0x04;	break;  //DI DO
		default: cp[12]=0x00;cp[13]=0x00;cp[14]=0x00;
	} 	 
}