

// image_display.c
#include "image_display.h"
#include "fm25w32.h"
#include <stdlib.h>
#include "st7789.h"

/**
 * @brief 根据文件名查找图片信息
 * @param name      图片名称（不含路径，例如 "logo"）
 * @param out_info  输出图片信息
 * @return 1 成功，0 失败
 */
int find_image(uint32_t baseAddr,const char *name, image_info_t *out_info) {
    // 1. 从文件末尾读取尾部信息（最后12字节）
//    uint32_t file_size = get_merged_file_size(); // 你需要知道合并文件总大小，或从 Flash 末尾读取固定位置
//    uint32_t footer_addr = MERGED_BASE_ADDR + file_size - 12;
//    uint32_t file_count, index_offset, index_size;
//    spi_flash_read(footer_addr, (uint8_t*)&file_count, 4);
//    spi_flash_read(footer_addr + 4, (uint8_t*)&index_offset, 4);
//    spi_flash_read(footer_addr + 8, (uint8_t*)&index_size, 4);

    // 2. 遍历索引表
//    for (uint32_t i = 0; i < file_count; i++) {
//        uint32_t entry_addr = baseAddr + index_offset + i * sizeof(image_info_t);
//        spi_flash_read(entry_addr, (uint8_t*)out_info, sizeof(image_info_t));
//        if (strcmp(out_info->name, name) == 0) {
//            return 1;
//        }
//    }
//    return 0;
}



void draw_image(uint32_t flash_addr, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 设置窗口
    //    
    //    ST7789_WriteCmd(0x2C);
    //ST7789_SetAddressWindow(x, y, x, y);
	ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    // 逐行读取并发送（每行 w*2 字节）
    uint16_t row_size = w * 2;
    uint8_t row_buf[640];// = (uint8_t *)malloc(row_size);
    //if (!row_buf)
      //  return;

    for (uint16_t row = 0; row < h; row++)
    {
        uint32_t addr = flash_addr + row * row_size;
        //        spi_flash_read(addr, row_buf, row_size);
        FM25_Read(row_buf, addr, row_size);
        for (uint16_t col = 0; col < w; col++)
        {
            // 小端序合并为 16 位颜色
            uint16_t color = row_buf[col * 2] | (row_buf[col * 2 + 1] << 8);
            //            ST7789_WriteColor(color);
            ST7789_Select();
            ST7789_WriteData((unsigned char)(color >> 8), 1);
            ST7789_WriteData((unsigned char)(color & 0xFF), 1);
            ST7789_UnSelect();
        }
    }
    //free(row_buf);
}
// 显示图片
void show_image(const char *name, uint16_t x, uint16_t y,uint32_t baseAddr,uint32_t index) {
    image_info_t info;
	info.height = 48;
	info.width = 48;
	info.size = 0x1200;
	info.offset = index * info.size;
    draw_image( baseAddr + info.offset,x, y, info.width, info.height);
}


