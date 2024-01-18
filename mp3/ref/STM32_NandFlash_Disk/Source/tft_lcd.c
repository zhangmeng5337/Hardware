/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* �ļ���: tft_lcd.c
* ���ݼ���: ��ģ�����TFT LCD��ʾ��������������
*
*	����������������TFT��ʾ��������оƬΪ SPFD5420A���ֱ���Ϊ400x240
*	����оƬ�ķ��ʵ�ַΪ
*		(1) 0x60000000
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-01-02 armfly  �������ļ�
*
*/

#include "stm32f10x.h"
#include <stdio.h>
#include "systick.h"
#include "tft_lcd.h"

#include "fonts_ascii16x24.h"

typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
}
LCD_TypeDef;

/* ����LCD�������ķ��ʵ�ַ */
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0C000000))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

static __IO uint16_t s_TextColor = 0x0000;
static __IO uint16_t s_BackColor = 0xFFFF;

/* armfly���͸����־
	s_Transparent = 1 ��ʾ��ʾ����ʱ�����޸ı�����ɫ
*/
static __IO uint8_t s_Transparent = 0;

/*******************************************************************************
*	������: LCD_Init
*	��  ��: ��
*	��  ��: ��
*	��  ��: ��ʼ��TFT��ʾ��
*/
void LCD_Init(void)
{
	/* ����LCD���ƿ���GPIO */
	LCD_CtrlLinesConfig();

	/* ����FSMC�ӿڣ��������� */
	LCD_FSMCConfig();

	/* FSMC���ú������ӳٲ��ܷ��������豸  */
	DelayMS(20);

	/* ��ʼ��LCD��дLCD�Ĵ����������� */
	LCD_WriteReg(0x0000, 0x0000);
	LCD_WriteReg(0x0001, 0x0100);
	LCD_WriteReg(0x0002, 0x0100);

	/*
		R003H �Ĵ����ܹؼ��� Entry Mode ��������ɨ�跽��
		�μ���SPFD5420A.pdf ��15ҳ

		240x400��Ļ��������(px,py)����:
		    R003 = 0x1018                  R003 = 0x1008
		  -------------------          -------------------
		 |(0,0)              |        |(0,0)              |
		 |                   |        |
		 |  ^           ^    |        |   ^           ^   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |  ------>  |    |        |   | <------   |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |                   |        |
		 |                   |        |                   |
		 |      (x=239,y=399)|        |      (x=239,y=399)|
		 |-------------------|        |-------------------|
		 |                   |        |                   |
		  -------------------          -------------------

		���հ�����������LCD�ķ����������������������ɨ�跽�����£�(����ͼ��1���Ǻ�)
		 --------------------------------
		|  |(0,0)                        |
		|  |     --------->              |
		|  |         |                   |
		|  |         |                   |
		|  |         |                   |
		|  |         V                   |
		|  |     --------->              |
		|  |                    (399,239)|
		 --------------------------------

		��������(x,y) �����������ת����ϵ
		x = 399 - py;
		y = px;

		py = 399 - x;
		px = y;

	*/
	LCD_WriteReg(0x0003, 0x1018); /* 0x1018 1030 */

	LCD_WriteReg(0x0008, 0x0808);
	LCD_WriteReg(0x0009, 0x0001);
	LCD_WriteReg(0x000B, 0x0010);
	LCD_WriteReg(0x000C, 0x0000);
	LCD_WriteReg(0x000F, 0x0000);
	LCD_WriteReg(0x0007, 0x0001);
	LCD_WriteReg(0x0010, 0x0013);
	LCD_WriteReg(0x0011, 0x0501);
	LCD_WriteReg(0x0012, 0x0300);
	LCD_WriteReg(0x0020, 0x021E);
	LCD_WriteReg(0x0021, 0x0202);
	LCD_WriteReg(0x0090, 0x8000);
	LCD_WriteReg(0x0100, 0x17B0);
	LCD_WriteReg(0x0101, 0x0147);
	LCD_WriteReg(0x0102, 0x0135);
	LCD_WriteReg(0x0103, 0x0700);
	LCD_WriteReg(0x0107, 0x0000);
	LCD_WriteReg(0x0110, 0x0001);
	LCD_WriteReg(0x0210, 0x0000);
	LCD_WriteReg(0x0211, 0x00EF);
	LCD_WriteReg(0x0212, 0x0000);
	LCD_WriteReg(0x0213, 0x018F);
	LCD_WriteReg(0x0280, 0x0000);
	LCD_WriteReg(0x0281, 0x0004);
	LCD_WriteReg(0x0282, 0x0000);
	LCD_WriteReg(0x0300, 0x0101);
	LCD_WriteReg(0x0301, 0x0B2C);
	LCD_WriteReg(0x0302, 0x1030);
	LCD_WriteReg(0x0303, 0x3010);
	LCD_WriteReg(0x0304, 0x2C0B);
	LCD_WriteReg(0x0305, 0x0101);
	LCD_WriteReg(0x0306, 0x0807);
	LCD_WriteReg(0x0307, 0x0708);
	LCD_WriteReg(0x0308, 0x0107);
	LCD_WriteReg(0x0309, 0x0105);
	LCD_WriteReg(0x030A, 0x0F04);
	LCD_WriteReg(0x030B, 0x0F00);
	LCD_WriteReg(0x030C, 0x000F);
	LCD_WriteReg(0x030D, 0x040F);
	LCD_WriteReg(0x030E, 0x0300);
	LCD_WriteReg(0x030F, 0x0701);
	LCD_WriteReg(0x0400, 0x3500);
	LCD_WriteReg(0x0401, 0x0001);
	LCD_WriteReg(0x0404, 0x0000);
	LCD_WriteReg(0x0500, 0x0000);
	LCD_WriteReg(0x0501, 0x0000);
	LCD_WriteReg(0x0502, 0x0000);
	LCD_WriteReg(0x0503, 0x0000);
	LCD_WriteReg(0x0504, 0x0000);
	LCD_WriteReg(0x0505, 0x0000);
	LCD_WriteReg(0x0600, 0x0000);
	LCD_WriteReg(0x0606, 0x0000);
	LCD_WriteReg(0x06F0, 0x0000);
	LCD_WriteReg(0x07F0, 0x5420);
	LCD_WriteReg(0x07DE, 0x0000);
	LCD_WriteReg(0x07F2, 0x00DF);
	LCD_WriteReg(0x07F3, 0x0810);
	LCD_WriteReg(0x07F4, 0x0077);
	LCD_WriteReg(0x07F5, 0x0021);
	LCD_WriteReg(0x07F0, 0x0000);
	LCD_WriteReg(0x0007, 0x0173);

	/* ������ʾ���� WINDOWS */
	LCD_WriteReg(0x0210, 0);	/* ˮƽ��ʼ��ַ */
	LCD_WriteReg(0x0211, 239);	/* ˮƽ�������� */
	LCD_WriteReg(0x0212, 0);	/* ��ֱ��ʼ��ַ */
	LCD_WriteReg(0x0213, 399);	/* ��ֱ������ַ */
}

