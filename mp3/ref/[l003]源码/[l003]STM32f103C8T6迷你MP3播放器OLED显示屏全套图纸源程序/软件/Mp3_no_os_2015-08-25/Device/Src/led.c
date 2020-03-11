
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : led.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : led program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "led.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
#define	D_LED_STA_GPIO			GPIOA	
#define	D_LED_STA_PIN			GPIO_Pin_9	
#define	D_LED_STA_CLK			RCC_APB2Periph_GPIOA



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/

	
	
/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

   //使能LED相关GPIO口时钟
	RCC_APB2PeriphClockCmd(D_LED_STA_CLK, ENABLE);

	//配置LED的输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		
	GPIO_InitStructure.GPIO_Pin = D_LED_STA_PIN;
	GPIO_Init(D_LED_STA_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(D_LED_STA_GPIO, D_LED_STA_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	LED_STA_On(void)
{
	GPIO_ResetBits(D_LED_STA_GPIO, D_LED_STA_PIN);
}	



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void	LED_STA_Off(void)
{
	GPIO_SetBits(D_LED_STA_GPIO, D_LED_STA_PIN);
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void LED_STA_Trun(void)
{
	if(GPIO_ReadOutputDataBit(D_LED_STA_GPIO, D_LED_STA_PIN))
	{
		LED_STA_On();
	}
	else
	{
		LED_STA_Off();
	}
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
