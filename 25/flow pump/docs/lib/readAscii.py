with open('my_ascii8x16.bin', 'rb') as f:
    f.seek(0x99C)
    data = f.read(16)
    print(' '.join(f'{b:02X}' for b in data))

    data = bytes([0x00,0x00,0x00,0x1C,0x14,0x14,0x16,0x22,0x22,0x22,0x7F,0x41,0x41,0x00,0x00,0x00])
from PIL import Image
img = Image.new('1', (8,16))
for y in range(16):
    byte = data[y]
    for x in range(8):
        if (byte >> (7-x)) & 1:
            img.putpixel((x,y), 1)
img.show()

