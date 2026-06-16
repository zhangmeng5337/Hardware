/**
* \addtogroup ETG5003_2020_MFC ETG5003_2020_MFC
* @{
*/

/**
\file ETG5003_2020_MFCObjects
\author ET9300Utilities.ApplicationHandler (Version 1.3.6.0) | EthercatSSC@beckhoff.com

\brief ETG5003_2020_MFC specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
#define PROTO
#else
#define PROTO extern
#endif
/******************************************************************************
*                    Object 0x1600 : Output mapping 0
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | Output mapping 0
* @{
* \brief Object 0x1600 (Output mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0x7003.1<br>
* SubIndex 2 - Reference to 0x7007.1<br>
* SubIndex 3 - Reference to 0x7008.1<br>
* SubIndex 4 - Reference to 0x7009.1<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0x7003.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x7009.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x7009.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex4 - Reference to 0x7008.1 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "1600 Default RxPDO-Map\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x7003.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x7009.1 */
UINT32 SI3; /* Subindex3 - Reference to 0x7009.2 */
UINT32 SI4; /* Subindex4 - Reference to 0x7008.1 */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 OutputMapping00x1600
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={4,0x70030120,0x70090108,0x70090220,0x70080120}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1601 : Output mapping 0
******************************************************************************/
/**
* \addtogroup 0x1601 0x1601 | Output mapping 0
* @{
* \brief Object 0x1601 (Output mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1601[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , (ACCESS_READ | ACCESS_WRITE_PREOP) },
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex5 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex6 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex7 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex8 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex9 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }}; /* Subindex10 */		 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1601[] = "1601 Flexible RxPDO-Map\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 */
UINT32 SI2; /* Subindex2 */
UINT32 SI3; /* Subindex3 */
UINT32 SI4; /* Subindex4 */
UINT32 SI5; /* Subindex5 */
UINT32 SI6; /* Subindex6 */
UINT32 SI7; /* Subindex7 */
UINT32 SI8; /* Subindex8 */
UINT32 SI9; /* Subindex9 */
UINT32 SI10; /* Subindex10 */
} OBJ_STRUCT_PACKED_END
TOBJ1601;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1601 OutputMapping00x1601
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1A00 : Input mapping 0
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | Input mapping 0
* @{
* \brief Object 0x1A00 (Input mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Reference to 0xF380<br>
* SubIndex 2 - Reference to 0x6000.1<br>
* SubIndex 3 - Reference to 0x6001.1<br>
* SubIndex 4 - Reference to 0x6002.1<br>
* SubIndex 5 - Reference to 0x6009.2<br>
* SubIndex 6 - Reference to 0x6009.1<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - Reference to 0xF380.0 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - Reference to 0x6000.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - Reference to 0x6001.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - Reference to 0x6002.1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 - Reference to 0x6009.2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex6 - Reference to 0x6009.1 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "1A00 Default TxPDO-Map\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - Reference to 0x6000.1 */
UINT32 SI2; /* Subindex2 - Reference to 0x6001.1 */
UINT32 SI3; /* Subindex3 - Reference to 0x6002.1 */
UINT32 SI4; /* Subindex4 - Reference to 0x6004.1 */
UINT32 SI5; /* Subindex5 - Reference to 0x6005.1 */
UINT32 SI6; /* Subindex6 - Reference to 0x6006.1 */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 InputMapping00x1A00
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={6,0xF3800008,0x60000120,0x60010120,0x60020120,0x60090220,0x60090120}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1A01 : Input mapping 0
******************************************************************************/
/**
* \addtogroup 0x1A01 0x1A01 | Input mapping 0
* @{
* \brief Object 0x1A01 (Input mapping 0) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A01[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , (ACCESS_READ | ACCESS_WRITE_PREOP) },
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex5 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex6 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex7 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex8 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }, /* Subindex9 */	
{ DEFTYPE_UNSIGNED32 , 0x20 , (ACCESS_READ | ACCESS_WRITE_PREOP) }}; /* Subindex10 */	

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A01[] = "1A01 Flexible TxPDO-Map\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 */
UINT32 SI2; /* Subindex2 */
UINT32 SI3; /* Subindex3 */
UINT32 SI4; /* Subindex4 */
UINT32 SI5; /* Subindex5 */
UINT32 SI6; /* Subindex6 */
UINT32 SI7; /* Subindex7 */
UINT32 SI8; /* Subindex8 */
UINT32 SI9; /* Subindex9 */
UINT32 SI10; /* Subindex10 */
} OBJ_STRUCT_PACKED_END
TOBJ1A01;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A01 InputMapping00x1A01
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x1C12 : SyncManager 2 assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | SyncManager 2 assignment
* @{
* \brief Object 0x1C12 (SyncManager 2 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "SyncManager 2 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[2];  /**< \brief Subindex 1 - 2 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x1600,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : SyncManager 3 assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | SyncManager 3 assignment
* @{
* \brief Object 0x1C13 (SyncManager 3 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "SyncManager 3 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[2];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x1A00,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6000 : Sensor: Flow (floating)
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | Sensor: Flow (floating)
* @{
* \brief Object 0x6000 (Sensor: Flow (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow Reading [REAL]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Flow Reading [REAL] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "Sensor: Flow (floating)\000"
"Flow Reading [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 FlowReadingREAL; /* Subindex1 - Flow Reading [REAL] */
} OBJ_STRUCT_PACKED_END
TOBJ6000;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6000 SensorFlowFloating0x6000
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6001 : Sensor: Pressure (floating)
******************************************************************************/
/**
* \addtogroup 0x6001 0x6001 | Sensor: Pressure (floating)
* @{
* \brief Object 0x6001 (Sensor: Pressure (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Pressure Reading [REAL]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Pressure Reading [REAL] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6001[] = "Sensor: Pressure (floating)\000"
"Pressure Reading [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 PressureReadingREAL; /* Subindex1 - Pressure Reading [REAL] */
} OBJ_STRUCT_PACKED_END
TOBJ6001;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6001 SensorPressureFloating0x6001
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6002 : Sensor: Temperature (floating)
******************************************************************************/
/**
* \addtogroup 0x6002 0x6002 | Sensor: Temperature (floating)
* @{
* \brief Object 0x6002 (Sensor: Temperature (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Temperature Reading [REAL]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6002[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Temperature Reading [REAL] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6002[] = "Sensor: Temperature (floating)\000"
"Temperature Reading [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 TemperatureReadingREAL; /* Subindex1 - Temperature Reading [REAL] */
} OBJ_STRUCT_PACKED_END
TOBJ6002;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6002 SensorTemperatureFloating0x6002
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6004 : Sensor: Flow (integer)
******************************************************************************/
/**
* \addtogroup 0x6004 0x6004 | Sensor: Flow (integer)
* @{
* \brief Object 0x6004 (Sensor: Flow (integer)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow Reading [INT]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6004[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Flow Reading [INT] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6004[] = "Sensor: Flow (integer)\000"
"Flow Reading [INT]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 FlowReadingINT; /* Subindex1 - Flow Reading [INT] */
} OBJ_STRUCT_PACKED_END
TOBJ6004;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6004 SensorFlowInteger0x6004
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6005 : Sensor: Pressure (integer)
******************************************************************************/
/**
* \addtogroup 0x6005 0x6005 | Sensor: Pressure (integer)
* @{
* \brief Object 0x6005 (Sensor: Pressure (integer)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Pressure Reading [INT]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6005[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Pressure Reading [INT] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6005[] = "Sensor: Pressure (integer)\000"
"Pressure Reading [INT]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 PressureReadingINT; /* Subindex1 - Pressure Reading [INT] */
} OBJ_STRUCT_PACKED_END
TOBJ6005;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6005 SensorPressureInteger0x6005
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6006 : Sensor: Temperature (integer)
******************************************************************************/
/**
* \addtogroup 0x6006 0x6006 | Sensor: Temperature (integer)
* @{
* \brief Object 0x6006 (Sensor: Temperature (integer)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Temperature Reading [INT]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6006[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex1 - Temperature Reading [INT] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6006[] = "Sensor: Temperature (integer)\000"
"Temperature Reading [INT]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 TemperatureReadingINT; /* Subindex1 - Temperature Reading [INT] */
} OBJ_STRUCT_PACKED_END
TOBJ6006;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6006 SensorTemperatureInteger0x6006
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6009 : Actuator
******************************************************************************/
/**
* \addtogroup 0x6009 0x6009 | Actuator
* @{
* \brief Object 0x6009 (Actuator) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Actuator Setpoint [REAL]<br>
* SubIndex 2 - Position Readback [REAL]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6009[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex1 - Actuator Setpoint [REAL] */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex2 - Position Readback [REAL] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6009[] = "Actuator\000"
"Actuator Setpoint [REAL]\000"
"Position Readback [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 ActuatorSetpointREAL; /* Subindex1 - Actuator Setpoint [REAL] */
REAL32 PositionReadbackREAL; /* Subindex2 - Position Readback [REAL] */
} OBJ_STRUCT_PACKED_END
TOBJ6009;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6009 Actuator0x6009
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={2,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x600F : Status
******************************************************************************/
/**
* \addtogroup 0x600F 0x600F | Status
* @{
* \brief Object 0x600F (Status) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Service in Progress<br>
* SubIndex 2 - Invalid Data Input<br>
* SubIndex 3 - Ramp Active<br>
* SubIndex 4 - Valve Control Mode (fully-open)<br>
* SubIndex 5 - Valve Control Mode (fully-closed)<br>
* SubIndex 6 - Valve Control Mode (Position Control)<br>
* SubIndex 7<br>
* SubIndex 8 does not exists<br>
* SubIndex 9 does not exists<br>
* SubIndex 10 does not exists<br>
* SubIndex 11 does not exists<br>
* SubIndex 12 does not exists<br>
* SubIndex 13 - new message in diagnosis history<br>
* SubIndex 14 - TxPdoState<br>
* SubIndex 15 - Input Cycle Counter<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x600F[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex1 - Service in Progress */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex2 - Invalid Data Input */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex3 - Ramp Active */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex4 - Valve Control Mode (fully-open) */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex5 - Valve Control Mode (fully-closed) */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex6 - Valve Control Mode (Position Control) */
{ DEFTYPE_NULL , 0x06 , 0x0000 }, /* Subindex7 */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex8 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex9 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex10 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex11 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex12 does not exists */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex13 - new message in diagnosis history */
{ DEFTYPE_BOOLEAN , 0x01 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }, /* Subindex14 - TxPdoState */
{ DEFTYPE_BIT2 , 0x02 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }}; /* Subindex15 - Input Cycle Counter */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x600F[] = "Status\000"
"Service in Progress\000"
"Invalid Data Input\000"
"Ramp Active\000"
"Valve Control Mode (fully-open)\000"
"Valve Control Mode (fully-closed)\000"
"Valve Control Mode (Position Control)\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"new message in diagnosis history\000"
"TxPdoState\000"
"Input Cycle Counter\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
BOOLEAN(ServiceInProgress); /* Subindex1 - Service in Progress */
BOOLEAN(InvalidDataInput); /* Subindex2 - Invalid Data Input */
BOOLEAN(RampActive); /* Subindex3 - Ramp Active */
BOOLEAN(ValveControlModeFullyOpen); /* Subindex4 - Valve Control Mode (fully-open) */
BOOLEAN(ValveControlModeFullyClosed); /* Subindex5 - Valve Control Mode (fully-closed) */
BOOLEAN(ValveControlModePositionControl); /* Subindex6 - Valve Control Mode (Position Control) */
ALIGN6(SI7) /* Subindex7 */
BOOLEAN(NewMessageInDiagnosisHistory); /* Subindex13 - new message in diagnosis history */
BOOLEAN(TxPdoState); /* Subindex14 - TxPdoState */
BIT2(InputCycleCounter); /* Subindex15 - Input Cycle Counter */
} OBJ_STRUCT_PACKED_END
TOBJ600F;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ600F Status0x600F
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={15,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7003 : Controller: Flow SP (floating)
******************************************************************************/
/**
* \addtogroup 0x7003 0x7003 | Controller: Flow SP (floating)
* @{
* \brief Object 0x7003 (Controller: Flow SP (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow SP [REAL]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7003[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | ACCESS_WRITE_OP | OBJACCESS_RXPDOMAPPING }}; /* Subindex1 - Flow SP [REAL] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7003[] = "Controller: Flow SP (floating)\000"
"Flow SP [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 FlowSPREAL; /* Subindex1 - Flow SP [REAL] */
} OBJ_STRUCT_PACKED_END
TOBJ7003;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7003 ControllerFlowSPFloating0x7003
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7007 : Controller: Flow SP (integer)
******************************************************************************/
/**
* \addtogroup 0x7007 0x7007 | Controller: Flow SP (integer)
* @{
* \brief Object 0x7007 (Controller: Flow SP (integer)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow SP [INT]<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7007[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER16 , 0x10 , ACCESS_READ | ACCESS_WRITE_OP | OBJACCESS_RXPDOMAPPING }}; /* Subindex1 - Flow SP [INT] */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7007[] = "Controller: Flow SP (integer)\000"
"Flow SP [INT]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT16 FlowSPINT; /* Subindex1 - Flow SP [INT] */
} OBJ_STRUCT_PACKED_END
TOBJ7007;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7007 ControllerFlowSPInteger0x7007
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x0000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7008 : Controller: Ramp Time
******************************************************************************/
/**
* \addtogroup 0x7008 0x7008 | Controller: Ramp Time
* @{
* \brief Object 0x7008 (Controller: Ramp Time) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Ramp Time<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7008[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_OP | OBJACCESS_RXPDOMAPPING }}; /* Subindex1 - Ramp Time */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7008[] = "Controller: Ramp Time\000"
"Ramp Time\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 RampTime; /* Subindex1 - Ramp Time */
} OBJ_STRUCT_PACKED_END
TOBJ7008;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7008 ControllerRampTime0x7008
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x00000000}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7009 : Actuator
******************************************************************************/
/**
* \addtogroup 0x7009 0x7009 | Actuator
* @{
* \brief Object 0x7009 (Actuator) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Actuator Control<br>
* SubIndex 2 - Actuator <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7009[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_OP | OBJACCESS_RXPDOMAPPING }, /* Subindex1 - Actuator Control */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ | ACCESS_WRITE_OP | OBJACCESS_RXPDOMAPPING }}; /* Subindex2 - Actuator Position */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x7009[] = "Actuator\000"
"Actuator Control\000"
"Actuator Position SP [REAL]\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 ActuatorControl; /* Subindex1 - Actuator Control */
REAL32 ActuatorPositionSPREAL; /* Subindex2 - Actuator Position */
} OBJ_STRUCT_PACKED_END
TOBJ7009;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7009 Actuator0x7009
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={2,0x00, 0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x700F : Status
******************************************************************************/
/**
* \addtogroup 0x700F 0x700F | Status
* @{
* \brief Object 0x700F (Status) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2 does not exists<br>
* SubIndex 3 does not exists<br>
* SubIndex 4 does not exists<br>
* SubIndex 5 does not exists<br>
* SubIndex 6 does not exists<br>
* SubIndex 7 does not exists<br>
* SubIndex 8 does not exists<br>
* SubIndex 9 does not exists<br>
* SubIndex 10 does not exists<br>
* SubIndex 11 does not exists<br>
* SubIndex 12 does not exists<br>
* SubIndex 13 does not exists<br>
* SubIndex 14 does not exists<br>
* SubIndex 15 - Output Cycle Counter<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x700F[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_NULL , 0x0E , 0x0000 }, /* Subindex1 */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex2 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex3 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex4 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex5 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex6 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex7 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex8 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex9 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex10 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex11 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex12 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex13 does not exists */
{ DEFTYPE_NULL , 0x00 , 0x0000 }, /* Subindex14 does not exists */
{ DEFTYPE_BIT2 , 0x02 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }}; /* Subindex15 - Output Cycle Counter */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x700F[] = "Status\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"\000"
"Output Cycle Counter\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
ALIGN14(SI1) /* Subindex1 */
BIT2(OutputCycleCounter); /* Subindex15 - Output Cycle Counter */
} OBJ_STRUCT_PACKED_END
TOBJ700F;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ700F Status0x700F
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={15,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8000 : Sensor: Flow (floating)
******************************************************************************/
/**
* \addtogroup 0x8000 0x8000 | Sensor: Flow (floating)
* @{
* \brief Object 0x8000 (Sensor: Flow (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow Data Unit<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_BACKUP }}; /* Subindex1 - Flow Data Unit */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8000[] = "Sensor: Flow (floating)\000"
"Flow Data Unit\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 FlowDataUnit; /* Subindex1 - Flow Data Unit */
} OBJ_STRUCT_PACKED_END
TOBJ8000;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8000 SensorFlowFloating0x8000
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8001 : Sensor: Pressure (floating)
******************************************************************************/
/**
* \addtogroup 0x8001 0x8001 | Sensor: Pressure (floating)
* @{
* \brief Object 0x8001 (Sensor: Pressure (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Pressure Data Unit<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_BACKUP }}; /* Subindex1 - Pressure Data Unit */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8001[] = "Sensor: Pressure (floating)\000"
"Pressure Data Unit\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 PressureDataUnit; /* Subindex1 - Pressure Data Unit */
} OBJ_STRUCT_PACKED_END
TOBJ8001;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8001 SensorPressureFloating0x8001
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8002 : Sensor: Temperature (floating)
******************************************************************************/
/**
* \addtogroup 0x8002 0x8002 | Sensor: Temperature (floating)
* @{
* \brief Object 0x8002 (Sensor: Temperature (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Temperature Data Unit<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8002[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_BACKUP }}; /* Subindex1 - Temperature Data Unit */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8002[] = "Sensor: Temperature (floating)\000"
"Temperature Data Unit\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 TemperatureDataUnit; /* Subindex1 - Temperature Data Unit */
} OBJ_STRUCT_PACKED_END
TOBJ8002;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8002 SensorTemperatureFloating0x8002
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8008 : Controller: Ramp Time
******************************************************************************/
/**
* \addtogroup 0x8008 0x8008 | Controller: Ramp Time
* @{
* \brief Object 0x8008 (Controller: Ramp Time) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Max Ramp Time<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8008[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_BACKUP }}; /* Subindex1 - Max Ramp Time */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8008[] = "Controller: Ramp Time\000"
"Max Ramp Time\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 MaxRampTime; /* Subindex1 - Max Ramp Time */
} OBJ_STRUCT_PACKED_END
TOBJ8008;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8008 ControllerRampTime0x8008
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x0000EA60}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8009 : Actuator
******************************************************************************/
/**
* \addtogroup 0x8009 0x8009 | Actuator
* @{
* \brief Object 0x8009 (Actuator) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Safe State<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x8009[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READ | ACCESS_WRITE_PREOP | OBJACCESS_BACKUP }}; /* Subindex1 - Safe State */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x8009[] = "Actuator\000"
"Safe State\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 SafeState; /* Subindex1 - Safe State */
} OBJ_STRUCT_PACKED_END
TOBJ8009;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ8009 Actuator0x8009
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x00}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x800A : Active Gas Calibration Index
******************************************************************************/
/**
* \addtogroup 0x800A 0x800A | Active Gas Calibration Index
* @{
* \brief Object 0x800A (Active Gas Calibration Index) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x800A[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE | OBJACCESS_BACKUP }}; /* Subindex1 - Index */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x800A[] = "Active Gas Calibration Index\000"
"Index\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Index; /* Subindex1 - Index */
} OBJ_STRUCT_PACKED_END
TOBJ800A;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ800A ActiveGasCalibrationIndex0x800A
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0x01}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x9000 : Sensor: Flow (floating)
******************************************************************************/
/**
* \addtogroup 0x9000 0x9000 | Sensor: Flow (floating)
* @{
* \brief Object 0x9000 (Sensor: Flow (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Flow Reading Zero Offset<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex1 - Flow Reading Zero Offset */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9000[] = "Sensor: Flow (floating)\000"
"Flow Reading Zero Offset\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 FlowReadingZeroOffset; /* Subindex1 - Flow Reading Zero Offset */
} OBJ_STRUCT_PACKED_END
TOBJ9000;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ9000 SensorFlowFloating0x9000
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x9001 : Sensor: Pressure (floating)
******************************************************************************/
/**
* \addtogroup 0x9001 0x9001 | Sensor: Pressure (floating)
* @{
* \brief Object 0x9001 (Sensor: Pressure (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Pressure Sensor Full Scale<br>
* SubIndex 2 - Pressure Reading Zero Offset<br>
* SubIndex 3 - MFC Pressure High Threshold Alarm<br>
* SubIndex 4 - MFC Pressure Low Threshold Alarm<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex1 - Pressure Sensor Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex2 - Pressure Reading Zero Offset */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex3 - MFC Pressure High Threshold Alarm */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex4 - MFC Pressure Low Threshold Alarm */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9001[] = "Sensor: Pressure (floating)\000"
"Pressure Sensor Full Scale\000"
"Pressure Reading Zero Offset\000"
"MFC Pressure High Threshold Alarm\000"
"MFC Pressure Low Threshold Alarm\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 PressureSensorFullScale; /* Subindex1 - Pressure Sensor Full Scale */
REAL32 PressureReadingZeroOffset; /* Subindex2 - Pressure Reading Zero Offset */
REAL32 MFCPressureHighThresholdAlarm; /* Subindex3 - MFC Pressure High Threshold Alarm */
REAL32 MFCPressureLowThresholdAlarm; /* Subindex4 - MFC Pressure Low Threshold Alarm */
} OBJ_STRUCT_PACKED_END
TOBJ9001;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ9001 SensorPressureFloating0x9001
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={4,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x9002 : Sensor: Temperature (floating)
******************************************************************************/
/**
* \addtogroup 0x9002 0x9002 | Sensor: Temperature (floating)
* @{
* \brief Object 0x9002 (Sensor: Temperature (floating)) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Temperature Sensor Full Scale<br>
* SubIndex 2 - MFC Temp High Threshold Alarm<br>
* SubIndex 3 - MFC Temp Low Threshold Alarm<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9002[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex1 - Temperature Sensor Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex2 - MFC Temp High Threshold Alarm */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex3 - MFC Temp Low Threshold Alarm */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9002[] = "Sensor: Temperature (floating)\000"
"Temperature Sensor Full Scale\000"
"MFC Temp High Threshold Alarm\000"
"MFC Temp Low Threshold Alarm\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 TemperatureSensorFullScale; /* Subindex1 - Temperature Sensor Full Scale */
REAL32 MFCTempHighThresholdAlarm; /* Subindex2 - MFC Temp High Threshold Alarm */
REAL32 MFCTempLowThresholdAlarm; /* Subindex3 - MFC Temp Low Threshold Alarm */
} OBJ_STRUCT_PACKED_END
TOBJ9002;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ9002 SensorTemperatureFloating0x9002
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x9008 : Controller: Ramp Time
******************************************************************************/
/**
* \addtogroup 0x9008 0x9008 | Controller: Ramp Time
* @{
* \brief Object 0x9008 (Controller: Ramp Time) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Min Ramp Time<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9008[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex1 - Min Ramp Time */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9008[] = "Controller: Ramp Time\000"
"Min Ramp Time\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 MinRampTime; /* Subindex1 - Min Ramp Time */
} OBJ_STRUCT_PACKED_END
TOBJ9008;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ9008 ControllerRampTime0x9008
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x9009 : Actuator
******************************************************************************/
/**
* \addtogroup 0x9009 0x9009 | Actuator
* @{
* \brief Object 0x9009 (Actuator) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Valve Resolution<br>
* SubIndex 2 - Stroke Limits<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x9009[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex1 - Valve Resolution */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }}; /* Subindex2 - Stroke Limits */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x9009[] = "Actuator\000"
"Valve Resolution\000"
"Stroke Limits\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
REAL32 ValveResolution; /* Subindex1 - Valve Resolution */
REAL32 StrokeLimits; /* Subindex2 - Stroke Limits */
} OBJ_STRUCT_PACKED_END
TOBJ9009;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ9009 Actuator0x9009
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={2,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x900A : Gas Parameter Instance 1
******************************************************************************/
/**
* \addtogroup 0x900A 0x900A | Gas Parameter Instance 1
* @{
* \brief Object 0x900A (Gas Parameter Instance 1) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Gas Calibration Index<br>
* SubIndex 2 - Gas Number<br>
* SubIndex 3 - Gas Symbol<br>
* SubIndex 4 - Gas Name<br>
* SubIndex 5 - Minimum Full Scale<br>
* SubIndex 6 - Nominal Full Scale<br>
* SubIndex 7 - Configured Full Scale Range<br>
* SubIndex 8 - Minimum Flow SP<br>
* SubIndex 9 - Maximum Flow SP<br>
* SubIndex 10 - Device Bin Number<br>
* SubIndex 11 - Gas Calibration file revision number<br>
* SubIndex 12 - Revision of supported gas table<br>
* SubIndex 13 - Date of Factory Calibration<br>
* SubIndex 14 - Date of Last Calibration<br>
* SubIndex 15 - Cardinal Points Array Size<br>
* SubIndex 16 - Cardinal Points<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x900A[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex1 - Gas Calibration Index */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Gas Number */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex3 - Gas Symbol */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex4 - Gas Name */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex5 - Minimum Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex6 - Nominal Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex7 - Configured Full Scale Range */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex8 - Minimum Flow SP */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex9 - Maximum Flow SP */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex10 - Device Bin Number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex11 - Gas Calibration file revision number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex12 - Revision of supported gas table */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex13 - Date of Factory Calibration */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex14 - Date of Last Calibration */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex15 - Cardinal Points Array Size */
{ DEFTYPE_OCTETSTRING , 0x500 , ACCESS_READ }}; /* Subindex16 - Cardinal Points */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x900A[] = "Gas Parameter Instance 1\000"
"Gas Calibration Index\000"
"Gas Number\000"
"Gas Symbol\000"
"Gas Name\000"
"Minimum Full Scale\000"
"Nominal Full Scale\000"
"Configured Full Scale Range\000"
"Minimum Flow SP\000"
"Maximum Flow SP\000"
"Device Bin Number\000"
"Gas Calibration file revision number\000"
"Revision of supported gas table\000"
"Date of Factory Calibration\000"
"Date of Last Calibration\000"
"Cardinal Points Array Size\000"
"Cardinal Points\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 GasCalibrationIndex; /* Subindex1 - Gas Calibration Index */
UINT16 GasNumber; /* Subindex2 - Gas Number */
CHAR GasSymbol[128]; /* Subindex3 - Gas Symbol */
CHAR GasName[128]; /* Subindex4 - Gas Name */
REAL32 MinimumFullScale; /* Subindex5 - Minimum Full Scale */
REAL32 NominalFullScale; /* Subindex6 - Nominal Full Scale */
REAL32 ConfiguredFullScaleRange; /* Subindex7 - Configured Full Scale Range */
REAL32 MinimumFlowSP; /* Subindex8 - Minimum Flow SP */
REAL32 MaximumFlowSP; /* Subindex9 - Maximum Flow SP */
UINT16 DeviceBinNumber; /* Subindex10 - Device Bin Number */
UINT16 GasCalibrationFileRevisionNumber; /* Subindex11 - Gas Calibration file revision number */
UINT16 RevisionOfSupportedGasTable; /* Subindex12 - Revision of supported gas table */
CHAR DateOfFactoryCalibration[10]; /* Subindex13 - Date of Factory Calibration */
CHAR DateOfLastCalibration[10]; /* Subindex14 - Date of Last Calibration */
UINT8 CardinalPointsArraySize; /* Subindex15 - Cardinal Points Array Size */
UINT8 CardinalPoints[160]; /* Subindex16 - Cardinal Points */
} OBJ_STRUCT_PACKED_END
TOBJ900AE;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ900AE GasParameterInstance10x900A
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={16,0x0001,0x000D,"N2\000","Nitrogen\000",0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x900B : Gas Parameter Instance 2
******************************************************************************/
/**
* \addtogroup 0x900B 0x900B | Gas Parameter Instance 2
* @{
* \brief Object 0x900B (Gas Parameter Instance 2) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Gas Calibration Index<br>
* SubIndex 2 - Gas Number<br>
* SubIndex 3 - Gas Symbol<br>
* SubIndex 4 - Gas Name<br>
* SubIndex 5 - Minimum Full Scale<br>
* SubIndex 6 - Nominal Full Scale<br>
* SubIndex 7 - Configured Full Scale Range<br>
* SubIndex 8 - Minimum Flow SP<br>
* SubIndex 9 - Maximum Flow SP<br>
* SubIndex 10 - Device Bin Number<br>
* SubIndex 11 - Gas Calibration file revision number<br>
* SubIndex 12 - Revision of supported gas table<br>
* SubIndex 13 - Date of Factory Calibration<br>
* SubIndex 14 - Date of Last Calibration<br>
* SubIndex 15 - Cardinal Points Array Size<br>
* SubIndex 16 - Cardinal Points<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x900B[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex1 - Gas Calibration Index */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Gas Number */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex3 - Gas Symbol */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex4 - Gas Name */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex5 - Minimum Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex6 - Nominal Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex7 - Configured Full Scale Range */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex8 - Minimum Flow SP */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex9 - Maximum Flow SP */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex10 - Device Bin Number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex11 - Gas Calibration file revision number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex12 - Revision of supported gas table */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex13 - Date of Factory Calibration */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex14 - Date of Last Calibration */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex15 - Cardinal Points Array Size */
{ DEFTYPE_OCTETSTRING , 0x500 , ACCESS_READ }}; /* Subindex16 - Cardinal Points */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x900B[] = "Gas Parameter Instance 2\000"
"Gas Calibration Index\000"
"Gas Number\000"
"Gas Symbol\000"
"Gas Name\000"
"Minimum Full Scale\000"
"Nominal Full Scale\000"
"Configured Full Scale Range\000"
"Minimum Flow SP\000"
"Maximum Flow SP\000"
"Device Bin Number\000"
"Gas Calibration file revision number\000"
"Revision of supported gas table\000"
"Date of Factory Calibration\000"
"Date of Last Calibration\000"
"Cardinal Points Array Size\000"
"Cardinal Points\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO TOBJ900AE GasParameterInstance20x900B
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={16,0x0000,0x000D,"N2\000","Nitrogen\000",0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x900C : Gas Parameter Instance 3
******************************************************************************/
/**
* \addtogroup 0x900C 0x900C | Gas Parameter Instance 3
* @{
* \brief Object 0x900C (Gas Parameter Instance 3) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Gas Calibration Index<br>
* SubIndex 2 - Gas Number<br>
* SubIndex 3 - Gas Symbol<br>
* SubIndex 4 - Gas Name<br>
* SubIndex 5 - Minimum Full Scale<br>
* SubIndex 6 - Nominal Full Scale<br>
* SubIndex 7 - Configured Full Scale Range<br>
* SubIndex 8 - Minimum Flow SP<br>
* SubIndex 9 - Maximum Flow SP<br>
* SubIndex 10 - Device Bin Number<br>
* SubIndex 11 - Gas Calibration file revision number<br>
* SubIndex 12 - Revision of supported gas table<br>
* SubIndex 13 - Date of Factory Calibration<br>
* SubIndex 14 - Date of Last Calibration<br>
* SubIndex 15 - Cardinal Points Array Size<br>
* SubIndex 16 - Cardinal Points<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x900C[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex1 - Gas Calibration Index */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Gas Number */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex3 - Gas Symbol */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex4 - Gas Name */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex5 - Minimum Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex6 - Nominal Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex7 - Configured Full Scale Range */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex8 - Minimum Flow SP */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex9 - Maximum Flow SP */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex10 - Device Bin Number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex11 - Gas Calibration file revision number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex12 - Revision of supported gas table */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex13 - Date of Factory Calibration */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex14 - Date of Last Calibration */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex15 - Cardinal Points Array Size */
{ DEFTYPE_OCTETSTRING , 0x500 , ACCESS_READ }}; /* Subindex16 - Cardinal Points */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x900C[] = "Gas Parameter Instance 3\000"
"Gas Calibration Index\000"
"Gas Number\000"
"Gas Symbol\000"
"Gas Name\000"
"Minimum Full Scale\000"
"Nominal Full Scale\000"
"Configured Full Scale Range\000"
"Minimum Flow SP\000"
"Maximum Flow SP\000"
"Device Bin Number\000"
"Gas Calibration file revision number\000"
"Revision of supported gas table\000"
"Date of Factory Calibration\000"
"Date of Last Calibration\000"
"Cardinal Points Array Size\000"
"Cardinal Points\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO TOBJ900AE GasParameterInstance30x900C
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={16,0x0000,0x000D,"N2\000","Nitrogen\000",0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x900D : Gas Parameter Instance 4
******************************************************************************/
/**
* \addtogroup 0x900D 0x900D | Gas Parameter Instance 4
* @{
* \brief Object 0x900D (Gas Parameter Instance 4) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Gas Calibration Index<br>
* SubIndex 2 - Gas Number<br>
* SubIndex 3 - Gas Symbol<br>
* SubIndex 4 - Gas Name<br>
* SubIndex 5 - Minimum Full Scale<br>
* SubIndex 6 - Nominal Full Scale<br>
* SubIndex 7 - Configured Full Scale Range<br>
* SubIndex 8 - Minimum Flow SP<br>
* SubIndex 9 - Maximum Flow SP<br>
* SubIndex 10 - Device Bin Number<br>
* SubIndex 11 - Gas Calibration file revision number<br>
* SubIndex 12 - Revision of supported gas table<br>
* SubIndex 13 - Date of Factory Calibration<br>
* SubIndex 14 - Date of Last Calibration<br>
* SubIndex 15 - Cardinal Points Array Size<br>
* SubIndex 16 - Cardinal Points<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x900D[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex1 - Gas Calibration Index */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Gas Number */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex3 - Gas Symbol */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex4 - Gas Name */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex5 - Minimum Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex6 - Nominal Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex7 - Configured Full Scale Range */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex8 - Minimum Flow SP */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex9 - Maximum Flow SP */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex10 - Device Bin Number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex11 - Gas Calibration file revision number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex12 - Revision of supported gas table */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex13 - Date of Factory Calibration */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex14 - Date of Last Calibration */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex15 - Cardinal Points Array Size */
{ DEFTYPE_OCTETSTRING , 0x500 , ACCESS_READ }}; /* Subindex16 - Cardinal Points */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x900D[] = "Gas Parameter Instance 4\000"
"Gas Calibration Index\000"
"Gas Number\000"
"Gas Symbol\000"
"Gas Name\000"
"Minimum Full Scale\000"
"Nominal Full Scale\000"
"Configured Full Scale Range\000"
"Minimum Flow SP\000"
"Maximum Flow SP\000"
"Device Bin Number\000"
"Gas Calibration file revision number\000"
"Revision of supported gas table\000"
"Date of Factory Calibration\000"
"Date of Last Calibration\000"
"Cardinal Points Array Size\000"
"Cardinal Points\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO TOBJ900AE GasParameterInstance40x900D
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={16,0x00,0x000D,"N2\000","Nitrogen\000",0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x900E : Gas Parameter Instance 5
******************************************************************************/
/**
* \addtogroup 0x900E 0x900E | Gas Parameter Instance 5
* @{
* \brief Object 0x900E (Gas Parameter Instance 5) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Gas Calibration Index<br>
* SubIndex 2 - Gas Number<br>
* SubIndex 3 - Gas Symbol<br>
* SubIndex 4 - Gas Name<br>
* SubIndex 5 - Minimum Full Scale<br>
* SubIndex 6 - Nominal Full Scale<br>
* SubIndex 7 - Configured Full Scale Range<br>
* SubIndex 8 - Minimum Flow SP<br>
* SubIndex 9 - Maximum Flow SP<br>
* SubIndex 10 - Device Bin Number<br>
* SubIndex 11 - Gas Calibration file revision number<br>
* SubIndex 12 - Revision of supported gas table<br>
* SubIndex 13 - Date of Factory Calibration<br>
* SubIndex 14 - Date of Last Calibration<br>
* SubIndex 15 - Cardinal Points Array Size<br>
* SubIndex 16 - Cardinal Points<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x900E[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex1 - Gas Calibration Index */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex2 - Gas Number */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex3 - Gas Symbol */
{ DEFTYPE_VISIBLESTRING , 0x400 , ACCESS_READ }, /* Subindex4 - Gas Name */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex5 - Minimum Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex6 - Nominal Full Scale */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex7 - Configured Full Scale Range */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex8 - Minimum Flow SP */
{ DEFTYPE_REAL32 , 0x20 , ACCESS_READ }, /* Subindex9 - Maximum Flow SP */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex10 - Device Bin Number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex11 - Gas Calibration file revision number */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex12 - Revision of supported gas table */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex13 - Date of Factory Calibration */
{ DEFTYPE_VISIBLESTRING , 0x50 , ACCESS_READ }, /* Subindex14 - Date of Last Calibration */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex15 - Cardinal Points Array Size */
{ DEFTYPE_OCTETSTRING , 0x500 , ACCESS_READ }}; /* Subindex16 - Cardinal Points */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x900E[] = "Gas Parameter Instance 5\000"
"Gas Calibration Index\000"
"Gas Number\000"
"Gas Symbol\000"
"Gas Name\000"
"Minimum Full Scale\000"
"Nominal Full Scale\000"
"Configured Full Scale Range\000"
"Minimum Flow SP\000"
"Maximum Flow SP\000"
"Device Bin Number\000"
"Gas Calibration file revision number\000"
"Revision of supported gas table\000"
"Date of Factory Calibration\000"
"Date of Last Calibration\000"
"Cardinal Points Array Size\000"
"Cardinal Points\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO TOBJ900AE GasParameterInstance50x900E
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={16,0x0000,0x000D,"N2\000","Nitrogen\000",0,0,0,0,0,0,0,0,0,0,0,0}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0xF000 : Semiconductor Device Profile
******************************************************************************/
/**
* \addtogroup 0xF000 0xF000 | SDP
* @{
* \brief Object 0xF000 (SDP) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index Distance<br>
* SubIndex 2 - Element 1<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF000[] = {
	{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ },	/* Subindex1 - Index Distance */
	{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};	/* Subindex2 - Element 1 */

	/**
	* \brief Object/Entry names
	*/
	OBJCONST UCHAR OBJMEM aName0xF000[] = "Semiconductor Device Profile\000"
	"Index Distance\000"
	"Maximum Number of Modules\000\377";
	#endif //#ifdef _OBJD_

	#ifndef _ETG5003_2020__MFC_OBJECTS_H_
	/**
	* \brief Object structure
	*/
	typedef struct OBJ_STRUCT_PACKED_START {
		UINT16 u16SubIndex0;
		UINT16 u16IndexDistance; /* Subindex1 - Index Distance */
		UINT16 u16MaxNumModules; /* Subindex2 - Element 1 */
	} OBJ_STRUCT_PACKED_END
	TOBJF000;
	#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

	/**
	* \brief Object variable
	*/
	PROTO TOBJF000 SemiconductorDeviceProfile0xF000
	#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
	={2,16,1}
	#endif
	;
