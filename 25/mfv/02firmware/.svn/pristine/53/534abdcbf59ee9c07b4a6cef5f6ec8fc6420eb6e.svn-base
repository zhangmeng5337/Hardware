/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup ETG5003_2020_MFC ETG5003_2020_MFC
@{
*/

/**
\file ETG5003_2020_MFC.c
\brief Implementation

\version 1.0.0.11
*/

#ifdef ECAT
/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "main.h"
#include "ecat_def.h"

#include "applInterface.h"
#include "bootmode.h"
//#include "bootloader.h"

//#include "errors.h"
//#include "sensor.h"
//#include "valves.h"
//#include "pid.h"
//#include "dispense.h"
//#include "gasconvert.h"
#include "libunits.h"

#define _ETG5003_2020__MFC_ 1
#include "ETG5003_2020_MFC.h"
#undef _ETG5003_2020__MFC_
/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/
#define    MAX_FILE_NAME_SIZE    16

/** \brief  MAX_FILE_SIZE: Maximum file size */
#define MAX_FILE_SIZE                             0x180
/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
UINT32           nFileWriteOffset;
CHAR             aFileName[MAX_FILE_NAME_SIZE];
UINT16 MBXMEM     aFileData[(MAX_FILE_SIZE >> 1)];


UINT32 u32FileSize;
UINT32 address = 0;
UINT16 MBXMEM sii[128];

//// Data Units
//#define DATAUNIT_COUNTS     0x1001
//#define DATAUNIT_PERCENT    0x1007

//#define DATAUNIT_SCCM            0x1400
//#define DATAUNIT_SLM             0x1401
//#define DATAUNIT_CFM             0x1402
//#define DATAUNIT_PACMS           0x1403
//#define DATAUNIT_CMS             0x1405
//#define DATAUNIT_LS              0x1406
//#define DATAUNIT_MLS             0x1407
//#define DATAUNIT_LM              0x1413
//#define DATAUNIT_MLM             0x1411

//#define DATAUNIT_PSIA            0x1300
//#define DATAUNIT_TORR            0x1301
//#define DATAUNIT_MTORR           0x1302
//#define DATAUNIT_MMHG0C          0x1303
//#define DATAUNIT_INHG0C          0x1304
//#define DATAUNIT_BAR             0x1307
//#define DATAUNIT_MBAR            0x1308
//#define DATAUNIT_PA              0x1309
//#define DATAUNIT_KPA             0x130A
//#define DATAUNIT_ATM             0x130B
//#define DATAUNIT_PSIG            0x0300

//#define DATAUNIT_FAHRENHEIT      0x1201
//#define DATAUNIT_CELSIUS         0x1200
//#define DATAUNIT_KELVIN          0x1202
//#define DATAUNIT_RANKINE         0x1203

//typedef struct {
//	uint32_t UnitsCode;
//	float  scale;
//	float  offset;
//} DATA_UNITS_CONV_TABLE;

//typedef struct {
//	uint16_t UnitsCode;
//	uint16_t NativeUnitsCode;
//}DATA_UNITS_MAP_TABLE;

//const DATA_UNITS_CONV_TABLE UnitsConvPressure[] = {
//	{ DATAUNIT_PSIG,           6894.757F,  1.013250E+05F },
//	{ DATAUNIT_PSIA,           6894.757F,  0.0F },
//	{ DATAUNIT_KPA,            1000.0F,    0.0F },
//	{ DATAUNIT_TORR,           133.3220F,  0.0F },
//	{ DATAUNIT_MTORR,          0.1333220F, 0.0F },
//	{ DATAUNIT_MMHG0C,         133.3220F,  0.0F },
//	{ DATAUNIT_INHG0C,         3386.389F,  0.0F },
//	{ DATAUNIT_BAR,            100000.0F,  0.0F },
//	{ DATAUNIT_MBAR,           100.0F,     0.0F },
//	{ DATAUNIT_ATM,            101325.0F,  0.0F },
//	{ DATAUNIT_PA,             1.0F,       0.0F },
//	{ 0x0000,               1.0F,       0.0F } // end of table, use Pascal as defaults
//};

//const DATA_UNITS_CONV_TABLE UnitsConvTemperature[] = {
//	{ DATAUNIT_CELSIUS,        1.8F,       4.916700E+02F },
//	{ DATAUNIT_FAHRENHEIT,     1.0F,       4.596700E+02F },
//	{ DATAUNIT_KELVIN,         1.8F,       0.0F },
//	{ DATAUNIT_RANKINE,        1.0F,       0.0F },
//	{ 0x0000,               1.0F,       0.0F } // end of table, use Rankine as defaults
//};

//const DATA_UNITS_CONV_TABLE UnitsConvFlow[] = {
//	{ DATAUNIT_SLM,            1000.0F,    0.0F },
//	{ DATAUNIT_CFM,            28316.85F,  0.0F },
//	{ DATAUNIT_SCCM,           1.0F,       0.0F },
//	{ DATAUNIT_PACMS,          6.0E+07F,   0.0F },
//	{ DATAUNIT_CMS,            6.0E+02F,   0.0F },
//	{ DATAUNIT_LS,             6.0E+04F,   0.0F },
//	{ DATAUNIT_MLS,            60.0F,      0.0F },
//	{ DATAUNIT_LM,             1000.0F,    0.0F },
//	{ DATAUNIT_MLM,            1.0F,       0.0F },
//	{ 0x0000,               1.0F,       0.0F } // end of table, use SCCM as defaults
//};

typedef struct {
	uint32_t EcUnitsCode;
	uint16_t NativeUnitsCode;
} EC_DATA_UNITS_CONV_TABLE;

//const EC_DATA_UNITS_CONV_TABLE EcToNativePresUnits[] = {
//	// Pressure units
//	{ 0x00A10000, DATAUNIT_TORR    },
//	{ 0x00A20000, DATAUNIT_ATM     },
//	{ 0x00A30000, DATAUNIT_PSIA    },
//	{ 0x00A40000, DATAUNIT_PSIG    },
//	{ 0x004E0000, DATAUNIT_BAR     },
//	{ 0xFD4E0000, DATAUNIT_MBAR    },
//	{ 0x00220000, DATAUNIT_PA      },
//	{ 0x03220000, DATAUNIT_KPA     },
//	// End of List
//	{ 0x00000000, 0x0000        }
//};
//const EC_DATA_UNITS_CONV_TABLE EcToNativeFlowUnits[] = {
//	{ 0x00A00000, DATAUNIT_SCCM    },
//	{ 0x03A00000, DATAUNIT_SLM     },
//	// End of List
//	{ 0x00000000, 0x0000        }
//};
//const EC_DATA_UNITS_CONV_TABLE EcToNativeTempUnits[] = {
//	{ 0x002D0000, DATAUNIT_CELSIUS },
//	{ 0x00050000, DATAUNIT_KELVIN  },
//	// End of List
//	{ 0x00000000, 0x0000        }
//};

typedef struct
{
	uint16_t flowunits;
	uint16_t pressureunits;
	int16_t temperatureunits;
	
	uint16_t status;
}ECAT_VAR_STRUCT;

ECAT_VAR_STRUCT	  ecat_Var;

/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/
uint32_t APPL_ConvertToEtherCATUnits( uint16_t argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr ) {
	uint8_t i = 0;
	// Loop thru table until a match is found OR until end of table is reached
	while( 0 != tablePtr[i].EcUnitsCode ) {
		if ( tablePtr[i].NativeUnitsCode == argNativeUnits ) {
			// Match found, return the Ethercat equivalent
			return tablePtr[i].EcUnitsCode;
		}
		i++;
	}
	// END of Table, no match, return 0
	return 0;
}

int16_t APPL_ConvertFromEtherCATUnits( uint32_t argEtherCATUnits, uint16_t *argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr ) {
	uint8_t i = 0;
	// Loop thru table until a match is found OR until end of table is reached
	while( 0 != tablePtr[i].EcUnitsCode ) {
		if( tablePtr[i].EcUnitsCode == argEtherCATUnits ) {
			// Match found, return the Ethercat equivalent
			*argNativeUnits = tablePtr[i].NativeUnitsCode;
			return 0;
		}
		i++;
	}
	// END of Table, no match, return error
	return -1;
}

static void ScaleTable ( const DATA_UNITS_CONV_TABLE *table, float *scale, float *offset, uint16_t dUnits ) {
	uint8_t i=0;
	uint8_t found = 0;
	
	while ( ( table[i].UnitsCode != 0x0000 ) & ( !found ) ) {
		if ( table[i].UnitsCode == dUnits ) {
			*scale = table[i].scale;
			*offset = table[i].offset;
			found = 1;
			} else {
			i++;
		}
	}
	
	if ( !found ) {
		*scale = table[i].scale;
		*offset = table[i].offset;
	}
}

float convert_units ( const DATA_UNITS_CONV_TABLE *table, float value, uint16_t sUnits, uint16_t dUnits ) {
	float sScale, sOffset, dScale, dOffset;
	float baseValue;
	ScaleTable ( table, &sScale, &sOffset, sUnits ); // scale values to convert source units
	ScaleTable ( table, &dScale, &dOffset, dUnits ); // scale values to convert target units
	baseValue = value * sScale + sOffset;
	return ( baseValue - dOffset ) / dScale;
}


//uint16_t ConvertToTargetUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argNativeUnits ) {
//	uint16_t locNativeUnits;
//	uint8_t i = 0;

//	// Loop thru table until a match is found OR until end of table is reached
//	while ( 1 ) {
//		locNativeUnits = table[i].NativeUnitsCode;

//		if ( locNativeUnits == argNativeUnits ) {
//			// Match found, return the DeviceNet equivalent
//			return table[i].UnitsCode;

//			} else {
//			if ( locNativeUnits == 0 ) {
//				// END of Table, no match, return 0
//				return 0;
//			}
//		}

//		// Not found, go to next entry in table
//		i++;
//	}
//}

//int16_t ConvertToNativeUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argUnits, uint16_t *argNativeUnits ) {
//	uint16_t locEcUnits;
//	uint8_t i = 0;

//	// Loop thru table until a match is found OR until end of table is reached
//	while ( 1 ) {
//		locEcUnits = table[i].UnitsCode;

//		if ( locEcUnits == 0 ) {
//			// END of Table, no match, return error
//			return -1;
//		}
//		else {
//			if ( locEcUnits == argUnits ) {
//				// Match found, return the Ethercat equivalent
//				*argNativeUnits = table[i].NativeUnitsCode;
//				return 0;

//			}
//		}
//		// Not found, go to next entry in table
//		i++;
//	}
//}

void ecat_userobj_init (UINT8 init_type) {
	
	switch (init_type) {
		case 1:
		default:
//			ecat_Var.flowunits = DATAUNIT_SCCM;
//			ecat_Var.pressureunits = DATAUNIT_PSIA;
//			ecat_Var.temperatureunits = DATAUNIT_CELSIUS;
//			ecat_Var.status = 0x0000;		
			break;
	}
}


/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/
UINT16 ESC_EEPROM_Manager (void) {
//	uint32_t fl_storage_addr = FLASH_STORAGE_BLOCK;		
//	UINT32 wordaddress = 0;
//	volatile uint32_t *app_check_blk = APP_CHECK_BLOCK;
//	uint32_t app_chsum_info_block[3];
//	
//	memcpy(app_chsum_info_block, app_check_blk, sizeof(app_chsum_info_block));
//	// check for new SII downloaded pattern from application
//	if ((app_chsum_info_block[2] & 0xFF000000) == 0x5a000000) {		
//		if (ESC_EepromAccess(0,128,sii,ESC_RD) == ALSTATUSCODE_NOERROR) {
//			if (ESC_EepromAccess(wordaddress, 2048, (UINT16 *)(fl_storage_addr+app_chsum_info_block[1]), ESC_WR) == ALSTATUSCODE_NOERROR) {			
//				app_chsum_info_block[2] &= ~0xFF000000;	// clear pattern
//				memory_erase(APP_CHECK_BLOCK,12);
//				memory_write_word(APP_CHECK_BLOCK,app_chsum_info_block,12);
//				// TODO: Do we need to reset if we are not writing config area
//				//resetDevice = 1;
//			}			
//		}
//	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when a read request from the master is issued

*////////////////////////////////////////////////////////////////////////////////////////
UINT16 FoE_Read(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData)
{
	UINT16 i = 0;
	UINT16 sizeError = 0;

	CHAR aReadFileName[MAX_FILE_NAME_SIZE];


	if ((nameSize + 1) > MAX_FILE_NAME_SIZE)
	{

		return ECAT_FOE_ERRCODE_DISKFULL;
	}

	/*Read requested file name to endianess conversion if required*/
	MBXSTRCPY(aReadFileName, pName, nameSize);
	aReadFileName[nameSize] = '\0';

	if (u32FileSize == 0)
	{
		/* No file stored*/
		return ECAT_FOE_ERRCODE_NOTFOUND;
	}

	/* for test only the written file name can be read */
	for (i = 0; i < nameSize; i++)
	{
		if (aReadFileName[i] != aFileName[i])
		{
			/* file name not found */
			return ECAT_FOE_ERRCODE_NOTFOUND;
		}
	}

	sizeError = maxBlockSize;

	if (u32FileSize < sizeError)
	{
		sizeError = (UINT16)u32FileSize;
	}

	/*copy the first foe data block*/
	MEMCPY(pData, aFileData, sizeError);

	return sizeError;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when a block of data is requested for read

*////////////////////////////////////////////////////////////////////////////////////////
UINT16 FoE_ReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData)
{
	UINT16 sizeError = 0;

	if (u32FileSize < offset)
	{
		return 0;
	}

	/*get file length to send*/
	sizeError = (UINT16)(u32FileSize - offset);


	if (sizeError > maxBlockSize)
	{
		/*transmit max block size if the file data to be send is greater than the max data block*/
		sizeError = maxBlockSize;
	}

	/*copy the foe data block 2 .. n*/
	MEMCPY(pData, &(((UINT8 *)aFileData)[offset]), sizeError);

	return sizeError;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when a block of data is sent via FoE by the master

*////////////////////////////////////////////////////////////////////////////////////////
UINT16 FoE_WriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing)
{
	if (bBootMode)
	{
		return BL_Data(pData, Size);
	}
	else
	if ((nFileWriteOffset + Size) > MAX_FILE_SIZE)
	{
		return ECAT_FOE_ERRCODE_DISKFULL;
	}

	if (Size)
	{
		MBXMEMCPY(&aFileData[(nFileWriteOffset >> 1)], pData, Size);

	}

	if (bDataFollowing)
	{
		/* FoE-Data services will follow */
		nFileWriteOffset += Size;
		
	}
	else
	{
		/* last part of the file is written */
		u32FileSize = nFileWriteOffset + Size;
		nFileWriteOffset = 0;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when a write request is issued by the master

*////////////////////////////////////////////////////////////////////////////////////////
UINT16 FoE_Write(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password)
{
	if (nameSize < MAX_FILE_NAME_SIZE)
	{
		/* for test every file name can be written */
		MBXSTRCPY(aFileName, pName, nameSize);
		MBXSTRCPY(aFileName + nameSize, "\0", 1); //string termination

		// TODO: If file system is implemented, this is the downloaded image filename
		// TODO: Filename or Password can be initially checked here before download is started

		nFileWriteOffset = 0;
		u32FileSize = 0;
		return 0;
	}
	else
	{
		return ECAT_FOE_ERRCODE_DISKFULL;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    APPL_AckErrorInd(UINT16 stateTrans)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
           all general settings were checked to start the input handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

#if COE_SUPPORTED
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
   
    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
                pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            /*assigned PDO was not found in object dictionary. return invalid mapping*/
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }

    OutputSize = (OutputSize + 7) >> 3;

    if(result == 0)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
                    pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                /*assigned PDO was not found in object dictionary. return invalid mapping*/
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

#else
#if _WIN32
   #pragma message ("Warning: Define 'InputSize' and 'OutputSize'.")
#else
    #warning "Define 'InputSize' and 'OutputSize'."
#endif
#endif

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
    return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT8 getBits(UINT32 *pISIEntry, UINT16 Index, UINT16 Subindex, UINT16 size) {
	UINT8 bit=0;
	UINT16 data = (UINT16)*pISIEntry;
	UINT16 mask = 0;
	
	switch (Index) {
		case 0x600F:	// ETG.2020 application specific for retrieving bits
			for (int i=0; i<size; i++) {
				mask <<= 1;
				mask |= 1;
			}
			bit = (data >> (Subindex-1)) & mask;
		break;
	}
	
	return bit;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void putBits(UINT8 *pData_lcl, UINT32 *pISIEntry, UINT16 Index, UINT16 Subindex, UINT8 u8ctr, UINT16 size) {
	UINT16 mask = 0;
	//UINT16 data = 0;
	UINT8 bit=0;
	
	switch (Index) {
		case 0x700F:	// ETG.2020 application specific for retrieving bits
		for (int i=0; i<size; i++) {
			mask <<= 1;
			mask |= 1;
		}
		bit = (*pData_lcl >> u8ctr) & mask;
		*pISIEntry &= ~(mask << (Subindex-1));
		*pISIEntry |= (bit << (Subindex-1));
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data

\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(UINT16* pData)
{
#if _WIN32
   #pragma message ("Warning: Implement input (Slave -> Master) mapping")
#else
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	UINT8 *pData_lcl = (UINT8*)(pData);
	UINT32 *pPDOEntry = NULL;
	UINT32 *pISIEntry = NULL;
	UINT16 InputSize = 0;
	UINT16 Subindex = 0;
	UINT16 Index = 0;
	UINT8 fill_in_progress = 0;
	UINT8 u8fill = 0;
	UINT8 u8ctr = 0;
		
	/*Scan Object 0x1C13 TXPDO assign*/
	for (UINT16 PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++) {
		// Get the PDO object (default, flexible)
	    pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);		
	    if(pPDO != NULL)
	    {	// Look at how many indices do we need to pack per PDO assignment
		    UINT16 PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
		    for(UINT16 PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
		    {
				// Get the PDO entry and parse the Index, Subindex and bit size
			    pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);
				Index = (UINT16) (((*pPDOEntry) >> 16) & 0xFFFF);
				Subindex = (UINT16) (((*pPDOEntry)>> 8) & 0xFF);
				InputSize = (UINT16) ((*pPDOEntry) & 0xFF);
				
				// Retrieve the object based on the object Index
				OBJCONST TOBJECT OBJMEM * pISI = OBJ_GetObjectHandle(Index);
				// Retrieve the data based on the subindex of the object
				pISIEntry = (UINT32 *)((UINT16 *)pISI->pVarPtr + (OBJ_GetEntryOffset((Subindex),pISI)>>3)/2);
								
				if (InputSize >= 0x08) {
					if (fill_in_progress == 1) {						
						// pack the data based on the size
						memcpy( pData_lcl, &u8fill, 1);
						pData_lcl+=1;
						u8fill = 0;
						fill_in_progress = 0;
					}
					// pack the data based on the size
					memcpy( pData_lcl, pISIEntry, InputSize/8);
					pData_lcl += InputSize/8;
									
				} else {
					u8fill |= (getBits(pISIEntry, Index, Subindex, InputSize) << u8ctr);
					u8ctr += InputSize;
					fill_in_progress = 1;
					// check if the last fill exceeds byte boundary. If so, transfer to data buffer. Left over bits are also handled here by subtracting 8 from the fill counter
					if (u8ctr >= 8) {
						// pack the data based on the size
						memcpy( pData_lcl, &u8fill, 1);
						pData_lcl+=1;
						u8ctr -= 8;
						u8fill = 0;
						fill_in_progress = 0;
					}		
				}			
		    }
	    }
	    else
	    {
		    /*assigned PDO was not found in object dictionary. return invalid mapping*/
	    }				
	}
	
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(UINT16* pData)
{
#if _WIN32
   #pragma message ("Warning: Implement output (Master -> Slave) mapping")
#else
	OBJCONST TOBJECT OBJMEM * pPDO = NULL;
	UINT8 *pData_lcl = (UINT8*)(pData);
	UINT32 *pPDOEntry = NULL;
	UINT32 *pISIEntry = NULL;
	UINT16 InputSize = 0;
	UINT16 Subindex = 0;
	UINT16 Index = 0;
	UINT8 u8fill = 0;
	UINT8 u8ctr = 0;
	
	/*Scan object 0x1C12 RXPDO assign*/
	for (UINT16 PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++) {
		// Get the PDO object (default, flexible)
		pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
		if(pPDO != NULL)
		{	// Look at how many indices do we need to pack per PDO assignment
			UINT16 PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
			for(UINT16 PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
			{
				// Get the PDO entry and parse the Index, Subindex and bit size
				pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);
				Index = (UINT16) (((*pPDOEntry) >> 16) & 0xFFFF);
				Subindex = (UINT16) (((*pPDOEntry)>> 8) & 0xFF);
				InputSize = (UINT16) ((*pPDOEntry) & 0xFF);
				
				// Retrieve the object based on the object Index
				OBJCONST TOBJECT OBJMEM * pISI = OBJ_GetObjectHandle(Index);	
				// Retrieve the data based on the subindex of the object then absorb the data based on the 		
				pISIEntry = (UINT32 *)((UINT16 *)pISI->pVarPtr + (OBJ_GetEntryOffset((Subindex),pISI)>>3)/2);
				
				if (InputSize >= 0x08) {	// process 8 bit aligned data
					memcpy(pISIEntry, pData_lcl, InputSize>>3);
					pData_lcl += (InputSize>>3);
				} else {	// process <8 bits data set here					
					
					putBits(pData_lcl, pISIEntry, Index, Subindex, u8ctr, InputSize);
					u8ctr += InputSize;
					if (u8ctr >= 8) {
						pData_lcl+=1;
						u8ctr = 0;
					}
				}
			}
		}
		else
		{
			/*assigned PDO was not found in object dictionary. return invalid mapping*/
		}
	}
 	
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronization ISR 
            or from the mainloop if no synchronization is supported
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
#if _WIN32
   #pragma message ("Warning: Implement the slave application")
#else
	
	ServiceResetRequest(); // check for device reset request flag
	
	UpdateAliasRegister(); // update station alias register per switch
		
	// TODO: This is the link between application and EtherCAT
	//		 Populate depending on application variables
	//
//	SensorFlowFloating0x6000.FlowReadingREAL                    = convert_units( UnitsConvFlow, Status.fCurrentSccm, DATAUNIT_SCCM, ecat_Var.flowunits );
//	SensorPressureFloating0x6001.PressureReadingREAL            = convert_units( UnitsConvPressure, Sensors.fOvPidPsia, DATAUNIT_PSIA, ecat_Var.pressureunits );
//	SensorTemperatureFloating0x6002.TemperatureReadingREAL      = convert_units( UnitsConvTemperature, Sensors.fOvFilteredDegK, DATAUNIT_KELVIN, ecat_Var.temperatureunits );
//	SensorFlowInteger0x6004.FlowReadingINT						= Status.fCurrentSccm / Gas.fFullscaleSccm * 24576;
//	SensorPressureInteger0x6005.PressureReadingINT				= Sensors.fOvPidPsia / sensorSerial.FullScale[1] * 24576;
//	SensorTemperatureInteger0x6006.TemperatureReadingINT		= Sensors.fOvFilteredDegK / 423.15f * 24576;		
//	Actuator0x6009.ActuatorSetpointREAL                         = Pid.fCurrentCo * powerLimitSettings.lastMaxOutletPWMfract;
//	Actuator0x6009.PositionReadbackREAL                         = 0.0F;     // we don't have a position sensor
	//Status0x600F[0]												= 0;
//	dispense.fSccmSetpoint										= convert_units(UnitsConvFlow, ControllerFlowSPFloating0x7003.FlowSPREAL, ecat_Var.flowunits, DATAUNIT_SCCM);          // sccm setpoint
//	//ControllerFlowSPInteger0x7007.FlowSPINT;
//	dispense.fRampTime  										= ControllerRampTime0x7008.RampTime;
//	
//	switch (Actuator0x7009.ActuatorControl) {
//		case 2: // fully open
//			startPurge();
//		break;
//		case 3: // fully closed
//			dispense.fSccmSetpoint = 0;
//		break;
//		case 0:
//		default:
//			dispense.bDigitalMode = bDigitalMode;
//		break;
//	}
	
	//Actuator0x7009.ActuatorPositionSPREAL;  // we don't have a position sensor
	//Status0x700F.OutputCycleCounter;
//	SensorFlowFloating0x8000.FlowDataUnit						= APPL_ConvertToEtherCATUnits(ecat_Var.flowunits, EcToNativeFlowUnits);
//	SensorPressureFloating0x8001.PressureDataUnit				= APPL_ConvertToEtherCATUnits(ecat_Var.pressureunits, EcToNativePresUnits);
//	SensorTemperatureFloating0x8002.TemperatureDataUnit			= APPL_ConvertToEtherCATUnits(ecat_Var.temperatureunits, EcToNativeTempUnits);
//	ControllerRampTime0x8008.MaxRampTime						= dispense.fMaxRampTime;
	//Actuator0x8009.SafeState;
	//ActiveGasCalibrationIndex0x800A.Index                       = GAS.;
//	SensorFlowFloating0x9000.FlowReadingZeroOffset				= 0.0f;
//	SensorPressureFloating0x9001.PressureSensorFullScale        = convert_units(UnitsConvPressure, sensorSerial.FullScale[1], DATAUNIT_PSIA, ecat_Var.pressureunits);
//	SensorPressureFloating0x9001.PressureReadingZeroOffset		= 0.0f;
//	SensorPressureFloating0x9001.MFCPressureHighThresholdAlarm  = 100.0f; // 100% of FS
//	SensorPressureFloating0x9001.MFCPressureLowThresholdAlarm   = 10.0f; // 1% of FS
//	SensorTemperatureFloating0x9002.TemperatureSensorFullScale  = convert_units( UnitsConvTemperature, 423.15f, DATAUNIT_KELVIN, ecat_Var.temperatureunits );
//	SensorTemperatureFloating0x9002.MFCTempHighThresholdAlarm	= 80.0f; // 65C is at 80% of FS
//	SensorTemperatureFloating0x9002.MFCTempLowThresholdAlarm    = 64.0f; // 0C os at 60% of FS;	
//	ControllerRampTime0x9008.MinRampTime                        = dispense.fMinRampTime; // minimum ramp time
//	Actuator0x9009.ValveResolution                              = 0; //no position control capabilities
//	Actuator0x9009.StrokeLimits                                 = 0; //no position control capabilities
	//
	//GasParameterInstance10x900A;
	//GasParameterInstance20x900B;
	//GasParameterInstance30x900C;
	//GasParameterInstance40x900D;
	//GasParameterInstance50x900E;
	//
	//ActiveDeviceWarningDetails0xF381.aEntries[0];
	//ActiveDeviceErrorDetails0xF383.aEntries[0];
	//LatchedExceptionStatus0xF390;
	//LatchedDeviceWarningDetails0xF391.aEntries[0];
	//LatchedDeviceErrorDetails0xF393.aEntries[0];
	//DeviceWarningMask0xF3A1.aEntries[0];
	//DeviceErrorMask0xF3A3.aEntries[0];
	//LatchedTimestamp0xF6F0.aEntries[0];
	//SerialNumber0xF9F0;
	//CDPFGN0xF9F1.aEntries[0];
	//SDPFGN0xF9F2.aEntries[0];
	//VendorName0xF9F3;
	//DeviceName0xF9F4.aEntries[0];
	//OutputIdentifier0xF9F5.aEntries[0];
	//UpTime0xF9F6;
	//TotalUpTime0xF9F7;
	//FWUFGN0xF9F8;
	//
	// Services:
	//  ZeroAdjustForFlowSensor0xFB10
	//  ZeroAdjustForPressureSensor0xFB11
	//  SetTemperatureSensor0xFB12
	//  CardinalPointCalibration0xFB13
	//  CreateGasPageService0xFB14
	//  DeviceReset0xFBF0
	//  ExceptionReset0xFBF1
	//  StoreParam0xFBF2
	//  CalcChecksum0xFBF3
	//  LoadParam0xFBF4	

#endif
}

#if EXPLICIT_DEVICE_ID
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Calculate the Explicit Device ID
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDeviceID()
{
#if _WIN32
   #pragma message ("Warning: Implement explicit Device ID latching")
#else
	// return macid
	return 5; //ulMacId;
#endif
    /* Explicit Device 5 is expected by Explicit Device ID conformance tests*/
	// should not get here!!!
    return 0x5;
}
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT8 ExtractGasPageInfo(TOBJ900AE *gaspageinfo, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess) {
		
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &gaspageinfo->u16SubIndex0, sizeof(gaspageinfo->u16SubIndex0));	
		pData_lcl += sizeof(gaspageinfo->u16SubIndex0);	
		MEMCPY(pData_lcl, &gaspageinfo->GasCalibrationIndex, sizeof(gaspageinfo->GasCalibrationIndex));
		pData_lcl += sizeof(gaspageinfo->GasCalibrationIndex);
		MEMCPY(pData_lcl, &gaspageinfo->GasNumber, sizeof(gaspageinfo->GasNumber));
		pData_lcl += sizeof(gaspageinfo->GasNumber);
		MEMCPY(pData_lcl, &gaspageinfo->GasSymbol, sizeof(gaspageinfo->GasSymbol));
		pData_lcl += sizeof(gaspageinfo->GasSymbol);
		MEMCPY(pData_lcl, &gaspageinfo->GasName, sizeof(gaspageinfo->GasName));
		pData_lcl += sizeof(gaspageinfo->GasName);
		MEMCPY(pData_lcl, &gaspageinfo->MinimumFullScale, sizeof(gaspageinfo->MinimumFullScale));
		pData_lcl += sizeof(gaspageinfo->MinimumFullScale);
		MEMCPY(pData_lcl, &gaspageinfo->NominalFullScale, sizeof(gaspageinfo->NominalFullScale));
		pData_lcl += sizeof(gaspageinfo->NominalFullScale);
		MEMCPY(pData_lcl, &gaspageinfo->ConfiguredFullScaleRange, sizeof(gaspageinfo->ConfiguredFullScaleRange));
		pData_lcl += sizeof(gaspageinfo->ConfiguredFullScaleRange);
		MEMCPY(pData_lcl, &gaspageinfo->MinimumFlowSP, sizeof(gaspageinfo->MinimumFlowSP));
		pData_lcl += sizeof(gaspageinfo->MinimumFlowSP);
		MEMCPY(pData_lcl, &gaspageinfo->MaximumFlowSP, sizeof(gaspageinfo->MaximumFlowSP));
		pData_lcl += sizeof(gaspageinfo->MaximumFlowSP);
		MEMCPY(pData_lcl, &gaspageinfo->DeviceBinNumber, sizeof(gaspageinfo->DeviceBinNumber));
		pData_lcl += sizeof(gaspageinfo->DeviceBinNumber);
		MEMCPY(pData_lcl, &gaspageinfo->GasCalibrationFileRevisionNumber, sizeof(gaspageinfo->GasCalibrationFileRevisionNumber));
		pData_lcl += sizeof(gaspageinfo->GasCalibrationFileRevisionNumber);
		MEMCPY(pData_lcl, &gaspageinfo->RevisionOfSupportedGasTable, sizeof(gaspageinfo->RevisionOfSupportedGasTable));
		pData_lcl += sizeof(gaspageinfo->RevisionOfSupportedGasTable);
		MEMCPY(pData_lcl, &gaspageinfo->DateOfFactoryCalibration, sizeof(gaspageinfo->DateOfFactoryCalibration));
		pData_lcl += sizeof(gaspageinfo->DateOfFactoryCalibration);
		MEMCPY(pData_lcl, &gaspageinfo->DateOfLastCalibration, sizeof(gaspageinfo->DateOfLastCalibration));
		pData_lcl += sizeof(gaspageinfo->DateOfLastCalibration);
		MEMCPY(pData_lcl, &gaspageinfo->CardinalPointsArraySize, sizeof(gaspageinfo->CardinalPointsArraySize));
		pData_lcl += sizeof(gaspageinfo->CardinalPointsArraySize);
		MEMCPY(pData_lcl, &gaspageinfo->CardinalPoints, sizeof(gaspageinfo->CardinalPoints));
		return 0;
	}
			
	switch (subindex) {
		case 0:
			MEMCPY(pData, &gaspageinfo->u16SubIndex0, dataSize);
			break;
		case 1:
			MEMCPY(pData, &gaspageinfo->GasCalibrationIndex, dataSize);
			break;
		case 2:
			MEMCPY(pData, &gaspageinfo->GasNumber, dataSize);
			break;
		case 3:
			MEMCPY(pData, &gaspageinfo->GasSymbol, dataSize);
			break;
		case 4:
			MEMCPY(pData, &gaspageinfo->GasName, dataSize);
			break;
		case 5:
			MEMCPY(pData, &gaspageinfo->MinimumFullScale, dataSize);
			break;
		case 6:
			MEMCPY(pData, &gaspageinfo->NominalFullScale, dataSize);
			break;
		case 7:
			MEMCPY(pData, &gaspageinfo->ConfiguredFullScaleRange, dataSize);
			break;
		case 8:
			MEMCPY(pData, &gaspageinfo->MinimumFlowSP, dataSize);
			break;
		case 9:
			MEMCPY(pData, &gaspageinfo->MaximumFlowSP, dataSize);
			break;
		case 10:
			MEMCPY(pData, &gaspageinfo->DeviceBinNumber, dataSize);
			break;
		case 11:
			MEMCPY(pData, &gaspageinfo->GasCalibrationFileRevisionNumber, dataSize);
			break;
		case 12:
			MEMCPY(pData, &gaspageinfo->RevisionOfSupportedGasTable, dataSize);
			break;
		case 13:
			MEMCPY(pData, &gaspageinfo->DateOfFactoryCalibration, dataSize);
			break;
		case 14:
			MEMCPY(pData, &gaspageinfo->DateOfLastCalibration, dataSize);
			break;
		case 15:
			MEMCPY(pData, &gaspageinfo->CardinalPointsArraySize, dataSize);
			break;
		case 16:
			MEMCPY(pData, &gaspageinfo->CardinalPoints, dataSize);
			break;								
		default:
			return ABORTIDX_UNSUPPORTED_ACCESS;
			break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x900A..E
*/
UINT8 Read0x900A_E(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	UINT8 retcode = ABORTIDX_PARAM_IS_INCOMPATIBLE;
	// Pass the pointer of the target gas page instance
	switch (index) {
		case 0x900A:
			retcode = ExtractGasPageInfo(&GasParameterInstance10x900A, subindex, dataSize, pData, bCompleteAccess);
			break;	
		case 0x900B:
			retcode = ExtractGasPageInfo(&GasParameterInstance20x900B, subindex, dataSize, pData, bCompleteAccess);
			break;	
		case 0x900C:
			retcode = ExtractGasPageInfo(&GasParameterInstance30x900C, subindex, dataSize, pData, bCompleteAccess);
			break;	
		case 0x900D:
			retcode = ExtractGasPageInfo(&GasParameterInstance40x900D, subindex, dataSize, pData, bCompleteAccess);
			break;	
		case 0x900E:
			retcode = ExtractGasPageInfo(&GasParameterInstance50x900E, subindex, dataSize, pData, bCompleteAccess);
			break;												
	}	

	return retcode;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFB10(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFB10)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &ZeroAdjustForFlowSensor0xFB10.u16SubIndex0, sizeof(ZeroAdjustForFlowSensor0xFB10.u16SubIndex0));
		pData_lcl += sizeof(ZeroAdjustForFlowSensor0xFB10.u16SubIndex0);
		MEMCPY(pData_lcl, &ZeroAdjustForFlowSensor0xFB10.Command, sizeof(ZeroAdjustForFlowSensor0xFB10.Command));
		pData_lcl += sizeof(ZeroAdjustForFlowSensor0xFB10.Command);
		MEMCPY(pData_lcl, &ZeroAdjustForFlowSensor0xFB10.Status, sizeof(ZeroAdjustForFlowSensor0xFB10.Status));
		pData_lcl += sizeof(ZeroAdjustForFlowSensor0xFB10.Status);
		MEMCPY(pData_lcl, &ZeroAdjustForFlowSensor0xFB10.Response, sizeof(ZeroAdjustForFlowSensor0xFB10.Response));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &ZeroAdjustForFlowSensor0xFB10.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &ZeroAdjustForFlowSensor0xFB10.Command, dataSize);
		break;
		case 2:
		MEMCPY(pData, &ZeroAdjustForFlowSensor0xFB10.Status, dataSize);
		break;
		case 3:
		MEMCPY(pData, &ZeroAdjustForFlowSensor0xFB10.Response, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFB11(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFB11)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &ZeroAdjustForPressureSensor0xFB11.u16SubIndex0, sizeof(ZeroAdjustForPressureSensor0xFB11.u16SubIndex0));
		pData_lcl += sizeof(ZeroAdjustForPressureSensor0xFB11.u16SubIndex0);
		MEMCPY(pData_lcl, &ZeroAdjustForPressureSensor0xFB11.Command, sizeof(ZeroAdjustForPressureSensor0xFB11.Command));
		pData_lcl += sizeof(ZeroAdjustForPressureSensor0xFB11.Command);
		MEMCPY(pData_lcl, &ZeroAdjustForPressureSensor0xFB11.Status, sizeof(ZeroAdjustForPressureSensor0xFB11.Status));
		pData_lcl += sizeof(ZeroAdjustForPressureSensor0xFB11.Status);
		MEMCPY(pData_lcl, &ZeroAdjustForPressureSensor0xFB11.Response, sizeof(ZeroAdjustForPressureSensor0xFB11.Response));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &ZeroAdjustForPressureSensor0xFB11.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &ZeroAdjustForPressureSensor0xFB11.Command, dataSize);
		break;
		case 2:
		MEMCPY(pData, &ZeroAdjustForPressureSensor0xFB11.Status, dataSize);
		break;
		case 3:
		MEMCPY(pData, &ZeroAdjustForPressureSensor0xFB11.Response, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFB12(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFB12)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &SetTemperatureSensor0xFB12.u16SubIndex0, sizeof(SetTemperatureSensor0xFB12.u16SubIndex0));
		pData_lcl += sizeof(SetTemperatureSensor0xFB12.u16SubIndex0);
		MEMCPY(pData_lcl, &SetTemperatureSensor0xFB12.Command, sizeof(SetTemperatureSensor0xFB12.Command));
		pData_lcl += sizeof(SetTemperatureSensor0xFB12.Command);
		MEMCPY(pData_lcl, &SetTemperatureSensor0xFB12.Status, sizeof(SetTemperatureSensor0xFB12.Status));
		pData_lcl += sizeof(SetTemperatureSensor0xFB12.Status);
		MEMCPY(pData_lcl, &SetTemperatureSensor0xFB12.Response, sizeof(SetTemperatureSensor0xFB12.Response));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &SetTemperatureSensor0xFB12.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &SetTemperatureSensor0xFB12.Command, dataSize);
		break;
		case 2:
		MEMCPY(pData, &SetTemperatureSensor0xFB12.Status, dataSize);
		break;
		case 3:
		MEMCPY(pData, &SetTemperatureSensor0xFB12.Response, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFB13(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFB13)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &CardinalPointCalibration0xFB13.u16SubIndex0, sizeof(CardinalPointCalibration0xFB13.u16SubIndex0));
		pData_lcl += sizeof(CardinalPointCalibration0xFB13.u16SubIndex0);
		MEMCPY(pData_lcl, &CardinalPointCalibration0xFB13.Command, sizeof(CardinalPointCalibration0xFB13.Command));
		pData_lcl += sizeof(CardinalPointCalibration0xFB13.Command);
		MEMCPY(pData_lcl, &CardinalPointCalibration0xFB13.Status, sizeof(CardinalPointCalibration0xFB13.Status));
		pData_lcl += sizeof(CardinalPointCalibration0xFB13.Status);
		MEMCPY(pData_lcl, &CardinalPointCalibration0xFB13.Response, sizeof(CardinalPointCalibration0xFB13.Response));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 170)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &CardinalPointCalibration0xFB13.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &CardinalPointCalibration0xFB13.Command, dataSize);
		break;
		case 2:
		MEMCPY(pData, &CardinalPointCalibration0xFB13.Status, dataSize);
		break;
		case 3:
		MEMCPY(pData, &CardinalPointCalibration0xFB13.Response, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFBF0(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFBF0)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &DeviceReset0xFBF0.u16SubIndex0, sizeof(DeviceReset0xFBF0.u16SubIndex0));
		pData_lcl += sizeof(DeviceReset0xFBF0.u16SubIndex0);
		MEMCPY(pData_lcl, &DeviceReset0xFBF0.ResetCommand, sizeof(DeviceReset0xFBF0.ResetCommand));
		pData_lcl += sizeof(DeviceReset0xFBF0.ResetCommand);
		MEMCPY(pData_lcl, &DeviceReset0xFBF0.ResetStatus, sizeof(DeviceReset0xFBF0.ResetStatus));
		pData_lcl += sizeof(DeviceReset0xFBF0.ResetStatus);
		MEMCPY(pData_lcl, &DeviceReset0xFBF0.ResetResponse, sizeof(DeviceReset0xFBF0.ResetResponse));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &DeviceReset0xFBF0.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &DeviceReset0xFBF0.ResetCommand, dataSize);
		break;
		case 2:
		MEMCPY(pData, &DeviceReset0xFBF0.ResetStatus, dataSize);
		break;
		case 3:
		MEMCPY(pData, &DeviceReset0xFBF0.ResetResponse, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFBF2(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFBF2)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &StoreParam0xFBF2.u16SubIndex0, sizeof(StoreParam0xFBF2.u16SubIndex0));
		pData_lcl += sizeof(StoreParam0xFBF2.u16SubIndex0);
		MEMCPY(pData_lcl, &StoreParam0xFBF2.ResetCommand, sizeof(StoreParam0xFBF2.ResetCommand));
		pData_lcl += sizeof(StoreParam0xFBF2.ResetCommand);
		MEMCPY(pData_lcl, &StoreParam0xFBF2.ResetStatus, sizeof(StoreParam0xFBF2.ResetStatus));
		pData_lcl += sizeof(StoreParam0xFBF2.ResetStatus);
		MEMCPY(pData_lcl, &StoreParam0xFBF2.ResetResponse, sizeof(StoreParam0xFBF2.ResetResponse));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &StoreParam0xFBF2.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &StoreParam0xFBF2.ResetCommand, dataSize);
		break;
		case 2:
		MEMCPY(pData, &StoreParam0xFBF2.ResetStatus, dataSize);
		break;
		case 3:
		MEMCPY(pData, &StoreParam0xFBF2.ResetResponse, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFBF3(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFBF3)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &CalcChecksum0xFBF3.u16SubIndex0, sizeof(CalcChecksum0xFBF3.u16SubIndex0));
		pData_lcl += sizeof(CalcChecksum0xFBF3.u16SubIndex0);
		MEMCPY(pData_lcl, &CalcChecksum0xFBF3.ResetCommand, sizeof(CalcChecksum0xFBF3.ResetCommand));
		pData_lcl += sizeof(CalcChecksum0xFBF3.ResetCommand);
		MEMCPY(pData_lcl, &CalcChecksum0xFBF3.ResetStatus, sizeof(CalcChecksum0xFBF3.ResetStatus));
		pData_lcl += sizeof(CalcChecksum0xFBF3.ResetStatus);
		MEMCPY(pData_lcl, &CalcChecksum0xFBF3.ResetResponse, sizeof(CalcChecksum0xFBF3.ResetResponse));
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
		MEMCPY(pData, &CalcChecksum0xFBF3.u16SubIndex0, dataSize);
		break;
		case 1:
		MEMCPY(pData, &CalcChecksum0xFBF3.ResetCommand, dataSize);
		break;
		case 2:
		MEMCPY(pData, &CalcChecksum0xFBF3.ResetStatus, dataSize);
		break;
		case 3:
		MEMCPY(pData, &CalcChecksum0xFBF3.ResetResponse, dataSize);
		break;
		default:
		return ABORTIDX_UNSUPPORTED_ACCESS;
		break;
	};
	return 0;
}

