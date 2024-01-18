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
		CLK_HSICmd(ENABLE);      //ʹ���ڲ�����ʱ��Դ 16MHZ
		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);  //HSI��Ϊϵͳʱ��Դ
	  	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);   //����ϵͳ��Ƶ����ƵԽ��ϵͳָ��ʱ��Խ��������Խ��
	
#endif   

}



