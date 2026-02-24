#include <string.h>
#include "protocol.h"
#include "crc16.h"
#include "nvmem.h"
#include "main.h"

api_instance uart_api;
api_instance usbd_api;

static void action_idle (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_rx_waiting (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_validatepacket (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_searchtable (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_buildread (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_buildwrite (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_ack (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);
static void action_nak (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo);

/**
  @brief initialize UART API
*/
void init_UART_API (uint8_t source) {
  
  memset(&uart_api, 0, sizeof(api_instance));
  switch (source) {
    case SOURCE_UART_EXTPROC:
      uart_com_driver.source = SOURCE_UART_EXTPROC;
      //NETX90_NRESET
      HAL_Delay(100);
      NETX90_RUN
      HAL_Delay(100);
      //NETX90_NORMAL
      HAL_Delay(100);
      break;
    default:
      uart_com_driver.source = SOURCE_UART;
      break;      
  }
  uart_com_driver.tx.pending = 0;
  uart_api.cmdInfo.address = nvmem.app.network.address;
  uart_api.comms_driver = &uart_com_driver;
}

/**
  @brief initialize USBD API
*/
void init_USBD_API (void) {
  
  memset(&usbd_api, 0, sizeof(api_instance));
  usbd_com_driver.source = SOURCE_USB;
  usbd_com_driver.tx.pending = 0;
  usbd_api.cmdInfo.address = 1; // USB defualts to 1 as there is only one unit per USB line
  usbd_api.comms_driver = &usbd_com_driver;
}

/**
    @brief This is the protcol state machine table
*/
static void (*const state_table[]) (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) = {
	action_idle, 
	action_rx_waiting,
	action_validatepacket, 
	action_searchtable,  
	action_buildread,
	action_buildwrite,
	action_ack,
	action_nak
};

/**
    @brief IDLE state. Device will just be waiting for a packet to come
*/
static void action_idle (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
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
        *current_state = RXWAIT;
        break;
      case SOURCE_UART:
        RS485_RXEN;
      case SOURCE_UART_EXTPROC:
        HAL_UART_Receive_IT(&TOOLINTERFACE, &comms->rx.data, 1);
        *current_state = RXWAIT;      
        break;
      default:
        *current_state = RXWAIT;
        break;
    }
  }
}

/**
    @brief RX_WAITING state. Device just got a character and waiting for the packet timeout to occur.
*/
static void action_rx_waiting (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
  // pre conditioning
  switch (comms->source) {
    case SOURCE_USB:
      break;
    case SOURCE_UART:
      RS485_RXEN;
    case SOURCE_UART_EXTPROC:
      HAL_UART_Receive_IT(&TOOLINTERFACE, &comms->rx.data, 1);       
      break;
    default:
      break;
  }
  // check if we have pending data
	if (comms->rx.pending) {
		*current_state = PARSEPACKET;
	}
    
  
}

/**
    @breif VALIDATEPACKET. This state verifies the integrity of the packet using CRC16 CCITT
*/
static void action_validatepacket (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    uint16_t crc16;
    uint16_t packect_crc16;
    uint16_t packet_address;
    
    if (comms->rx.ctr < 2) { // not enough data check
        comms->rx.ctr = 0;
        comms->rx.pending = 0;
        *current_state = IDLE;
        return;
    }
 
    packet_address = (comms->rx.buffer[0] << 8) |  comms->rx.buffer[1];
          
    // check if the packet is for the device
    if ((packet_address != cmdInfo->address) && (packet_address != 0xFFFF)) {
        comms->rx.ctr = 0;
        comms->rx.pending = 0;
        *current_state = IDLE;
        return;
    }

    if (comms->source == SOURCE_UART) {
      RS485_TXEN  // data is for me
    }
    
    packect_crc16 = (comms->rx.buffer[comms->rx.ctr-2] << 8) | comms->rx.buffer[comms->rx.ctr-1]; 
    crc16 = crc16_ccitt(0, comms->rx.buffer, comms->rx.ctr - 2);
    
    // check if the packet is valid
    if (crc16 != packect_crc16) {
        comms->rx.ctr = 0;
        comms->rx.pending = 0;
        *current_state = IDLE;
        return;
    } 
    
    *current_state = SEARCHTABLE;
    
}

/**
    @brief SEARCHTABLE. This state searches the command list table and the specific commands 
           table. If the command is found then we proceed in processing the packet data payload         
*/
static void action_searchtable (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    uint16_t i,j;
    
    cmdInfo->cmdMain = (comms->rx.buffer[2] << 8) |  comms->rx.buffer[3];
    cmdInfo->cmdSpecific = (comms->rx.buffer[4] << 8) |  comms->rx.buffer[5];
    cmdInfo->cmdCommand = comms->rx.buffer[6];
    
    // search main commands here
    for (i=0; i<0xFFFF;i++) {
        if ((cmdList[i].cmdMain == 0) || (i==0xFFFF)) {
            *current_state = NAKRESPONSE;
            return;
        } else if (cmdList[i].cmdMain == cmdInfo->cmdMain){
            break;
        }
    }
    
    // search specific commands here
    for (j=0; j<0xFFFF;j++) {
        if ((cmdList[i].command[j].cmdSpecific == 0) || (j==0xFFFF)) {
            *current_state = NAKRESPONSE;
            return;
        } else if (cmdList[i].command[j].cmdSpecific == cmdInfo->cmdSpecific){
            break;
        }
    }

    // At this point, we found the specific command. Move to a different state depending on
    // the operation.
    // point to command for processing
    cmdInfo->cmdPtr = &cmdList[i].command[j];
    memcpy(cmdInfo->data_array, &comms->rx.buffer[7], comms->rx.ctr-9);
    cmdInfo->data_size = cmdList[i].command[j].dataSize;
    
    // we found the element, move to the next state
    if (cmdInfo->cmdCommand == CMD_READ) {
        *current_state = BUILDREAD;
    } else if (cmdInfo->cmdCommand == CMD_WRITE) {
        *current_state = BUILDWRITE;
    } else {
        *current_state = NAKRESPONSE;        
    }
}

/**
    @brief BUILDREAD. This state processes the read command.
*/
static void action_buildread (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    binary temp_var;
    uint8_t (*fptr)();
    
    // run read function
    if (cmdInfo->cmdPtr->readFunction != 0x00) { 
        fptr = cmdInfo->cmdPtr->readFunction;
        if ((*fptr)(cmdInfo) == _SUCCESS) {
            comms->rx.ctr = 0;
            comms->rx.pending = 0;
            *current_state = IDLE;
            return;
        }
    }
    
    // read from RAM pointer if not NULL
    if (cmdInfo->cmdPtr->memptr != 0x00) {
        temp_var._uint64 = 0x0000000000000000;
        switch (cmdInfo->cmdPtr->dataType) {
            case _UINT8:
                memcpy(&temp_var._uint8, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                memcpy(cmdInfo->data_array, temp_var._uint8Arr, cmdInfo->data_size);
                break;
            case _UINT16:
                memcpy(&temp_var._uint16, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                memcpy(cmdInfo->data_array, temp_var._uint8Arr, cmdInfo->data_size);
                break;
            case _UINT32:
                memcpy(&temp_var._uint32, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                memcpy(cmdInfo->data_array, temp_var._uint8Arr, cmdInfo->data_size);
                break;
            case _UINT64:
                memcpy(&temp_var._uint64, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                memcpy(cmdInfo->data_array, temp_var._uint8Arr, cmdInfo->data_size);
                break;
            case _FLOAT32:         
                memcpy(&temp_var._float, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                memcpy(cmdInfo->data_array, temp_var._uint8Arr, cmdInfo->data_size);
                break;
            case _OCTETSTR:
                memcpy(cmdInfo->data_array, cmdInfo->cmdPtr->memptr, cmdInfo->data_size);
                break;
            default:
                break;
        }
    }
    
    *current_state = ACKRESPONSE;
        
}

/**
    @brief BUILDWRITE. This state processes the write command
*/
static void action_buildwrite (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    binary temp_var;
    uint8_t (*fptr)();
    uint8_t func_resp;
    
    // execute write command here. when done, respond with an ACK/NAK
    
    if (cmdInfo->cmdPtr->accessRights & _READ_ONLY) {
        // we shouldn't be here if the command is a read only
        *current_state = NAKRESPONSE;
        return;
    }
    
    // run write function if not NULL
    if (cmdInfo->cmdPtr->writeFunction != 0x00) { 
        fptr = cmdInfo->cmdPtr->writeFunction;
        func_resp = (*fptr)(cmdInfo);
        if ( func_resp== _FAIL) {
            *current_state = NAKRESPONSE;
            return;
        } else if (func_resp == _SUCCESS) {
            *current_state = ACKRESPONSE;
            return;
        } else if(func_resp == _NORESP) {
            comms->rx.ctr = 0;
            comms->rx.pending = 0;
            comms->tx.pending = 0;
            *current_state = IDLE;
            return;
        }
    }
    
    // write to RAM if not NULL
    if (cmdInfo->cmdPtr->memptr != 0x00) {
        switch (cmdInfo->cmdPtr->dataType) {
            case _UINT8:
                memcpy(temp_var._uint8Arr, cmdInfo->data_array , cmdInfo->data_size);
                memcpy(cmdInfo->cmdPtr->memptr, &temp_var._uint8 , cmdInfo->data_size);
                break;
            case _UINT16:
                memcpy(temp_var._uint8Arr, cmdInfo->data_array , cmdInfo->data_size);
                memcpy(cmdInfo->cmdPtr->memptr, &temp_var._uint16 , cmdInfo->data_size);
                break;
            case _UINT32:
                memcpy(temp_var._uint8Arr, cmdInfo->data_array , cmdInfo->data_size);
                memcpy(cmdInfo->cmdPtr->memptr, &temp_var._uint32 , cmdInfo->data_size);
                break;
            case _UINT64:
                memcpy(temp_var._uint8Arr, cmdInfo->data_array , cmdInfo->data_size);
                memcpy(cmdInfo->cmdPtr->memptr, &temp_var._uint64 , cmdInfo->data_size);
                break;
            case _FLOAT32:
                memcpy(temp_var._uint8Arr, cmdInfo->data_array , cmdInfo->data_size);
                memcpy(cmdInfo->cmdPtr->memptr, &temp_var._float , cmdInfo->data_size);
                break;
            case _OCTETSTR:
                memset(cmdInfo->cmdPtr->memptr, 0, cmdInfo->cmdPtr->dataSize);
                memcpy(cmdInfo->cmdPtr->memptr, cmdInfo->data_array, cmdInfo->data_size);
                break;
            default:
                break;
        }
    }
    
    // trigger a write if we are writing into NV memory
    if (cmdInfo->cmdPtr->accessRights & _FLASH) {
      write_to_flash(FLASH_TIMEOUT);  
    }
    
    *current_state = ACKRESPONSE;
}

/**
    @brief ACKRESPONSE. This state builds the ACK response to a packet. If there is a payload to read, the data is built
           with the packet during this state.
*/
static void action_ack (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    uint8_t ack_response[256];
    uint16_t packect_crc16;
    
    comms->rx.ctr = 0;
    comms->rx.pending = 0;
    *current_state = IDLE;
    ack_response[0] = ACK;
    ack_response[1] = cmdInfo->address >> 8;
    ack_response[2] = cmdInfo->address & 0xFF;
    ack_response[3] = cmdInfo->cmdMain >> 8;
    ack_response[4] = cmdInfo->cmdMain & 0xFF;
    ack_response[5] = cmdInfo->cmdSpecific >> 8;
    ack_response[6] = cmdInfo->cmdSpecific & 0xFF;
    memcpy(&ack_response[7], cmdInfo->data_array, cmdInfo->data_size);
    // calculate crc then transmit the packet with crc value
    packect_crc16 = crc16_ccitt(0, ack_response, cmdInfo->data_size+7);
    ack_response[cmdInfo->data_size+7] = packect_crc16 >> 8;
    ack_response[cmdInfo->data_size+8] = packect_crc16 & 0xFF;
    comms_send_bytes(comms, ack_response, cmdInfo->data_size+9);
}

/**
    @brief ACTIONNAK. This state builds the NAK response to a packet.
*/
static void action_nak (comms_struct *comms, protocol_states *current_state, command_info *cmdInfo) {
    uint8_t nak_response[9];
    uint16_t packect_crc16;
    
    comms->rx.ctr = 0;
    comms->rx.pending = 0;
    *current_state = IDLE;
    nak_response[0] = NAK;
    nak_response[1] = cmdInfo->address >> 8;
    nak_response[2] = cmdInfo->address & 0xFF;
    nak_response[3] = cmdInfo->cmdMain >> 8;
    nak_response[4] = cmdInfo->cmdMain & 0xFF;
    nak_response[5] = cmdInfo->cmdSpecific >> 8;
    nak_response[6] = cmdInfo->cmdSpecific & 0xFF;
    // calculate crc then transmit the packet with crc value
    packect_crc16 = crc16_ccitt(0, nak_response, 7);
    nak_response[7] = packect_crc16 >> 8;
    nak_response[8] = packect_crc16 & 0xFF;
    comms_send_bytes(comms, nak_response, 9);
    
}

/**
    @brief This is where the communicaitons protocol state machine is implemented.
*/
void protocol_sm (api_instance *api)
{   
    if (api->current_state >= STATEERROR) {
        /* invalid event/state - handle appropriately */
    } else {
        state_table [api->current_state ] (api->comms_driver, &api->current_state, &api->cmdInfo); /* call the action procedure */        
    }
}