/**
\param     index                    index of the requested object.
\param     subindex                subindex of the requested object.
\param    dataSize                received data size of the SDO Download
\param    pData                    Pointer to the buffer where the written data can be copied from
\param    bCompleteAccess    Indicates if a complete write of all subindices of the
object shall be done or not

\return    result of the write operation (0 (success) or an abort code (ABORTIDX_.... defined in
sdosrv.h))

\brief    This function reads the object 0x10F8
*/
UINT8 Read0xFBF4(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	
	if (index != 0xFBF4)
	{
		return ABORTIDX_PARAM_IS_INCOMPATIBLE;
	}
	
	if (bCompleteAccess)
	{
		UINT8 *pData_lcl = (UINT8 *)pData;
		MEMCPY(pData_lcl, &LoadParam0xFBF4.u16SubIndex0, sizeof(LoadParam0xFBF4.u16SubIndex0));
		pData_lcl += sizeof(LoadParam0xFBF4.u16SubIndex0);
		MEMCPY(pData_lcl, &LoadParam0xFBF4.ResetCommand, sizeof(LoadParam0xFBF4.ResetCommand));
		pData_lcl += sizeof(LoadParam0xFBF4.ResetCommand);
		MEMCPY(pData_lcl, &LoadParam0xFBF4.ResetStatus, sizeof(LoadParam0xFBF4.ResetStatus));
		pData_lcl += sizeof(LoadParam0xFBF4.ResetStatus);
		MEMCPY(pData_lcl, &LoadParam0xFBF4.ResetResponse, sizeof(LoadParam0xFBF4.ResetResponse));					
		return 0;
	}

	if (subindex > 3)
	{
		return ABORTIDX_SUBINDEX_NOT_EXISTING;
	}

	if (dataSize > 9)
	{
		return ABORTIDX_PARAM_LENGTH_TOO_LONG;
	}

	switch (subindex) {
		case 0:
			MEMCPY(pData, &LoadParam0xFBF4.u16SubIndex0, dataSize);	
			break;
		case 1:
			MEMCPY(pData, &LoadParam0xFBF4.ResetCommand, dataSize);
			break;		
		case 2:
			MEMCPY(pData, &LoadParam0xFBF4.ResetStatus, dataSize);
			break;		
		case 3:
			MEMCPY(pData, &LoadParam0xFBF4.ResetResponse, dataSize);
			break;		
		default:
			return ABORTIDX_UNSUPPORTED_ACCESS;
			break;
	};
	return 0;
}

