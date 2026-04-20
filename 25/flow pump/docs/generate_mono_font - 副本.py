#!/usr/bin/env python3
"""
单色点阵汉字库生成器（1bpp，适合 OLED/黑白屏）
功能：
  - 从字符列表文件读取汉字（每行一个），或自动生成 GB2312 一级汉字（3755个）
  - 使用 FreeType 渲染为单色位图（0=背景，1=前景）
  - 支持固定尺寸（所有字符统一宽高）或实际尺寸（每个字符独立宽高）
  - 输出 .bin 文件（紧凑格式）和 .txt 表格，可选输出 C 头文件

二进制文件结构（实际尺寸模式）：
  + 文件头 (32 bytes)
    - magic: 4 bytes "MONO"
    - version: 4 bytes (1)
    - char_count: 4 bytes
    - flags: 2 bytes (bit0: 1=实际尺寸，0=固定尺寸)
    - reserved: 18 bytes
  + 索引表（每个字符 20 bytes）
    - unicode: 4 bytes
    - offset: 4 bytes (像素数据起始偏移)
    - width: 2 bytes
    - height: 2 bytes
    - pitch: 2 bytes (每行字节数 = (width+7)//8)
    - reserved: 6 bytes
  + 像素数据区
    - 每个字符连续存储 height * pitch 字节的位图数据

固定尺寸模式时，width/height 在文件头中指定，索引表中不再重复。
"""

import sys
import os
import argparse
import struct
import numpy as np
import freetype

# ---------- 配置 ----------
DEFAULT_FONT = "C:/Windows/Fonts/msyh.ttc"  # 微软雅黑
DEFAULT_SIZE = 16            # 字号（像素）
DEFAULT_FIXED_W = 16         # 固定画布宽度（0表示实际尺寸）
DEFAULT_FIXED_H = 16         # 固定画布高度（0表示实际尺寸）
DEFAULT_BOLD = 0             # 加粗半径（0 不加粗）
DEFAULT_INVERT = False       # 是否反转颜色（1=前景，0=背景）

# ---------- 辅助函数 ----------
def dilate_bitmap(bitmap, radius):
    """对二值位图进行膨胀（加粗）"""
    if radius == 0:
        return bitmap
    h, w = bitmap.shape
    dilated = np.zeros_like(bitmap)
    for y in range(h):
        for x in range(w):
            if bitmap[y, x]:
                ymin = max(0, y - radius)
                ymax = min(h, y + radius + 1)
                xmin = max(0, x - radius)
                xmax = min(w, x + radius + 1)
                dilated[ymin:ymax, xmin:xmax] = 1
    return dilated

