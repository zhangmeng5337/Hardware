#ifndef FONT_LIB_H
#define FONT_LIB_H

#include <stdint.h>
#include "main.h"
#define MAX_FONTS 5

// 字库文件头（与生成脚本一致，共32字节）
typedef struct {
    uint8_t  magic[4];      // "MONO"
    uint32_t version;
    uint32_t char_count;
    uint16_t flags;         // bit0: 1=实际尺寸, 0=固定尺寸
    uint8_t  reserved[18];
} font_header_t;

// 索引条目（20字节）
typedef struct {
    uint32_t unicode;
    uint32_t offset;        // 像素数据区内的偏移
    uint16_t width;
    uint16_t height;
    uint16_t pitch;         // (width+7)/8
    uint8_t  reserved[6];
} font_index_t;

// 字库实例（仅缓存必要信息）
typedef struct {
    uint32_t base_addr;     // 字库在 SPI Flash 中的基地址
    uint32_t pixel_start;   // 像素数据区起始绝对地址
    uint32_t char_count;    // 字符总数
    uint32_t index_start;   // 索引表起始绝对地址
    uint8_t  is_fixed;      // 1=固定尺寸, 0=实际尺寸
    uint8_t  font_size;     // 逻辑字号（用于选择，如16表示16x16）
    // 以下字段仅在固定尺寸模式下有效
    uint16_t fixed_width;
    uint16_t fixed_height;
    uint16_t fixed_pitch;
	uint32_t unicode_min;
    uint32_t unicode_max;
} font_instance_t;

// 注册一个字库（从 Flash 读取头信息，填充 font_instance_t）
// base_addr: 字库在 Flash 中的起始地址
// font_size: 逻辑字号（0 表示自动，用于字符串绘制时选择）
// 返回值: 0-成功, -1-失败
int font_register(uint32_t base_addr, uint8_t font_size, font_instance_t *font);

// 根据 Unicode 和期望字号查找合适的字库实例
// 如果 desired_size==0，则返回第一个包含该字符的字库
// 否则优先匹配相同 font_size 的字库
font_instance_t* font_select(uint32_t unicode, uint8_t desired_size);

// 在指定字库中查找字符索引（直接读 Flash，二分查找）
// 返回 0 表示找到，并将索引信息填入 idx_buf；否则返回 -1
int font_find_char(font_instance_t *font, uint32_t unicode, font_index_t *idx_buf);

// 绘制单个字符（从 Flash 读取点阵并发送到屏幕）
// x, y: 左上角坐标
// font: 字库实例
// idx: 字符索引信息
// fg_color, bg_color: 前景/背景色（RGB565）
void draw_char(uint16_t x, uint16_t y, font_instance_t *font,
               font_index_t *idx, uint16_t fg_color, uint16_t bg_color);

// 绘制 UTF-8 字符串（自动选择字库，可指定字号和背景色）
// x, y: 起始坐标
// color: 文字颜色
// font_size: 0 自动选择，否则优先使用此字号
// str: UTF-8 字符串
// bg_color: 背景色
void draw_string_ex(uint16_t x, uint16_t y, uint16_t color,
                    uint8_t font_size, const char *str, uint16_t bg_color);

// 简化版（背景黑色，字号自动）
static inline void draw_string(uint16_t x, uint16_t y, const char *str,uint16_t color, uint16_t bgcolor) {
    draw_string_ex(x, y, color, 0, str, bgcolor);
}

#endif

