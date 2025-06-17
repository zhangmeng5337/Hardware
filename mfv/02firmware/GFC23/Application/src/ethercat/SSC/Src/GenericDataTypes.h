/*******************************************************************

                  Generic Type Definitions

********************************************************************
 FileName:        GenericTypeDefs.h
 Dependencies:    None
 Processor:       PIC10, PIC12, PIC16, PIC18, PIC24, dsPIC, PIC32
 Compiler:        MPLAB C Compilers for PIC18, PIC24, dsPIC, & PIC32
                  Hi-Tech PICC PRO, Hi-Tech PICC18 PRO
 Company:         Microchip Technology Inc.

 Software License Agreement

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") is intended and supplied to you, the Company's
 customer, for use solely and exclusively with products manufactured
 by the Company.

 The software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.1   09/11/06     Add base signed types
  1.2   02/28/07     Add QWORD, LONGLONG, QWORD_VAL
  1.3   02/06/08     Add def's for PIC32
  1.4   08/08/08     Remove LSB/MSB Macros, adopted by Peripheral lib
  1.5   08/14/08     Simplify file header
  Draft 2.0   07/13/09     Updated for new release of coding standards
*******************************************************************/

#ifndef __GENERIC_TYPE_DEFS_H_
#define __GENERIC_TYPE_DEFS_H_

#include "ecat_def.h"

/* Specify an extension for GCC based compilers */
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif

typedef union
{
	UINT16 Val;
	UINT8 v[2];
	struct
	{
		UINT8 LB;
		UINT8 HB;
	} byte;
	struct
	{
		__EXTENSION UINT8 b0:1;
		__EXTENSION UINT8 b1:1;
		__EXTENSION UINT8 b2:1;
		__EXTENSION UINT8 b3:1;
		__EXTENSION UINT8 b4:1;
		__EXTENSION UINT8 b5:1;
		__EXTENSION UINT8 b6:1;
		__EXTENSION UINT8 b7:1;
		__EXTENSION UINT8 b8:1;
		__EXTENSION UINT8 b9:1;
		__EXTENSION UINT8 b10:1;
		__EXTENSION UINT8 b11:1;
		__EXTENSION UINT8 b12:1;
		__EXTENSION UINT8 b13:1;
		__EXTENSION UINT8 b14:1;
		__EXTENSION UINT8 b15:1;
	} bits;
} UINT16_VAL, UINT16_BITS;

typedef union
{
	UINT32 Val;
	UINT16 w[2];
	UINT8  v[4];
	struct
	{
		UINT16 LW;
		UINT16 HW;
	} word;
	struct
	{
		UINT8 LB;
		UINT8 HB;
		UINT8 UB;
		UINT8 MB;
	} byte;
	struct
	{
		UINT16_VAL low;
		UINT16_VAL high;
	}wordUnion;
	struct
	{
		__EXTENSION UINT8 b0:1;
		__EXTENSION UINT8 b1:1;
		__EXTENSION UINT8 b2:1;
		__EXTENSION UINT8 b3:1;
		__EXTENSION UINT8 b4:1;
		__EXTENSION UINT8 b5:1;
		__EXTENSION UINT8 b6:1;
		__EXTENSION UINT8 b7:1;
		__EXTENSION UINT8 b8:1;
		__EXTENSION UINT8 b9:1;
		__EXTENSION UINT8 b10:1;
		__EXTENSION UINT8 b11:1;
		__EXTENSION UINT8 b12:1;
		__EXTENSION UINT8 b13:1;
		__EXTENSION UINT8 b14:1;
		__EXTENSION UINT8 b15:1;
		__EXTENSION UINT8 b16:1;
		__EXTENSION UINT8 b17:1;
		__EXTENSION UINT8 b18:1;
		__EXTENSION UINT8 b19:1;
		__EXTENSION UINT8 b20:1;
		__EXTENSION UINT8 b21:1;
		__EXTENSION UINT8 b22:1;
		__EXTENSION UINT8 b23:1;
		__EXTENSION UINT8 b24:1;
		__EXTENSION UINT8 b25:1;
		__EXTENSION UINT8 b26:1;
		__EXTENSION UINT8 b27:1;
		__EXTENSION UINT8 b28:1;
		__EXTENSION UINT8 b29:1;
		__EXTENSION UINT8 b30:1;
		__EXTENSION UINT8 b31:1;
	} bits;
} UINT32_VAL;

typedef union
{
    UINT64 Val;
    UINT32 d[2];
    UINT16 w[4];
    UINT8 v[8];
    struct
    {
        UINT32 LD;
        UINT32 HD;
    } dword;
    struct
    {
        UINT16 LW;
        UINT16 HW;
        UINT16 UW;
        UINT16 MW;
    } word;
    struct
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
        __EXTENSION UINT8 b24:1;
        __EXTENSION UINT8 b25:1;
        __EXTENSION UINT8 b26:1;
        __EXTENSION UINT8 b27:1;
        __EXTENSION UINT8 b28:1;
        __EXTENSION UINT8 b29:1;
        __EXTENSION UINT8 b30:1;
        __EXTENSION UINT8 b31:1;
        __EXTENSION UINT8 b32:1;
        __EXTENSION UINT8 b33:1;
        __EXTENSION UINT8 b34:1;
        __EXTENSION UINT8 b35:1;
        __EXTENSION UINT8 b36:1;
        __EXTENSION UINT8 b37:1;
        __EXTENSION UINT8 b38:1;
        __EXTENSION UINT8 b39:1;
        __EXTENSION UINT8 b40:1;
        __EXTENSION UINT8 b41:1;
        __EXTENSION UINT8 b42:1;
        __EXTENSION UINT8 b43:1;
        __EXTENSION UINT8 b44:1;
        __EXTENSION UINT8 b45:1;
        __EXTENSION UINT8 b46:1;
        __EXTENSION UINT8 b47:1;
        __EXTENSION UINT8 b48:1;
        __EXTENSION UINT8 b49:1;
        __EXTENSION UINT8 b50:1;
        __EXTENSION UINT8 b51:1;
        __EXTENSION UINT8 b52:1;
        __EXTENSION UINT8 b53:1;
        __EXTENSION UINT8 b54:1;
        __EXTENSION UINT8 b55:1;
        __EXTENSION UINT8 b56:1;
        __EXTENSION UINT8 b57:1;
        __EXTENSION UINT8 b58:1;
        __EXTENSION UINT8 b59:1;
        __EXTENSION UINT8 b60:1;
        __EXTENSION UINT8 b61:1;
        __EXTENSION UINT8 b62:1;
        __EXTENSION UINT8 b63:1;
    } bits;
} UINT64_VAL;

#endif
