#include "key.h"
key_stru key_usr;

void key_init()
{
	key_usr.detal_cnt = 0;
	key_usr.encode_cnt = 0;
	key_usr.encode_Direction = 0;
	key_usr.encode_nodivcnt = 0;
	key_usr.overnumber = 0;

}
key_stru *get_key_state()
{
    return &key_usr;
}
void key_tick(void)
{
    if (key_usr.key_press_flag == 1)
    {
        key_usr.press_delay_time ++;
    }
    else
        key_usr.press_delay_time = 0;
}
void key_scan(void)
{
    if (key_usr.press_delay_time >= KEY_DELAY_TIME)
    {
        if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin))
        {
            key_usr.key1 = 1;
            key_usr.press_update = 1;

        }

        if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin))
        {
            key_usr.key2 = 1;
            key_usr.press_update = 1;

        }
        if (HAL_GPIO_ReadPin(KEYSW_GPIO_Port, KEYSW_Pin))//
        {
            //key_usr.key3 = 1;
            key_usr.ok_press_update = 1;
        }
        if (key_usr.encode_update == 1)
        {
           // key_usr.key3 = 1;
            key_usr.encode_update = 1;//

        }

        //key_usr.press_delay_time = 0;//去抖动清零
        key_usr.key_press_flag = 0;//按下标志清零

    }

}


