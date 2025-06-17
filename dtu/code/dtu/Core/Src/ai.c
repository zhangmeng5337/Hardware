#include "ai.h"
#include "sys.h"
#include "kalman.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
stru_adc_stru adc_usr;

kalman kalman_table[] = 
{
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05},
	{50,0,0,0,0.001,0.05}
};

void adc_init(void)
{
	HAL_ADC_Start_DMA(&hadc1, adc_usr.adc, AI_SIZE);
	HAL_ADC_Start(&hadc1);
}

void adc_proc(void)
{
	registerTick(ADC_POLL_TICK,ADC_POLL_TIME);
	if(GetTickResult(ADC_POLL_TICK) == 1)
	{
	    unsigned char i;
		float buf[AI_SIZE];
		for(i = 0;i<AI_SIZE;i++)
		{
			buf[i] = adc_usr.adc[i]*ADC_VREF/4096;
			buf[i] = buf[i] /VOL_RATIO;
			adc_usr.ai_buf[i] = kalman_filter(&kalman_table[i], buf[i]);
		}
	    
		reset_registerTick(ADC_POLL_TICK);
	    registerTick(ADC_POLL_TICK,ADC_POLL_TIME);
		
	}
}
void ai_press_conv_proc()
{
	adc_proc();


}
