// 自动生成的 ASCII 字库数据结构定义
#ifndef ASCII_FONT_H
#define ASCII_FONT_H

#include <stdint.h>

typedef struct {
    uint8_t  magic[4];
    uint32_t version;
    uint32_t char_count;
    uint16_t flags;
    uint8_t  reserved[18];
} font_header_t;

typedef struct {
    uint32_t unicode;
    uint32_t offset;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t  reserved[6];
} font_index_t;

#endif
