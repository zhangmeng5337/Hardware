#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include <stdint.h>
#include "main.h"


#define APPLICATION_ADDRESS      (uint32_t)FLASH_APP_START_ADDR
#define BL_VERSION               0x20000000
  
typedef struct {
    uint32_t magicnumber;
    uint32_t app_size;
    uint16_t app_crc16_ccitt;
    uint16_t header_crc16_ccitt;
} app_header;

typedef struct {
    app_header app;
    uint8_t spares[FLASH_PAGE_SIZE - sizeof(app_header)];
} header;

typedef void (*pFunction) (void);

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
extern char *blver;

extern uint8_t validate_app_image(void);
extern uint8_t update_app_image(void);
extern void runApplicationFirmware(void);

#endif