#define STATION_ALIAS_REGISTER	0x0012
/************************************************************************/
/*                                                                      */
/************************************************************************/
void UpdateAliasRegister(void) {
//	static bool configComplete = false;
//	
//	if (!configComplete) {
//		if (SwitchesCheckMacId()) {
//			UINT16 address = SwitchesGetMacId();
//			HW_EscWriteWord( address, STATION_ALIAS_REGISTER);
//			
//			configComplete = true;			
//		}
//	}
}

#define RESET_TIMEOUT_MS	500
extern void reset_now(void);
UINT8 resetDevice = 0;
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ServiceResetRequest(void) {
//	static int countdownms = RESET_TIMEOUT_MS;
//	
//	if (resetDevice > 0) {
//		countdownms--;
//		if (countdownms <= 0) {
//			reset_now();
//		}
//	} else {
//		countdownms = RESET_TIMEOUT_MS;	// RESET_TIMEOUT_MS timeout before resetting
//	}
	
}

#if USE_DEFAULT_MAIN
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This is the main function

*////////////////////////////////////////////////////////////////////////////////////////
#if _PIC24
int main(void)
#else
void main(void)
#endif
{
    /* initialize the Hardware and the EtherCAT Slave Controller */
#if FC1100_HW
    if(HW_Init())
    {
        HW_Release();
        return;
    }
#else
    HW_Init();
#endif
    MainInit();

    bRunApplication = TRUE;
    do
    {
        MainLoop();
        
    } while (bRunApplication == TRUE);

    HW_Release();
#if _PIC24
    return 0;
#endif
}
#endif //#if USE_DEFAULT_MAIN
/** @} */

#endif          // ifdef ECAT
