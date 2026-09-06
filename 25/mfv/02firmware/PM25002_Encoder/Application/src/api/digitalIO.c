#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "digitalIO.h"
#include "utilities.h"
#include "awe.h"

digitalIO_struct pb[MAX_DIGITAL_IO];

/**
  @brief
*/
uint8_t zeroButton(void) {
  static bool risingEdge = false;

  pb[0].pin = HAL_GPIO_ReadPin(GPIO_ZERO_CS4_GPIO_Port, GPIO_ZERO_CS4_Pin);
  
  if (risingEdge) {
    if (pb[0].pin == GPIO_PIN_SET) {
      if (pb[0].dbctr >= DEBOUNCE_RELOAD) {
        pb[0].state = PB_OFF;
        pb[0].dbctr = 0;
        risingEdge = false;
      } else {
        pb[0].dbctr++;
      }
    } else {
      pb[0].dbctr = 0;
    }    
  } else {  
    if (pb[0].pin == GPIO_PIN_RESET) {
      if (pb[0].dbctr >= DEBOUNCE_RELOAD) {
        pb[0].state = PB_ON;
        pb[0].dbctr = 0;
        risingEdge = true;
        return 1;        
      } else {
        pb[0].dbctr++;
      }
    } else {
      pb[0].state = PB_OFF;
      pb[0].dbctr = 0;
    }
  }
  
  return 0;
}

/**
  @brief Check if we got an interrupt from IO expander
*/
uint8_t checkAddressInt(void) {
  static uint8_t debounce = 0;
  
  if (HAL_GPIO_ReadPin(GPIO_EXTENDER_INT_GPIO_Port, GPIO_EXTENDER_INT_Pin) == GPIO_PIN_RESET) {
    debounce++;
    if (debounce > 10) {
      return 1;
    }
  } else {
    debounce = 0;
  }
  return 0;
}  

/**
  @brief Get ID from I2C IO expander.
*/
uint16_t getSwitch(void) {
	uint16_t address = 0;
	uint8_t aTxBuffer[TXBUFFERSIZE] = {0};
	uint8_t aRxBuffer[RXBUFFERSIZE] = {0,0};

	  /*##- Start the transmission process #####################################*/
  /* While the I2C in reception process, user can transmit data through
     "aTxBuffer" buffer */
  /* Timeout is set to 10S */
  while (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, aTxBuffer, TXBUFFERSIZE, 10000) != HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge its address)
       Master restarts communication */
    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }
	
	  /*##- Put I2C peripheral in reception process ###########################*/
  /* Timeout is set to 10S  */
  if (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)I2C_ADDRESS, aRxBuffer, RXBUFFERSIZE, 10000) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	
	aRxBuffer[1] ^= 0x0f;
	aRxBuffer[0] ^= 0xff;

	address =  ((aRxBuffer[0] & 0xf0) << 4) | ((aRxBuffer[1] << 4) & 0xf0) | (aRxBuffer[0] & 0x0f);
	
	return address;
}
