/*-----------------------------------------------------------------------------------
*   CS1237芯片驱动  CS1237.h
*   2023-4-24   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # 介绍
*   - 这是一个异步获取数据的库，需要每1ms调用一次CS1237_Ref()，以获取新的数据
*   - 暂时只支持单路CS1237，后续可能会更新多路支持
*   - 理论上支持所有控制芯片，只需要移植CS1237_bsp.c文件的内容
*   - 注意_CS1237_ms(uint32_t ms)和_CS1237_Delay()函数的移植
*------------------------------------------------------------------------------------
*   # 使用流程：
*   - 移植CS1237_bsp.c文件的内容，让其适配您使用的处理器
*   - 调用CS1237_Init函数，将CS1237芯片初始化
*   - 调用CS1237_Start函数，让芯片上电，并且开始采集数据
*   - 调用CS1237_GetData函数，返回当前采集到的电压值
*   - （可选）调用CS1237_Finish函数，让CS1237芯片进入掉电模式
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
 *     引入芯片头文件
 * ===============================================================*/
//CH32系列
//#include "debug.h"
//STM32F103系列
//#include "stm32f10x.h"

/*==================================================================
 *     驱动库头文件
 * ===============================================================*/

#include "main.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//  л SDA   ?   
#define SDA_IN() cs1237_sda_in()
#define SDA_OUT() cs1237_sda_out()

////   ?SDA   ? ?
//#define SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)
//// д  SDA   ? ?
//#define SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
//#define SDA_LOW() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
//// д  SCK   ? ?
//#define SCK_HIGH() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET)
//#define SCK_LOW() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET)

#define SCK_HIGH() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_SET)
#define SCK_LOW() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_RESET)

#define SDA_HIGH() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_SET)
#define SDA_LOW() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_RESET)

#define SDA_READ() GPIO_ReadPin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin)

/* cs1237 璁惧缁撴瀯浣?*/
struct cs1237_device
{
#define CS1237_DEFAULT_FREQUENCY 10 // 榛樿杈撳嚭棰戠巼
#define RAW_DATA_MAX_NUM 3			// 鐢ㄤ簬婊ゆ尝, 杩炵画璇诲彇 RAW_DATA_MAX_NUM 娆℃暟鎹劧鍚庡幓鎺夋渶楂? 鏈�浣? 
									// 鍐嶈绠楀钩鍧囧�?骞冲潎鍊煎甫鍏ュ崱灏旀浖婊ゆ尝鍣?
	int32_t adc_data;                                 // 璇诲彇鍒扮殑鏁版嵁, 鍘熷adc鏁版嵁, 鍗曟璇诲彇
    int32_t adc_calculate_raw_data[RAW_DATA_MAX_NUM]; // 璇诲彇鏁版嵁鍚庡瓨鍏? 鐢ㄤ簬婊ゆ尝, 杩炵画閲囬泦12娆?  
    int32_t adc_calculate_deal_data;                  // 璇诲彇鏁版嵁鍚庡瓨鍏? 鐢ㄤ簬婊ゆ尝
    uint8_t adc_config;                               // 妯″潡閰嶇疆鍙傛暟
    uint16_t output_frequency;                        // 妯″潡杈撳嚭棰戠巼
    unsigned char reconfig;
    enum dev_ch
    {
        DEV_CH_NONE,          // 榛樿鐘舵�?        
        DEV_CH_A,             // 閫氶亾A
        DEV_CH_SAVE,          // 淇濈暀
        DEV_CH_TEMPERERATURE, // 娓╁害
        DEV_CH_SHORT,         // 鍐呴儴鐭矾
        DEV_CH_TOTAL_STATE    // 鐘舵�佺粺璁?    
		} dev_ch_em;              // cs1237 閫氶亾閫夋嫨

    enum dev_pga
    {
        DEV_PGA_NONE,
        DEV_PGA_1,
        DEV_PGA_2,
        DEV_PGA_64,
        DEV_PGA_128,
        DEV_PGA_TOTAL_STATE
    } dev_pga_em; // cs1237 pga閫夋嫨, 鍦ㄦ祴娓╂椂閫夋嫨 DEV_PGA_1

    enum dev_frequency
    {
        DEV_FREQUENCY_NONE, 
        DEV_FREQUENCY_10,//1,2
        DEV_FREQUENCY_40,
        DEV_FREQUENCY_640,
        DEV_FREQUENCY_1280,
        DEV_FREQUENCY_TOTAL_STATE
    } dev_frequency_em; // cs1237 杈撳嚭棰戠巼閫夋嫨

    enum dev_state
    {
        DEV_NONE,       // 榛樿鐘舵�?    
        DEV_ONLINE,     // 璁惧鍦ㄧ嚎
        DEV_OFFLINE,    // 璁惧鎺夌嚎
        DEV_TOTAL_STATE // 鎵�鏈夌姸鎬佹暟閲?   
	} dev_state_em;     // cs1237 鏄惁鍦ㄧ嚎, 涓嶅湪绾垮氨涓嶈鍙栨暟鎹?
    int32_t get_adc_data;// 璇诲彇鏁版嵁, 澶栭儴鎿嶄綔鐨勫嚱鏁?    
    uint8_t get_adc_config; // 璇诲彇閰嶇疆鏁版嵁, 澶栭儴鎿嶄綔鐨勫嚱鏁?
};



void cs1237_sda_in(void);
void cs1237_sda_out(void);

uint32_t cs1237_read_adc(void);
uint8_t cs1237_read_config2(void);
void cs1237_set_config(uint8_t registertowrite, uint8_t valuetowrite);
void cs1237_init2(void);
void system_setup(void);
int32_t system_loop(void);

