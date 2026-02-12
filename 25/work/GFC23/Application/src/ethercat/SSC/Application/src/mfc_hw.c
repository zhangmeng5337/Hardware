/**
\addtogroup EL9800_HW EL9800 Platform (Serial ESC Access)
@{
*/

/**
\file    mfc_hw.c
\author Chris
\brief Implementation
Hardware access implementation for ATMEL connected via SPI to ESC

 If using the ET1100, no define is required. 
 If using LAN9252, you need to define USE_LAN9252 in the project option


Copyright Flow Devices and Systems, Inc. 2016

*/

/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "main.h"
#include "ecat_def.h"
#include "ecatslv.h"
#include "ecatappl.h"
//#include "status_codes.h"
//#include "spi.h"
//#include "delay.h"
//#include "timer.h"
#include "mfc_hw.h"
//#include "led_custom.h"

extern void Delay(UINT16 Count);

//#warning need a h file for those
//#include "rtc_config.h"
//#define ET1100_SPI_CLOCK             10000000
//#define SPI_ET100_CHIP_SEL         1

//struct spi_device spi_dev_ET1100 = {
//	/** Board specific select id */
//	.id = SPI_ET100_CHIP_SEL
//};
/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

typedef union
{
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;


/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/

#define SPI1_STAT_VALUE                    0x8000
#define SPI1_CON1_VALUE                    0x027E
#define SPI1_CON1_VALUE_16BIT            0x047E
#define SPI_DEACTIVE                    1
#define SPI_ACTIVE                        0



/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/


#define INIT_ECAT_TIMER           //     {(PR7) = 2000;/*set period*/ \
    (T7CONbits.TCKPS) = 2;/*set prescaler to 1:8*/ \
    (TMR7) = 0;/*clear timer register*/}

#define STOP_ECAT_TIMER         //       {(T7CONbits.TON) = 0; /*disable timer*/}

#define START_ECAT_TIMER       //     {(T7CONbits.TON) = 1; /*enable timer*/}

UINT32 my_ecat_timer=0, ecat_deltaT=0;
UINT16 run_led, err_led;
/*-----------------------------------------------------------------------------------------
------
------    Configuration Bits
------
-----------------------------------------------------------------------------------------*/
//_FGS(GCP_OFF);
//_FOSCSEL(FNOSC_PRI & IESO_OFF);
//_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_HS);
//_FWDT(FWDTEN_OFF);
//_FPOR(FPWRT_PWR1);
//_FICD(ICS_PGD3 & JTAGEN_OFF);

/*-----------------------------------------------------------------------------------------
------
------    LED defines
------
-----------------------------------------------------------------------------------------*/
// EtherCAT Status LEDs -> StateMachine
#define LED_ECATGREEN                 LATFbits.LATF1
#define LED_ECATRED                    LATFbits.LATF0

void ET100_spi_Init(void)
{
	/* external DAC is communicated over SPI, so initialize SPI system  */
//	spi_master_setup_device(SPI_MASTER_BASE, &spi_dev_ET1100, SPI_MODE_3,
//	ET1100_SPI_CLOCK, 0);
//	spi_enable(SPI_MASTER_BASE);
	
}

/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc

