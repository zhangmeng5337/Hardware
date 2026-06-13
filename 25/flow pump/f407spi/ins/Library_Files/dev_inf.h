#ifndef __DEVICE_INFO_H
#define __DEVICE_INFO_H

#include <stdint.h>

#define DEVICE_NAME_LEN     32

typedef struct {
    char     DevName[DEVICE_NAME_LEN];  /* 设备名称 */
    uint32_t DevAdress;                 /* 设备起始地址 */
    uint32_t DevSize;                   /* 设备总大小 */
    uint32_t PageSize;                  /* 页大小 */
    uint32_t SectorSize;                /* 扇区大小 */
    uint32_t reserved1;                 /* 保留 */
    uint32_t reserved2;                 /* 保留 */
    uint32_t EraseValue;                /* 擦除后值 */

    struct {
        uint32_t Read     : 1;
        uint32_t Write    : 1;
        uint32_t Erase    : 1;
        uint32_t SecErase : 1;
        uint32_t MassErase: 1;
        uint32_t reserved : 3;
    } Support;

} FLASH_DeviceInfoDef;

extern const FLASH_DeviceInfoDef Device_Info;

#endif