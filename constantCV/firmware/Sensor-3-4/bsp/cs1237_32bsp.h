/*
 * CS1237_bsp.c
 *
 *  Created on: 2023年4月23日
 *      Author: Simgor001
 */
#include "cs1237_32.h"
#include "main.h"


/*==================================================================
 *     移植层
 * ===============================================================*/

void  Delay_Ms(uint32_t ms)
{
    uint32_t tick;
	tick = GetTick();
	while((GetTick()-tick)<ms)
		;
}



/**
 *@brief 延迟毫秒
 *@param ms 毫秒
 **/
void _CS1237_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

/**
 * @brief 延迟一个脉冲
 */
void _CS1237_Delay()
{
    //这里至少要延迟500ns
    __IO char i = 50;
    while(--i);
}

/**
 *@brief 初始化GPIO（需要移植）
 **/
//void _CS1237_GPIO_Init()
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
//    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);
//
//    GPIO_InitStruct.GPIO_Pin = _CS1237_CLK_PIN;
//    GPIO_Init(_CS1237_CLK_GPIO, &GPIO_InitStruct);
//}

/**
 *@brief 设置数据引脚方向为输入（需要移植）
 **/
void _CS1237_Input()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

}

/**
 *@brief 设置数据引脚方向为输出（需要移植）
 **/
void _CS1237_Output()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pins = CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CS1237_DOUT_GPIO_Port, &GPIO_InitStruct);

}

/**
 *@brief 时钟输出低电平（需要移植）
 **/
void _CS1237_CLK_ResetBits()
{
	GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_RESET);
}

/**
 *@brief 时钟输出高电平（需要移植）
 **/
void _CS1237_CLK_SetBits()
{
	GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_SET);
}

/**
 *@brief 数据输出低电平（需要移植）
 **/
void _CS1237_SDA_ResetBits()
{
	GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_RESET);
}

/**
 *@brief 数据输出高电平（需要移植）
 **/
void _CS1237_SDA_SetBits()
{

GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_SET);
}

/**
 *@brief 读取数据线的值（需要移植）
 **/
uint8_t _CS1237_SDA_ReadBits()
{
    return GPIO_ReadPin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin);
}

