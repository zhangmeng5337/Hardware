import struct

with open('my_ascii8x16.bin', 'rb') as f:
    f.seek(32)  # 文件头大小
    for i in range(95):
        data = f.read(20)
        if len(data) < 20:
            print("文件长度不足")
            break
        # 解包前14字节：Unicode(4) + offset(4) + width(2) + height(2) + pitch(2)
        unicode_val, offset, w, h, pitch = struct.unpack('<IIHHH', data[:14])
        if unicode_val == 0x41:
            print(f"Found 'A' at index {i}: offset=0x{offset:08X}, w={w}, h={h}, pitch={pitch}")
            break
    else:
        print("'A' not found")


