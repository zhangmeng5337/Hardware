// image_display.h
#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <stdint.h>
#include "main.h"
// 索引条目结构（与 merge_bins.py 生成的格式一致）
typedef struct {
    char     name[32];       // 文件名（不含扩展名，不足32字节补'\0'）
    uint32_t offset;         // 该图片数据在合并文件中的偏移（字节）
    uint32_t size;           // 图片数据大小（字节）
    uint16_t width;          // 图片宽度（像素）
    uint16_t height;         // 图片高度（像素）
    // 注意：此处实际占 32+4+4+2+2 = 44 字节，无额外填充
} image_info_t;

// 合并文件尾部信息结构（最后12字节）
typedef struct {
    uint32_t file_count;     // 图片数量
    uint32_t index_offset;   // 索引表在合并文件中的偏移
    uint32_t index_size;     // 索引表大小（字节）
} image_footer_t;
typedef struct
{
	uint32_t baseAddr;
	uint32_t offset;
	uint32_t iconsCount;
}image_index_t;

// 显示一张 RGB565 图片（数据已连续存储在 SPI Flash 中）
// flash_addr: 图片数据在 Flash 中的绝对地址
// x, y: 显示位置
// w, h: 图片宽度、高度（像素）
void draw_image(uint32_t flash_addr, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void show_image(char *name, uint16_t x, uint16_t y);
void resourceInit(uint32_t baseAddr,uint32_t offset,uint32_t count);

#endif

