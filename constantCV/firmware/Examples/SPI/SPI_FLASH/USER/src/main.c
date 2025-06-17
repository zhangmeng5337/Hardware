/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2022
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
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//UARTx
#define  DEBUG_UARTx                   CW_UART2
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART2
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           24000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           SYSCTRL_AHB_PERIPH_GPIOB
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_5
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_6

//GPIO AF
#define  DEBUG_UART_AFTX               PB05_AFx_UART2TXD()
#define  DEBUG_UART_AFRX               PB06_AFx_UART2RXD()

#define  BufferSize                     (ARRAY_SZ(TxBuffer) - 1)

#define  FLASH_WriteAddress             0x00000
#define  FLASH_ReadAddress              FLASH_WriteAddress
#define  FLASH_SectorToEraseAddress     FLASH_WriteAddress
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer[] = "CW32L010 SPI FLASH Example";
uint8_t RxBuffer[BufferSize];

uint8_t DeviceID = 0;
uint16_t ManufactDeviceID = 0;
uint32_t JedecID = 0;
uint8_t UniqueID[8];

volatile TestStatus TransferStatus = FAILED;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置GPIO
    GPIO_Configuration();

    //配置UART
    UART_Configuration();

    //配置SPI
    SPI_FLASH_Init();

    printf("\r\nCW32L010 SPI FLASH Example\r\n");

    DeviceID = SPI_FLASH_DeviceID();
    ManufactDeviceID = SPI_FLASH_ManufactDeviceID();
    JedecID = SPI_FLASH_JedecID();
    SPI_FLASH_UniqueID(UniqueID);

    printf("\r\nDeviceID = 0x%X\r\nManufactDeviceID = 0x%X\r\nJedecID = 0x%X", DeviceID, ManufactDeviceID, JedecID);
    printf("\r\nUniqueID = 0x ");
    for(uint8_t i = 0; i<8; i++)
    {
        printf("%X ", UniqueID[i]);
    }

    /* Check JEDEC ID */
    if(JedecID == sJedecID)
    {
        printf("\r\n\nFLASH Detected\r\n");;

        //擦除扇区 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //写数据
        SPI_FLASH_BufferWrite(TxBuffer, FLASH_WriteAddress, BufferSize);
        printf("\r\n写入的数据为：%s\r\n", TxBuffer);

        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);
        printf("\r\n读出的数据为：%s\r\n", RxBuffer);

        //检查写入的数据与读出的数据是否一致
        TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

        //擦除扇区 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);

        if(TransferStatus == PASSED)
        {
            PB02_SETHIGH();
            printf("\r\nFLASH Success\r\n");
        }
        else
        {            
            printf("\r\nFLASH Error 1\r\n");
        }

        for(uint8_t j = 0; j < BufferSize; j++)
        {
            if(RxBuffer[j] != 0xFF)
            {               
                printf("\r\nFLASH Error 2\r\n");
            }
        }
    }
    else// (FlashID ！= sFLASH_ID)
    {        
        printf("\r\nFLASH Error 3\r\n");
    }

    while(1)
    {

    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 24MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2);
    

    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // uart
    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
    //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    
    //LED灭
    PB02_SETLOW();    
}

/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
}

/**
 * @brief Compares two buffers
 *
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus
 *     @arg PASSED: pBuffer1 identical to pBuffer2
 *     @arg FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

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
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

