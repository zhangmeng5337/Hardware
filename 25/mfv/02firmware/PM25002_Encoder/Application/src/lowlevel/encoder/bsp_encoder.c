/**
  ******************************************************************************
  * @file    bsp_motor_control.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   编码器接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_encoder.h"
encoder_stru encoder_u;
/* 定时器溢出次数 */

extern TIM_HandleTypeDef htim2;

/**
  * @brief  配置TIMx编码器模式
  * @param  无
  * @retval 无
  */
static void TIM_Encoder_Init(void)
{
    TIM_Encoder_InitTypeDef Encoder_ConfigStructure;

    /* 清零中断标志位 */
    __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
    /* 设置更新事件请求源为：计数器溢出 */
    __HAL_TIM_URS_ENABLE(&htim2);

    /* 设置中断优先级 */
    HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 5, 1);
    /* 使能定时器中断 */
    HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);

    /* 使能编码器接口 */
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2, PULSE_OFFSET);


	
}
encoder_stru *get_encoder(void)
{
    return &encoder_u;
}

/**
  * @brief  编码器接口初始化
  * @param  无
  * @retval 无
  */
void Encoder_Init(void)
{
    // Encoder_GPIO_Init();    /* 引脚初始化 */
    TIM_Encoder_Init();     /* 配置编码器接口 */
    encoder_u.hit_flag = 0;
    encoder_u.zero_capture_count = 0;
    encoder_u.encoder_max = 0;
    encoder_u.home_complete_flag = 0;
}

/*********************************************END OF FILE**********************/

void Set_Encoder_Zero()
{
    uint32_t encoder_tmp;
    __HAL_GPIO_EXTI_CLEAR_IT(EZ_Pin);

    float cal_val;
    encoder_tmp = __HAL_TIM_GET_COUNTER(&htim2);
    if (encoder_u.encoder_max <= encoder_tmp && encoder_u.home_complete_flag == 2)
    {
        encoder_u.encoder_max = encoder_tmp + (get_encoder()->encoder_overflow_count * PULSE_OFFSET);//53000   52900
    }
    cal_val = encoder_tmp;
    cal_val = encoder_u.encoder_max - cal_val;
    cal_val = fabs(cal_val);
    if (cal_val >= 5.0 && encoder_u.home_complete_flag == 1)
    {
      //  __HAL_TIM_SET_COUNTER(&htim2, encoder_u.encoder_max);
       ;// get_encoder()->encoder_overflow_count++;
    }
}
void encoder_over_flow(void)
{
 /* ?D??μ±?°??êy·??ò */
    if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2))
      /* ??ò? */
      get_encoder()->encoder_overflow_count--;
    else
      /* é?ò? */
      get_encoder()->encoder_overflow_count++;
}