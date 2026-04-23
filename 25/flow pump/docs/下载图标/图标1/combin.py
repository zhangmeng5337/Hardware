#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
多个二进制文件合并工具
将指定目录下的所有 .bin 文件（或指定文件列表）合并为一个大的 .bin 文件，
并在文件末尾生成文件索引表（包含文件名、偏移、大小等）。
输出：
    merged.bin      : 合并后的二进制文件
    merged.txt      : 可读的索引表
    merged_index.h  : C 语言头文件（定义索引结构体）
"""

import os
import sys
import argparse
import struct
from pathlib import Path

# 索引条目结构（固定32字节）
# 格式：名称(32字节) + 偏移(4字节) + 大小(4字节) + 保留(4字节) = 44字节
# 也可以更紧凑，这里为方便解析使用固定长度
INDEX_ENTRY_SIZE = 32 + 4 + 4 + 4  # 44 bytes

def create_index_entry(name, offset, size):
    """生成一个索引条目（二进制）"""
    name_bytes = name.encode('utf-8')[:31]
    name_padded = name_bytes.ljust(32, b'\x00')
    return name_padded + struct.pack('<II', offset, size) + b'\x00\x00\x00\x00'

def parse_index_entry(data):
    """从二进制数据解析索引条目"""
    name = data[:32].split(b'\x00', 1)[0].decode('utf-8')
    offset, size = struct.unpack('<II', data[32:40])
    return name, offset, size

def merge_files(input_dir, output_bin, output_txt=None, output_header=None,
                extensions=None, file_list=None, align=1):
    """
    合并文件
    :param input_dir:   输入目录（如果 file_list 为 None，则扫描该目录）
    :param output_bin:  输出 .bin 文件名
    :param output_txt:  输出文本表格文件名
    :param output_header: 输出 C 头文件名
    :param extensions:   要扫描的文件扩展名列表，如 ['.bin', '.dat']
    :param file_list:    直接指定文件列表（优先级高于 input_dir）
    :param align:        文件数据对齐字节数（例如 256，使每个文件起始地址对齐到页边界）
    """
    # 1. 获取所有待合并的文件
    files = []
    if file_list:
        files = file_list
    elif input_dir:
        if not os.path.isdir(input_dir):
            print(f"错误：目录 {input_dir} 不存在")
            return False
        if extensions is None:
            extensions = ['.bin']   # 默认只合并 .bin 文件
        for f in os.listdir(input_dir):
            if any(f.lower().endswith(ext) for ext in extensions):
                files.append(os.path.join(input_dir, f))
        files.sort()  # 按文件名排序，保证顺序可预测
    else:
        print("错误：必须指定 input_dir 或 file_list")
        return False

    if not files:
        print("错误：没有找到任何文件")
        return False

    print(f"找到 {len(files)} 个文件，开始合并...")

    # 2. 合并数据并记录索引
    merged_data = bytearray()
    index_entries = []  # (name, offset, size)

    for file_path in files:
        name = os.path.basename(file_path)
        with open(file_path, 'rb') as f:
            data = f.read()
        size = len(data)
        # 对齐处理
        current_offset = len(merged_data)
        if align > 1 and current_offset % align != 0:
            pad = align - (current_offset % align)
            merged_data.extend(b'\x00' * pad)
            current_offset += pad
        offset = current_offset
        merged_data.extend(data)
        index_entries.append((name, offset, size))
        print(f"  {name} : offset=0x{offset:08X}, size={size} bytes")

    # 3. 生成索引表
    index_table = bytearray()
    for name, offset, size in index_entries:
        index_table.extend(create_index_entry(name, offset, size))
    index_offset = len(merged_data)
    # 写入索引表
    merged_data.extend(index_table)
    # 写入索引表信息（文件数量 + 索引表偏移 + 索引表大小）
    footer = struct.pack('<III', len(index_entries), index_offset, len(index_table))
    merged_data.extend(footer)

    # 4. 写入最终 .bin 文件
    with open(output_bin, 'wb') as f:
        f.write(merged_data)
    print(f"\n合并完成！输出文件: {output_bin} (总大小 {len(merged_data)} 字节)")

    # 5. 生成可读的 .txt 表格
    if output_txt is None:
        output_txt = os.path.splitext(output_bin)[0] + '.txt'
    with open(output_txt, 'w', encoding='utf-8') as f:
        f.write("文件索引表\n")
        f.write("=" * 80 + "\n")
        f.write(f"{'序号':<4} {'文件名':<32} {'偏移(Hex)':<12} {'大小(Bytes)':<12}\n")
        f.write("-" * 80 + "\n")
        for i, (name, offset, size) in enumerate(index_entries):
            f.write(f"{i:<4} {name:<32} 0x{offset:08X}   {size:<12}\n")
        f.write("=" * 80 + "\n")
        f.write(f"总文件数: {len(index_entries)}\n")
        f.write(f"索引表偏移: 0x{index_offset:08X}\n")
        f.write(f"索引表大小: {len(index_table)} 字节\n")
    print(f"表格文件已保存: {output_txt}")

    # 6. 生成 C 头文件
    if output_header:
        with open(output_header, 'w', encoding='utf-8') as f:
            f.write("""// 自动生成的文件索引表结构定义
#ifndef MERGED_INDEX_H
#define MERGED_INDEX_H

#include <stdint.h>

// 索引条目结构（44字节）
typedef struct {
    char name[32];          // 文件名
    uint32_t offset;        // 在合并文件中的偏移
    uint32_t size;          // 文件大小
    uint32_t reserved;      // 保留
} file_index_t;

// 文件尾信息结构
typedef struct {
    uint32_t file_count;    // 文件数量
    uint32_t index_offset;  // 索引表起始偏移
    uint32_t index_size;    // 索引表大小（字节）
} file_footer_t;

#endif
""")
        print(f"C头文件已保存: {output_header}")

    return True

def main():
    parser = argparse.ArgumentParser(description='合并多个二进制文件为一个，并生成索引表')
    parser.add_argument('-d', '--dir', help='输入目录（扫描该目录下的文件）')
    parser.add_argument('-f', '--files', nargs='+', help='直接指定要合并的文件列表')
    parser.add_argument('-e', '--extensions', nargs='+', default=['.bin'],
                        help='要扫描的文件扩展名（默认 .bin），如 -e .bin .dat')
    parser.add_argument('-o', '--output', default='merged.bin', help='输出 .bin 文件名（默认 merged.bin）')
    parser.add_argument('-t', '--txt', help='输出表格文件名（默认与 .bin 同名 .txt）')
    parser.add_argument('--header', help='输出 C 头文件名')
    parser.add_argument('-a', '--align', type=int, default=1,
                        help='每个文件数据对齐字节数（例如 256），默认 1（不对齐）')
    args = parser.parse_args()

    if not args.dir and not args.files:
        parser.error("必须指定 --dir 或 --files")
    if args.dir and args.files:
        parser.error("不能同时指定 --dir 和 --files")

    merge_files(
        input_dir=args.dir,
        output_bin=args.output,
        output_txt=args.txt,
        output_header=args.header,
        extensions=args.extensions if args.dir else None,
        file_list=args.files,
        align=args.align
    )

if __name__ == '__main__':
    main()

