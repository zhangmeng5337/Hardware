with open('fon32.bin', 'rb') as f:
    index_start = 32  # 文件头大小
    char_count = 3755 # 预期字符数，也可以从文件头中读取
    prev = 0
    for i in range(char_count):
        f.seek(index_start + i * 20)
        unicode_val = int.from_bytes(f.read(4), 'little')
        if i > 0 and unicode_val < prev:
            print(f"排序错误：索引 {i} 处值 0x{unicode_val:08X} 小于前一个值 0x{prev:08X}")
            break
        prev = unicode_val
    else:
        print("索引表按 Unicode 升序排列，排序正确。")
