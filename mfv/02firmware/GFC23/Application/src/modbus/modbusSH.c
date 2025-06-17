#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "modbus.h"
#include "modbussh.h"
#include "crc16.h"
#include "main.h"
#include "utilities.h"
#include "protocol.h"

#include "meter.h"
#include "controller.h"

uint8_t __2000_SHR(modbus_pdu_struct *pdu) {
  ctrl.setpointunits = (nvmem.app.controllerObj.mode > CONTROLMODE_DIGITAL ? ctrl.asetpoint : ctrl.setpoint) * 100.0f;
  return _CONTINUE;
}

uint8_t __2000_SHW(modbus_pdu_struct *pdu) {
  
  ctrl.setpoint = ctrl.setpointunits / 100.0f;
  return _CONTINUE;
}
