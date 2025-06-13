/*
 * CS1237_bsp.c
 *
 *  Created on: 2023��4��23��
 *      Author: Simgor001
 */
#include "cs1237_32.h"
#include "main.h"


/*==================================================================
 *     ��ֲ��
 * ===============================================================*/

void  Delay_Ms(uint32_t ms)
{
    uint32_t tick;
	tick = GetTick();
	while((GetTick()-tick)<ms)
		;
}



/**
 *@brief �ӳٺ���
 *@param ms ����
 **/
void _CS1237_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

/**
 * @brief �ӳ�һ������
 */
void _CS1237_Delay()
{
    //��������Ҫ�ӳ�500ns
    __IO char i = 50;
    while(--i);
}

/**
 *@brief ��ʼ��GPIO����Ҫ��ֲ��
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
 *@brief �����������ŷ���Ϊ���루��Ҫ��ֲ��
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
 *@brief �����������ŷ���Ϊ�������Ҫ��ֲ��
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
 *@brief ʱ������͵�ƽ����Ҫ��ֲ��
 **/
void _CS1237_CLK_ResetBits()
{
	GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_RESET);
}

/**
 *@brief ʱ������ߵ�ƽ����Ҫ��ֲ��
 **/
void _CS1237_CLK_SetBits()
{
	GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_SET);
}

/**
 *@brief ��������͵�ƽ����Ҫ��ֲ��
 **/
void _CS1237_SDA_ResetBits()
{
	GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_RESET);
}

/**
 *@brief ��������ߵ�ƽ����Ҫ��ֲ��
 **/
void _CS1237_SDA_SetBits()
{

GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_SET);
}

/**
 *@brief ��ȡ�����ߵ�ֵ����Ҫ��ֲ��
 **/
uint8_t _CS1237_SDA_ReadBits()
{
    return GPIO_ReadPin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin);
}