/** @}*/

/******************************************************************************
*                    Object 0xF010 : Module Profile List
******************************************************************************/
/**
* \addtogroup 0xF010 0xF010 | Module Profile List
* @{
* \brief Object 0xF010 (Module Profile List) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
*
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF010[] = "Module Profile List\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
	UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF010;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF010 ModuleProfileLists0xF010
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{2020}}
#endif
;
/** @}*/

/*---------------------------------------------
-    0xF380
-----------------------------------------------*/
/**
 * \brief 0xF380 Active Exception Status
 
 */
#ifdef _OBJD_

unsigned char ExceptionStatus0xF380 = 0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF380[] = {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING};
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF380 Active Exception Status
 */   
OBJCONST UCHAR OBJMEM aName0xF380[] = "Active Exception Status\000\377";
#endif //#ifdef _OBJD_


/******************************************************************************
*                    Object 0xF381 : Active Device Warning Details
******************************************************************************/
/**
* \addtogroup 0xF381 0xF381 | Active Device Warning Details
* @{
* \brief Object 0xF381 (Active Device Warning Details) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF381[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF381[] = "Active Device Warning Details\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF381;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF381 ActiveDeviceWarningDetails0xF381
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF383 : Active Device Error Details
******************************************************************************/
/**
* \addtogroup 0xF383 0xF383 | Active Device Error Details
* @{
* \brief Object 0xF383 (Active Device Error Details) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF383[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF383[] = "Active Device Error Details\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF383;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF383 ActiveDeviceErrorDetails0xF383
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0}}
#endif
;
/** @}*/

