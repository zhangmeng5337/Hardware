#include "relay_driver.h"

void relay_init()
{
    relay_ctrl(0xff, GPIO_PIN_RESET);

}
void relay_ctrl(unsigned char relay_num, unsigned char status)
{
    GPIO_PinState ctrl;
    if (status == ON)
        ctrl = GPIO_PIN_RESET;
    else
        ctrl = GPIO_PIN_SET;
    switch (relay_num)
    {
        case ROOT_UP_RELAY:
            HAL_GPIO_WritePin(relay_ctrl_1_GPIO_Port, relay_ctrl_1_Pin, ctrl);
            break;
        case HIGHT_FLOW_UP_RELAY:
            HAL_GPIO_WritePin(relay_ctrl_2_GPIO_Port, relay_ctrl_2_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_3_GPIO_Port, relay_ctrl_3_Pin, GPIO_PIN_SET);
            break;
        case HIGHT_FLOW_UP_SRELAY:
            HAL_GPIO_WritePin(relay_ctrl_2_GPIO_Port, relay_ctrl_2_Pin, ctrl);
             // HAL_GPIO_WritePin(relay_ctrl_3_GPIO_Port, relay_ctrl_3_Pin, GPIO_PIN_SET);
            break;

        case LOW_FLOW_UP_RELAY:
            HAL_GPIO_WritePin(relay_ctrl_3_GPIO_Port, relay_ctrl_3_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_2_GPIO_Port, relay_ctrl_2_Pin, GPIO_PIN_SET);
            break;
        case LOW_FLOW_UP_SRELAY:
            HAL_GPIO_WritePin(relay_ctrl_3_GPIO_Port, relay_ctrl_3_Pin, ctrl);
           // HAL_GPIO_WritePin(relay_ctrl_2_GPIO_Port, relay_ctrl_2_Pin, GPIO_PIN_SET);
            break;

        case DOWN_RELAY:
            HAL_GPIO_WritePin(relay_ctrl_4_GPIO_Port, relay_ctrl_4_Pin, ctrl);
            break;
        default:
            HAL_GPIO_WritePin(relay_ctrl_1_GPIO_Port, relay_ctrl_1_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_2_GPIO_Port, relay_ctrl_2_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_3_GPIO_Port, relay_ctrl_3_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_4_GPIO_Port, relay_ctrl_4_Pin, ctrl);
            HAL_GPIO_WritePin(relay_ctrl_5_GPIO_Port, relay_ctrl_5_Pin, ctrl);
            break;
    }
}
