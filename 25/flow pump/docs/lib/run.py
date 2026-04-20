python generate_ascii_font.py -f "C:/Windows/Fonts/consola.ttf" -s 16 -W 16 -H 16 -o ascii_16.bin --offset-table ascii_offset.txt --header ascii_font.h
python pic_bin.py ./icons -o image_data.bin -w 48 -H 48
python generate_mono_font.py -f "C:/Windows/Fonts/msyh.ttc" -s 32 -W 32 -H 32 -o gb2312_32.bin --offset-table chinese32.txt --header font32.h

python generate_mono_font.py -f "C:/Windows/Fonts/msyh.ttc" -s 16 -W 16 -H 16 -o gb2312_16.bin --offset-table offsets.txt --header font.h
python combin.py -f .\gb2312_16.bin .\gb2312_32.bin .\image_data.bin -o all.bin --header index.h -a 256
//all bin data struct
typedef struct {
    char name[32];
    uint32_t offset;
    uint32_t size;
    uint32_t reserved;
} file_index_t;

typedef struct {
    uint32_t file_count;
    uint32_t index_offset;
    uint32_t index_size;
} file_footer_t;
//gb32
typedef struct {
    uint8_t  magic[4];      // "MONO"
    uint32_t version;
    uint32_t char_count;
    uint16_t flags;         // bit0: 1=实际尺寸
    uint8_t  reserved[18];
} font_header_t;

typedef struct {
    uint32_t unicode;
    uint32_t offset;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t  reserved[6];
} font_index_t;

//icons
+-----------------------------------+  <-- 文件开头 (0x00000000)
| 图片1的RGB565像素数据              |
| (宽度 × 高度 × 2 字节)             |
+-----------------------------------+
| 图片2的RGB565像素数据              |
+-----------------------------------+
| ...                               |
+-----------------------------------+
| 图片N的RGB565像素数据              |
+-----------------------------------+  <-- 索引表起始位置 (table_offset)
| 文件数量 (4字节, 小端)             |
| 条目1 (44字节)                     |
| 条目2 (44字节)                     |
| ...                               |
| 条目N (44字节)                     |
+-----------------------------------+  <-- 文件末尾
| 索引表偏移量 (4字节, 小端)         |
+-----------------------------------+


typedef struct {
    char     name[32];      // 文件名（不含扩展名），UTF-8 编码，不足32字节用 '\0' 填充
    uint32_t offset;        // 该图片像素数据在文件中的起始偏移（字节，从文件头算起）
    uint32_t size;          // 该图片像素数据的大小（字节）
    uint16_t width;         // 图片宽度（像素）
    uint16_t height;        // 图片高度（像素）
    // 注：此处实际占 32+4+4+2+2 = 44 字节，脚本中通过 struct.pack('<IIHH') 打包 offset, size, width, height
    // 但由于 name 字段已固定 32 字节，因此总长度为 44 字节，没有额外填充。
} image_entry_t;



