#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成常用汉字单色点阵字库 (16x16) - 索引表按 Unicode 升序排列
"""

import argparse
import struct
import os
import sys
import freetype
import numpy as np

DEFAULT_FONT = "C:/Windows/Fonts/msyh.ttc"
FIXED_W = 16
FIXED_H = 16

# ---------- 字符列表生成 ----------
def get_gb2312_level1_chars():
    """生成 GB2312 一级汉字列表（3755个，原始按区位码顺序）"""
    chars = []
    for row in range(0xB0, 0xD8):
        for col in range(0xA1, 0xFF):
            try:
                gb_bytes = bytes([row, col])
                ch = gb_bytes.decode('gb2312')
                chars.append(ch)
            except UnicodeDecodeError:
                continue
    return chars

# ---------- 图像处理 ----------
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

def render_char_fixed(ch, face, fixed_w, fixed_h):
    face.set_char_size(16 * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    glyph = face.glyph
    bitmap = glyph.bitmap
    w, h = bitmap.width, bitmap.rows
    canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
    off_x = max(0, (fixed_w - w) // 2)
    off_y = max(0, (fixed_h - h) // 2)

    if bitmap.buffer:
        pitch_ft = bitmap.pitch if hasattr(bitmap, 'pitch') else (w + 7) // 8
        for y in range(min(h, fixed_h - off_y)):
            for x in range(min(w, fixed_w - off_x)):
                byte_idx = y * pitch_ft + (x // 8)
                bit = (bitmap.buffer[byte_idx] >> (7 - (x % 8))) & 1
                if bit:
                    canvas[off_y + y, off_x + x] = 1

    return pack_bitmap(canvas)

# ---------- 主生成函数 ----------
def generate_font_bin(char_list_file, font_path, output_bin, output_txt=None, encoding='utf-8'):
    # 获取字符列表
    if char_list_file and os.path.exists(char_list_file):
        with open(char_list_file, 'r', encoding=encoding) as f:
            chars = [line.strip() for line in f if line.strip()]
        print(f"从文件读取 {len(chars)} 个字符")
    else:
        chars = get_gb2312_level1_chars()
        print(f"自动生成 GB2312 一级汉字，共 {len(chars)} 个")

    # 关键：按 Unicode 升序排序
    chars.sort(key=lambda ch: ord(ch))
    print("字符列表已按 Unicode 升序排序")

    # 加载字体
    try:
        face = freetype.Face(font_path)
    except Exception as e:
        print(f"无法打开字体文件: {font_path}\n{e}")
        sys.exit(1)

    pixel_data = bytearray()
    index_entries = []   # (unicode, offset, width, height, pitch)

    total = len(chars)
    for i, ch in enumerate(chars):
        if (i+1) % 100 == 0:
            print(f"  渲染进度: {i+1}/{total}")
        packed, pitch = render_char_fixed(ch, face, FIXED_W, FIXED_H)
        offset = len(pixel_data)
        pixel_data.extend(packed)
        index_entries.append((ord(ch), offset, FIXED_W, FIXED_H, pitch))

    print(f"渲染完成，像素数据总大小: {len(pixel_data)} 字节")

    # 构建二进制文件
    char_count = len(chars)
    header = struct.pack('<4sIIH18s', b'MONO', 1, char_count, 0, b'\x00'*18)
    index_table = bytearray()
    for unicode_val, offset, w, h, pitch in index_entries:
        index_table.extend(struct.pack('<IIHHH6s', unicode_val, offset, w, h, pitch, b'\x00'*6))
    bin_data = header + index_table + pixel_data
    with open(output_bin, 'wb') as f:
        f.write(bin_data)
    print(f"二进制字库已保存: {output_bin} (总大小 {len(bin_data)} 字节)")

    # 生成 TXT 解析表
    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + '.txt'
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("16x16 汉字点阵字库索引表 (按 Unicode 排序)\n")
        f.write(f"字符数量: {len(chars)}\n")
        f.write(f"固定尺寸: {FIXED_W}x{FIXED_H}\n")
        f.write(f"字体: {font_path}\n\n")
        f.write(f"{'序号':<6} {'字符':<4} {'Unicode':<12} {'偏移(Hex)':<12} {'宽度':<6} {'高度':<6} {'pitch':<6}\n")
        f.write("-" * 80 + "\n")
        for i, (ch, (unicode_val, offset, w, h, pitch)) in enumerate(zip(chars, index_entries)):
            f.write(f"{i:<6} {ch:<4} 0x{unicode_val:08X}   0x{offset:08X}     {w:<6} {h:<6} {pitch:<6}\n")
        f.write("=" * 80 + "\n")
        f.write(f"文件头大小: 32 字节\n")
        f.write(f"索引表大小: {len(index_table)} 字节\n")
        f.write(f"像素数据大小: {len(pixel_data)} 字节\n")
    print(f"解析表格已保存: {output_txt}")

# ---------- 命令行入口 ----------
def main():
    parser = argparse.ArgumentParser(description='生成常用汉字单色点阵字库 (16x16) - 索引表按 Unicode 排序')
    parser.add_argument('-c', '--char-list', help='字符列表文件（每行一个汉字），若不提供则自动生成 GB2312 一级汉字')
    parser.add_argument('-f', '--font', default=DEFAULT_FONT, help=f'TrueType字体文件路径，默认 {DEFAULT_FONT}')
    parser.add_argument('-o', '--output', default='hzk16_unicode.bin', help='输出 .bin 文件名，默认 hzk16_unicode.bin')
    parser.add_argument('-t', '--txt', help='输出详细表格文件名，默认与 .bin 同名 .txt')
    parser.add_argument('--encoding', default='utf-8', help='字符列表文件的编码，默认 utf-8')
    args = parser.parse_args()

    generate_font_bin(
        char_list_file=args.char_list,
        font_path=args.font,
        output_bin=args.output,
        output_txt=args.txt,
        encoding=args.encoding
    )

if __name__ == '__main__':
    main()
