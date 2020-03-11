
#include "74hc595.h"

void M74HC595_Init(void)
{
//	RCC_AHBPeriphClockCmd(M74HC595_PORT_RCC, ENABLE);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Pin = (M74HC595_G_NOT | M74HC595_RCK | M74HC595_SCK | M74HC595_SCLR_NOT | M74HC595_SI);
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_Init(M74HC595_PORT, &GPIO_InitStructure);

//	M74HC595_ClearData();
}

void M74HC595_ClearData(void)
{
	//GPIO_ResetBits(M74HC595_PORT, M74HC595_SCLR_NOT);
//	GPIO_SetBits(M74HC595_PORT, M74HC595_SCLR_NOT);
}

void M74HC595_EnableData(void)
{
	//GPIO_ResetBits(M74HC595_PORT, M74HC595_G_NOT);
}

void M74HC595_WriteData(const unsigned char data)
{
	unsigned char i;
	unsigned  char j;
	j = 1;
	for(i=0; i < 8; i++)
	{
		if((j << i) & data)
			HAL_GPIO_WritePin(M74HC595_PORT, M74HC595_SI,GPIO_PIN_SET);	//bit by bit send the 8-bit data to SI
		else
			HAL_GPIO_WritePin(M74HC595_PORT, M74HC595_SI,GPIO_PIN_RESET);	//bit by bit send the 8-bit data to SI		
		
		HAL_GPIO_WritePin(M74HC595_PORT, M74HC595_SCK,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(M74HC595_PORT, M74HC595_SCK,GPIO_PIN_SET);		//low to high transition allows data to be registered in shift register
	}
	HAL_GPIO_WritePin(M74HC595_RCK_PORT, M74HC595_RCK,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(M74HC595_RCK_PORT, M74HC595_RCK,GPIO_PIN_SET);			//low to high transition allows data in shift register to be transfered to storage register

	M74HC595_EnableData();
}

void seg_select(unsigned char num)
{
	switch(num)
	{
		case 1:
		{
				HAL_GPIO_WritePin(DIS_COM4_GPIO_Port, DIS_COM4_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM3_GPIO_Port, DIS_COM3_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM2_GPIO_Port, DIS_COM2_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM1_GPIO_Port, DIS_COM1_Pin,GPIO_PIN_RESET);				
		}break;
		case 2:
		{
				HAL_GPIO_WritePin(DIS_COM4_GPIO_Port, DIS_COM4_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM3_GPIO_Port, DIS_COM3_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM2_GPIO_Port, DIS_COM2_Pin,GPIO_PIN_RESET);		
				HAL_GPIO_WritePin(DIS_COM1_GPIO_Port, DIS_COM1_Pin,GPIO_PIN_SET);				
		}break;
		case 3:
		{
				HAL_GPIO_WritePin(DIS_COM4_GPIO_Port, DIS_COM4_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM3_GPIO_Port, DIS_COM3_Pin,GPIO_PIN_RESET);		
				HAL_GPIO_WritePin(DIS_COM2_GPIO_Port, DIS_COM2_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM1_GPIO_Port, DIS_COM1_Pin,GPIO_PIN_SET);				
		}break;
		case 4:
		{
				HAL_GPIO_WritePin(DIS_COM4_GPIO_Port, DIS_COM4_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM3_GPIO_Port, DIS_COM3_Pin,GPIO_PIN_RESET);		
				HAL_GPIO_WritePin(DIS_COM2_GPIO_Port, DIS_COM2_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM1_GPIO_Port, DIS_COM1_Pin,GPIO_PIN_SET);				
		}break;		
		default:
		{
				HAL_GPIO_WritePin(DIS_COM4_GPIO_Port, DIS_COM4_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM3_GPIO_Port, DIS_COM3_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM2_GPIO_Port, DIS_COM2_Pin,GPIO_PIN_SET);		
				HAL_GPIO_WritePin(DIS_COM1_GPIO_Port, DIS_COM1_Pin,GPIO_PIN_SET);				
		}break;	
	}	
}

