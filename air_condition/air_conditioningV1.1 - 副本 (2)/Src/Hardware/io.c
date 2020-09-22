#include "io.h"
#include "adc.h"

void device_ctrl(unsigned char devNo,unsigned char on_off)
{

	if(on_off == ON)
	{
		switch(devNo)
		{
			case FAN_HOT:  HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_SET);break;
			case FAN_COOL: HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_SET);break;
			case PUMP:     HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_SET);break;
			case HUMID:    HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin,GPIO_PIN_RESET);break;
			case GATE :    HAL_GPIO_WritePin(CTR_OUT4_GPIO_Port, CTR_OUT4_Pin,GPIO_PIN_SET);break;

      case DEV_ALL:
			HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CTR_OUT4_GPIO_Port, CTR_OUT4_Pin,GPIO_PIN_SET);break;
			

		}
	}
	else
	{
		switch(devNo)
		{
			case FAN_HOT:HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_RESET);break;
			case FAN_COOL:HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_RESET);break;
			case PUMP:HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_RESET);break;
			case HUMID:HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin,GPIO_PIN_SET);break;
			case GATE: HAL_GPIO_WritePin(CTR_OUT4_GPIO_Port, CTR_OUT4_Pin,GPIO_PIN_RESET);break;

            case DEV_ALL:
			HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PWM_GPIO_Port, PWM_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(CTR_OUT4_GPIO_Port, CTR_OUT4_Pin,GPIO_PIN_RESET);
			break;	
		}
	}
		#if DEBUG_USER
		printf("device_ctrl is:   %d  %d\n",devNo,on_off);
		#endif
}
extern adc_io_str adc_io;
void get_io()
{
     
	if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 1)
	{	 
		adc_io.io_status = adc_io.io_status|0x01;
		adc_io.fault_status = adc_io.fault_status|0xe20;
	}
	else
	{	    
		adc_io.fault_status = adc_io.fault_status&(~0x020);
		adc_io.io_status = adc_io.io_status&0xfe;
	}

	if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 1)
	{	 
		adc_io.io_status =adc_io.io_status|0x02;
		adc_io.fault_status = adc_io.fault_status|0xe40;
	}
	else
	{	   
		adc_io.io_status =  adc_io.io_status&0xed0;
		adc_io.fault_status = adc_io.fault_status&(~0x0d0);
	}
}