/*---------------------------------------------
-    0xF390
-----------------------------------------------*/
/**
 * \brief 0xF380 Latched Exception Status
 
 */
#ifdef _OBJD_

unsigned char LatchedExceptionStatus0xF390 =0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF390[] = {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ | OBJACCESS_TXPDOMAPPING};
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF380 Latched Exception Status
 */   
OBJCONST UCHAR OBJMEM aName0xF390[] = "Latched Exception Status\000\377";
#endif //#ifdef _OBJD_



/******************************************************************************
*                    Object 0xF391 : Latched Device Warning Details
******************************************************************************/
/**
* \addtogroup 0xF391 0xF391 | Latched Device Warning Details
* @{
* \brief Object 0xF391 (Latched Device Warning Details) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF391[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF391[] = "Latched Device Warning Details\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF391;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF391 LatchedDeviceWarningDetails0xF391
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF393 : Latched Device Error Details
******************************************************************************/
/**
* \addtogroup 0xF393 0xF393 | Latched Device Error Details
* @{
* \brief Object 0xF393 (Latched Device Error Details) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF393[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF393[] = "Latched Device Error Details\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF393;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF393 LatchedDeviceErrorDetails0xF393
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF3A1 : Device Warning Mask
******************************************************************************/
/**
* \addtogroup 0xF3A1 0xF3A1 | Device Warning Mask
* @{
* \brief Object 0xF3A1 (Device Warning Mask) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF3A1[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_BACKUP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF3A1[] = "Device Warning Mask\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF3A1;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF3A1 DeviceWarningMask0xF3A1
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0xFFFFFFFF}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF3A3 : Device Error Mask
******************************************************************************/
/**
* \addtogroup 0xF3A3 0xF3A3 | Device Error Mask
* @{
* \brief Object 0xF3A3 (Device Error Mask) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF3A3[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE | OBJACCESS_BACKUP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF3A3[] = "Device Error Mask\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF3A3;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF3A3 DeviceErrorMask0xF3A3
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0xFFFFFFFF}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF6F0 : Input Latch local timestamp
******************************************************************************/
/**
* \addtogroup 0xF6F0 0xF6F0 | Input Latch local timestamp
* @{
* \brief Object 0xF6F0 (Device Error Mask) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF6F0[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ | OBJACCESS_TXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF6F0[] = "Input Latch Local Timestamp\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF6F0;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF6F0 LatchedTimestamp0xF6F0
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0xFFFFFFFF}}
#endif
;
/** @}*/

