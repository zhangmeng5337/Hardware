#include "filter.h"
float average_filterFloat(float *pb,unsigned int sampleCount)  //中值滤波+均值滤波

{
    float value_buf[100];
    unsigned char count,i,j;
    uint32_t  sum=0,temp;
    unsigned char ik;
    for (ik = 0; ik<100; ik++)
        value_buf[ik]=0;
    for (ik = 0; ik<sampleCount; ik++)
        value_buf[ik]=pb[ik];
    for (j=0; j<sampleCount-1; j++)

    {

        for (i=0; i<sampleCount-j; i++)

        {

            if ( value_buf[i]>value_buf[i+1] )

            {

                temp = value_buf[i];

                value_buf[i] = value_buf[i+1];

                value_buf[i+1] = temp;

            }

        }

    }


    if(sampleCount<=4)
    {
        for(count=0; count<sampleCount; count++)

            sum += value_buf[count];
        return (0);
    }
    else  if(sampleCount>2)
    {
        for(count=2; count<sampleCount-2; count++)

            sum += value_buf[count];
        return (sum/(sampleCount-4));
    }


}

