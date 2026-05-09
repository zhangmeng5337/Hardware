import os
import sys
import argparse
import struct
from PIL import Image

def rgb888_to_rgb565(r, g, b):
    """将24位RGB转换为RGB565格式的16位像素值"""
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

def convert_image_to_rgb565(image_path, width=None, height=None, resize_mode='fit'):
    """转换单张图片为RGB565格式的像素列表"""
    try:
        img = Image.open(image_path).convert('RGB')
        orig_width, orig_height = img.size

        # 图片缩放
        if width and height:
            if resize_mode == 'fit':
                img.thumbnail((width, height), Image.LANCZOS)
            elif resize_mode == 'stretch':
                img = img.resize((width, height), Image.LANCZOS)
            elif resize_mode == 'crop':
                ratio = max(width/orig_width, height/orig_height)
                new_size = (int(orig_width*ratio), int(orig_height*ratio))
                img = img.resize(new_size, Image.LANCZOS)
                left = (new_size[0] - width) // 2
                top = (new_size[1] - height) // 2
                img = img.crop((left, top, left+width, top+height))
        elif width:
            ratio = width / orig_width
            img = img.resize((width, int(orig_height*ratio)), Image.LANCZOS)
        elif height:
            ratio = height / orig_height
            img = img.resize((int(orig_width*ratio), height), Image.LANCZOS)

        final_width, final_height = img.size
        pixels = list(img.getdata())
        total_pixels = len(pixels)

        # 转换为RGB565
        rgb565_values = bytearray()
        for pixel in pixels:
            rgb565 = rgb888_to_rgb565(*pixel[:3])
            rgb565_values.extend(rgb565.to_bytes(2, 'little'))  # 小端字节序
        return rgb565_values, final_width, final_height
    except Exception as e:
        print(f"处理图片 {image_path} 时出错: {e}")
        return None, 0, 0

def batch_convert_to_bin(input_dir, output_bin, width=None, height=None, resize_mode='fit'):
    """批量转换目录下所有图片并合并为一个.bin文件，同时生成.txt表格"""
    if not os.path.exists(input_dir):
        print(f"目录 {input_dir} 不存在！")
        return False

    supported_formats = ('.jpg', '.jpeg', '.png', '.bmp', '.gif')
    image_files = [f for f in os.listdir(input_dir) if f.lower().endswith(supported_formats)]

    if not image_files:
        print(f"目录 {input_dir} 中没有找到支持的图片文件。")
        return False

    print(f"找到 {len(image_files)} 个图片文件，开始转换...")
    all_data = bytearray()
    file_table = []

    for i, img_file in enumerate(image_files):
        print(f"正在处理 ({i+1}/{len(image_files)}): {img_file}")
        img_path = os.path.join(input_dir, img_file)
        img_data, w, h = convert_image_to_rgb565(img_path, width, height, resize_mode)
        if img_data is not None:
            offset = len(all_data)
            all_data.extend(img_data)
            file_table.append({
                'name': os.path.splitext(img_file)[0],
                'offset': offset,
                'size': len(img_data),
                'width': w,
                'height': h
            })
            print(f"  -> 尺寸: {w}x{h}, 偏移: 0x{offset:08X}, 大小: {len(img_data)} 字节")
        else:
            print(f"  -> 转换失败，已跳过。")

    # 写入文件表到.bin文件末尾
    table_offset = len(all_data)
    all_data.extend(struct.pack('<I', len(file_table)))  # 文件数量
    for entry in file_table:
        name_bytes = entry['name'].encode('utf-8')[:31]
        all_data.extend(name_bytes.ljust(32, b'\x00'))  # 固定32字节的文件名
        all_data.extend(struct.pack('<IIHH', entry['offset'], entry['size'], entry['width'], entry['height']))
    all_data.extend(struct.pack('<I', table_offset))  # 末尾写入表偏移量

    # 写入最终.bin文件
    with open(output_bin, 'wb') as f:
        f.write(all_data)

    # 生成 .txt 表格文件
    txt_file = os.path.splitext(output_bin)[0] + '.txt'
    with open(txt_file, 'w', encoding='utf-8') as f:
        f.write("Image Index Table\n")
        f.write("=" * 80 + "\n")
        f.write(f"{'No.':<4} {'Name':<32} {'Offset(Hex)':<12} {'Size(Bytes)':<12} {'Width':<8} {'Height':<8}\n")
        f.write("-" * 80 + "\n")
        for idx, entry in enumerate(file_table):
            f.write(f"{idx:<4} {entry['name']:<32} 0x{entry['offset']:08X}   {entry['size']:<12} {entry['width']:<8} {entry['height']:<8}\n")
        f.write("=" * 80 + "\n")
        f.write(f"Total images: {len(file_table)}\n")
        f.write(f"Binary file size: {len(all_data)} bytes\n")
        f.write(f"Table offset in binary: 0x{table_offset:08X}\n")

    print(f"\n转换完成！共成功转换 {len(file_table)} 个文件。")
    print(f"输出二进制文件: {output_bin} (大小: {len(all_data)} 字节)")
    print(f"输出表格文件: {txt_file}")
    return True

def main():
    parser = argparse.ArgumentParser(description='批量转换图片为RGB565格式的.bin文件，并生成.txt表格')
    parser.add_argument('input_dir', help='输入图片目录')
    parser.add_argument('-o', '--output', default='images.bin', help='输出.bin文件名 (默认: images.bin)')
    parser.add_argument('-w', '--width', type=int, help='目标宽度 (可选)')
    parser.add_argument('-H', '--height', type=int, help='目标高度 (可选)')
    parser.add_argument('-m', '--mode', choices=['fit', 'stretch', 'crop'], default='fit', help='缩放模式 (默认: fit)')
    args = parser.parse_args()

    batch_convert_to_bin(args.input_dir, args.output, args.width, args.height, args.mode)

if __name__ == "__main__":
    main()
