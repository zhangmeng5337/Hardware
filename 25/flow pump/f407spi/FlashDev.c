/**************************************************************************//**
 * @file     FlashDev.c
 * @brief    Flash Device Description for New Device Flash
 * @version  V1.0.0
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2010-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "FlashOS.H"        // FlashOS Structures
#include "fm25w32.h"      // STM32F4 标准外设库


#define FM25W32_DEVICE_NAME      "FM25W32 4MB SPI Flash"

 const struct FlashDeviceU  FlashDevice = {
   FLASH_DRV_VERS,             // 驱动版本，必须
   "FM25W32 4MB SPI Flash",    // 算法名称，会在 Keil 的 Flash Download 列表里显示
   EXTSPI,                     // 设备类型：外部 SPI Flash
   0x90000000,                 // Flash 起始地址（可自定义逻辑地址）
   FM25W32_TOTAL_SIZE,            // 总容量: 4MB (32Mbit)
   FM25W32_PAGE_SIZE,                        // 页大小: 256 字节
   0,                          // 保留，必须为 0
   0xFF,                       // 擦除后的默认值: 0xFF
   1000,                       // 页编程超时时间 (ms)
   3000,                       // 扇区擦除超时时间 (ms)
   // 扇区信息列表：FM25W32 共 1024 个扇区，每个 4KB (0x1000 字节)
   // 实际使用时，Keil 会遍历这些项直到遇到 SECTOR_END
   // 这里用 8 个扇区为一组快速填充，实际共需要 1024 项，Keil 会根据地址自动计算
   0x001000, 0x001000, 
   // ... 此处可继续填充至 1024 项，或利用 Keil 对重复扇区的支持自动填充
   SECTOR_END                  // 结束标记
};
