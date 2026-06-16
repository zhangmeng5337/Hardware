/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup ETG5003_2020_MFC ETG5003_2020_MFC
 * @{
 */

/**
\file ETG5003_2020_MFC.h
\brief ETG5003_2020_MFC function prototypes and defines

\version 1.0.0.11
 */

 
 #ifndef _ETG5003_2020__MFC_H_
#define _ETG5003_2020__MFC_H_


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "ecatappl.h"

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

#endif //_ETG5003_2020__MFC_H_

//include custom application object dictionary 
#include "ETG5003_2020_MFCObjects.h"


#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif


PROTO void APPL_Application(void);
#if EXPLICIT_DEVICE_ID
PROTO UINT16 APPL_GetDeviceID(void);
#endif

PROTO void   APPL_AckErrorInd(UINT16 stateTrans);
PROTO UINT16 APPL_StartMailboxHandler(void);
PROTO UINT16 APPL_StopMailboxHandler(void);
PROTO UINT16 APPL_StartInputHandler(UINT16 *pIntMask);
PROTO UINT16 APPL_StopInputHandler(void);
PROTO UINT16 APPL_StartOutputHandler(void);
PROTO UINT16 APPL_StopOutputHandler(void);

PROTO UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize);
PROTO void APPL_InputMapping(UINT16* pData);
PROTO void APPL_OutputMapping(UINT16* pData);

PROTO void UpdateAliasRegister(void);
PROTO void ServiceResetRequest(void);
PROTO void ecat_userobj_init (UINT8 init_type);

PROTO UINT16 FoE_Read(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData);
PROTO UINT16 FoE_ReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData);
PROTO UINT16 FoE_WriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing);
PROTO UINT16 FoE_Write(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password);

PROTO UINT16 ESC_EEPROM_Manager (void);

#undef PROTO
/** @}*/