def render_char_bitmap(ch, face, font_size):
    """渲染单个字符为二值位图（1=前景）"""
    face.set_char_size(font_size * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    glyph = face.glyph
    bitmap = glyph.bitmap
    width = bitmap.width
    height = bitmap.rows
    left = glyph.bitmap_left
    top = glyph.bitmap_top

    if width == 0 or height == 0:
        return np.zeros((1, 1), dtype=np.uint8), 1, 1, left, top

    pitch = (width + 7) // 8
    buffer_data = bitmap.buffer
    if buffer_data is None:
        return np.zeros((height, width), dtype=np.uint8), width, height, left, top

    try:
        buffer = np.frombuffer(buffer_data, dtype=np.uint8)
    except TypeError:
        buffer = np.array(buffer_data, dtype=np.uint8)

    bitmap_data = np.zeros((height, width), dtype=np.uint8)
    for row in range(height):
        row_start = row * pitch
        for col in range(width):
            byte_idx = row_start + (col // 8)
            if byte_idx < len(buffer):
                bit = 7 - (col % 8)
                if (buffer[byte_idx] >> bit) & 1:
                    bitmap_data[row, col] = 1
    return bitmap_data, width, height, left, top

def pack_bitmap(bitmap):
    """将二值位图打包为字节数组（每行按字节对齐，高位在左）"""
    h, w = bitmap.shape
    pitch = (w + 7) // 8
    packed = bytearray(h * pitch)
    for y in range(h):
        for x in range(w):
            if bitmap[y, x]:
                byte_idx = y * pitch + (x // 8)
                bit = 7 - (x % 8)
                packed[byte_idx] |= (1 << bit)
    return packed, pitch

def render_char_fixed(ch, face, font_size, fixed_w, fixed_h, bold_radius):
    """渲染字符并居中放置到固定画布，返回打包后的字节数据及实际宽高"""
    try:
        bitmap, w, h, left, top = render_char_bitmap(ch, face, font_size)
        if bold_radius > 0:
            bitmap = dilate_bitmap(bitmap, bold_radius)
    except Exception as e:
        print(f"  警告：字符 '{ch}' 渲染失败: {e}")
        # 返回全背景画布
        canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
        packed, pitch = pack_bitmap(canvas)
        return packed, fixed_w, fixed_h

    canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
    offset_x = max(0, (fixed_w - w) // 2)
    offset_y = max(0, (fixed_h - h) // 2)

    for y in range(min(h, fixed_h - offset_y)):
        for x in range(min(w, fixed_w - offset_x)):
            if bitmap[y, x]:
                canvas[offset_y + y, offset_x + x] = 1

    packed, pitch = pack_bitmap(canvas)
    return packed, fixed_w, fixed_h

def render_char_actual(ch, face, font_size, bold_radius, invert):
    """渲染字符为实际尺寸，返回打包后的字节数据及宽高"""
    try:
        bitmap, w, h, left, top = render_char_bitmap(ch, face, font_size)
        if bold_radius > 0:
            bitmap = dilate_bitmap(bitmap, bold_radius)
    except Exception as e:
        print(f"  警告：字符 '{ch}' 渲染失败: {e}")
        # 返回 1x1 空白
        packed = bytearray(1)
        return packed, 1, 1

    if invert:
        bitmap = 1 - bitmap
    packed, pitch = pack_bitmap(bitmap)
    return packed, w, h

def generate_gb2312_level1_chars():
    """生成 GB2312 一级汉字列表（3755个）"""
    chars = []
    for row in range(0xB0, 0xD8):
        for col in range(0xA1, 0xFF):
            try:
                gb_bytes = bytes([row, col])
                ch = gb_bytes.decode('gb2312')
                chars.append(ch)
            except:
                continue
    return chars

# ---------- 主函数 ----------
def generate_font_bin(char_list_file, font_path, font_size, fixed_w, fixed_h,
                      bold_radius, invert, output_bin, output_txt=None, output_header=None,
                      encoding='utf-8'):
    """
    生成单色点阵字库二进制文件
    """
    # 1. 读取字符列表
    if char_list_file and os.path.exists(char_list_file):
        with open(char_list_file, 'r', encoding=encoding) as f:
            chars = [line.strip() for line in f if line.strip()]
        print(f"从文件读取 {len(chars)} 个字符")
    else:
        print("未提供字符列表文件，自动生成 GB2312 一级汉字（3755个）")
        chars = generate_gb2312_level1_chars()
        print(f"生成 {len(chars)} 个汉字")

    # 2. 初始化 FreeType
    try:
        face = freetype.Face(font_path)
    except Exception as e:
        print(f"无法打开字体文件: {font_path}\n{e}")
        sys.exit(1)

    # 3. 确定模式
    use_fixed = (fixed_w > 0 and fixed_h > 0)
    print(f"模式: {'固定尺寸' if use_fixed else '实际尺寸'}")
    if use_fixed:
        print(f"固定画布: {fixed_w}x{fixed_h}")
    else:
        print("每个字符使用实际渲染尺寸")

    # 4. 渲染所有字符
    print(f"开始渲染（字号={font_size}，加粗半径={bold_radius}，反转={invert}）...")
    pixel_data = bytearray()
    index_entries = []  # (unicode, offset, width, height, pitch)

    for i, ch in enumerate(chars):
        print(f"  进度: {i+1}/{len(chars)} - {ch}", end='\r')
        sys.stdout.flush()
        if use_fixed:
            packed, w, h = render_char_fixed(ch, face, font_size, fixed_w, fixed_h, bold_radius)
            pitch = (fixed_w + 7) // 8
        else:
            packed, w, h = render_char_actual(ch, face, font_size, bold_radius, invert)
            pitch = (w + 7) // 8
        offset = len(pixel_data)
        pixel_data.extend(packed)
        index_entries.append((ord(ch), offset, w, h, pitch))

    print(f"\n渲染完成。总像素数据大小: {len(pixel_data)} 字节")

    # 5. 构建二进制文件
    magic = b'MONO'
    version = 1
    char_count = len(chars)
    flags = 0
    if not use_fixed:
        flags |= 0x0001  # bit0: 1=实际尺寸
    reserved_header = b'\x00' * 18
    header = struct.pack('<4sIIH18s', magic, version, char_count, flags, reserved_header)

    # 索引表（实际尺寸时每个条目20字节，固定尺寸时可省略 width/height/pitch 但为统一仍保留）
    index_table = bytearray()
    for unicode_val, offset, w, h, pitch in index_entries:
        index_table.extend(struct.pack('<IIHHH6s', unicode_val, offset, w, h, pitch, b'\x00\x00\x00\x00\x00\x00'))

    # 最终 bin = header + index_table + pixel_data
    bin_data = header + index_table + pixel_data

    # 写入 .bin 文件
    with open(output_bin, 'wb') as f:
        f.write(bin_data)
    print(f"二进制文件已保存: {output_bin} (大小: {len(bin_data)} 字节)")

    # 6. 生成 .txt 表格
    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + '.txt'
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("单色点阵汉字库索引表\n")
        f.write("=" * 90 + "\n")
        f.write(f"{'序号':<6} {'字符':<4} {'Unicode':<10} {'偏移(Hex)':<12} {'宽度':<6} {'高度':<6} {'行字节':<6}\n")
        f.write("-" * 90 + "\n")
        for i, (ch, (unicode_val, offset, w, h, pitch)) in enumerate(zip(chars, index_entries)):
            f.write(f"{i:<6} {ch:<4} 0x{unicode_val:08X}   0x{offset:08X}     {w:<6} {h:<6} {pitch:<6}\n")
        f.write("=" * 90 + "\n")
        f.write(f"总字符数: {len(chars)}\n")
        if use_fixed:
            f.write(f"固定尺寸: {fixed_w}x{fixed_h}\n")
        else:
            f.write("实际尺寸模式\n")
        f.write(f"像素数据总字节: {len(pixel_data)}\n")
    print(f"表格文件已保存: {output_txt}")

    # 7. 可选生成 C 头文件
    if output_header:
        with open(output_header, 'w', encoding='utf-8') as f:
            f.write(f"""// 自动生成的单色字库数据结构定义
#ifndef FONT_MONO_H
#define FONT_MONO_H

#include <stdint.h>

// 文件头结构
typedef struct {{
    uint8_t  magic[4];      // "MONO"
    uint32_t version;       // 版本号
    uint32_t char_count;    // 字符总数
    uint16_t flags;         // 标志位 (bit0: 1=实际尺寸)
    uint8_t  reserved[18];
}} font_header_t;

// 索引条目结构（实际尺寸模式下每个字符独立宽高）
typedef struct {{
    uint32_t unicode;       // Unicode 码点
    uint32_t offset;        // 像素数据偏移（相对于文件头后）
    uint16_t width;         // 字符宽度（像素）
    uint16_t height;        // 字符高度（像素）
    uint16_t pitch;         // 每行字节数 = (width+7)/8
    uint8_t  reserved[6];
}} font_index_t;

#endif
""")
        print(f"C头文件已保存: {output_header}")

def main():
    parser = argparse.ArgumentParser(description='单色点阵汉字库生成器（1bpp，适合 OLED/黑白屏）')
    parser.add_argument('-c', '--char-list', help='字符列表文件（每行一个汉字），若不提供则自动生成GB2312一级汉字')
    parser.add_argument('-f', '--font', default=DEFAULT_FONT, help=f'TrueType字体文件路径（默认: {DEFAULT_FONT}）')
    parser.add_argument('-s', '--size', type=int, default=DEFAULT_SIZE, help=f'字号（像素，默认: {DEFAULT_SIZE}）')
    parser.add_argument('-W', '--fixed-width', type=int, default=DEFAULT_FIXED_W, help=f'固定画布宽度（0表示实际尺寸，默认: {DEFAULT_FIXED_W}）')
    parser.add_argument('-H', '--fixed-height', type=int, default=DEFAULT_FIXED_H, help=f'固定画布高度（0表示实际尺寸，默认: {DEFAULT_FIXED_H}）')
    parser.add_argument('-b', '--bold', type=int, default=DEFAULT_BOLD, help=f'加粗半径（默认: {DEFAULT_BOLD}，0为不加粗）')
    parser.add_argument('-i', '--invert', action='store_true', help='反转颜色（默认前景1背景0，反转后前景0背景1）')
    parser.add_argument('-o', '--output', default='font_mono.bin', help='输出二进制文件名（默认: font_mono.bin）')
    parser.add_argument('-t', '--txt', help='输出表格文件名（默认与bin同名但扩展名.txt）')
    parser.add_argument('--header', help='输出C头文件名（可选）')
    parser.add_argument('--encoding', default='utf-8', help='字符列表文件编码（默认utf-8）')
    args = parser.parse_args()

    generate_font_bin(
        char_list_file=args.char_list,
        font_path=args.font,
        font_size=args.size,
        fixed_w=args.fixed_width,
        fixed_h=args.fixed_height,
        bold_radius=args.bold,
        invert=args.invert,
        output_bin=args.output,
        output_txt=args.txt,
        output_header=args.header,
        encoding=args.encoding
    )

if __name__ == '__main__':
    main()

