#include "filter.h"
#include "calman.h"
extern  float adcBuf_ref[N];
extern	float adcBuf_humid[N];
extern	float adcBuf_ta[N];
extern	float adcBuf_tb[N];
extern float adcBuf_cali[N];//温度b电压缓冲区



void seq_fileter(unsigned int *pb,unsigned int sampleCount)
{
    unsigned char i,j;
    unsigned int temp,tmp1,tmp2;

    for (j=0; j<sampleCount-1; j++)

    {

        for (i=0; i<sampleCount-j-1; i++)

        {
					  tmp1 = pb[i];
					  tmp2 = pb[i+1];
            if ( tmp1>tmp2 )
            {
                 
                temp = pb[i];

                pb[i] = pb[i+1];

                pb[i+1] = temp;

            }

        }

    }



}
float average_filter(float *pb,unsigned int sampleCount)  //中值滤波+均值滤波

{
    float value_buf[100];
	 unsigned int countS;
    unsigned char count,i,j;
    float  sum=0,temp;

    //value_buf = pb;
    unsigned char ik;
    for (ik = 0; ik<100; ik++)
        value_buf[ik]=0;
    for (ik = 0; ik<sampleCount; ik++)
        value_buf[ik]=pb[ik];
    //memmove(value_buf,pb,48);
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

//   for(count=1;count<sampleCount-1;count++)

//      sum += value_buf[count];

//   return (sum/(sampleCount-2));
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
			countS = sampleCount-4;
			sum =sum/countS;
        return (sum);
    }
}
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

/****************************************
均值滤波
*****************************************/
float filter(uint32_t seq)
{
    unsigned char count,j,i;
    float sum;
    uint32_t  temp;
    switch(seq)
    {
    case 0:
        // value_buf=adcBuf_ref;
   		Claman(adcBuf_ref,0.01,10,0);
        sum = average_filter(adcBuf_ref,N);
        break;
    case 1:
        //value_buf=adcBuf_humid;
        Claman(adcBuf_humid,0.004,10,1);
        sum = average_filter(adcBuf_humid,N);
        break;
    case 2:
        // value_buf=adcBuf_ta;
        Claman(adcBuf_ta,0.008,10,2);
        sum = average_filter(adcBuf_ta,N);
        break;
    case 3:
        //value_buf=adcBuf_tb;
        Claman(adcBuf_tb,0.008,10,3);
        sum = average_filter(adcBuf_tb,N);
        break;
    case 4:
        //value_buf=adcBuf_tb;
       // Claman(adcBuf_cali,0.008,10,4);
        sum = average_filter(adcBuf_cali,N);
        break;

    }

//    for(count=0; count<N; count++)
//        sum += value_buf[count];
    return (sum);


}

