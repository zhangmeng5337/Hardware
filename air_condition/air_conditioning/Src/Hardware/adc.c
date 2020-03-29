
#include "adc.h"
#include "ntc.h"


extern ADC_HandleTypeDef hadc1;
float get_temperature(uint16_t data,unsigned char SensorTypes,float env_temper);

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
void Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_41CYCLES_5;        //采样时间
    //ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
//	return (unsigned int)HAL_ADC_GetValue(&hadc1);	        //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值

adc_io_str adc_io;

void Get_Adc_Average(unsigned char times)
{
	uint32_t temp_val=0,adcBuf[6];
	unsigned char i,t;
	for(t=0;t<6;t++)
	{;			
		adcBuf[t]=0;
	}

	for(i=0;i<times;i++)
	{                               //开启ADC
		//Get_Adc( ch);
		for(t=0;t<6;t++)
		{;
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 			
			adcBuf[t]=temp_val + adcBuf[t];
			if(temp_val>500000)
				adcBuf[t] = 0;
		}
	}
		HAL_ADC_Stop(&hadc1);
	for(t=0;t<6;t++)
	{		
		adcBuf[t]=adcBuf[t]/times;
	}
	for(t=0;t<5;t++)
	{		
		adcBuf[t]=adcBuf[t]+(uint32_t)(2.75*adcBuf[5]-4096);
	}


	for(t=1;t<5;t++)
	{
			adc_io.adc_result[t-1]=(float)(get_temperature(adcBuf[t],TEMPERATURE,adc_io.adc_result[1]));
		//else
			//adc_io.adc_result[t]=(float)(get_temperature(adcBuf[t],TEMPERATURE,adc_io.adc_result[1]));			
	}
	adc_io.adc_result[0]=(float)(get_temperature(adcBuf[t],HUMID,adc_io.adc_result[1]));

	#if DEBUG_USER
		printf("adcBuf[%d] is:   %d\n",i,adcBuf[i]);
		#endif
	
	//return &adc_io.adc_result[0];
} 
//查表函数
u8 look_up_table(u16 *a,u8 ArrayLong,u16 data)
{    
    u16 begin,end,middle ;  
    u8 i ;  
	

	    begin = 0 ;
		end = ArrayLong-1 ;  
		i = 0  ;  
		if(data >= a[begin]) return begin ;  
		else if(data <= a[end]) return end ;  
		while(begin < end)	
		{  
				middle = (begin+end)/2 ;  
				if(data == a[middle] ) break ;	
				if(data < a[middle] && data > a[middle+1]) break ;	 
				if(data > a[middle])  end = middle ;					  
				else begin = middle ;	   
				if(i++ > ArrayLong) break ;  
		}  
		if(begin > end ) return 0 ;   
		return middle ; 

 
}

//输入表的序号值，得到温度值
float num_to_temperature(u8 num,u8 types)
{
    float  data;
	  if(types == 0)
		{
    data = 1.0*num-40;		
		}
    else
		{
			data = 5.0*num;
		}
    return data;
}
float t1,tx;
#define TEMPERATURE 0x00
#define HUMID   0x01

float get_temperature(uint16_t data,unsigned char SensorTypes,float env_temper)
{
    unsigned int num,index;
	if(SensorTypes == TEMPERATURE)
	{
		if( (data<=NTCTAB[0]) && (data>NTCTAB[NTC_SIZE-1]) )
			{
				num=look_up_table((u16 *)NTCTAB,NTC_SIZE,data);
				t1=num_to_temperature(num,0);
				return tx=0.5*(data-NTCTAB[num])/(NTCTAB[num+1]-NTCTAB[num])+t1;	
			}

	}
	if(SensorTypes == HUMID)
	{
	    index = (unsigned int)env_temper/5;
		if( (data<=HLTAB[ index][0]) && (data>HLTAB[ index][HL_SIZE_COLUMB-1]) )
			{
				num=look_up_table((u16 *)(HLTAB[index]),HL_SIZE_COLUMB,data);
				t1=num_to_temperature(num,1);
				return tx=0.5*(data-HLTAB[ index][num])/(HLTAB[ index][num+1]-HLTAB[ index][num])+t1;	
			}

	}

		
}

unsigned int adc_process()
{

	Get_Adc_Average(100);

	if(adc_io.adc_result[0]>=MAX_TEMPER ||adc_io.adc_result[0]<=MIN_TEMPER)
	{
		adc_io.fault_status = adc_io.fault_status|0xe01;
	}
	else if(adc_io.adc_result[1]>=MAX_TEMPER ||adc_io.adc_result[1]<=MIN_TEMPER)
	{
		adc_io.fault_status = adc_io.fault_status|0xe02;
	}
	else if(adc_io.adc_result[2]>=MAX_TEMPER ||adc_io.adc_result[2]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe04;
	}
	else if(adc_io.adc_result[3]>=MAX_TEMPER ||adc_io.adc_result[3]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe08;
	}
	else if(adc_io.adc_result[4]>=MAX_TEMPER ||adc_io.adc_result[4]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe10;
	}
	else 
		adc_io.fault_status = 0;

	return adc_io.fault_status;

}
