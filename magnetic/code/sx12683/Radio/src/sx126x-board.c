/*
  ______                              _
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX126x driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
//#include "stm32f0xx.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include "radio.h"
#include "sx126x.h"
#include "sx126x-board.h"
#include "bsp.h"

void SX126xReset( void )
{
    DelayMs( 10 );
    GPIO_ResetBits( RADIO_nRESET_PORT, RADIO_nRESET_PIN);
    DelayMs( 20 );
    GPIO_SetBits( RADIO_nRESET_PORT, RADIO_nRESET_PIN);
    DelayMs( 10 );
}

void SX126xWaitOnBusy( void )
{
   while(GPIO_ReadInputDataBit(RADIO_BUSY_PORT,RADIO_BUSY_PIN)==1);
}


void SX126xWakeup( void )
{
    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);
   
    SpiInOut(RADIO_GET_STATUS);
    SpiInOut(0);
    
     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    // Wait for chip to be ready.
    SX126xWaitOnBusy( );
}

void SX126xWriteCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{

    SX126xCheckDeviceReady( );

    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SpiInOut(( uint8_t )command );

    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut(buffer[i] );
    }

    GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);
    
    if( command != RADIO_SET_SLEEP )
    {
        SX126xWaitOnBusy( );
    }
}

void SX126xReadCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SpiInOut(( uint8_t )command );
    SpiInOut(0x00 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut(0 );
    }

     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SX126xWaitOnBusy( );
}

void SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);
    
    SpiInOut(RADIO_WRITE_REGISTER );
    SpiInOut(( address & 0xFF00 ) >> 8 );
    SpiInOut( address & 0x00FF );
    
    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut(buffer[i] );
    }


     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SX126xWaitOnBusy( );
}

void SX126xWriteRegister( uint16_t address, uint8_t value )
{
    SX126xWriteRegisters( address, &value, 1 );
}

void SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    SX126xCheckDeviceReady( );

    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SpiInOut(RADIO_READ_REGISTER );
    SpiInOut(( address & 0xFF00 ) >> 8 );
    SpiInOut( address & 0x00FF );
    SpiInOut( 0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut(0 );
    }

     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SX126xWaitOnBusy( );
}

uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    SX126xReadRegisters( address, &data, 1 );
    return data;
}

void SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    SX126xCheckDeviceReady( );

    GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);
    
    SpiInOut( RADIO_WRITE_BUFFER );
    SpiInOut( offset );
    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SX126xWaitOnBusy( );
}

void SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    SX126xCheckDeviceReady( );

     GPIO_ResetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);

    SpiInOut(  RADIO_READ_BUFFER );
    SpiInOut(  offset );
    SpiInOut(  0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }

     GPIO_SetBits( RADIO_NSS_PORT, RADIO_NSS_PIN);
    
    SX126xWaitOnBusy( );
}

void SX126xSetRfTxPower( int8_t power )
{
    SX126xSetTxParams( power, RADIO_RAMP_40_US );
}

uint8_t SX126xGetPaSelect( uint32_t channel )
{
//    if( GpioRead( &DeviceSel ) == 1 )
//    {
//        return SX1261;
//    }
//    else
//    {
//        return SX1262;
//    }
  
  return SX1262;
}

void SX126xAntSwOn( void )
{
    //GpioInit( &AntPow, ANT_SWITCH_POWER, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
}

void SX126xAntSwOff( void )
{
   // GpioInit( &AntPow, ANT_SWITCH_POWER, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

bool SX126xCheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
