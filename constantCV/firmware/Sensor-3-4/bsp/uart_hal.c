#include "uart_hal.h"
#include "cmd_queue.h"
#include "cw32l010_uart.h"
#include "key.h"
#include "reg.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

uart_stru  uart_usr;
/**
 * @brief Retargets the C library printf function to the UART.
 *
 */
PUTCHAR_PROTOTYPE
{
    GPIO_WritePin(EN_GPIO_PORT, EN_GPIO_PIN, GPIO_Pin_SET);
    UART_SendData_8bit(DEBUG_UARTx, (uint8_t)ch);

    while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);

    GPIO_WritePin(EN_GPIO_PORT, EN_GPIO_PIN, GPIO_Pin_RESET);

    return ch;
}
void uart_tx(uint8_t *dat, unsigned char len)
{
    unsigned int i;
    GPIO_WritePin(EN_GPIO_PORT, EN_GPIO_PIN, GPIO_Pin_SET);
	SysTickDelay(1);

    for (i = 0; i < len; i ++)
    {
        UART_SendData_8bit(DEBUG_UARTx, dat[i]);
        while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);
	//while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TC) != RESET);

    }

	

    SysTickDelay(10);
    GPIO_WritePin(EN_GPIO_PORT, EN_GPIO_PIN, GPIO_Pin_RESET);
		// SysTickDelay(25);
}
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }


    for (/* Empty */; size != 0; --size)
    {
        UART_SendData_8bit(DEBUG_UARTx, *buffer++);
        while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);
        ++nChars;
    }
    return nChars;
}


