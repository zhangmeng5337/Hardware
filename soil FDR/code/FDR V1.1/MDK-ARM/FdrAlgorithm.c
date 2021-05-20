#include "FdrAlgorithm.h"
#include "EEPROM.h"
#include "adc.h"
#include "math.h"
#include "filter.h"
factor_stru factor_usr;


factor_stru *getFactor()
{
    return &factor_usr;
}
  static float value2_buf[SAMPLE_COUNT];
float GlideFilterAD(float val,unsigned char val2)
{
    static unsigned char FilterI;
 
    //unsigned char	count;
    static unsigned char flag;
    float	sum=0;
    value2_buf[FilterI++]=val;
    if(FilterI==SAMPLE_COUNT)
    {
        flag = 1;
        FilterI=0; //先进先出，再求平均值
    }
    if(flag == 1)
    {
			  average_filterFloat(value2_buf,SAMPLE_COUNT);
			
        for(int count=0; count<SAMPLE_COUNT; count++)
            sum+=value2_buf[count];
        return (sum/SAMPLE_COUNT);
    }
    else
    {
			  average_filterFloat(value2_buf,FilterI);
        for(int count=0; count<FilterI; count++)
            sum+=value2_buf[count];
        return (sum/FilterI);
    }


}

float GlideFilterAD2(float val,unsigned char val2)
{
    static unsigned char FilterI;
    static float value2_buf[SAMPLE_COUNT];
    //unsigned char	count;
    static unsigned char flag;
    float	sum=0,temp;
	
	
	
	  unsigned char count,i,j;  
  	value2_buf[FilterI++]=val;
    if(FilterI==SAMPLE_COUNT)
    {
        flag = 1;
        FilterI=0; //先进先出，再求平均值
    }
		if(flag == 1)
		{
			for (j=0;j<SAMPLE_COUNT-1;j++)  //中值滤波+均值滤波

			 {  

					for (i=0;i<SAMPLE_COUNT-j;i++)  

					{  

						 if ( value2_buf[i]>value2_buf[i+1] )  

						 {  

								temp = value2_buf[i];  

								value2_buf[i] = value2_buf[i+1];   

								 value2_buf[i+1] = temp;  

						 }  

					}  

			 }  

			 for(count=1;count<SAMPLE_COUNT-1;count++)  

					sum += value2_buf[count];  

			 return (sum/(SAMPLE_COUNT-2)); 			
		}
    else
		{
        for(int count=0; count<FilterI; count++)
            sum+=value2_buf[count];
        return (sum/FilterI);
		}
	
	
	
	
	
	
	
	
	
	
	
//    value2_buf[FilterI++]=val;
//    if(FilterI==SAMPLE_COUNT)
//    {
//        flag = 1;
//        FilterI=0; //先进先出，再求平均值
//    }
//    if(flag == 1)
//    {
//        for(int count=0; count<SAMPLE_COUNT; count++)
//            sum+=value2_buf[count];
//        return (sum/SAMPLE_COUNT);
//    }
//    else
//    {
//        for(int count=0; count<FilterI; count++)
//            sum+=value2_buf[count];
//        return (sum/FilterI);
//    }


}
unsigned int result;
float SoilHumid(unsigned char status,float AdcValueVol)
{
    static unsigned char index;

    float tmp3,v3,v2,v1;
    static float last_temp,kk;
    float a0,a1,a2,a3;
    a0 = 1110;
    a1 = -2610;
    a2 = 2091.9;
    a3 = -519.5;
   AdcValueVol =DigitRound(AdcValueVol,3);
    v3 = AdcValueVol*AdcValueVol;
    v3 = v3*AdcValueVol;
    v2 = AdcValueVol*AdcValueVol;
    v1= AdcValueVol;

    factor_usr.humid=a0*v3;//校准前湿度输出
    factor_usr.humid=factor_usr.humid+a1*v2;
    factor_usr.humid=factor_usr.humid+a2*v1;
    factor_usr.humid=factor_usr.humid+a3;

    /*if(factor_usr.humid>=80)
    {
        a0 = 564.39;
        a1 = 22.747;
        factor_usr.humid=a0*log(exp(AdcValueVol));//校准前湿度输出
        factor_usr.humid=factor_usr.humid+a1;
    }*/
    factor_usr.humid=factor_usr.humid/100;
    if(factor_usr.status == CALIBRATION)//校准后湿度输出
    {

        float humid_cal;

        v2 = factor_usr.humid*factor_usr.humid;
        v1= factor_usr.humid;

        humid_cal=factor_usr.a0;
        humid_cal=humid_cal+factor_usr.a1*v1;
        humid_cal=humid_cal+factor_usr.a2*v2;
       // if(fabs(humid_cal/factor_usr.humid)>=2||fabs(factor_usr.humid/humid_cal)>=2)
       //     factor_usr.humid = factor_usr.humid;
       // else
            factor_usr.humid = humid_cal;
    }


    /* if(AdcValueVol<0.63)
     {
         factor_usr.humid = 0;
         last_temp = 0;
     }*/

    last_temp = last_temp +factor_usr.humid;
    index++;
    tmp3= factor_usr.humid;//GlideFilterAD2(factor_usr.humid,index);
    if(index>=SAMPLE_COUNT)
    {
        index =0;
    }
    /************滤波去抖动，防止数据过于频繁跳动****************/


    result = (unsigned int)(tmp3*1000);
    factor_usr.humid = result;
		#if DEBUG == 0
		{
			if(factor_usr.humid>=1000)
					factor_usr.humid = 1000;
			if(factor_usr.humid<0)
					factor_usr.humid = 0;
			if(AdcValueVol/V_Nom<=1.03&&AdcValueVol/V_Nom>=0.97)
			
			{
					factor_usr.humid = 0;
					last_temp = 0;
			}		
		}
    #endif
    return (unsigned int) (factor_usr.humid);
}



