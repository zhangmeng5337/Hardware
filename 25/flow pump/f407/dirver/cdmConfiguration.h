#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "main.h"
#define uChar 	0
#define uINT 	1
#define INT 	2
#define FLOAT 	3

#define __READ_ONLY 	0
#define __RW 			1
#define __FLASH 		2



typedef struct
{
    uint16_t cmdSpecific;   // specific command ID
    uint8_t dataType;       // data type of the command
    uint16_t dataSize;      // size of the data type on the wire (in bytes)
    void *memptr;           // pointer to memory location (volatile or nonvolatile)
    uint8_t (*readFunction)(uint8_t *cmdInfo);     // pointer to read function
    uint8_t (*writeFunction)(uint8_t *cmdInfo);    // pointer to write function
    uint32_t accessRights;  // access rights to this command
} command_struct;
typedef struct
{
    uint16_t cmdMain;   // specific command ID
    command_struct *command;

} cmdListStru;
#endif


