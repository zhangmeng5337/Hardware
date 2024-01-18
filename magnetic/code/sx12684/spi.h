#ifndef  _SPI_H_
#define  _SPI_H_

void SPI_Int(void);
uint8_t SpiInOut( uint8_t txBuffer);
void SpiIn( uint8_t *txBuffer, uint16_t size );

#endif
