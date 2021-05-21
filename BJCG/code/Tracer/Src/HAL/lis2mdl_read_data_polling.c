/*
 ******************************************************************************
 * @file    read_data_simple.c
 * @author  Sensors Software Solution Team
 * @brief   This file show the simplest way to get data from sensor.
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
 * - STEVAL_MKI109V3 + STEVAL-MKI181V1
 * - NUCLEO_F411RE + X_NUCLEO_IKS01A3
 * - DISCOVERY_SPC584B + STEVAL-MKI181V1
 *
 * and STM32CubeMX tool with STM32CubeF4 MCU Package
 *
 * Used interfaces:
 *
 * STEVAL_MKI109V3    - Host side:   USB (Virtual COM)
 *                    - Sensor side: SPI(Default) / I2C(supported)
 *
 * NUCLEO_STM32F411RE - Host side: UART(COM) to USB bridge
 *                    - I2C(Default) / SPI(supported)
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
#include <string.h>
#include <stdio.h>
#include "lis2mdl_reg.h"
#include "main.h"
#include "lis2mdl.h"

#define SENSOR_BUS hi2c1


extern I2C_HandleTypeDef hi2c1;
#define SENSOR_BUS hi2c1


/* Private macro -------------------------------------------------------------*/
#define    BOOT_TIME        20 //ms

/* Private variables ---------------------------------------------------------*/
sensors_stru sensors_usr;

static uint8_t whoamI, rst;
//static uint8_t tx_buffer[1000];

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*
 *   WARNING:
 *   Functions declare in this section are defined at the end of this file
 *   and are strictly related to the hardware platform used.
 *
 */
static int32_t platform_write(void *handle, uint8_t reg,
                              uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);

//static void platform_delay(uint32_t ms);

stmdev_ctx_t dev_ctx1;
sensors_stru *GetMagnetic(void)
{
	return &sensors_usr;
}
extern I2C_HandleTypeDef hi2c1;
/* Main Example --------------------------------------------------------------*/
unsigned char lis2mdl_init()
{
	 /* Initialize mems driver interface */
	 
	 dev_ctx1.write_reg = platform_write;
	 dev_ctx1.read_reg = platform_read;
	 dev_ctx1.handle = &hi2c1;
	 /* Initialize platform specific hardware */
	 //platform_init();
	 /* Wait sensor boot time */
	// platform_delay(BOOT_TIME);
	
	 /* Check device ID */
	 lis2mdl_device_id_get(&dev_ctx1, &whoamI);
	
	 if (whoamI != LIS2MDL_ID)
	   return 1;
	
	 /* Restore default configuration */
	 lis2mdl_reset_set(&dev_ctx1, PROPERTY_ENABLE);
	
	 do {
	   lis2mdl_reset_get(&dev_ctx1, &rst);
	 } while (rst);
	
	 /* Enable Block Data Update */
	 lis2mdl_block_data_update_set(&dev_ctx1, PROPERTY_ENABLE);
	 /* Set Output Data Rate */
	 lis2mdl_data_rate_set(&dev_ctx1, LIS2MDL_ODR_10Hz);
	 /* Set / Reset sensor mode */
	 lis2mdl_set_rst_mode_set(&dev_ctx1, LIS2MDL_SENS_OFF_CANC_EVERY_ODR);
	 /* Enable temperature compensation */
	 lis2mdl_offset_temp_comp_set(&dev_ctx1, PROPERTY_ENABLE);
	 /* Set device in continuous mode */
	 lis2mdl_operating_mode_set(&dev_ctx1, LIS2MDL_CONTINUOUS_MODE);
	 return 0;

}
void lis2mdl_read_data_simple(void)
{


  /* Read samples in polling mode (no int) */
 // while (1)
		{
    uint8_t reg;
    /* Read output only if new value is available */
    lis2mdl_mag_data_ready_get(&dev_ctx1, &reg);

    if (reg) {
      /* Read magnetic field data */
      memset(sensors_usr.data_raw_magnetic, 0x00, 3 * sizeof(int16_t));
      lis2mdl_magnetic_raw_get(&dev_ctx1, sensors_usr.data_raw_magnetic);
      sensors_usr.magnetic_mG[0] = lis2mdl_from_lsb_to_mgauss(sensors_usr.data_raw_magnetic[0]);
      sensors_usr. magnetic_mG[1] = lis2mdl_from_lsb_to_mgauss(sensors_usr.data_raw_magnetic[1]);
      sensors_usr.magnetic_mG[2] = lis2mdl_from_lsb_to_mgauss(sensors_usr.data_raw_magnetic[2]);
    /*  sprintf((char *)tx_buffer,
              "Magnetic field [mG]:%4.2f\t%4.2f\t%4.2f\r\n",
              magnetic_mG[0], magnetic_mG[1], magnetic_mG[2]);
      tx_com(tx_buffer, strlen((char const *)tx_buffer));*/
      /* Read temperature data */
      memset(&(sensors_usr.data_raw_temperature), 0x00, sizeof(int16_t));
      lis2mdl_temperature_raw_get(&dev_ctx1, &(sensors_usr.data_raw_temperature));
      sensors_usr.temperature_degC = lis2mdl_from_lsb_to_celsius(sensors_usr.data_raw_temperature);
     // sprintf((char *)tx_buffer, "Temperature [degC]:%6.2f\r\n",
       //       temperature_degC);
     // tx_com(tx_buffer, strlen((char const *)tx_buffer));
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

static int32_t platform_write(void *handle, uint8_t reg,
                              uint8_t *bufp,
                              uint16_t len)
{
  HAL_I2C_Mem_Write(handle, LIS2MDL_I2C_ADD, reg,
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
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  HAL_I2C_Mem_Read(handle, LIS2MDL_I2C_ADD, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  return 0;
}
/*
 * @brief  Send buffer to console (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  len           number of byte to send
 *
 */

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */

