/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANALOGIO_H
#define __ANALOGIO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
#define MAX_ANALOG_IO   4
#define VREF    3.0f
#define MAX_DAC_COUNTS_16BIT  65535
#define MAX_DAC_COUNTS_14BIT  16383
#define ONE_16BIT_FLOAT 0.0000152587890625f
#define AO_SATURATION 1.19f
#define AO_DRIVE_SATURATION 1.0f

#define AO_CH0  0
#define AO_CH1  1
#define AO_CH2  2
#define AO_CH3  3

/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint16_t dacRaw;
  float dacNormalized;
  uint8_t freeze;
  float *signal;
  float saturation;
  uint8_t (*cback)(uint16_t data, uint32_t address);
} analogIO_struct;
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
extern analogIO_struct dac[MAX_ANALOG_IO];
/* Exported functions ------------------------------------------------------- */
extern void analogIOinstall(float *signal, uint8_t aochannel, float saturation, uint8_t dactype);
extern void analogOutput(float normalized, uint8_t idx, float saturation_pt);
extern float analogInput(uint8_t idx);
#endif /* __ANALOGIO_H */
