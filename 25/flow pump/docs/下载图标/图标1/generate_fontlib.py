#!/usr/bin/env python3
"""
批量汉字点阵生成器（支持加粗，索引表增加汉字注释）
从字符列表文件读取汉字，为每个汉字生成点阵，打包成C语言字库。
背景色为白色 (0xFFFF)，前景色为黑色 (0x0000)。
可选加粗参数 --bold <半径> 对笔画进行膨胀加粗。
索引表中每行末尾添加汉字字符和索引号注释。
"""

import sys
import argparse
import numpy as np
import freetype

def rgb888_to_rgb565(r, g, b):
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

def dilate_bitmap(bitmap, radius):
    """
    对二值位图进行膨胀操作
    bitmap: 二维numpy数组 (0/1)
    radius: 膨胀半径，例如1表示3x3邻域
    """
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

def render_char_to_bitmap(ch, face, font_size):
    """渲染单个字符，返回二值位图 (0/1) 及度量信息"""
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

def render_char_fixed_size(ch, face, font_size, fixed_w, fixed_h, bold_radius):
    """
    渲染单个字符，应用膨胀加粗，并放置到固定大小的画布中央
    返回画布的位图 (fixed_h x fixed_w) 以及字符在画布中的偏移 (offset_x, offset_y)
    """
    try:
        bitmap, w, h, left, top = render_char_to_bitmap(ch, face, font_size)
        # 膨胀加粗
        if bold_radius > 0:
            bitmap = dilate_bitmap(bitmap, bold_radius)
            w, h = bitmap.shape[1], bitmap.shape[0]  # 膨胀可能改变尺寸，但这里返回值不变（因为膨胀函数返回相同尺寸）
    except Exception as e:
        print(f"  渲染字符 '{ch}' 时出错: {e}")
        return np.zeros((fixed_h, fixed_w), dtype=np.uint8), fixed_w, fixed_h, 0, 0

    canvas = np.zeros((fixed_h, fixed_w), dtype=np.uint8)
    offset_x = max(0, (fixed_w - w) // 2)
    offset_y = max(0, (fixed_h - h) // 2)

    for y in range(min(h, fixed_h - offset_y)):
        for x in range(min(w, fixed_w - offset_x)):
            if bitmap[y, x]:
                canvas[offset_y + y, offset_x + x] = 1
    return canvas, fixed_w, fixed_h, offset_x, offset_y

def generate_font_lib(char_list_file, font_path, font_size, output_file,
                      array_name="font_lib", fixed_width=None, fixed_height=None,
                      encoding='utf-8', bold_radius=0):
    """
    从字符列表文件生成完整的C语言字库
    """
    with open(char_list_file, 'r', encoding=encoding) as f:
        chars = [line.strip() for line in f if line.strip()]
    print(f"需要生成的字符数量: {len(chars)}")

    try:
        face = freetype.Face(font_path)
    except Exception as e:
        print(f"无法打开字体文件: {font_path}\n{e}")
        sys.exit(1)

    font_data = []
    char_offsets = []
    current_offset = 0
    max_width = 0
    max_height = 0
    glyph_metrics = []

    print("开始渲染字符...")
    for i, ch in enumerate(chars):
        try:
            if fixed_width is not None and fixed_height is not None:
                canvas, w, h, offset_x, offset_y = render_char_fixed_size(
                    ch, face, font_size, fixed_width, fixed_height, bold_radius)
                max_width = max(max_width, w)
                max_height = max(max_height, h)
                glyph_metrics.append((w, h, offset_x, offset_y))
            else:
                # 实际尺寸模式暂不支持加粗，如有需要可类似实现
                print("警告：实际尺寸模式不支持加粗，建议使用固定尺寸")
                bitmap, w, h, left, top = render_char_to_bitmap(ch, face, font_size)
                canvas = bitmap
                max_width = max(max_width, w)
                max_height = max(max_height, h)
                glyph_metrics.append((w, h, left, top))

            char_offsets.append(current_offset)

            for y in range(h):
                for x in range(w):
                    if canvas[y, x]:
                        font_data.append(0x0000)  # 黑色文字
                    else:
                        font_data.append(0xFFFF)  # 白色背景
            current_offset += w * h

            if (i+1) % 100 == 0:
                print(f"  已处理 {i+1}/{len(chars)} 字符")
        except Exception as e:
            print(f"警告：字符 '{ch}' (U+{ord(ch):04X}) 处理失败: {e}")
            if fixed_width and fixed_height:
                w, h = fixed_width, fixed_height
                offset_x, offset_y = 0, 0
            else:
                w, h = 1, 1
                offset_x, offset_y = 0, 0
            glyph_metrics.append((w, h, offset_x, offset_y))
            char_offsets.append(current_offset)
            font_data.append(0xFFFF)
            current_offset += 1

    total_chars = len(chars)
    total_pixels = len(font_data)

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(f"""/**
 * @file {output_file}
 * @brief 批量生成的点阵字库（白色背景，黑色字符）
 * 
 * 字符数量: {total_chars}
 * 字体: {font_path}
 * 字号: {font_size}px
 * 加粗半径: {bold_radius}
""")
        if fixed_width and fixed_height:
            f.write(f" * 固定字符尺寸: {fixed_width}x{fixed_height} (每个字符居中放置)\n")
        f.write(f" * 生成时间: {__import__('datetime').datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        f.write(" */\n\n")

        f.write(f"#ifndef {array_name.upper()}_H\n")
        f.write(f"#define {array_name.upper()}_H\n\n")
        f.write("#include <stdint.h>\n\n")

        f.write("/*========================= 字库元数据 =========================*/\n")
        f.write(f"#define {array_name.upper()}_CHAR_COUNT {total_chars}\n")
        f.write(f"#define {array_name.upper()}_MAX_WIDTH   {max_width}\n")
        f.write(f"#define {array_name.upper()}_MAX_HEIGHT  {max_height}\n")
        if fixed_width and fixed_height:
            f.write(f"#define {array_name.upper()}_FIXED_WIDTH  {fixed_width}\n")
            f.write(f"#define {array_name.upper()}_FIXED_HEIGHT {fixed_height}\n\n")
        else:
            f.write("\n")

        f.write("""/**
 * @brief 字符信息结构体
 */
typedef struct {
    uint16_t unicode;      /**< 字符的Unicode码点 */
    uint16_t width;        /**< 字符点阵宽度 */
    uint16_t height;       /**< 字符点阵高度 */
    uint16_t left;         /**< 字符在画布中的左偏移（固定尺寸时有效，否则为原始左偏移） */
    uint16_t top;          /**< 字符在画布中的上偏移（固定尺寸时有效，否则为原始上偏移） */
    uint32_t offset;       /**< 在像素数组中的起始索引 */
} """ + f"{array_name}_char_info_t;\n\n")

        f.write(f"/**\n * @brief 像素数据数组（RGB565格式，白色背景0xFFFF，黑色字符0x0000）\n */\n")
        f.write(f"const uint16_t {array_name}_pixels[{total_pixels}] = {{\n")
        for i, val in enumerate(font_data):
            f.write(f"    0x{val:04X}")
            if i != total_pixels - 1:
                f.write(",")
            if (i + 1) % 8 == 0:
                f.write("\n")
        f.write("\n};\n\n")

        f.write(f"/**\n * @brief 字符索引表（每行末尾注释为汉字字符和索引号）\n */\n")
        f.write(f"const {array_name}_char_info_t {array_name}_chars[{total_chars}] = {{\n")
        for i, ch in enumerate(chars):
            w, h, left, top = glyph_metrics[i]
            # 添加汉字字符注释和索引号
            f.write(f"    {{ .unicode = 0x{ord(ch):04X}, .width = {w}, .height = {h}, .left = {left}, .top = {top}, .offset = {char_offsets[i]} }}, /* {ch} [{i}] */\n")
        f.write("};\n\n")

        f.write(f"""
/*========================= 使用示例 =========================*/
/**
 * @code
 * #include "{output_file}"
 * 
 * const {array_name}_char_info_t* find_char(uint16_t unicode) {{
 *     for (int i = 0; i < {array_name.upper()}_CHAR_COUNT; i++) {{
 *         if ({array_name}_chars[i].unicode == unicode) {{
 *             return &{array_name}_chars[i];
 *         }}
 *     }}
 *     return NULL;
 * }}
 * 
 * void draw_char(uint16_t x, uint16_t y, uint16_t unicode, TFT_eSPI* tft) {{
 *     const {array_name}_char_info_t* info = find_char(unicode);
 *     if (!info) return;
 *     const uint16_t* pixels = &{array_name}_pixels[info->offset];
 *     tft->pushImage(x, y, info->width, info->height, pixels);
 * }}
 * @endcode
 */

#endif /* {array_name.upper()}_H */
""")

    print(f"\n字库生成完成！")
    print(f"输出文件: {output_file}")
    print(f"总字符数: {total_chars}")
    print(f"像素数据大小: {total_pixels * 2} 字节")
    if bold_radius > 0:
        print(f"加粗半径: {bold_radius}")

def main():
    parser = argparse.ArgumentParser(description='批量汉字点阵生成器（支持加粗，索引表带汉字注释）')
    parser.add_argument('char_list', help='字符列表文件（每行一个汉字）')
    parser.add_argument('font', help='TrueType字体文件路径')
    parser.add_argument('size', type=int, help='字号（像素）')
    parser.add_argument('-o', '--output', default='fontlib.h', help='输出文件名，默认fontlib.h')
    parser.add_argument('-n', '--name', default='font_lib', help='C数组名称前缀，默认font_lib')
    parser.add_argument('-W', '--char-width', type=int, help='固定字符宽度（可选，需与高度同时指定）')
    parser.add_argument('-H', '--char-height', type=int, help='固定字符高度（可选，需与宽度同时指定）')
    parser.add_argument('--encoding', default='utf-8', help='字符列表文件的编码，例如 utf-8, gbk, gb2312（默认utf-8）')
    parser.add_argument('--bold', type=int, default=0, help='加粗半径（像素），例如1表示笔画加粗一圈，需配合固定尺寸使用')
    args = parser.parse_args()

    if (args.char_width is None) != (args.char_height is None):
        parser.error("必须同时指定 --char-width 和 --char-height，或都不指定")

    generate_font_lib(args.char_list, args.font, args.size, args.output,
                      args.name, args.char_width, args.char_height,
                      encoding=args.encoding, bold_radius=args.bold)

if __name__ == "__main__":
    sys.exit(main())

