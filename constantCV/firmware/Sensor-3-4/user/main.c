
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
* ������ɺ�������Ϣ
* �人оԴ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人оԴ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人оԴ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/

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
#include "cw32l010_flash.h"
#include "cw32l010_systick.h"
#include "cw32_eval_spi_flash.h"
#include "cw32l010_flash.h"
#include "cw32l010_gtim.h"
#include "app.h"
#include "cs1237.h"
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

void SPI_FLASH_Test(void);
void PWM_OutputConfig(void);

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2,
                     uint16_t BufferLength);
volatile uint8_t gKey1Status, gKey2Status;


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

//EEPROM�ڲ���ַ
#define WRITEADDRESS   0      //д����߶�ȡ��EEPROM�ĵ�ַ
#define WriteReadCycle  22    //д������,ÿ��д������Ϊn+i(nΪ������i=0~7)

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
    uint32_t tmp32;
    RCC_Configuration();
    SYSCTRL_SystemCoreClockUpdate(MAIN_FREQ);//36mhz  8000000

    InitTick(MAIN_FREQ);
    SysTickDelay(2000);



  #ifdef DISPLAY_DEBUG
    GPIO_SWD2GPIO();

    REGBITS_CLR(CW_GPIOA->ANALOG, bv7 | bv8);       //����PA07/PA08Ϊ���ֹ���
    REGBITS_CLR(CW_GPIOA->DIR, bv7 | bv8); //����PA07Ϊ���룬PA08Ϊ���

#endif
    //����GPIO
    GPIO_Configuration();

    //����UART
    UART_Configuration();

    NVIC_Configuration();
    app_init();
    //printf("\r\n Connect  Pins:set the jumps \r\n");

    /* NVIC Configuration */


    while (1)
    {
        app_proc();

    }
}


/**
 * @brief ����RCC
 *
 */
void RCC_Configuration(void)
{
//��FLASHʱ��
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);  //����ϵͳʱ��ΪHSI 48M
    __SYSCTRL_FLASH_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GTIM1_CLK_ENABLE();
    // SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);


//    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
//    FLASH_SetLatency(FLASH_Latency_2);   //ϵͳʱ�Ӵ���24M��Ҫ����FlashWait
//    //�ر�FLASHʱ��
//    REGBITS_CLR(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
//
//    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);  //����ϵͳʱ��ΪHSI 48M
//
////    //SYSCLK = HSI =8MHz = HCLK = PCLK
////    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);
////    CW_SYSCTRL->HSI_f.TRIM = 0x0757;  //У׼ʱ�ӣ���mcu����
////    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
////    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
//    __SYSCTRL_FLASH_CLK_ENABLE();
//    // SYSCTRL_HSE_Enable(SYSCTRL_HSE_MODE_OSC, 16000000, SYSCTRL_HSE_DRIVER_LEVEL5,
//    //  RCC_HSE_FLT_CLOSE);
//    // SYSCTRL_LSE_Enable(SYSCTRL_LSE_MODE_OSC, SYSCTRL_LSE_DRIVER_LEVEL2);
////    SYSCTRL_SysClk_Switch(SYSCTRL_SYSCLKSRC_HSI);
////    SYSCTRL_SystemCoreClockUpdate(MAIN_FREQ);
//    __SYSCTRL_GPIOB_CLK_ENABLE();
//    __SYSCTRL_GPIOA_CLK_ENABLE();
//  __SYSCTRL_GTIM1_CLK_ENABLE();
//    /* ʹ����������ʱ��*/
//    CW_SYSCTRL->AHBEN  = 0xFFFFFFFF;
//    CW_SYSCTRL->APBEN1 = 0xFFFFFFFF;
//    CW_SYSCTRL->APBEN2 = 0xFFFFFFFF;
//    REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 | bv1));
//    SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
//    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
//
//    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
//    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);


}


