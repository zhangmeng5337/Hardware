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
#include "interrupts_cw32l010.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//#define  SPI_MASTER //主机模式
#define  SPI_SLAVE  //从机模式

#define  BufferSize                    ARRAY_SZ(TxBuffer1)
#define  DUMMY_DATA                    0x55
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
void SPI_Configuration(void);
void NVIC_Configuration(void);
void SPI1_IRQHandlerCallback(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                       0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                       0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                       0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23};
uint8_t RxBuffer1[BufferSize];
uint8_t TxCounter = 0;
uint8_t RxCounter = 0;																							
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

    //配置SPI
    SPI_Configuration();
	
    //配置NVIC
    NVIC_Configuration();

#ifdef SPI_MASTER
    // 拉低CS，选中从机
    SPI_NSSInternalSoftwareConfig(CW_SPI, 0);
    
    // 配置为发送
    CW_SPI->CR3_f.HDOE = 1;
    while(TxCounter < BufferSize)
    {
        if (SPI_GetFlagStatus(CW_SPI,SPI_IT_TXE) == SET)
        {
            SPI_SendData(CW_SPI, TxBuffer1[TxCounter++]);
        }
    }
    //发送完成
    while(SPI_GetFlagStatus(CW_SPI,SPI_FLAG_BUSY) == SET);
    
    FirmwareDelay(1000);   // 延时1ms
    
    // 准备接收
    CW_SPI->ICR_f.FLUSH = 0;
    CW_SPI->CR3_f.HDOE = 0;
      
    while(RxCounter < BufferSize)
    {
        while(SPI_GetFlagStatus(CW_SPI,SPI_IT_TXE) == RESET);
        SPI_SendData(CW_SPI, DUMMY_DATA);    // 发送伪数据
        
        while(SPI_GetFlagStatus(CW_SPI,SPI_IT_RXNE) == RESET);
        RxBuffer1[RxCounter++] = SPI_ReceiveData(CW_SPI);        
    }
    while(SPI_GetFlagStatus(CW_SPI,SPI_FLAG_BUSY) == SET);
    
    // 拉高CS，释放从机
    SPI_NSSInternalSoftwareConfig(CW_SPI, 1);  
#endif

 
#ifdef SPI_SLAVE
    // 从机对SPI滤波
    CW_SPI->CR1_f.FLTEN = 1;
    
    // 选中从机
    SPI_NSSInternalSoftwareConfig(CW_SPI, 0);
    
    // 配置为接收模式
    CW_SPI->CR3_f.HDOE = 0;
    
    // 等待接收
    while(RxCounter < BufferSize)
    {
        while(SPI_GetFlagStatus(CW_SPI,SPI_IT_RXNE) == RESET);
        RxBuffer1[RxCounter++] = SPI_ReceiveData(CW_SPI);        
    }
    
    // 接收完成，开始发送    
    CW_SPI->ICR_f.FLUSH = 0;   
    CW_SPI->CR3_f.HDOE = 1;
       
    while(TxCounter <= BufferSize)
    {
        if (SPI_GetFlagStatus(CW_SPI,SPI_IT_TXE) == SET)
        {
            SPI_SendData(CW_SPI, TxBuffer1[TxCounter++]);
        }
    }
    //发送完成
    while(SPI_GetFlagStatus(CW_SPI,SPI_FLAG_BUSY) == SET);
    
    // 配置为接收模式
    CW_SPI->CR3_f.HDOE = 0;
#endif
   
    TransferStatus = Buffercmp(TxBuffer1, RxBuffer1,BufferSize);
	 		
    while(1)
    {
        if(TransferStatus == PASSED) //PASSED
        {
            //LED2闪速
            PB02_TOG();
            FirmwareDelay(100000);
        }
        else //FAILED
        {
            //LED2灭
            PB02_SETLOW(); 
        }
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    // 配置系统时钟为8MHz    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
    
    // 开SPI和SPI端口的配置时钟    
	SYSCTRL_AHBPeriphClk_Enable(SYSCTRL_AHB_PERIPH_GPIOA, ENABLE);
    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_SPI, ENABLE);   
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    // 配置SPI的口线

#ifdef SPI_MASTER
    
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pins = GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2;
    
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    PA05_AFx_SPI1SCK();
    PA04_AFx_SPI1MOSI();
    PA02_AFx_SPI1NCS();
#endif
    
#ifdef SPI_SLAVE
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pins = GPIO_PIN_5 | GPIO_PIN_3;
    
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    PA05_AFx_SPI1SCK();
    PA03_AFx_SPI1MISO();
#endif
    
    // 配置LED
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pins = GPIO_PIN_2;
    
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    
    PB02_SETHIGH();  
}

/**
 * @brief 配置UART
 *
 */
void SPI_Configuration(void)
{
   SPI_InitTypeDef SPI_InitStructure = {0};

#ifdef SPI_MASTER   
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
#endif

#ifdef SPI_SLAVE   
   SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                          // 从机模式   
#endif

   SPI_InitStructure.SPI_Direction = SPI_Direction_1Lines_HalfDuplex;    // 单工半双工 
   
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           // 时钟空闲电平为高
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // 第一个边沿采样
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;    // 波特率为PCLK的16分频
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
   SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // 低速SPI
   
   SPI_Init(CW_SPI,&SPI_InitStructure);
   SPI_Cmd(CW_SPI,ENABLE); 
		
}

void SPI1_IRQHandlerCallback(void)
{
}

/**
 * @brief 配置NVIC
 * 
 */
void NVIC_Configuration(void)
{
    
}	
	
/**
 * @brief Compares two buffers
 *
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus
 * @arg PASSED: pBuffer1 identical to pBuffer2
 * @arg FAILED: pBuffer1 differs from pBuffer2
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

