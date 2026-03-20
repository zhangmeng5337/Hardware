#include "my_font.h"
#include "main.h"
//extern uint16_t my_font_pixels[2048];
//extern my_font_char_info_t  my_font_chars[MY_FONT_CHAR_COUNT];

extern uint16_t bold_font_pixels[1024];
extern bold_font_char_info_t  bold_font_chars[BOLD_FONT_CHAR_COUNT];
bold_font_char_info_t *find_char(uint16_t unicode)
{
    for (uint32_t i = 0; i < BOLD_FONT_CHAR_COUNT; i++)
    {
        if (bold_font_chars[i].unicode == unicode)
        {
            return &bold_font_chars[i];
        }
    }
    return NULL; // 未找到
}



			
uint16_t *piexls(uint16_t unicode)
{
    uint16_t *p;
    p = (uint16_t *)&bold_font_pixels[unicode];
    return p;
}

