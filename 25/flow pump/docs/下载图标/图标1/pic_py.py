#!/usr/bin/env python3
"""
JPEG图片转RGB565数组转换器
支持单张图片转换、批量转换、合并输出，并生成图片索引
数组名称基于文件名自动生成，支持汉字转拼音（需安装pypinyin）
"""

import sys
import argparse
import os
import datetime
import re

# 尝试导入pypinyin，用于汉字转拼音
try:
    from pypinyin import lazy_pinyin
    HAS_PINYIN = True
except ImportError:
    HAS_PINYIN = False

from PIL import Image

def filename_to_c_identifier(filename, for_macro=False):
    """
    将文件名转换为合法的C语言标识符
    规则：
      - 去除扩展名
      - 汉字转换为拼音（如果已安装pypinyin），否则替换为下划线
      - 将非法字符（非字母、数字、下划线）替换为下划线
      - 合并连续的下划线
      - 如果第一个字符是数字，在前面加下划线
      - 如果用于宏（for_macro=True），则转换为大写
    """
    # 分离文件名和扩展名
    base = os.path.splitext(os.path.basename(filename))[0]
    if not base:
        base = "image"

    # 汉字转拼音（如果支持）
    if HAS_PINYIN:
        # 检查是否包含中文字符（简单通过Unicode范围判断）
        def has_chinese(s):
            for ch in s:
                if '\u4e00' <= ch <= '\u9fff':
                    return True
            return False

        if has_chinese(base):
            # 使用lazy_pinyin转换，返回拼音列表
            pinyin_list = lazy_pinyin(base)
            # 用下划线连接
            base = '_'.join(pinyin_list)
    else:
        # 提示用户安装pypinyin以获得更好支持
        if any('\u4e00' <= ch <= '\u9fff' for ch in base):
            print("提示：文件名包含汉字，但未安装pypinyin，汉字将被替换为下划线。")
            print("      建议安装：pip install pypinyin")

    # 将非法字符替换为下划线
    identifier = re.sub(r'[^a-zA-Z0-9_]', '_', base)
    # 合并连续的下划线
    identifier = re.sub(r'_+', '_', identifier)
    # 去除首尾下划线
    identifier = identifier.strip('_')
    # 如果第一个字符是数字，前面加下划线
    if identifier and identifier[0].isdigit():
        identifier = '_' + identifier
    # 如果为空，使用默认名
    if not identifier:
        identifier = 'image'

    # 转换为大写（用于宏）或保持原样（用于变量名）
    if for_macro:
        return identifier.upper()
    else:
        return identifier.lower()

def rgb888_to_rgb565(r, g, b):
    """将24位RGB888转换为16位RGB565格式"""
    r5 = (r >> 3) & 0x1F
    g6 = (g >> 2) & 0x3F
    b5 = (b >> 3) & 0x1F
    return (r5 << 11) | (g6 << 5) | b5

def process_image(image_path, width=None, height=None, resize_mode='fit'):
    """
    处理单张图片，返回图片数据和标识符信息
    返回: (var_name, macro_prefix, final_width, final_height, rgb565_values)
    """
    print(f"处理图片: {image_path}")
    img = Image.open(image_path)
    if img.mode != 'RGB':
        img = img.convert('RGB')
    orig_width, orig_height = img.size

    # 调整图片大小
    if width is not None and height is not None:
        if resize_mode == 'fit':
            img.thumbnail((width, height), Image.Resampling.LANCZOS)
        elif resize_mode == 'crop':
            img = img.resize((width, height), Image.Resampling.LANCZOS)
        elif resize_mode == 'stretch':
            img = img.resize((width, height), Image.Resampling.LANCZOS)
        else:
            img = img.resize((width, height), Image.Resampling.LANCZOS)
    elif width is not None:
        ratio = width / orig_width
        new_height = int(orig_height * ratio)
        img = img.resize((width, new_height), Image.Resampling.LANCZOS)
    elif height is not None:
        ratio = height / orig_height
        new_width = int(orig_width * ratio)
        img = img.resize((new_width, height), Image.Resampling.LANCZOS)

    final_width, final_height = img.size
    pixels = list(img.getdata())
    total_pixels = len(pixels)

    # 转换为RGB565
    rgb565_values = []
    for i, pixel in enumerate(pixels):
        if len(pixel) == 4:  # RGBA
            r, g, b, a = pixel
        else:                 # RGB
            r, g, b = pixel
        rgb565_values.append(rgb888_to_rgb565(r, g, b))
        if i % 10000 == 0 and i > 0:
            percent = (i / total_pixels) * 100
            print(f"  转换进度: {percent:.1f}% ({i}/{total_pixels})")
    print(f"  完成: {final_width}x{final_height}, {total_pixels} 像素")

    # 生成标识符
    var_name = filename_to_c_identifier(image_path, for_macro=False)
    macro_prefix = filename_to_c_identifier(image_path, for_macro=True)

    return var_name, macro_prefix, final_width, final_height, rgb565_values

