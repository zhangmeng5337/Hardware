#include "typedefs.h"
#include <stdio.h>
#include <string.h>
#include "gpio_defs.h"
#include "radio.h"

#define RF_PACKET_SIZE 6               /* Define the payload size here */

static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

static BOOL g_bEnableMaster = TRUE;     /* Master/Slave selection */

static u16 g_nRecvCount = 0;
static u16 g_nSendCount = 0;
static u16 g_nErrCount  = 0;



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
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);//发送数据
        break;
    }
    
    case RF_TX_DONE:
    {

        
        RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, 1000);
        break;
    }
    
    case RF_RX_DONE:
    {
        
        g_nRecvCount++;


        break;
    }
    
    case RF_RX_TIMEOUT:
    {
        
        break;
    }
    
    case RF_ERROR:
    {

        
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
        
        g_nRecvCount++;

        CMT2300A_DelayMs(10);

        g_nSendCount++;
        g_txBuffer[0] = (u8)g_nSendCount;
        g_txBuffer[1] = g_nSendCount >> 8;
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);
        
        break;
    }
    
    case RF_TX_DONE:
    {

        
        break;
    }
    
    case RF_ERROR:
    {

        
        break;
    }
    
    default:
        break;
    }
}

/* Main application entry point */
int main(void)
{
    int i;

    for(i=0; i<RF_PACKET_SIZE; i++)
        g_txBuffer[i] = i+1;
    
    RF_Init();
    
    if(FALSE==CMT2300A_IsExist()) {
        while(1);
    }
    
    while(1)
    {
        if(TRUE==g_bEnableMaster) {
            OnMaster();//主动发射，发射完成后进入接受模式
        }
        else {
            OnSlave();//主动接收，接收完成后进入发射模式
        }
    }
}

