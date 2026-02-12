/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "ads1258.h"


data_ADC_stru data_ADC;

//****************************************************************************
//
// Macros
//
//****************************************************************************
/** Alias used for setting GPIOs pins to the logic "high" state */
#define HIGH                ((bool) true)

/** Alias used for setting GPIOs pins to the logic "low" state */
#define LOW                 ((bool) false)



//****************************************************************************
//
// Internal variables
//
//****************************************************************************

/* Internal register map array (to recall current configuration) */
static uint8_t registerMap[NUM_REGISTERS];



//****************************************************************************
//
// Functions
//
//****************************************************************************

/**
 * \fn uint8_t getRegisterValue(uint8_t address)
 * \brief Getter function to access the registerMap array outside of this module
 * \param address The 8-bit register address
 * \return The 8-bit register value
 */
uint8_t getRegisterValue(uint8_t address)
{
    if (address < NUM_REGISTERS)
        return registerMap[address];
}



/**
 * \fn void adcStartupRoutine(void)
 * \brief Startup function to be called before communicating with the ADC
 */
//void adcStartupRoutine(void)
//{
//    /* (OPTIONAL) Provide additional delay time for power supply settling */
//    delay_ms(50);

//    /* (REQUIRED) Set nRESET/nPWDN pin high for ADC operation */
//    setPWDN(HIGH);

//    /* (OPTIONAL) Start ADC conversions with HW pin control  */
//    setSTART(HIGH);

//    /* (REQUIRED) tWAKE delay */
//    delay_ms(5);

//    /* (OPTIONAL) Toggle nRESET pin to assure default register settings. */
//    /* NOTE: This also ensures that the device registers are unlocked.   */
//   // toggleRESET();

//    /* Ensure internal register array is initialized */
//    restoreRegisterDefaults();

//    /* (OPTIONAL) Configure initial device register settings here */
//    uint8_t initRegisterMap[NUM_REGISTERS];
//    initRegisterMap[REG_ADDR_CONFIG0]   =   CONFIG0_DEFAULT;
//    initRegisterMap[REG_ADDR_CONFIG1]   =   CONFIG1_DLY_64us | CONFIG1_DRATE_7813SPS;
//    initRegisterMap[REG_ADDR_MUXSCH]    =   MUXSCH_DEFAULT;
//    initRegisterMap[REG_ADDR_MUXDIF]    =   MUXDIF_DEFAULT;
//    initRegisterMap[REG_ADDR_MUXSG0]    =   MUXSG0_DEFAULT;
//    initRegisterMap[REG_ADDR_MUXSG1]    =   MUXSG1_DEFAULT;
//    initRegisterMap[REG_ADDR_SYSRED]    =   SYSRED_DEFAULT;
//    initRegisterMap[REG_ADDR_GPIOC]     =   GPIOC_DEFAULT;
//    initRegisterMap[REG_ADDR_GPIOD]     =   GPIOD_DEFAULT;
//    initRegisterMap[REG_ADDR_ID]        =   0x00;           // Read-only register

//    /* (OPTIONAL) Write to all (writable) registers */
//    writeMultipleRegisters(REG_ADDR_CONFIG0, NUM_REGISTERS - 1, initRegisterMap);

//    /* (OPTIONAL) Read back all registers */
//    readMultipleRegisters(REG_ADDR_CONFIG0, NUM_REGISTERS);

//    /* (OPTIONAL) Start ADC conversions with the SPI command.
//     * Not needed if the START pin has already been set HIGH.
//     *
//     * sendCommand(START_OPCODE);
//     */
//}



/**
 * \fn uint8_t readSingleRegister(uint8_t addr)
 * \brief Reads contents of a single register at the specified address
 * \param addr address of the register to read
 * \return 8-bit register read result
 */
uint8_t readSingleRegister(uint8_t address)
{
    /* Check that the register address is in range */
    //assert(address < NUM_REGISTERS);

    /* Initialize arrays */
    uint8_t DataTx[2] = { 0 };
    uint8_t DataRx[2] = { 0 };

    /* Build TX array and send it */
    DataTx[0] = OPCODE_RREG | (address & OPCODE_A_MASK);
    spiSendReceiveArrays(1, DataTx, DataRx, 2);

    /* Update register array and return read result*/
    registerMap[address] = DataRx[1];
    return DataRx[1];
}



