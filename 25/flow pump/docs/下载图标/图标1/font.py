#!/usr/bin/env python3
"""
单字符串版 FreeType 转 RGB565
直接渲染给定的文字（如“你好”），生成包含所有字符拼接点阵的C头文件。
"""

import sys
import numpy as np
import freetype

def rgb888_to_rgb565(r, g, b):
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

def render_char(ch, face, font_size):
    """渲染单个字符，返回RGB565列表和宽高"""
    face.set_char_size(font_size * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_NORMAL)
    glyph = face.glyph
    bitmap = glyph.bitmap
    width = bitmap.width
    height = bitmap.rows
    left = glyph.bitmap_left
    top = glyph.bitmap_top

    # 创建画布
    canvas_h = height + top + 2
    canvas_w = width + left + 2
    canvas = np.full((canvas_h, canvas_w, 3), (0,0,0), dtype=np.uint8)

    for y in range(height):
        for x in range(width):
            gray = bitmap.buffer[y * width + x]
            if gray > 128:
                canvas[top + y, left + x] = (255,255,255)

    rgb565 = []
    for y in range(canvas_h):
        for x in range(canvas_w):
            r, g, b = canvas[y, x]
            rgb565.append(rgb888_to_rgb565(r, g, b))
    return rgb565, canvas_w, canvas_h

def main():
    if len(sys.argv) < 3:
        print("用法: python font.py <文字> <字体文件> [字号] [输出文件]")
        print("示例: python font.py \"你好\" C:/Windows/Fonts/msyh.ttc 16 hello.h")
        sys.exit(1)

    text = sys.argv[1]
    font_path = sys.argv[2]
    font_size = int(sys.argv[3]) if len(sys.argv) > 3 else 32
    output = sys.argv[4] if len(sys.argv) > 4 else "output.h"

    # 打开字体
    try:
        face = freetype.Face(font_path)
    except Exception as e:
        print(f"无法打开字体文件: {font_path}")
        print("请检查文件是否存在、路径是否正确。")
        sys.exit(1)

    # 渲染每个字符并横向拼接
    all_pixels = []
    total_width = 0
    max_height = 0
    char_data = []

    for ch in text:
        try:
            rgb, w, h = render_char(ch, face, font_size)
            char_data.append((rgb, w, h))
            total_width += w
            max_height = max(max_height, h)
        except Exception as e:
            print(f"字符 '{ch}' 渲染失败: {e}")

    if not char_data:
        print("没有成功渲染的字符，退出")
        sys.exit(1)

    # 构建最终画布
    canvas = np.full((max_height, total_width, 3), (0,0,0), dtype=np.uint8)
    pen_x = 0
    for rgb, w, h in char_data:
        # 将rgb列表还原到画布（反向转换用于显示）
        idx = 0
        for y in range(h):
            for x in range(w):
                val = rgb[idx]
                # RGB565 转回近似 RGB888
                r = ((val >> 11) & 0x1F) << 3
                g = ((val >> 5) & 0x3F) << 2
                b = (val & 0x1F) << 3
                if y < max_height and pen_x + x < total_width:
                    canvas[y, pen_x + x] = (r, g, b)
                idx += 1
        pen_x += w

    # 将最终画布转换为 RGB565 列表
    final_rgb = []
    for y in range(max_height):
        for x in range(total_width):
            r, g, b = canvas[y, x]
            final_rgb.append(rgb888_to_rgb565(r, g, b))

    # 生成C头文件
    array_name = "font_data"
    with open(output, 'w', encoding='utf-8') as f:
        f.write(f"#define {array_name.upper()}_WIDTH {total_width}\n")
        f.write(f"#define {array_name.upper()}_HEIGHT {max_height}\n")
        f.write(f"#define {array_name.upper()}_SIZE {total_width * max_height}\n")
        f.write(f"const uint16_t {array_name}[{total_width * max_height}] = {{\n")
        for i, val in enumerate(final_rgb):
            f.write(f"    0x{val:04X}")
            if i != len(final_rgb)-1:
                f.write(",")
            if (i+1) % 8 == 0:
                f.write("\n")
        f.write("\n};\n")

    print(f"转换完成！输出文件: {output}")
    print(f"拼接后尺寸: {total_width}x{max_height} 像素")

if __name__ == "__main__":
    main()

