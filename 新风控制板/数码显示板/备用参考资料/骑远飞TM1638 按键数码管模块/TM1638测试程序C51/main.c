

#include	<REGX51.H>
#include	<tm1638.h>

unsigned char num[8];		//�����������ʾ��ֵ

int main(void)
{
	unsigned char i;
	init_TM1638();	                           //��ʼ��TM1638
	for(i=0;i<8;i++)
	Write_DATA(i<<1,tab[0]);	               //��ʼ���Ĵ���
	
	while(1)
	{
		i=Read_key();                          //������ֵ
		if(i<8)
		{
			num[i]++;
			while(Read_key()==i);		       //�ȴ������ͷ�
			if(num[i]>15)
			num[i]=0;
			Write_DATA(i*2,tab[num[i]]);
			Write_allLED(1<<i);
		}
	}
}