/**
 * \fn void readMultipleRegisters(uint8_t startAddress, uint8_t count)
 * \brief Reads a group of registers starting at the specified address
 * \param startAddress register address from which we start reading
 * \param count number of registers we want to read
 * NOTE: Use getRegisterValue() to retrieve the read values
 */
void readMultipleRegisters(uint8_t startAddress, uint8_t count)
{
    /* Check that the register address and count are in range */
    // assert(startAddress + count <= NUM_REGISTERS);


    //
    // SPI communication
    //

    setCS(1, LOW);

    uint8_t dataTx = OPCODE_RREG | OPCODE_MUL_MASK | (startAddress & OPCODE_A_MASK);
    spiSendReceiveByte(dataTx);

    uint8_t i;
    for (i = startAddress; i < startAddress + count; i++)
    {
        // Read register data bytes
        registerMap[i] = spiSendReceiveByte(0x00);
    }

    setCS(1, HIGH);
}



/**
 * \fn void writeSingleRegister(uint8_t address, uint8_t data)
 * \brief Write data to a single register at the specified address
 * \param address The address of the register to write
 * \param data The 8-bit data to write to the register
 */
void writeSingleRegister(uint8_t address, uint8_t data)
{
    /* Check that the register address is in range */
    // assert(address < NUM_REGISTERS);

    /* Initialize arrays */
    uint8_t DataTx[2];
    uint8_t DataRx[2] = { 0 };

    /* Build TX array and send it */
    DataTx[0] = (OPCODE_WREG | (address & OPCODE_A_MASK));
    DataTx[1] = data;
    spiSendReceiveArrays(1, DataTx, DataRx, 2);

    /* Update register array */
    registerMap[address] = DataTx[1];
}



/**
 * \fn void writeMultipleRegisters(uint8_t startAddress, uint8_t count, const uint8_t regData[])
 * \brief Writes data to a group of registers
 * \param startAddress register address from which we start write
 * \param count number of registers we want to write to
 * \param regData Array that holds the data to write, where element zero is the data to write to the starting address.
 * NOTES:
 * - Use getRegisterValue() to retrieve the written values.
 * - Registers should be re-read after a write operaiton to ensure proper configuration.
 */
void writeMultipleRegisters(unsigned char adc_No, uint8_t startAddress, uint8_t count, const uint8_t regData[])
{
    /* Check that the register address and count are in range */
    //  assert(startAddress + count <= NUM_REGISTERS);

    /* Check that regData is not a NULL pointer */
    // assert(regData);


    //
    // SPI communication
    //

    setCS(adc_No, LOW);

    uint8_t dataTx = OPCODE_WREG | OPCODE_MUL_MASK | (startAddress & OPCODE_A_MASK);
    spiSendReceiveByte(dataTx);

    uint8_t i;
    for (i = startAddress; i < startAddress + count; i++)
    {
        // write register data bytes
        spiSendReceiveByte(regData[i]);

        /* Update register array */
        registerMap[i] = regData[i];
    }

    setCS(adc_No, HIGH);

    /*setCS(adc_No,LOW);
    spiSendReceiveByte(dataTx);

    for (i = startAddress; i < startAddress + count; i++)
    {
        // write register data bytes
        spiSendReceiveByte(regData[i]);

   /* eUpdate register array */
    registerMap[i] = regData[i];
    //}

    /*setCS(adc_No,HIGH);*/


}



/**
 * \fn void sendCommand(uint8_t op_code)
 * \brief Sends the specified SPI command to the ADC
 * \param op_code SPI command byte
 */
void sendCommand(uint8_t op_code)
{
    /* Assert if this function is used to send any of the following commands */
    // assert(OPCODE_RREG         != op_code);    /* Use "readSingleRegister()"  or "readMultipleRegisters()"  */
    //assert(OPCODE_WREG         != op_code);    /* Use "writeSingleRegister()" or "writeMultipleRegisters()" */
    //assert(OPCODE_READ_DIRECT  != op_code);    /* Use "readData()" */
    // assert(OPCODE_READ_COMMAND != op_code);    /* Use "readData()" */

    /* SPI communication */
    setCS(1, LOW);
    spiSendReceiveByte(op_code);
    setCS(1, HIGH);

    // Check for RESET command
    if (OPCODE_RESET == op_code)
    {
        /* Update register array to keep software in sync with device */
        restoreRegisterDefaults();
    }
}



