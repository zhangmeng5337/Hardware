#include "fm25w32hal.h"
#include "fm25w32.h"
// 等待器件内部操作完成
void FM25W32_WaitForWriteEnd(void) {
    uint8_t status = 0;
    FM25W32_CS_LOW();
    SPI_TransmitByte(FM25W32_CMD_RDSR1); // 发送读状态寄存器命令
    do {
        status = SPI_TransmitByte(0xFF);
    } while (status & 0x01); // 检查最低位(BUSY位)，为1表示忙
    FM25W32_CS_HIGH();
}

// 写使能
void FM25W32_WriteEnable(void) {
    FM25W32_CS_LOW();
    SPI_TransmitByte(FM25W32_CMD_WREN);
    FM25W32_CS_HIGH();
}

// 页编程 (写入数据)
void FM25W32_PageProgram(uint32_t address, uint8_t *pData, uint16_t size) {
    FM25W32_WriteEnable();            // 1. 发送写使能
    FM25W32_WaitForWriteEnd();        // 等待内部操作完成
    FM25W32_CS_LOW();                 // 2. 拉低片选，开始命令
    SPI_TransmitByte(FM25W32_CMD_PP); // 3. 发送页编程指令
    SPI_TransmitByte((address >> 16) & 0xFF); // 4. 发送24位地址
    SPI_TransmitByte((address >> 8) & 0xFF);
    SPI_TransmitByte(address & 0xFF);
    SPI_TransmitBytes(pData, size);   // 5. 发送数据
    FM25W32_CS_HIGH();                // 6. 拉高片选，结束命令
    FM25W32_WaitForWriteEnd();        // 7. 等待编程完成
}

// 读取数据
void FM25W32_ReadData(uint32_t address, uint8_t *pBuffer, uint16_t size) {
    FM25W32_CS_LOW();
    SPI_TransmitByte(FM25W32_CMD_READ);
    SPI_TransmitByte((address >> 16) & 0xFF);
    SPI_TransmitByte((address >> 8) & 0xFF);
    SPI_TransmitByte(address & 0xFF);
    SPI_ReceiveBytes(pBuffer, size);
    FM25W32_CS_HIGH();
}

// 扇区擦除 (4KB)
void FM25W32_SectorErase(uint32_t address) {
    FM25W32_WriteEnable();
    FM25W32_WaitForWriteEnd();
    FM25W32_CS_LOW();
    SPI_TransmitByte(FM25W32_CMD_SE);
    SPI_TransmitByte((address >> 16) & 0xFF);
    SPI_TransmitByte((address >> 8) & 0xFF);
    SPI_TransmitByte(address & 0xFF);
    FM25W32_CS_HIGH();
    FM25W32_WaitForWriteEnd();
}
/**
 * @brief FM25W32 全片擦除
 * @note  将整个芯片(4MB)擦除为0xFF，耗时约30秒[reference:0]
 * @retval 0:成功; -1:失败
 */
int FM25W32_ChipErase(void)
{
    // 1. 发送写使能指令 (0x06)
    FM25W32_WriteEnable();
    FM25W32_WaitForWriteEnd();

    // 2. 发送全片擦除指令
    FM25W32_CS_LOW();
    SPI_TransmitByte(FM25W32_CMD_CE); // 0xC7 或 0x60
    FM25W32_CS_HIGH();

    // 3. 等待擦除完成 (最长约30秒)[reference:1]
    FM25W32_WaitForWriteEnd();

    return 0;
}
void FM25W32_Test(void) {
    uint8_t write_data[256];
    uint8_t read_data[256];
    uint32_t test_address = 0x000000;

    // 1. 准备测试数据 (例如，0-255的递增数据)
    for (uint16_t i = 0; i < 256; i++) {
        write_data[i] = (uint8_t)i;
    }

    // 2. 擦除扇区
    FM25W32_SectorErase(test_address);
    // 添加适当延时等待擦除完成

    // 3. 执行页编程
    FM25W32_PageProgram(test_address, write_data, 256);

    // 4. 读取数据
    FM25W32_ReadData(test_address, read_data, 256);

    // 5. 比较数据是否一致
    for (uint16_t i = 0; i < 256; i++) {
        if (write_data[i] != read_data[i]) {
            Error_Handler(); // 数据不一致，进入错误处理
        }
    }
    // 如果全部一致，测试通过
}