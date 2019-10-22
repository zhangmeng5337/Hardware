#include "bsp_uart.h"
#include "bsp_spi.h"
#include "key.h"
#include "main.h"
#include "eeprom.h"
#include <CMT2300drive.h>
#include "uartParse.h"
#include "bsp_i2c_ee.h"
#include "command.h"
#include "led.h"
#include "radio_TxRx.h"
/**/						//false: for contain packet length in Tx message, the same mean with variable lenth
//true : for doesn't include packet length in Tx message, the same mean with fixed length
/**/	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//void Device_Init(void);

//void loop_Tx(void);
//void loop_Rx(void);

#define LEN 21

unsigned char str[LEN] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','3','0','0','A'};
//byte getstr[LEN+1];



/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
* @brief  Main program.
* @param  None
* @retval None
*/


		
unsigned char recv[16];
void main(void)
{

  LED_Init();

    //delay1ms(10000);
  //KEY_Init();

  //CFG->GCR |= CFG_GCR_SWD;
 
 // ee_Test();

    
      while(1)
      {

        radio_process();
	  //LED_blink(LED_RED,1000);
       ; // command_process();

      }

    

}


/**
* @brief Device Initialize configuration
* @param None
* @retval None
*/


