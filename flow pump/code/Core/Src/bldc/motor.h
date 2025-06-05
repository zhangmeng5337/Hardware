#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

#include "hall.h"

#define PID_CAL_TIME    10
#define ADC_BUF_SIZE    1
#define ADC_REF			2.5

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
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //��ȡ��������ʱ���еļ���ֵ
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //��ȡ�Զ�װ��ֵ,������Ϊ20000
#define MOTOR_SPEED_RERATIO 45u    //������ٱ�
#define PULSE_PRE_ROUND 11 //һȦ���ٸ�����
#define RADIUS_OF_TYRE 34 //��̥�뾶����λ����
#define LINE_SPEED_C RADIUS_OF_TYRE * 2 * 3.14
#define SPEED_RECORD_NUM 20 // �����ԣ�50Hz������ֵ�����˲���Ч���ȽϺ�

/*��ʼ�������Ľṹ��*/
typedef struct
{
	uint8_t Direction;//����
	float Set_Speed;//�ٶ�
	float Speed;//�ٶ�
	float Speed_Filter;//�ٶ�	
	uint8_t Duty;//ռ�ձ�
	uint8_t Status;//���״̬ 1��RUN
	uint8_t Hall_status;//HALL״ֵ̬
    uint32_t current_adc;
    float current;

	int32_t totalCount;
	int32_t lastCount;
	int32_t overflowNum;
} MOTOR;



void Six_Step_Phase(uint8_t hall_state,uint8_t direction);
void Change_PWM_Duty(uint16_t duty);
void Motor_Stop(void);
void Motor_Start(void);
MOTOR *getMotr(void);
void motor_ctrl_proc(void);

#endif


