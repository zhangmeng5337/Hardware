/*
 ******************************************************************************
 * @file    _read_data_polling.c
 * @author  Sensors Software Solution Team
 * @brief   This file show how to get data from sensor.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/*
 * This example was developed using the following STMicroelectronics
 * evaluation boards:
 *
 * - STEVAL_MKI109V3
 * - NUCLEO_F411RE
 * - DISCOVERY_SPC584B
 *
 * Used interfaces:
 *
 * STEVAL_MKI109V3    - Host side:   USB (Virtual COM)
 *                    - Sensor side: SPI(Default) / I2C(supported)
 *
 * NUCLEO_STM32F411RE - Host side: UART(COM) to USB bridge
 *                    - Sensor side: I2C(Default) / SPI(supported)
 *
 * DISCOVERY_SPC584B  - Host side: UART(COM) to USB bridge
 *                    - Sensor side: I2C(Default) / SPI(supported)
 *
 * If you need to run this example on a different hardware platform a
 * modification of the functions: `platform_write`, `platform_read`,
 * `tx_com` and 'platform_init' is required.
 *
 */

/* STMicroelectronics evaluation boards definition
 *
 * Please uncomment ONLY the evaluation boards in use.
 * If a different hardware is used please comment all
 * following target board and redefine yours.
 */

//#define STEVAL_MKI109V3  /* little endian */
//#define NUCLEO_F411RE    /* little endian */
//#define SPC584B_DIS      /* big endian */

/* ATTENTION: By default the driver is little endian. If you need switch
 *            to big endian please see "Endianness definitions" in the
 *            header file of the driver (_reg.h).
 */


/* Includes ------------------------------------------------------------------*/
#include "lsm6ds3_reg.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "lsm6ds3tr.h"
extern I2C_HandleTypeDef hi2c1;
#define SENSOR_BUS hi2c1

//#if defined(NUCLEO_F411RE)
//#include "stm32f4xx_hal.h"
//#include "usart.h"
//#include "gpio.h"
//#include "i2c.h"

//#elif defined(STEVAL_MKI109V3)
//#include "stm32f4xx_hal.h"
//#include "usbd_cdc_if.h"
//#include "gpio.h"
//#include "spi.h"
//#include "tim.h"

//#elif defined(SPC584B_DIS)
//#include "components.h"
//#endif

/* Private macro -------------------------------------------------------------*/
#define    BOOT_TIME   20 //ms

/* Private variables ---------------------------------------------------------*/
AccAng_stru AccAng_usr;

static uint8_t whoamI, rst;
//static uint8_t tx_buffer[TX_BUF_DIM];

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*   WARNING:
 *   Functions declare in this section are defined at the end of this file
 *   and are strictly related to the hardware platform used.
 *
 */
