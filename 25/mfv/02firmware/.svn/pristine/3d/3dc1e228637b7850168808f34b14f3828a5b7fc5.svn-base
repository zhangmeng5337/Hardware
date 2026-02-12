#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>
#include "main.h"
#include "commsAPI.h"

#define ACK         0x06
#define NAK         0x16

#define CMD_WRITE   0x01
#define CMD_READ    0x02
#define CMD_WRITEREAD 0x03

#define _READ_ONLY  0x00000001
#define _WRITE_ONLY 0x00000002
#define _FLASH      0x00000004

#define _SUCCESS    1
#define _FAIL       2
#define _CONTINUE   3
#define _NORESP     4

#define _UINT8      1
#define _UINT16     2
#define _UINT32     3
#define _UINT64     4
#define _FLOAT32    5
#define _INT8       6
#define _INT16      7
#define _INT32      8
#define _INT64      9
#define _OCTETSTR   10

#define MAX_STREAMABLE_SIGNALS  5

typedef enum {
	IDLE = 0,
	RXWAIT,
	PARSEPACKET,
	SEARCHTABLE,
	BUILDREAD,
	BUILDWRITE,
	ACKRESPONSE,
	NAKRESPONSE,
	STATEERROR
} protocol_states;

typedef struct {
  uint16_t cmdSpecific;   // specific command ID
  uint8_t dataType;       // data type of the command
  uint16_t dataSize;      // size of the data type on the wire (in bytes)
  void *memptr;           // pointer to memory location (volatile or nonvolatile)
  uint8_t (*readFunction)(uint8_t *cmdInfo);     // pointer to read function
  uint8_t (*writeFunction)(uint8_t *cmdInfo);    // pointer to write function
  uint32_t accessRights;  // access rights to this command
} command_struct;

typedef struct {
  uint16_t address;
  uint16_t cmdMain;
  uint16_t cmdSpecific;
  uint16_t cmdCommand;
  uint8_t data_size;      // contains data type size of the target command
  uint8_t data_array[MAX_BUFFER_ARRAY]; // temporary place holder of data to process
  command_struct *cmdPtr; // pointer to command structure
} command_info;

typedef struct {
  uint16_t cmdMain;
  command_struct *command;
} command_list_strcture;

typedef struct {
  uint8_t enabled;    // enable/disable streaming
  uint16_t spdivider; // signal processing divider
  uint16_t overrun;   // count how many divider
  uint16_t count;     // coun thow many streaming samples left
  float *signals[MAX_STREAMABLE_SIGNALS]; // pointer to max 5 signals to stream
} stream_structure;

typedef struct {
  comms_struct *comms_driver;
  protocol_states current_state;
  command_info cmdInfo;
  stream_structure stream;
} api_instance;

extern const command_list_strcture cmdList[];

extern api_instance uart_api;
extern api_instance usbd_api;

extern void protocol_sm (api_instance *api);

extern void init_UART_API (uint8_t source);
extern void init_USBD_API (void);

// Bare Metal implementation
extern void Init_commsBM(void);
extern void protocolBM(void);

#endif
