#include "mystring.h"

#include "math.h"
void string_to_float(char *string, float *data,uint32_t *result)
{
	unsigned int i=0,j=0,k=0;
	unsigned char flag=0;  			//判断正负号的标志
	unsigned char flag_dot=1; 	//判断小数点的标志
	float num=0;     					//临时存储计算结果的变量
	
	for(i=0;*(string+i)!=0;i++) //循环直到字符串结尾
	{
		if(*(string+i)>='0'&&*(string+i)<='9'&&flag_dot==1)  //如果当前字符为数字且在小数点之前
		{
			if(j==0) num = num*pow(10,j)+(double)(*(string+i)-'0');     //运算并存储中间计算结果
			else     num = num*pow(10,1)+(double)(*(string+i)-'0');
			j++;
		}
		else if(*(string+i)>='0'&&*(string+i)<='9'&&flag_dot==0) //如果当前字符为数字且在小数点之后
		{
			num = num+(double)(*(string+i)-'0')*pow(0.1,j+1);     //运算并存储中间计算结果
			j++;
		}
		else if(*(string+i)=='.')                               //读到了小数点则将对应标志位数值改变
		{
			flag_dot=0;
			j=0;
		}
		else if(*(string+i)=='-')                              //读到减号同样改变对应标志位的值
		{
			flag = 1;
		}
		else if(*(string+i)==',')                             //读完一个数据，重置标志位，记录最终计算结果
		{
			*(data+k) = num*pow(-1,flag);
			flag = 0;
			flag_dot=1;
			j=0;
			k++;
			num = 0;
		}
	}
	*(data+k) = num*pow(-1,flag);                            //补上最后一个数
	*result = *(data+k);
}

void float2char(float slope,char*buffer,int n)  //浮点型数，存储的字符数组，字符数组的长度
{
    int temp,i,j;
//    if(slope>=0)//判断是否大于0
//        buffer[0] = ' ';
//    else
//    {
//        buffer[0] = '-';
//        slope = -slope;
//    }
//    temp = (int)slope;//取整数部分
//    i = 0;

	sprintf(&buffer[0], "%.1f,", slope);

	
//    for(i=0;temp!=0;i++)//计算整数部分的位数
//        temp /=10;
//    temp =(int)slope;
//    for(j=i;j>0;j--)//将整数部分转换成字符串型
//    {
//        buffer[j] = temp%10+'0';
//        temp /=10;
//    }
//    buffer[i+1] = '.';
//    slope -=(int)slope;
//    for(i=i+2;i<n-1;i++)//将小数部分转换成字符串型
//    {
//        slope*=10;
//        buffer[i]=(int)slope+'0';
//        slope-=(int)slope;
//    }
//    buffer[n-1] = '\0';
}


