#include "di_do.h"
di_do_stru diDoSt;
void readDi()
{

    if (HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di0;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di0);

    if (HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di1;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di1);
	
    if (HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di2;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di2);

    if (HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di3;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di3);

    if (HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di4;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di4);

    if (HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di5;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di5);

    if (HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin) == GPIO_PIN_SET)
        diDoSt.diLevel = diDoSt.diLevel | di6;
    else
        diDoSt.diLevel = diDoSt.diLevel & (~di6);
}
void getDiStatus(void)
{
	if(diDoSt.diLevel == 0)
		diDoSt.diStatus = 0;
	else
		diDoSt.diStatus = 1;
}
void doInit(void)
{
setDo(0,DO_OFF);
setDo(1,DO_OFF);
}
void setDo(unsigned char doNum,unsigned char level)
{
	if(doNum == 0)
	{
	   if(level == DO_ON)
		diDoSt.dolevel = diDoSt.dolevel &0xfe;
	   else
		diDoSt.dolevel = diDoSt.dolevel | 0x01;
	   HAL_GPIO_WritePin(DO_ctrl_0_GPIO_Port,DO_ctrl_0_Pin,level);
	}
	else if(doNum == 1)
	{
	   if(level == DO_ON)
		diDoSt.dolevel = diDoSt.dolevel &0xfd;
	   else
		diDoSt.dolevel = diDoSt.dolevel | 0x02;
	   HAL_GPIO_WritePin(DO_ctrl_1_GPIO_Port,DO_ctrl_1_Pin,level);
	}
		
}