def write_single_file(output_file, var_name, macro_prefix, width, height, rgb_values, source_file):
    """将单个图片的数据写入C头文件"""
    total_pixels = len(rgb_values)
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(f"""/**
 * RGB565像素数组
 * 来源: {os.path.basename(source_file)}
 * 尺寸: {width}x{height} (宽x高)
 * 像素数: {total_pixels}
 * 生成时间: {current_time}
 */
#include <stdint.h>

#define {macro_prefix}_WIDTH {width}
#define {macro_prefix}_HEIGHT {height}
#define {macro_prefix}_SIZE {total_pixels}

const uint16_t {var_name}[{macro_prefix}_SIZE] = {{
""")
        # 写入像素数据，每行8个
        for i, value in enumerate(rgb_values):
            f.write(f"    0x{value:04X}")
            if i != total_pixels - 1:
                f.write(",")
            if (i + 1) % 8 == 0:
                f.write("\n")
        f.write("\n};\n\n")
        f.write(f"""/*
 * 使用示例:
 *    display_draw_rgb565(0, 0, {macro_prefix}_WIDTH, {macro_prefix}_HEIGHT, {var_name});
 *    for (int y = 0; y < {macro_prefix}_HEIGHT; y++) {{
 *        for (int x = 0; x < {macro_prefix}_WIDTH; x++) {{
 *            uint16_t pixel = {var_name}[y * {macro_prefix}_WIDTH + x];
 *        }}
 *    }}
 */
""")

def write_combined_file(output_file, images_data):
    """
    将所有图片数据合并写入一个C头文件，并生成图片索引数组
    images_data: 列表，每个元素为 (var_name, macro_prefix, width, height, rgb_values, source_file)
    """
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    # 生成头文件保护宏（基于输出文件名）
    guard_macro = filename_to_c_identifier(output_file, for_macro=True).replace('.', '_')

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(f"""/**
 * 合并RGB565像素数组
 * 包含 {len(images_data)} 张图片
 * 生成时间: {current_time}
 */
#ifndef {guard_macro}_H
#define {guard_macro}_H

#include <stdint.h>

""")
        # 写入每个图片的数组和宏定义
        for var_name, macro_prefix, width, height, rgb_values, source_file in images_data:
            total_pixels = len(rgb_values)
            f.write(f"/* 来源: {os.path.basename(source_file)} */\n")
            f.write(f"#define {macro_prefix}_WIDTH {width}\n")
            f.write(f"#define {macro_prefix}_HEIGHT {height}\n")
            f.write(f"#define {macro_prefix}_SIZE {total_pixels}\n")
            f.write(f"const uint16_t {var_name}[{macro_prefix}_SIZE] = {{\n")

            # 写入像素数据，每行8个
            for i, value in enumerate(rgb_values):
                f.write(f"    0x{value:04X}")
                if i != total_pixels - 1:
                    f.write(",")
                if (i + 1) % 8 == 0:
                    f.write("\n")
            f.write("\n};\n\n")

        # 生成图片索引结构
        f.write("""/**
 * 图片信息结构体，用于索引所有图片
 */
typedef struct {
    uint16_t width;          // 图片宽度
    uint16_t height;         // 图片高度
    const uint16_t *data;    // 指向RGB565像素数组的指针
} image_info_t;

/**
 * 所有图片的信息数组，按以下顺序排列：
""")
        # 添加注释说明每个索引对应的源文件
        for idx, (var_name, macro_prefix, width, height, _, source_file) in enumerate(images_data):
            f.write(f" *   [{idx}] {os.path.basename(source_file)} ({var_name})\n")
        f.write(" */\n")
        f.write("const image_info_t images[] = {\n")
        for var_name, macro_prefix, width, height, _, _ in images_data:
            f.write(f"    {{ .width = {macro_prefix}_WIDTH, .height = {macro_prefix}_HEIGHT, .data = {var_name} }},\n")
        f.write("};\n\n")

        # 定义图片总数宏
        f.write(f"#define IMAGE_COUNT (sizeof(images) / sizeof(images[0]))\n\n")

        f.write(f"#endif /* {guard_macro}_H */\n")

