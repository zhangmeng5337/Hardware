/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    interrupts.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "base_types.h"
#include "cw32l010_gpio.h"
#include "uart_hal.h"
#include "interrupts_cw32l010.h"
#include "cw32l010_uart.h"
#include "key.h"
#include "display.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
	extern volatile uint8_t gKey1Status,gKey2Status;
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern void GPIOA_IRQHandlerCallback(void);
extern void GPIOB_IRQHandlerCallback(void);

/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* USER CODE END EV */


/******************************************************************************/
/*           Cortex-M0P Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn */

    /* USER CODE END NonMaskableInt_IRQn */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn */

    /* USER CODE END HardFault_IRQn */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn */

        /* USER CODE END W1_HardFault_IRQn */
    }
}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn */

    /* USER CODE END SVCall_IRQn */
}


/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn */

    /* USER CODE END PendSV_IRQn */
}


/******************************************************************************/
/* CW030 Peripheral Interrupt Handlers                                        */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_ps030.s).                        */
/******************************************************************************/

/**
 * @brief This funcation handles WDT
 */
void WDT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LVD
 */
void LVD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles RTC
 */
void RTC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles FLASHRAM
 */
void FLASHRAM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SYSCTRL
 */
void SYSCTRL_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOA
 */
void GPIOA_IRQHandler(void)
{
    /* USER CODE BEGIN */
	GPIOA_IRQHandlerCallback();

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOB
 */
void GPIOB_IRQHandler(void)
{
    /* USER CODE BEGIN */
	GPIOB_IRQHandlerCallback();

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOC
 */
void GPIOC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOF
 */
void GPIOF_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMACH1
 */
void DMACH1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMACH23
 */
void DMACH23_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMA34
 */
void DMACH4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles ADC
 */
//extern volatile uint8_t gFlagIrq;
//extern uint16_t gCntEoc;
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN */
//		    gFlagIrq = CW_ADC->ISR;
//    CW_ADC->ICR = 0x00;

    /* USER CODE END */
}

/**
 * @brief This funcation handles ATIM
 */
void ATIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC1
 */
void VC1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC2
 */
void VC2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM1
 */
void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM2
 */
void GTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM1
 */
void BTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM2
 */
void BTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM3
 */
void BTIM3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

void SysTick_IRQnHandler(void)
{
	uint32_t tick;
	tick++;

	  #ifdef DISPLAY_DEBUG 
	if(getTaskIndex() == 0)
		 display();
	else
	display_led(GetDisp()->cusor, getKey()->bit_sel);
#endif
}

/**
 * @brief This funcation handles I2C1
 */
void I2C1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SPI1
 */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART1
 */
void UART1_IRQHandler(void)
{
	  uint8_t result;

		/* USER CODE BEGIN */
		if (UART_GetITStatus(CW_UART1, UART_IT_RC) != RESET)
		{
			result = UART_ReceiveData_8bit(CW_UART1);
			uart_recv_proc(result,0);
			UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
		}
		if (UART_GetITStatus(CW_UART1, UART_IT_RXIDLE) != RESET)
		{
			//uart_recv_proc(UART_ReceiveData_8bit(CW_UART1),1);
			getuart()->recv_update = 1;
			UART_ClearITPendingBit(CW_UART1, UART_IT_RXIDLE);
			UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
		}
	

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART2
 */
void UART2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART3
 */
void UART3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles AWT
 */
void AWT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles FAULT
 */
void FAULT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}



/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
