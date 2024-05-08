
/**--------------File Info------------------------------------------------------------------------------
** File name:             rf200_drv.c
** Last modified Date:  2018-05-30
** Last Version:        V1.00
** Descriptions:        RF200 FRAME HANDLE
**------------------------------------------------------------------------------------------------------
** Created date:        2018-05-30
** Version:             V1.00
** Descriptions:        RF200 FRAME HANDLE
**
********************************************************************************************************/
#define _RF200_DRV_MANL
#include "uart.h"

#include "rf_drv.h"
unsigned char rf_cmd[11] = {0xBB, 0x00, 0xC0, 0x00, 0x0E, 0x34, 0x04, 0x00, 0x00, 0x00, 0x00};
unsigned char id_card_num[8 * 18] = {0xBB, 0x00, 0xC0, 0x00, 0x0E, 0x34, 0x04, 0x00, 0x00, 0x00, 0x00};
unsigned char buf[18][8] =
{
    {24, 01, 02, 00, 00, 00, 85},
    {24, 01, 02, 00, 00, 00, 84},
    {24, 01, 02, 00, 00, 00, 83},
    {24, 01, 02, 00, 00, 00, 82},
    {24, 01, 02, 00, 00, 00, 81},
    {24, 01, 02, 00, 00, 00, 80},
    {24, 01, 02, 00, 00, 00, 79},
    {24, 01, 02, 00, 00, 00, 78},
    {24, 01, 02, 00, 00, 00, 77},
    {24, 01, 02, 00, 00, 00, 76},
    {24, 01, 02, 00, 00, 00, 75},
    {24, 01, 02, 00, 00, 00, 74},
    {24, 01, 02, 00, 00, 00, 73},
    {24, 01, 02, 00, 00, 00, 72},
    {24, 01, 02, 00, 00, 00, 71},
    {24, 01, 02, 00, 00, 00, 70},
    {24, 01, 02, 00, 00, 00, 69},
    {24, 01, 02, 00, 00, 00, 68}
};
rf_recv_stru rf_recv_usr;

void rf_tx(unsigned char card_id)
{
    unsigned char tx_buf[21];
    unsigned char sum;
    memcpy(tx_buf, rf_cmd, 11);
    memcpy(&tx_buf[11], &buf[card_id][0], 8);
    sum = sum_Compute(&tx_buf[1], 18);
    tx_buf[19] = sum;
    tx_buf[20] = 0x7e;
    uart_transmit(UART_RF, tx_buf, 21);

}
void rf_recv()
{
    uart_recv_stru *uart_rf;
    unsigned char sig_bit;
    unsigned char tune;
    unsigned int temp_hbyte, temp_lbyte;
    unsigned int temp;
    float cal_temp;

    uart_rf = get_uart_recv_stru(UART_RF);
    if (uart_rf->uartRecFlag == 1)
    {
        if (uart_rf->uartRecBuff[0] == RF_HEADER)
        {
            if (uart_rf->uartRecBuff[1] == RF_TYPE && uart_rf->uartRecBuff[2] == RF_COMMAND)
            {
                sig_bit = uart_rf->uartRecBuff[19];
                tune = uart_rf->uartRecBuff[20];
                temp_hbyte = uart_rf->uartRecBuff[21];
                temp_lbyte = uart_rf->uartRecBuff[22];
                if (temp_hbyte & 0xf0 <= 0x30)
                {
                    temp = temp_hbyte & 0x0f;
                    temp = temp_hbyte << 4;
                    temp = temp_hbyte + temp_lbyte;
                    if (sig_bit == 0x01)
                    {
                        cal_temp = temp - tune;
                        cal_temp = (cal_temp - 500);
                        cal_temp = cal_temp / 5.4817 + 24.9;
                    }
                    else if (sig_bit == 0x00)
                    {
                        cal_temp = temp + tune;
                        cal_temp = (cal_temp - 500);
                        cal_temp = cal_temp / 5.4817 + 24.9;
                    }
                    if (rf_recv_usr.wait_card_id <= 18)
                    {
                        rf_recv_usr.card_T[rf_recv_usr.wait_card_id] = cal_temp;
                        rf_recv_usr.wait_card_id = RF_RECV_FLAG;

                    }
                    cal_temp = 0;
                }

            }
        }
		uart_rf->uartRecFlag = 0;
    }
}

void rf_averge()
{
	unsigned char i;
	float tmp;
	static unsigned char averge_count;
	averge_count = 0;
	tmp =0;
	for(i=0;i<18;i++)
	{
		if(rf_recv_usr.id_status[i] == 1)
		{
			averge_count++;
			tmp = tmp +rf_recv_usr.card_T[i];
			
		}
	}
	if(averge_count>0)
	{
		tmp = tmp /averge_count;
	}
	else
		tmp = 100;
	rf_recv_usr.average_T = tmp;

}
rf_recv_stru*get_rf_status()
{
	return &rf_recv_usr;
}
void rf_ctrl_proc()
{
    static uint32_t tick_start;
    if ((HAL_GetTick() - tick_start) >= RF_POLL_T)
    {
        tick_start = HAL_GetTick();
        if (rf_recv_usr.wait_card_id != RF_RECV_FLAG)
        {
            if (rf_recv_usr.retry_times < RF_RETRY_TIMES)
            {
                rf_recv_usr.retry_times++;
                rf_tx(rf_recv_usr.tx_card_id);
				
            }
            else
            {
                rf_recv_usr.retry_times = 0;
				if(rf_recv_usr.id_status[rf_recv_usr.wait_card_id]<10)
                rf_recv_usr.id_status[rf_recv_usr.wait_card_id] =  
					rf_recv_usr.id_status[rf_recv_usr.wait_card_id]+1;
                rf_recv_usr.tx_card_id++;
                if (rf_recv_usr.tx_card_id >= 18)
                    rf_recv_usr.tx_card_id = 0;
                rf_recv_usr.wait_card_id = rf_recv_usr.tx_card_id;
            }

        }
        else  //recv dat
        {
            rf_recv_usr.id_status[rf_recv_usr.wait_card_id] = 1; 
					
            rf_recv_usr.retry_times = 0;
            rf_recv_usr.tx_card_id++;
            if (rf_recv_usr.tx_card_id >= 18)
                rf_recv_usr.tx_card_id = 0;
            rf_recv_usr.wait_card_id = rf_recv_usr.tx_card_id;
        }
    }
   rf_recv();
}
