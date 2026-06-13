#include "font_lib.h"
#include "utf8.h"
//#include "spi_flash.h"
#include "st7789.h"
#include <string.h>
#include <stdlib.h>
#include "fm25w32.h"

// 全局字库列表（最多支持 4 个）

static font_instance_t *g_fonts[MAX_FONTS];
static int g_font_count = 0;

/**
 * @brief 注册一个字库实例
 * @param base_addr  字库在 SPI Flash 中的基地址
 * @param font_size  逻辑字号（如 8,16,32，用于 draw_string 选择）
 * @param font       指向未初始化的 font_instance_t 结构体的指针（由调用者分配内存）
 * @return 0 成功，-1 失败（字库无效或注册表满）
 */
int font_register(uint32_t base_addr, uint8_t font_size, font_instance_t *font) {
    if (g_font_count >= MAX_FONTS) return -1;

    // 1. 记录基本信息
    font->base_addr = base_addr;
    font->font_size = font_size;

    // 2. 读取文件头
    font_header_t header;
//    spi_flash_read(base_addr, (uint8_t*)&header, sizeof(header));
	  FM25_Read((uint8_t*)&header, base_addr, sizeof(header));
    if (memcmp(header.magic, "MONO", 4) != 0) return -1;

    font->char_count = header.char_count;
    font->is_fixed = (header.flags & 0x01) == 0;   // 固定尺寸模式
    font->index_start = base_addr + sizeof(font_header_t);

    // 3. 计算像素数据区起始地址
    uint32_t index_size = font->char_count * sizeof(font_index_t);
    font->pixel_start = base_addr + sizeof(font_header_t) + index_size;

    // 4. 如果是固定尺寸，读取第一个索引条目获得固定宽高
    if (font->is_fixed) {
        font_index_t first_idx;
        //spi_flash_read(font->index_start, (uint8_t*)&first_idx, sizeof(font_index_t));
			FM25_Read((uint8_t*)&first_idx, font->index_start, sizeof(font_index_t));
        font->fixed_width  = first_idx.width;
        font->fixed_height = first_idx.height;
        font->fixed_pitch  = first_idx.pitch;
    } else {
        // 实际尺寸模式暂不支持自动范围，可以留空或后续动态获取
        font->fixed_width = font->fixed_height = font->fixed_pitch = 0;
    }

    // 5. 获取该字库支持的 Unicode 范围（通过读取第一个和最后一个索引条目）
    font_index_t first_idx, last_idx;
    //spi_flash_read(font->index_start, (uint8_t*)&first_idx, sizeof(font_index_t));
		FM25_Read((uint8_t*)&first_idx, font->index_start, sizeof(font_index_t));
    font->unicode_min = first_idx.unicode;

    uint32_t last_entry_addr = font->index_start + (font->char_count - 1) * sizeof(font_index_t);
    //spi_flash_read(last_entry_addr, (uint8_t*)&last_idx, sizeof(font_index_t));
		FM25_Read((uint8_t*)&last_idx, last_entry_addr, sizeof(font_index_t));
    font->unicode_max = last_idx.unicode;

    // 6. 存入全局列表
    g_fonts[g_font_count++] = font;
    return 0;
}


/**
 * @brief 根据 Unicode 和期望字号选择最合适的字库
 * @param unicode       字符的 Unicode 码点
 * @param desired_size  期望的字号（0 表示自动选择）
 * @return              匹配的字库实例指针，若未找到返回 NULL
 */
font_instance_t* font_select(uint32_t unicode, uint8_t desired_size) {
    font_instance_t *fallback = NULL;

    // 第一轮：优先匹配 desired_size
    if (desired_size != 0) {
        for (int i = 0; i < g_font_count; i++) {
            font_instance_t *f = g_fonts[i];
            if (f->font_size == desired_size && unicode >= f->unicode_min && unicode <= f->unicode_max) {
                return f;
            }
        }
    }

    // 第二轮：不限制字号，根据 Unicode 范围选择第一个匹配的字库
    for (int i = 0; i < g_font_count; i++) {
        font_instance_t *f = g_fonts[i];
        if (unicode >= f->unicode_min && unicode <= f->unicode_max) {
            return f;
        }
        // 记录第一个字库作为最终回退
        if (fallback == NULL) fallback = f;
    }

    // 第三轮：没有找到匹配范围，返回第一个字库（或 NULL）
    return fallback;
}


