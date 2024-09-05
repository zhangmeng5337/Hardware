#include "led.h"


void led_init(void)
{
    HAL_GPIO_WritePin(RUN_GPIO_Port, RUN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(WARN_GPIO_Port, WARN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FAULT_GPIO_Port, FAULT_Pin, GPIO_PIN_RESET);

}
void led_ctrl(unsigned char led_num, unsigned char ctrl)
{
    GPIO_PinState status;

    if (ctrl == OFF)
        status = GPIO_PIN_RESET;
    else if (ctrl == ON)
        status = GPIO_PIN_SET;
    switch (led_num)
    {
        case LED_RUN:
            HAL_GPIO_WritePin(RUN_GPIO_Port, RUN_Pin, status);
            break;
        case LED_WARN:
            HAL_GPIO_WritePin(WARN_GPIO_Port, WARN_Pin, status);
            break;
        case LED_FAULT:
            HAL_GPIO_WritePin(FAULT_GPIO_Port, FAULT_Pin, status);
            break;
        case LED_ALL:
            HAL_GPIO_WritePin(RUN_GPIO_Port, RUN_Pin, status);
            HAL_GPIO_WritePin(WARN_GPIO_Port, WARN_Pin, status);
            HAL_GPIO_WritePin(FAULT_GPIO_Port, FAULT_Pin, status);
            break;
        default:
            break;
    }

}

