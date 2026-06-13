#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>
#include "main.h"

// 从 UTF-8 字节流中解码一个 Unicode 码点，指针前进
uint32_t utf8_decode(const uint8_t **ptr);

#endif

