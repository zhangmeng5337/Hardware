#include "hall.h"
hall_stru hall_usr;

uint8_t get_hall_state(void)
{
    uint8_t state = 0; 
    uint8_t A,B,C;
    A = HAL_GPIO_ReadPin(MHU_GPIO_Port, MHU_Pin);
    B = HAL_GPIO_ReadPin(MHV_GPIO_Port, MHV_Pin);
    C = HAL_GPIO_ReadPin(MHW_GPIO_Port, MHW_Pin);
    state = (C << 2) + (B << 1) + A;
    return state;
}
