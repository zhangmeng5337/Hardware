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
#include "bsp.h"
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
*	?    ?? : ??gpio???i2c????, ??????STM32F4???CPU??????��??????��??????????????I2C?????????????????
*
*	????? :
*		?��??  ????        ????     ???
*		V1.0    2016-04-027 Charls  ???????
*
*	Copyright (C), 2016, QST
*
*********************************************************************************************************
*/

/*
	????????
	?????I2C?��?????????? i2c_CheckDevice() ???I2C?��??????????��?????????GPIO
*/

#include "main.h"
//#include "stm32f0xx_hal.h"

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

/* ????I2C?????????GPIO???, ??????????????4?��???????????SCL??SDA?????? */





/*
*********************************************************************************************************
*	?? ?? ??: i2c_Delay
*	???????: I2C????��???????400KHz
*	??    ??:  ??
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void i2c_Delay(uint32_t count)
{
//	uint8_t i;

	/*??
		CPU???168MHz????????Flash????, MDK???????????????????????��?
		????????5???SCL??? = 1.78MHz (?????: 92ms, ??��?????????????????????????��????????????)
		????????10???SCL??? = 1.1MHz (?????: 138ms, ?????: 118724B/s)
		????????30???SCL??? = 440KHz?? SCL???????1.0us??SCL???????1.2us

		???????????2.2K????SCL??????????0.5us??????4.7K????????????1us

		?????????400KHz????????????
	*/
	while (count--);
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

			I2C1_SDA_OUT();
			I2C1_SDA_1();
			I2C1_SCL_1();
			
			i2c_Delay(5);
			I2C1_SDA_0();
			
			i2c_Delay(5);
			I2C1_SCL_0();
}

/*
*********************************************************************************************************
*	?? ?? ??: i2c_Start
*	???????: CPU????I2C?????????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void StopI2C1()
{
			I2C1_SDA_OUT();
			I2C1_SDA_0();
			I2C1_SCL_0();
			
			i2c_Delay(5);
			I2C1_SDA_1();
			I2C1_SCL_1();
			
			i2c_Delay(5);

}
/*
*********************************************************************************************************
*	?? ?? ??: i2c_WaitAck
*	???????: CPU??????????????????????ACK??????
*	??    ??:  TermNumber:I2C?????
*	?? ?? ?: ????0?????????1????????????
*********************************************************************************************************
*/
uint8_t I2CAcknowledgeByte()
{
	uint8_t ucErrTime;
	        I2C1_SDA_IN();
			I2C1_SDA_1();	/* CPU???SDA???? */
			I2C1_SCL_0();
			i2c_Delay(5);
			I2C1_SCL_1();
			//I2C1_SCL_1();	/* CPU????SCL = 1, ???????????ACK??? */
			//i2c_Delay();
			
			while(I2C1_SDA_READ())
			{
				ucErrTime++;
				if(ucErrTime>50)
				{
					StopI2C1();
					return 1;
				}
				i2c_Delay(5);
			}  
			I2C1_SCL_1();
			i2c_Delay(5); 
			I2C1_SCL_0();//ʱ�����0	
			
	return 0;
}

void IIC_Ack(void)
{
	I2C1_SCL_0();
	I2C1_SDA_OUT();
	I2C1_SDA_0();
		i2c_Delay(5);
	I2C1_SCL_1();
		i2c_Delay(5);
	I2C1_SCL_0();
}
void IIC_NAck(void)
{
	I2C1_SCL_0();
	I2C1_SDA_OUT();
	I2C1_SDA_1();


	i2c_Delay(5);
	I2C1_SCL_1();
	i2c_Delay(5);
	I2C1_SCL_0();

}


void MasterWriteI2C1(uint8_t _ucByte)
{
	    uint8_t i;
			I2C1_SDA_OUT();
			I2C1_SCL_0();
		/* ??????????��bit7 */
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
			i2c_Delay(2);
			I2C1_SCL_1();
			i2c_Delay(5);
			I2C1_SCL_0();
			i2c_Delay(3);
			_ucByte <<= 1;	/* ???????bit */

			}
}


uint8_t MasterReadI2C1(unsigned char ack)
{
	uint8_t i;
	uint8_t value;
	value = 0;
			I2C1_SDA_IN();
			for (i = 0; i < 8; i++)
			{
			   I2C1_SCL_0();
			   i2c_Delay(5);
			    I2C1_SCL_1();
				value <<= 1;

				if (I2C1_SDA_READ())
				{
					value++;
				}
				i2c_Delay(5);
			}
			if(ack)
				IIC_Ack();
			else
				IIC_NAck();
				
	return value;
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
    MasterWriteI2C1((slave_addr<<1)|(0x00));
	I2CAcknowledgeByte();
    MasterWriteI2C1(reg_addr);
	I2CAcknowledgeByte();
	for(i=0;i<length;i++){
           MasterWriteI2C1(data[i]);
		   I2CAcknowledgeByte();
               
	}
        StopI2C1();								//Send the Stop condition
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
	MasterWriteI2C1((slave_addr<<1)|(0x00));
	I2CAcknowledgeByte();
	MasterWriteI2C1(reg_addr);
	I2CAcknowledgeByte();
    //StopI2C1();    

	StartI2C1();                        //Send the Start Bit
	MasterWriteI2C1((slave_addr<<1)|(0x01));
	I2CAcknowledgeByte();
	for(i=0;i<length;i++) {
		data[i] = MasterReadI2C1(1);
		if(i<(length-1)) {
		I2CAcknowledgeByte();

		}
		else {
		MasterReadI2C1(0);

		}
	}
	StopI2C1();								//Send the Stop condition

	return 0;
}





