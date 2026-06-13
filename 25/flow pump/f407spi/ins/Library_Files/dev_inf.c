#include "dev_inf.h"

const FLASH_DeviceInfoDef Device_Info =
{
  "FM25W32",  // Device name
  0x00000000,  // Start address
  0x00001000,  // Device size 4KB
  0x00000100,  // Page size
  0x00001000,  // Sector size
  0xFFFFFFFF,  // Erase value
  {1,1,0,0,0}  // Read, Write, Erase, SecErase, MassErase
};