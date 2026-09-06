#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "ads1256.h"

#define MAX_SEQUENCE  20

uint8_t prevchannel = 0;
uint8_t ads1256_regs[21] = {0};
ads1256seqtype adcdata1256[MAX_1256_CHANNEL];
uint8_t seq = 0;
const uint8_t chseq[] = {0,4,5,1, 0,4,5,2, 0,4,5,3, 0,4,5,6, 0,4,5,7};

uint8_t seqComplete = 0;

/**
*/
static void ads1256sendcmd(uint8_t cmd_reg) {
  uint8_t tx = cmd_reg;
  ADS1256_CS_LOW();  
  HAL_SPI_Transmit(&hspi1, &tx, 1, 1000);
  ADS1256_CS_HIGH();  
}

/**
  @brief Read regs with CS
*/
static void ads1256rreg(uint8_t address, uint8_t *dout, uint8_t len) {
  uint8_t tx[2] = {(CMD_RREG|address), len-1};
  uint8_t rx[3] = {0};  
  ADS1256_CS_LOW();  
  HAL_SPI_Transmit(&hspi1, tx, 2, 1000);
  HAL_SPI_Receive(&hspi1, rx, len, 1000);  
  ADS1256_CS_HIGH();
  for (int i=0; i< len; i++) {
    dout[i]=rx[i];
  }   
}

/**
  @brief Write regs with CS
*/
static void ads1256wreg(uint8_t address, uint8_t wdata) {
  uint8_t tx[3] = {CMD_WREG|address, 0, wdata};  
  ADS1256_CS_LOW();  
  HAL_SPI_Transmit(&hspi1, tx, 3, 1000);   
  ADS1256_CS_HIGH();
}

/**
  @brief Read ADC data
*/
static void ads1256rdata(uint32_t *dout) {
  uint8_t tx = {CMD_RDATA};
  uint8_t rx[3] = {0};  
  ADS1256_CS_LOW();  
  HAL_SPI_Transmit(&hspi1, &tx, 1, 1000);
  HAL_SPI_Receive(&hspi1, rx, 3, 1000);  
  ADS1256_CS_HIGH();
  
  *dout=0;
  for (int i=0; i<3; i++) {
    *dout<<=8;
    *dout|=rx[i];
  }   
    
}

/**
*/
static void readallregs(void) {
  
  for (uint8_t i=REGS_ADS1256_STATUS; i<(REGS_ADS1256_CMDSTAT+1); i++) {
    ads1256rreg(i, &ads1256_regs[i], 1);
  }
  
}

/**
*/
void initADS1256struct(void (*fx)(void), uint32_t *pa0, uint32_t *pa1, uint32_t *dp0, uint32_t *dp1, uint32_t *itemp, uint32_t *mtemp, uint32_t *cpa0, uint32_t *cpa1) {
  for (int i=0;i<MAX_1256_CHANNEL;i++) { 
    adcdata1256[i].eventhandler = NULL;    
  }
  adcdata1256[0].res = pa0;
  adcdata1256[1].res = pa1;
  adcdata1256[2].res = dp0;
  adcdata1256[3].res = dp1;
  adcdata1256[4].res = itemp;
  adcdata1256[5].res = mtemp;  
  adcdata1256[6].res = cpa0;
  adcdata1256[7].res = cpa1;
  adcdata1256[0].eventhandler = fx;
}

/**
*/
void initADS1256(void) {
  // Start MCO clock for ADx
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  
  ads1256sendcmd(CMD_RESET);
  ads1256sendcmd(CMD_SDATAC);
  ads1256wreg(REGS_ADS1256_DRATE, DR_30KSPS);
  ads1256sendcmd(CMD_SELFCAL);
  readallregs(); // Read initial values  
  seq=0;
  prevchannel = chseq[seq];
#ifdef ADS_RDYx_ENABLED     
  __HAL_GPIO_EXTI_CLEAR_IT(SENSOR_ADC_RDYB_Pin);
  HAL_NVIC_EnableIRQ(SENSOR_ADC_RDYB_EXTI_IRQn); // start waiting for data ready bar    
  ads1256adcEngine();  
#endif  
}

/**
*/
uint32_t getAds1256ADC(uint8_t idx) {
  return *adcdata1256[idx].res;
}

/**
*/
void startADS1256(void) {
#ifdef ADS_RDYx_ENABLED   
  setAds1256Channel(chseq[seq], ADS1256_MUXN_AINCOM);  // start conversion
  __HAL_GPIO_EXTI_CLEAR_IT(SENSOR_ADC_RDYB_Pin);
  HAL_NVIC_EnableIRQ(SENSOR_ADC_RDYB_EXTI_IRQn); // start waiting for data ready bar  
#endif  
}

/**
*/
void setAds1256Channel(uint8_t AIN_P, uint8_t AIN_N) {
  uint8_t MUX_CHANNEL;
  uint8_t MUXP;
  uint8_t MUXN;
  
  switch (AIN_P) {
    case 0:
      MUXP = ADS1256_MUXP_AIN0;
      break;
    case 1:
      MUXP = ADS1256_MUXP_AIN1;
      break;
    case 2:
      MUXP = ADS1256_MUXP_AIN2;
      break;
    case 3:
      MUXP = ADS1256_MUXP_AIN3;
      break;
    case 4:
      MUXP = ADS1256_MUXP_AIN4;
      break;
    case 5:
      MUXP = ADS1256_MUXP_AIN5;
      break;
    case 6:
      MUXP = ADS1256_MUXP_AIN6;
      break;
    case 7:
      MUXP = ADS1256_MUXP_AIN7;
      break;
    default:
      MUXP = ADS1256_MUXP_AINCOM;
  }

  switch (AIN_N) {
    case 0:
      MUXN = ADS1256_MUXN_AIN0;
      break;
    case 1:
      MUXN = ADS1256_MUXN_AIN1;
      break;
    case 2:
      MUXN = ADS1256_MUXN_AIN2;
      break;
    case 3:
      MUXN = ADS1256_MUXN_AIN3;
      break;
    case 4:
      MUXN = ADS1256_MUXN_AIN4;
      break;
    case 5:
      MUXN = ADS1256_MUXN_AIN5;
      break;
    case 6:
      MUXN = ADS1256_MUXN_AIN6;
      break;
    case 7:
      MUXN = ADS1256_MUXN_AIN7;
      break;
    default:
      MUXN = ADS1256_MUXN_AINCOM;
  }

  MUX_CHANNEL = MUXP | MUXN;
 
  ads1256wreg(REGS_ADS1256_MUX, MUX_CHANNEL);
  ads1256sendcmd(CMD_SYNC);
  ads1256sendcmd(CMD_WAKEUP);
}

/**
*/
void ads1256adcEngine(void) {
  setAds1256Channel(chseq[seq], ADS1256_MUXN_AINCOM);  // start conversion
  ads1256rdata(adcdata1256[prevchannel].res);   
  prevchannel = chseq[seq];
  seq = (seq + 1) % MAX_SEQUENCE;  

  if ((adcdata1256[prevchannel].eventhandler != NULL) && (seqComplete)) {
    (*adcdata1256[prevchannel].eventhandler)(); // call installed event handler
  } else {
    if (seq == 0) { // if we rolledover that means we've gone through the entire sequence list
      seqComplete = 1;
    }  
  }
}
