# generate_common_chars.py
import sys

def generate_gb2312_level1(output_file='common_chars.txt'):
    """
    生成GB2312一级汉字（共3755个）列表文件
    编码范围：0xB0A1 - 0xD7F9
    """
    chars = []
    # GB2312 一级汉字区域：第一字节 0xB0-0xD7，第二字节 0xA1-0xFE
    for row in range(0xB0, 0xD8):      # 0xB0 ~ 0xD7
        for col in range(0xA1, 0xFF):  # 0xA1 ~ 0xFE
            try:
                # 将GB2312字节转换为字符
                gb_bytes = bytes([row, col])
                char = gb_bytes.decode('gb2312')
                chars.append(char)
            except UnicodeDecodeError:
                continue  # 非汉字区跳过
    # GB2312 一级汉字正好 3755 个
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write('\n'.join(chars))
    print(f"已生成 {len(chars)} 个常用汉字到 {output_file}")

if __name__ == '__main__':
    generate_gb2312_level1()

