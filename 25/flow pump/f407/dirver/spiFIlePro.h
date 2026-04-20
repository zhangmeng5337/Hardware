#ifndef SPIFILEPRO_H
#define SPIFILEPRO_H
#incldue "main.h"
typedef struct {
    char name[32];      // 图片名称，不足32字节用'\0'填充
    uint32_t offset;    // 图片数据在.bin文件中的起始地址
    uint32_t size;      // 图片数据的总字节数
    uint16_t width;     // 图片宽度 (像素)
    uint16_t height;    // 图片高度 (像素)
} image_info_t;
typedef struct {
    char name[32];
    uint32_t offset;
    uint32_t size;
    uint32_t reserved;
} file_index_t;

typedef struct {
    uint32_t file_count;
    uint32_t index_offset;
    uint32_t index_size;
} file_footer_t;

#endif

