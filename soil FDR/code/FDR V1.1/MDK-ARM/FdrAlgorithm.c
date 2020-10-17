#include "FdrAlgorithm.h"
#include "EEPROM.h"
#include "adc.h"
factor_stru factor_usr;

float GlideFilterAD(float val,unsigned char val2)
{
	static unsigned char FilterI;
  static float value2_buf[SAMPLE_COUNT];	
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
	for(int count=0;count<SAMPLE_COUNT;count++)
		sum+=value2_buf[count];	
		return (sum/SAMPLE_COUNT);
	}
	else
	{
	for(int count=0;count<FilterI;count++)
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
	float	sum=0;
	value2_buf[FilterI++]=val;
	if(FilterI==SAMPLE_COUNT) 
	{
	  flag = 1;
		FilterI=0; //先进先出，再求平均值
	}	
	if(flag == 1)
	{
	for(int count=0;count<SAMPLE_COUNT;count++)
		sum+=value2_buf[count];	
		return (sum/SAMPLE_COUNT);
	}
	else
	{
	for(int count=0;count<FilterI;count++)
		sum+=value2_buf[count];
    return (sum/FilterI);
	}


}
unsigned int result;
float SoilHumid(unsigned char status,float AdcValueVol)
{
	static unsigned char index;
	
	float tmp3;
	static float last_temp;
	if(status == CALIBRATION)
	{
		factor_usr.humid=factor_usr.a0*AdcValueVol*AdcValueVol+factor_usr.a2;
		factor_usr.humid=factor_usr.humid+factor_usr.a1*AdcValueVol;	
	}
	else
	{
		factor_usr.humid=factor_usr.a0*AdcValueVol*AdcValueVol+factor_usr.a2;
		factor_usr.humid=factor_usr.humid+factor_usr.a1*AdcValueVol;	
	}
	if(AdcValueVol<1.515)
	{
		factor_usr.humid = 0;
	   last_temp = 0;
  }

	last_temp = last_temp +factor_usr.humid;
	index++;
	tmp3=GlideFilterAD2(factor_usr.humid,index);
		if(index>=SAMPLE_COUNT)
	{
     index =0;
	}
	/************滤波去抖动，防止数据过于频繁跳动****************/
//	if(index<SAMPLE_COUNT)
//	{
//		tmp3 = last_temp/index;
//	}
//	else
//	{
//		tmp3 = last_temp/index;
//    index = 0;	
//		last_temp = 0;		
//	}
   
	if(tmp3<0)
	{
		result = (unsigned int)(tmp3*10);
		//result = ~result+1;
	}
	else 
		result = (unsigned int)(tmp3*10);
	factor_usr.humid = result;
	if(factor_usr.humid>=1000)
		factor_usr.humid = 1000;
		if(AdcValueVol<=0.3)
	{
		factor_usr.humid = 0;
	   last_temp = 0;
  }
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

	unsigned char index,index2;
	unsigned int result;
	//	unsigned char i;
	//AdcValueVol1 = ((uint32_t)(AdcValueVol1*1000))/1000.0;
	//	AdcValueVol2 = ((uint32_t)(AdcValueVol2*1000))/1000.0;
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
	index++;
	/************滤波去抖动，防止数据过于频繁跳动****************/
	tmp3=GlideFilterAD(tmp3,index);
	if(index>=SAMPLE_COUNT)
	{
		index =0;
	}
	tmp3 = tmp3 -30;

	if(tmp3<0)
	{
		result = (unsigned int)(tmp3);
		result = ~result+1;
	}
	else 
		result = (unsigned int)(tmp3);
	return result;
}


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
		case 0:tmp3 = 10;break;
		case 1:tmp3 = 100;break;		
		case 2:tmp3 = 1000;break;		
		case 3:tmp3 = 10000;break;
		case 4:tmp3 = 10000;break;		
	}
	tmp = (uint32_t)(p*tmp3); 
	if(tmp%10>=5)
		tmp = tmp+10;
	tmp2 = tmp/1.0/tmp3;
	tmp = (uint32_t)(tmp2*(tmp3/10)); 
	tmp2 = tmp/(tmp3/10.0);
	return tmp2;
}