/*******************************************************************************
*	������: LCD_SetTextColor
*	��  ��: Color : �ı���ɫ
*	��  ��: ��
*	��  ��: �����ı���ɫ��������ȫ������s_TextColor
*/
void LCD_SetTextColor(__IO uint16_t Color)
{
	s_TextColor = Color;
}

/*******************************************************************************
*	������: LCD_SetBackColor
*	��  ��: Color : ������ɫ
*	��  ��: ��
*	��  ��: ���ñ�����ɫ��������ȫ������ s_BackColor
*/
void LCD_SetBackColor(__IO uint16_t Color)
{
	s_BackColor = Color;
}

/*******************************************************************************
*	������: LCD_ClearLine
*	��  ��: Line : �ı��к�0 - 9
*	��  ��: ��
*	��  ��: ���ѡ�����ı���
*/
void LCD_ClearLine(uint8_t Line)
{
	LCD_DisplayString(0, Line, "                    ");
}

/*******************************************************************************
*	������: LCD_Clear
*	��  ��: Color : ����ɫ
*	��  ��: ��
*	��  ��: �����������ɫֵ����
*/
void LCD_Clear(uint16_t Color)
{
	uint32_t index = 0;

	LCD_SetCursor(0, 0);	/* ���ù��λ�� */

	LCD_WriteRAM_Prepare(); 	/* ׼��д�Դ� */

	for (index = 0; index < 400 * 240; index++)
	{
		LCD->LCD_RAM = Color;
	}
}

/*******************************************************************************
*	������: LCD_SetCursor
*	��  ��: Xpos : X����; Ypos: Y����
*	��  ��: ��
*	��  ��: ���ù��λ��
*/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	/*
		px��py ���������꣬ x��y����������
		ת����ʽ:
		py = 399 - x;
		px = y;
	*/

	LCD_WriteReg(0x0200, Ypos);  		/* px */
	LCD_WriteReg(0x0201, 399 - Xpos);	/* py */
}

