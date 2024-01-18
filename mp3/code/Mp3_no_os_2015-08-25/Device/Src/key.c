
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : key.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : key program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "key.h"
#include "oled.h"
#include "timer.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
/*#define	D_KEY_VINC_GPIO			GPIOC
#define	D_KEY_VINC_PIN			GPIO_Pin_1	
#define	D_KEY_VINC_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_VDEC_GPIO			GPIOB	
#define	D_KEY_VDEC_PIN			GPIO_Pin_7	
#define	D_KEY_VDEC_CLK			RCC_APB2Periph_GPIOB

#define	D_KEY_POWER_GPIO		GPIOC	
#define	D_KEY_POWER_PIN			GPIO_Pin_2	
#define	D_KEY_POWER_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_BACK_GPIO			GPIOB	
#define	D_KEY_BACK_PIN			GPIO_Pin_5	
#define	D_KEY_BACK_CLK			RCC_APB2Periph_GPIOB

#define	D_KEY_NEXT_GPIO			GPIOC
#define	D_KEY_NEXT_PIN			GPIO_Pin_3	
#define	D_KEY_NEXT_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_BEFORE_GPIO		GPIOA	
#define	D_KEY_BEFORE_PIN		GPIO_Pin_2
#define	D_KEY_BEFORE_CLK		RCC_APB2Periph_GPIOA

#define	D_KEY_STOP_GPIO			GPIOA	
#define	D_KEY_STOP_PIN			GPIO_Pin_1	
#define	D_KEY_STOP_CLK			RCC_APB2Periph_GPIOA

#define	D_KEY_LOCK_GPIO			GPIOA	
#define	D_KEY_LOCK_PIN			GPIO_Pin_3	
#define	D_KEY_LOCK_CLK			RCC_APB2Periph_GPIOA*/

#define	D_KEY_VINC_GPIO			GPIOC
#define	D_KEY_VINC_PIN			GPIO_Pin_1	
#define	D_KEY_VINC_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_VDEC_GPIO			GPIOC	
#define	D_KEY_VDEC_PIN			GPIO_Pin_2	
#define	D_KEY_VDEC_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_POWER_GPIO		GPIOA	
#define	D_KEY_POWER_PIN			GPIO_Pin_2	
#define	D_KEY_POWER_CLK			RCC_APB2Periph_GPIOA

#define	D_KEY_BACK_GPIO			GPIOA	
#define	D_KEY_BACK_PIN			GPIO_Pin_8	
#define	D_KEY_BACK_CLK			RCC_APB2Periph_GPIOA

#define	D_KEY_NEXT_GPIO			GPIOC
#define	D_KEY_NEXT_PIN			GPIO_Pin_3	
#define	D_KEY_NEXT_CLK			RCC_APB2Periph_GPIOC

#define	D_KEY_BEFORE_GPIO		GPIOB	
#define	D_KEY_BEFORE_PIN		GPIO_Pin_1
#define	D_KEY_BEFORE_CLK		RCC_APB2Periph_GPIOB

#define	D_KEY_STOP_GPIO			GPIOA	
#define	D_KEY_STOP_PIN			GPIO_Pin_0	
#define	D_KEY_STOP_CLK			RCC_APB2Periph_GPIOA

#define	D_KEY_LOCK_GPIO			GPIOA	
#define	D_KEY_LOCK_PIN			GPIO_Pin_15	
#define	D_KEY_LOCK_CLK			RCC_APB2Periph_GPIOA


/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/
volatile	u8		KEY_ActiveFlag;
volatile	u8		KEY_Type;
volatile	u8		KEY_UpDown;
volatile	u8		KEY_LongPushCnt;
volatile	s64	KEY_ScanTime;



/* Private functions  ==  私有函数定义 ------------------------------*/