/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
 void GetInterruptRegister(void)
{
 	//uint32_t timeout = SPI_TIMEOUT;
    UINT8            EscMbxReadEcatCtrl;
    UINT8 WriteData;
	
    /* select the SPI */
  CSLOW();
	//spi_select_device(SPI_MASTER_BASE, &spi_dev_ET1100);

    /* reset transmission flag */
//    SPI1_IF=0;

    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* write to SPI1_BUF register starts the SPI access*/
	WriteData = 0;
  HAL_SPI_Transmit(&ECAT_IF, &WriteData, 1, 1000);  
    //spi_write_single(SPI_MASTER_BASE, WriteData);

    /* SPI is busy */
//	timeout = SPI_TIMEOUT;
//	while (!spi_is_rx_ready(&spi_dev_ET1100)) {
//		if (!timeout--) {
//			return; // ERR_TIMEOUT;
//		}
//	}

    /* get first byte of AL Event register */
    HAL_SPI_Receive(&ECAT_IF, &EscALEvent.Byte[0], 1, 1000);  
//    spi_read_single(SPI_MASTER_BASE, &EscALEvent.Byte[0]);

    /* reset SPI interrupt flag */
//    SPI1_IF = 0;

    /* send a read command */
	WriteData = ESC_RD;
  HAL_SPI_Transmit(&ECAT_IF, &WriteData, 1, 1000);  
//    spi_write_single (SPI_MASTER_BASE, WriteData);  
 
//	timeout = SPI_TIMEOUT;
//	while (!spi_is_rx_ready(&spi_dev_ET1100)) {
//		if (!timeout--) {
//			return; // ERR_TIMEOUT;
//		}
//	}

    /* get second byte of AL Event register */
    HAL_SPI_Receive(&ECAT_IF, &EscALEvent.Byte[1], 1, 1000);  
//    spi_read_single(SPI_MASTER_BASE, &EscALEvent.Byte[1]);

    /* reset SPI interrupt flag */
//    SPI1_IF = 0;

    /* write 0xFF to finish  */
	WriteData = 0xFF;
  HAL_SPI_Transmit(&ECAT_IF, &WriteData, 1, 1000);  
//	spi_write_single (SPI_MASTER_BASE, WriteData);

//	timeout = SPI_TIMEOUT;
//	while (!spi_is_rx_ready(&spi_dev_ET1100)) {
//		if (!timeout--) {
//			return; // ERR_TIMEOUT;
//		}
//	}

    /* get third byte of AL Event register */
     HAL_SPI_Receive(&ECAT_IF, &EscMbxReadEcatCtrl, 1, 1000);  
//    spi_read_single(SPI_MASTER_BASE, &EscMbxReadEcatCtrl);
	
    /* reset SPI interrupt flag */
//    SPI1_IF = 0;
    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */

  CSHIGH();
//	spi_deselect_device(SPI_MASTER_BASE, &spi_dev_ET1100);

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
#define ISR_GetInterruptRegister GetInterruptRegister


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
*////////////////////////////////////////////////////////////////////////////////////////
static void AddressingEsc( UINT16 Address, UINT8 Command )
{
    UBYTETOWORD tmp;
//	uint32_t timeout = SPI_TIMEOUT;
    tmp.Word = ( Address << 3 ) | Command;
    /* select the SPI */
  CSLOW();
//	spi_select_device(SPI_MASTER_BASE, &spi_dev_ET1100);

//		timeout = SPI_TIMEOUT;
//		while (!spi_is_tx_ready(&spi_dev_ET1100)) {
//			if (!timeout--) {
//				return; // ERR_TIMEOUT;             //! \todo what do we do in case of timeout?
//			}
//		}
    /* there have to be at least 15 ns after the SPI1_SEL signal was active (0) before
       the transmission shall be started */
    /* send the first address/command byte to the ESC */
  HAL_SPI_Transmit(&ECAT_IF, &tmp.Byte[1], 1, 1000);  
//    spi_write_single (SPI_MASTER_BASE, tmp.Byte[1]);

//		timeout = SPI_TIMEOUT;
//		while (!spi_is_rx_full(&spi_dev_ET1100)) {
//			if (!timeout--) {
//				return; // ERR_TIMEOUT;             //! \todo what do we do in case of timeout?
//			}
//		}

    /* get first byte of AL Event register */
    HAL_SPI_Receive(&ECAT_IF, &EscALEvent.Byte[0], 1, 1000);  
//    spi_read_single(SPI_MASTER_BASE, &EscALEvent.Byte[0]);

//		timeout = SPI_TIMEOUT;
//		while (!spi_is_tx_ready(&spi_dev_ET1100)) {
//			if (!timeout--) {
//				return; // ERR_TIMEOUT;
//			}
//		}

    /* send the second address/command byte to the ESC */
    HAL_SPI_Transmit(&ECAT_IF, &tmp.Byte[0], 1, 1000);  
//    spi_write_single (SPI_MASTER_BASE,  tmp.Byte[0]);

//		timeout = SPI_TIMEOUT;
//		while (!spi_is_rx_full(&spi_dev_ET1100)) {
//			if (!timeout--) {
//				return; // ERR_TIMEOUT;
//			}
//		}
    /* get second byte of AL Event register */
    HAL_SPI_Receive(&ECAT_IF, &EscALEvent.Byte[1], 1, 1000);  
//    spi_read_single(SPI_MASTER_BASE, &EscALEvent.Byte[1]);


//		delay_us(5);
    Delay(15);

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
        Shall be implemented if interrupts are supported else this function is equal to "AddressingEsc()"
*////////////////////////////////////////////////////////////////////////////////////////
#define ISR_AddressingEsc AddressingEsc

/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 HW_Init(void)
{
/*ECATCHANGE_START(V5.11) EL9800 2*/
    UINT32 intMask=0;
/*ECATCHANGE_END(V5.11) EL9800 2*/

    ET100_spi_Init();

/*ECATCHANGE_START(V5.11) EL9800 2*/
    do
    {
        intMask = 0x93;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

/*ECATCHANGE_END(V5.11) EL9800 2*/

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void HW_Release(void)
{

}


#if !defined (USE_LAN9252)

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i = Len;
    UINT8 *pTmpData = (UINT8 *)pData;
    char WriteData;
	char readData=0;
	uint32_t timeout = SPI_TIMEOUT;
		
    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
         AddressingEsc( Address, ESC_RD );

		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(&spi_dev_ET1100)) {
			if (!timeout--) {
				return ERR_TIMEOUT;
			}
		}
        /* when reading the last byte the DI pin shall be 1 */
		WriteData = 0xFF;
        spi_write_single (SPI_MASTER_BASE, WriteData);
		delay_us(15);
		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_full(&spi_dev_ET1100)) {
			if (!timeout--) {
				return ERR_TIMEOUT;
			}
		}
        /* get data byte */
        spi_read_single(SPI_MASTER_BASE, &readData);	
		*pTmpData++ = readData;

        spi_deselect_device(SPI_MASTER_BASE, &spi_dev_ET1100);

        /* next address */
        Address++;
        /* reset transmission flag */
//        SPI1_IF = 0;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;
	uint32_t timeout = SPI_TIMEOUT;    
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
        /* HBu 24.01.06: wrong parameter ESC_RD */
         AddressingEsc( Address, ESC_WR );
		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(&spi_dev_ET1100)) {
			if (!timeout--) {
				return ERR_TIMEOUT;
			}
		}
        /* start transmission */
		spi_write_single (SPI_MASTER_BASE, *pTmpData++);
        /* wait until transmission finished */
		delay_us(15);
		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_full(&spi_dev_ET1100)) {
			if (!timeout--) {
				return ERR_TIMEOUT;
			}
		}
        /* enable the ESC interrupt to get the AL Event ISR the chance to interrupt */
        /* SPI1_BUF must be read, otherwise the module will not transfer the next received data from SPIxSR to SPIxRXB.*/
        spi_read_single(SPI_MASTER_BASE, dummy);

        spi_deselect_device(SPI_MASTER_BASE, &spi_dev_ET1100);
        /* next address */
        Address++;
        /* reset transmission flag */
