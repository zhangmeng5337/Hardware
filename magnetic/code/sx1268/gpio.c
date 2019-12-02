#include "bsp.h"
#include "gpio.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_rst.h"
 void IWDG_Config(void)
{

    if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
  {
    /* IWDGF flag set */
    /* Turn on LED1 */

    /* Clear IWDGF Flag */
    RST_ClearFlag(RST_FLAG_IWDGF);
  }
  
  
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LSI/128)
                         = 0.25s / (LsiFreq/128)
                         = LsiFreq/(128 * 4)
                         = LsiFreq/512
   */
  IWDG_SetReload((uint8_t)(254));
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}
void GPIO_int()
{
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
  /****************************************
   RF_NSS
  ****************************************/
    GPIO_Init(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_NSS_PORT, RADIO_NSS_PIN);
  /****************************************
   RF_RST
  ****************************************/
    GPIO_Init(RADIO_nRESET_PORT, RADIO_nRESET_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_nRESET_PORT, RADIO_nRESET_PIN);
  /****************************************
   Radio_BUSY
  ****************************************/
    GPIO_Init(RADIO_BUSY_PORT, RADIO_BUSY_PIN, GPIO_Mode_In_FL_IT);                //Radio_BUSY
    EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising);
   
    
    /*PB2 PB1 分别代表M0 M1*/
    GPIO_Init(PORT_SX127X_M0, PIN_SX127X_M0, GPIO_Mode_In_PU_No_IT);//GPIO_Mode_In_PU_IT
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising_Falling);
    GPIO_Init(PORT_SX127X_M1, PIN_SX127X_M1, GPIO_Mode_In_PU_No_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising_Falling);


    /*PB0 AUX*/
    GPIO_Init(PORT_SX127X_AUX, PIN_SX127X_AUX, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(PORT_SX127X_AUX, PIN_SX127X_AUX);


    /*SX1276数据接收外部中断*/
    GPIO_Init(RADIO_DIO1_PORT, RADIO_DIO1_PIN, GPIO_Mode_In_FL_IT);                //dio1
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
    //EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
    /*SX1276数据接收外部中断*/
    GPIO_Init(RADIO_DIO3_PORT, RADIO_DIO3_PIN, GPIO_Mode_In_FL_IT);                //dio3
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising); 

     GPIO_Init(RADIO_TX_PORT, RADIO_TX_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_ResetBits(RADIO_TX_PORT, RADIO_TX_PIN);

    GPIO_Init(RADIO_RX_PORT, RADIO_RX_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(RADIO_RX_PORT, RADIO_RX_PIN); 
}
