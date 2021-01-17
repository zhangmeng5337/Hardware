#include "app.h"
#include "adc.h"
extern TIM_HandleTypeDef htim2;
Params_stru Params;
Params_stru *getParams()
{
	return &Params;
}
void Params_Init(void)
{
	Params.display_flag = 0;
	Params.mode = 0;
	Params.KeyStatus = 0;
	Params.fm_flag = 0;
	Params.gear_period = HAL_GetTick();
	Params.temperature_period = HAL_GetTick();

}

void SCR_ON(void)
{
	HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_SET);

}
void SCR_OFF(void)
{
	HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_RESET);

}


void FM_ON(void)	
{
	HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_SET);
}
void FM_OFF(void)	
{
	HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_RESET);
}
void key_process(void)
{
	switch(getParams()->KeyStatus != 0)
		{
		case 0:
			break;
		case 1:
		{
			if(getParams()->mode == 0x01)
			{
				getParams()->mode = 0x00;//关闭冷水模式热水模式
			}
			else
				getParams()->mode = 0x01;//修改模式为冷水模式
		}
		break;
		case 2:
		{
			if(getParams()->mode == 0x02)
			{
				getParams()->mode = 0x00;
			}
			else
				getParams()->mode = 0x02;//修改模式为热水模式
		}
		break;
		default :
			getParams()->mode = 0x00;//关闭冷水模式热水模式
			break;
	
		}
		getParams()->KeyStatus = 0;

}
void device_ctrl_process(void)
{
    static uint32_t tick;
    unsigned char flag;
	if(adc_process() == 1)
	{
		oled_process(1);////温度显示
	}


	 if(getParams()->mode != 0)//运行模式
		{
			if(getParams()->temperature >= 50)//温度过高报警
			{
				if(getParams()->display_flag&0x20 == 0)
				{
					oled_process(6);//显示报警图标
					getParams()->display_flag = 0x20;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
				}
	//******************************蜂鸣器控制******************************************
	
				if(getParams()->fm_flag==0) 	//当前状态是关闭状态且倒计时为0
				{
					if(flag == 0)
					{
						getParams()->fm_period=(getParams()->temperature-50)*1000/50;		//将温度数值*10赋值给蜂鸣器鸣叫倒计时 最大1000
						tick = HAL_GetTick();
						flag = 1;
						FM_ON();
					}
	
				}
				else
				{
	
					if(flag == 0)
					{
						getParams()->fm_period=1000-(getParams()->temperature-50)*1000/50;
						tick = HAL_GetTick();
						flag = 1;
					}
	
				}
				if((HAL_GetTick()-tick)>=getParams()->fm_period)
				{
					tick = HAL_GetTick();
					flag = 0;
					if(getParams()->fm_flag == 1)
					{
	
						getParams()->fm_flag = 0;
						FM_OFF();
					}
					else
					{
						getParams()->fm_flag = 1; //置位蜂鸣器标志位
					}
				}
	
			}
			else
			{
				if(flag == 1||getParams()->fm_flag == 1)
				{
					flag = 0;
					FM_OFF();
				}
	
				if(getParams()->display_flag&0x40 == 0)
				{
					oled_process(7);//冷水热模式显示运行图标
					getParams()->display_flag = 0x40;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
				}
	
			}
	
		}
		else //停止模式
		{
			if(getParams()->display_flag&0x80 == 0)
			{
				//getParams()->display_flag = 0x80;
				oled_process(8);//关闭模式显示停止图标
				getParams()->display_flag = 0x80;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
			}
	
		}
		switch(getParams()->mode)
		{
		case 0:
		{
			if(getParams()->display_flag&0x04 == 0)
			{
				//getParams()->display_flag = 0x80;
				oled_process(3);//
				getParams()->display_flag = 0x04;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
			}
	
		}
		break;
		case 1:
		{
			if(getParams()->display_flag&0x08 == 0)
			{
				//getParams()->display_flag = 0x80;
				oled_process(4);//
				getParams()->display_flag = 0x08;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
			}
	
		}
		break;
		case 2:
		{
			if(getParams()->display_flag&0x10 == 0)
			{
				//getParams()->display_flag = 0x80;
				oled_process(5);//
				getParams()->display_flag = 0x10;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
			}
	
		}
		break;
	
		}
		if(HAL_GPIO_ReadPin(WATER_GPIO_Port, WATER_Pin)==1||getParams()->temperature>90)
		{
			getParams()->mode = 0;
	
		}
			
		if(HAL_GPIO_ReadPin(WATER_GPIO_Port, WATER_Pin)==1)
			FM_ON();
		else
		   FM_OFF();

}
void gear_process(void)
{
	//档位调节与显示
	if(adc_process() == 2)
	{
		oled_process(2);//档位显示
	}

	if(getParams()->mode!=0) 	//如果为出水模式 则根据调节旋钮获取的数据修改pwm占空比
	{
		if(getParams()->mode==2&&getParams()->precent>0.2)			//如果为出热水模式且达到一定出水量 则驱动晶闸管
		{
			SCR_ON();
		}
		else				//否则关闭晶闸管
		{
			SCR_OFF();
		}
		
		getParams()->duty=round(700*(1-getParams()->precent));	//计算对应的比较寄存器值 700为电机刚能被驱动时对应的比较寄存器值
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, getParams()->duty);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, getParams()->duty);

	}
	else					//如果模式为停止模式 则电机停止
	{
		SCR_OFF();	//停止模式关闭晶闸管
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000-1);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000-1);

	}

}
void app_process()
{
	key_process();
	gear_process();
	device_ctrl_process();
	

   
}
