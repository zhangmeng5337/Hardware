#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "usbd_cdc_if.h"
#include "commsAPI.h"
#include "crc16.h"
#include "cmd_queue.h"
#include "semtaskcomms.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;

uint8_t push_CDG_data;
comms_struct uart_com_driver;
comms_struct usbd_com_driver;

uint8_t requestchange = 0;
uint8_t uart_shutdown_delay = UART_SHUTDOWN_DELAY;

uint16_t errcounter[5] = {0};
uint16_t chartout = 15;  // assuming 115K

/**
    @brief uart timeout wrapper
*/
uint8_t wrapper_uart_timeout(void) {

//	uart_com_driver.rx.uart_timeout_ctr--;
//	if (uart_com_driver.rx.uart_timeout_ctr == 0) {
//		uart_com_driver.rx.pending = 1; // set comms process command flag
//		HAL_TIM_Base_Stop_IT(&htim3);
//		return 1;
//	}  
	return 0;
}

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
  @brief check for ORE caused by incorrect baudrate
*/
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

//  if (huart->Instance == USART1) {
//    __nop();
//    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_ORE);
//  }
//  
//  if (huart->Instance == USART2) {
//    __nop();
//  }
//    
//}

/**
    @brief uart receive callback
*/
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//  
//  if (huart->Instance == USART1) {  // CDG
//    queue_push(push_CDG_data);
//    HAL_UART_Receive_IT(huart, &push_CDG_data, 1);
//  }
//  
//	if (huart->Instance == USART2) {  // RS485 TOOL Side
//    if (uart_com_driver.rx.ctr < MAX_BUFFER_ARRAY) {
//      uart_com_driver.rx.buffer[uart_com_driver.rx.ctr] = uart_com_driver.rx.data;
//      uart_com_driver.rx.ctr++;
//      uart_com_driver.rx.uart_timeout_ctr = chartout;
//      __HAL_TIM_SET_COUNTER(&htim3 ,0);
//      HAL_TIM_Base_Start_IT(&htim3);
//      HAL_UART_Receive_IT(huart, &uart_com_driver.rx.data, 1);      
//    } else {
//      uart_com_driver.rx.uart_timeout_ctr = 1;  // process now
//    }
//	}
//}

/**
	@brief uart transmit callback
*/
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) { 
//	if (huart->Instance == USART2) {
//		if (++uart_com_driver.tx.ctr == uart_com_driver.tx.length) {
//      uart_com_driver.tx.pending = 2;  // ready to shutdown transmitter
//		} else {
//			HAL_UART_Transmit_IT(huart, &uart_com_driver.tx.buffer[uart_com_driver.tx.ctr], 1);
//		}  
//	}
//}

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
    case SOURCE_UART:
    case SOURCE_UART_EXTPROC:  
      comms->tx.pending = 1;
      comms->tx.length = length;
      comms->tx.ctr = 0;
      HAL_UART_Transmit_IT(&TOOLINTERFACE, comms->tx.buffer, 1);      
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
    // check for MODBUS frame checksum
    if (crc16_modbus(comms->rx.buffer, comms->rx.ctr-2) == ((comms->rx.buffer[comms->rx.ctr-1] << 8) | comms->rx.buffer[comms->rx.ctr-2])) {
      ret_val = 1;  // confirmed MODBUS frame
    } else if (crc16_ccitt(0, comms->rx.buffer, comms->rx.ctr - 2) == ((comms->rx.buffer[comms->rx.ctr-2] << 8) | comms->rx.buffer[comms->rx.ctr-1])) {
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
uint8_t pbaudrate(uint32_t code, uint32_t *baudrate, uint8_t sbaudrate) {
  uint8_t ret = 0;
  switch (code) {
    case 0:
      *baudrate = ((sbaudrate < MIN_BAUD_CONFIG) || (sbaudrate > MAX_BAUD_CONFIG)) ? baudtable[2].baud : baudtable[sbaudrate].baud;
      chartout = ((sbaudrate < MIN_BAUD_CONFIG) || (sbaudrate > MAX_BAUD_CONFIG)) ? baudtable[2].ctout : baudtable[sbaudrate].ctout;
      ret = 3;  // auto-baud (scanner mode)
      break;
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

/**
  @brief pconfig
*/
uint8_t pconfig(uint32_t l, uint32_t p, uint32_t s, uint32_t *length, uint32_t *parity, uint32_t *stopbits) {
  uint8_t ret = 0;
  // setup length
  switch(l) {
    case 0:
    case 7:
      *length = UART_WORDLENGTH_7B;
      break;      
    case 2:
    case 9:
      *length = UART_WORDLENGTH_9B;
      break;      
    case 8:    
    default:  // 1
      *length = UART_WORDLENGTH_8B; 
      break;
  }     
  // setup parity 
  switch(p) {
    case 2:
      *parity = UART_PARITY_EVEN;
      break; 
    case 1:
      *parity = UART_PARITY_ODD;
      break;       
    default:  // 0
      *parity = UART_PARITY_NONE;
      break;
  }
  // setup stop bits
  switch(s) {
    case 5:
      *stopbits = UART_STOPBITS_0_5;
      break;
    case 15:
      *stopbits = UART_STOPBITS_1_5;
      break;
    case 2:
      *stopbits = UART_STOPBITS_2;
      break;
    case 1:
    default:  // 1
      *stopbits = UART_STOPBITS_1;
      break;        
  }
  return ret;
}

/**
  @brief start auto-baud sequence
*/
void startautobaud(UART_HandleTypeDef *huart) {
  uint16_t timeout = 0xFFFF;
  /*##-2- Configure the AutoBaudRate method */
  huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_AUTOBAUDRATE_INIT;
  huart->AdvancedInit.AutoBaudRateEnable = UART_ADVFEATURE_AUTOBAUDRATE_ENABLE;
  huart->AdvancedInit.AutoBaudRateMode = UART_ADVFEATURE_AUTOBAUDRATE_ON0X55FRAME;
  //__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  // enable IDLE interrupt
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    Error_Handler();
  }
  timeout = 0xFFFF;
  /* Wait until Receive enable acknowledge flag is set */
  while ((__HAL_UART_GET_FLAG(huart,UART_FLAG_REACK) == RESET) && (timeout))
  { HAL_Delay(1); timeout--; }
  timeout = 0xFFFF;
  /* Wait until Transmit enable acknowledge flag is set */
  while ((__HAL_UART_GET_FLAG(huart,UART_FLAG_TEACK) == RESET) && (timeout))
  { HAL_Delay(1); timeout--; }
}

/**
  @brief setbuadrate
*/
void setbuadrate(UART_HandleTypeDef *huart, uint32_t baudrate) {
  
  huart->Init.BaudRate = baudrate;
  //__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  // enable IDLE interrupt
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    Error_Handler();
  }
}