/*******************************************************************************
*	������: LCD_SetTransparent
*	��  ��: _mode ; ͸����־��1��ʾ͸�� 0 ��ʾ��͸��
*	��  ��: ��
*	��  ��: ���ù��λ��
*/
void LCD_SetTransparent(uint8_t _mode)
{
	if (_mode == 0)
	{
		s_Transparent = 0;	/* ����Ϊ������͸�� */
	}
	else
	{
		s_Transparent = 1;	/* ����Ϊ����͸�� */
	}
}

/*******************************************************************************
*	������: LCD_DrawChar
*	��  ��:
*		Xpos : X����;
*		Ypos: Y���ꣻ
*		c : ָ���ַ������ָ��
*	��  ��: ��
*	��  ��: ��LCD����ʾһ���ַ�(16x24)
*/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
	uint32_t index = 0, i = 0;
	uint8_t Yaddress;

	Yaddress = Ypos;

	LCD_SetCursor(Xpos, Ypos);

	if (s_Transparent == 0)
	{
		for (index = 0; index < 24; index++)	/* �ַ��߶� */
		{
			LCD_WriteRAM_Prepare();
			for (i = 0; i < 16; i++)	/* �ַ���� */
			{
				if ((c[index] & (1 << i)) == 0x00)
				{
					LCD_WriteRAM(s_BackColor);
				}
				else
				{
					LCD_WriteRAM(s_TextColor);
				}
			}
			Yaddress++;
			LCD_SetCursor(Xpos, Yaddress);
		}
	}
	else	/* armfly ��ӣ�ʵ�����ֵ�����ͼƬ�ϵĹ��� */
	{
		for (index = 0; index < 24; index++)	/* �ַ��߶� */
		{
			uint16_t x = Xpos;

			for (i = 0; i < 16; i++)	/* �ַ���� */
			{
				if ((c[index] & (1 << i)) != 0x00)
				{
					LCD_SetCursor(x, Yaddress);

					LCD->LCD_REG = 0x202;
					LCD->LCD_RAM = s_TextColor;
				}
				x++;
			}
			Yaddress++;
		}
	}
}

/*******************************************************************************
*	������: LCD_DisplayChar
*	��  ��:
*		Xpos : X����);
*		Ypos : Y����)
*		Ascii : �ַ���ASCII���룬��0x20��0x7E֮��
*	��  ��: ��
*	��  ��: ��LCD����ʾһ���ַ�(16x24)
*/
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
	Ascii -= ' ';
	LCD_DrawChar(Xpos, Ypos, &Ascii_16x24[Ascii * 24]);
}

/*******************************************************************************
*	������: LCD_DisplayString
*	��  ��:
*		Xpos : X���� 0 - 399
*		Ypos : Y���� 0 - 239
*		ptr  : �ַ���ָ�룻
*	��  ��: ��
*	��  ��: ��LCDָ���ı�����ʾһ���ַ���
*/
void LCD_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;

	while ((*ptr != 0) & (i < 30))
	{
		LCD_DisplayChar(Xpos, Ypos, *ptr);		/* ��LCD����ʾ1���ַ� */
		Xpos += 16;									/* �е�ַ��16 */
		ptr++;			/* ָ��ָ����һ���ַ� */
		i++;
	}
}

/*******************************************************************************
*	������: LCD_SetDisplayWindow
*	��  ��:
*		Xpos : ��ʾ�к�
*		Ypos  : �ַ���ָ�룻
*		Height: ���ڸ߶�
*		Width : ���ڿ��
*	��  ��: ��
*	��  ��: ������ʾ����
*/
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
	/* ˮƽ��ʼ��ַ */
	if (Xpos >= Height)
	{
		LCD_WriteReg(R80, (Xpos - Height + 1));
	}
	else
	{
		LCD_WriteReg(R80, 0);
	}

	/* ˮƽ������ַ */
	LCD_WriteReg(R81, Xpos);

	/* ��ֱ��ʼ��ַ */
	if (Ypos >= Width)
	{
		LCD_WriteReg(R82, (Ypos - Width + 1));
	}
	else
	{
		LCD_WriteReg(R82, 0);
	}

	/* ��ֱ������ַ */
	LCD_WriteReg(R83, Ypos);

	LCD_SetCursor(Xpos, Ypos);
}

