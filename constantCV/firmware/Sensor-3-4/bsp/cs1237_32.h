/*-----------------------------------------------------------------------------------
*   CS1237оƬ����  CS1237.h
*   2023-4-24   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # ����
*   - ����һ���첽��ȡ���ݵĿ⣬��Ҫÿ1ms����һ��CS1237_Ref()���Ի�ȡ�µ�����
*   - ��ʱֻ֧�ֵ�·CS1237���������ܻ���¶�·֧��
*   - ������֧�����п���оƬ��ֻ��Ҫ��ֲCS1237_bsp.c�ļ�������
*   - ע��_CS1237_ms(uint32_t ms)��_CS1237_Delay()��������ֲ
*------------------------------------------------------------------------------------
*   # ʹ�����̣�
*   - ��ֲCS1237_bsp.c�ļ������ݣ�����������ʹ�õĴ�����
*   - ����CS1237_Init��������CS1237оƬ��ʼ��
*   - ����CS1237_Start��������оƬ�ϵ磬���ҿ�ʼ�ɼ�����
*   - ����CS1237_GetData���������ص�ǰ�ɼ����ĵ�ѹֵ
*   - ����ѡ������CS1237_Finish��������CS1237оƬ�������ģʽ
*------------------------------------------------------------------------------------
*   The MIT License (MIT)
*
*   Copyright (C) 2023 Simgor001
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*-----------------------------------------------------------------------------------*/
/*==================================================================
 *     ����оƬͷ�ļ�
 * ===============================================================*/
//CH32ϵ��
//#include "debug.h"
//STM32F103ϵ��
//#include "stm32f10x.h"

/*==================================================================
 *     ������ͷ�ļ�
 * ===============================================================*/

#include "main.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//  �� SDA   ?   
#define SDA_IN() cs1237_sda_in()
#define SDA_OUT() cs1237_sda_out()

////   ?SDA   ? ?
//#define SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)
//// ��  SDA   ? ?
//#define SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
//#define SDA_LOW() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
//// ��  SCK   ? ?
//#define SCK_HIGH() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET)
//#define SCK_LOW() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET)

#define SCK_HIGH() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_SET)
#define SCK_LOW() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_RESET)

#define SDA_HIGH() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_SET)
#define SDA_LOW() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_RESET)

#define SDA_READ() GPIO_ReadPin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin)

/* cs1237 设备结构�?*/
struct cs1237_device
{
#define CS1237_DEFAULT_FREQUENCY 10 // 默认输出频率
#define RAW_DATA_MAX_NUM 3			// 用于滤波, 连续读取 RAW_DATA_MAX_NUM 次数据然后去掉最�? 最�? 
									// 再计算平均�?平均值带入卡尔曼滤波�?
	int32_t adc_data;                                 // 读取到的数据, 原始adc数据, 单次读取
    int32_t adc_calculate_raw_data[RAW_DATA_MAX_NUM]; // 读取数据后存�? 用于滤波, 连续采集12�?  
    int32_t adc_calculate_deal_data;                  // 读取数据后存�? 用于滤波
    uint8_t adc_config;                               // 模块配置参数
    uint16_t output_frequency;                        // 模块输出频率
    unsigned char reconfig;
    enum dev_ch
    {
        DEV_CH_NONE,          // 默认状�?        
        DEV_CH_A,             // 通道A
        DEV_CH_SAVE,          // 保留
        DEV_CH_TEMPERERATURE, // 温度
        DEV_CH_SHORT,         // 内部短路
        DEV_CH_TOTAL_STATE    // 状态统�?    
		} dev_ch_em;              // cs1237 通道选择

    enum dev_pga
    {
        DEV_PGA_NONE,
        DEV_PGA_1,
        DEV_PGA_2,
        DEV_PGA_64,
        DEV_PGA_128,
        DEV_PGA_TOTAL_STATE
    } dev_pga_em; // cs1237 pga选择, 在测温时选择 DEV_PGA_1

    enum dev_frequency
    {
        DEV_FREQUENCY_NONE, 
        DEV_FREQUENCY_10,//1,2
        DEV_FREQUENCY_40,
        DEV_FREQUENCY_640,
        DEV_FREQUENCY_1280,
        DEV_FREQUENCY_TOTAL_STATE
    } dev_frequency_em; // cs1237 输出频率选择

    enum dev_state
    {
        DEV_NONE,       // 默认状�?    
        DEV_ONLINE,     // 设备在线
        DEV_OFFLINE,    // 设备掉线
        DEV_TOTAL_STATE // 所有状态数�?   
	} dev_state_em;     // cs1237 是否在线, 不在线就不读取数�?
    int32_t get_adc_data;// 读取数据, 外部操作的函�?    
    uint8_t get_adc_config; // 读取配置数据, 外部操作的函�?
};



void cs1237_sda_in(void);
void cs1237_sda_out(void);

uint32_t cs1237_read_adc(void);
uint8_t cs1237_read_config2(void);
void cs1237_set_config(uint8_t registertowrite, uint8_t valuetowrite);
void cs1237_init2(void);
void system_setup(void);
int32_t system_loop(void);