/*---------------------------------------------
-    0xF9F0
-----------------------------------------------*/
/**
 * \brief 0xF9F0 Serial number
 
 */
#ifdef _OBJD_

unsigned char SerialNumber0xF9F0 =0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F0[] = {DEFTYPE_VISIBLESTRING, 0x40, ACCESS_READ };
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF9F0 Active Exception Status
 */   
OBJCONST UCHAR OBJMEM aName0xF9F0[] = "Manufacturer Serial Number\000\377";
#endif //#ifdef _OBJD_


/******************************************************************************
*                    Object 0xF9F1 : CDP FGN
******************************************************************************/
/**
* \addtogroup 0xF9F1 0xF9F1 | CDP FGN
* @{
* \brief Object 0xF6F0 (Device Error Mask) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F1[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF9F1[] = "CDP Functional Generation Number\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF9F1;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_


/**
* \brief Object variable
*/
PROTO TOBJF9F1 CDPFGN0xF9F1
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x00000002}}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0xF9F2 : SDP FGN
******************************************************************************/
/**
* \addtogroup 0xF9F2 0xF9F2 | SDP FGN
* @{
* \brief Object 0xF9F2 SDP FGN
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F2[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF9F2[] = "SDP Functional Generation Number\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF9F2;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_


/**
* \brief Object variable
*/
PROTO TOBJF9F2 SDPFGN0xF9F2
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x00000001}}
#endif
;
/** @}*/

