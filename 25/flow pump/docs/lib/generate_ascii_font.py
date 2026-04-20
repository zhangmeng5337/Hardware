#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成常用符号和英文字母的点阵字库（单色 1bpp）
默认生成 ASCII 32~126（共95个字符）
输出二进制 .bin 文件及辅助表格
"""

import sys
import os
import argparse
import struct
import numpy as np
import freetype

# ---------- 默认参数 ----------
DEFAULT_FONT = "C:/Windows/Fonts/consola.ttf"  # 等宽字体，也可用 arial.ttf
DEFAULT_SIZE = 16           # 字号（像素）
DEFAULT_FIXED_W = 16        # 固定宽度（0表示实际尺寸）
DEFAULT_FIXED_H = 16        # 固定高度
DEFAULT_BOLD = 0
DEFAULT_INVERT = False

# ---------- 图像处理函数（复用）----------
def dilate_bitmap(bitmap, radius):
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
    try:
        bitmap, w, h, _, _ = render_char_bitmap(ch, face, font_size)
        if bold_radius > 0:
            bitmap = dilate_bitmap(bitmap, bold_radius)
    except Exception as e:
        print(f"  警告：字符 '{ch}' 渲染失败: {e}")
        canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
        packed, _ = pack_bitmap(canvas)
        return packed, fixed_w, fixed_h

    canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
    offset_x = max(0, (fixed_w - w) // 2)
    offset_y = max(0, (fixed_h - h) // 2)

    for y in range(min(h, fixed_h - offset_y)):
        for x in range(min(w, fixed_w - offset_x)):
            if bitmap[y, x]:
                canvas[offset_y + y, offset_x + x] = 1

    packed, _ = pack_bitmap(canvas)
    return packed, fixed_w, fixed_h

def render_char_actual(ch, face, font_size, bold_radius, invert):
    try:
        bitmap, w, h, _, _ = render_char_bitmap(ch, face, font_size)
        if bold_radius > 0:
            bitmap = dilate_bitmap(bitmap, bold_radius)
    except Exception as e:
        print(f"  警告：字符 '{ch}' 渲染失败: {e}")
        packed = bytearray(1)
        return packed, 1, 1

    if invert:
        bitmap = 1 - bitmap
    packed, _ = pack_bitmap(bitmap)
    return packed, w, h

# ---------- ASCII 字符列表 ----------
def get_ascii_chars(start=32, end=126):
    """返回 ASCII 可打印字符列表（默认 32~126）"""
    return [chr(i) for i in range(start, end+1)]

# ---------- 主生成函数 ----------
def generate_font_bin(chars, font_path, font_size, fixed_w, fixed_h,
                      bold_radius, invert, output_bin, output_txt=None,
                      offset_table=None, output_header=None):
    """
    根据给定的字符列表生成字库
    """
    print(f"字符数量: {len(chars)}")

    # 加载字体
    try:
        face = freetype.Face(font_path)
    except Exception as e:
        print(f"无法打开字体文件: {font_path}\n{e}")
        sys.exit(1)

    use_fixed = (fixed_w > 0 and fixed_h > 0)
    print(f"模式: {'固定尺寸' if use_fixed else '实际尺寸'}")
    if use_fixed:
        print(f"固定画布: {fixed_w}x{fixed_h}")
    else:
        print("每个字符使用实际尺寸")

    print(f"渲染参数: 字号={font_size}, 加粗半径={bold_radius}, 反色={invert}")

    pixel_data = bytearray()
    index_entries = []   # (unicode, offset_in_pixel_data, width, height, pitch)

    total = len(chars)
    for i, ch in enumerate(chars):
        if (i+1) % 20 == 0:
            print(f"  进度: {i+1}/{total}")
        if use_fixed:
            packed, w, h = render_char_fixed(ch, face, font_size, fixed_w, fixed_h, bold_radius)
            pitch = (fixed_w + 7) // 8
        else:
            packed, w, h = render_char_actual(ch, face, font_size, bold_radius, invert)
            pitch = (w + 7) // 8
        offset = len(pixel_data)
        pixel_data.extend(packed)
        index_entries.append((ord(ch), offset, w, h, pitch))
    print(f"渲染完成，像素数据大小: {len(pixel_data)} 字节")

    # 构建二进制文件
    magic = b'MONO'
    version = 1
    char_count = len(chars)
    flags = 0 if use_fixed else 1
    reserved_header = b'\x00' * 18
    header = struct.pack('<4sIIH18s', magic, version, char_count, flags, reserved_header)

    index_table = bytearray()
    for unicode_val, offset, w, h, pitch in index_entries:
        index_table.extend(struct.pack('<IIHHH6s', unicode_val, offset, w, h, pitch, b'\x00\x00\x00\x00\x00\x00'))

    bin_data = header + index_table + pixel_data
    with open(output_bin, 'wb') as f:
        f.write(bin_data)
    print(f"二进制字库已保存: {output_bin} (总大小 {len(bin_data)} 字节)")

    # 详细表格
    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + '.txt'
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("ASCII 点阵字库索引表\n")
        f.write("=" * 90 + "\n")
        f.write(f"{'序号':<6} {'字符':<4} {'ASCII':<6} {'Unicode':<10} {'偏移(像素区)':<12} {'宽度':<6} {'高度':<6} {'行字节':<6}\n")
        f.write("-" * 90 + "\n")
        for i, (ch, (unicode_val, offset, w, h, pitch)) in enumerate(zip(chars, index_entries)):
            ascii_val = ord(ch)
            f.write(f"{i:<6} {ch:<4} {ascii_val:<6} 0x{unicode_val:08X}   0x{offset:08X}     {w:<6} {h:<6} {pitch:<6}\n")
        f.write("=" * 90 + "\n")
        f.write(f"总字符数: {len(chars)}\n")
        f.write(f"固定尺寸: {'是' if use_fixed else '否'}\n")
        if use_fixed:
            f.write(f"尺寸: {fixed_w}x{fixed_h}\n")
        f.write(f"像素数据总字节: {len(pixel_data)}\n")
    print(f"详细表格已保存: {output_txt}")

    # 简洁偏移对照表（可选）
    if offset_table:
        base_offset = len(header) + len(index_table)
        with open(offset_table, 'w', encoding='utf-8') as f:
            f.write("# 字符 绝对偏移(十六进制)  (从文件头开始)\n")
            for ch, (unicode_val, offset, _, _, _) in zip(chars, index_entries):
                abs_offset = base_offset + offset
                f.write(f"{ch} 0x{abs_offset:08X}\n")
        print(f"偏移对照表已保存: {offset_table}")

    # C 头文件（可选）
    if output_header:
        with open(output_header, 'w', encoding='utf-8') as f:
            f.write("""// 自动生成的 ASCII 字库数据结构定义
