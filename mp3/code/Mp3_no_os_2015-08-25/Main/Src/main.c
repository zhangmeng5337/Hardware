
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
#include "oled2.h"


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
	u8 t;
	//OLED_Init();
	//OLED_ColorTurn(0);//0正常显示，1 反色显示
//  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Refresh();
	t=' ';
	while(1)
	{
		//OLED_ShowPicture(0,0,120,4,BMP1);
		delay_ms(500);
		OLED_Clear();
		//OLED_ShowPicture(0,0,120,4,BMP2);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(4,0,0,16);//中
		OLED_ShowChinese(20,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(52,0,3,16);//电
		OLED_ShowChinese(68,0,4,16);//子
		OLED_ShowChinese(84,0,5,16);//科
		OLED_ShowChinese(100,0,6,16);//技
		OLED_ShowString(21,16,"ZHONGJINGYUAN",12);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(20,0,"2014/05/01",12);
		OLED_ShowString(0,14,"ASCII:",12);  
		OLED_ShowString(53,14,"CODE:",12);
		OLED_ShowChar(38,14,t,12);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(93,14,t,3,12);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);  //16*16 中
	  OLED_ShowChinese(16,0,0,24); //24*24 中
		OLED_Refresh();
	  delay_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
	  OLED_ShowString(20,0,"ABC",16);//8*16 “ABC”
		OLED_ShowString(50,0,"ABC",24);//12*24 “ABC”
	  OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4);
	}
	
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