/*---------------------------------------------
-    0xF9F3
-----------------------------------------------*/
/**
 * \brief 0xF9F3 Vendor Name
 
 */
#ifdef _OBJD_

unsigned char VendorName0xF9F3 =0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F3[] = {DEFTYPE_VISIBLESTRING, 0x40, ACCESS_READ };
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF9F3 Vendor Name
 */   
OBJCONST UCHAR OBJMEM aName0xF9F3[] = "Vendor Name\000\377";
#endif //#ifdef _OBJD_


/******************************************************************************
*                    Object 0xF9F4 : Device Name
******************************************************************************/
/**
* \addtogroup 0xF9F4 0xF9F4 | Device Name
* @{
* \brief Object 0xF9F4 Device Name
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F4[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_VISIBLESTRING , 0x20 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF9F4[] = "Semiconductor SDP Device Name\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF9F4;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_


/**
* \brief Object variable
*/
PROTO TOBJF9F4 DeviceName0xF9F4
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x00000001}}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0xF9F5 : Output Identifier
******************************************************************************/
/**
* \addtogroup 0xF9F5 0xF9F5 | Output Identifier
* @{
* \brief Object 0xF9F4 Device Name
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F5[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READWRITE | OBJACCESS_RXPDOMAPPING }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF9F5[] = "Output Identifier\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT8 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF9F5;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_


/**
* \brief Object variable
*/
PROTO TOBJF9F5 OutputIdentifier0xF9F5
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={1,{0x01}}
#endif
;
/** @}*/

