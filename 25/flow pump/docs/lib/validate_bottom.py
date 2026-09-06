#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys, struct, os

def parse_mono(data):
    if len(data) < 32:
        raise ValueError("文件太小")
    magic, ver, count, flags, _ = struct.unpack('<4sIIH18s', data[:32])
    if magic != b'MONO':
        raise ValueError("不是MONO格式")
    index = {}
    pos = 32
    for _ in range(count):
        if pos + 20 > len(data):
            raise ValueError("索引表不完整")
        u, off, w, h, p, _ = struct.unpack('<IIHHH6s', data[pos:pos+20])
        index[u] = (off, w, h, p)
        pos += 20
    return index, pos

def extract_char_raw(data, off, w, h, pitch, base):
    raw = data[base + off : base + off + h*pitch]
    rows = []
    for y in range(h):
        row = 0
        for x in range(w):
            byte_idx = y * pitch + (x // 8)
            bit = (raw[byte_idx] >> (7 - (x % 8))) & 1
            if bit:
                row |= (1 << (w - 1 - x))  # 方便显示，但这里只统计是否有像素
        rows.append(row)
    return rows

def get_bottom_line(rows):
    """返回有像素的最大行号（从0开始），如果没有像素则返回-1"""
    for y in range(len(rows)-1, -1, -1):
        if rows[y] != 0:
            return y
    return -1

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("用法: py validate_bottom.py <bin文件>")
        sys.exit(1)
    fname = sys.argv[1]
    if not os.path.exists(fname):
        print(f"文件 {fname} 不存在")
        sys.exit(1)
    with open(fname, 'rb') as f:
        data = f.read()
    try:
        index, base = parse_mono(data)
        # 检查所有字符（32~126）
        all_ok = True
        for code in range(32, 127):
            if code not in index:
                print(f"字符 0x{code:02X} 不在索引中")
                continue
            off, w, h, pitch = index[code]
            rows = extract_char_raw(data, off, w, h, pitch, base)
            bottom = get_bottom_line(rows)
            ch = chr(code)
            # 对于可见字符，底部行号应为 h-1 或 h-2（如果字符本身无下行部分）
            # 但至少应 > h-3，否则可能未贴底
            expected_bottom = h - 1
            # 对于小字符（如 '`'），可能只有1像素高，会在底部，允许微调
            if bottom < h - 3 and bottom != -1:
                print(f"警告: 字符 '{ch}' (0x{code:02X}) 底部行={bottom}，期望 ~{expected_bottom}")
                all_ok = False
            # 打印一些典型字符的底部行信息
            if code in [ord(c) for c in '.,_\'g']:
                print(f"字符 '{ch}': 底部行={bottom} (高度={h})")
        if all_ok:
            print("所有字符底部对齐验证通过！")
        else:
            print("存在未贴底的字符，请检查。")
    except Exception as e:
        print(f"错误: {e}")
        sys.exit(1)

