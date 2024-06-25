#include "filter.h"

/************************ ���������˲��� *****************************/

float buffer[6][window_size] = {0}; //������������buf

/*********************** ���������˲����� ****************************/
float sliding_average_filter(unsigned char adc_num, float value)
{
    static int data_num = 0;
    float output[8] = {0};
    output[adc_num] = -1;
    if (data_num < window_size) //�������ڣ������
    {
        buffer[adc_num][data_num++] = value;
        output[adc_num] = value; //������ͬ��ֵ
        output[adc_num] = -1;
    }
    else
    {
        int i = 0;
        float sum[8] = {0};

        memcpy(&buffer[adc_num][0], &buffer[adc_num][1],
               (window_size - 1) * 4); //��1֮��������Ƶ�0֮�󣬼��Ƴ�ͷ��
        buffer[adc_num][window_size - 1] = value;                       //�����β��

        for (i = 0; i < window_size; i++) //ÿһ�ζ����㣬���Ա����ۼƸ���������
            sum[adc_num] += buffer[adc_num][i];

        output[adc_num] = sum[adc_num] / window_size;
    }

    return output[adc_num];
}

float filter(unsigned char adc_num, float *p)
{
    static float *value_buf, temp, result;
    static unsigned int  i, j ;

    result = -1;
    value_buf = p;
    //if (i >= N)
    {
        i = 0;
        for (j = 0; j < N - 1; j++)
        {
            for (i = 0; i < N - j; i++)
            {
                if (value_buf[i] > value_buf[i + 1])
                {
                    temp = value_buf[i];
                    value_buf[i] = value_buf[i + 1];
                    value_buf[i + 1] = temp;
                }
            }
        }
        result = value_buf[(N - 1) / 2];
        //result = sliding_average_filter(adc_num,result);//


    }

    return result;
}

float  GetMedianNum(float *bArray, int iFilterLen)
{
    int i, j; // ѭ������
    float bTemp;

    // ��ð�ݷ��������������
    for (j = 0; j < iFilterLen - 1; j ++)
    {
        for (i = 0; i < iFilterLen - j - 1; i ++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // ����
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }

    // ������ֵ
    if ((iFilterLen & 1) > 0)
    {
        // ������������Ԫ�أ������м�һ��Ԫ��
        bTemp = bArray[(iFilterLen + 1) / 2];
    }
    else
    {
        // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
    }

    return bTemp;
}

float average_filter(float bArray)
{
	static float  sum = 0, result;
	   static unsigned int count, flag=0, i;
	   static float buf[N];
	   if (count < N)
	   {
		   buf[count] = bArray;
		   count++;
	   }
	   else
	   {
	
		   flag = 1;
		   count = 0;
	
	   }
	   sum = 0;
	   if (flag == 1)
	   {
		   for (i = 0; i < N; i++)
		   {
			   sum	= sum + buf[i];
	
		   }
		   result = sum / N;
	   }
	   else
	   {
	
		   for (i = 0; i < count; i++)
		   {
			   sum	= sum + buf[i];
	
		   }
		   result = sum / count;
	
	   }
	
	   return result;
}

float average_filter_pressure(float bArray)
{
    static float  sum = 0, result;
    static unsigned int count, flag=0, i;
    static float buf[N_2];
    if (count < N_2)
    {
        buf[count] = bArray;
        count++;
    }
    else
    {

        flag = 1;
        count = 0;

    }
    sum = 0;
    if (flag == 1)
    {
        for (i = 0; i < N_2; i++)
        {
            sum  = sum + buf[i];

        }
        result = sum / N_2;
    }
    else
    {

        for (i = 0; i < count; i++)
        {
            sum  = sum + buf[i];

        }
        result = sum / count;

    }

    return result;
}


