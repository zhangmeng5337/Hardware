/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
<<<<<<< HEAD

  	DWORD time = 0;
	RTC_TimeTypeDef Time;
	RTC_DateTypeDef Date;
	extern RTC_HandleTypeDef hrtc;
	if (HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN) != HAL_OK || 
		HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN) != HAL_OK)
	{
		return 0;
	}
	time = (Date.Year - 10) << 25;							//RTC里的年基准是1970，FATFS里的年基准是1980
	time |= Date.Month << 21;
	time |= Date.Date << 16;
	time |= Time.Hours << 11;
	time |= Time.Minutes << 5;
	time |= Time.Seconds >> 1;

  return time;
=======
  return 0;
>>>>>>> 1ae077bb4952fdb979a8f9608a90611b9cba3ce6
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
