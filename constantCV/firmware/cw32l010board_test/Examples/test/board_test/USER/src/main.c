
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
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
#include "../inc/main.h"
#include "main.h"
#include "stdint.h"
#include "base_types.h"
#include "cw32l010.h"
#include "system_cw32l010.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"
#include "cw32l010_uart.h"
#include "cw32l010_spi.h"
#include "cw32l010_adc.h"
#include "cw32l010_i2c.h"
#include "cw32l010_flash.h"
#include "cw32l010_systick.h"
#include "cw32_eval_spi_flash.h"
#include "cw32l010_flash.h"
#include "cw32l010_gtim.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
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
void NVIC_Configuration(void);
void I2C_Test_Init(void);
void SPI_FLASH_Test(void);
void I2C_Test(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
volatile uint8_t gKey1Status,gKey2Status;
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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//UARTx
#define  DEBUG_UARTx                   CW_UART2
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART2
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOB)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_5
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_6

//GPIO AF
#define  DEBUG_UART_AFTX               PB05_AFx_UART2TXD()
#define  DEBUG_UART_AFRX               PB06_AFx_UART2RXD()


//KEY
	#define  KEY1_GPIO_PIN                   GPIO_PIN_4
	#define  KEY1_GPIO_PORT                  CW_GPIOB
	#define  KEY2_GPIO_PIN                   GPIO_PIN_6
	#define  KEY2_GPIO_PORT                  CW_GPIOA
	
//LED
	#define  LED1_GPIO_PIN                   GPIO_PIN_3	
	#define  LED1_GPIO_PORT                  CW_GPIOB
	#define  LED2_GPIO_PIN                   GPIO_PIN_2	
	#define  LED2_GPIO_PORT                  CW_GPIOB

	#define  LED1_ON                         PB03_SETHIGH()
	#define  LED1_OFF                        PB03_SETLOW()
	#define  LED1_TOG                        PB03_TOG()
	#define  LED2_ON                         PB02_SETHIGH()
	#define  LED2_OFF                        PB02_SETLOW()
	#define  LED2_TOG                        PB02_TOG()
	
#define  I2C1_SCL_GPIO_PORT       CW_GPIOA
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_6    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C1_SDA_GPIO_PORT       CW_GPIOA
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_5    //如果改动口线则GPIO初始化代码需要做同步修改


//EEPROM内部地址
#define WRITEADDRESS   0      //写入或者读取的EEPROM的地址
#define WriteReadCycle  22    //写读次数,每次写入数据为n+i(n为次数，i=0~7)

uint8_t u8Senddata[8] = {0x66, 0x02, 0x03, 0x04, 0x05, 0x60, 0x70, 0x20};
uint8_t u8Recdata[8] = {0x00};
uint8_t u8SendLen = 0;
uint8_t u8RecvLen = 0;
uint8_t SendFlg = 0, Comm_flg = 0;
uint8_t u8recvflg = 0;
uint8_t u8State = 0;

#define  BufferSize                     (ARRAY_SZ(TxBuffer) - 1)

#define  FLASH_WriteAddress             0x00000
#define  FLASH_ReadAddress              FLASH_WriteAddress
#define  FLASH_SectorToEraseAddress     FLASH_WriteAddress
	
uint8_t TxBuffer[] = "CW32L010 SPI FLASH Example";
uint8_t RxBuffer[BufferSize];

uint8_t DeviceID = 0;
uint16_t ManufactDeviceID = 0;
uint32_t JedecID = 0;
uint8_t UniqueID[8];
//uint16_t valueAdc;
//volatile uint8_t gFlagIrq;
volatile TestStatus TransferStatus = FAILED;
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief
 *
 * @return int32_t
 */
int32_t main(void)
{
  RCC_Configuration(); 
  SYSCTRL_SystemCoreClockUpdate(8000000);
  InitTick(8000000);	

		//配置GPIO
  GPIO_Configuration();

  //配置UART
  UART_Configuration();

   printf("\r\n Connect  Pins:set the jumps \r\n");
	
	 /* NVIC Configuration */
   NVIC_Configuration();
			
//   SYSCTRL_LSI_Enable();    

		 printf("\r\n if connect all pins please press key1  \r\n");

    while (1)
    {
				
				gKey1Status = 0;
		//wait key is press
		do
		{
			/* Toggle LED */
			LED2_TOG;
			SysTickDelay(500);//0.5s

		}while (gKey1Status == 0);	

	 	SysTickDelay(1000);
			I2C_Test_Init();
		 I2C_Test();
			SysTickDelay(100);
		  //配置和测试SPI
			SPI_FLASH_Init(); 
	 	SPI_FLASH_Test();
    }
}



