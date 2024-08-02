#include "cdg.h"
#include "uart.h"
#include "filter.h"
#include "cmd_queue.h"

CDG_stru sensor_cdg;
CDG_PRESSURE_stru sensor_pressure;
CDG_config_stru sensor_config;
uint8_t cmd_buffer[CMD_MAX_SIZE];
void cdg_zero()
{
    static uint32_t tick;
    if (sensor_pressure.zero_flag == 1)
    {
        if ((HAL_GetTick() - tick) >= 3000)
        {
            HAL_GPIO_WritePin(CDG_ZERO_GPIO_Port, CDG_ZERO_Pin, GPIO_PIN_RESET);
            sensor_pressure.zero_flag = 0;
        }
        else
            HAL_GPIO_WritePin(CDG_ZERO_GPIO_Port, CDG_ZERO_Pin, GPIO_PIN_SET);
    }
    else
    {
        tick = HAL_GetTick();
    }


}
void cdg_config()
{
    unsigned char *pb;
    sensor_config.checksum = 0;
    if (sensor_config.update == 1)
    {
        sensor_config.len      = 3;
        sensor_config.checksum = sensor_config.command + sensor_config.addr;
        sensor_config.checksum = sensor_config.checksum + sensor_config.dat;
        sensor_config.update = 0;
        pb[0] = 3;
        pb[1] = sensor_config.command;
        pb[2] = sensor_config.addr;
        pb[3] = sensor_config.dat;
        pb[4] = sensor_config.checksum;
        uart_transmit(UART_CDG, pb,  pb[0] + 2);
    }
	cdg_zero();

}
CDG_config_stru *get_cdg_config()
{
    return &sensor_config;
}
CDG_PRESSURE_stru *get_cdg_status(void)
{
    return &sensor_pressure;

}

unsigned char cdg_data_analy()
{
    unsigned char res;
    qsize  size = 0;
    res = 1;

    size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); //从缓冲区中获取一条指令
    if (size > 0) //接收到指令
    {
        sensor_cdg.dat_len         =  cmd_buffer[0];
        sensor_cdg.page_NO         =  cmd_buffer[1];
        sensor_cdg.status          =  cmd_buffer[2];
        sensor_cdg.error           =  cmd_buffer[3];
        sensor_cdg.value_high_byte =  cmd_buffer[4];
        sensor_cdg.value_low_byte  =  cmd_buffer[5];
        sensor_cdg.read_command    =  cmd_buffer[6];
        sensor_cdg.sensor_type     =  cmd_buffer[7];
        sensor_cdg.checksum        =  cmd_buffer[8];
        res = 0;

    }