/**
 * \fn void startConversions()
 * \brief Wakes the device from power-down and starts continuous conversions
 */
void startConversions(void)
{
    /* Ensure device is not in PWDN mode */
    setPWDN(HIGH);

    /* Begin continuous conversions */
    setSTART(HIGH);
}



/* Retrieves the ADC conversion result
 * NOTE: Call this function after /DRDY goes low and specify the
 * the number of bytes to read and the starting position of data
 * (based on whe)
 *
 * This function implements the read command
*/

/**
 * \fn int32_t readData(uint8_t status[], uint8_t data[], readMode mode)
 * \brief Sends the read command and retrieves STATUS (if enabled) and data
 * \param status[] pointer to address where STATUS byte will be stored
 * \param data[] pointer to starting address where data bytes will be stored
 * \param mode readMode typedef to select which read operation to use
 * \return 32-bit sign-extended conversion result (data only)
 */
unsigned  char channel;
void readData(unsigned char adc_No, readMode mode)
{
	   uint32_t tmp;
    uint8_t DataTx[5] = { 0 };    // Initialize all array elements to 0
    uint8_t DataRx[5] = { 0 };    // Relies on C99 [$6.7.8/10], [$6.7.8/21]
    uint8_t byteLength;
    uint8_t dataPosition;
    bool    status_byte_enabled;
    
    // unsigned char tmp, tmp1;
    /* Build TX array and send it */
    if (mode == DIRECT)
    {
        DataTx[0] = OPCODE_READ_DIRECT;

        /* STATUS byte enable depends on MUXMOD and STAT (see table 17 in datasheet) */
        status_byte_enabled = IS_STAT_SET & !IS_MUXMOD_SET;
        byteLength = (status_byte_enabled ? 3 : 2);
        dataPosition = (status_byte_enabled ? 1 : 0);
    }
    else if (mode == COMMAND)
    {
        DataTx[0] = OPCODE_READ_COMMAND | OPCODE_MUL_MASK;

        /* STATUS byte always enabled, but undefined in fixed-channel mode (see table 17 in datasheet) */
        status_byte_enabled = true;
        byteLength = 4;
        dataPosition = 2;
    }
    spiSendReceiveArrays(adc_No, DataTx, DataRx, byteLength);
    //	HAL_Delay(1);

    /* Check for "data" pointer before making assignments */
    //  if (data!=NULL)
    {
        //data[0] = DataRx[dataPosition + 0];
        // data[1] = DataRx[dataPosition + 1];
        // data[2] = DataRx[dataPosition + 2];
    }
    if (adc_No == 1) //ads1158 1采集   ai
    {
        data_ADC.status_byte = DataRx[dataPosition - 1] & 0x80; //判断数据是否更新
        if (data_ADC.status_byte)
        {

            data_ADC.ch_id = DataRx[dataPosition - 1] & 0x7f; //判断数据是否是有效
            if (data_ADC.ch_id < 0x1e)
            {
                if (data_ADC.ch_id < 26)
                    channel = data_ADC.ch_id - 0x08;
                else
                    channel = data_ADC.ch_id - 0x09; //通道25没有
																		
										tmp = DataRx[dataPosition];
										tmp = tmp<<8;
                    data_ADC.data_2byte[channel] = tmp;
										tmp = DataRx[dataPosition+1];
										tmp = tmp |data_ADC.data_2byte[channel];
										data_ADC.data_2byte[channel] =  tmp;
               /* data_ADC.data_2byte[channel] = DataRx[dataPosition];
                data_ADC.data_2byte[channel] = data_ADC.data_2byte[channel] << 8;
                data_ADC.data_2byte[channel] = data_ADC.data_2byte[channel] | DataRx[dataPosition + 1];*/
               // memcpy(data_ADC.data_inADC + 2 * channel, &DataRx[dataPosition], byteLength - 1);

            }

        }

    }
    if (adc_No == 2) //ads1158 2采集
    {
        if (DataRx[dataPosition - 1] & 0x80)
        {
            data_ADC.status_byte = DataRx[dataPosition - 1] & 0x80;
            if (data_ADC.status_byte)
            {
                data_ADC.ch_id = DataRx[dataPosition - 1] & 0x7f;
                if (data_ADC.ch_id < 0x1e)
                {
                    if (data_ADC.ch_id < 26)
                        channel = data_ADC.ch_id - 0x08;
                    else
                        channel = data_ADC.ch_id - 0x09;
										//uint32_t tmp;
										tmp = DataRx[dataPosition];
										tmp = tmp<<8;
                    data_ADC.data2_2byte[channel] = tmp;
										tmp = DataRx[dataPosition+1];
										tmp = tmp |data_ADC.data2_2byte[channel];
										data_ADC.data2_2byte[channel] =  tmp;
                    //data_ADC.data2_2byte[channel] = data_ADC.data_2byte[channel] << 8;
                   // data_ADC.data2_2byte[channel] = data_ADC.data_2byte[channel] | DataRx[dataPosition +1];
                   // memcpy(data_ADC.data2_inADC + 2 * channel, &DataRx[dataPosition], byteLength - 1);
                    if (channel == (CHANNEL_SIZE - 1))
                        data_ADC.update = 1;//数据采集完成

                }

            }

        }


        /*uint32_t upperByte 	= ((uint32_t) (DataRx[dataPosition + 0]) & 0xFF) << 8;
        uint32_t middleByte  = ((uint32_t)(DataRx[dataPosition + 1])  & 0xFF) ;
        uint32_t lowerByte	= ((int32_t) DataRx[dataPosition -1] & 0xFF) << 16;

        return ( upperByte | middleByte|lowerByte);*/
    }
}
void dat_adc_proc()
{
    unsigned char i;
    for (i = 0; i < CHANNEL_SIZE; i++)
    {

        // Not required, but added to allow for debugger to halt code execution
        setSTART(HIGH);
        readData(1, DIRECT);
        // Wait and check that /DRDY interrupt occurred
        //b_pass = waitForDRDYinterrupt(1,10);

        //  Read data in read direct mode
        //if(b_pass)
        //dataValues[i] = readData(&statusBytes[i], NULL, DIRECT);

        // Not required, but added to allow for debugger to halt code execution
        setSTART(LOW);
    }

}

