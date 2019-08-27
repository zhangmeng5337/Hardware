#ifndef __MAG3D3100_H
#define __MAG3D3100_H

#include "main.h"
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
#define M3D_3100_POLL 	0			//0X00
#define M3D_3100_CMM		1			//0X00
#define M3D_3100_CCX		4			//0X00C8
#define M3D_3100_CCY		6			//0X00C8
#define M3D_3100_CCZ		8			//0X00C8
#define M3D_3100_TMRC	0x0B		//0X96
#define M3D_3100_MX		0x24
#define M3D_3100_MY		0x27
#define M3D_3100_MZ		0x2A
#define M3D_3100_BIST	0x33
#define M3D_3100_STATUS	0x34
#define M3D_3100_HSHAKE	0x35		//0X1B
#define M3D_3100_REVID  0x36


/// Configurations for continuous measurement mode
#define CMM_OFF                       0x00
#define CM_START                      0x01
#define ALARM_BIT                     0x02
#define DRDY_WHEN_ALARM_AND_ALL_AXIS  0x00
#define DRDY_WHEN_ANY_AXIS_MEASURED   0x04
#define DRDY_WHEN_ALL_AXIS_MEASURED   0x08
#define DRDY_WHEN_ALARM               0x0C
#define CMM_ALL_AXIS_ON               0x70
#define CMM_X_AXIS                    0x10
#define CMM_Y_AXIS                    0x20
#define CMM_Z_AXIS                    0x40
#define LDM_BIT                       0x80

/// Possible data rate choices
#define CMM_UPDATERATE_600            0x92
#define CMM_UPDATERATE_300            0x93
#define CMM_UPDATERATE_150            0x94
#define CMM_UPDATERATE_75             0x95
#define CMM_UPDATERATE_37             0x96
#define CMM_UPDATERATE_18             0x97
#define CMM_UPDATERATE_9              0x98
#define CMM_UPDATERATE_4_5            0x99
#define CMM_UPDATERATE_2_3            0x9A
#define CMM_UPDATERATE_1_2            0x9B
#define CMM_UPDATERATE_0_6            0x9C
#define CMM_UPDATERATE_0_3            0x9D
#define CMM_UPDATERATE_0_15           0x9E
#define CMM_UPDATERATE_0_075          0x9F

#define M3D_DRDY  	HAL_GPIO_ReadPin(RM_DRY_GPIO_Port, RM_DRY_Pin)
//#define CS_L			GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define CS_H			GPIO_SetBits(GPIOB, GPIO_Pin_12)

/// Configurations for Self test
#define STE_ON    0x80
#define STE_OFF   0x00
#define BW_00    0x00
#define BW_01    0x04
#define BW_10    0x08
#define BW_11    0x0C
#define BP_00    0x00
#define BP_01    0x01
#define BP_10    0x02
#define BP_11    0x03

typedef struct {
  s32 MAG_X;
  s32 MAG_Y;
  s32 MAG_Z;
} MagData_t;

#define SM_ALL_AXIS    0x70 /** Single measument mode */
#define STATUS_MASK    0x80 /** To get status of data ready */
#define BIST_MASK      0x70 /** To get status of the Ev Board */

void SPI2_Init(void);
void ThreeD3100_magic_init(void);
void ThreeD3100_magic_GetData(MagData_t* buff);
u8 SPI_Write_M3D3100(SPI_TypeDef* SPIx,u8 reg_addr,u8 *datain,u8 lenth);
u8 SPI_Read_M3D3100(SPI_TypeDef* SPIx,u8 reg_addr,u8 *dataout,u8 lenth);
BOOL requestSingleMeasurement ( void );

#endif
