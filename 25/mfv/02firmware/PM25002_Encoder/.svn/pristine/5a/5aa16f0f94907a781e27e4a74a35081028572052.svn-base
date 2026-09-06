#ifndef __ADS1256_H_
#define __ADS1256_H_

#include <stdint.h>
#include "main.h"

#define ADS1256_CS_LOW()   HAL_GPIO_WritePin(SPI1_SENSOR_ADC_CS_GPIO_Port, SPI1_SENSOR_ADC_CS_Pin, GPIO_PIN_RESET)
#define ADS1256_CS_HIGH()   HAL_GPIO_WritePin(SPI1_SENSOR_ADC_CS_GPIO_Port, SPI1_SENSOR_ADC_CS_Pin, GPIO_PIN_SET)

#define REGS_ADS1256_STATUS   0
#define REGS_ADS1256_MUX      1
#define REGS_ADS1256_ADCON    2
#define REGS_ADS1256_DRATE    3
#define REGS_ADS1256_IO       4
#define REGS_ADS1256_OFC0     5
#define REGS_ADS1256_OFC1     6
#define REGS_ADS1256_OFC2     7
#define REGS_ADS1256_FSC0     8
#define REGS_ADS1256_FSC1     9
#define REGS_ADS1256_FSC2     10
#define REGS_ADS1256_CFG0     11
#define REGS_ADS1256_CFG1     12
#define REGS_ADS1256_CFG2     13
#define REGS_ADS1256_CFG3     14
#define REGS_ADS1256_CFG4     15
#define REGS_ADS1256_CFG5     16
#define REGS_ADS1256_CFG6     17
#define REGS_ADS1256_MODSTAT  18
#define REGS_ADS1256_DEVSTAT  19
#define REGS_ADS1256_CMDSTAT  20

#define CMD_WAKEUP            0x00
#define CMD_RDATA             0x01
#define CMD_RDATAC            0x03
#define CMD_SDATAC            0x0F
#define CMD_RREG              0x10
#define CMD_WREG              0x50
#define CMD_SELFCAL           0xF0
#define CMD_SELFOCAL          0xF1
#define CMD_SELFGCAL          0xF2
#define CMD_SYSOCAL           0xF3
#define CMD_SYSGCAL           0xF4
#define CMD_SYNC              0xFC
#define CMD_STANDBY           0xFD
#define CMD_RESET             0xFE

#define MAX_1256_CHANNEL       8

// define multiplexer codes
#define ADS1256_MUXP_AIN0 0x00
#define ADS1256_MUXP_AIN1 0x10
#define ADS1256_MUXP_AIN2 0x20
#define ADS1256_MUXP_AIN3 0x30
#define ADS1256_MUXP_AIN4 0x40
#define ADS1256_MUXP_AIN5 0x50
#define ADS1256_MUXP_AIN6 0x60
#define ADS1256_MUXP_AIN7 0x70
#define ADS1256_MUXP_AINCOM 0x80

#define ADS1256_MUXN_AIN0 0x00
#define ADS1256_MUXN_AIN1 0x01
#define ADS1256_MUXN_AIN2 0x02
#define ADS1256_MUXN_AIN3 0x03
#define ADS1256_MUXN_AIN4 0x04
#define ADS1256_MUXN_AIN5 0x05
#define ADS1256_MUXN_AIN6 0x06
#define ADS1256_MUXN_AIN7 0x07
#define ADS1256_MUXN_AINCOM 0x08

#define DR_30KSPS   0xF0
#define DR_15KSPS   0xE0
#define DR_7K5SPS   0xD0
#define DR_3K75SPS  0xC0
#define DR_2KSPS    0xB0
#define DR_1KSPS    0xA1
#define DR_500SPS   0x92
#define DR_100SPS   0x82
#define DR_60SPS    0x72
#define DR_50SPS    0x63
#define DR_30SPS    0x53
#define DR_25SPS    0x43
#define DR_15SPS    0x33
#define DR_10SPS    0x23
#define DR_5SPS     0x13
#define DR_2i5SPS   0x03

typedef struct ads1256sequence {  
  uint32_t *res;
  void (*eventhandler)(void);
} ads1256seqtype;

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;

extern void startADS1256(void);
extern void initADS1256struct(void (*fx)(void), uint32_t *pa0, uint32_t *pa1, uint32_t *dp0, uint32_t *dp1, uint32_t *itemp, uint32_t *mtemp, uint32_t *cpa0, uint32_t *cpa1);
extern void initADS1256(void);
extern void ads1256adcEngine(void);
extern uint32_t getAds1256ADC(uint8_t idx);

#endif
