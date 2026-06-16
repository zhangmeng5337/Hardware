#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "protocol.h"
#include "pressure.h"
#include "meter.h"
#include "controller.h"
#include "utilities.h"
#include "dnsengine.h"

#pragma pack(push,1)    // make sure we byte align the structure
// PollIO 0x01
typedef struct {
  uint8_t override;
  binary setpoint;
} in_1_struct;

typedef struct {  
  binary flow;
  binary pm;
  binary temperature;
  binary valve;
  binary setpoint;  
  uint8_t override;
  uint8_t status;
  uint64_t warning;
  uint64_t alarm;
} out_1_struct;

in_1_struct input_0x01 = {0};
out_1_struct output_0x01 = {0};
#pragma pack(pop)

/**
  @breif
*/
binary dns_calculate_value(float normalized, uint16_t cscale, float fscale, dns_datatype dtype, dns_dataunits dunits) {
  binary val;
  
  switch (dunits) {
    case DNS_DATAUNITS_COUNTS:
      if (dtype == DNS_DATATYPE_INT) {        
        val._sint16 = normalized * cscale;
      } else if (dtype == DNS_DATATYPE_REAL) {
        val._float = normalized * cscale;
      }      
      break;
    case DNS_DATAUNITS_SCCM:
    // pressure
    // temperature
    // mdot
    // drive in volts
      val._float = normalized * fscale;
      break;
    case DNS_DATAUNITS_PERCENT:
      val._float = normalized * 100.0f;
      break;
  } 
  
  return val;
}

/**
*/
static uint8_t cmd10w(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  uint16_t prodcons = 0;
  
  // Recieve producer and consumber settings
  memcpy((uint8_t*)&prodcons, &cmdInfo_lcl->data_array[0], sizeof(prodcons));
  nvmem.app.network.producer = prodcons >> 8;
  nvmem.app.network.consumer = prodcons & 0xff;
  return _SUCCESS;
}

/**
*/
static uint8_t cmd11w(uint8_t *cmdInfo) {
  command_info *cmdInfo_lcl = (command_info *)cmdInfo;
  binary val;
  
  // Recieve Poll-IO
  switch (nvmem.app.network.consumer) {
    case 8:
      memcpy((uint8_t*)&input_0x01.override, &cmdInfo_lcl->data_array[0], sizeof(input_0x01.override)+sizeof(input_0x01.setpoint._uint16));
      if (dns.devstate == EXECUTING_STATUS) {
        ctrl.override = input_0x01.override;
        ctrl.setpoint = input_0x01.setpoint._uint16 * 4.0690104167e-5f;  // 1/0x6000      
      }
      break;
    case 19:
      memcpy((uint8_t*)&input_0x01.setpoint._float, &cmdInfo_lcl->data_array[0], sizeof(input_0x01.setpoint._float));
      if (dns.devstate == EXECUTING_STATUS) {
        if (nvmem.app.sccObj.dUnits == DNS_DATAUNITS_COUNTS) {
          ctrl.setpoint = input_0x01.setpoint._float * 4.0690104167e-5f;
        } else {
          ctrl.setpoint = input_0x01.setpoint._float / nvmem.app.sasObj[0].fsrange; 
        }       
      }
      break;
    case 20:
      memcpy((uint8_t*)&input_0x01.override, &cmdInfo_lcl->data_array[0], sizeof(input_0x01.override)+sizeof(input_0x01.setpoint._float));
      if (dns.devstate == EXECUTING_STATUS) {
        ctrl.override = input_0x01.override;
        if (nvmem.app.sccObj.dUnits == DNS_DATAUNITS_COUNTS) {
          ctrl.setpoint = input_0x01.setpoint._float * 4.0690104167e-5f;
        } else {
          ctrl.setpoint = input_0x01.setpoint._float / nvmem.app.sasObj[0].fsrange; 
        } 
      }      
      break;
    case 7:
    default:
      memcpy((uint8_t*)&input_0x01.setpoint._uint16, &cmdInfo_lcl->data_array[0], sizeof(input_0x01.setpoint._uint16));
      if (dns.devstate == EXECUTING_STATUS) {
        ctrl.setpoint = input_0x01.setpoint._uint16 * 4.0690104167e-5f;  // 1/0x6000
      }
      break;
  }
  
  switch (nvmem.app.network.producer) {
    case 1:
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._sint16 = val._sint16;    
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.flow._uint16);
      break;
    case 3:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._sint16 = val._sint16;  
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      output_0x01.valve._uint16 = DNS_FS_COUNTS * ctrl.ctrlv;
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._sint16 = val._sint16;     
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.valve._uint16, sizeof(output_0x01.valve._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.valve._uint16);
      break;
    case 4:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);    
      output_0x01.flow._sint16 = val._sint16;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.setpoint._uint16, sizeof(output_0x01.setpoint._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.setpoint._uint16);
      break;
    case 5:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);    
      output_0x01.flow._sint16 = val._sint16;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.setpoint._uint16, sizeof(output_0x01.setpoint._uint16));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.valve._uint16, sizeof(output_0x01.valve._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.setpoint._uint16) + 
                               sizeof(output_0x01.valve._uint16);
      break;
    case 6:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);    
      output_0x01.flow._sint16 = val._sint16;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.setpoint._uint16, sizeof(output_0x01.setpoint._uint16));
      output_0x01.override = ctrl.override;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.override, sizeof(output_0x01.override));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[6], (uint8_t*)&output_0x01.valve._uint16, sizeof(output_0x01.valve._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.setpoint._uint16) + 
                               sizeof(output_0x01.override) + 
                               sizeof(output_0x01.valve._uint16);
      break;
    case 9:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status);
      break;
