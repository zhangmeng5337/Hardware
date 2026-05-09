#include "font_lib.h"
#include "image_display.h"
#include "spiFileProc.h"
#include "fm25w32.h"

//
//文件索引表
//================================================================================
//序号   文件名                              偏移(Hex)      大小(Bytes)   
//--------------------------------------------------------------------------------
//0    gb2312_16.bin                    0x00000000   195292      
//1    gb2312_32.bin                    0x0002FB00   555772      
//2    ascii_8.bin                      0x000B7600   3452        
//3    ascii_16.bin                     0x000B8400   4972        
//4    image_data.bin                   0x000B9800   277976      
//================================================================================
//总文件数: 5
//索引表偏移: 0x000FD5D8
//索引表大小: 220 字节

// 全局字库实例（通常放在静态区）
static font_instance_t ascii16_font;
static font_instance_t ascii24_font;
static font_instance_t ascii32_font;


static font_instance_t han16_font;
static font_instance_t han32_font;
//static image_instance_t image_ins;
binStru binU;

void getBinBaseAddr(void)
{
   FM25_Read((uint8_t *)&binU, BIN_SIZE-4*3, sizeof(binU));
   uint32_t addrOffset;
   addrOffset = binU.offset+FILE_NAME_SIZE;
   for(unsigned char i = 0; i < binU.fileCount; i++)
   	{
		FM25_Read((uint8_t *)&binU.fileIndex[i] , addrOffset , sizeof(binU.fileIndex[i]));
		addrOffset = addrOffset + sizeof(binU.fileIndex[i])+FILE_NAME_SIZE;
   }
//   image_ins.base_addr = binU.fileIndex[FONT32_INDEX].offset;
//   image_ins.index_start = image_ins.base_addr + 4;
}
int stest(void) {
    // 初始化硬件（SPI、GPIO、ST7789 等）
//    spi_init();
//    st7789_init();
//    st7789_fill_screen(0x0000);   // 黑色背景
  //FM25_EraseChip();
		getBinBaseAddr();
    // 注册字库（注意：传入的 font 实例必须保持有效）
    font_register(binU.fileIndex[ASCII16_INDEX].offset, 16, &ascii16_font);    
    font_register(binU.fileIndex[ASCII24_INDEX].offset, 24, &ascii24_font);
    font_register(binU.fileIndex[ASCII32_INDEX].offset, 32, &ascii32_font);	
    font_register(binU.fileIndex[FONT16_INDEX].offset, 16, &han16_font);
    font_register(binU.fileIndex[FONT32_INDEX].offset, 32, &han32_font);
//    font_register(binU.fileIndex[IMAG_INDEX].offset, 48, &han32_font);

   
    font_index_t idx;
if (font_find_char(&ascii16_font, 0x41, &idx) == 0) {
    draw_char(10, 10, &ascii16_font, &idx, 0x0000,0xFFFF);
}
	uint32_t unicode = 0x4E16;	 // '世'

	if (font_find_char(&han16_font, unicode, &idx) == 0) {
		draw_char(50, 50, &han16_font, &idx, 0x0000,0xFFFF);
	}

    // 显示混合字符串（自动选择字号，背景黑色）
    draw_string(10, 10, "Hello 世界!",0,0xFFFF);
    
    // 指定使用 32x32 字号显示
    draw_string_ex(10, 50, 0x07E0, 16, "32px大字", 0xffff);
    draw_string_ex(10, 50, 0x07E0, 24, "32px大字", 0xffff);
	  draw_string_ex(10, 50, 0x07E0, 32, "32px大字", 0xffff);
    // 显示图片
    //draw_image(IMAGE_BASE, 10, 100, 100, 100);
    
    show_image("image_data.bin", 100, 100,binU.fileIndex[IMAG_INDEX].offset,0);
    while (1);
}