/**
 * @brief 配置RCC
 * 
 */
void RCC_Configuration(void)
{

  //SYSCLK = HSI =8MHz = HCLK = PCLK
  SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
		CW_SYSCTRL->HSI_f.TRIM=0x0757;    //校准时钟，依mcu而定
	 SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
  __SYSCTRL_FLASH_CLK_ENABLE();
  SYSCTRL_HSE_Enable(SYSCTRL_HSE_MODE_OSC,16000000,SYSCTRL_HSE_DRIVER_LEVEL5,RCC_HSE_FLT_CLOSE);
		SYSCTRL_LSE_Enable(SYSCTRL_LSE_MODE_OSC,SYSCTRL_LSE_DRIVER_LEVEL2);
	 SYSCTRL_SysClk_Switch(SYSCTRL_SYSCLKSRC_HSI);
   SYSCTRL_SystemCoreClockUpdate(8000000);
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_GPIOA_CLK_ENABLE();
  /* 使能所有外设时钟*/
  CW_SYSCTRL->AHBEN  = 0xFFFFFFFF;
  CW_SYSCTRL->APBEN1 = 0xFFFFFFFF;
  CW_SYSCTRL->APBEN2 = 0xFFFFFFFF;	
}


/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{  
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStruct);

     //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;
	
	   //LED
	   GPIO_InitStruct.Pins = LED1_GPIO_PIN	| LED2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
	
			 //KEY2
    GPIO_InitStruct.Pins = KEY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
	   GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

	   //KEY2
    GPIO_InitStruct.Pins = KEY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
	   GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
				
    //LED灭
				LED1_OFF;
    LED2_OFF;

	}

	
///**
// * @brief 配置UART
// * 
// */
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
	UART_ITConfig(DEBUG_UARTx, UART_IT_RC, ENABLE);
}

void SPI_FLASH_Test(void)
{
	  printf("\r\nCW32L010 SPI FLASH Test\r\n");

    DeviceID = SPI_FLASH_DeviceID();
    ManufactDeviceID = SPI_FLASH_ManufactDeviceID();
    JedecID = SPI_FLASH_JedecID();
    SPI_FLASH_UniqueID(UniqueID);
				printf("DeviceID = 0x%X\r\n", DeviceID);
    printf("MDeviceID =0x%X\r\n", ManufactDeviceID);
				printf("JID = 0x%X\r\n",JedecID);
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


        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);


        //检查写入的数据与读出的数据是否一致
        TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

        //擦除扇区 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);

        if(TransferStatus == PASSED)
        {
            PB03_SETHIGH();
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

}

void I2C_Test_Init(void)
{
				 GPIO_InitTypeDef GPIO_InitStruct = {0};
					GPIO_InitStruct.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStruct);
				PA05_AFx_I2C1SDA();
				PA06_AFx_I2C1SCL();
	   I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_BaudEn = ENABLE;
    I2C_InitStruct.I2C_Baud = 0x01;//500K=(8000000/(8*(1+1))
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA = DISABLE;

    I2C1_DeInit();
    I2C_Master_Init(CW_I2C1, &I2C_InitStruct); //初始化模块
    I2C_Cmd(CW_I2C1, ENABLE); //模块使能
}

void I2C_Test(void)
{
	printf("\r\nI2C_EEPORM Test Start\r\n");

	       I2C_MasterWriteEepromData(CW_I2C1, WRITEADDRESS, u8Senddata, 8);
        FirmwareDelay(3000);
        I2C_MasterReadEepomData(CW_I2C1, WRITEADDRESS, u8Recdata, 8);
				   	TransferStatus = Buffercmp(u8Senddata, u8Recdata,  (ARRAY_SZ(u8Senddata) - 1));
       	if(TransferStatus == PASSED)
	{ 
		printf("I2C_EEPROM Test Success\r\n");
	}
	else
	{        
		printf("I2C_EEPROM Test Error:Read and Write Data is not same\r\n");
		while(1);
	}	
	
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq(); 
	GPIOA_INTFLAG_CLR( 0xFFFF ); 	//clear GPIOA ALL INT FLAG
	
 NVIC_EnableIRQ(GPIOB_IRQn);   
	NVIC_EnableIRQ(GPIOA_IRQn);
  __enable_irq();  
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif
