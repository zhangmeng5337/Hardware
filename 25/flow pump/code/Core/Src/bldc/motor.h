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

//定义一些宏，提高程序可读性
#define RUN 1
#define HALT 0

#define FORWORD 1
#define REVERSAL 0
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //获取编码器定时器中的计数值
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //获取自动装载值,本例中为20000
#define MOTOR_SPEED_RERATIO 45u    //电机减速比
#define PULSE_PRE_ROUND 11 //一圈多少个脉冲
#define RADIUS_OF_TYRE 34 //轮胎半径，单位毫米
#define LINE_SPEED_C RADIUS_OF_TYRE * 2 * 3.14
#define SPEED_RECORD_NUM 20 // 经测试，50Hz个采样值进行滤波的效果比较好

/*开始定义电机的结构体*/
typedef struct
{
	uint8_t Direction;//方向
	float Set_Speed;//速度
	float Speed;//速度
	float Speed_Filter;//速度	
	uint8_t Duty;//占空比
	uint8_t Status;//电机状态 1：RUN
	uint8_t Hall_status;//HALL状态值
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


