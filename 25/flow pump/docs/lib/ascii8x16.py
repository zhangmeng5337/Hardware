#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成 8x16 ASCII 单色点阵字库，支持加粗，并输出 TXT 解析表
"""

import argparse
import struct
import os
import freetype
import numpy as np

DEFAULT_FONT = "C:/Windows/Fonts/consola.ttf"
FIXED_W = 8
FIXED_H = 16

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

def pack_bitmap(bitmap):
    h, w = bitmap.shape
    pitch = (w + 7) // 8
    packed = bytearray(h * pitch)
    for y in range(h):
        for x in range(w):
            if bitmap[y, x]:
                idx = y * pitch + (x // 8)
                packed[idx] |= (0x80 >> (x % 8))
    return packed, pitch

def render_char_fixed(ch, face, fixed_w, fixed_h, bold_radius):
    face.set_char_size(16 * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    glyph = face.glyph
    bitmap = glyph.bitmap
    w, h = bitmap.width, bitmap.rows
    canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
    off_x = max(0, (fixed_w - w) // 2)
    off_y = max(0, (fixed_h - h) // 2)

    buffer = bitmap.buffer
    if buffer:
        pitch_ft = bitmap.pitch if hasattr(bitmap, 'pitch') else (w + 7) // 8
        for y in range(min(h, fixed_h - off_y)):
            for x in range(min(w, fixed_w - off_x)):
                byte_idx = y * pitch_ft + (x // 8)
                bit = (buffer[byte_idx] >> (7 - (x % 8))) & 1
                if bit:
                    canvas[off_y + y, off_x + x] = 1

    if bold_radius > 0:
        canvas = dilate_bitmap(canvas, bold_radius)

    return pack_bitmap(canvas)

def generate_ascii_font(font_path, bold_radius, output_bin, output_txt=None):
    chars = [chr(i) for i in range(32, 127)]
    print(f"生成 {len(chars)} 个 ASCII 字符，加粗半径={bold_radius}")

    face = freetype.Face(font_path)
    pixel_data = bytearray()
    index_entries = []   # (unicode, offset, width, height, pitch)

    for ch in chars:
        packed, pitch = render_char_fixed(ch, face, FIXED_W, FIXED_H, bold_radius)
        offset = len(pixel_data)
        pixel_data.extend(packed)
        index_entries.append((ord(ch), offset, FIXED_W, FIXED_H, pitch))

    # 生成二进制文件
    header = struct.pack('<4sIIH18s', b'MONO', 1, len(chars), 0, b'\x00'*18)
    index_table = bytearray()
    for unicode_val, offset, w, h, pitch in index_entries:
        index_table.extend(struct.pack('<IIHHH6s', unicode_val, offset, w, h, pitch, b'\x00'*6))
    bin_data = header + index_table + pixel_data
    with open(output_bin, 'wb') as f:
        f.write(bin_data)
    print(f"字库生成成功: {output_bin} (总大小 {len(bin_data)} 字节)")

    # 生成 TXT 解析表
    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + '.txt'
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("ASCII 8x16 点阵字库索引表\n")
        f.write(f"字符范围: 32-126, 共 {len(chars)} 个字符\n")
        f.write(f"固定尺寸: {FIXED_W}x{FIXED_H}\n")
        f.write(f"加粗半径: {bold_radius}\n\n")
        f.write(f"{'序号':<6} {'字符':<4} {'ASCII':<6} {'Unicode':<10} {'偏移(Hex)':<12} {'宽度':<6} {'高度':<6} {'pitch':<6}\n")
        f.write("-" * 80 + "\n")
        for i, (ch, (unicode_val, offset, w, h, pitch)) in enumerate(zip(chars, index_entries)):
            f.write(f"{i:<6} {ch:<4} {unicode_val:<6} 0x{unicode_val:08X}   0x{offset:08X}     {w:<6} {h:<6} {pitch:<6}\n")
        f.write("=" * 80 + "\n")
        f.write(f"文件头大小: 32 字节\n")
        f.write(f"索引表大小: {len(index_table)} 字节\n")
        f.write(f"像素数据大小: {len(pixel_data)} 字节\n")
    print(f"解析表格已保存: {output_txt}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--font', default=DEFAULT_FONT, help='TrueType 字体文件路径')
    parser.add_argument('-b', '--bold', type=int, default=0, help='加粗半径（0 不加粗）')
    parser.add_argument('-o', '--output', default='ascii8x16.bin', help='输出 .bin 文件名')
    parser.add_argument('-t', '--txt', help='输出 .txt 解析表文件名（默认与 .bin 同名）')
    args = parser.parse_args()
    generate_ascii_font(args.font, args.bold, args.output, args.txt)