def convert_single_image(image_path, output_file, width, height, mode):
    """单文件转换入口"""
    try:
        var_name, macro_prefix, w, h, rgb_vals = process_image(image_path, width, height, mode)
        write_single_file(output_file, var_name, macro_prefix, w, h, rgb_vals, image_path)
        print(f"转换完成! 输出文件: {output_file}")
        print(f"数组名称: {var_name}")
        print(f"宏前缀: {macro_prefix}")
        print(f"内存占用: {len(rgb_vals) * 2} 字节 ({len(rgb_vals) * 2 / 1024:.1f} KB)")
        return True
    except Exception as e:
        print(f"错误: {e}")
        return False

def batch_convert_images(input_dir, output_dir, width=None, height=None, mode='fit'):
    """批量转换到目录（每个图片单独文件）"""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    supported_formats = ['.jpg', '.jpeg', '.png', '.bmp', '.gif']
    image_files = [os.path.join(input_dir, f) for f in os.listdir(input_dir)
                   if any(f.lower().endswith(fmt) for fmt in supported_formats)]

    if not image_files:
        print(f"在目录 {input_dir} 中未找到支持的图片文件")
        return

    # 按文件名排序，使顺序可预测
    image_files.sort()

    print(f"找到 {len(image_files)} 个图片文件")
    success_count = 0
    for i, image_path in enumerate(image_files):
        print(f"\n处理文件 {i+1}/{len(image_files)}: {os.path.basename(image_path)}")
        base_name = os.path.splitext(os.path.basename(image_path))[0]
        output_file = os.path.join(output_dir, f"{base_name}_rgb565.h")
        try:
            var_name, macro_prefix, w, h, rgb_vals = process_image(image_path, width, height, mode)
            write_single_file(output_file, var_name, macro_prefix, w, h, rgb_vals, image_path)
            success_count += 1
        except Exception as e:
            print(f"  处理失败: {e}")
    print(f"\n批量转换完成! 成功: {success_count}/{len(image_files)} 文件")

def combine_images_to_single_file(input_dir, output_file, width=None, height=None, mode='fit'):
    """将目录中所有图片合并到一个.h文件"""
    supported_formats = ['.jpg', '.jpeg', '.png', '.bmp', '.gif']
    image_files = [os.path.join(input_dir, f) for f in os.listdir(input_dir)
                   if any(f.lower().endswith(fmt) for fmt in supported_formats)]

    if not image_files:
        print(f"在目录 {input_dir} 中未找到支持的图片文件")
        return

    # 按文件名排序，使顺序可预测
    image_files.sort()

    print(f"找到 {len(image_files)} 个图片文件，准备合并到单个文件: {output_file}")
    images_data = []
    success_count = 0
    for i, image_path in enumerate(image_files):
        print(f"\n处理文件 {i+1}/{len(image_files)}: {os.path.basename(image_path)}")
        try:
            data = process_image(image_path, width, height, mode)
            images_data.append(data + (image_path,))  # 添加源文件路径用于注释
            success_count += 1
        except Exception as e:
            print(f"  处理失败: {e}")

    if images_data:
        write_combined_file(output_file, images_data)
        print(f"\n合并完成! 输出文件: {output_file}")
        print(f"成功合并 {success_count} 张图片，失败 {len(image_files)-success_count} 张")
    else:
        print("没有成功处理的图片，未生成文件")

