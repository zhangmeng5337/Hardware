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

#include "hal.h"
#include "sys.h"
#include "ads1258.h"


//#define SSI_PERIP (SSI3_BASE)
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

//****************************************************************************
//
// Internal variables and macros
//
//****************************************************************************
static structChan ads1158_adc1_data;
// Flag to indicate if an
static volatile bool flag_nDRDY_INTERRUPT = false;

/** Alias used for setting GPIOs pins to the logic "high" state */
//#define HIGH                ((bool) true)

/** Alias used for setting GPIOs pins to the logic "low" state */
//#define LOW                 ((bool) false)



//****************************************************************************
//
// Internal function prototypes
//
//****************************************************************************
//void InitGPIO(void);
//void InitSPI(void);



//****************************************************************************
//
// External Functions (prototypes declared in interface.h)
//
//****************************************************************************

/**
 * \fn void InitADCPeripherals(void)
 * \brief Initializes MCU peripherals for interfacing with the ADS1258
 */
void InitADCPeripherals(void)
{
    // IMPORTANT: Make sure device is powered before setting GPIOs pins to HIGH state.
    // TODO: If you have additional code to power the ADC, you may want to add it here:


    // Initialize GPIOs pins used by ADS1258
    //InitGPIO();

    // Initialize SPI peripheral used by ADS1258
    // InitSPI();
}



//****************************************************************************
//
// Timing functions
//
//****************************************************************************

/**
 * \fn void delay_ms(uint32_t delay_time_ms, uint32_t sysClock_Hz)
 * \brief Provides a timing delay with ms resolution
 * \param delay_time_ms number of ms to delay
 */
void delay_ms(uint32_t delay_time_ms)
{
    /* --- TODO: INSERT YOUR CODE HERE --- */

    HAL_Delay(delay_time_ms);
}

/**
 * \fn void delay_ns(uint32_t delay_time_us, uint32_t sysClock_Hz)
 * \brief Provides a timing delay with ns resolution
 * \param delay_time_us number of us to delay
 */
//void delay_ns(uint32_t delay_time_us)
//{
//  /* --- TODO: INSERT YOUR CODE HERE --- */

//  const uint32_t cycles_per_loop = 3;
// MAP_SysCtlDelay( delay_time_us * getSysClock() / (cycles_per_loop * 1000000u) );
//}



//****************************************************************************
//
// GPIO initialization
//
//****************************************************************************

/**
 * \fn void InitGPIO(void)
 * \brief Configures the ADS1258 GPIO pins
 */
//void InitGPIO(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    // NOTE: Not all hardware implementations may control each of these pins...

//    /* Enable the clock to the GPIO Port D and wait for it to be ready */
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)))
//    {

//    }

//    /* Configure the GPIO for 'CLKSEL' as output and set low */
//    MAP_GPIOPinTypeGPIOOutput(CLKSEL_PORT, CLKSEL_PIN);
//    MAP_GPIOPinWrite(CLKSEL_PORT, CLKSEL_PIN, 0);

//    /* Enable the clock to the GPIO Port M and wait for it to be ready */
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
//    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM)))
//    {

//    }

//    /* Configure the GPIO for 'nCS' as output and set high */
//    MAP_GPIOPinTypeGPIOOutput(nCS_PORT, nCS_PIN);
//    MAP_GPIOPinWrite(nCS_PORT, nCS_PIN, nCS_PIN);

//    /* Configure the GPIO for 'nRST_nPWDN' as output and set high */
//    MAP_GPIOPinTypeGPIOOutput(nRESET_PORT, nRESET_PIN);
//    MAP_GPIOPinWrite(nRESET_PORT, nRESET_PIN, nRESET_PIN);

//    /* Configure the GPIO for 'nRST_nPWDN' as output and set high */
//    MAP_GPIOPinTypeGPIOOutput(nPWDN_PORT, nPWDN_PIN);
//    MAP_GPIOPinWrite(nPWDN_PORT, nPWDN_PIN, nPWDN_PIN);

//    /* Enable the clock to the GPIO Port P and wait for it to be ready */
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
//    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOP)))
//    {