//    if (get_uart_recv_stru(UART_CDG)->uartRecFlag == 1)
//    {
//
//        unsigned char recv_len_tmp;
//        unsigned char *pb;
//        unsigned char i, j;
//        unsigned int checksum;
//        recv_len_tmp = get_uart_recv_stru(UART_CDG)->uartRecLen;
//        pb = get_uart_recv_stru(UART_CDG)->uartRecBuff;
    // if (recv_len_tmp > CDG_SEND_STR_LEN)
    {
//            for (i = 0; i < recv_len_tmp; i++)
//            {
//                if (pb[i] == CDG_SEND_STR_LEN)
//                    break;
//            }
//            checksum = 0;
//            res = 1;
//                     i = 0;
//            for (j = i + 1; j < (i + CDG_SEND_STR_LEN + 1); j++)
//            {
//                checksum = pb[j] + checksum;
//            }
//            if ((unsigned char)checksum == pb[i + CDG_SEND_STR_LEN + 1])
//            {
//                sensor_cdg.dat_len         =  pb[i];
//                sensor_cdg.page_NO         =  pb[i + 1];
//                sensor_cdg.status          =  pb[i + 2];
//                sensor_cdg.error           =  pb[i + 3];
//                sensor_cdg.value_high_byte =  pb[i + 4];
//                sensor_cdg.value_low_byte  =  pb[i + 5];
//                sensor_cdg.read_command    =  pb[i + 6];
//                sensor_cdg.sensor_type     =  pb[i + 7];
//                sensor_cdg.checksum        =  pb[i + 8];
//                res = 0;
//
//            }
//
//        }
//     get_uart_recv_stru(UART_CDG)->uartRecFlag = 0;
//        memset(pb,0,UART_REC_SIZE);
    }
    return res;
}
unsigned char sensor_params_sel()
{
    unsigned char unit_tmp, unit_tmp2;
    if (sensor_cdg.page_NO == 2 || sensor_cdg.page_NO == 3)
    {

        unit_tmp = sensor_cdg.status;
        unit_tmp = unit_tmp >> 4;
        unit_tmp = unit_tmp & 0x03;

        unit_tmp2 = sensor_cdg.sensor_type;
        switch (unit_tmp2 & 0x0f) //F.S.R._Exp
        {
            case 0:
                sensor_pressure.fsr_exp = 0.001;
                break;
            case 1:
                sensor_pressure.fsr_exp = 0.01;
                break;
            case 2:
                sensor_pressure.fsr_exp = 0.1;
                break;
            case 3:
                sensor_pressure.fsr_exp = 1;
                break;
            case 4:
                sensor_pressure.fsr_exp = 10.0;
                break;
            case 5:
                sensor_pressure.fsr_exp = 100.0;
                break;
            case 6:
                sensor_pressure.fsr_exp = 1000.0;
                break;
            case 7:
                sensor_pressure.fsr_exp = 10000.0;
                break;

        }

        unit_tmp2 = unit_tmp2 >> 4;
        unit_tmp2 = unit_tmp2 & 0x0f;
        switch (unit_tmp2) //F.S.R._Mantissa
        {
            case 0:
                sensor_pressure.fsr = 1.0;
                break;
            case 1:
                sensor_pressure.fsr = 1.1;
                break;
            case 2:
                sensor_pressure.fsr = 2.0;
                break;
            case 3:
                sensor_pressure.fsr = 2.5;
                break;
            case 4:
                sensor_pressure.fsr = 5.0;
                break;
        }

        if (unit_tmp2 == 0 || unit_tmp2 > 1) //factor a factor b
        {
            switch (unit_tmp)
            {
                case 0:  //mbar
                {
                    sensor_pressure.factor_a = 1.3332;
                    sensor_pressure.factor_b = 24000;
                }
                break;
                case 1://torr
                {
                    sensor_pressure.factor_a = 1.0000;
                    sensor_pressure.factor_b = 32000;
                }
                break;
                case 2://pa
                {
                    sensor_pressure.factor_a = 133.32;
                    sensor_pressure.factor_b = 24000;
                }
                break;

            }
        }
        else if (unit_tmp2 == 1)
        {
            switch (unit_tmp)
            {
                case 0://mbar
                {
                    sensor_pressure.factor_a = 13332;
                    sensor_pressure.factor_b = 26400;
                }
                break;
            }

        }
    }
    else if (sensor_cdg.page_NO == 4) //CDG025D 10V
    {
        sensor_pressure.factor_b = 32767;
    }
    return (sensor_cdg.status >> 4) & 0x03;
}
void unit_convert()
{
    static unsigned char i = 0;
    switch (sensor_pressure.unit)
    {
        case UNIT_MBAR:
            sensor_pressure.pressure_pa = sensor_pressure.pressure / 1.3332;
            break;//mbar--->pa
        case UNIT_TORR:
            sensor_pressure.pressure_pa = sensor_pressure.pressure * 133.32;
            break;//mbar--->pa
        case UNIT_PA:
            sensor_pressure.pressure_pa = sensor_pressure.pressure;
            break;//mbar--->pa


    }
    sensor_pressure.pressure_torr = sensor_pressure.pressure_pa / 133.32;

    sensor_pressure.buf[i++] = sensor_pressure.pressure_torr;
    //sensor_pressure.pressure_filter_torr = sensor_pressure.pressure_torr ;

    sensor_pressure.pressure_filter_torr =  average_filter_pressure(
            sensor_pressure.pressure_torr);//GetMedianNum(sensor_pressure.buf, i);
    sensor_pressure.pressure_nofilter_torr = sensor_pressure.pressure_torr;
    sensor_pressure.pressure_filter_pa = sensor_pressure.pressure_filter_torr *
                                         133.32;
    if (i >= PREE_SIZE)
        i = 0;

    //filter(5,sensor_pressure.buf);
}
void sensor_cal()
{
    unsigned char error;
    error = sensor_cdg.error & 0x7f;
    //if (error == 0x00)
    //if (error == 0x00)
    {
        sensor_cdg.rec_count++;
        sensor_pressure.unit = sensor_params_sel();
        int value_16;
        unsigned int tmp;
        tmp = sensor_cdg.value_high_byte;
        tmp = tmp << 8;
        tmp = tmp | sensor_cdg.value_low_byte;
        if (sensor_cdg.value_high_byte & 0x80)
        {
            tmp = (~tmp);
            tmp = tmp & 0x0000ffff;
            value_16 = tmp + 1;
            value_16 = -value_16;
        }
        else
            value_16 = tmp;
        sensor_pressure.pressure = value_16 * sensor_pressure.factor_a /
                                   sensor_pressure.factor_b;
        sensor_pressure.pressure = sensor_pressure.pressure * sensor_pressure .fsr;
        sensor_pressure.pressure = sensor_pressure.pressure * sensor_pressure.fsr_exp;
        unit_convert();
    }
}
void sensor_cdg_proc()
{
    if (cdg_data_analy() == 0)
    {
        sensor_cal();
    }
    cdg_config();
}
