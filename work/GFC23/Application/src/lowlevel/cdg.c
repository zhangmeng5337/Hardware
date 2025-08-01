#include <stdint.h>
#include "main.h"
#include "cdg.h"
#include "filter.h"
#include "commsapi.h"
#include "cmd_queue.h"

CDG_stru sensor_cdg;
CDG_PRESSURE_stru sensor_pressure;
CDG_config_stru sensor_config;
uint8_t cmd_buffer[CMD_MAX_SIZE];

/**
  @brief Configure CDG?
*/
void cdg_config()
{
  unsigned char *pb;
  sensor_config.checksum = 0;
  if (sensor_config.update == 1) {
    sensor_config.len      = 3;
    sensor_config.checksum = sensor_config.command + sensor_config.addr;
    sensor_config.checksum = sensor_config.checksum + sensor_config.dat;
    sensor_config.update = 0;
    pb[0] = 3;
    pb[1] = sensor_config.command;
    pb[2] = sensor_config.addr;
    pb[3] = sensor_config.dat;
    pb[4] = sensor_config.checksum;
    HAL_UART_Transmit(&CDGINTERFACE, pb, pb[0] + 2, 100);
  }
}

/**
  @brief Return pointer to to sensor config structure.
*/
CDG_config_stru *get_cdg_config()
{
  return &sensor_config;
}

/**
  @brief Return cdg pressure structure. Why status? this returns a structure.Consider refactoring to avoid confussion
*/
CDG_PRESSURE_stru *get_cdg_status(void)
{
  return &sensor_pressure;
}

/**
    @brief CDG API initialization that follows all the rest of LLD inits and hooks
*/
void init_CDG_API() 
{    
  __HAL_UART_ENABLE_IT(&CDGINTERFACE, UART_IT_RXNE);
  HAL_UART_Receive_IT(&CDGINTERFACE, &push_CDG_data, 1);
}

/**
*/
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
  return res;
}

/**
*/
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

/**
    @brief Why dont we use our standard unit library?
*/
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

/**
  @brief
*/
void sensor_cal()
{
  unsigned char error;
  error = sensor_cdg.error & 0x7f;  // used where?

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
  else {
    value_16 = tmp;
  }
  sensor_pressure.pressure = value_16 * sensor_pressure.factor_a / sensor_pressure.factor_b;
  sensor_pressure.pressure = sensor_pressure.pressure * sensor_pressure .fsr;
  sensor_pressure.pressure = sensor_pressure.pressure * sensor_pressure.fsr_exp;
  unit_convert();
}

/**
*/
void sensor_cdg_proc()
{
  if (cdg_data_analy() == 0) {
    sensor_cal();
  }
  cdg_config(); // What is this for?
}
