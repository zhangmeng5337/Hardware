#include "sysclock.h"


//#define HSE_Enable

/*******************************************************************************
 * ����: Sysclock_Init
 * ����: ����ϵͳʱ��Ƶ��
 * �β�: ��
 * ����: ��
 * ˵��: ʱ��ѡ��

 ******************************************************************************/
void SystemClock_Init(void)
{
//ʹ�ú궨���д��
  
#ifdef  HSE_Enable
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     //�л����ⲿʱ��,���ȴ�ʱ��ȴ���ɹ�
#else
		//CLK_HSICmd(ENABLE);
		CLK_HSICmd(ENABLE);

		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	  	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); 
	
#endif   

}



