#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "modbus.h"
#include "modbussh.h"
#include "crc16.h"
#include "main.h"
#include "utilities.h"
#include "protocol.h"
#include "nvmem.h"

mb_instance uart_mb_api;
mb_instance usb_mb_api;

/**
  @brief MB register mapping to internal RAM register
*/
const mb_register_struct input_regs[] = {
//  {1000, 2, _FLOAT32, &meter.flowSCCM, NULL, NULL, _READ_ONLY},
//  {1002, 2, _FLOAT32, &meter.psia, NULL, NULL, _READ_ONLY},  
//  {1004, 2, _FLOAT32, &meter.gasTempModel, NULL, NULL, _READ_ONLY},
//  {1006, 2, _FLOAT32, &meter.totalflowsccm, NULL, NULL, _READ_ONLY}, 
//  {1008, 2, _FLOAT32, &meter.totaltimesec, NULL, NULL, _READ_ONLY},   
  {0, 0, 0}  // null terminator
};

const uint16_t mb_input_stop_address = 1009; 

const mb_register_struct holding_regs[] = {
//  {2000, 2, _FLOAT32, &ctrl.setpointunits, __2000_SHR, __2000_SHW, 0},
//  {2002, 2, _FLOAT32, &ctrl.Pgain, NULL, NULL, 0},
//  {2004, 2, _FLOAT32, &ctrl.Igain, NULL, NULL, 0},
//  {2006, 2, _FLOAT32, &ctrl.Dgain, NULL, NULL, 0},
//  {2008, 2, _FLOAT32, &ctrl.icrack, NULL, NULL, 0},
//  {2010, 2,  _UINT32, &ctrl.ramprate, NULL, NULL, 0},
//  {2012, 1,   _UINT8, &ctrl.override, NULL, NULL, 0},   
//  {2013, 2, _FLOAT32, &ctrl.ctrlv, NULL, NULL, 0},  
//  {2015, 1,   _UINT8, &meter.dpzen, NULL, NULL, 0}, 
//  
//  {2020, 2, _FLOAT32, &nvmem.app.controllerObj.pgain, NULL, NULL, _FLASH},
//  {2022, 2, _FLOAT32, &nvmem.app.controllerObj.igain, NULL, NULL, _FLASH},
//  {2024, 2, _FLOAT32, &nvmem.app.controllerObj.dgain, NULL, NULL, _FLASH},
//  {2026, 2, _FLOAT32, &nvmem.app.controllerObj.icrack, NULL, NULL, _FLASH},    

  {0, 0, 0}  // null terminator
};

const uint16_t mb_holding_stop_address = 2027; 

/**
  @brief float to MB register utility converter
*/
void upd_double(uint16_t *dst, uint16_t *src) {
  dst[0] = src[3] >> 8;
  dst[1] = src[3] & 0xff;  
  dst[2] = src[2] >> 8;
  dst[3] = src[2] & 0xff;
  dst[4] = src[1] >> 8;
  dst[5] = src[1] & 0xff;  
  dst[6] = src[0] >> 8;
  dst[7] = src[0] & 0xff;
}

/**
  @brief float to MB register utility converter
*/
void upd_flt(uint8_t *dst, uint16_t *src) {  
  dst[0] = src[1] >> 8;
  dst[1] = src[1] & 0xff;  
  dst[2] = src[0] >> 8;
  dst[3] = src[0] & 0xff;  
}

/**
  @brief uint32 to MB register utility converter
*/
void upd_u32(uint8_t *dst, uint16_t *src) {
  dst[0] = src[1] >> 8;
  dst[1] = src[1] & 0xff;  
  dst[2] = src[0] >> 8;
  dst[3] = src[0] & 0xff;  
}

/**
  @brief uint16 to MB register utility converter
*/
void upd_u16(uint8_t *dst, uint16_t *src) {  
  dst[0] = src[0] >> 8;
  dst[1] = src[0] & 0xff;  
}

/**
  @brief uint16 to MB register utility converter
*/
void upd_u8(uint8_t *dst, uint16_t *src) {  
  dst[0] = 0;
  dst[1] = src[0] & 0xff;  
}

