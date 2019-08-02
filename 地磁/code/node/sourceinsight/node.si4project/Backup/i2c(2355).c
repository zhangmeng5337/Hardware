/**
 *  Driver developed in HERMES project
 */
/**
 *  @addtogroup  I2C
 *  @brief       My I2C lib.
 *  @{
 *      @file       i2c.c
 *      @brief      An I2C lib for i2c operations.
 *      @details    Change History:
 *                  ************************************************************************
 *                  | VERSION      | DATE	    | AUTHORS	  | DESCRIPTION            |
 *                  |:------------:|:--------------:|:------------|:-----------------------|
 *                  | 1.0          |   25/8/2014    | MR          | First Release          |
 *                  \n\n
 */
#include "i2c.h"

//#include "hardware.h"
//#include <peripheral/i2c.h>

/**
 *  @brief  Initialize i2c module:
 *  I2C clock is BRG
 *  If mode parameter is SLAVE, uses address to set slave address for the module
 *  Enable module
 *  @param[in]  I2C_MODULE i2cnum (use I2C1 otherwise is necessary to modify the functions)
 *  @param[in]  i2cmode mode (MASTER or SLAVE)
 *  @param[in]  BYTE address for SLAVE mode
 *  @return     none
 */
 
 
 
 
 
 
/*
*********************************************************************************************************
*
*	??????? : I2C???????????
*	??????? : bsp_i2c_gpio.c
*	??    ?? : V1.0
*	?    ?? : ??gpio???i2c????, ??????STM32F4???CPU??????φφ??????¨°??????????????I2C?????????????????
*
*	????? :
*		?·Ϊ??  ????        ????     ???
*		V1.0    2016-04-027 Charls  ???????
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	????????
	?????I2C?υτ?????????? i2c_CheckDevice() ???I2C?υτ??????????¨²?????????GPIO
*/

#include "main.h"
#include "stm32f0xx_hal.h"
  GPIO_InitTypeDef GPIO_InitStruct;
/*
	QST ?????? i2c????GPIO:
	??????I2C?????
 		|--PB8/I2C1_SCL
 		|--PB9/I2C1_SDA
		
	???I2C?????
		|--PB10/I2C2_SCL
		|--PB11/I2C2_SDA
		
	???????I2C????
		|--PA8/I2C3_SCL
		|--PC9/I2C3_SDA
*/

/* ????I2C?????????GPIO???, ??????????????4?§Υ???????????SCL??SDA?????? */




/* ?????§ΥSCL??SDA??? */
#define I2C1_SCL_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C1_SCL_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C1_SDA_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);				/* SDA = 1 */
#define I2C1_SDA_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);				/* SDA = 0 */

#define I2C1_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)	/* ??SDA?????? */
#define I2C1_SCL_READ()  HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)		/* ??SCL?????? */


//#define I2C1_SCL_IN() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=0<<8*2;}   /* PB8???¨®????? */
//#define I2C1_SCL_OUT() 					{GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=1<<8*2;}		/* PB8???¨®???? */

void I2C1_SCL_IN() 					
	{  GPIO_InitStruct.Pin = SCL_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9???¨®????? */
void I2C1_SCL_OUT() 				
	{  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}		/* PB9???¨®???? */
void I2C1_SDA_IN() 					
	{  GPIO_InitStruct.Pin = SDA_Pin;
																		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
																		GPIO_InitStruct.Pull = GPIO_NOPULL;
																			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
															      HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);}   /* PB9???¨®????? */
void I2C1_SDA_OUT() 				
	{  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
	}		/* PB9???¨®???? */

