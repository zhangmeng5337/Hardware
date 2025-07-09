/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMSAPI_H
#define __COMMSAPI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
#define MAX_BUFFER_ARRAY    256
#ifdef BOOTLOADER
#define TIMEOUT_RELOAD_UART 100
#define TIMEOUT_RELOAD_USB  2
#else
#define TIMEOUT_RELOAD_USB  2
#endif

//#define SCAN_BAUD_FEATURE
#define RX_ERROR_MAX  3

#define UART_SHUTDOWN_DELAY 50 // software loop wait counts

#define MIN_BAUD_CONFIG 1
#define MAX_BAUD_CONFIG 5

typedef enum {
  SOURCE_USB = 0,
  SOURCE_UART,
  SOURCE_UART_EXTPROC,
  SOURCE_UART_CDG
} source_enum;

/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint32_t baud;
  uint8_t ctout;
} baudrate_struct;

typedef struct {
  uint8_t pending;
  uint16_t ctr;
  uint16_t length;
  uint8_t buffer[MAX_BUFFER_ARRAY];
} comms_tx_struct;

typedef struct {
  uint16_t uart_timeout_ctr;
  uint8_t pending;
  uint16_t length;
  uint16_t ctr;
  uint8_t data;
  uint8_t buffer[MAX_BUFFER_ARRAY];
} comms_rx_struct;

typedef struct {
  source_enum source;
  comms_tx_struct tx;
  comms_rx_struct rx;
} comms_struct;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
extern uint8_t push_CDG_data;
extern comms_struct uart_com_driver;
extern comms_struct usbd_com_driver;
extern uint8_t uart_shutdown_delay;
/* Exported functions ------------------------------------------------------- */
extern uint8_t wrapper_uart_timeout(void);
extern uint8_t wrapper_usbd_timeout(void);
extern uint8_t panalyzer(comms_struct *comms);
extern uint8_t pbaudrate(uint32_t code, uint32_t *baudrate, uint8_t sbaudrate);
extern uint8_t pconfig(uint32_t l, uint32_t p, uint32_t s, uint32_t *length, uint32_t *parity, uint32_t *stopbits);
extern void comms_send_bytes(comms_struct *comms, uint8_t *data, uint16_t length);
extern void HAL_USBD_RxCpltCallback(uint8_t* Buf, uint32_t *Len);
extern void startautobaud(UART_HandleTypeDef *huart);
extern void setbuadrate(UART_HandleTypeDef *huart, uint32_t baudrate);

#endif /* __UART_H */
