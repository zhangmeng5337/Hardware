/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup Bootloader Bootloader Sample
 * @{
 */

/**
\file bootloader.h
\author EthercatSSC@beckhoff.com

\version 5.12

<br>Changes to version - :<br>
V5.12: file created
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#ifndef _BOOTLOADER_APPL_H_
#define _BOOTLOADER_APPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

#endif //_BOOTLOADER_APPL_H_


#ifdef _BOOTLOADER_APPLICATION_
    #define PROTO
#else
    #define PROTO extern
#endif


PROTO void APPL_Application(void);

PROTO UINT16 APPL_GetDeviceID(void);

PROTO void   APPL_AckErrorInd(UINT16 stateTrans);
PROTO UINT16 APPL_StartMailboxHandler(void);
PROTO UINT16 APPL_StopMailboxHandler(void);

#undef PROTO
/** @}*/
