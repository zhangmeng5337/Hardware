#ifndef EEPROM_H
#define EEPROM_H
#include "uartParse.h"
params_typedef *eeprom_data_read(void);
en_result_t eeprom_write(params_typedef params);
#endif
