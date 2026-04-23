#include "fm25w32.h"
#include "spi.h"        // 包含你的 SPI 句柄声明

extern SPI_HandleTypeDef hspi1;   // 在 main.c 中已定义

// ---------------------------- 底层 SPI 读写 ---------------------------------
static uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    uint8_t RxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &TxData, &RxData, 1, HAL_MAX_DELAY);
    return RxData;
}

// ---------------------------- 写使能 ---------------------------------------
static void FM25_WriteEnable(void)
{
    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_WRITE_ENABLE);
    FM25_CS_HIGH();
}

// ---------------------------- 等待内部操作完成 (带超时) --------------------
static uint8_t FM25_WaitForWriteEnd(void)
{
    uint8_t status = 0;
    uint32_t timeout = FM25_BUSY_TIMEOUT_MAX;

    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_READ_STATUS);
    do {
        status = SPI_ReadWriteByte(0xFF);
        timeout--;
        if (timeout == 0) {
            FM25_CS_HIGH();
            return 1;   // 超时错误
        }
    } while (status & 0x01);   // WIP 位为 1 表示忙
    FM25_CS_HIGH();
    return 0;   // 成功
}

// ---------------------------- 读取芯片ID -----------------------------------
uint32_t FM25_ReadID(void)
{
    uint32_t id = 0;
    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_READ_ID);
    id |= (SPI_ReadWriteByte(0xFF) << 16);
    id |= (SPI_ReadWriteByte(0xFF) << 8);
    id |= SPI_ReadWriteByte(0xFF);
    FM25_CS_HIGH();
    return id;
}

// ---------------------------- 扇区擦除 (4KB) -------------------------------
uint8_t FM25_EraseSector(uint32_t SectorAddr)
{
    FM25_WriteEnable();
    if (FM25_WaitForWriteEnd()) return 1;

    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_SECTOR_ERASE);
    SPI_ReadWriteByte((SectorAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((SectorAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(SectorAddr & 0xFF);
    FM25_CS_HIGH();

    return FM25_WaitForWriteEnd();
}

// ---------------------------- 整片擦除 -------------------------------------
uint8_t FM25_EraseChip(void)
{
    FM25_WriteEnable();
    if (FM25_WaitForWriteEnd()) return 1;

    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_CHIP_ERASE);
    FM25_CS_HIGH();

    return FM25_WaitForWriteEnd();
}

// ---------------------------- 连续读数据 -----------------------------------
void FM25_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_READ_DATA);
    SPI_ReadWriteByte((ReadAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((ReadAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(ReadAddr & 0xFF);
    for (uint32_t i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = SPI_ReadWriteByte(0xFF);
    }
    FM25_CS_HIGH();
}

// ---------------------------- 页编程 (最多256字节) -------------------------
uint8_t FM25_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    if (NumByteToWrite > FM25_PAGE_SIZE) return 1;

    FM25_WriteEnable();
    if (FM25_WaitForWriteEnd()) return 1;

    FM25_CS_LOW();
    SPI_ReadWriteByte(FM25_CMD_PAGE_PROGRAM);
    SPI_ReadWriteByte((WriteAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((WriteAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(WriteAddr & 0xFF);
    for (uint32_t i = 0; i < NumByteToWrite; i++) {
        SPI_ReadWriteByte(pBuffer[i]);
    }
    FM25_CS_HIGH();

    return FM25_WaitForWriteEnd();
}

// ---------------------------- 跨页写入 (任意长度) --------------------------
void FM25_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    uint32_t page_offset = WriteAddr % FM25_PAGE_SIZE;
    uint32_t first_page_bytes = FM25_PAGE_SIZE - page_offset;
    if (first_page_bytes > NumByteToWrite) first_page_bytes = NumByteToWrite;

    // 写入第一页（可能不完整）
    FM25_WritePage(pBuffer, WriteAddr, first_page_bytes);

    // 剩余数据
    NumByteToWrite -= first_page_bytes;
    WriteAddr += first_page_bytes;
    pBuffer += first_page_bytes;

    while (NumByteToWrite > 0) {
        uint32_t write_bytes = (NumByteToWrite > FM25_PAGE_SIZE) ? FM25_PAGE_SIZE : NumByteToWrite;
        FM25_WritePage(pBuffer, WriteAddr, write_bytes);
        NumByteToWrite -= write_bytes;
        WriteAddr += write_bytes;
        pBuffer += write_bytes;
    }
}