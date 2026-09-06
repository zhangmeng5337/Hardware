#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import argparse, struct, freetype, numpy as np, os

DEFAULT_FONT = "C:/Windows/Fonts/arial.ttf"
FONT_SIZE = 16

def pack_bitmap(bitmap):
    h, w = bitmap.shape
    pitch = (w + 7) // 8
    packed = bytearray(h * pitch)
    for y in range(h):
        for x in range(w):
            if bitmap[y, x]:
                idx = y * pitch + (x // 8)
                packed[idx] |= (0x80 >> (x % 8))
    return packed, int(pitch), int(w), int(h)

def render_char_bottom(ch, face, font_size):
    """
    渲染单个字符，垂直对齐到底部（基线位置）
    高度固定为 font_size，宽度为字符实际有效宽度（至少为1）
    """
    face.set_char_size(font_size * 64)
    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    glyph = face.glyph
    bitmap = glyph.bitmap
    w, h = bitmap.width, bitmap.rows
    if w == 0 or h == 0:
        canvas = np.zeros((font_size, 1), dtype=np.uint8)
        return pack_bitmap(canvas)
    pitch_ft = bitmap.pitch
    buffer = bitmap.buffer
    img = np.zeros((h, w), dtype=np.uint8)
    for y in range(h):
        for x in range(w):
            byte_idx = y * pitch_ft + (x // 8)
            bit = (buffer[byte_idx] >> (7 - (x % 8))) & 1
            if bit:
                img[y, x] = 1
    rows_with_content = np.any(img, axis=1)
    cols_with_content = np.any(img, axis=0)
    if not np.any(rows_with_content) or not np.any(cols_with_content):
        canvas = np.zeros((font_size, 1), dtype=np.uint8)
        return pack_bitmap(canvas)
    top = np.argmax(rows_with_content)
    bottom = h - 1 - np.argmax(rows_with_content[::-1])
    left = np.argmax(cols_with_content)
    right = w - 1 - np.argmax(cols_with_content[::-1])
    char_img = img[top:bottom+1, left:right+1]
    char_h, char_w = char_img.shape
    if char_h > font_size:
        start = (char_h - font_size) // 2
        char_img = char_img[start:start+font_size, :]
        char_h = font_size
    if char_w > font_size:
        start = (char_w - font_size) // 2
        char_img = char_img[:, start:start+font_size]
        char_w = font_size
    if char_w == 0:
        char_w = 1
    canvas = np.zeros((font_size, char_w), dtype=np.uint8)
    # 关键修改：垂直对齐到底部（而不是居中）
    # y_offset = (font_size - char_h) // 2   # 居中
    y_offset = font_size - char_h            # 底部对齐
    canvas[y_offset:y_offset+char_h, :] = char_img
    return pack_bitmap(canvas)

def generate_ascii_font(font_path, font_size, output_bin, output_txt=None):
    chars = [chr(i) for i in range(32, 127)]
    print(f"生成 {len(chars)} 个 ASCII 字符，字号={font_size}px")
    if not os.path.exists(font_path):
        print(f"错误：字体文件 '{font_path}' 不存在")
        return
    face = freetype.Face(font_path)
    pixel_data = bytearray()
    char_infos = []

    for ch in chars:
        packed, pitch, w, h = render_char_bottom(ch, face, font_size)
        relative_offset = len(pixel_data)
        pixel_data.extend(packed)
        char_infos.append((ord(ch), relative_offset, w, h, pitch))

    char_count = len(chars)
    header = struct.pack('<4sIIH18s', b'MONO', 1, char_count, 1, b'\x00'*18)
    index_table = bytearray()
    for unicode_val, rel_off, w, h, pitch in char_infos:
        # 使用相对偏移（兼容STM32程序）
        index_table.extend(struct.pack('<IIHHH6s',
            int(unicode_val), int(rel_off), int(w), int(h), int(pitch), b'\x00'*6))

    bin_data = header + index_table + pixel_data
    with open(output_bin, 'wb') as f:
        f.write(bin_data)

    print(f"字库已保存: {output_bin} (总大小 {len(bin_data)} 字节)")

    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + "_index.txt"
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("ASCII 字库索引表（底部对齐）\n")
        f.write(f"{'序号':<6} {'字符':<4} {'Unicode':<10} {'偏移(Hex)':<12} {'宽度':<6} {'高度':<6} {'pitch':<6}\n")
        for i, (ch, (u, rel_off, w, h, pitch)) in enumerate(zip(chars, char_infos)):
            f.write(f"{i:<6} {ch:<4} 0x{u:08X}   0x{rel_off:08X}     {w:<6} {h:<6} {pitch:<6}\n")
    print(f"索引表格已自动保存: {output_txt}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--font', default=DEFAULT_FONT)
    parser.add_argument('-s', '--size', type=int, default=FONT_SIZE)
    parser.add_argument('-o', '--output', default='ascii_font.bin')
    parser.add_argument('-t', '--txt', help='可选，指定索引表文件名')
    args = parser.parse_args()
    generate_ascii_font(args.font, args.size, args.output, args.txt)
