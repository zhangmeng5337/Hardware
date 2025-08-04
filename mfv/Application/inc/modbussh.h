#ifndef __MODBUSSH_H
#define __MODBUSSH_H

#include <stdint.h>
#include "main.h"
#include "modbus.h"

extern uint8_t __2000_SHR(modbus_pdu_struct *pdu);
extern uint8_t __2000_SHW(modbus_pdu_struct *pdu);

#endif