/*******************************************************************************
*	������: LCD_WindowModeDisable
*	��  ��: ��
*	��  ��: ��
*	��  ��: �˳�������ʾģʽ����Ϊȫ����ʾģʽ
*/
void LCD_WindowModeDisable(void)
{
	LCD_SetDisplayWindow(239, 399, 240, 400);
	LCD_WriteReg(R3, 0x1018);	/* ����Ĵ����ı�ɨ�跽�� */
}

/*******************************************************************************
*	������: LCD_DrawLine
*	��  ��: Xpos ��X����
*			YPos ��Y����
*			Length    ������
*			Direction ������(Horizontal��Vertical)
*	��  ��: ��
*	��  ��: ��LCD�ϻ�1����
*/
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
	uint32_t i = 0;

	/* �����ߵ���ʼ���� */
	LCD_SetCursor(Xpos, Ypos);

	if (Direction == Horizontal)	/* ˮƽ���� */
	{
		LCD_WriteRAM_Prepare();
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM(s_TextColor);
		}
	}
	else	/* ��ֱ���� */
	{
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM_Prepare();
			LCD_WriteRAM(s_TextColor);
			Ypos++;
			LCD_SetCursor(Xpos, Ypos);
		}
	}
}

/*******************************************************************************
*	������: LCD_DrawPoints
*	��  ��: _points ������������
*			_offset : ƫ������
*			_Size : �����
*			_Color ����ɫ
*	��  ��: ��
*	��  ��: ��LCD�ϻ�һ���
*/
void LCD_DrawPoints(POINT_T _offset, POINT_T *_points, uint16_t _Size, uint16_t _Color)
{
#if 1
	uint16_t i;

	for (i = 0 ; i < _Size; i++)
	{
		/* �����ߵ���ʼ���� */
		LCD_SetCursor(_points[i].x + _offset.x, _points[i].y + _offset.y);
		/* �޸��Դ����� */
		LCD_WriteReg(0x202, _Color);
	}
#else
	/* �������Ż���Ĵ��� */
	for (i = 0 ; i < _Size; i++)
	{
		LCD->LCD_REG = 0x0200;
		LCD->LCD_RAM = _points[i]->x;

		LCD->LCD_REG = 0x0201;
		LCD->LCD_RAM = _points[i]->y;

		LCD->LCD_REG = 0x0202;
		LCD->LCD_RAM = _Color;
	}
#endif

}

/*******************************************************************************
*	������: LCD_DrawRect
*	��  ��: Xpos ��X����
*			YPos ��Y����
*			Height ���߶�
*			Width  �����
*	��  ��: ��
*	��  ��: ��LCD�ϻ�һ�����ο�
*/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
	/*
	 ---------------->---
	|(Xpos��Ypos)        |
	V                    V  Height
	|                    |
	 ---------------->---
		  Width
	*/

	LCD_DrawLine(Xpos, Ypos, Width, Horizontal);  				/* �� */
	LCD_DrawLine(Xpos, Ypos + Height, Width, Horizontal);		/* �� */

	LCD_DrawLine(Xpos, Ypos, Height, Vertical);					/* �� */
	LCD_DrawLine(Xpos + Width, Ypos, Height, Vertical);	/* �� */
}

