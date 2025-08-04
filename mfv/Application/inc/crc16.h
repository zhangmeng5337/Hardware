
#ifndef _CRC16_H_
#define _CRC16_H_

#include <stdint.h>

uint16_t crc16_ccitt ( uint16_t seed, const uint8_t *buf, int32_t len )/*@modifies nothing @*/;
unsigned short crc16_modbus ( unsigned char *puchMsg, unsigned short usDataLen );

#endif /* _CRC16_H_ */