//    }

//    /* Configure the GPIO for 'nDRDY' as input with falling edge interrupt */
//    MAP_GPIOPinTypeGPIOInput(nDRDY_PORT, nDRDY_PIN);
//    MAP_GPIOIntTypeSet(nDRDY_PORT, nDRDY_PIN, GPIO_FALLING_EDGE);
//    MAP_GPIOIntEnable(nDRDY_PORT, nDRDY_PIN);
//    MAP_IntEnable(nDRDY_INT);

//    /* Enable the clock to the GPIO Port P and wait for it to be ready */
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
//    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ)))
//    {

//    }

//    /* Configure the GPIO for 'START' as output and set high */
//    MAP_GPIOPinTypeGPIOOutput(START_PORT, START_PIN);
//    MAP_GPIOPinWrite(START_PORT, START_PIN, START_PIN);
//}



//*****************************************************************************
//
// Interrupt handler for nDRDY GPIO
//
//*****************************************************************************

/**
 * \fn void GPIOP5_IRQHandler(void)
 * \brief Interrupt handler for /DRDY falling edge interrupt
 */
//void GPIOP5_IRQHandler(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    //TODO: Rename or register this interrupt function with your processor

//    // Possible ways to handle this interrupt:
//    //If you decide to read data here, you may want to disable other interrupts to avoid partial data reads.

//    //In this example we set a flag and exit the interrupt routine. In the main program loop, your application can example
//    // all state flags and decide which state (operation) to perform next.

//    /* Get the interrupt status from the GPIO and clear the status */
//    uint32_t getIntStatus = MAP_GPIOIntStatus(nDRDY_PORT, true);

//    if((getIntStatus & nDRDY_PIN) == nDRDY_PIN)
//    {
//        MAP_GPIOIntClear(nDRDY_PORT, getIntStatus);

//        flag_nDRDY_INTERRUPT = true;
//    }
//}


//****************************************************************************
//
// GPIO helper functions
//
//****************************************************************************

/**
 * \fn bool getCS(void)
 * \brief Returns the state of the "/CS" GPIO pin
 * \return boolean (false=low, true=high)
 */
GPIO_PinState getCS(void)
{
    /* --- TODO: INSERT YOUR CODE HERE --- */
    return HAL_GPIO_ReadPin(MIBSPI1MCS1_GPIO_Port,MIBSPI1MCS1_Pin);
}

/**
 * \fn bool getPWDN(void)
 * \brief Returns the state of the "/PWDN" GPIO pin
 * \return boolean (false=low, true=high)
 */
//bool getPWDN(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    return (bool) GPIOPinRead(nPWDN_PORT, nPWDN_PIN);
//}

/**
 * \fn bool getSTART(void)
 * \brief Returns the state of the "START" GPIO pin
 * \return boolean (false=low, true=high)
 */
GPIO_PinState getSTART(void)
{
    /* --- TODO: INSERT YOUR CODE HERE --- */
    return HAL_GPIO_ReadPin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin);
}

/**
 * \fn bool getRESET(void)
 * \brief Returns the state of the "/RESET" GPIO pin
 * \return boolean (false=low, true=high)
 */
//bool getRESET(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    return (bool) GPIOPinRead(nRESET_PORT, nRESET_PIN);
//}

/**
 * \fn void setCS(bool state)
 * \brief Sets the state of the "/CS" GPIO pin
 * \param state boolean indicating which state to set the CS pin (0=low, 1=high)
 */

void setCS(unsigned char adc_No,bool state)
{
    /* --- TODO: INSERT YOUR CODE HERE --- */
    // uint8_t value = (uint8_t) (state ? nCS_PIN : 0);
    if(adc_No==1)
    {
        ADS1158_CS2_HIGH
        if(state == 0)
            ADS1158_CS1_LOW
            else
                ADS1158_CS1_HIGH
     }
    else if(adc_No==2)
    {
        ADS1158_CS1_HIGH
        if(state == 0)
            ADS1158_CS2_LOW
        else
             ADS1158_CS2_HIGH

     }
}

