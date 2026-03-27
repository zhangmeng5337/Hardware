#include "my_font.h"
#include "main.h"
//extern uint16_t my_font_pixels[2048];
//extern my_font_char_info_t  my_font_chars[MY_FONT_CHAR_COUNT];

extern uint16_t bold_font_pixels16[BOLD_FONT_CHAR_COUNT16x16];
extern uint16_t bold_font_pixels32[BOLD_FONT_CHAR_COUNT32x32];

extern bold_font_char_info_t  bold_font_chars16[BOLD_FONT_CHAR_COUNT16x16];
extern bold_font_char_info_t  bold_font_chars32[BOLD_FONT_CHAR_COUNT32x32];

bold_font_char_info_t *find_char(uint16_t unicode, unsigned char fontSize)
{
    if (fontSize == 16)
    {
        for (uint32_t i = 0; i < BOLD_FONT_CHAR_COUNT16x16; i++)
        {

            if (bold_font_chars16[i].unicode == unicode)
            {
                return &bold_font_chars16[i];
            }
        }
    }
    else 
    {
        for (uint32_t i = 0; i < BOLD_FONT_CHAR_COUNT32x32; i++)
        {
            if (bold_font_chars32[i].unicode == unicode)
            {
                return &bold_font_chars32[i];
            }


        }
    }

    return NULL; // 未找到
}




uint16_t *piexls(uint16_t unicode,unsigned char fontSize)
{
    uint16_t *p;
	if(fontSize == 16)
    p = (uint16_t *)&bold_font_pixels16[unicode];
	else
    p = (uint16_t *)&bold_font_pixels32[unicode];		
    return p;
}