def convert_image_to_binary(image_path, output_file, width=None, height=None):
    """将图片转换为二进制文件（直接存储RGB565数据，文件头包含宽度和高度）"""
    try:
        img = Image.open(image_path)
        if img.mode != 'RGB':
            img = img.convert('RGB')
        if width is not None and height is not None:
            img = img.resize((width, height), Image.Resampling.LANCZOS)
        width, height = img.size
        pixels = list(img.getdata())

        with open(output_file, 'wb') as f:
            f.write(width.to_bytes(2, 'little'))
            f.write(height.to_bytes(2, 'little'))
            for pixel in pixels:
                if len(pixel) == 4:
                    r, g, b, a = pixel
                else:
                    r, g, b = pixel
                rgb565 = rgb888_to_rgb565(r, g, b)
                f.write(rgb565.to_bytes(2, 'little'))

        print(f"二进制文件已生成: {output_file}")
        print(f"尺寸: {width}x{height}")
        print(f"文件大小: {os.path.getsize(output_file)} 字节")
        return True
    except Exception as e:
        print(f"错误: {e}")
        return False

def main():
    parser = argparse.ArgumentParser(
        description='JPEG图片转RGB565数组转换器，支持汉字转拼音、合并输出及图片索引',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例:
  %(prog)s input.jpg output.h                    # 基本转换
  %(prog)s input.jpg output.h -w 320 -H 240     # 转换为指定尺寸
  %(prog)s input.jpg output.h -w 320            # 宽度320，高度按比例
  %(prog)s input.jpg output.h --binary          # 转换为二进制文件
  %(prog)s input_dir output_dir -b              # 批量转换到目录（每个图片独立文件）
  %(prog)s input_dir all_icons.h -b -c          # 批量转换并合并到单个.h文件（带索引）
        """
    )
    parser.add_argument('input', help='输入图片文件或目录')
    parser.add_argument('output', help='输出文件或目录')
    parser.add_argument('-w', '--width', type=int, help='目标宽度')
    parser.add_argument('-H', '--height', type=int, help='目标高度')
    parser.add_argument('-m', '--mode', choices=['fit', 'crop', 'stretch'],
                        default='fit', help='调整大小模式: fit(适应), crop(裁剪), stretch(拉伸)')
    parser.add_argument('--binary', action='store_true', help='输出为二进制文件')
    parser.add_argument('-b', '--batch', action='store_true', help='批量转换模式（输入为目录）')
    parser.add_argument('-c', '--combined', action='store_true', help='与-b配合使用，将所有图片数据合并到一个.h文件中，并生成图片索引')
    args = parser.parse_args()

    # 检查参数有效性
    if args.binary and args.combined:
        print("错误：--binary 和 --combined 不能同时使用")
        return 1

    if args.batch:
        if not os.path.isdir(args.input):
            print(f"错误: {args.input} 不是目录")
            return 1
        if args.combined:
            # 合并到单个文件
            combine_images_to_single_file(args.input, args.output, args.width, args.height, args.mode)
        else:
            # 批量输出到目录
            batch_convert_images(args.input, args.output, args.width, args.height, args.mode)
    else:
        if args.combined:
            print("错误：--combined 只能与 -b 一起使用")
            return 1
        if not os.path.isfile(args.input):
            print(f"错误: {args.input} 不是文件")
            return 1
        if args.binary:
            convert_image_to_binary(args.input, args.output, args.width, args.height)
        else:
            convert_single_image(args.input, args.output, args.width, args.height, args.mode)
    return 0

if __name__ == "__main__":
    sys.exit(main())

