#ifndef __BSP_ENCOEDER_H
#define	__BSP_ENCOEDER_H

#include "main.h"
#define FORWARD 0
#define BACKWORD 1
#define PULSE_OFFSET  TIM4PERIOD/2
/* 定时器选择 */
#define ENCODER_TIM                            TIM4
#define ENCODER_TIM_CLK_ENABLE()  				     __HAL_RCC_TIM4_CLK_ENABLE()

/* 定时器溢出值 */		
#define ENCODER_TIM_PERIOD                     65535
/* 定时器预分频值 */
#define ENCODER_TIM_PRESCALER                  0      

/* 定时器中断 */
#define ENCODER_TIM_IRQn                       TIM4_IRQn
#define ENCODER_TIM_IRQHandler                 TIM4_IRQHandler

/* 编码器接口引脚 */
//#define ENCODER_TIM_CH1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
//#define ENCODER_TIM_CH1_GPIO_PORT              GPIOC
//#define ENCODER_TIM_CH1_PIN                    GPIO_PIN_6
//#define ENCODER_TIM_CH1_GPIO_AF                GPIO_AF2_TIM3
//
//#define ENCODER_TIM_CH2_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
//#define ENCODER_TIM_CH2_GPIO_PORT              GPIOC
//#define ENCODER_TIM_CH2_PIN                    GPIO_PIN_7
//#define ENCODER_TIM_CH2_GPIO_AF                GPIO_AF2_TIM3

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
typedef struct
{
     float last_count ;
   float capture_count ;//total pulse count
   float capture_per_unit ;// 
   int cont_val ;
   int16_t encoder_overflow_count;
   float encoder_error_resolution;
   float totalNumRotation;
   unsigned char dir;//0:forward 1:backword

  unsigned char hit_dir;//0:forward 1:back
  float speed;
  unsigned int pollPeriod;
}encoder_stru;

typedef struct
{
     float last_count ;
     float capture_count ;//total pulse count
     float capture_per_unit ;//
     unsigned int pollPeriod;
	 unsigned char status;//1:hitted
	 unsigned char dir;
	 unsigned char enter;
}button_stru;

encoder_stru *get_encoder(void);
void Encoder_Init(void);
void encoder_over_flow(void);
void clear_encoder(void);
void encoder_proc(void);
button_stru *get_button(void);
#endif   /* __BSP_ENCODER_H */

