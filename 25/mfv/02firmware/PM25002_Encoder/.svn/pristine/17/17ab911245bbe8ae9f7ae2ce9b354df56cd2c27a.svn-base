#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "usbd_cdc_if.h"
#include "commsAPI.h"
#include "crc16.h"
#include "semtaskcomms.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;

comms_struct usbd_com_driver;

uint8_t requestchange = 0;
uint8_t uart_shutdown_delay = UART_SHUTDOWN_DELAY;

uint16_t errcounter[5] = {0};
uint16_t chartout = 15;  // assuming 115K

/**
    @brief usbd timeout wrapper
*/
uint8_t wrapper_usbd_timeout(void) {

	usbd_com_driver.rx.uart_timeout_ctr--;
	if (usbd_com_driver.rx.uart_timeout_ctr == 0) {
		usbd_com_driver.rx.pending = 1; // set comms process command flag
		HAL_TIM_Base_Stop_IT(&htim7);
		return 1;
	}  
	return 0;
}

/**
  @brief usbd receive callback
*/
void HAL_USBD_RxCpltCallback(uint8_t* Buf, uint32_t *Len) {
  if ((usbd_com_driver.rx.ctr + *Len) < MAX_BUFFER_ARRAY) {
    memcpy(&usbd_com_driver.rx.buffer[usbd_com_driver.rx.ctr], Buf, *Len);
    usbd_com_driver.rx.ctr += *Len;
    usbd_com_driver.rx.uart_timeout_ctr = TIMEOUT_RELOAD_USB;
    __HAL_TIM_SET_COUNTER(&htim7 ,0);
    HAL_TIM_Base_Start_IT(&htim7);
  }
}

/**
@brief this function starts the transmit process by triggering the first byte to transmit

@params pointer transmit data, length of the data to be transmitted

@return None
*/
void comms_send_bytes(comms_struct *comms, uint8_t *data, uint16_t length) {

  memcpy(comms->tx.buffer, data, length);
  switch (comms->source) {
    case SOURCE_USB:
      while (CDC_Transmit_FS(comms->tx.buffer, length) == USBD_BUSY);
      break;
    default:
      break;
  }

}

/**
  @brief Analyze protocol frame format.
*/
uint8_t panalyzer(comms_struct *comms) {
  uint8_t ret_val = 0;
  
  if (comms->rx.ctr < 4) {  // something strange here. if buadrate is at 115200 and host tries to talk at 9600, we get byte counter set to 3
    ret_val = 3;
  } else {
    if (crc16_ccitt(0, comms->rx.buffer, comms->rx.ctr - 2) == ((comms->rx.buffer[comms->rx.ctr-2] << 8) | comms->rx.buffer[comms->rx.ctr-1])) {
      ret_val = 0;  // confirmed OPM frame
    } else {  
      ret_val = 2;  // could not confirm frame. Could either be a bad frame or the baudrate was not set correctly
    }
  }
  
  return ret_val;
}

// baudrate table
const baudrate_struct baudtable[] = {
  {0,       0},
  {9600,    80},  // ~4.16ms
  {19200,   40},  // ~2.08ms	used to be 40
  {38400,   15},  // ~1.75ms
  {57600,   15},  // ~1.75ms
  {115200,  15},  // ~1.6ms
  {460800,  35}   // ~1.75ms
};

/**
  @brief Baudrate parser
*/
uint8_t pbaudrate(uint32_t code, uint32_t *baudrate) {
  uint8_t ret = 0;
  switch (code) {
    case 1:
    case 9600:
      *baudrate = baudtable[1].baud;
      chartout = baudtable[1].ctout;
      break;
    case 2:
    case 19200:
      *baudrate = baudtable[2].baud;
      chartout = baudtable[2].ctout;
      break;
    case 3:
    case 38400:
      *baudrate = baudtable[3].baud;
      chartout = baudtable[3].ctout;
      break;
    case 4:
    case 57600:
      *baudrate = baudtable[4].baud;
      chartout = baudtable[4].ctout;
      break;   
    case 5:
    case 115200:
      *baudrate = baudtable[5].baud;
      chartout = baudtable[5].ctout;
      break;
    case 6:
    case 460800:
      *baudrate = baudtable[6].baud;
      chartout = baudtable[6].ctout;      
      break;
    default:
      *baudrate = baudtable[2].baud;
      chartout = baudtable[2].ctout;
      ret = 1;  // not supported, defaulting to 19200
      break;    
  }
  return ret;
}
