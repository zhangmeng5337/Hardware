#include "fm25w32.h"
#include "spi.h" // 包含你的SPI初始化头文件

// 注意：这里假设你在外部定义了SPI的句柄，例如在 main.c 中
// extern SPI_HandleTypeDef hspi1;

// --- 私有函数 ---
/**
 * @brief 向FM25W32发送一个字节并通过MISO接收一个字节
 * @param txData: 待发送的数据
 * @return 接收到的数据
 */
static uint8_t SPI_ReadWriteByte(uint8_t txData) {
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &txData, &rxData, 1, HAL_MAX_DELAY);
    return rxData;
}

/**
 * @brief 发送写使能命令 (WREN)
 */
static void FM25W32_WriteEnable(void) {
    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_WRITE_ENABLE);
    FM25W32_CS_HIGH();
}

/**
 * @brief 读取FM25W32的状态寄存器
 * @return 状态寄存器的值
 */
static uint8_t FM25W32_ReadStatus(void) {
    uint8_t status = 0;
    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_READ_STATUS);
    status = SPI_ReadWriteByte(0xFF);
    FM25W32_CS_HIGH();
    return status;
}

// --- 公共函数 ---
/**
 * @brief 等待FM25W32内部写操作完成 (轮询WIP位)
 */
void FM25W32_WaitForWriteEnd(void) {
    uint8_t status = 0;
    do {
        status = FM25W32_ReadStatus();
    } while (status & 0x01); // WIP位为1表示忙
}

/**
 * @brief 初始化SPI接口，可选读取ID验证通信
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_Init(void) {
    // 验证SPI通信：尝试读取ID
    uint8_t mid = 0;
    uint16_t did = 0;
    if (FM25W32_ReadID(&mid, &did) == 0) {
        // 可选：将读取到的mid和did与预期值比较
        return 0; // 成功
    }
    return 1; // 失败
}

/**
 * @brief 读取FM25W32的ID (Manufacturer ID 和 Device ID)
 * @param pMID: 存储Manufacturer ID的指针
 * @param pDID: 存储Device ID的指针
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_ReadID(uint8_t *pMID, uint16_t *pDID) {
    uint8_t id[3] = {0};
    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_READ_ID);
    id[0] = SPI_ReadWriteByte(0xFF);
    id[1] = SPI_ReadWriteByte(0xFF);
    id[2] = SPI_ReadWriteByte(0xFF);
    FM25W32_CS_HIGH();
    if (pMID) *pMID = id[0];
    if (pDID) *pDID = (id[1] << 8) | id[2];
    return 0;
}

/**
 * @brief 从FM25W32中读取数据
 * @param pBuffer: 存储读出数据的指针
 * @param ReadAddr: 读取的起始地址 (24-bit地址)
 * @param NumByteToRead: 要读取的字节数
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead) {
    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_READ_DATA);
    SPI_ReadWriteByte((ReadAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((ReadAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(ReadAddr & 0xFF);
    for (uint32_t i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = SPI_ReadWriteByte(0xFF);
    }
    FM25W32_CS_HIGH();
    return 0;
}

/**
 * @brief 页编程：写入最多256字节到FM25W32的一个页内
 * @param pBuffer: 待写入数据的指针
 * @param WriteAddr: 写入的起始地址 (24-bit地址)
 * @param NumByteToWrite: 要写入的字节数 (<=256)
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite) {
    if (NumByteToWrite > FM25W32_PAGE_SIZE) return 1;

    FM25W32_WriteEnable();
    FM25W32_WaitForWriteEnd();

    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_PAGE_PROGRAM);
    SPI_ReadWriteByte((WriteAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((WriteAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(WriteAddr & 0xFF);
    for (uint32_t i = 0; i < NumByteToWrite; i++) {
        SPI_ReadWriteByte(pBuffer[i]);
    }
    FM25W32_CS_HIGH();
    FM25W32_WaitForWriteEnd();
    return 0;
}

/**
 * @brief 擦除一个扇区 (4KB)
 * @param SectorAddr: 扇区起始地址 (24-bit地址)
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_EraseSector(uint32_t SectorAddr) {
    FM25W32_WriteEnable();
    FM25W32_WaitForWriteEnd();

    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_SECTOR_ERASE);
    SPI_ReadWriteByte((SectorAddr >> 16) & 0xFF);
    SPI_ReadWriteByte((SectorAddr >> 8) & 0xFF);
    SPI_ReadWriteByte(SectorAddr & 0xFF);
    FM25W32_CS_HIGH();
    FM25W32_WaitForWriteEnd();
    return 0;
}

/**
 * @brief 整片擦除芯片
 * @return 0: 成功, 1: 失败
 */
uint8_t FM25W32_EraseChip(void) {
    FM25W32_WriteEnable();
    FM25W32_WaitForWriteEnd();

    FM25W32_CS_LOW();
    SPI_ReadWriteByte(FM25W32_CMD_CHIP_ERASE);
    FM25W32_CS_HIGH();
    FM25W32_WaitForWriteEnd();
    return 0;
}