/**
  @brief MB register to float converter
*/
void upd_reg_flt(uint16_t *dst, uint8_t *src) {
  dst[1] = (src[0] << 8) | src[1];
  dst[0] = (src[2] << 8) | src[3];
}

/**
  @brief MB register to uint16 converter
*/
void upd_reg_u32(uint16_t *dst, uint8_t *src) {
  dst[1] = (src[0] << 8) | src[1];
  dst[0] = (src[2] << 8) | src[3];
}

/**
  @brief MB register to uint32 converter
*/
void upd_reg_u16(uint16_t *dst, uint8_t *src) {
  dst[0] = (src[0] << 8) | src[1];
}

/**
  @brief MB register to uint8 converter
*/
void upd_reg_u8(uint16_t *dst, uint8_t *src) {
  dst[0] = src[1];
}

/**
  @brief Generate exception response frame
*/
modbus_states mb_excep_rsp_pdu(mb_instance *mb_api, uint8_t error_code) {
  uint16_t crc16_tx;
  mb_api->comms_driver->tx.buffer[0] = mb_api->address; // device server address
  mb_api->comms_driver->tx.buffer[1] = mb_api->pdu.function | 0x80; // function code with error flag
  mb_api->comms_driver->tx.buffer[2] = error_code;
  crc16_tx = crc16_modbus(mb_api->comms_driver->tx.buffer, 3);
  mb_api->comms_driver->tx.buffer[3] = crc16_tx & 0xff;
  mb_api->comms_driver->tx.buffer[4] = crc16_tx >> 8;
  mb_api->comms_driver->tx.length = 5;  
  return MODBUS_RESPONSE;
}

/**
  @brief process function code 0x03 READ_HOLDING_REGISTERS
*/
modbus_states mb_rsp_pdu_0x03(mb_instance *mb_api) {
  uint16_t i = 0;
  uint16_t *data_tx = 0;
  uint8_t *data8ptr = 0;
  uint16_t crc16_tx = 0;
  uint16_t address = (mb_api->pdu.data[0] << 8) | mb_api->pdu.data[1];
  int16_t num_regs = (mb_api->pdu.data[2] << 8) | mb_api->pdu.data[3];

  binary temp_var;
  uint8_t (*fptr)();
  uint8_t func_resp;
  
  // search for the register
  for (i=0; i<0xFFFF; i++) {
    if (holding_regs[i].start_address == REGISTER_NULL) {
      // we hit the last element of holding register and still could not find a valid address. Return with exception code.
      return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
    } else if (address == holding_regs[i].start_address) {
      // we found the address, proceed
      break;
    }    
  }  
  // check if requested read range on multple register reads is valid
  if ((address + num_regs - 1) > mb_holding_stop_address) {
    return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
  }

  // build response frame and send out
  mb_api->comms_driver->tx.buffer[0] = mb_api->address; // device server address
  mb_api->comms_driver->tx.buffer[1] = mb_api->pdu.function; // feedback function code
  mb_api->comms_driver->tx.buffer[2] = num_regs * 2;
  mb_api->comms_driver->tx.length = 3;
  data8ptr = &mb_api->comms_driver->tx.buffer[3];

  while (num_regs > 0) {
    
    if (holding_regs[i].readFunction != NULL) {  // does register require special write handler?
      fptr = holding_regs[i].readFunction;
      func_resp = (*fptr)(mb_api->pdu);
      if (func_resp == _FAIL) {
          return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_VALUE);
      } 
    }    
        
    data_tx = (uint16_t*)holding_regs[i].memptr;  
    switch (holding_regs[i].dataType) {
      case _FLOAT32:
        upd_flt(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 4;
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT32:
      case _INT32:
        upd_u32(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 4;
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT16:
      case _INT16:
        upd_u16(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 2;
        data8ptr += 2;
        num_regs -= 1;
        break;         
      case _UINT8:
      case _INT8:    
        upd_u8(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 2;
        data8ptr += 2;
        num_regs -= 1;
        break;
      default:
        return mb_excep_rsp_pdu(mb_api, SERVER_DEVICE_FAILURE);
        break; 
    }
     
    i++;
  }  
  
  if (num_regs < 0) { // if this index is less than 0, it means that register addressing is invalid
    return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
  }
  
  crc16_tx = crc16_modbus(mb_api->comms_driver->tx.buffer, mb_api->comms_driver->tx.length);
  *data8ptr++ = crc16_tx & 0xff;
  *data8ptr++ = crc16_tx >> 8;
  mb_api->comms_driver->tx.length += 2;  
  return MODBUS_RESPONSE;
}

/**
  @brief process function code 0x04 READ_INPUT_REGISTERS
*/
modbus_states mb_rsp_pdu_0x04(mb_instance *mb_api) {
  uint16_t i = 0;
  uint16_t *data_tx = 0;
  uint8_t *data8ptr = 0;
  uint16_t crc16_tx = 0;
  uint16_t address = (mb_api->pdu.data[0] << 8) | mb_api->pdu.data[1];
  int16_t num_regs = (mb_api->pdu.data[2] << 8) | mb_api->pdu.data[3];

  // search for the register
  for (i=0; i<0xFFFF; i++) {
    if (input_regs[i].start_address == REGISTER_NULL) {
      // we hit the last element of holding register and still could not find a valid address. Return with exception code.
      return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
    } else if (address == input_regs[i].start_address) {
      // we found the address, proceed
      break;
    }    
  }  
  // check if requested read range on multple register reads is valid
  if ((address + num_regs - 1) > mb_input_stop_address) {
    return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
  }

  // build response frame and send out
  mb_api->comms_driver->tx.buffer[0] = mb_api->address; // device server address
  mb_api->comms_driver->tx.buffer[1] = mb_api->pdu.function; // feedback function code
  mb_api->comms_driver->tx.buffer[2] = num_regs * 2;
  mb_api->comms_driver->tx.length = 3;
  data8ptr = &mb_api->comms_driver->tx.buffer[3];

  while (num_regs > 0) {
    data_tx = (uint16_t*)input_regs[i].memptr;  
    switch (input_regs[i].dataType) {
      case _FLOAT32:
        upd_flt(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 4;
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT32:
      case _INT32:
        upd_u32(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 4;
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT16:
      case _INT16:     
        upd_u16(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 2;
        data8ptr += 2;
        num_regs -= 1;
        break;         
      case _UINT8:
      case _INT8:    
        upd_u8(data8ptr, data_tx);
        mb_api->comms_driver->tx.length += 2;
        data8ptr += 2;
        num_regs -= 1;
        break;
      default:
        return mb_excep_rsp_pdu(mb_api, SERVER_DEVICE_FAILURE);
        break;            
    }
    
    i++;
  }  
  
  if (num_regs < 0) { // if this index is less than 0, it means that register addressing is invalid
    return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
  }
  
  crc16_tx = crc16_modbus(mb_api->comms_driver->tx.buffer, mb_api->comms_driver->tx.length);
  *data8ptr++ = crc16_tx & 0xff;
  *data8ptr++ = crc16_tx >> 8;
  mb_api->comms_driver->tx.length += 2;  
  return MODBUS_RESPONSE;
}

/**
  @brief process function code 16 WRITE_MULTIPLE_REGISTER
*/
modbus_states mb_rsp_pdu_0x10(mb_instance *mb_api) {
  uint16_t i = 0;
  uint16_t *data_rx = 0;
  uint8_t *data8ptr = 0;
  uint16_t crc16_tx = 0;
  uint16_t address = (mb_api->pdu.data[0] << 8) | mb_api->pdu.data[1];
  int16_t num_regs = (mb_api->pdu.data[2] << 8) | mb_api->pdu.data[3];
  uint8_t bytecount = mb_api->pdu.data[4];
  
  binary temp_var;
  uint8_t (*fptr)();
  uint8_t func_resp;

  // search for the register
  for (i=0; i<0xFFFF; i++) {
    if (holding_regs[i].start_address == REGISTER_NULL) {
      // we hit the last element of holding register and still could not find a valid address. Return with exception code.
      return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
    } else if (address == holding_regs[i].start_address) {
      // we found the address, proceed
      break;
    }    
  }  
  
  // check for legal data values
  if (!(((num_regs >= 1) && (num_regs <= 0x007b)) &&
     (bytecount == num_regs*2))) {
     return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_VALUE);
  }
  
  // check if requested write range on multple register write is valid
  if ((address + num_regs - 1) > mb_holding_stop_address) {
    return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
  }
  
  while (num_regs > 0) {
    data_rx = (uint16_t*)holding_regs[i].memptr;
    data8ptr = &mb_api->pdu.data[5];
    
    switch (holding_regs[i].dataType) { // update internal registers
      case _FLOAT32:
        upd_reg_flt(data_rx, data8ptr);
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT32:
      case _INT32:
        upd_reg_u32(data_rx, data8ptr);
        data8ptr += 4;
        num_regs -= 2;
        break;
      case _UINT16:
      case _INT16:      
        upd_reg_u16(data_rx, data8ptr);        
        data8ptr += 2;
        num_regs -= 1;
        break;        
      case _UINT8:
      case _INT8:    
        upd_reg_u8(data_rx, data8ptr);        
        data8ptr += 2;
        num_regs -= 1;
        break; 
      default:
        return mb_excep_rsp_pdu(mb_api, SERVER_DEVICE_FAILURE);
        break;      
    }
    
    if (holding_regs[i].writeFunction != NULL) {  // does register require special write handler?
      fptr = holding_regs[i].writeFunction;
      func_resp = (*fptr)(mb_api->pdu);
      if (func_resp == _FAIL) {
          return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_VALUE);
      } 
    }    
    
    if (holding_regs[i].accessRights & (uint32_t)_FLASH) {  // is register a non-volatile sotrage element?
      // start flash write timeout
      write_to_flash(FLASH_TIMEOUT);
    }
    i++;
  }  

  // build response frame and send out
  mb_api->comms_driver->tx.buffer[0] = mb_api->address; // device server address
  mb_api->comms_driver->tx.buffer[1] = mb_api->pdu.function; // feedback function code
  mb_api->comms_driver->tx.buffer[2] = mb_api->pdu.data[0];
  mb_api->comms_driver->tx.buffer[3] = mb_api->pdu.data[1];
  mb_api->comms_driver->tx.buffer[4] = mb_api->pdu.data[2];
  mb_api->comms_driver->tx.buffer[5] = mb_api->pdu.data[3];
  mb_api->comms_driver->tx.length = 6;
  data8ptr = &mb_api->comms_driver->tx.buffer[6];
  crc16_tx = crc16_modbus(mb_api->comms_driver->tx.buffer, mb_api->comms_driver->tx.length);
  *data8ptr++ = crc16_tx & 0xff;
  *data8ptr++ = crc16_tx >> 8;
  mb_api->comms_driver->tx.length += 2;  
  return MODBUS_RESPONSE;
}

/**
  @brief process function code 6 WRITE_SINGLE_REGISTER
*/
modbus_states mb_rsp_pdu_0x06(mb_instance *mb_api) {
  uint16_t i = 0;
  uint16_t *data_rx = 0;
  uint8_t *data8ptr = 0;
  uint16_t crc16_tx = 0;
  uint16_t address = (mb_api->pdu.data[0] << 8) | mb_api->pdu.data[1];
  int16_t num_regs = 1;
  uint8_t bytecount = 2;
  
  binary temp_var;
  uint8_t (*fptr)();
  uint8_t func_resp;

  // search for the register
  for (i=0; i<0xFFFF; i++) {
    if (holding_regs[i].start_address == REGISTER_NULL) {
      // we hit the last element of holding register and still could not find a valid address. Return with exception code.
      return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_ADDRESS);
    } else if (address == holding_regs[i].start_address) {
      // we found the address, proceed
      break;
    }    
  }      

  data_rx = (uint16_t*)holding_regs[i].memptr;
  data8ptr = &mb_api->pdu.data[2];
  
  switch (holding_regs[i].dataType) { // update internal registers
    case _UINT16:
    case _INT16:      
      upd_reg_u16(data_rx, data8ptr);        
      data8ptr += 2;
      num_regs -= 1;
      break;        
    case _UINT8:
    case _INT8:    
      upd_reg_u8(data_rx, data8ptr);        
      data8ptr += 2;
      num_regs -= 1;
      break;          
    default:
      return mb_excep_rsp_pdu(mb_api, SERVER_DEVICE_FAILURE);
      break;
  }
  
  if (holding_regs[i].writeFunction != NULL) {  // does register require special write handler?
    fptr = holding_regs[i].writeFunction;
    func_resp = (*fptr)(mb_api->pdu);
    if (func_resp == _FAIL) {
        return mb_excep_rsp_pdu(mb_api, ILLEGAL_DATA_VALUE);
    } 
  }    
  
  if (holding_regs[i].accessRights & (uint32_t)_FLASH) {  // is register a non-volatile sotrage element?
    // start flash write timeout
    write_to_flash(FLASH_TIMEOUT);
  }


  // build response frame and send out
  mb_api->comms_driver->tx.buffer[0] = mb_api->address; // device server address
  mb_api->comms_driver->tx.buffer[1] = mb_api->pdu.function; // feedback function code
  mb_api->comms_driver->tx.buffer[2] = mb_api->pdu.data[0];
  mb_api->comms_driver->tx.buffer[3] = mb_api->pdu.data[1];
  mb_api->comms_driver->tx.buffer[4] = mb_api->pdu.data[2];
  mb_api->comms_driver->tx.buffer[5] = mb_api->pdu.data[3];
  mb_api->comms_driver->tx.length = 6;
  data8ptr = &mb_api->comms_driver->tx.buffer[6];
  crc16_tx = crc16_modbus(mb_api->comms_driver->tx.buffer, mb_api->comms_driver->tx.length);
  *data8ptr++ = crc16_tx & 0xff;
  *data8ptr++ = crc16_tx >> 8;
  mb_api->comms_driver->tx.length += 2;  
  return MODBUS_RESPONSE;
}

/**
  @brief INIT state handles activation of RX interrupt as well as drive direction if using RS485 as the physical layer
*/
static void modbus_init (comms_struct *comms, mb_instance *mb_api) {
  static uint32_t shwait = UART_SHUTDOWN_DELAY;
  
  // check if we we have a pending transmit operation
  if (comms->tx.pending > 0) {
      // bail out, wait until any pending transmission is done
    if (comms->tx.pending == 2) {  // we have a pending shutdown
      shwait--;
      if (shwait == 0) {
        comms->tx.pending = 0; // shut the transceiver down
        shwait = uart_shutdown_delay;
      }
    }
  } else { // ready to receive packet
    switch (comms->source) {
      case SOURCE_USB:
        mb_api->current_state = MODBUS_IDLE;  // switch to next state
        break;
      case SOURCE_UART:
        RS485_RXEN;
      case SOURCE_UART_EXTPROC:
      default:        
        HAL_UART_Receive_IT(&TOOLINTERFACE, &comms->rx.data, 1); // activate RX interrupt with 1 byte threshold
        mb_api->current_state = MODBUS_IDLE;  // switch to next state
        break;
    }
  }
}

/**
  @brief IDLE state is were we wait for a RX frame pending flag. Proper tout must be checked per MODBUS serial protocol spec.
*/
static void modbus_idle (comms_struct *comms, mb_instance *mb_api) {
  // check if we have pending data
	if (comms->rx.pending) {
		mb_api->current_state = MODBUS_CHECK_REQ; // move to chekc frame request
	}
}

/**
  @brief CHECK REQ handles address and frame validaity. If ADU is not for this device (by checking address), just go to INIT state.
         If ADU is for device, framw is checked by calcualting crc16 based on modbus specifications.
*/
static void modbus_check_req (comms_struct *comms, mb_instance *mb_api) {

  // check address
  if (comms->rx.buffer[0] != mb_api->address) {
    comms->rx.ctr = 0;
    comms->rx.pending = 0;
    mb_api->current_state = MODBUS_INIT;
    return;  
  }
  
//  if (comms->source == SOURCE_UART) {
//    RS485_TXEN  // data is for me
//  }
  
  // check for framing errors 
  if (crc16_modbus(comms->rx.buffer, comms->rx.ctr-2) != ((comms->rx.buffer[comms->rx.ctr-1] << 8) | comms->rx.buffer[comms->rx.ctr-2])) {
    comms->rx.ctr = 0;
    comms->rx.pending = 0;
    // increment framing error counter here!
    mb_api->current_state = MODBUS_INIT;
    return;
  }  
  
  // frame is good, proceed to process state
  mb_api->pdu.function = comms->rx.buffer[1]; // get function code
  mb_api->pdu.data = &comms->rx.buffer[2];  // get pointer to data
  mb_api->pdu.length = comms->rx.ctr - 4; // PDU length = total length - (address, function code, crclo, crchi)
  mb_api->current_state = MODBUS_PROCESSING;
}

/**
  @brief This is where we process PDU data and respond accordingly.
*/
static void modbus_processing (comms_struct *comms, mb_instance *mb_api) {
  
  switch (mb_api->pdu.function) {
    case READ_HOLDING_REGISTERS:
      mb_api->current_state = mb_rsp_pdu_0x03(mb_api);
      break;    
    case READ_INPUT_REGISTERS: 
      mb_api->current_state = mb_rsp_pdu_0x04(mb_api);
      break;
    case WRITE_MULTIPLE_REGISTERS:
      mb_api->current_state = mb_rsp_pdu_0x10(mb_api);
      break;      
    case READ_COILS:
    case READ_DISCRETE_INPUTS:
    case WRITE_COILS:
    case WRITE_SINGLE_REGISTER:
      mb_api->current_state = mb_rsp_pdu_0x06(mb_api);
      break;
    case READ_EXCEPTION_STATUS:
    case DIAGNOSTICS:
    case GET_COMM_EVENT_COUNTER:
    case GET_COMM_EVENT_LOG:
    case WRITE_MULTIPLE_COILS:
    case REPORT_SERVER_ID:
    case READ_FILE_RECORD:
    case WRITE_FILE_RECORD:
    case MASK_WRITE_REGISTER:
    case READ_WRITE_MULTIPLE_REGS:
    case READ_FIFO_QUEUE:
    case ENCAPSULATED_INTERFACE_TRANSPORT:
    case CANOPEN_GENERAL_REFERNCE_REQ_RESP:
    case READ_DEVICE_IDENTIFICATION:
    default:
      mb_api->current_state = mb_excep_rsp_pdu(mb_api, ILLEGAL_FUNCTION);
      break;  
  } 
}

static void modbus_response (comms_struct *comms, mb_instance *mb_api) {
  comms->rx.ctr = 0;
  comms->rx.pending = 0;
  if (comms->source == SOURCE_UART) {
    RS485_TXEN  // data is for me
  }
  comms_send_bytes(comms, comms->tx.buffer, comms->tx.length);
  mb_api->current_state = MODBUS_INIT;
}

/**
  @brief initialize UART MODBUS API. This is where we create the hook for low level driver management
*/
void init_UART_MB_API (uint8_t address) {
  
  memset(&uart_mb_api, 0, sizeof(mb_instance));
  uart_com_driver.source = SOURCE_UART;
  uart_com_driver.tx.pending = 0;
  uart_mb_api.address = address;
  uart_mb_api.comms_driver = &uart_com_driver;
}

/**
  @brief initialize USB MODBUS API. This is where we create the hook for low level driver management
*/
void init_USB_MB_API  (uint8_t address) {
  
  memset(&usb_mb_api, 0, sizeof(mb_instance));
  usbd_com_driver.source = SOURCE_USB;
  usbd_com_driver.tx.pending = 0;
  usb_mb_api.address = address;
  usb_mb_api.comms_driver = &usbd_com_driver;
}

/**
    @brief This is the protcol state machine table. This table of function is used to traverse into different states, instead of using a 
           switch. Make sure to properly map the state index with the function that is inteded for that particular state.
*/
static void (*const mb_state_table[]) (comms_struct *comms, mb_instance *mb_api) = {
  modbus_init,
	modbus_idle,
  modbus_check_req,
  modbus_processing,
  modbus_response
};

/**
    @brief This is where the communicaitons protocol state machine is implemented.
*/
void modbus_sm (mb_instance *api)
{   
    if (api->current_state >= MODBUS_STATEERROR) {
        /* invalid event/state - handle appropriately */
    } else {
        mb_state_table [api->current_state ] (api->comms_driver, api); /* call the action procedure */        
    }
}