/**
 * \fn void setPWDN(bool state)
 * \brief Sets the state of the "/PWDN" GPIO pin
 * \param state boolean indicating which state to set the /PWDN pin (0=low, 1=high)
 */
void setPWDN(bool state)
{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    uint8_t value = (uint8_t) (state ? nPWDN_PIN : 0);
//    MAP_GPIOPinWrite(nPWDN_PORT, nPWDN_PIN, value);

//    // Minimum nPWDN width: 2 tCLKs
//    delay_ns(125);

    // Reset register array when powering down
    if(!state)
    {
        restoreRegisterDefaults();
    }




}

/**
 * \fn void setRESET(bool state)
 * \brief Sets the state of the "/RESET" GPIO pin
 * \param state boolean indicating which state to set the CS pin (0=low, 1=high)
 */
//void setRESET(bool state)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    uint8_t value = (uint8_t) (state ? nRESET_PIN : 0);
//    MAP_GPIOPinWrite(nRESET_PORT, nRESET_PIN, value);
//}

/**
 * \fn void setSTART(bool state)
 * \brief Sets the state of the "START" GPIO pin
 * \param state boolean indicating which state to set the START pin (0=low, 1=high)
 */

void setSTART(bool state)
{
    if(ads1158_adc1_data.chip_No == 1)
    {
        if(ads1158_adc1_data.ads1_start== 0||
                ads1158_adc1_data.ads1_update == 1)
        {
            //ads_Pulse(ads1158_adc1_data.chip_No);
            ads1158_adc1_data.ads1_start=1;//ads1158 1启动标志
            ads1158_adc1_data.ads2_start=0;
            ads1158_adc1_data.ads1_update = 0;

        }


    }
    else if(ads1158_adc1_data.chip_No == 2)
    {
        if(ads1158_adc1_data.ads2_start== 0||
                ads1158_adc1_data.ads2_update == 1)
        {
            //	ads_Pulse(ads1158_adc1_data.chip_No);
            ads1158_adc1_data.ads2_start=1;//ads1158 1启动标志
            ads1158_adc1_data.ads1_start=0;
            ads1158_adc1_data.ads2_update = 0;

        }


    }
    /* --- TODO: INSERT YOUR CODE HERE --- */
    if(state == 0)
        START_L
        else
            START_H

        }

/**
 * \fn void toggleRESET(void)
 * \brief Toggles the /RESET pin
 */
//void toggleRESET(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE --- */
//    MAP_GPIOPinWrite(nRESET_PORT, nRESET_PIN, 0);

//    // Minimum nRESET width: 2 tCLKs
//    delay_ns(125);

//    MAP_GPIOPinWrite(nRESET_PORT, nRESET_PIN, nRESET_PIN);
//}






//****************************************************************************
//
// SPI initialization
//
//****************************************************************************

/**
 * \fn void initSPI(void)
 * \brief Configures the MCU's SPI peripheral
 */
//void InitSPI(void)
//{
//    /* --- TODO: INSERT YOUR CODE HERE ---
//     * NOTE: The ADS1258 operates in SPI mode 0.
//     */

//    //
//    // Enable clocks to GPIO Port A and configure pins as SSI
//    //
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
//    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ)))
//    {
//    }

//    MAP_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
//    //MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS); // Using GPIO for nCS instead of the FSS pin.
//    MAP_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
//    MAP_GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
//    MAP_GPIOPinTypeSSI(GPIO_PORTQ_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3));

//    //
//    // Enable the clock to SSI-3 module and configure the SSI Master
//    //
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
//    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_SSI3)))
//    {
//    }

//    MAP_SSIConfigSetExpClk(SSI3_BASE, getSysClock(), SSI_FRF_MOTO_MODE_0,
//                           SSI_MODE_MASTER, (getSysClock()/24), 8);
//    MAP_SSIEnable(SSI3_BASE);

//    //
//    // Flush the Receive FIFO
//    //
//    uint32_t junk;
//    while(MAP_SSIDataGetNonBlocking(SSI3_BASE, &junk));
//}


