/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
 *
 */
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/

#include "..\inc\main.h"

#define I2C_SLAVEADDRESS          0x3C
#define I2C_SLAVEADDRESS_OTHER1   0xAA
#define I2C_SLAVEADDRESS_OTHER2   0x96

#define  I2C1_SCL_GPIO_PORT       CW_GPIOA
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_6    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C1_SDA_GPIO_PORT       CW_GPIOA
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_5   //如果改动口线则GPIO初始化代码需要做同步修改

//UARTx
#define  DEBUG_UARTx                   CW_UART2
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART2
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           16000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOB)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_5
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_6

//GPIO AF
#define  DEBUG_UART_AFTX               PB05_AFx_UART2TXD()
#define  DEBUG_UART_AFRX               PB06_AFx_UART2RXD()

uint8_t u8Senddata[16] = {0x55};
uint8_t u8Recdata[16]= {0x00};
uint8_t u8SendLen;
uint8_t u8RecvCnt = 0, u8SendCnt = 0, u8RecvOverFlower = 0;


void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void I2C_Configuration(void);
void I2C1_IRQHandlerCallback(void);
void DebugUART_Configuration(uint32_t UclkFreq);

int32_t main(void)
{
    uint8_t u8ReadPtr = 0;
	 //时钟初始化
    SYSCTRL_Configuration();
	
	//IO口初始化
    GPIO_Configuration();
    
    DebugUART_Configuration(16000000);
   
    I2C_Configuration();
    
    NVIC_Configuration();
    
	u8SendLen = 15;
    while (1)
    {
        if (u8RecvOverFlower)
        {
            printf("%x", u8Recdata[u8ReadPtr]);
            u8ReadPtr++;            
        }
        else
        {
            if (u8ReadPtr < u8RecvCnt)    // 收到数据
            {
                printf("%x", u8Recdata[u8ReadPtr]);
                u8ReadPtr++;
            }		
        }
        
        if (u8ReadPtr == sizeof(u8Recdata)/sizeof(u8Recdata[0]))
        {
            u8RecvOverFlower = 0;
            u8ReadPtr = 0;
        }
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
	SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV3);
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_I2C_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_UART2_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	PA05_AFx_I2C1SDA();
	PA06_AFx_I2C1SCL();
    GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void I2C_Configuration(void)
{
	I2C_InitTypeDef I2C_InitStruct;
	
	//I2C初始化
	I2C_InitStruct.PCLK_Freq = 16000000;
    I2C_InitStruct.I2C_SCL_Freq = 1000000;
    I2C_InitStruct.I2C_OwnSlaveAddr0 = I2C_SLAVEADDRESS;         //TEST ADDRESS0
    I2C_InitStruct.I2C_OwnSlaveAddr1 = I2C_SLAVEADDRESS_OTHER1;  //TEST ADDRESS1
    I2C_InitStruct.I2C_OwnSlaveAddr2 = I2C_SLAVEADDRESS_OTHER2;  //TEST ADDRESS2

    I2C_InitStruct.I2C_OwnGc = DISABLE;
	
	I2C1_DeInit();
    I2C_Slave_Init(CW_I2C1,&I2C_InitStruct);//初始化模块
	CW_I2C1->CR_f.FLT = 1;
    I2C_Cmd(CW_I2C1, ENABLE);//模块使能
    I2C_AcknowledgeConfig(CW_I2C1,ENABLE);//ACK打开
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(I2C1_IRQn);
    __enable_irq();
}

/**
 * @brief 配置UART
 *
 */
void DebugUART_Configuration(uint32_t UclkFreq)
{
    UART_InitTypeDef UART_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;    

    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
}


void I2C1_IRQHandlerCallback(void)
{
    static uint8_t u8Cnt = 0;
    uint8_t u8State;
    u8State = CW_I2C1->STAT;
    switch (u8State)
    {
        // 从机收到写入的请求
        case 0x60:   //接收到SLA+W，ACK已回应
        case 0x68:   //主机丢失仲裁后被SLA+W寻址，ACK已回应
        case 0x70:   //接收到SLA（广播地址）+W，ACK已回应
        case 0x78:   //主机丢失仲裁后被SLA（广播地址）+W寻址，ACK已回应
            break;
        case 0x80:   //从机已接收到1字节数据，ACK已回复
        case 0x90:   //从机已接收到1字节广播数据，ACK已回复
            u8Recdata[u8RecvCnt++] = CW_I2C1->DR;
            if (u8RecvCnt == sizeof(u8Recdata)/sizeof(u8Recdata[0]))
            {
                u8RecvCnt = 0;
                u8RecvOverFlower = 1;
            }            
            break;                
        case 0x88:   //从机已接收到1字节数据，NACK已回应
        case 0x98:   //从机已接收到1字节广播数据，NACK已回应
            u8Recdata[u8RecvCnt] = CW_I2C1->DR;
            CW_I2C1->CR_f.STO = 1;
            CW_I2C1->CR_f.SI = 0;
            return;
        case 0xA0:  //重复START信号或者STOP信号已收到
            break;
            
        // 从机收到读出的请求
        case 0xA8:   //接收到SLA+R,ACK已回应
        case 0xB0:   //主机丢失仲裁后被转从机并被SLA+R寻址，ACK已回应
            u8Cnt = 0;
            CW_I2C1->DR = u8Senddata[u8Cnt++];
            break;            
        case 0xB8:   //从机已发送1字节数据，ACK已收到
            CW_I2C1->DR = u8Senddata[u8Cnt++];            
            if (u8Cnt == u8SendLen)
            {
                // 发送最后一个字节后 AA设置为0
                CW_I2C1->CR_f.AA  = 0;
            }                
            break;
        case 0xC8:    //从机最后一个数据字节已被发送(发送前自身AA被设置为0)，并已接收到ACK
            CW_I2C1->CR_f.AA  = 1;    // 响应下一次的SLA+R
            break;
        case 0xC0:    //已发送数据字节，已接收 NACK
        default:
            break;
    }
    CW_I2C1->CR_f.SI = 0;
}

 
 #ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __/io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
 * @brief Retargets the C library printf function to the UART.
 *
 */
PUTCHAR_PROTOTYPE
{
    UART_SendData_8bit(DEBUG_UARTx, (uint8_t)ch);

    while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);

    return ch;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
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
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

