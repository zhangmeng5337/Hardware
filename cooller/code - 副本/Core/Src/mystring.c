#include "mystring.h"

#include "math.h"
void string_to_float(char *string, float *data,uint32_t *result)
{
	unsigned int i=0,j=0,k=0;
	unsigned char flag=0;  			//�ж������ŵı�־
	unsigned char flag_dot=1; 	//�ж�С����ı�־
	float num=0;     					//��ʱ�洢�������ı���
	
	for(i=0;*(string+i)!=0;i++) //ѭ��ֱ���ַ�����β
	{
		if(*(string+i)>='0'&&*(string+i)<='9'&&flag_dot==1)  //�����ǰ�ַ�Ϊ��������С����֮ǰ
		{
			if(j==0) num = num*pow(10,j)+(double)(*(string+i)-'0');     //���㲢�洢�м������
			else     num = num*pow(10,1)+(double)(*(string+i)-'0');
			j++;
		}
		else if(*(string+i)>='0'&&*(string+i)<='9'&&flag_dot==0) //�����ǰ�ַ�Ϊ��������С����֮��
		{
			num = num+(double)(*(string+i)-'0')*pow(0.1,j+1);     //���㲢�洢�м������
			j++;
		}
		else if(*(string+i)=='.')                               //������С�����򽫶�Ӧ��־λ��ֵ�ı�
		{
			flag_dot=0;
			j=0;
		}
		else if(*(string+i)=='-')                              //��������ͬ���ı��Ӧ��־λ��ֵ
		{
			flag = 1;
		}
		else if(*(string+i)==',')                             //����һ�����ݣ����ñ�־λ����¼���ռ�����
		{
			*(data+k) = num*pow(-1,flag);
			flag = 0;
			flag_dot=1;
			j=0;
			k++;
			num = 0;
		}
	}
	*(data+k) = num*pow(-1,flag);                            //�������һ����
	*result = *(data+k);
}

void float2char(float slope,char*buffer,int n)  //�����������洢���ַ����飬�ַ�����ĳ���
{
    int temp,i,j;
    if(slope>=0)//�ж��Ƿ����0
        buffer[0] = ' ';
    else
    {
        buffer[0] = '-';
        slope = -slope;
    }
    temp = (int)slope;//ȡ��������
    for(i=0;temp!=0;i++)//�����������ֵ�λ��
        temp /=10;
    temp =(int)slope;
    for(j=i;j>0;j--)//����������ת�����ַ�����
    {
        buffer[j] = temp%10+'0';
        temp /=10;
    }
    buffer[i+1] = '.';
    slope -=(int)slope;
    for(i=i+2;i<n-1;i++)//��С������ת�����ַ�����
    {
        slope*=10;
        buffer[i]=(int)slope+'0';
        slope-=(int)slope;
    }
    buffer[n-1] = '\0';
}