/*
CS引脚需要单独控制，在一串spi操作前置低 在一串spi操作结束后再拉高，在这其中的spi操作不需要延时！
spi的数据首发除了常见的那两个函数外，也可以用Ads1158_TransmitReceive，尤其对一串操作！ 用这个操作比较合适 避免时序错误。
*/
void Ads1158_TransmitReceive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
//		pRxData[i] = SPI1_ReadWriteByte(pTxData[i+1]);
    HAL_SPI_TransmitReceive(&hspi3, pTxData, pRxData,Size, 2000);

}
void Ads1158_Transmit(uint8_t *pTxData, uint16_t Size)
{

    HAL_SPI_Transmit(&hspi3,pTxData,Size,1000);

}
void Ads1158_SPI_Receive(uint8_t *pData, uint16_t Size)
{

    HAL_SPI_Receive(&hspi3, pData,Size, 2000);
}
//****************************************************************************
//
// SPI helper functions
//
//****************************************************************************

/**
 * \fn void SPI_SendReceive(uint8_t *DataTx, uint8_t *DataRx, uint8_t byteLength)
 * \brief Sends SPI commands to ADC and returns a response in array format
 * \param *DataTx array of SPI data to send to DIN pin
 * \param *DataRx array of SPI data that will be received from DOUT pin
 * \param byteLength number of bytes to send/receive on the SPI
 */
void spiSendReceiveArrays(unsigned char adc_No,uint8_t DataTx[], uint8_t DataRx[], uint8_t byteLength)
{
    /*  --- TODO: INSERT YOUR CODE HERE ---
     *
     *  This function should send and receive multiple bytes over the SPI.
     *
     *  A typical SPI send/receive sequence may look like the following:
     *  1) Make sure SPI receive buffer is empty
     *  2) Set the /CS pin low (if controlled by GPIO)
     *  3) Send command bytes to SPI transmit buffer
     *  4) Wait for SPI receive interrupt
     *  5) Retrieve data from SPI receive buffer
     *  6) Set the /CS pin high (if controlled by GPIO)
     *
     */

    /* Set the nCS pin LOW */
    setCS(adc_No,LOW);
    HAL_Delay(1);
//    /* Remove any residual or old data from the receive FIFO */
//    uint32_t junk;
//    while (SSIDataGetNonBlocking(SSI3_BASE, &junk));

//    /* SSI TX & RX */
//    uint8_t i;
//    for (i = 0; i < byteLength; i++)
//    {
//        DataRx[i] = spiSendReceiveByte(DataTx[i]);
//    }
    Ads1158_TransmitReceive(DataTx, DataRx, byteLength);
    /* Set the nCS pin HIGH */
    setCS(adc_No,HIGH);
}


/**
 * \fn uint8_t spiSendReceiveByte(uint8_t dataTx)
 * \brief Sends SPI command to ADC and returns a response, one byte at a time.
 * \param dataTx data to send over SPI
 * \return Returns SPI response byte
 */
uint8_t spiSendReceiveByte(uint8_t dataTx)
{
    /*  --- TODO: INSERT YOUR CODE HERE ---
     *  This function should send and receive single bytes over the SPI.
     *  NOTE: This function does not control the /CS pin to allow for
     *  more programming flexibility.
     */

    /* Remove any residual or old data from the receive FIFO */
//    uint32_t junk;
//    while (SSIDataGetNonBlocking(SSI3_BASE, &junk));

    /* SSI TX & RX */
    uint8_t dataRx;
//    MAP_SSIDataPut(SSI3_BASE, dataTx);
//    MAP_SSIDataGet(SSI3_BASE, &dataRx);
    Ads1158_TransmitReceive(&dataTx, &dataRx, 1);
    return dataRx;
}

