#include "filter.h"

/************************ 滑动窗口滤波器 *****************************/

float buffer[4][window_size] = {0}; //滑动窗口数据buf

/*********************** 滑动窗口滤波函数 ****************************/
float sliding_average_filter(unsigned char adc_num, float value)
{
    static int data_num = 0;
    float output[5] = {0};
    output[adc_num] = -1;
    if (data_num < window_size) //不满窗口，先填充
    {
        buffer[adc_num][data_num++] = value;
        output[adc_num] = value; //返回相同的值
        output[adc_num] = -1;
    }
    else
    {
        int i = 0;
        float sum[5] = {0};

        memcpy(&buffer[adc_num][0], &buffer[adc_num][1],
               (window_size - 1) * 4); //将1之后的数据移到0之后，即移除头部
        buffer[adc_num][window_size - 1] = value;                       //即添加尾部

        for (i = 0; i < window_size; i++) //每一次都计算，可以避免累计浮点计算误差
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
        result = sliding_average_filter(adc_num, result); //


    }

    return result;
}

float  GetMedianNum(float *bArray, int iFilterLen)
{
    int i, j; // 循环变量
    float bTemp;

    // 用冒泡法对数组进行排序
    for (j = 0; j < iFilterLen - 1; j ++)
    {
        for (i = 0; i < iFilterLen - j - 1; i ++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // 互换
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }

    // 计算中值
    if ((iFilterLen & 1) > 0)
    {
        // 数组有奇数个元素，返回中间一个元素
        bTemp = bArray[(iFilterLen + 1) / 2];
    }
    else
    {
        // 数组有偶数个元素，返回中间两个元素平均值
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
    }

    return bTemp;
}
static float  sum = 0, result;
unsigned int count;
float average_filter(float bArray)
{

    if (count <= N2)
    {
        sum  = sum + bArray;
        count++;
        result = sum / count ;
    }
    else
    {     
       result = sum / N2;
	   sum = result;
        count = 1;
  
    }

    return result;
}



