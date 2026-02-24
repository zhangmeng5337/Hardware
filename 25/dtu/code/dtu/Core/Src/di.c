#include "di.h"
static stru_di_stru di_usr;
void HC165_init(void)
{
		
  	  HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(CP_GPIO_Port,CP_Pin,GPIO_PIN_SET);
}


uint32_t HC165_READ(void)
{
		uint32_t temp_165 = 0;

	
	  HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_RESET);
	  //osDelay(10);
	  HAL_GPIO_WritePin(PL_GPIO_Port,PL_Pin,GPIO_PIN_SET);
	
	  if(HAL_GPIO_ReadPin(Q7_GPIO_Port,Q7_Pin))
		{
			temp_165 |= 0x01;
		}
		
		for(uint8_t i = 0; i < 32 ; i++)// ¶ÁÈ¡£¨3*8 - 1 £© = 24 ´Î
		{
			 temp_165 = temp_165 << 1;
			 HAL_GPIO_WritePin(CP_GPIO_Port,CP_Pin,GPIO_PIN_RESET);
			// osDelay(10);
			 HAL_GPIO_WritePin(CP_GPIO_Port,CP_Pin,GPIO_PIN_SET);
			// osDelay(10);
			if(HAL_GPIO_ReadPin(Q7_GPIO_Port,Q7_Pin))
			{
				temp_165 |= 0x01;
			}
			//osDelay(10);
		}
		return temp_165;
}


uint32_t di_input_read()
{   
	di_usr.di_status = HC165_READ();
    return di_usr.di_status;
}
void di_proc()
{
	di_input_read( );

}



