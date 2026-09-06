#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "nvmem.h"
#include "protocol.h"
#include "analogIO.h"
#include "digitalio.h"

// 4
const command_struct cmdIO[] = 
{

  {23,      _UINT8,     1,      &pb[0].pin,         0x00,       0x00,      0x00   },	

  {0x0000,   0x00,    0x00,   0x00,   0x00,   0x00}
};
