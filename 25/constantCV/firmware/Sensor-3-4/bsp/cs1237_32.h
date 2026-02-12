/*-----------------------------------------------------------------------------------
*   CS1237Ð¾Æ¬Çý¶¯  CS1237.h
*   2023-4-24   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # ½éÉÜ
*   - ÕâÊÇÒ»¸öÒì²½»ñÈ¡Êý¾ÝµÄ¿â£¬ÐèÒªÃ¿1msµ÷ÓÃÒ»´ÎCS1237_Ref()£¬ÒÔ»ñÈ¡ÐÂµÄÊý¾Ý
*   - ÔÝÊ±Ö»Ö§³Öµ¥Â·CS1237£¬ºóÐø¿ÉÄÜ»á¸üÐÂ¶àÂ·Ö§³Ö
*   - ÀíÂÛÉÏÖ§³ÖËùÓÐ¿ØÖÆÐ¾Æ¬£¬Ö»ÐèÒªÒÆÖ²CS1237_bsp.cÎÄ¼þµÄÄÚÈÝ
*   - ×¢Òâ_CS1237_ms(uint32_t ms)ºÍ_CS1237_Delay()º¯ÊýµÄÒÆÖ²
*------------------------------------------------------------------------------------
*   # Ê¹ÓÃÁ÷³Ì£º
*   - ÒÆÖ²CS1237_bsp.cÎÄ¼þµÄÄÚÈÝ£¬ÈÃÆäÊÊÅäÄúÊ¹ÓÃµÄ´¦ÀíÆ÷
*   - µ÷ÓÃCS1237_Initº¯Êý£¬½«CS1237Ð¾Æ¬³õÊ¼»¯
*   - µ÷ÓÃCS1237_Startº¯Êý£¬ÈÃÐ¾Æ¬ÉÏµç£¬²¢ÇÒ¿ªÊ¼²É¼¯Êý¾Ý
*   - µ÷ÓÃCS1237_GetDataº¯Êý£¬·µ»Øµ±Ç°²É¼¯µ½µÄµçÑ¹Öµ
*   - £¨¿ÉÑ¡£©µ÷ÓÃCS1237_Finishº¯Êý£¬ÈÃCS1237Ð¾Æ¬½øÈëµôµçÄ£Ê½
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
 *     ÒýÈëÐ¾Æ¬Í·ÎÄ¼þ
 * ===============================================================*/
//CH32ÏµÁÐ
//#include "debug.h"
//STM32F103ÏµÁÐ
//#include "stm32f10x.h"

/*==================================================================
 *     Çý¶¯¿âÍ·ÎÄ¼þ
 * ===============================================================*/

#include "main.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//  §Ý SDA   ?   
#define SDA_IN() cs1237_sda_in()
#define SDA_OUT() cs1237_sda_out()

////   ?SDA   ? ?
//#define SDA_READ() HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)
//// §Õ  SDA   ? ?
//#define SDA_HIGH() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
//#define SDA_LOW() HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
//// §Õ  SCK   ? ?
//#define SCK_HIGH() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET)
//#define SCK_LOW() HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET)

#define SCK_HIGH() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_SET)
#define SCK_LOW() GPIO_WritePin(CS1237_SCL_GPIO_Port, CS1237_SCL_Pin, GPIO_Pin_RESET)

#define SDA_HIGH() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_SET)
#define SDA_LOW() GPIO_WritePin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin, GPIO_Pin_RESET)

#define SDA_READ() GPIO_ReadPin(CS1237_DOUT_GPIO_Port, CS1237_DOUT_Pin)

/* cs1237 è®¾å¤‡ç»“æž„ä½?*/
struct cs1237_device
{
#define CS1237_DEFAULT_FREQUENCY 10 // é»˜è®¤è¾“å‡ºé¢‘çŽ‡
#define RAW_DATA_MAX_NUM 3			// ç”¨äºŽæ»¤æ³¢, è¿žç»­è¯»å– RAW_DATA_MAX_NUM æ¬¡æ•°æ®ç„¶åŽåŽ»æŽ‰æœ€é«? æœ€ä½? 
									// å†è®¡ç®—å¹³å‡å€?å¹³å‡å€¼å¸¦å…¥å¡å°”æ›¼æ»¤æ³¢å™?
	int32_t adc_data;                                 // è¯»å–åˆ°çš„æ•°æ®, åŽŸå§‹adcæ•°æ®, å•æ¬¡è¯»å–
    int32_t adc_calculate_raw_data[RAW_DATA_MAX_NUM]; // è¯»å–æ•°æ®åŽå­˜å…? ç”¨äºŽæ»¤æ³¢, è¿žç»­é‡‡é›†12æ¬?  
    int32_t adc_calculate_deal_data;                  // è¯»å–æ•°æ®åŽå­˜å…? ç”¨äºŽæ»¤æ³¢
    uint8_t adc_config;                               // æ¨¡å—é…ç½®å‚æ•°
    uint16_t output_frequency;                        // æ¨¡å—è¾“å‡ºé¢‘çŽ‡
    unsigned char reconfig;
    enum dev_ch
    {
        DEV_CH_NONE,          // é»˜è®¤çŠ¶æ€?        
        DEV_CH_A,             // é€šé“A
        DEV_CH_SAVE,          // ä¿ç•™
        DEV_CH_TEMPERERATURE, // æ¸©åº¦
        DEV_CH_SHORT,         // å†…éƒ¨çŸ­è·¯
        DEV_CH_TOTAL_STATE    // çŠ¶æ€ç»Ÿè®?    
		} dev_ch_em;              // cs1237 é€šé“é€‰æ‹©

    enum dev_pga
    {
        DEV_PGA_NONE,
        DEV_PGA_1,
        DEV_PGA_2,
        DEV_PGA_64,
        DEV_PGA_128,
        DEV_PGA_TOTAL_STATE
    } dev_pga_em; // cs1237 pgaé€‰æ‹©, åœ¨æµ‹æ¸©æ—¶é€‰æ‹© DEV_PGA_1

    enum dev_frequency
    {
        DEV_FREQUENCY_NONE, 
        DEV_FREQUENCY_10,//1,2
        DEV_FREQUENCY_40,
        DEV_FREQUENCY_640,
        DEV_FREQUENCY_1280,
        DEV_FREQUENCY_TOTAL_STATE
    } dev_frequency_em; // cs1237 è¾“å‡ºé¢‘çŽ‡é€‰æ‹©

    enum dev_state
    {
        DEV_NONE,       // é»˜è®¤çŠ¶æ€?    
        DEV_ONLINE,     // è®¾å¤‡åœ¨çº¿
        DEV_OFFLINE,    // è®¾å¤‡æŽ‰çº¿
        DEV_TOTAL_STATE // æ‰€æœ‰çŠ¶æ€æ•°é‡?   
	} dev_state_em;     // cs1237 æ˜¯å¦åœ¨çº¿, ä¸åœ¨çº¿å°±ä¸è¯»å–æ•°æ?
    int32_t get_adc_data;// è¯»å–æ•°æ®, å¤–éƒ¨æ“ä½œçš„å‡½æ•?    
    uint8_t get_adc_config; // è¯»å–é…ç½®æ•°æ®, å¤–éƒ¨æ“ä½œçš„å‡½æ•?
};



void cs1237_sda_in(void);
void cs1237_sda_out(void);

uint32_t cs1237_read_adc(void);
uint8_t cs1237_read_config2(void);
void cs1237_set_config(uint8_t registertowrite, uint8_t valuetowrite);
void cs1237_init2(void);
void system_setup(void);
int32_t system_loop(void);