#ifndef ASCII_FONT_H
#define ASCII_FONT_H

#include <stdint.h>

typedef struct {
    uint8_t  magic[4];
    uint32_t version;
    uint32_t char_count;
    uint16_t flags;
    uint8_t  reserved[18];
} font_header_t;

typedef struct {
    uint32_t unicode;
    uint32_t offset;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t  reserved[6];
} font_index_t;

#endif
""")
        print(f"C头文件已保存: {output_header}")

# ---------- 命令行入口 ----------
def main():
    parser = argparse.ArgumentParser(description='生成 ASCII 字符点阵字库 (.bin)')
    parser.add_argument('-c', '--char-list', help='字符列表文件（每行一个字符），若不提供则生成 ASCII 32~126')
    parser.add_argument('-f', '--font', default=DEFAULT_FONT, help=f'字体文件路径，默认 {DEFAULT_FONT}')
    parser.add_argument('-s', '--size', type=int, default=DEFAULT_SIZE, help=f'字号（像素），默认 {DEFAULT_SIZE}')
    parser.add_argument('-W', '--fixed-width', type=int, default=DEFAULT_FIXED_W, help=f'固定宽度（0=实际尺寸），默认 {DEFAULT_FIXED_W}')
    parser.add_argument('-H', '--fixed-height', type=int, default=DEFAULT_FIXED_H, help=f'固定高度（0=实际尺寸），默认 {DEFAULT_FIXED_H}')
    parser.add_argument('-b', '--bold', type=int, default=DEFAULT_BOLD, help=f'加粗半径，默认 {DEFAULT_BOLD}')
    parser.add_argument('-i', '--invert', action='store_true', help='反色')
    parser.add_argument('-o', '--output', default='ascii_font.bin', help='输出 .bin 文件名')
    parser.add_argument('-t', '--txt', help='输出详细表格文件名')
    parser.add_argument('--offset-table', help='输出简洁偏移对照表')
    parser.add_argument('--header', help='输出 C 头文件')
    args = parser.parse_args()

    # 获取字符列表
    if args.char_list:
        with open(args.char_list, 'r', encoding='utf-8') as f:
            chars = [line.strip() for line in f if line.strip()]
        print(f"从文件读取 {len(chars)} 个字符")
    else:
        chars = get_ascii_chars()
        print(f"生成默认 ASCII 32~126，共 {len(chars)} 个字符")

    generate_font_bin(
        chars=chars,
        font_path=args.font,
        font_size=args.size,
        fixed_w=args.fixed_width,
        fixed_h=args.fixed_height,
        bold_radius=args.bold,
        invert=args.invert,
        output_bin=args.output,
        output_txt=args.txt,
        offset_table=args.offset_table,
        output_header=args.header
    )

if __name__ == '__main__':
    main()