void uart_config()
{

    static uint32_t delay_time;
    UART_InitTypeDef UART_InitStructure = {0};
	static unsigned char flag = 0;
    if (uart_usr.reconfig)
    {
        //flag = 1;
		delay_time =GetTick();
        if (getKey()->indat[0].baud <= 7)
        {
            if (uart_usr.reconfig == 1)
            {
                GetReg()->pb[eREG_RATE].val_u32ToFloat = getKey()->indat[0].baud;

                uart_usr.baudrate = getKey()->indat[0].baud;

            }
            else  if (uart_usr.reconfig == 2 || uart_usr.reconfig == 3)
            {
                //GetReg()->pb[eREG_RATE].val_u32ToFloat = GetReg()->pb[eREG_CHECK];

                uart_usr.baudrate = GetReg()->pb[eREG_RATE].val_u32ToFloat;

            }

        }
        if (getKey()->indat[0].par <= 2)
        {

            if (uart_usr.reconfig == 1)
            {
                GetReg()->pb[eREG_CHECK].val_u32ToFloat = getKey()->indat[0].par;
                uart_usr.parity =  getKey()->indat[0].par;


            }
            else  if (uart_usr.reconfig == 2)
            {

                uart_usr.parity =  GetReg()->pb[eREG_CHECK].val_u32ToFloat;

            }

        }

        if (uart_usr.reconfig == 2 || uart_usr.reconfig == 1 ||
                uart_usr.reconfig == 3)
        {
            if (uart_usr.reconfig == 3)
            {
                uart_usr.reconfig =  0;
                delay_time = GetTick();
			    flag = 1;

            }

            else
            {    flag = 0;
                UART_ITConfig(DEBUG_UARTx, UART_IT_RC | UART_IT_RXIDLE, DISABLE);
                switch (uart_usr.parity)
                {
                    case parityNone:
                        UART_InitStructure.UART_Parity = UART_Parity_No ;
                        break;
                    case parityEven:
                        UART_InitStructure.UART_Parity = UART_Parity_Odd  ;
                        break;
                    case parityOdd:
                        UART_InitStructure.UART_Parity = UART_Parity_Even ;
                        break;
                    default:
                        UART_InitStructure.UART_Parity = UART_Parity_No ;
                        break;
                }

                switch (uart_usr.baudrate)
                {
                    case 0:
                        UART_InitStructure.UART_BaudRate = baudrate_1200;
                        break;
                    case 1:
                        UART_InitStructure.UART_BaudRate = baudrate_2400;
                        break;
                    case 2:
                        UART_InitStructure.UART_BaudRate = baudrate_4800;
                        break;
                    case 3:
                        UART_InitStructure.UART_BaudRate = baudrate_9600;
                        break;
                    case 4:
                        UART_InitStructure.UART_BaudRate = baudrate_19200;
                        break;
                    case 5:
                        UART_InitStructure.UART_BaudRate = baudrate_38400;
                        break;
                    case 6:
                        UART_InitStructure.UART_BaudRate = baudrate_57600;
                        break;
                    case 7:
                        UART_InitStructure.UART_BaudRate = baudrate_115200;
                        break;
                    default:
                        UART_InitStructure.UART_BaudRate = baudrate_9600;
                        break;
                }

                // UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
                UART_InitStructure.UART_Over = UART_Over_16;
                UART_InitStructure.UART_Source = UART_Source_PCLK;
                UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
                UART_InitStructure.UART_StartBit = UART_StartBit_FE;
                UART_InitStructure.UART_StopBits = UART_StopBits_1;
                //UART_InitStructure.UART_Parity = UART_Parity_No ;
                UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
                UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
                UART_Init(DEBUG_UARTx, &UART_InitStructure);
                uart_usr.reconfig = 0;
                UART_ITConfig(DEBUG_UARTx, UART_IT_RC | UART_IT_RXIDLE, ENABLE);
                UART_ClearITPendingBit(DEBUG_UARTx, UART_IT_RC);
                UART_ClearITPendingBit(DEBUG_UARTx, UART_IT_RXIDLE);
                uart_usr.reconfig =  0;

            }


        }





    }
    if ((GetTick() - delay_time) >= 10000&& flag == 1)
    {
        flag = 0;
        UART_ITConfig(DEBUG_UARTx, UART_IT_RC | UART_IT_RXIDLE, DISABLE);
        switch (uart_usr.parity)
        {
            case parityNone:
                UART_InitStructure.UART_Parity = UART_Parity_No ;
                break;
            case parityEven:
                UART_InitStructure.UART_Parity = UART_Parity_Odd  ;
                break;
            case parityOdd:
                UART_InitStructure.UART_Parity = UART_Parity_Even ;
                break;
            default:
                UART_InitStructure.UART_Parity = UART_Parity_No ;
                break;
        }

        switch (uart_usr.baudrate)
        {
            case 0:
                UART_InitStructure.UART_BaudRate = baudrate_1200;
                break;
            case 1:
                UART_InitStructure.UART_BaudRate = baudrate_2400;
                break;
            case 2:
                UART_InitStructure.UART_BaudRate = baudrate_4800;
                break;
            case 3:
                UART_InitStructure.UART_BaudRate = baudrate_9600;
                break;
            case 4:
                UART_InitStructure.UART_BaudRate = baudrate_19200;
                break;
            case 5:
                UART_InitStructure.UART_BaudRate = baudrate_38400;
                break;
            case 6:
                UART_InitStructure.UART_BaudRate = baudrate_57600;
                break;
            case 7:
                UART_InitStructure.UART_BaudRate = baudrate_115200;
                break;
            default:
                UART_InitStructure.UART_BaudRate = baudrate_9600;
                break;
        }

        // UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
        UART_InitStructure.UART_Over = UART_Over_16;
        UART_InitStructure.UART_Source = UART_Source_PCLK;
        UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
        UART_InitStructure.UART_StartBit = UART_StartBit_FE;
        UART_InitStructure.UART_StopBits = UART_StopBits_1;
        //UART_InitStructure.UART_Parity = UART_Parity_No ;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
        UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
        UART_Init(DEBUG_UARTx, &UART_InitStructure);
        uart_usr.reconfig = 0;
        UART_ITConfig(DEBUG_UARTx, UART_IT_RC | UART_IT_RXIDLE, ENABLE);
        UART_ClearITPendingBit(DEBUG_UARTx, UART_IT_RC);
        UART_ClearITPendingBit(DEBUG_UARTx, UART_IT_RXIDLE);
        uart_usr.reconfig =  0;

    }
}




void uart_init(void)
{
    uart_usr.index = 0;
    uart_usr.reconfig = 0;
    uart_usr.parity = GetReg()->pb[eREG_CHECK].val_u32ToFloat;
    uart_usr.baudrate = GetReg()->pb[eREG_RATE].val_u32ToFloat;
    uart_usr.reconfig = 2;
    uart_config();
    uart_usr.recv_update = 0;
    uart_usr.index = 0;


}
uart_stru  *getuart(void)
{
    return &uart_usr;
}

void uart_recv_proc(unsigned char recv_dat, unsigned char irq_flag)
{
    uart_usr.rx_buf[uart_usr.index++]  = recv_dat;
    if (uart_usr.index >= RX_BUF_SIZE)
    {
        uart_usr.index = 0;
        //memset(uart_usr.rx_buf, 0, RX_BUF_SIZE);

    }

    if (irq_flag == 1)
        uart_usr.recv_update = 1;
}
void uart_proc(void)
{
    uart_config();

}
