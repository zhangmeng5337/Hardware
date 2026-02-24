#include <stdint.h>
#include "protocol.h"

extern command_struct cmdFirmware[];
#ifndef BOOTLOADER
extern command_struct cmdConfiguration[];
#endif

const command_list_strcture cmdList[] = 
{
    {   170,       cmdFirmware    },
#ifndef BOOTLOADER
    {   2,       cmdConfiguration  },
#endif
    {0x00, 0x00}
};