static int32_t platform_write2(void *handle, uint8_t reg,
                              uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read2(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
static void tx_com( uint8_t *tx_buffer, uint16_t len );
//static void platform_delay(uint32_t ms);
//static void platform_init(void);
stmdev_ctx_t dev_ctx;
AccAng_stru *GetAccAngtic(void)
{
	return &AccAng_usr;
}
extern I2C_HandleTypeDef hi2c1;

/* Main Example --------------------------------------------------------------*/
unsigned char lsm6ds3tr_init()
{
	  /* Initialize mems driver interface */
	  
	  dev_ctx.write_reg = platform_write2;
	  dev_ctx.read_reg = platform_read2;
	  dev_ctx.handle = &hi2c1;
	  /* Init test platform */
	//	platform_init();
	  /* Wait sensor boot time */
	 // platform_delay(BOOT_TIME);
	  /* Check device ID */
	  whoamI = 0;
  lsm6ds3_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LSM6DS3_ID)
     return 1;

  /* Restore default configuration */
  lsm6ds3_reset_set(&dev_ctx, PROPERTY_ENABLE);

  do {
    lsm6ds3_reset_get(&dev_ctx, &rst);
  } while (rst);

  /*  Enable Block Data Update */
  lsm6ds3_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /* Set full scale */
  lsm6ds3_xl_full_scale_set(&dev_ctx, LSM6DS3_2g);
  lsm6ds3_gy_full_scale_set(&dev_ctx, LSM6DS3_2000dps);
  /* Set Output Data Rate for Acc and Gyro */
  lsm6ds3_xl_data_rate_set(&dev_ctx, LSM6DS3_XL_ODR_12Hz5);
  lsm6ds3_gy_data_rate_set(&dev_ctx, LSM6DS3_GY_ODR_12Hz5);
	return 0;
}
  void lsm6dPowerDown()
  {
	  lsm6ds3_xl_data_rate_set(&dev_ctx, LSM6DS3_XL_ODR_OFF);
	  lsm6ds3_gy_data_rate_set(&dev_ctx, LSM6DS3_GY_ODR_OFF);
  }

  void lsm6ds3tr_c_read_data_polling(void)
{

  /* Read samples in polling mode (no int) */
  //while (1) 
  {
    uint8_t reg;
    /* Read output only if new value is available */
    lsm6ds3_xl_flag_data_ready_get(&dev_ctx, &reg);

    if (reg) {
      /* Read acceleration field data */
      memset(AccAng_usr.data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
      lsm6ds3_acceleration_raw_get(&dev_ctx, AccAng_usr.data_raw_acceleration);
      AccAng_usr.acceleration_mg[0] =
        lsm6ds3_from_fs2g_to_mg(AccAng_usr.data_raw_acceleration[0]);
      AccAng_usr.acceleration_mg[1] =
        lsm6ds3_from_fs2g_to_mg(AccAng_usr.data_raw_acceleration[1]);
      AccAng_usr.acceleration_mg[2] =
        lsm6ds3_from_fs2g_to_mg(AccAng_usr.data_raw_acceleration[2]);
     /* sprintf((char *)tx_buffer,
              "Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n",
              acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
      tx_com( tx_buffer, strlen( (char const *)tx_buffer ) );*/
    }

    lsm6ds3_gy_flag_data_ready_get(&dev_ctx, &reg);

    if (reg) {
      /* Read angular rate field data */
      memset(AccAng_usr.data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
      lsm6ds3_angular_rate_raw_get(&dev_ctx, AccAng_usr.data_raw_angular_rate);
      AccAng_usr.angular_rate_mdps[0] =
        lsm6ds3_from_fs2000dps_to_mdps(AccAng_usr.data_raw_angular_rate[0]);
      AccAng_usr.angular_rate_mdps[1] =
        lsm6ds3_from_fs2000dps_to_mdps(AccAng_usr.data_raw_angular_rate[1]);
      AccAng_usr.angular_rate_mdps[2] =
        lsm6ds3_from_fs2000dps_to_mdps(AccAng_usr.data_raw_angular_rate[2]);
     /* sprintf((char *)tx_buffer,
              "Angular rate [mdps]:%4.2f\t%4.2f\t%4.2f\r\n",
              angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
      tx_com( tx_buffer, strlen( (char const *)tx_buffer ) );*/
    }

    lsm6ds3_temp_flag_data_ready_get(&dev_ctx, &reg);

    if (reg) {
      /* Read temperature data */
      memset(&AccAng_usr.data_raw_temperature, 0x00, sizeof(int16_t));
      lsm6ds3_temperature_raw_get(&dev_ctx, &AccAng_usr.data_raw_temperature);
      AccAng_usr.temperature_degC =
        lsm6ds3_from_lsb_to_celsius(AccAng_usr.data_raw_temperature);
     // sprintf((char *)tx_buffer,
      /*sprintf((char *)tx_buffer, "Temperature [degC]:%6.2f\r\n",
              temperature_degC );
      tx_com( tx_buffer, strlen( (char const *)tx_buffer ) );*/
    }
  }
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t platform_write2(void *handle, uint8_t reg,
                              uint8_t *bufp,
                              uint16_t len)
{
  HAL_I2C_Mem_Write(handle, LSM6DS3_I2C_ADD_L, reg,
                    I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);

  return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t platform_read2(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  HAL_I2C_Mem_Read(handle, LSM6DS3_I2C_ADD_L, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  len           number of byte to send
 *
 */
static void tx_com(uint8_t *tx_buffer, uint16_t len)
{

}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */


