#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdint.h>
#include "main.h"
#include "commsAPI.h"

// MODBUS Exception Code
#define ILLEGAL_FUNCTION      0x01
#define ILLEGAL_DATA_ADDRESS  0x02
#define ILLEGAL_DATA_VALUE    0x03
#define SERVER_DEVICE_FAILURE 0x04
#define ACKNOWLEDFE           0x05
#define SERVER_DEVICE_BUSY    0x06
#define MEMORY_PARITY_ERROR   0x08
#define GATEWAY_PATH_UNAVAILABLE  0x0A
#define GATEWAY_RESPONSE_FAILURE  0x0B

// FUNCTION Code
#define READ_COILS                0x01
#define READ_DISCRETE_INPUTS      0x02
#define READ_HOLDING_REGISTERS    0x03
#define READ_INPUT_REGISTERS      0x04
#define WRITE_COILS               0x05
#define WRITE_SINGLE_REGISTER     0x06
#define READ_EXCEPTION_STATUS     0x07
#define DIAGNOSTICS               0x08
#define GET_COMM_EVENT_COUNTER    0x0B
#define GET_COMM_EVENT_LOG        0x0C
#define WRITE_MULTIPLE_COILS      0x0F
#define WRITE_MULTIPLE_REGISTERS  0x10
#define REPORT_SERVER_ID          0x11
#define READ_FILE_RECORD          0x14
#define WRITE_FILE_RECORD         0x15
#define MASK_WRITE_REGISTER       0x16
#define READ_WRITE_MULTIPLE_REGS  0x17
#define READ_FIFO_QUEUE           0x18
#define ENCAPSULATED_INTERFACE_TRANSPORT  0x2B
#define CANOPEN_GENERAL_REFERNCE_REQ_RESP  0x0D
#define READ_DEVICE_IDENTIFICATION  0x0E

#define MB_FLUID_MODE             1
#define MB_TARE                   4
#define MB_TOTALIZER_RESET        5
#define MB_CHANGE_CTRL_VAR        11
#define MB_SAVE_SP_POWERUP        12
#define MB_TOTALIZER_ENABLE       15
#define MB_VALVE_OVERRIDE         16
#define MB_SAVE_CONFIGURATION     17
#define MB_CONTROL_MODE           18
#define MB_BATCH_RUN              19

#define MB_CHANGE_SLAVE_ID        32767
#define MB_CHANGE_BAUDRATE        32768

#define MB_CMD_SUCCESS            0x0000
#define MB_CMD_INVALID            0x8001
#define MB_SETTING_INVALID        0x8002
#define MB_UNSUPPORTED            0x8003

typedef enum {
	MODBUS_INIT = 0,
  MODBUS_IDLE,
  MODBUS_CHECK_REQ,
  MODBUS_PROCESSING,
  MODBUS_RESPONSE,
  MODBUS_STATEERROR
} modbus_states;

typedef struct {
  uint8_t function;
  uint8_t *data;
  uint8_t length;
} modbus_pdu_struct;

typedef struct {
  uint8_t address;
  comms_struct *comms_driver;
  modbus_states current_state;
  modbus_pdu_struct pdu;
} mb_instance;


typedef enum {
  REGISTER_NULL = 0,
  REGISTER_READ_ONLY,
  REGSITER_WRITE_ONLY,
  REGISTER_READ_WRITE
} mb_access;

typedef struct {
  uint16_t start_address; // register address (hex)
  uint16_t dataSize;      // size of the data type on the wire (in bytes)  
  uint8_t dataType;       // data type of the command
  void *memptr;           // pointer to memory location volatile
  uint8_t (*readFunction)(modbus_pdu_struct *pdu);     // pointer to read function
  uint8_t (*writeFunction)(modbus_pdu_struct *pdu);    // pointer to write function
  uint32_t accessRights;  // access rights to this command
} mb_register_struct;

typedef union {
	uint8_t  _uint8;
	uint16_t _uint16;
	uint32_t _uint32;
	uint64_t _uint64;
	float   _float;
  double  _double;
  uint8_t _arrU8[8];
} mb_binary;

extern mb_instance uart_mb_api;
extern mb_instance usb_mb_api;

extern void init_USB_MB_API (uint8_t address);
extern void init_UART_MB_API (uint8_t address);
extern void modbus_sm(mb_instance *api);

extern void upd_flt(uint8_t *dst, uint16_t *src);
extern void modbus_update_objects(void);

#endif
