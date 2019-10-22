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



#include <stdio.h>
#include <string.h>
#include "gpio_defs.h"
#include "radio.h"
#define RF_PACKET_SIZE 32               /* Define the payload size here */

static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

static BOOL g_bEnableMaster = TRUE;     /* Master/Slave selection */

static u16 g_nRecvCount = 0;
static u16 g_nSendCount = 0;
static u16 g_nErrCount  = 0;

void Mcu_Init(void)
{

	
	  CMT2300A_SetGpio1In();
    CMT2300A_SetGpio2In();
    CMT2300A_SetGpio3In();

    /* system init */
  //  SystemInit();
  //  GPIO_Config();
  //  NVIC_Config();
    //SystemTimerDelay_Config();
    //Timer5_Config();
    
    /* services init */
//    time_server_init();
//    IntSrv_Init();
//    Key_Init();
//    
//    /* periph init */
//    led_init();
//    lcd12864_init();
//    buzzer_init();
//    
//    lcd12864_led_on();
}

/* Manages the master operation */
void OnMaster(void)
{
    char str[32];
    
    switch(RF_Process())
    {
    case RF_IDLE:
    {
        g_nSendCount++;
        g_txBuffer[0] = (u8)g_nSendCount;
        g_txBuffer[1] = g_nSendCount >> 8;
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);
        break;
    }
    
    case RF_TX_DONE:
    {
			   LED_blink(LED_RED,50);
        sprintf(str, "send: %d", g_nSendCount);
       // views_print_line(1, str);
        
        RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, 1000);
        break;
    }
    
    case RF_RX_DONE:
    {
         LED_blink(LED_GREEN,50);
        
        g_nRecvCount++;
        sprintf(str, "recv: %d", g_nRecvCount);
       // views_print_line(2, str);

        break;
    }
    
    case RF_RX_TIMEOUT:
    {
        sprintf(str, "recv: timeout");
        //views_print_line(2, str);
        
        break;
    }
    
    case RF_ERROR:
    {
        LED_blink(LED_RED,1000);
        
        sprintf(str, "error: %d", ++g_nErrCount);
        //views_print_line(3, str);
        
        break;
    }
    
    default:
        break;
    }
}

/* Manages the slave operation */
void OnSlave(void)
{
    char str[32];
    
    switch(RF_Process())
    {
    case RF_IDLE:
    {
        RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, INFINITE);
        break;
    }
    
    case RF_RX_DONE:
    {
                LED_blink(LED_RED,50);
        
        g_nRecvCount++;
        sprintf(str, "recv: %d", g_nRecvCount);
        //views_print_line(1, str);

        CMT2300A_DelayMs(10);

        g_nSendCount++;
        g_txBuffer[0] = (u8)g_nSendCount;
        g_txBuffer[1] = g_nSendCount >> 8;
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);
        
        break;
    }
    
    case RF_TX_DONE:
    {
        LED_blink(LED_RED,50);
        
        sprintf(str, "send: %d", g_nSendCount);
        //views_print_line(2, str);
        
        break;
    }
    
    case RF_ERROR:
    {
        LED_blink(LED_RED,1000);
        
        sprintf(str, "error: %d", ++g_nErrCount);
       // views_print_line(3, str);
        
        break;
    }
    
    default:
        break;
    }
}

/* Main application entry point */
int radio_process(void)
{
    int i;

    for(i=0; i<RF_PACKET_SIZE; i++)
        g_txBuffer[i] = i+1;
    
    Mcu_Init();
    RF_Init();
    
    if(FALSE==CMT2300A_IsExist()) {
        //views_print_line(0, "CMT2300A not found!");
        LED_blink(LED_RED,1000);
        while(1);
    }
    else {
        ;//views_print_line(0, "CMT2300A ready");
    }
    
    while(1)
    {
        if(TRUE==g_bEnableMaster) {
            OnMaster();
        }
        else {
            OnSlave();
        }
    }
}


