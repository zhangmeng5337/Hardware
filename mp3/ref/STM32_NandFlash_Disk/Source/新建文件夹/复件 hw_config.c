/*****************  (C) COPYRIGHT 2008 Create  *****************
       
    �� �� ����STM32ϵͳ����
    ��    ˾��
    ��    �ߣ�liu_xf
    �� �� �ţ�V1.00
    �������ڣ�
    �޸����ڣ�   
    ���뻷����
    Ӳ��ƽ̨��
    ���������� ��MCUӲ����صļĴ�������
    
              

***************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
#include "hw_config.h"
#include "fsmc_nand.h"
#include "sdcard.h"
#include "mass_mal.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void RCC_Config(void);
/* Private functions ---------------------------------------------------------*/

/********************************************************************
�������ܣ�ϵͳ��ʼ��
��ڲ������ޡ�
��    �أ��ޡ�
��    ע��
            ����ϵͳʱ��
			��ʼ��������ʱ��
			����Ҫ�õ�����
			�ж�����
			����IO���ţ�����IO����
			��ʼ���洢��
			��ʼ��USB
********************************************************************/
void System_Init(void)
{

  /* RCC configuration */
  RCC_Config();		 //��ʱ����صļĴ�������
  //IO���� ------------------
  USB_Disconnect_Config();	  //USB�����ӶϿ���������
  Led_Config();  			  //����LED
  /* nand configuration */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);	//ʹ��
  FlashInit();  //��ʼ��NAND
  SD_Init();

  Set_USBClock();   //����USBʱ��
  
  USB_Interrupts_Config();	//USB�ж�����
  USB_Init();
  
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
static void RCC_Config(void)
{

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();	   //��RCC�Ĵ����豸ȱʡֵ

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);  //�ⲿ��������HSE ����

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//�ȴ�HSE����

  if (HSEStartUpStatus == SUCCESS)	  //���HSE�����ȶ��Ҿ���
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //Ԥȡָ����ʹ��

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);	//����FLASH�洢����ʱʱ��������Ϊ2��ʱ����

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //����AHBʱ�ӣ�ϵͳʱ��

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);	 //����APB2ʱ�� �� HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);	   //����APB1ʱ�� �� HCLK/2

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//����ADCʱ�ӣ�PCLK/6

	//PLL����
	//PLL����ʱ�ӣ�HSEʱ��Ƶ��   ��� 9��Ƶ��HSE��9��
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //72M

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	 //�ȴ�PLL����
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//ѡ��PLL��Ϊϵͳʱ��

    /* Wait till PLL is used as system clock source */
	//�ȴ�����ϵͳʱ��ΪPLL�ɹ�
	//����ֵΪ	00��HSI  04��HSE  08��PLL
    while (RCC_GetSYSCLKSource() != 0x08)  
    {}

  }
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
                  ������ӶϿ�USB���� 
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);	  //����USB
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);	  //�Ͽ�USB
  }
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* USBCLK = PLLCLK */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);	 //USBʱ��ΪPLLʱ�ӳ���1.5	Ϊ48MHz

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/********************************************************************
�������ܣ�USB�ж�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Set the Vector Table base location at 0x20000000 */
 // NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);	   //�ж�������ΪFLASH

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//�������ȼ��飺���ȼ��������ȼ�.

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;  //USB�����ȼ��ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	  //�������ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//���ô����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN_TX_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/********************************************************************
�������ܣ�USB�����ӶϿ����� IO��������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void USB_Disconnect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT_PIN used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
}

/********************************************************************
�������ܣ�LED IO��������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Led_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_LED_PORT clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LED, ENABLE);	  //LED����IO��ʱ��ʹ��

  /* Configure USB_LED_PORT pin6, pin7, pin8 and pin9 as output push-pull */
  //PF6��LED1   USBö�ٳɹ�
  //PF7��LED2
  //PF8��LED3	��дLED
  //PF9��LED4
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;	   //ѡ�йܽ�6,7,8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO��������Ƶ��Ϊ50MHz
  GPIO_Init(USB_LED_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}


/*******************************************************************************
* Function Name  : Led_RW_ON
* Description    : Turn ON the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_ON(void)
{
  GPIO_SetBits(USB_LED_PORT, GPIO_Pin_8);	 //IO����ߵ�ƽ��LED����
}

/*******************************************************************************
* Function Name  : Led_RW_OFF
* Description    : Turn off the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_OFF(void)
{
  GPIO_ResetBits(USB_LED_PORT, GPIO_Pin_8);
}
/*******************************************************************************
* Function Name  : USB_Configured_LED
* Description    : Turn ON the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Configured_LED(void)
{
  GPIO_SetBits(USB_LED_PORT, GPIO_Pin_6);	  
}

/*******************************************************************************
* Function Name  : USB_NotConfigured_LED
* Description    : Turn off the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_NotConfigured_LED(void)
{
  GPIO_ResetBits(USB_LED_PORT, GPIO_Pin_6);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(vu32*)(0x1FFFF7E8);	//96λ��оƬ���֤���룬ֻ��
  Device_Serial1 = *(vu32*)(0x1FFFF7EC);
  Device_Serial2 = *(vu32*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    MASS_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
    MASS_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
    MASS_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
    MASS_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

    MASS_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
    MASS_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
    MASS_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
    MASS_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

    MASS_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
    MASS_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
    MASS_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
    MASS_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