/*---------------------------------------------
-    0xF9F6
-----------------------------------------------*/
/**
 * \brief 0xF9F6 Time since power on
 
 */
#ifdef _OBJD_

unsigned char UpTime0xF9F6 =0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F6[] = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ };
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF380 Active Exception Status
 */   
OBJCONST UCHAR OBJMEM aName0xF9F6[] = "Time since power on\000\377";
#endif //#ifdef _OBJD_
/*---------------------------------------------
-    0xF9F7
-----------------------------------------------*/
/**
 * \brief 0xF9F7 Total Time powered
 
 */
#ifdef _OBJD_

unsigned char TotalUpTime0xF9F7 =0;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F7[] = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ };
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF9F7 
 */   
OBJCONST UCHAR OBJMEM aName0xF9F7[] = "Total time powered\000\377";
#endif //#ifdef _OBJD_

/*---------------------------------------------
-    0xF9F8
-----------------------------------------------*/
/**
 * \brief 0xF9F8 WU  FGN
 
 */
#ifdef _OBJD_

unsigned char FWUFGN0xF9F8 =1;

OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF9F8[] = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ };
/*ECATCHANGE_END(V5.11) COE3*/

/**
 * \brief 0xF380 Active Exception Status
 */   
OBJCONST UCHAR OBJMEM aName0xF9F8[] = "Firmware Update Functional Generation Number\000\377";
#endif //#ifdef _OBJD_

/******************************************************************************
*                    Object 0xFB10 : Zero Adjust for Flow Sensor
******************************************************************************/
/**
* \addtogroup 0xFB10 0xFB10 | Zero Adjust for Flow Sensor
* @{
* \brief Object 0xFB10 (Zero Adjust for Flow Sensor) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFB10[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READ | ACCESS_WRITE_OP }, /* Subindex1 - Command */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
{ DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }}; /* Subindex3 - Response */
/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFB10[] = "Zero Adjust for Flow Sensor\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Command[4]; /* Subindex1 - Command */
UINT8 Status; /* Subindex2 - Status */
UINT8 Response[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFB10;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFB10 ZeroAdjustForFlowSensor0xFB10
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xFB11 : Zero Adjust for Pressure Sensor
******************************************************************************/
/**
* \addtogroup 0xFB11 0xFB11 | Zero Adjust for Pressure Sensor
* @{
* \brief Object 0xFB11 (Zero Adjust for Pressure Sensor) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFB11[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READ | ACCESS_WRITE_OP }, /* Subindex1 - Command */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
{ DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }}; /* Subindex3 - Response */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFB11[] = "Zero Adjust for Pressure Sensor\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Command[4]; /* Subindex1 - Command */
UINT8 Status; /* Subindex2 - Status */
UINT8 Response[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFB11;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFB11 ZeroAdjustForPressureSensor0xFB11
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xFB12 : Set Temperature Sensor
******************************************************************************/
/**
* \addtogroup 0xFB12 0xFB12 | Set Temperature Sensor
* @{
* \brief Object 0xFB12 (Set Temperature Sensor) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFB12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READ | ACCESS_WRITE_OP }, /* Subindex1 - Command */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
{ DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }}; /* Subindex3 - Response */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFB12[] = "Set Temperature Sensor\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Command[4]; /* Subindex1 - Command */
UINT8 Status; /* Subindex2 - Status */
UINT8 Response[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFB12;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFB12 SetTemperatureSensor0xFB12
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xFB13 : Cardinal Point Calibration
******************************************************************************/
/**
* \addtogroup 0xFB13 0xFB13 | Cardinal Point Calibration
* @{
* \brief Object 0xFB13 (Cardinal Point Calibration) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFB13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_OCTETSTRING , 0x550 , ACCESS_READ | ACCESS_WRITE_OP }, /* Subindex1 - Command */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
{ DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }}; /* Subindex3 - Response */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFB13[] = "Cardinal Point Calibration\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Command[170]; /* Subindex1 - Command */
UINT8 Status; /* Subindex2 - Status */
UINT8 Response[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFB13;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFB13 CardinalPointCalibration0xFB13
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xFB14 : Create Gas Page Service
******************************************************************************/
/**
* \addtogroup 0xFB14 0xFB14 | Create Gas Page Service
* @{
* \brief Object 0xFB14 (Create Gas Page Service) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFB14[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_OCTETSTRING , 0x38 , ACCESS_READ | ACCESS_WRITE_PREOP| ACCESS_WRITE_SAFEOP }, /* Subindex1 - Command */
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
{ DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }}; /* Subindex3 - Response */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFB14[] = "Create Gas Page Service\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 Command[7]; /* Subindex1 - Command */
UINT8 Status; /* Subindex2 - Status */
UINT8 Response[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFB14;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFB14 CreateGasPageService0xFB14
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0xFBF0 : Device Reset Command
******************************************************************************/
/**
* \addtogroup 0xFBF0 0xFBF0 | Gas Parameter Instance 1
* @{
* \brief Object 0xFBF0 (Gas Parameter Instance 1) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFBF0[] = {
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
    { DEFTYPE_OCTETSTRING , 0x30 , ACCESS_READWRITE }, /* SubIndex 1 - Command */
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
    { DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }, /* SubIndex 3 - Response */
}; 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFBF0[] = "Device Reset Command\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 u16SubIndex0;
	CHAR ResetCommand[6]; /* Subindex1 - Command */
	UINT8 ResetStatus; /* Subindex2 - Status */
	CHAR ResetResponse[2]; /* Subindex3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFBF0;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFBF0 DeviceReset0xFBF0
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0xFBF1 : Exception Reset 
******************************************************************************/
/**
* \addtogroup 0xFBF1 0xFBF1 | Exception Rest 
* @{
* \brief Object 0xFBF1 Exception Rest 
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFBF1[] = {
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
    { DEFTYPE_OCTETSTRING , 0x28 , ACCESS_READWRITE }, /* SubIndex 1 - Command */
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* SubIndex 2 - Status */
    { DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }, /* SubIndex 3 - Response */
}; 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFBF1[] = "Exception Reset Command\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 u16SubIndex0;
	CHAR ResetCommand[5]; /* SubIndex 1 - Command */
	UINT8 ResetStatus; /* SubIndex 2 - Status */
	CHAR ResetResponse[2]; /* SubIndex 3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFBF1;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFBF1 ExceptionReset0xFBF1
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0xFBF2 : Store Parameters  Command 
******************************************************************************/
/**
* \addtogroup 0xFBF2 0xFBF2 | Store Parameters  Command 
* @{
* \brief Object 0xFBF2 Store Parameters  Command 
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFBF2[] = {
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
    { DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READWRITE }, /* SubIndex 1 - Command */
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
    { DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }, /* SubIndex 3 - Response */
}; 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFBF2[] = "Store Parameters Command\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 u16SubIndex0;
	CHAR ResetCommand[4]; /* SubIndex 1 - Command */
	UINT8 ResetStatus; /* SubIndex 2 - Status */
	CHAR ResetResponse[2]; /* SubIndex 3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFBF2;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFBF2 StoreParam0xFBF2
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0xFBF3 : calc checksum 
******************************************************************************/
/**
* \addtogroup 0xFBF3 0xFBF3 | calc checksum
* @{
* \brief Object 0xFBF3 calc checksum 
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFBF3[] = {
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
    { DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READWRITE }, /* SubIndex 1 - Command */
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
    { DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }, /* SubIndex 3 - Response */
}; 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFBF3[] = "Calculate Checksum Command\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 u16SubIndex0;
	CHAR ResetCommand[4]; /* SubIndex 1 - Command */
	UINT8 ResetStatus; /* SubIndex 2 - Status */
	CHAR ResetResponse[2]; /* SubIndex 3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFBF3;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFBF3 CalcChecksum0xFBF3
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xFBF4 : Load Parameters Command 
******************************************************************************/
/**
* \addtogroup 0xFBF4 0xFBF4 | Load Parameters Command 
* @{
* \brief Object 0xFBF4 Store Parameters Command 
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Command<br>
* SubIndex 2 - Status<br>
* SubIndex 3 - Response<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xFBF4[] = {
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
    { DEFTYPE_OCTETSTRING , 0x20 , ACCESS_READWRITE }, /* SubIndex 1 - Command */
    { DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ }, /* Subindex2 - Status */
    { DEFTYPE_OCTETSTRING , 0x10 , ACCESS_READ }, /* SubIndex 3 - Response */
}; 

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xFBF4[] = "Load Parameters Command\000"
"Command\000"
"Status\000"
"Response\000\377";
#endif //#ifdef _OBJD_

