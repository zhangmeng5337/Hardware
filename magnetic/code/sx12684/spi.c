#include "bsp.h"

void SPI_Int()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
    SPI_DeInit(SPI1);

    // ����SPI��ز���,2��Ƶ��8MHZ��
    SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
                SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
                SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 7);
    SPI_Cmd(SPI1, ENABLE);

    // SPI���IO������
    GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);       // MISO (PB7)
    GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);  // SCLK (PB5)
    GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);  // MOSI (PB6)
}

/*!
 * @brief Sends txBuffer and receives rxBuffer
 *
 * @param [IN] txBuffer Byte to be sent
 * @param [OUT] rxBuffer Byte to be sent
 * @param [IN] size Byte to be sent
 */
uint8_t SpiInOut( uint8_t txBuffer)
{
    
    SPI_SendData(SPI1, txBuffer);
    while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));   // �ȴ����ݴ������
    while( SPI_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET );
    //while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));  // �ȴ����ݽ������
    return (SPI_ReceiveData(SPI1));      
   
}

void SpiIn( uint8_t *txBuffer, uint16_t size )
{
    uint16_t i;
    
    for(i=0;i<size;i++)
    {
    SPI_SendData(SPI1, txBuffer[i]);
    while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));   // �ȴ����ݴ������
    while( SPI_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET );
    //while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));  // �ȴ����ݽ������
    SPI_ReceiveData(SPI1);    
      
    }
}