#if MFC_PROFILE    
    case 10:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));      
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.alarm, sizeof(output_0x01.alarm));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.alarm);
      break;
    case 11:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));      
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.warning, sizeof(output_0x01.warning));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) +
                               sizeof(output_0x01.warning);
      break;
    case 12:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.alarm, sizeof(output_0x01.alarm));
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[8], (uint8_t*)&output_0x01.warning, sizeof(output_0x01.warning));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.alarm) + 
                               sizeof(output_0x01.warning);
      break;
#endif    
    case 13:
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.flow._float);
      break;
    case 14:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));    
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float);
      break;
    case 15:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._float = val._float;  
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.valve._float, sizeof(output_0x01.valve._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.valve._float);
      break;
    case 16:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.setpoint._float, sizeof(output_0x01.setpoint._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.setpoint._float);
      break;
    case 17:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.setpoint._float, sizeof(output_0x01.setpoint._float));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[9], (uint8_t*)&output_0x01.valve._float, sizeof(output_0x01.valve._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.setpoint._float) + 
                               sizeof(output_0x01.valve._float);
      break;
    case 18:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(ctrl.setpoint, 
                                DNS_FS_COUNTS,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sccObj.dType,
                                nvmem.app.sccObj.dUnits);    
      output_0x01.setpoint._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.setpoint._float, sizeof(output_0x01.setpoint._float));
      output_0x01.override = ctrl.override;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[9], (uint8_t*)&output_0x01.override, sizeof(output_0x01.override));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[10], (uint8_t*)&output_0x01.valve._float, sizeof(output_0x01.valve._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.setpoint._float) + 
                               sizeof(output_0x01.override) + 
                               sizeof(output_0x01.valve._float);
      break;
