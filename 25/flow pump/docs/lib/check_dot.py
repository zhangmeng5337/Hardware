#!/usr/bin/env python3
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

def extract_char(data, off, w, h, pitch, base):
    raw = data[base + off : base + off + h*pitch]
    rows = []
    for y in range(h):
        row = []
        for x in range(w):
            byte_idx = y * pitch + (x // 8)
            bit = (raw[byte_idx] >> (7 - (x % 8))) & 1
            row.append(bit)
        rows.append(row)
    return rows

def display(rows, name):
    print(f"\n{name} 的点阵 ({len(rows)}x{len(rows[0])}):")
    for y, row in enumerate(rows):
        byte_val = 0
        for x, bit in enumerate(row):
            if bit:
                byte_val |= (0x80 >> x)
        hex_str = f"{byte_val:02x}"
        pattern = ''.join('█' if bit else ' ' for bit in row)
        print(f"{y:2d}: {hex_str}  {pattern}")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("用法: py check_font.py <bin文件> [字符]")
        sys.exit(1)
    fname = sys.argv[1]
    ch = sys.argv[2] if len(sys.argv) > 2 else '.'
    if not os.path.exists(fname):
        print(f"文件 {fname} 不存在")
        sys.exit(1)
    with open(fname, 'rb') as f:
        data = f.read()
    try:
        index, base = parse_mono(data)
        u = ord(ch)
        if u not in index:
            print(f"字符 '{ch}' (0x{u:02X}) 不在字库中")
            sys.exit(1)
        off, w, h, pitch = index[u]
        print(f"字符: '{ch}'  Unicode: 0x{u:04X}  偏移: 0x{off:08X}  尺寸: {w}x{h}")
        rows = extract_char(data, off, w, h, pitch, base)
        display(rows, ch)
    except Exception as e:
        print(f"错误: {e}")
        sys.exit(1)
