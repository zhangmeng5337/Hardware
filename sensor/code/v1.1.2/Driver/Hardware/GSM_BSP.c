
#include "GSM_BSP.h"
#include "bsp.h"
//#include "systick_timer.h"
//extern unsigned char LTE_RX_BUFF[buffer_size];

//void LTE_hw_init()
//{
//	//��Դ��ʹ�ܣ�GPS���߲����磬�ػ�
//	HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin|GPS_PWR_CE_Pin|MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
//	//lte��λ��Ч
//	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
//}


//lte��λ����
//void LET_reset()
//{
//	//__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
//
//	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_SET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
//}


//lte���ػ�����
//void LET_power_ctrl()
//{
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_SET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
//}


//gps���ߵ�Դʹ�ܺ���
//void LET_gps_power_ctrl(unsigned char  flag)
//{
//	switch(flag)
//	{
//	case 0:
//		HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_RESET);
//	case 1:
//		HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_SET);
//	}
//}

//void LTE_power_oFF()
//{
//	//��Դ��ʹ�ܣ�GPS���߲����磬�ػ�
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_SET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
//	delay_ms(500);
//	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_SET);
//HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin, GPIO_PIN_RESET);
//
//}
//void LTE_power_on()
//{
//	//��Դ��ʹ�ܣ�GPS���߲����磬�ػ�
//	HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin, GPIO_PIN_SET);
//
//}
//extern Uart_Types UsartType5;
/******************************************************************************/
void Send_Comm(unsigned char* comm,unsigned short len)
{

	//HAL_UART_DMAStop(&huart5);
	//HAL_UART_DMAResume(&huart5);
       //UsartType5.real_index=0;
	//UsartType5.rx_len=0;
	//UsartType5.rx_len_var=0;
	//UsartType5.loop_index=0;
	//HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
	//delay_ms(100);
	//uart5_dma_state=0;
	USART_SenByte(comm,len);

}

//void dma_clear_flag()
//{
//	;
//}

void modulePowerOn(unsigned char pwr_ctrl)
{
  if(pwr_ctrl == ON)
  {
    GPIO_SetBits(PORT_POWER_ON, PIN_POWER_ON);  
    GPIO_SetBits(PORT_PWRKEY_IN, PIN_PWRKEY_IN); 
  }
  else
  {
    GPIO_ResetBits(PORT_POWER_ON, PIN_POWER_ON);  
    GPIO_ResetBits(PORT_PWRKEY_IN, PIN_PWRKEY_IN); 
  }
   
}

