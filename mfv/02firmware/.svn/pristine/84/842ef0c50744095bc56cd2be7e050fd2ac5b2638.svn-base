/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIGITALIO_H
#define __DIGITALIO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "main.h"

/* Exported constants --------------------------------------------------------*/
#define MAX_DIGITAL_IO  2
#define DEBOUNCE_RELOAD 2


#define I2C_ADDRESS 0x40
#define TXBUFFERSIZE  1
#define RXBUFFERSIZE  2

#define SYS_DIS   0x00
#define SYS_EN    0x01
#define LCD_OFF   0x02
#define LCD_ON    0x03
#define TIMER_DIS 0x04
#define WDT_DIS   0x05
#define TIMER_EN  0x06
#define WDT_EN    0x07
#define TONE_OFF  0x08
#define TONE_ON   0x09
	
	// Set bias and commons: 1/2 or 1/3 bias, 2,3 or 4 commons
#define BIAS_HALF_2_COM  0x20
#define BIAS_HALF_3_COM  0x24
#define BIAS_HALF_4_COM  0x28
#define BIAS_THIRD_2_COM 0x21
#define BIAS_THIRD_3_COM 0x25
#define BIAS_THIRD_4_COM 0x29
	
	// Buzzer tones
#define TONE_4K 0x80
#define TONE_2K 0xC0

	// For factory testing
#define TEST_ON   0xE0
#define TEST_OFF  0xE3

#define HT1621_CMD  0x04
#define HT1621_DATA 0x05
#define HT1621_ADDR0  0x00

#define A_  3
#define B_  2
#define C_  1
#define D_  4
#define E_  5
#define F_  7
#define G_  6
#define DP_ 0

#define  FEATURE_ALPHABET

#define MAX_DISPLAY_INDEX 4

typedef enum {
  ODS_NORMAL = 0,
  ODS_TRIGGER
} odrive_state;

typedef enum {
	OD_NORMAL = 0,
	OD_HARDWARE,
	OD_FREQUENCY,
	OD_FAULT
} protection_state;

enum pbstate {
  PB_OFF  = 0,
  PB_ON
};

typedef enum {
  HT_INIT = 0,
  HT_RUN,
  HT_FAULT  
} HT1621_state;

typedef struct {
  uint8_t type;
  uint8_t ico;
  void *data;
} display_struct;

/* Exported types ------------------------------------------------------------*/
typedef struct {
  GPIO_PinState pin;
  uint16_t dbctr;
  uint8_t state;
} digitalIO_struct;

/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
extern digitalIO_struct pb[MAX_DIGITAL_IO];
/* Exported functions ------------------------------------------------------- */
extern uint8_t zeroButton(void);
extern uint8_t dispButton(void);
extern uint8_t vdOverride(void);
extern uint16_t getSwitch(void);
extern uint8_t checkAddressInt();
extern void HT1621sm();
extern void HT1621engine();
extern uint8_t calculate_segments(uint8_t character, uint8_t dp);
extern void installDisplay(void *fp, uint8_t type, uint8_t idx, uint8_t ico);
extern void cycleDisplay();
extern void updateDisplay();
#endif /* __ANALOGIO_H */