#ifndef MFC_PROFILE                               
    case 21:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._sint16 = val._sint16;    
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      val = dns_calculate_value(meter.dnet_sas[1].value, 
                                nvmem.app.sasObj[1].fscounts,
                                nvmem.app.sasObj[1].fsrange,
                                nvmem.app.sasObj[1].dType,
                                nvmem.app.sasObj[1].dUnits);
      output_0x01.pm._sint16 = val._sint16;    
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.pm._uint16, sizeof(output_0x01.pm._uint16));
      val = dns_calculate_value(meter.dnet_sas[2].value, 
                                nvmem.app.sasObj[2].fscounts,
                                nvmem.app.sasObj[2].fsrange,
                                nvmem.app.sasObj[2].dType,
                                nvmem.app.sasObj[2].dUnits);
      output_0x01.temperature._sint16 = val._sint16;    
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.temperature._uint16, sizeof(output_0x01.temperature._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.pm._uint16) + 
                               sizeof(output_0x01.temperature._uint16);
      break;
    case 22:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._sint16 = val._sint16; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._sint16 = val._sint16;  
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[3], (uint8_t*)&output_0x01.valve._uint16, sizeof(output_0x01.valve._uint16));    
      val = dns_calculate_value(meter.dnet_sas[1].value, 
                                nvmem.app.sasObj[1].fscounts,
                                nvmem.app.sasObj[1].fsrange,
                                nvmem.app.sasObj[1].dType,
                                nvmem.app.sasObj[1].dUnits);
      output_0x01.pm._sint16 = val._sint16;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.pm._uint16, sizeof(output_0x01.pm._uint16));
      val = dns_calculate_value(meter.dnet_sas[2].value, 
                                nvmem.app.sasObj[2].fscounts,
                                nvmem.app.sasObj[2].fsrange,
                                nvmem.app.sasObj[2].dType,
                                nvmem.app.sasObj[2].dUnits);
      output_0x01.temperature._sint16 = val._sint16;    
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[7], (uint8_t*)&output_0x01.temperature._uint16, sizeof(output_0x01.temperature._uint16));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16) + 
                               sizeof(output_0x01.valve._uint16) +                                  
                               sizeof(output_0x01.pm._uint16) + 
                               sizeof(output_0x01.temperature._uint16);
      break;
    case 23:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(meter.dnet_sas[1].value, 
                          nvmem.app.sasObj[1].fscounts,
                          nvmem.app.sasObj[1].fsrange,
                          nvmem.app.sasObj[1].dType,
                          nvmem.app.sasObj[1].dUnits);
      output_0x01.pm._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.pm._float, sizeof(output_0x01.pm._float));
      val = dns_calculate_value(meter.dnet_sas[2].value, 
                          nvmem.app.sasObj[2].fscounts,
                          nvmem.app.sasObj[2].fsrange,
                          nvmem.app.sasObj[2].dType,
                          nvmem.app.sasObj[2].dUnits);
      output_0x01.temperature._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[9], (uint8_t*)&output_0x01.temperature._float, sizeof(output_0x01.temperature._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.pm._float) + 
                               sizeof(output_0x01.temperature._float);
      break;
    case 24:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                          nvmem.app.sasObj[0].fscounts,
                          nvmem.app.sasObj[0].fsrange,
                          nvmem.app.sasObj[0].dType,
                          nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._float, sizeof(output_0x01.flow._float));
      val = dns_calculate_value(ctrl.ctrlv, 
                                DNS_FS_COUNTS,
                                DNS_FS_VOLTS,
                                nvmem.app.saaObj.dType,
                                nvmem.app.saaObj.dUnits);
      output_0x01.valve._float = val._float; 
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[5], (uint8_t*)&output_0x01.valve._float, sizeof(output_0x01.valve._float));    
      val = dns_calculate_value(meter.dnet_sas[1].value, 
                          nvmem.app.sasObj[1].fscounts,
                          nvmem.app.sasObj[1].fsrange,
                          nvmem.app.sasObj[1].dType,
                          nvmem.app.sasObj[1].dUnits);
      output_0x01.pm._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[9], (uint8_t*)&output_0x01.pm._float, sizeof(output_0x01.pm._float));
      val = dns_calculate_value(meter.dnet_sas[2].value, 
                          nvmem.app.sasObj[2].fscounts,
                          nvmem.app.sasObj[2].fsrange,
                          nvmem.app.sasObj[2].dType,
                          nvmem.app.sasObj[2].dUnits);
      output_0x01.temperature._float = val._float;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[13], (uint8_t*)&output_0x01.temperature._float, sizeof(output_0x01.temperature._float));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._float) + 
                               sizeof(output_0x01.valve._float) +                                  
                               sizeof(output_0x01.pm._float) + 
                               sizeof(output_0x01.temperature._float);
      break;
    case 25:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.alarm, sizeof(output_0x01.alarm));
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[9], (uint8_t*)&output_0x01.warning, sizeof(output_0x01.warning));
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.alarm) + 
                               sizeof(output_0x01.warning);
      break;
#endif
    case 2:
    default:
      output_0x01.status = dns.eStatus;
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[0], (uint8_t*)&output_0x01.status, sizeof(output_0x01.status));
      val = dns_calculate_value(meter.dnet_sas[0].value, 
                                nvmem.app.sasObj[0].fscounts,
                                nvmem.app.sasObj[0].fsrange,
                                nvmem.app.sasObj[0].dType,
                                nvmem.app.sasObj[0].dUnits);
      output_0x01.flow._sint16 = val._sint16;      
      memcpy((uint8_t*)&cmdInfo_lcl->data_array[1], (uint8_t*)&output_0x01.flow._uint16, sizeof(output_0x01.flow._uint16));    
      cmdInfo_lcl->data_size = sizeof(output_0x01.status) + 
                               sizeof(output_0x01.flow._uint16);
      break;                               
  }
  
  return _CONTINUE;
}
// 10
const command_struct cmdPollIO[] = 
{

  {10,   _UINT16,       2,            0x00,   0x00,     &cmd10w,      0x00  },
  {11, _OCTETSTR,    0x00,            0x00,   0x00,     &cmd11w,      0x00  },
  {12,    _UINT8,       1,   &dns.devstate,   0x00,        0x00,      0x00  },
  
  {20,    _UINT8,       1,  &dns.dStatus,   0x00,   0x00,   0x00  },
  {21,    _UINT8,       1,  &dns.eStatus,   0x00,   0x00,   0x00  }, 
  {22, _OCTETSTR,  sizeof(dns.aException),  &dns.aException, 0x00,   0x00,   0x00  },
  {23, _OCTETSTR,  sizeof(dns.wException),  &dns.wException, 0x00,   0x00,   0x00  },


  
  {0x0000,   0x00,    0x00,   0x00,   0x00,   0x00}
};
