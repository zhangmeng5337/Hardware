#include "stm32f4xx_hal.h"

__declspec(dllexport) uint32_t Init(void)
{
  // 什么都不做，只返回成功
  return 0;
}

__declspec(dllexport) uint32_t ReadData(uint32_t Address, uint32_t Size, uint8_t* pData)
{
  return 0;
}

__declspec(dllexport) uint32_t WriteData(uint32_t Address, uint32_t Size, uint8_t* pData)
{
  return 0;
}

__declspec(dllexport) uint32_t EraseArea(uint32_t Address, uint32_t Size)
{
  return 0;
}

__declspec(dllexport) uint32_t MassErase(void)
{
  return 0;
}

__declspec(dllexport) uint32_t CheckIntegrity(uint32_t Address, uint32_t Size, uint32_t CRC)
{
  return 0;
}