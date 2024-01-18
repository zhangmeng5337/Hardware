
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : power.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : power program body
* Description    : --
**********************************************************************/

/* Includes  == �ļ����� --------------------------------------------*/
#include "power.h"



/* Private typedef    ==  ˽������ ----------------------------------*/
/* Private define     ==  ˽�ж��� ----------------------------------*/

//-----------------------------------------//
#define	D_POWER_GPIO		GPIOB	
#define	D_POWER_PIN			GPIO_Pin_4	
#define	D_POWER_CLK			RCC_APB2Periph_GPIOB



/* Private macro      ==  ˽�к� ------------------------------------*/
/* Private variables  ==  ˽�б��� ----------------------------------*/

	
	
/* Private functions  ==  ˽�к������� ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	POWER_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

   //ʹ��LED���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(D_POWER_CLK, ENABLE);

	//����LED���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		
	GPIO_InitStructure.GPIO_Pin = D_POWER_PIN;
	GPIO_Init(D_POWER_GPIO, &GPIO_InitStructure);
	
	POWER_Off();
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	POWER_On(void)
{
	GPIO_SetBits(D_POWER_GPIO, D_POWER_PIN);
}



/**********************************************************************
* Function Name	: None
* Description		: None
* Input				: None
* Output				: None
* Return 			: None
* Remark				: None
**********************************************************************/
void	POWER_Off(void)
{
	GPIO_ResetBits(D_POWER_GPIO, D_POWER_PIN);
}



/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
