#include "adc.h"
#include "led.h"
#include "stm32f0xx_hal.h"
extern uint32_t adc_dma_value;

unsigned char AdcDetectionResult()
{     
       unsigned char adc_result;
	if(adc_dma_value < LIMIT_A)
		adc_result = 1;
	else if (adc_dma_value >= LIMIT_A  && adc_dma_value <= LIMIT_B)
		adc_result = 2;
	else if (adc_dma_value > LIMIT_B)
		adc_result = 3;
	else 
		adc_result = 0;
	return  adc_result;
}

void adc_process()
{
	switch(AdcDetectionResult())
	{
		case ON:
		{
			led_ctrl(ON);
		}break;
		case OFF:
		{
			led_ctrl(OFF);
		}break;
		case BLINKINF:
		{
			led_ctrl(BLINKINF);
		}break;
	}

}
