#ifndef __MOTOR_H__
#define __MOTOR_H__


#include "hall.h"
#define WL_ON HAL_GPIO_WritePin(MCU_WL_GPIO_Port, MCU_WL_Pin,GPIO_PIN_RESET);
#define WL_OFF HAL_GPIO_WritePin(MCU_WL_GPIO_Port, MCU_WL_Pin,GPIO_PIN_SET);

#define VL_ON HAL_GPIO_WritePin(MCU_VL_GPIO_Port, MCU_VL_Pin,GPIO_PIN_RESET);
#define VL_OFF HAL_GPIO_WritePin(MCU_VL_GPIO_Port, MCU_VL_Pin,GPIO_PIN_SET);

#define UL_ON HAL_GPIO_WritePin(MCU_UL_GPIO_Port, MCU_UL_Pin,GPIO_PIN_RESET);
#define UL_OFF HAL_GPIO_WritePin(MCU_UL_GPIO_Port, MCU_UL_Pin,GPIO_PIN_SET);

//����һЩ�꣬��߳���ɶ���
#define RUN 1
#define HALT 0

#define FORWORD 1
#define REVERSAL 0

/*��ʼ�������Ľṹ��*/
typedef struct
{
	uint8_t Direction;//����
	uint8_t Speed;//�ٶ�
	uint8_t Duty;//ռ�ձ�
	uint8_t Status;//���״̬
	uint8_t Hall_status;//HALL״ֵ̬
} MOTOR;



void Six_Step_Phase(uint8_t hall_state,uint8_t direction);
void Change_PWM_Duty(uint16_t duty);
void Motor_Stop(void);
void Motor_Start(void);
MOTOR *getMotr(void);

#endif


