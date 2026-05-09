#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
全自动：将目录下中文图片文件名转为英文简写，记录映射，并生成bin文件。
依赖：pip install Pillow pypinyin
"""

import os
import sys
import argparse
import subprocess
import re
from pypinyin import lazy_pinyin

# 可扩展的常见词翻译字典（中文 -> 英文）
COMMON_WORD_MAP = {
    "首页": "home",
    "设置": "settings",
    "图标": "icon",
    "背景": "bg",
    "按钮": "btn",
    "头像": "avatar",
    "确认": "confirm",
    "取消": "cancel",
    "保存": "save",
    "删除": "delete",
    "编辑": "edit",
    "添加": "add",
    "返回": "back",
    "菜单": "menu",
    "登录": "login",
    "注册": "register",
    "用户": "user",
    "密码": "password",
    "关于": "about",
    "帮助": "help",
    "退出": "exit",
    "加载": "load",
    "刷新": "refresh",
    "搜索": "search",
    "放大": "zoom_in",
    "缩小": "zoom_out",
    "播放": "play",
    "暂停": "pause",
    "停止": "stop",
    "上一页": "prev",
    "下一页": "next",
    "完成": "done",
    "确定": "ok",
}

def chinese_to_english_simplified(text):
    """
    将中文文本转换为英文简写：
    1. 优先使用常见词映射表
    2. 其余部分取拼音首字母大写
    """
    # 先尝试整体匹配映射表
    if text in COMMON_WORD_MAP:
        return COMMON_WORD_MAP[text]
    # 否则按词语分割（简单按单个字符）
    result = []
    for ch in text:
        if ch in COMMON_WORD_MAP:  # 单个字符映射
            result.append(COMMON_WORD_MAP[ch])
        elif '\u4e00' <= ch <= '\u9fff':  # 汉字
            pinyins = lazy_pinyin(ch)
            if pinyins:
                # 取首字母大写，例如 "你" -> "N"
                result.append(pinyins[0][0].upper())
            else:
                result.append('X')
        else:  # 非中文字符保留
            result.append(ch)
    # 合并，如果有多个部分，用下划线连接
    simplified = ''.join(result)
    # 去除多余下划线，限制长度
    simplified = re.sub(r'_+', '_', simplified).strip('_')
    if not simplified:
        simplified = 'img'
    return simplified

def auto_rename_images(input_dir, dry_run=False, use_suffix=True):
    """
    扫描目录下的图片文件，自动生成英文简写并重命名。
    返回 mapping 列表和是否成功。
    """
    supported = ('.jpg', '.jpeg', '.png', '.bmp', '.gif')
    files = [f for f in os.listdir(input_dir) if f.lower().endswith(supported)]
    if not files:
        print("目录中没有图片文件")
        return []

    files.sort()
    mapping = []
    used_names = set()
    for old_name in files:
        stem, ext = os.path.splitext(old_name)
        new_stem = chinese_to_english_simplified(stem)
        # 处理重名：如果新文件名已存在，加数字后缀
        new_name = new_stem + ext
        counter = 1
        while new_name in used_names or os.path.exists(os.path.join(input_dir, new_name)):
            new_name = f"{new_stem}_{counter}{ext}"
            counter += 1
        used_names.add(new_name)
        mapping.append((old_name, new_name))
        if not dry_run:
            old_path = os.path.join(input_dir, old_name)
            new_path = os.path.join(input_dir, new_name)
            os.rename(old_path, new_path)
            print(f"重命名: {old_name} -> {new_name}")
        else:
            print(f"[模拟] 重命名: {old_name} -> {new_name}")
    return mapping

def save_mapping(mapping_file, mapping):
    """保存映射关系到txt文件"""
    with open(mapping_file, 'w', encoding='utf-8') as f:
        f.write("# 原文件名 -> 新文件名\n")
        for old, new in mapping:
            f.write(f"{old}\t{new}\n")
    print(f"映射表已保存: {mapping_file}")

def convert_to_bin(input_dir, output_bin, width, height, mode):
    """调用转换脚本（假设脚本名为 convert_images_to_bin.py）"""
    script_path = os.path.join(os.path.dirname(__file__), "convert_images_to_bin.py")
    if not os.path.exists(script_path):
        # 尝试当前目录
        script_path = "pic_bin.py"
    if not os.path.exists(script_path):
        print("错误: 找不到 convert_images_to_bin.py 文件")
        return False
    cmd = [sys.executable, script_path, input_dir, "-o", output_bin]
    if width:
        cmd.extend(["-w", str(width)])
    if height:
        cmd.extend(["-H", str(height)])
    cmd.extend(["-m", mode])
    print("执行转换命令:", " ".join(cmd))
    result = subprocess.run(cmd)
    return result.returncode == 0

def main():
    parser = argparse.ArgumentParser(description='自动将目录下中文图片转为英文简写，并生成bin文件')
    parser.add_argument('input_dir', help='图片目录')
    parser.add_argument('-o', '--output', default='images.bin', help='输出bin文件')
    parser.add_argument('-w', '--width', type=int, help='目标宽度')
    parser.add_argument('-H', '--height', type=int, help='目标高度')
    parser.add_argument('-m', '--mode', default='fit', choices=['fit','stretch','crop'])
    parser.add_argument('--dry-run', action='store_true', help='仅预览，不实际重命名和转换')
    parser.add_argument('--no-convert', action='store_true', help='只重命名，不生成bin文件')
    args = parser.parse_args()

    input_dir = args.input_dir
    if not os.path.isdir(input_dir):
        print(f"目录不存在: {input_dir}")
        sys.exit(1)

    # 1. 自动重命名
    mapping = auto_rename_images(input_dir, dry_run=args.dry_run)
    if not mapping:
        print("没有需要处理的图片")
        return

    # 2. 保存映射txt
    mapping_file = os.path.join(input_dir, "rename_mapping.txt")
    save_mapping(mapping_file, mapping)

    # 3. 转换bin文件
    if not args.dry_run and not args.no_convert:
        convert_to_bin(input_dir, args.output, args.width, args.height, args.mode)

if __name__ == '__main__':
    main()
