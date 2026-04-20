#include "spiFIlePro.h"
#include <stdint.h>
#include <string.h>

#define FLASH_BASE_ADDR     0x00000000  // SPI Flash中.bin文件的烧录基地址
#define IMAGE_TABLE_OFFSET  4           // 文件表大小字段占用4字节
#define TABLE_PTR_OFFSET    4           // 文件表指针字段占用4字节

image_info_t g_img_table[100]; // 假设最多100张图，根据实际情况分配
uint32_t g_img_count = 0;

/**
 * @brief 从SPI Flash中解析文件表，建立图片索引
 * @note  需要在初始化阶段调用一次
 */
void parse_image_table(void) {
    uint32_t table_addr = 0;
    // 1. 从Flash末尾读取文件表起始地址
    spi_flash_read(FLASH_BASE_ADDR + TOTAL_BIN_SIZE - TABLE_PTR_OFFSET, (uint8_t*)&table_addr, 4);

    // 2. 从文件表起始地址读取图片数量
    spi_flash_read(FLASH_BASE_ADDR + table_addr, (uint8_t*)&g_img_count, 4);

    // 3. 循环读取每张图片的信息
    for (uint32_t i = 0; i < g_img_count; i++) {
        uint32_t entry_addr = FLASH_BASE_ADDR + table_addr + IMAGE_TABLE_OFFSET + i * sizeof(image_info_t);
        spi_flash_read(entry_addr, (uint8_t*)&g_img_table[i], sizeof(image_info_t));
    }
}
void FLASH_ReadData(uint32_t addr, uint8_t *buf, uint32_t len)
{
    // 实现：通过 SPI 发送读命令（0x03）+ 3字节地址，然后读取 len 字节到 buf
}

// 假设字库烧录在 SPI Flash 的起始地址 FONT_BASE_ADDR
#define FONT_BASE_ADDR  0x00000000
#define FONT_FIXED_W    16      // 固定宽度（像素）
#define FONT_FIXED_H    16      // 固定高度（像素）
#define BYTES_PER_CHAR  ((FONT_FIXED_W * FONT_FIXED_H) / 8)   // 单色点阵字节数

/**
 * @brief 根据 GB2312 内码获取字符点阵数据指针（从 Flash 读取到缓冲区）
 * @param gb_code  GB2312 内码，例如 0xB0A1
 * @param buffer   输出缓冲区，至少 BYTES_PER_CHAR 字节
 * @return         成功返回 1，失败返回 0
 */
uint8_t get_char_bitmap_by_gb(uint16_t gb_code, uint8_t *buffer) {
    uint8_t high = gb_code >> 8;
    uint8_t low  = gb_code & 0xFF;
    
    // 检查是否在一级汉字范围内 (0xB0A1 - 0xD7FE)
    if (high < 0xB0 || high > 0xD7 || low < 0xA1 || low > 0xFE) {
        return 0;
    }
    
    uint8_t area = high - 0xB0;        // 区号偏移（0~39）
    uint8_t pos  = low - 0xA1;         // 位号偏移（0~93）
    uint16_t index = area * 94 + pos;  // 在字库中的序号（0~3754）
    uint32_t offset = FONT_BASE_ADDR + sizeof(font_header_t) + index * BYTES_PER_CHAR;
    
    // 从 SPI Flash 读取数据
    spi_flash_read(offset, buffer, BYTES_PER_CHAR);
    return 1;
}

void DrawChar(uint16_t x, uint16_t y, uint16_t color, uint8_t *code)
{
    uint8_t font_buf[32];
    GetFontData(code, font_buf);
    get_char_bitmap_by_gb(code, font_buf);
    for (uint8_t row = 0; row < 16; row++) {
        for (uint8_t col = 0; col < 16; col++) {
            // 判断当前像素点是否为 1（需要点亮）
            if (font_buf[row * 2 + col / 8] & (0x80 >> (col % 8))) {
                ST7789_DrawPoint(x + col, y + row, color);
            }
        }
    }
}

void DrawString(uint16_t x, uint16_t y, uint16_t color,uint16_t bgcolor, char *str)
{
    uint16_t cur_x = x, cur_y = y;
    while (*str) {
        if ((uint8_t)*str >= 0xA1) {            // 汉字（高字节）
            uint8_t code[2] = { *str, *(str+1) };
            DrawChar(cur_x, cur_y, color, code);
            cur_x += 16 + 2;                    // 字宽 + 间距
            str += 2;
        } else {                                 // ASCII（需另备字库）
            // 这里调用显示 ASCII 字符的函数（略）
            cur_x += 8;
            str++;
        }
        if (cur_x + 16 > ST7789_WIDTH) {        // 换行
            cur_x = x;
            cur_y += 16 + 2;
        }
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();   // 用于 Flash
    MX_SPI2_Init();   // 用于 ST7789

    ST7789_Init();
    ST7789_FillRect(0, 0, 240, 320, 0xFFFF);   // 白屏

    // 显示汉字
    DrawString(20, 50, 0x0000, "你好，世界！");
    DrawString(20, 90, 0xF800, "STM32F407");
    DrawString(20, 130, 0x07E0, "SPI Flash字库");

    while (1);
}



#include "index.h"
#include <string.h>

// 假设合并后的文件烧录在 SPI Flash 基地址 FLASH_BASE
#define FLASH_BASE 0x00000000

/**
 * 根据文件名查找文件信息
 * @param name       要查找的文件名
 * @param out_offset 输出偏移
 * @param out_size   输出大小
 * @return           成功返回1，失败返回0
 */
uint8_t find_file(const char *name, uint32_t *out_offset, uint32_t *out_size) {
    file_footer_t footer;
    // 读取末尾12字节获取索引信息
    spi_flash_read(FLASH_BASE + TOTAL_BIN_SIZE - 12, (uint8_t*)&footer, sizeof(footer));

    // 读取索引表（可以一次性读入RAM，或者直接在Flash中查找）
    file_index_t idx;
    for (uint32_t i = 0; i < footer.file_count; i++) {
        uint32_t entry_offset = FLASH_BASE + footer.index_offset + i * sizeof(file_index_t);
        spi_flash_read(entry_offset, (uint8_t*)&idx, sizeof(idx));
        if (strcmp(idx.name, name) == 0) {
            *out_offset = idx.offset;
            *out_size = idx.size;
            return 1;
        }
    }
    return 0;
}