void ads_data_update(unsigned char ads_NO)
{


    if(ads1158_adc1_data.ads1_start==1)
    {
        if(ads_NO == 1)
            ads1158_adc1_data.ads1_update = 1;

    }
    if(ads1158_adc1_data.ads2_start==1)
    {
        if(ads_NO == 2)
            ads1158_adc1_data.ads2_update = 1;


    }

}
/**
 * \fn bool waitForDRDYinterrupt(uint32_t timeout_ms)
 * \brief Waits for a nDRDY interrupt or until a timeout condition occurs
 * \param timeout_ms number of milliseconds to allow until a timeout
 * \return Returns true if nDRDY interrupt occurred before the timeout
 */
unsigned char waitForDRDYinterrupt(uint32_t ads_No,uint32_t timeout)
{
    /* --- TODO: INSERT YOUR CODE HERE ---
     * Poll the nDRDY GPIO pin until it goes low. To avoid potential infinite
     * loops, you may also want to implement a timer interrupt to occur after
     * the specified timeout period, in case the nDRDY pin is not active.
     * Return a boolean to indicate if nDRDY went low or if a timeout occurred.
     */

    // Convert ms to # of loop iterations or use a timer
    uint32_t tick_curr = 0;   // convert to # of loop iterations
    static unsigned char result_int;
    result_int = 0;
    tick_curr = HAL_GetTick();
    // Reset interrupt flag

    // Wait for nDRDY interrupt or timeout - each iteration is about 20 ticks
    do
    {
        if(ads_No == 1)
        {
            if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0)
            {
                result_int = 1;
                ads1158_adc1_data.ads1_update=0;
                break;

            }
            else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0)
            {
                result_int = 2;
                ads1158_adc1_data.ads2_update=0;
                break;

            }
						else 
							result_int = 3;

        }
    }
    while ((HAL_GetTick()-tick_curr)<timeout);
    return result_int;
}
uint32_t sys_ID;
void ads1158_init()
{
    uint8_t cmd_reset = 0xC0;

    uint8_t cmd_readID2[2] = {0x49, 0x00}; //命令读ID寄存器
    uint8_t back_readID2[2] = {0x00, 0x00};
    ads1158_adc1_data.chip_No =1;
    ads1158_adc1_data.ads1_start=0;//ads1158 1启动标志
    ads1158_adc1_data.ads2_start=0;
    ads1158_adc1_data.ads1_update = 0;
    ads1158_adc1_data.ads2_update = 0;

    //SPI写0xc0（复位） 软件复位
    ADS1158_CS1_LOW;
    delay_us(20);
    Ads1158_Transmit(&cmd_reset, 1);
    delay_us(100);
    ADS1158_CS1_HIGH;
    delay_us(10);

    //SPI写0x49（寄存器读命令 一次访问一个寄存器 访问09h寄存器），然后读值，值应该是0x9b（判断设备ID）
    ADS1158_CS1_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(20);
    ADS1158_CS1_HIGH;
    delay_us(10);

    sys_ID = back_readID2[1];//11001101   10001011
    if (sys_ID == 0x9B)
    {
        //ok
        printf("ADS1158 ok\r\n");
    }
    else
    {
        //error
        printf("ADS1158 failed\r\n");
    }
    printf("ADS1158 id is %d\r\n",sys_ID);


    //SPI写0xc0（复位） 软件复位
    ADS1158_CS2_LOW;
    delay_us(20);
    Ads1158_Transmit(&cmd_reset, 1);
    delay_us(100);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    //SPI写0x49（寄存器读命令 一次访问一个寄存器 访问09h寄存器），然后读值，值应该是0x9b（判断设备ID）
    ADS1158_CS2_LOW;
    delay_us(20);
    Ads1158_TransmitReceive(cmd_readID2, back_readID2, 2);
    delay_us(20);
    ADS1158_CS2_HIGH;
    HAL_Delay(10);

    sys_ID = back_readID2[1];
    if (sys_ID == 0x9B)
    {
        //ok
        printf("ADS11582 ok\r\n");
    }
    else
    {
        //error
        printf("ADS11582 failed\r\n");
    }
    printf("ADS11582 id is %d\r\n",sys_ID);
    // ads_config();
    ads1158_adc1_data.chip_No = 1;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_2)
        ads_data_update(1);
    if(GPIO_Pin == GPIO_PIN_1)
        ads_data_update(2);
}