/**
 * @brief ����GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //UART TX RX ����
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;

    GPIO_InitStruct.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStruct);


    GPIO_InitStruct.Pins = EN_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(EN_GPIO_PORT, &GPIO_InitStruct);
    GPIO_WritePin(EN_GPIO_PORT, EN_GPIO_PIN, GPIO_Pin_RESET);







	PB07_AFx_GPIO();
	GPIO_RST2GPIO();


    //display
    GPIO_InitStruct.Pins = IN_SI_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(IN_SI_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pins = IN_SCK_GPIO_PIN | IN_RCK_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(IN_RCKGPIO_PORT, &GPIO_InitStruct);


    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(Curr_V_GPIO_PORT, &GPIO_InitStruct);
    Curr_V_AFRX;
//KEY1
    GPIO_InitStruct.Pins = KEY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

//KEY2
    GPIO_InitStruct.Pins = KEY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);


//KEY3
    GPIO_InitStruct.Pins = KEY3_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStruct);

//  PA08_AFx_GPIO();
//
//    GPIO_InitStruct.Pins = DIAG1_GPIO_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.IT = GPIO_IT_NONE;
//    GPIO_Init(DIAG1_GPIO_PORT, &GPIO_InitStruct);

    PB00_AFx_GPIO();
    PB01_AFx_GPIO();
	
#ifdef HW_VER2
	
    GPIO_InitStruct.Pins =  DIAG2_GPIO_PIN | DIAG3_GPIO_PIN|DIAG4_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
    GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
    GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);

	
#endif
#ifdef HW_VER1
		

    GPIO_InitStruct.Pins =  DIAG2_GPIO_PIN | DIAG3_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_RESET);
    GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_RESET);

#endif



    //cs1237 #define
    GPIO_InitStruct.Pins = CS1237_SCL_Pin ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(CS1237_SCL_GPIO_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pins =  CS1237_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(CS1237_SCL_GPIO_Port, &GPIO_InitStruct);
	GPIO_WritePin(DIAG3_GPIO_PORT, CS1237_DOUT_Pin, GPIO_Pin_RESET);



    PB04_DIGTAL_ENABLE();
    PB04_DIR_OUTPUT();
    PB04_PUSHPULL_ENABLE();
    PB04_AFx_GTIM1CH3();
    PWM_OutputConfig();

}



///**
// * @brief ����UART
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
    UART_ITConfig(DEBUG_UARTx, UART_IT_RC | UART_IT_RXIDLE, ENABLE);
    UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
    UART_ClearITPendingBit(CW_UART1, UART_IT_RXIDLE);

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
    printf("JID = 0x%X\r\n", JedecID);
    for (uint8_t i = 0; i < 8; i++)
    {
        printf("%X ", UniqueID[i]);
    }

    /* Check JEDEC ID */
    if (JedecID == sJedecID)
    {
        printf("\r\n\nFLASH Detected\r\n");;

        //�������� 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //д����
        SPI_FLASH_BufferWrite(TxBuffer, FLASH_WriteAddress, BufferSize);


        //������
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);


        //���д�������������������Ƿ�һ��
        TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

        //�������� 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //������
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);

        if (TransferStatus == PASSED)
        {
            PB03_SETHIGH();
            printf("\r\nFLASH Success\r\n");
        }
        else
        {
            printf("\r\nFLASH Error 1\r\n");
        }

        for (uint8_t j = 0; j < BufferSize; j++)
        {
            if (RxBuffer[j] != 0xFF)
            {
                printf("\r\nFLASH Error 2\r\n");
            }
        }
    }
    else// (FlashID ��= sFLASH_ID)
    {
        printf("\r\nFLASH Error 3\r\n");
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
    GPIOA_INTFLAG_CLR(0xFFFF);       //clear GPIOA ALL INT FLAG

    NVIC_EnableIRQ(GPIOB_IRQn);
    NVIC_EnableIRQ(GPIOA_IRQn);
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_EnableIRQ(GTIM1_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);


    __enable_irq();

}
void PWM_OutputConfig(void)
{

    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct = {DISABLE, DISABLE, 0};
    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 48 - 1;
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 1000 - 1;
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_DIS;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

    GTIM_OCModeCfgStruct.FastMode = DISABLE;
    GTIM_OCModeCfgStruct.OCMode = GTIM_OC_MODE_PWM1;
    GTIM_OCModeCfgStruct.OCPolarity = GTIM_OC_POLAR_NONINVERT;
    GTIM_OCModeCfgStruct.PreloadState = DISABLE;
    GTIM_OC3ModeCfg(CW_GTIM1, &GTIM_OCModeCfgStruct);

    GTIM_SetCompare3(CW_GTIM1, 250 - 1);
    GTIM_OC3Cmd(CW_GTIM1, ENABLE);
    GTIM_Cmd(CW_GTIM1, ENABLE);

}

void delay_us(uint32_t a)
{
    unsigned char i, j;
    uint32_t b;
    for (b = 0; b < a; b++)
    {
        i = 1;
        j = 1;
        do
        {
            //while (--j);
        }
        while (--i);
    }
}
void delay1_us(uint32_t a)
{
    unsigned char i, j;
    uint32_t b;
//    for (b = 0; b < a; b++)
//    {
//        i = 1;
//        j = 1;
//        do
//        {
//            //while (--j);
//        }
//        while (--i);
//    }
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
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2,
                     uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
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
#endif