/*
	Rt=R0（1+A*t+B*t^2）0-850   A=0.00394 B -5.802*10-7 c=-4.274*10-12
	Rt=R0[1+A*t+Bt^2+C(t-100)^3] -200-0
*/
float SoilTemperature(unsigned char status,float AdcValueVol1,float AdcValueVol2)
{
    float resistor;
    float A = 0.0012;
    float B = 2.3162;
    float C = -243.61;
    float tmp1,tmp2,tmp3;

    static unsigned int index3,index2;
    unsigned int result;
	  static unsigned int result_last;
    //	unsigned char i;
    //AdcValueVol1 = ((uint32_t)(AdcValueVol1*1000))/1000.0;
    //	AdcValueVol2 = ((uint32_t)(AdcValueVol2*1000))/1000.0;
	  //AdcValueVol1 = DigitRound(AdcValueVol1,3);
	  //AdcValueVol2 = DigitRound(AdcValueVol2,3);	
    resistor = (AdcValueVol1*499/AdcValueVol2)-499;
    resistor = ((uint32_t)(resistor*10000));
    resistor=resistor/10000.0;



    tmp1 = A*resistor*resistor;
    tmp2 = B*resistor;
    tmp3 = tmp1 + tmp2;
    tmp3 = tmp3 + C;
    tmp3 = tmp3*10;
    tmp1 = tmp3;
    //	last_temp[index2] = tmp3;
    index2++;
    index3++;
    /************滤波去抖动，防止数据过于频繁跳动****************/
    tmp3=GlideFilterAD(tmp3,index3);
    if(index3>=SAMPLE_COUNT)
    {
        index3 =0;
    }
   // tmp3 = tmp3 -30;

    if(tmp3<0)
    {
        result = (unsigned int)(tmp3);
        result = ~result+1;
    }
    else
        result = (unsigned int)(tmp3);
  
return result;		

    
}

/************************************************************
整数转浮点数
*************************************************************/
float DataMinusProc(uint32_t *p,unsigned char len,unsigned int times)
{
    uint32_t tmp,tmp3;
    float tmp2;
    tmp = 0;

    tmp3= (unsigned char)p[0];
    tmp3= tmp3<<24;
    tmp = tmp + tmp3;

    tmp3=(unsigned char) p[1];
    tmp3= tmp3<<16;
    tmp = tmp + tmp3;

    tmp3= (unsigned char)p[2];
    tmp3= tmp3<<8;
    tmp = tmp + tmp3;

    tmp3=(unsigned char) p[3];
    tmp = tmp + tmp3;

    if(tmp&0x80000000)
    {
        tmp = ~tmp+1;
        tmp2 = tmp*(-1.0);
        tmp2 = tmp2/1.0/times;
    }
    else
    {
        tmp2 = tmp;
        tmp2 = tmp2/1.0/times;
    }
    return tmp2;
}
float DataMinusProc2(unsigned char *p,unsigned char len,unsigned int times)
{
    uint32_t tmp,tmp3;
    unsigned char i;
    float tmp2;
    tmp = 0;

    tmp3= p[0];
    tmp3= tmp3<<24;
    tmp = tmp + tmp3;

    tmp3= p[1];
    tmp3= tmp3<<16;
    tmp = tmp + tmp3;

    tmp3= p[2];
    tmp3= tmp3<<8;
    tmp = tmp + tmp3;

    tmp3= p[3];
    tmp = tmp + tmp3;


    if(tmp&0x80000000)
    {
        tmp = ~tmp+1;
        tmp2 = tmp*(-1.0);
        tmp2 = tmp2/1.0/times;
    }
    else
    {
        tmp2 = tmp;
        tmp2 = tmp2/1.0/times;
    }
    return tmp2;
}

/**********************************************
            浮点转整型
**********************************************/
uint32_t FloatToCharProc(float p,unsigned int times)
{
    uint32_t tmp;
    // unsigned char i;
    float tmp2;
    if(p<0)
    {
        tmp2 = times*p*(-1);
        tmp =(uint32_t)tmp2;
        tmp = ~tmp +1;
    }
    else
    {
        tmp2 = p*times;
        tmp =(uint32_t)tmp2;
    }
    return tmp;
}
float DigitRound(float p,unsigned char digit)
{
    uint32_t tmp,tmp3;
    float tmp2;
    switch(digit)
    {
    case 0:
        tmp3 = 10;
        break;
    case 1:
        tmp3 = 100;
        break;
    case 2:
        tmp3 = 1000;
        break;
    case 3:
        tmp3 = 10000;
        break;
    case 4:
        tmp3 = 10000;
        break;
    }
    tmp = (uint32_t)(p*tmp3);
    if(tmp%10>=5)
        tmp = tmp+10;
    tmp2 = tmp/1.0/tmp3;
    tmp = (uint32_t)(tmp2*(tmp3/10));
    tmp2 = tmp/(tmp3/10.0);
    return tmp2;
}
