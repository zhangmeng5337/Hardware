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
button_stru button_u;
/* 定时器溢出次数 */

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;
/**
  * @brief  配置TIMx编码器模式
  * @param  无
  * @retval 无
  */
static void TIM_Encoder_Init(void)
{
    TIM_Encoder_InitTypeDef Encoder_ConfigStructure;

    /* 清零中断标志位 */
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
    /* 设置更新事件请求源为：计数器溢出 */
    __HAL_TIM_URS_ENABLE(&htim4);

    /* 设置中断优先级 */
    HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 5, 0);
    /* 使能定时器中断 */
    HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);

    /* 使能编码器接口 */
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim4, PULSE_OFFSET);

    //TIM_Encoder_InitTypeDef Encoder_ConfigStructure;

    /* 清零中断标志位 */
    __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
    /* 设置更新事件请求源为：计数器溢出 */
    __HAL_TIM_URS_ENABLE(&htim3);

    /* 设置中断优先级 */
    HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 5, 1);
    /* 使能定时器中断 */
    HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);

    /* 使能编码器接口 */
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim3, PULSE_OFFSET);

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
}

/*********************************************END OF FILE**********************/

void Set_Encoder_Zero()
{
    uint32_t encoder_tmp;
    //__HAL_GPIO_EXTI_CLEAR_IT(EZ_Pin);

    float cal_val;
    encoder_tmp = __HAL_TIM_GET_COUNTER(&htim4);
}
void encoder_over_flow(void)
{
    /* ?D??μ±?°??êy·??ò */
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
        /* ??ò? */
        get_encoder()->encoder_overflow_count--;
    else
        /* é?ò? */
        get_encoder()->encoder_overflow_count++;
}
void updateButton(void)
{
    uint32_t tmp1;
    if (button_u.pollPeriod >= 50)
    {
        button_u.capture_count = __HAL_TIM_GET_COUNTER(&htim3);
        button_u.capture_per_unit = button_u.capture_count - button_u.last_count;

        if (button_u.capture_count >= (PULSE_OFFSET + 20000) ||
                button_u.capture_count <= (PULSE_OFFSET - 20000))
        {
            __HAL_TIM_SET_COUNTER(&htim3, PULSE_OFFSET);
        }
		if(button_u.last_count != button_u.capture_count)
			button_u.status = 1;      
		button_u.last_count = button_u.capture_count;
        button_u.pollPeriod = 0;
    }
}
void update_speed(void)
{
    uint32_t tmp1;
    static uint16_t div = 0;
    if (div++ == 100)
    {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
            /* ??ò? */
            get_encoder()->dir = FORWARD;
        else
            get_encoder()->dir = BACKWORD;
        tmp1 = __HAL_TIM_GET_COUNTER(&htim4);

        get_encoder()->capture_count = tmp1 + (get_encoder()->encoder_overflow_count * PULSE_OFFSET);
        encoder_u.totalNumRotation = encoder_u.capture_count / ENCODER_TOTAL_RESOLUTION;
        // 单位时间的脉冲数 = 当前的脉冲总计数值 - 上一次的脉冲总计数值
        get_encoder()->capture_per_unit = get_encoder()->capture_count - get_encoder()->last_count;
        encoder_u.speed = encoder_u.capture_per_unit / ENCODER_TOTAL_RESOLUTION * 10;
        div = 0;
    }
}