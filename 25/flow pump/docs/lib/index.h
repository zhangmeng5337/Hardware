// 自动生成的文件索引表结构定义
#ifndef MERGED_INDEX_H
#define MERGED_INDEX_H

#include <stdint.h>

// 索引条目结构（44字节）
typedef struct {
    char name[32];          // 文件名
    uint32_t offset;        // 在合并文件中的偏移
    uint32_t size;          // 文件大小
    uint32_t reserved;      // 保留
} file_index_t;

// 文件尾信息结构
typedef struct {
    uint32_t file_count;    // 文件数量
    uint32_t index_offset;  // 索引表起始偏移
    uint32_t index_size;    // 索引表大小（字节）
} file_footer_t;

#endif