/*
*********************************************************************************************************
*	?? ?? ??: bsp_InitI2C
*	???????: ????I2C?????GPIO?????????IO???????
*	??    ??:  ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{


//GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

 

  /*Configure GPIO pin : SCL_Pin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);



	
	
	

	/* ??????????, ??¦ΛI2C????????????υτ???????? */
	i2c_Stop(1);
	i2c_Stop(2);
	i2c_Stop(3);
	
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_Delay
*	???????: I2C????¦Λ???????400KHz
*	??    ??:  ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*??
		CPU???168MHz????????Flash????, MDK???????????????????????¦±?
		????????5???SCL??? = 1.78MHz (?????: 92ms, ??§Υ?????????????????????????§Υ????????????)
		????????10???SCL??? = 1.1MHz (?????: 138ms, ?????: 118724B/s)
		????????30???SCL??? = 440KHz?? SCL???????1.0us??SCL???????1.2us

		???????????2.2K????SCL??????????0.5us??????4.7K????????????1us

		?????????400KHz????????????
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_Start
*	???????: CPU????I2C??????????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void StartI2C1()
{
	/* ??SCL???????SDA???????????????I2C?????????? */
	//switch(_TermNumber)
	{
	//	case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_1();
			I2C1_SCL_1();
			i2c_Delay();
			I2C1_SDA_0();
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
	//		break;
		
		//default:
		//	printf("No Such Terminal,Please Check Again,Start Err\r\n");
		//	break;	
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_Start
*	???????: CPU????I2C?????????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void i2c_Stop(uint8_t _TermNumber)
{
	/* ??SCL???????SDA???????????????I2C????????? */
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_0();
			I2C1_SCL_1();
			i2c_Delay();
			I2C1_SDA_1();
			break;
		default:
			printf("No Such Terminal,Please Check Again,Stop Err\r\n");
			break;
			
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_SendByte
*	???????: CPU??I2C?????υτ????8bit????
*	??    ??:  TermNumber:I2C????? _ucByte ?? ???????????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void MasterWriteI2C1(uint8_t _ucByte)
{
	uint8_t i;
	//switch( _TermNumber )
	{
	//	case 1:
			I2C1_SDA_OUT();
		/* ??????????¦Λbit7 */
			for (i = 0; i < 8; i++)
			{
				if (_ucByte & 0x80)
				{
					I2C1_SDA_1();
				}
				else
				{
				I2C1_SDA_0();
				}
				i2c_Delay();
				I2C1_SCL_1();
				i2c_Delay();
				I2C1_SCL_0();
				if (i == 7)
				{
					I2C1_SDA_1(); // ???????
				}
				_ucByte <<= 1;	/* ???????bit */
				i2c_Delay();
			}
		//break;
		
		
	//	default:
		//	printf("No Such Terminal,Please Check Again\r\n");
		//	break;
			
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_ReadByte
*	???????: CPU??I2C?????υτ???8bit????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??????????
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(uint8_t _TermNumber)
{
	uint8_t i;
	uint8_t value;

	/* ??????1??bit??????bit7 */
	value = 0;
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_IN();
			for (i = 0; i < 8; i++)
			{
				value <<= 1;
				I2C1_SCL_1();
				i2c_Delay();
				if (I2C1_SDA_READ())
				{
					value++;
				}
				I2C1_SCL_0();
				i2c_Delay();
			}
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,ReadByte Err\r\n");
			break;		
	}
	return value;
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_WaitAck
*	???????: CPU??????????????????????ACK??????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ????0?????????1????????????
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(uint8_t _TermNumber)
{
	uint8_t re;
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_1();	/* CPU???SDA???? */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU????SCL = 1, ???????????ACK??? */
			i2c_Delay();
			I2C1_SDA_IN();
			if (I2C1_SDA_READ())	/* CPU???SDA?????? */
			{
				re = 1;
			}
			else
			{
				re = 0;
			}
			I2C1_SCL_0();
			i2c_Delay();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,WaitACK Err\r\n");
			break;
	}
	return re;
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_Ack
*	???????: CPU???????ACK???
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void i2c_Ack(uint8_t _TermNumber)
{
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_0();	/* CPU????SDA = 0 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU????1????? */
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
			I2C1_SDA_1();	/* CPU???SDA???? */
		break;
		default:
			printf("No Such Terminal,Please Check Again,ACK Err\r\n");
			break;
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_NAck
*	???????: CPU????1??NACK???
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void i2c_NAck(uint8_t _TermNumber)
{
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_OUT();
			I2C1_SDA_1();	/* CPU????SDA = 1 */
			i2c_Delay();
			I2C1_SCL_1();	/* CPU????1????? */
			i2c_Delay();
			I2C1_SCL_0();
			i2c_Delay();
		break;
		default:
			printf("No Such Terminal,Please Check Again,NACK Err\r\n");
			break;
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_CheckDevice
*	???????: ???I2C?????υτ??CPU?????υτ???????????υτ??????§Ψ???υτ??????
*	??    ??: TermNumber:I2C????? _Address???υτ??I2C??????
*	?? ?? ?: ????? 0 ???????? ????1???¦Δ???
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _TermNumber , uint8_t _Address)
{
	uint8_t ucAck;
	
	switch(_TermNumber)
	{
		case 1:
			I2C1_SDA_IN();
			I2C1_SCL_IN();
			if (I2C1_SDA_READ() && I2C1_SCL_READ())
			{
				I2C1_SDA_OUT();
				I2C1_SCL_OUT();
				i2c_Start(1);		/* ?????????? */

				/* ?????υτ???+??§Υ????bit??0 = w?? 1 = r) bit7 ??? */
				i2c_SendByte(1 , _Address | I2C_WR);
				ucAck = i2c_WaitAck(1);	/* ????υτ??ACK??? */

				i2c_Stop(1);			/* ????????? */

				return ucAck;
			}
			I2C1_SDA_OUT();
			I2C1_SCL_OUT();
			break;
		
		default:
			printf("No Such Terminal,Please Check Again,CheckDevice Err\r\n");
			break;
		}
	return 1;	/* I2C?????? */
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
void i2c_init(BYTE address) {
//	//enabling i2c module doesnt need changing port
//	//direction/value etc, and is not a pin muxed peripheral
//	I2CConfigure ( i2cnum, I2C_ENABLE_SLAVE_CLOCK_STRETCHING);
//	I2CSetFrequency ( i2cnum, GetPeripheralClock(), BRG);

//	if(mode == SLAVE)
//	{
//		//address mask is set to 0
//		I2CSetSlaveAddress ( i2cnum, address&0x7f, 0, I2C_USE_7BIT_ADDRESS );
//	}

//	I2CEnable(i2cnum, TRUE);
}

/**
 *  @brief  Write to device using generic i2c protocol
 *  @param[in]  slave_addr - slave address
 *  @param[in]  reg_addr   - register address
 *  @param[in]  length     - number of bytes to write
 *  @param[in]  *data      - pointer for data to write
 *  @return     0 if sucessfull, 1 otherwise
 */
int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data) {

	BYTE i;

	StartI2C1();								//Send the Start Bit
	IdleI2C1();									//Wait to complete
        if (MasterWriteI2C1(((slave_addr<<1)|(0x00))))
            return 1;
	IdleI2C1();
        if (MasterWriteI2C1(reg_addr))
            return 1;
	IdleI2C1();

	for(i=0;i<length;i++){
            if (MasterWriteI2C1(data[i]))
                return 1;
	}
        StopI2C1();								//Send the Stop condition
        IdleI2C1();								//Wait to complete

	return 0;
}

/**
 *  @brief  Write to device using generic i2c protocol
 *  @param[in]  slave_addr - slave address
 *  @param[in]  reg_addr   - register address
 *  @param[in]  length     - number of bytes to read
 *  @param[in]  *data      - pointer to where register data is to be transfered
 *  @return     0 if sucessfull, 1 otherwise
 */
int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data) {

	BYTE i=2;

	StartI2C1();								//Send the Start Bit
	IdleI2C1();
        if (MasterWriteI2C1(((slave_addr<<1)|(0x00))))
            return 1;
	IdleI2C1();
        if (MasterWriteI2C1(reg_addr))
            return 1;
	IdleI2C1();
        StartI2C1();                        //Send the Start Bit
        IdleI2C1();                         //Wait to complete
        if (MasterWriteI2C1(((slave_addr<<1)|(0x01))))
            return 1;
	IdleI2C1();
	I2CReceiverEnable ( I2C1, TRUE);

	for(i=0;i<length;i++) {
            data[i] = MasterReadI2C1();
		if(i<(length-1)) {
                    I2CAcknowledgeByte(MPU_I2C, TRUE);
                    IdleI2C1();
		}
		else {
                    I2CAcknowledgeByte(MPU_I2C, FALSE);
                    IdleI2C1();
		}
	}
        StopI2C1();								//Send the Stop condition
        IdleI2C1();								//Wait to complete

	return 0;
}