// 在字库中二分查找字符索引（直接在 Flash 中读取，不加载全部索引表）
int font_find_char(font_instance_t *font, uint32_t unicode, font_index_t *idx_buf)
{
    int32_t left = 0, right = (int32_t)font->char_count - 1;
    while (left <= right)
    {
        int32_t mid = (left + right) / 2;
        uint32_t entry_addr = font->index_start + mid * sizeof(font_index_t);
//        spi_flash_read(entry_addr, (uint8_t *)idx_buf, sizeof(font_index_t));
		FM25_Read((uint8_t *)idx_buf, entry_addr, sizeof(font_index_t));

		if (idx_buf->unicode == unicode)
        {
            return 0;
        }
        else if (idx_buf->unicode < unicode)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
//void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
//{
//    if ((x < 0) || (x >= ST7789_WIDTH) ||
//            (y < 0) || (y >= ST7789_HEIGHT))
//        return;
//
//    ST7789_SetAddressWindow(x, y, x, y);
//    uint8_t data[] = {color >> 8, color & 0xFF};
//    ST7789_Select();
//    ST7789_WriteData((unsigned char)(color >> 8), 1);
//    ST7789_WriteData((unsigned char)(color & 0xFF), 1);
//    ST7789_UnSelect();
//}

// 绘制字符（逐行读取点阵，逐行发送，降低 RAM 占用）
void draw_char(uint16_t x, uint16_t y, font_instance_t *font,
               font_index_t *idx, uint16_t fg_color, uint16_t bg_color)
{
    uint16_t w = idx->width;
    uint16_t h = idx->height;
    uint16_t pitch = idx->pitch;
    uint32_t bitmap_addr = font->pixel_start + idx->offset;

    // 设置窗口
    //    ST7789_SetWindow(x, y, x + w - 1, y + h - 1);
    //    ST7789_WriteCmd(0x2C);
    ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    // 逐行处理：读取一行点阵，生成一行 RGB565 数据并发送
    uint8_t row_buffer[256];          // 足够容纳一行点阵（最大 pitch 通常 ≤ 32）
    uint16_t pixel_row[256];          // 一行像素颜色（最大宽度 256）
    for (uint16_t row = 0; row < h; row++)
    {
        // 读取该行点阵数据
        //        spi_flash_read(bitmap_addr + row * pitch, row_buffer, pitch);
        FM25_Read(row_buffer, bitmap_addr + row * pitch, pitch);

        // 解析该行每个像素的颜色
        for (uint16_t col = 0; col < w; col++)
        {
            uint8_t byte = row_buffer[col / 8];
            uint8_t bit = (byte >> (7 - (col % 8))) & 1;
			uint16_t color = bit ? fg_color : bg_color;    // 选择颜色
//            pixel_row[col] = fg_color;//bit ? fg_color : bg_color;  
            ST7789_Select();
            ST7789_WriteData((unsigned char)(color >> 8), 1);
            ST7789_WriteData((unsigned char)(color & 0xFF), 1);
            ST7789_UnSelect();
        }


    }
}

// 绘制字符串（自动选择字库）
void draw_string_ex(uint16_t x, uint16_t y, uint16_t color,
                    uint8_t font_size, const char *str, uint16_t bg_color)
{
    uint16_t cursor_x = x;
    const uint8_t *ptr = (const uint8_t *)str;

    while (*ptr)
    {
        uint32_t unicode = utf8_decode(&ptr);
        font_instance_t *font = font_select(unicode, font_size);
        if (!font)
        {
            cursor_x += 8;   // 未找到字库，占位宽度
            continue;
        }
        font_index_t idx;
        if (font_find_char(font, unicode, &idx) == 0)
        {
            draw_char(cursor_x, y, font, &idx, color, bg_color);
            cursor_x += idx.width + 1;   // 字间距 1 像素
        }
        else
        {
            cursor_x += 8;   // 未找到字符，占位
        }
    }
}




