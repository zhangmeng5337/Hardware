#ifndef __STEP_MOTOR_ENCODER_H
#define	__STEP_MOTOR_ENCODER_H

#include "main.h"

/* 定时器选择 */
#define ENCODER_TIM                            TIM2
#define ENCODER_TIM_CLK_ENABLE()  				     __HAL_RCC_TIM3_CLK_ENABLE()

/* 定时器溢出值 */		
#define ENCODER_TIM_PERIOD                     65535
/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER                  0      



/* 编码器接口倍频数 */
#define ENCODER_MODE                           TIM_ENCODERMODE_TI12

/* 编码器接口输入捕获通道相位设置 */           
#define ENCODER_IC1_POLARITY                   TIM_ICPOLARITY_FALLING
#define ENCODER_IC2_POLARITY                   TIM_ICPOLARITY_RISING

/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION                     1000

/* 经过倍频之后的总分辨率 */
#if ((ENCODER_MODE == TIM_ENCODERMODE_TI1) || (ENCODER_MODE == TIM_ENCODERMODE_TI2))
  #define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 2)  /* 2倍频后的总分辨率 */
#else
  #define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */
#endif



extern TIM_HandleTypeDef TIM_EncoderHandle;
typedef struct 
{
	int32_t 	Encoder_Overflow_Count;
	unsigned char dirction;
	uint32_t capture_count;
	uint32_t count_per_unit;
	uint32_t last_count;
	unsigned char valid_flag;
	float shaft_speed;
	float total_rotations;
}encoder_stru;
void Encoder_Init(void);
encoder_stru *get_encoder(void);
void encoder_cal_speed(void);
void Set_Encoder_Zero(void);
void encoder_over_flow(void);
void encoder_data_init(void);
#endif   /* __BSP_ENCODER_H */


