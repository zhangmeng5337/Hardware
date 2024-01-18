
#include "adc.h"
#include "ntc.h"


extern ADC_HandleTypeDef hadc1;
float get_sensor_restult(uint16_t data,unsigned char SensorTypes,float env_temper);

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
void Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_41CYCLES_5;        //����ʱ��
    //ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&hadc1);                               //����ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //��ѯת��
 
//	return (unsigned int)HAL_ADC_GetValue(&hadc1);	        //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ

adc_io_str adc_io;

void Get_Adc_Average(unsigned char times)
{
    #define adc_count 7
	uint32_t temp_val=0,adcBuf[adc_count];
	unsigned char i,t;

	for(t=0;t<adc_count;t++)
	{;			
		adcBuf[t]=0;
	}

	for(i=0;i<times;i++)
	{                               //����ADC
		//Get_Adc( ch);
		for(t=0;t<adc_count;t++)
		{;
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 			
			adcBuf[t]=temp_val + adcBuf[t];
			if(temp_val>500000)
				adcBuf[t] = 0;
		}
	}
	//	HAL_ADC_Stop(&hadc1);
	for(t=0;t<adc_count;t++)
	{		
		adcBuf[t]=adcBuf[t]/times;
	}

	for(t=0;t<(adc_count-1);t++)//�����ڲ��ο���ѹ����
	{		
		adcBuf[t]=adcBuf[t]+(uint32_t)(2.75*adcBuf[adc_count-1]-4095);
	}


	for(t=1;t<(adc_count-1);t++)
	{
			adc_io.adc_result[t-1]=(float)(get_sensor_restult(adcBuf[t],TEMPERATURE,adc_io.adc_result[1]));
		//else
			//adc_io.adc_result[t]=(float)(get_temperature(adcBuf[t],TEMPERATURE,adc_io.adc_result[1]));			
	}
	adc_io.adc_result[0]=(float)(get_sensor_restult(adcBuf[0],HUMIDTYPE,adc_io.adc_result[1]));

	#if DEBUG_USER
		printf("adcBuf[%d] is:   %d\n",i,adcBuf[i]);
		#endif
	
	//return &adc_io.adc_result[0];
} 
//�����
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

//���������ֵ���õ��¶�ֵ
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


float get_sensor_restult(uint16_t data,unsigned char SensorTypes,float env_temper)
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
	if(SensorTypes == HUMIDTYPE)
	{
	    index = (unsigned int)(env_temper+5)/5;
		if( (data<=HLTAB[ index][0]) && (data>HLTAB[ index][HL_SIZE_COLUMB-1]) )
			{
				num=look_up_table((u16 *)(HLTAB[index]),HL_SIZE_COLUMB,data);
				t1=num_to_temperature(num,1);
				return tx=0.5*(data-HLTAB[ index][num])/(HLTAB[ index][num+1]-HLTAB[ index][num])+t1;	
			}
			else
			{
				t1=num_to_temperature(num,1);
				return tx=0.5*(data-HLTAB[ index][0])/(HLTAB[ index][1]-HLTAB[ index][0])+t1;	
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
