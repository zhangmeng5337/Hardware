// utf8.h


// utf8.c
#include "utf8.h"

uint32_t utf8_decode(const uint8_t **ptr) {
    uint8_t c = **ptr;
    if (c < 0x80) {
        (*ptr)++;
        return c;
    }
    if ((c & 0xE0) == 0xC0) {
        uint32_t u = (c & 0x1F) << 6;
        (*ptr)++;
        u |= (**ptr & 0x3F);
        (*ptr)++;
        return u;
    }
    if ((c & 0xF0) == 0xE0) {
        uint32_t u = (c & 0x0F) << 12;
        (*ptr)++;
        u |= (**ptr & 0x3F) << 6;
        (*ptr)++;
        u |= (**ptr & 0x3F);
        (*ptr)++;
        return u;
    }
    if ((c & 0xF8) == 0xF0) {
        uint32_t u = (c & 0x07) << 18;
        (*ptr)++;
        u |= (**ptr & 0x3F) << 12;
        (*ptr)++;
        u |= (**ptr & 0x3F) << 6;
        (*ptr)++;
        u |= (**ptr & 0x3F);
        (*ptr)++;
        return u;
    }
    // 无效字符，跳过
    (*ptr)++;
    return 0xFFFD;
}