/*******************************************************************************
*	������: LCD_DrawCircle
*	��  ��: Xpos ��X����
*			Radius ��Բ�İ뾶
*	��  ��: ��
*	��  ��: ��LCD�ϻ�һ��Բ
*/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;

	while (CurX <= CurY)
	{
		LCD_SetCursor(Xpos + CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		LCD_WriteRAM(s_TextColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*******************************************************************************
*	������: LCD_DrawMonoPict
*	��  ��: Pict �� ͼƬ����ָ��
*	��  ��: ��
*	��  ��: ��LCD�ϻ�һ����ɫͼƬ
*/
void LCD_DrawMonoPict(const uint32_t *Pict)
{
	uint32_t index = 0, i = 0;

	LCD_SetCursor(0, 400);

	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for (index = 0; index < 2400; index++)
	{
		for (i = 0; i < 32; i++)
		{
			if ((Pict[index] & (1 << i)) == 0x00)
			{
				LCD_WriteRAM(s_BackColor);
			}
			else
			{
				LCD_WriteRAM(s_TextColor);
			}
		}
	}
}

/*******************************************************************************
*	������: LCD_WriteBMP
*	��  ��: ptr �� ͼƬ����ָ��
*	��  ��: ��
*	��  ��: ��LCD����ʾһ��BMPλͼ
*/
void LCD_WriteBMP(const uint16_t *ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* �޸�ɨ�跽�� */
	/* Set GRAM write direction and BGR = 1 */
	/* I/D=00 (Horizontal : decrement, Vertical : decrement) */
	/* AM=1 (address is updated in vertical writing direction) */
	//LCD_WriteReg(R3, 0x1008);

	LCD_WriteRAM_Prepare();

	p = ptr;
	for (index = 0; index < 400 * 240; index++)
	{
		/*
			armfly : �����Ż�, �����͵�չ��
			LCD_WriteRAM(ptr[index]);

			�˴��ɿ�����DMA����
		*/
		LCD->LCD_RAM = *p++;
	}

	/* ��ԭɨ�跽�� */
	/* Set GRAM write direction and BGR = 1 */
	/* I/D = 01 (Horizontal : increment, Vertical : decrement) */
	/* AM = 1 (address is updated in vertical writing direction) */
	//LCD_WriteReg(R3, 0x1018);
}

/*******************************************************************************
*	������: LCD_WriteReg
*	��  ��: LCD_Reg ���Ĵ�����ַ;  LCD_RegValue : �Ĵ���ֵ
*	��  ��: ��
*	��  ��: �޸�LCD�������ļĴ�����ֵ
*/
void LCD_WriteReg(__IO uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_Reg;
	/* Write 16-bit Reg */
	LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
*	������: LCD_ReadReg
*	��  ��: LCD_Reg ���Ĵ�����ַ
*	��  ��: �Ĵ�����ֵ
*	��  ��: ��LCD�������ļĴ�����ֵ
*/
uint16_t LCD_ReadReg(__IO uint16_t LCD_Reg)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD->LCD_REG = LCD_Reg;
	/* Read 16-bit Reg */
	return (LCD->LCD_RAM);
}

/*******************************************************************************
*	������: LCD_WriteRAM_Prepare
*	��  ��: ��
*	��  ��: ��
*	��  ��: д�Դ�ǰ��׼�����������Դ�Ĵ�����ַ��
*/
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = 0x202;
}

/*******************************************************************************
*	������: LCD_WriteRAM
*	��  ��: RGB_Code : ��ɫ����
*	��  ��: ��
*	��  ��: д�Դ棬�Դ��ַ�Զ�����
*/
void LCD_WriteRAM(uint16_t RGB_Code)
{
	/* Write 16-bit GRAM Reg */
	LCD->LCD_RAM = RGB_Code;
}

/*******************************************************************************
*	������: LCD_ReadRAM
*	��  ��: ��
*	��  ��: �Դ�����
*	��  ��: ���Դ棬��ַ�Զ�����
*/
uint16_t LCD_ReadRAM(void)
{
#if 0
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = 0x202; /* Select GRAM Reg */
#endif

  /* Read 16-bit Reg */
  return LCD->LCD_RAM;
}

/*******************************************************************************
*	������: LCD_DisplayOn
*	��  ��: ��
*	��  ��: ��
*	��  ��: ����ʾ
*/
void LCD_DisplayOn(void)
{
	/* Display On */
	LCD_WriteReg(R7, 0x0173); /* 262K color and display ON */
}

/*******************************************************************************
*	������: LCD_DisplayOff
*	��  ��: ��
*	��  ��: ��
*	��  ��: �ر���ʾ
*/
void LCD_DisplayOff(void)
{
	/* Display Off */
	LCD_WriteReg(R7, 0x0);
}

/*******************************************************************************
*	������: LCD_CtrlLinesConfig
*	��  ��: ��
*	��  ��: ��
*	��  ��: ����LCD���ƿ��ߣ�FSMC�ܽ�����Ϊ���ù���
*/
static void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ�� FSMC, GPIOD, GPIOE, GPIOF, GPIOG �� AFIO ʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* ���� PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) Ϊ����������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
                                GPIO_Pin_15; // | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* ���� PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) Ϊ����������� */
  /* PE3,PE4 ����A19, A20, STM32F103ZE-EK(REV 2.0)����ʹ�� */
  /* PE5,PE6 ����A19, A20, STM32F103ZE-EK(REV 2.0)����ʹ�� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* ���� PF.00(A0 (RS))  Ϊ����������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ���� PG.12(NE4 (LCD/CS)) Ϊ����������� - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/*******************************************************************************
*	������: LCD_FSMCConfig
*	��  ��: ��
*	��  ��: ��
*	��  ��: ����FSMC���ڷ���ʱ��
*/
static void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;

  /*-- FSMC Configuration ------------------------------------------------------*/
  /*----------------------- SRAM Bank 4 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 1;
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 2;
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;

  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* - BANK 3 (of NOR/SRAM Bank 0~3) is enabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

