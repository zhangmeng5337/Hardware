#include <stdint.h>
#include "protocol.h"

extern command_struct cmdFirmware[];
#ifndef BOOTLOADER
extern command_struct cmdConfiguration[];
extern command_struct cmdIO[];
extern command_struct cmdSensor[];
extern command_struct cmdController[];
//extern command_struct cmdPollIO[];
#endif

const command_list_strcture cmdList[] = 
{
    {   170,       cmdFirmware    },
#ifndef BOOTLOADER
    {   2,       cmdConfiguration  },
    {   3,       cmdSensor  },    
    {   4,       cmdIO  },    
    {   5,       cmdController  },
//    {  10,       cmdPollIO  },
#endif
    {0x00, 0x00}
};