data_ADC_stru *get_ADC_data()
{
    return &data_ADC;
}



/**
 * \fn void restoreRegisterDefaults(void)
 * \brief Updates the registerMap[] array to its default values.
 *
 * NOTES:
 * - If the MCU keeps a copy of the ADC register settings in memory,
 * then it is important to ensure that these values remain in sync with the
 * actual hardware settings. In order to help facilitate this, this function
 * should be called after powering up or resetting the device (either by
 * hardware pin control or SPI software command).
 *
 * - Reading back all of the registers after resetting the device will
 * accomplish the same result.
 */
void restoreRegisterDefaults(void)
{
    registerMap[REG_ADDR_CONFIG0]   =   CONFIG0_DEFAULT;
    registerMap[REG_ADDR_CONFIG1]   =   CONFIG1_DEFAULT;
    registerMap[REG_ADDR_MUXSCH]    =   MUXSCH_DEFAULT;
    registerMap[REG_ADDR_MUXDIF]    =   MUXDIF_DEFAULT;
    registerMap[REG_ADDR_MUXSG0]    =   MUXSG0_DEFAULT;
    registerMap[REG_ADDR_MUXSG1]    =   MUXSG1_DEFAULT;
    registerMap[REG_ADDR_SYSRED]    =   SYSRED_DEFAULT;
    registerMap[REG_ADDR_GPIOC]     =   GPIOC_DEFAULT;
    registerMap[REG_ADDR_GPIOD]     =   GPIOD_DEFAULT;
    registerMap[REG_ADDR_ID]        =
        0x00;               // Value of 0x00 indicates that we have not yet read the ID register
}