#ifndef _ETG5003_2020__MFC_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 u16SubIndex0;
	CHAR ResetCommand[4]; /* SubIndex 1 - Command */
	UINT8 ResetStatus; /* SubIndex 2 - Status */
	CHAR ResetResponse[2]; /* SubIndex 3 - Response */
} OBJ_STRUCT_PACKED_END
TOBJFBF4;
#endif //#ifndef _ETG5003_2020__MFC_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJFBF4 LoadParam0xFBF4
#if defined(_ETG5003_2020__MFC_) && (_ETG5003_2020__MFC_ == 1)
={3,0,0,0}
#endif
;
/** @}*/

// Custom ReadBack needed since indices have a non-word aligned SI
PROTO UINT8 Read0x900A_E(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFB10(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFB11(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFB12(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFB13(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFBF0(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFBF2(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFBF3(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 Read0xFBF4(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);

#ifdef _OBJD_
TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 4 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &OutputMapping00x1600, NULL , NULL , 0x0000 },
/* Object 0x1601 */
{NULL , NULL ,  0x1601 , {DEFTYPE_PDOMAPPING , 10 | (OBJCODE_REC << 8)} , asEntryDesc0x1601 , aName0x1601 , &OutputMapping00x1601, NULL , NULL , 0x0000 },	
/* Object 0x1A00 */
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 6 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &InputMapping00x1A00, NULL , NULL , 0x0000 },
/* Object 0x1A01 */
{NULL , NULL ,  0x1A01 , {DEFTYPE_PDOMAPPING , 10 | (OBJCODE_REC << 8)} , asEntryDesc0x1A01 , aName0x1A01 , &InputMapping00x1A01, NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 2 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign, NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 2 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign, NULL , NULL , 0x0000 },
/* Object 0x6000 */
{NULL , NULL ,  0x6000 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6000 , aName0x6000 , &SensorFlowFloating0x6000, NULL , NULL , 0x0000 },
/* Object 0x6001 */
{NULL , NULL ,  0x6001 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6001 , aName0x6001 , &SensorPressureFloating0x6001, NULL , NULL , 0x0000 },
/* Object 0x6002 */
{NULL , NULL ,  0x6002 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6002 , aName0x6002 , &SensorTemperatureFloating0x6002, NULL , NULL , 0x0000 },
/* Object 0x6004 */
{NULL , NULL ,  0x6004 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6004 , aName0x6004 , &SensorFlowInteger0x6004, NULL , NULL , 0x0000 },
/* Object 0x6005 */
{NULL , NULL ,  0x6005 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6005 , aName0x6005 , &SensorPressureInteger0x6005, NULL , NULL , 0x0000 },
/* Object 0x6006 */
{NULL , NULL ,  0x6006 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x6006 , aName0x6006 , &SensorTemperatureInteger0x6006, NULL , NULL , 0x0000 },
/* Object 0x6009 */
{NULL , NULL ,  0x6009 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x6009 , aName0x6009 , &Actuator0x6009, NULL , NULL , 0x0000 },
/* Object 0x600F */
{NULL , NULL ,  0x600F , {DEFTYPE_RECORD , 15 | (OBJCODE_REC << 8)} , asEntryDesc0x600F , aName0x600F , &Status0x600F, NULL , NULL , 0x0000 },
/* Object 0x7003 */
{NULL , NULL ,  0x7003 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x7003 , aName0x7003 , &ControllerFlowSPFloating0x7003, NULL , NULL , 0x0000 },
/* Object 0x7007 */
{NULL , NULL ,  0x7007 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x7007 , aName0x7007 , &ControllerFlowSPInteger0x7007, NULL , NULL , 0x0000 },
/* Object 0x7008 */
{NULL , NULL ,  0x7008 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x7008 , aName0x7008 , &ControllerRampTime0x7008, NULL , NULL , 0x0000 },
/* Object 0x7009 */
{NULL , NULL ,  0x7009 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x7009 , aName0x7009 , &Actuator0x7009, NULL , NULL , 0x0000 },
/* Object 0x700F */
{NULL , NULL ,  0x700F , {DEFTYPE_RECORD , 15 | (OBJCODE_REC << 8)} , asEntryDesc0x700F , aName0x700F , &Status0x700F, NULL , NULL , 0x0000 },
/* Object 0x8000 */
{NULL , NULL ,  0x8000 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8000 , aName0x8000 , &SensorFlowFloating0x8000, NULL , NULL , 0x0000 },
/* Object 0x8001 */
{NULL , NULL ,  0x8001 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8001 , aName0x8001 , &SensorPressureFloating0x8001, NULL , NULL , 0x0000 },
/* Object 0x8002 */
{NULL , NULL ,  0x8002 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8002 , aName0x8002 , &SensorTemperatureFloating0x8002, NULL , NULL , 0x0000 },
/* Object 0x8008 */
{NULL , NULL ,  0x8008 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8008 , aName0x8008 , &ControllerRampTime0x8008, NULL , NULL , 0x0000 },
/* Object 0x8009 */
{NULL , NULL ,  0x8009 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x8009 , aName0x8009 , &Actuator0x8009, NULL , NULL , 0x0000 },
/* Object 0x800A */
{NULL , NULL ,  0x800A , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x800A , aName0x800A , &ActiveGasCalibrationIndex0x800A, NULL , NULL , 0x0000 },
/* Object 0x9000 */
{NULL , NULL ,  0x9000 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x9000 , aName0x9000 , &SensorFlowFloating0x9000, NULL , NULL , 0x0000 },
/* Object 0x9001 */
{NULL , NULL ,  0x9001 , {DEFTYPE_RECORD , 4 | (OBJCODE_REC << 8)} , asEntryDesc0x9001 , aName0x9001 , &SensorPressureFloating0x9001, NULL , NULL , 0x0000 },
/* Object 0x9002 */
{NULL , NULL ,  0x9002 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0x9002 , aName0x9002 , &SensorTemperatureFloating0x9002, NULL , NULL , 0x0000 },
/* Object 0x9008 */
{NULL , NULL ,  0x9008 , {DEFTYPE_RECORD , 1 | (OBJCODE_REC << 8)} , asEntryDesc0x9008 , aName0x9008 , &ControllerRampTime0x9008, NULL , NULL , 0x0000 },
/* Object 0x9009 */
{NULL , NULL ,  0x9009 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x9009 , aName0x9009 , &Actuator0x9009, NULL , NULL , 0x0000 },
/* Object 0x900A */
{NULL , NULL ,  0x900A , {DEFTYPE_RECORD , 16 | (OBJCODE_REC << 8)} , asEntryDesc0x900A , aName0x900A , &GasParameterInstance10x900A, Read0x900A_E , NULL , 0x0000 },
/* Object 0x900B */
{NULL , NULL ,  0x900B , {DEFTYPE_RECORD , 16 | (OBJCODE_REC << 8)} , asEntryDesc0x900B , aName0x900B , &GasParameterInstance20x900B, Read0x900A_E , NULL , 0x0000 },
/* Object 0x900C */
{NULL , NULL ,  0x900C , {DEFTYPE_RECORD , 16 | (OBJCODE_REC << 8)} , asEntryDesc0x900C , aName0x900C , &GasParameterInstance30x900C, Read0x900A_E , NULL , 0x0000 },
/* Object 0x900D */
{NULL , NULL ,  0x900D , {DEFTYPE_RECORD , 16 | (OBJCODE_REC << 8)} , asEntryDesc0x900D , aName0x900D , &GasParameterInstance40x900D, Read0x900A_E , NULL , 0x0000 },
/* Object 0x900E */
{NULL , NULL ,  0x900E , {DEFTYPE_RECORD , 16 | (OBJCODE_REC << 8)} , asEntryDesc0x900E , aName0x900E , &GasParameterInstance50x900E, Read0x900A_E , NULL , 0x0000 },
/* Object 0xF000 */
{NULL , NULL ,  0xF000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0xF000 , aName0xF000 , &SemiconductorDeviceProfile0xF000, NULL , NULL , 0x0000 },	
/* Object 0xF010 */
{NULL , NULL ,  0xF010 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF010 , aName0xF010 , &ModuleProfileLists0xF010, NULL , NULL , 0x0000 },	
/* Object 0xF380 */
{NULL , NULL ,  0xF380 , {DEFTYPE_UNSIGNED8 , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF380 , aName0xF380 , &ExceptionStatus0xF380, NULL , NULL , 0x0000 },
/* Object 0xF381 */
{NULL , NULL ,  0xF381 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF381 , aName0xF381 , &ActiveDeviceWarningDetails0xF381, NULL , NULL , 0x0000 },
/* Object 0xF383 */
{NULL , NULL ,  0xF383 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF383 , aName0xF383 , &ActiveDeviceErrorDetails0xF383, NULL , NULL , 0x0000 },
/* Object 0xF390 */
{NULL , NULL ,  0xF390 , {DEFTYPE_UNSIGNED8 , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF390 , aName0xF390 , &LatchedExceptionStatus0xF390, NULL , NULL , 0x0000 },
/* Object 0xF391 */
{NULL , NULL ,  0xF391 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF391 , aName0xF391 , &LatchedDeviceWarningDetails0xF391, NULL , NULL , 0x0000 },
/* Object 0xF393 */
{NULL , NULL ,  0xF393 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF393 , aName0xF393 , &LatchedDeviceErrorDetails0xF393, NULL , NULL , 0x0000 },
/* Object 0xF3A1 */
{NULL , NULL ,  0xF3A1 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF3A1 , aName0xF3A1 , &DeviceWarningMask0xF3A1, NULL , NULL , 0x0000 },
/* Object 0xF3A3 */
{NULL , NULL ,  0xF3A3 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF3A3 , aName0xF3A3 , &DeviceErrorMask0xF3A3, NULL , NULL , 0x0000 },
/* Object 0xF6F0 */
{NULL , NULL ,  0xF6F0 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF6F0 , aName0xF6F0 , &LatchedTimestamp0xF6F0, NULL , NULL , 0x0000 },
/* Object 0xF9F0 */
{NULL , NULL ,  0xF9F0 , {DEFTYPE_VISIBLESTRING , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF9F0 , aName0xF9F0 , &SerialNumber0xF9F0, NULL , NULL , 0x0000 },
/* Object 0xF9F1 */
{NULL , NULL ,  0xF9F1 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF9F1 , aName0xF9F1 , &CDPFGN0xF9F1, NULL , NULL , 0x0000 },
/* Object 0xF9F2 */
{NULL , NULL ,  0xF9F2 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF9F2 , aName0xF9F2 , &SDPFGN0xF9F2, NULL , NULL , 0x0000 },
/* Object 0xF9F3 */
{NULL , NULL ,  0xF9F3 , {DEFTYPE_VISIBLESTRING , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF9F3 , aName0xF9F3 , &VendorName0xF9F3, NULL , NULL , 0x0000 },
/* Object 0xF9F4 */
{NULL , NULL ,  0xF9F4 , {DEFTYPE_VISIBLESTRING , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF9F4 , aName0xF9F4 , &DeviceName0xF9F4, NULL , NULL , 0x0000 },
/* Object 0xF9F5 */
{NULL , NULL ,  0xF9F5 , {DEFTYPE_UNSIGNED8 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF9F5 , aName0xF9F5 , &OutputIdentifier0xF9F5, NULL , NULL , 0x0000 },
/* Object 0xF9F6 */
{NULL , NULL ,  0xF9F6 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF9F6 , aName0xF9F6 , &UpTime0xF9F6, NULL , NULL , 0x0000 },
/* Object 0xF9F7 */
{NULL , NULL ,  0xF9F7 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF9F7 , aName0xF9F7 , &TotalUpTime0xF9F7, NULL , NULL , 0x0000 },
/* Object 0xF9F8 */
{NULL , NULL ,  0xF9F8 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_VAR << 8)} , asEntryDesc0xF9F8 , aName0xF9F8 , &FWUFGN0xF9F8, NULL , NULL , 0x0000 },
/* Object 0xFB10 */
{NULL , NULL ,  0xFB10 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFB10 , aName0xFB10 , &ZeroAdjustForFlowSensor0xFB10, Read0xFB10 , NULL , 0x0000 },
/* Object 0xFB11 */
{NULL , NULL ,  0xFB11 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFB11 , aName0xFB11 , &ZeroAdjustForPressureSensor0xFB11, Read0xFB11 , NULL , 0x0000 },
/* Object 0xFB12 */
{NULL , NULL ,  0xFB12 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFB12 , aName0xFB12 , &SetTemperatureSensor0xFB12, Read0xFB12 , NULL , 0x0000 },
/* Object 0xFB13 */
{NULL , NULL ,  0xFB13 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFB13 , aName0xFB13 , &CardinalPointCalibration0xFB13, Read0xFB13 , NULL , 0x0000 },
/* Object 0xFB14 */
{NULL , NULL ,  0xFB14 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFB14 , aName0xFB14 , &CreateGasPageService0xFB14, NULL , NULL , 0x0000 },
/* Object 0xFBF0 */
{NULL , NULL ,  0xFBF0 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFBF0 , aName0xFBF0 , &DeviceReset0xFBF0, Read0xFBF0 , NULL , 0x0000 },
/* Object 0xFBF1 */
{NULL , NULL ,  0xFBF1 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFBF1 , aName0xFBF1 , &ExceptionReset0xFBF1, NULL , NULL , 0x0000 },
/* Object 0xFBF2 */
{NULL , NULL ,  0xFBF2 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFBF2 , aName0xFBF2 , &StoreParam0xFBF2, Read0xFBF2 , NULL , 0x0000 },
/* Object 0xFBF3 */
{NULL , NULL ,  0xFBF3 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFBF3 , aName0xFBF3 , &CalcChecksum0xFBF3, Read0xFBF3 , NULL , 0x0000 },
/* Object 0xFBF4 */
{NULL , NULL ,  0xFBF4 , {DEFTYPE_RECORD , 3 | (OBJCODE_REC << 8)} , asEntryDesc0xFBF4 , aName0xFBF4 , &LoadParam0xFBF4, Read0xFBF4 , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_
#undef PROTO

/** @}*/
#define _ETG5003_2020__MFC_OBJECTS_H_
