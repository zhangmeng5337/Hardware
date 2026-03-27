/**
 * @file fixed_font.h
 * @brief 批量生成的点阵字库
 * 
 * 字符数量: 2
 * 字体: C:/Windows/Fonts/msyh.ttc
 * 字号: 16px
 * 固定字符尺寸: 32x32 (每个字符居中放置)
 * 生成时间: 2026-03-18 16:13:33
 */

#ifndef MY_FONT_H
#define MY_FONT_H

#include <stdint.h>
#include "main.h"
/*========================= 字库元数据 =========================*/

#define BOLD_FONT_CHAR_COUNT16x16 11
#define BOLD_FONT_CHAR_COUNT32x32 9



/**
 * @brief 字符信息结构体
 */
typedef struct {
    uint16_t unicode;      /**< 字符的Unicode码点 */
    uint16_t width;        /**< 字符点阵宽度 */
    uint16_t height;       /**< 字符点阵高度 */
    uint16_t left;         /**< 字符在画布中的左偏移（固定尺寸时有效，否则为原始左偏移） */
    uint16_t top;          /**< 字符在画布中的上偏移（固定尺寸时有效，否则为原始上偏移） */
    uint32_t offset;       /**< 在像素数组中的起始索引 */
} bold_font_char_info_t;
 typedef struct
{
	unsigned char str[10];
	uint16_t unicode;
}chineseUnicode_stru;



/*========================= 使用示例 =========================*/
/**
 * @code
 * #include "fixed_font.h"
 * 
 * const my_font_char_info_t* find_char(uint16_t unicode) {
 *     for (int i = 0; i < MY_FONT_CHAR_COUNT; i++) {
 *         if (my_font_chars[i].unicode == unicode) {
 *             return &my_font_chars[i];
 *         }
 *     }
 *     return NULL;
 * }
 * 
 * void draw_char(uint16_t x, uint16_t y, uint16_t unicode, TFT_eSPI* tft) {
 *     const my_font_char_info_t* info = find_char(unicode);
 *     if (!info) return;
 *     const uint16_t* pixels = &my_font_pixels[info->offset];
 *     tft->pushImage(x, y, info->width, info->height, pixels);
 * }
 * @endcode
 */
bold_font_char_info_t *findFont(unsigned char *unicode);
uint16_t *piexls(uint16_t unicode,unsigned char fontSize);
bold_font_char_info_t *find_char(uint16_t unicode, unsigned char fontSize);
#endif /* MY_FONT_H */