/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void	KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

   //使能KEY相关GPIO口时钟
	RCC_APB2PeriphClockCmd(	D_KEY_VINC_CLK 	|
									D_KEY_VDEC_CLK 	|
									D_KEY_POWER_CLK 	|
									D_KEY_BACK_CLK 	|
									D_KEY_NEXT_CLK 	|
									D_KEY_BEFORE_CLK	|
									D_KEY_STOP_CLK	|
									D_KEY_LOCK_CLK
									, ENABLE);
	
	//配置KEY的输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
   
	GPIO_InitStructure.GPIO_Pin = D_KEY_VINC_PIN;
	GPIO_Init(D_KEY_VINC_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D_KEY_VDEC_PIN;
	GPIO_Init(D_KEY_VDEC_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D_KEY_POWER_PIN;
	GPIO_Init(D_KEY_POWER_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D_KEY_BACK_PIN;
	GPIO_Init(D_KEY_BACK_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D_KEY_NEXT_PIN;
	GPIO_Init(D_KEY_NEXT_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = D_KEY_BEFORE_PIN;
	GPIO_Init(D_KEY_BEFORE_GPIO, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = D_KEY_STOP_PIN;
	GPIO_Init(D_KEY_STOP_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = D_KEY_LOCK_PIN;
	GPIO_Init(D_KEY_LOCK_GPIO, &GPIO_InitStructure); 
}
	


/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void	KEY_Config(void)
{
	KEY_GPIO_Config();
	KEY_ActiveFlag 	= D_KEY_ACTIVE_NO;
	KEY_Type          = D_KEY_NULL;
	KEY_UpDown 			= 0;
	KEY_LongPushCnt   = 0;
	KEY_ScanTime      = 0;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
u8 KEY_GetStatus(u8 keynum)
{
	u8 temp;
	switch(keynum)
	{
		case  D_KEY_IS_VINC:
               temp = GPIO_ReadInputDataBit(D_KEY_VINC_GPIO, D_KEY_VINC_PIN);
					break;
					
		case  D_KEY_IS_VDEC:	
				   temp = GPIO_ReadInputDataBit(D_KEY_VDEC_GPIO, D_KEY_VDEC_PIN);
					break;

		case  D_KEY_IS_POWER:
					temp = GPIO_ReadInputDataBit(D_KEY_POWER_GPIO, D_KEY_POWER_PIN);
					break;

		case  D_KEY_IS_BACK:
					temp = GPIO_ReadInputDataBit(D_KEY_BACK_GPIO, D_KEY_BACK_PIN);
					break;

		case  D_KEY_IS_NEXT:
					temp = GPIO_ReadInputDataBit(D_KEY_NEXT_GPIO, D_KEY_NEXT_PIN);
					break;

		case  D_KEY_IS_BEFORE:
					temp = GPIO_ReadInputDataBit(D_KEY_BEFORE_GPIO, D_KEY_BEFORE_PIN);
					break;
		
		case  D_KEY_IS_STOP:
					temp = GPIO_ReadInputDataBit(D_KEY_STOP_GPIO, D_KEY_STOP_PIN);
					break;

		case  D_KEY_IS_LOCK:
					temp = GPIO_ReadInputDataBit(D_KEY_LOCK_GPIO, D_KEY_LOCK_PIN);
					break;
		
		default: 
					break;
	}
	
	return temp;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
void KEY_Clear(D_KEY_StatusTypeDef key)
{
	key.State = D_KEY_ACTIVE_NO;
	key.Active = D_KEY_ACTIVE_NO;
	key.Type = D_KEY_ACTIVE_NO;
	key.LongPushCnt = 0;
}



/**********************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Remark         : None
**********************************************************************/
D_KEY_StatusTypeDef KEY_Scan(s64 scan_time)
{
	u8 i;
	s64 timetemp;
	D_KEY_StatusTypeDef key;
	
	key.Active = D_KEY_ACTIVE_NO;	

	timetemp = MICROSEC_GetTime();
	if((timetemp - KEY_ScanTime) > scan_time)
	{
		KEY_ScanTime = timetemp;
		if(KEY_ActiveFlag == D_KEY_ACTIVE_NO)
		{		
			for(i=0; i<D_KEY_NUM; i++)
			{
				if(KEY_GetStatus(i+1) == 0)
				{
					KEY_ActiveFlag = D_KEY_ACTIVE_YES;
					KEY_Type = i+1;
					KEY_UpDown = 1;     //001，按下 
				}
			}
		}
		else
		{
			OLED_SetDisplayOnOff(0x01);
			TIME4_BackLightTime = 0;		
			key.Type = KEY_Type;			
			if(KEY_GetStatus(KEY_Type) == 0)
			{
				KEY_UpDown = ((KEY_UpDown << 1) & 0x06) + 1;
			}
			else
			{
				KEY_UpDown = (KEY_UpDown << 1) & 0x06;
			}
			
			switch(KEY_UpDown)
			{
				case  0x07://111，长按
							key.State = D_KEY_LONGDOWN;
							key.Active = D_KEY_ACTIVE_YES;
							KEY_LongPushCnt++;
							break;
							
				case  0x03://011，按下	
							key.State = D_KEY_DOWN;
							key.Active = D_KEY_ACTIVE_YES;
							KEY_LongPushCnt = 0;
							break;

				case  0x06://110，松开
							key.State = D_KEY_UP;
							key.Active = D_KEY_ACTIVE_YES;
							KEY_LongPushCnt++;
							if(TIME4_BackLightFlag == 1)
							{
								TIME4_BackLightFlag = 0;
								key.Active = D_KEY_ACTIVE_NO;
								return  key;
							}	
							break;
				
				case  0x04://100，退出
							KEY_ActiveFlag = D_KEY_ACTIVE_NO;
							key.Active = D_KEY_ACTIVE_NO;
							KEY_LongPushCnt = 0;							
							break;

				case  0x02://010，退出
							KEY_ActiveFlag = D_KEY_ACTIVE_NO;
							key.Active = D_KEY_ACTIVE_NO;
							KEY_LongPushCnt = 0;	
							break;
				
				case  0x05://101，退出
							KEY_ActiveFlag = D_KEY_ACTIVE_NO;
							key.Active = D_KEY_ACTIVE_NO;
							KEY_LongPushCnt = 0;	
							break;

				case  0x00://000，退出
							KEY_ActiveFlag = D_KEY_ACTIVE_NO;
							key.Active = D_KEY_ACTIVE_NO;
							KEY_LongPushCnt = 0;	
							break;
				
				default: 
							break;
			}
			key.LongPushCnt = KEY_LongPushCnt;
		}
	}
	return key;
}




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
