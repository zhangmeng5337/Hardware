// 自动生成的单色字库数据结构定义
#ifndef FONT_MONO_H
#define FONT_MONO_H

#include <stdint.h>

typedef struct {
    uint8_t  magic[4];      // "MONO"
    uint32_t version;
    uint32_t char_count;
    uint16_t flags;         // bit0: 1=实际尺寸
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
