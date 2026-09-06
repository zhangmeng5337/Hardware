#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成实际尺寸的 ASCII 字库（紧凑排列）
每个字符的宽度等于其实际像素宽度，高度等于实际高度（通常 8-16 之间）
输出 .bin 文件（MONO 格式）
"""

import argparse
import struct
import freetype
import numpy as np

DEFAULT_FONT = "C:/Windows/Fonts/arial.ttf"
FONT_SIZE = 16   # 字号（像素）

def pack_bitmap(bitmap):
    h, w = bitmap.shape
    pitch = (w + 7) // 8
    packed = bytearray(h * pitch)
    for y in range(h):
        for x in range(w):
            if bitmap[y, x]:
                idx = y * pitch + (x // 8)
                packed[idx] |= (0x80 >> (x % 8))
    return packed, pitch, w, h

def render_char(ch, face, font_size):
    face.set_char_size(font_size * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    glyph = face.glyph
    bitmap = glyph.bitmap
    w, h = bitmap.width, bitmap.rows
    # 如果宽度为0（不可见字符）则返回 1x1 空白
    if w == 0 or h == 0:
        return bytearray(1), 1, 1, 1
    # 提取位图数据到 numpy 数组
    pitch_ft = bitmap.pitch if hasattr(bitmap, 'pitch') else (w + 7) // 8
    canvas = np.zeros((h, w), dtype=np.uint8)
    buffer = bitmap.buffer
    for y in range(h):
        for x in range(w):
            byte_idx = y * pitch_ft + (x // 8)
            bit = (buffer[byte_idx] >> (7 - (x % 8))) & 1
            if bit:
                canvas[y, x] = 1
    return pack_bitmap(canvas)

def generate_ascii_font(font_path, font_size, output_bin, output_txt=None):
    chars = [chr(i) for i in range(32, 127)]
    print(f"生成 {len(chars)} 个 ASCII 字符，字号={font_size}")

    face = freetype.Face(font_path)
    pixel_data = bytearray()
    index_entries = []   # (unicode, offset, width, height, pitch)

    for ch in chars:
        packed, pitch, w, h = render_char(ch, face, font_size)
        offset = len(pixel_data)
        pixel_data.extend(packed)
        index_entries.append((ord(ch), offset, w, h, pitch))

    char_count = len(chars)
    header = struct.pack('<4sIIH18s', b'MONO', 1, char_count, 1, b'\x00'*18)  # flags=1 表示实际尺寸
    index_table = bytearray()
    for unicode_val, offset, w, h, pitch in index_entries:
        index_table.extend(struct.pack('<IIHHH6s', unicode_val, offset, w, h, pitch, b'\x00'*6))
    bin_data = header + index_table + pixel_data
    with open(output_bin, 'wb') as f:
        f.write(bin_data)
    print(f"字库已保存: {output_bin} (大小 {len(bin_data)} 字节)")

    if output_txt:
        with open(output_txt, 'w', encoding='utf-8') as f:
            f.write("实际尺寸 ASCII 字库索引表\n")
            f.write(f"{'序号':<6} {'字符':<4} {'Unicode':<10} {'偏移(Hex)':<12} {'宽度':<6} {'高度':<6} {'pitch':<6}\n")
            for i, (ch, (unicode_val, offset, w, h, pitch)) in enumerate(zip(chars, index_entries)):
                f.write(f"{i:<6} {ch:<4} 0x{unicode_val:08X}   0x{offset:08X}     {w:<6} {h:<6} {pitch:<6}\n")
        print(f"表格已保存: {output_txt}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--font', default=DEFAULT_FONT)
    parser.add_argument('-s', '--size', type=int, default=FONT_SIZE, help='字号（像素）')
    parser.add_argument('-o', '--output', default='ascii_actual.bin')
    parser.add_argument('-t', '--txt')
    args = parser.parse_args()
    generate_ascii_font(args.font, args.size, args.output, args.txt)
