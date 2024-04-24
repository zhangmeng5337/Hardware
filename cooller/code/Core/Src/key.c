#include "key.h"
key_stru key_usr;

void key_hit_proc(unsigned char key_num)
{
  switch(key_num)
  {
	case OK_NUM:
		key_usr.key_hit_status = key_usr.key_hit_status|0x01;
		break;

  case SET_NUM:
	  key_usr.key_hit_status = key_usr.key_hit_status|0x02;
	  break;

	case INC_NUM:
		key_usr.key_hit_status = key_usr.key_hit_status|0x04;
		break;
	case DEC_NUM:
		key_usr.key_hit_status = key_usr.key_hit_status|0x10;
		break;
	default:
		;
		break;

  }
	key_usr.key_hit_update = 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == ok_Pin)
    {
		if(HAL_GPIO_ReadPin(ok_GPIO_Port,ok_Pin) == 0)
		{
			key_hit_proc(OK_NUM);
		}
	}
    if(GPIO_Pin == set_Pin)
    {
		if(HAL_GPIO_ReadPin(ok_GPIO_Port,set_Pin) == 0)
		{
			key_hit_proc(SET_NUM);
		}
	}
    if(GPIO_Pin == dec_Pin)
    {
		if(HAL_GPIO_ReadPin(dec_GPIO_Port,dec_Pin) == 0)
		{
			key_hit_proc(DEC_NUM);
		}
	}	
    if(GPIO_Pin == inc_Pin)
    {
		if(HAL_GPIO_ReadPin(inc_GPIO_Port,inc_Pin) == 0)
		{
			key_hit_proc(INC_NUM);
		}
	}	
}

void key_proc()
{


}