//        SPI1_IF = 0;

    }
}

#endif //    !defined (USE_LAN9252)


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function resets the hardware
*////////////////////////////////////////////////////////////////////////////////////////

void HW_RestartTarget(void)
{
}



UINT32 ecat_get_delta (void){
//TIMER tm;
//    
//    // determine the time since the last time we looked at it.
//	timer_time(&tm);
//	ecat_deltaT = ((tm.msl<<16)|tm.lsw) - my_ecat_timer;

//    return ecat_deltaT;
  return ECAT_TIMER_INC_P_MS;
}

UINT32 ecat_reset_delta (void){
//TIMER tm;
//    
//    // read the current time and set the delta to that
//	timer_time(&tm);
//	my_ecat_timer = ((tm.msl<<16)|tm.lsw);

//    return my_ecat_timer;
  return 0;
}

void HW_SetLed (UINT8 RunLed, UINT8 ErrLed) {
	
  HAL_GPIO_WritePin(HB_Red_LED_GPIO_Port, HB_Red_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(HB_Green_LED_GPIO_Port, HB_Green_LED_Pin, GPIO_PIN_SET);
  
	if (RunLed) {
		//update_led(1, LED_GRN);
    HAL_GPIO_WritePin(HB_Green_LED_GPIO_Port, HB_Green_LED_Pin, GPIO_PIN_RESET);
	}
		
	if (ErrLed) {
//		update_led(0, LED_RED);
    HAL_GPIO_WritePin(HB_Red_LED_GPIO_Port, HB_Red_LED_Pin, GPIO_PIN_RESET);    
	}	
	
}

/** @} */


