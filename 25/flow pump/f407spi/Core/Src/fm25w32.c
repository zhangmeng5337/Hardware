#include "fm25w32.h"

extern SPI_HandleTypeDef hspi1;

// 发送一个字节并接收一个字节
 uint8_t SPI_TransmitByte(uint8_t data) {
    uint8_t received_data = 0;
    HAL_SPI_TransmitReceive(&hspi1, &data, &received_data, 1, HAL_MAX_DELAY);
    return received_data;
}

// 发送多个字节
 void SPI_TransmitBytes(uint8_t *pData, uint16_t size) {
    HAL_SPI_Transmit(&hspi1, pData, size, HAL_MAX_DELAY);
}

// 接收多个字节
 void SPI_ReceiveBytes(uint8_t *pData, uint16_t size) {
    HAL_SPI_Receive(&hspi1, pData, size, HAL_MAX_DELAY);
}