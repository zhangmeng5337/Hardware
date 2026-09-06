#include "string.h"
#include "filter.h"

/************************ »¬¶¯´°¿ÚÂË²¨Æ÷ *****************************/

float buffer[6][window_size] = {0}; //»¬¶¯´°¿ÚÊı¾İbuf

/*********************** »¬¶¯´°¿ÚÂË²¨º¯Êı ****************************/
float sliding_average_filter(unsigned char adc_num, float value)
{
    static int data_num = 0;
    float output[8] = {0};
    output[adc_num] = -1;
    if (data_num < window_size) //²»Âú´°¿Ú£¬ÏÈÌî³ä
    {
        buffer[adc_num][data_num++] = value;
        output[adc_num] = value; //·µ»ØÏàÍ¬µÄÖµ
        output[adc_num] = -1;
    }
    else
    {
        int i = 0;
        float sum[8] = {0};

        memcpy(&buffer[adc_num][0], &buffer[adc_num][1],
               (window_size - 1) * 4); //½«1Ö®ºóµÄÊı¾İÒÆµ½0Ö®ºó£¬¼´ÒÆ³ıÍ·²¿
        buffer[adc_num][window_size - 1] = value;                       //¼´Ìí¼ÓÎ²²¿

        for (i = 0; i < window_size; i++) //Ã¿Ò»´Î¶¼¼ÆËã£¬¿ÉÒÔ±ÜÃâÀÛ¼Æ¸¡µã¼ÆËãÎó²î
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
    int i, j; // Ñ­»·±äÁ¿
    float bTemp;

    // ÓÃÃ°Åİ·¨¶ÔÊı×é½øĞĞÅÅĞò
    for (j = 0; j < iFilterLen - 1; j ++)
    {
        for (i = 0; i < iFilterLen - j - 1; i ++)
        {
            if (bArray[i] > bArray[i + 1])
            {
                // »¥»»
                bTemp = bArray[i];
                bArray[i] = bArray[i + 1];
                bArray[i + 1] = bTemp;
            }
        }
    }

    // ¼ÆËãÖĞÖµ
    if ((iFilterLen & 1) > 0)
    {
        // Êı×éÓĞÆæÊı¸öÔªËØ£¬·µ»ØÖĞ¼äÒ»¸öÔªËØ
        bTemp = bArray[(iFilterLen + 1) / 2];
    }
    else
    {
        // Êı×éÓĞÅ¼Êı¸öÔªËØ£¬·µ»ØÖĞ¼äÁ½¸öÔªËØÆ½¾ùÖµ
        bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
    }

    return bTemp;
}

float average_filter(float bArray, unsigned char num)
{
    static float  sum2 = 0;

    static float  sum = 0, result;
    static unsigned int count, flag = 0, i;
    static unsigned int count2, flag2 = 0;

    static float buf[N];
	static float buf2[N];
    if (num == 0)
    {
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
                sum  = sum + buf[i];

            }
            result = sum / N;
        }
        else
        {

            for (i = 0; i < count; i++)
            {
                sum  = sum + buf[i];

            }
            result = sum / count;

        }

    }
	else
	{
	if (count2 < N)
		   {
			   buf2[count2] = bArray;
			   count2++;
		   }
		   else
		   {
	
			   flag2 = 1;
			   count2 = 0;
	
		   }
		   sum2 = 0;
		   if (flag2 == 1)
		   {
			   for (i = 0; i < N; i++)
			   {
				   sum2	= sum2 + buf2[i];
	
			   }
			   result = sum2 / N;
		   }
		   else
		   {
	
			   for (i = 0; i < count2; i++)
			   {
				   sum2	= sum2 + buf2[i];
	
			   }
			   result = sum2 / count2;
	
		   }

	}


    return result;
}

float average_filter_pressure(float bArray)
{
    static float  sum = 0, result;
    static unsigned int count, flag = 0, i;
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

/**
*  ä¸­ä½å€¼å¹³å‡æ»¤æ³¢
*  pData:æ²¡æœ‰æ»¤æ³¢çš„æ•°æ® 
*  nSize:æ•°æ®å¤§å° 
* averageï¼šæ»¤æ³¢æ•°æ®çš„å¹³å‡å€¼ ï¼ˆé€šè¿‡å½¢å‚å†™å…¥ï¼Œéœ€æå‰å£°æ˜ä¸€ä¸ªå˜é‡ï¼Œå­˜å‚¨averageï¼‰
*/
int MedianFilter(float* pData,int nSize,float *average)
{
    int max,min;
    float sum=0;
    if(nSize>2)
    {
        max = pData[0];
        min = max;
        for(int i=0;i<nSize;i++)
        {
            sum += pData[i];            
            if(pData[i]>max)
            {
                max = pData[i];   //ä¸€ä¸ªå¾ªç¯ä¹‹åmaxå°±æ˜¯æœ€å¤§çš„å€¼
            }

            if(pData[i]<min)
            {
                min = pData[i];   //ä¸€ä¸ªå¾ªç¯ä¹‹åminå°±æ˜¯æœ€å°çš„å€¼
            }
        }
    	sum = sum-max-min;       //å»æ‰æœ€å¤§çš„å€¼å’Œæœ€å°çš„å€¼
    	(*average)=sum/(nSize-2); //å¯¹N-2ä¸ªæ•°æ±‚å¹³å‡å€¼,ä¿ç•™ä¸¤ä½å°æ•°  
    	return 1;        
    }
    return 0;
}

