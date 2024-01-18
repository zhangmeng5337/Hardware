
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : main.c
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : main program body
* Description    : --
**********************************************************************/

/* Includes  == 文件包含 --------------------------------------------*/
#include "main.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//



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
int main(void)
{
	MP3_Main();
}	



#ifdef  USE_FULL_ASSERT
/**********************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : file: pointer to the source file name
*                  line: assert_param error line source number
* Output         : None
* Return         : None
* Remark	        : None
**********************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
