/**
 ****************************************************************************************************
 * @file        diskio.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-14
 * @brief       FATFS�ײ�(diskio) ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220114
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./MALLOC/malloc.h"
#include "./SYSTEM/usart/usart.h"
#include "./FATFS/source/diskio.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"


#define SD_CARD     0       /* SD��,���Ϊ0 */
#define EX_FLASH    1       /* �ⲿspi flash,���Ϊ1 */

/**
 * ����25Q128 FLASHоƬ, ���ǹ涨ǰ 12M ��FATFSʹ��, 12M�Ժ�
 * �����ֿ�, 3���ֿ� + UNIGBK.BIN, �ܴ�С3.09M, ��ռ��15.09M
 * 15.09M�Ժ�Ĵ洢�ռ��ҿ������ʹ��. 
 */

#define SPI_FLASH_SECTOR_SIZE   512
#define SPI_FLASH_SECTOR_COUNT  12 * 1024 * 2   /* 25Q128, ǰ12M�ֽڸ�FATFSռ�� */
#define SPI_FLASH_BLOCK_SIZE    8               /* ÿ��BLOCK��8������ */
#define SPI_FLASH_FATFS_BASE    0               /* FATFS ���ⲿFLASH����ʼ��ַ ��0��ʼ */


/**
 * @brief       ��ô���״̬
 * @param       pdrv : ���̱��0~9
 * @retval      ִ�н��(�μ�FATFS, DSTATUS�Ķ���)
 */
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    return RES_OK;
}

/**
 * @brief       ��ʼ������
 * @param       pdrv : ���̱��0~9
 * @retval      ִ�н��(�μ�FATFS, DSTATUS�Ķ���)
 */
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    uint8_t res = 0;

    switch (pdrv)
    {
        case SD_CARD:           /* SD�� */
            res = sd_init();    /* SD����ʼ�� */
            break;

        case EX_FLASH:          /* �ⲿflash */
            norflash_init(); 
            break;

        default:
            res = 1;
            break;
    }

    if (res)
    {
        return STA_NOINIT;
    }
    else
    {
        return 0; /* ��ʼ���ɹ�*/
    }
}

/**
 * @brief       ������
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : ���ݽ��ջ����׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫ��ȡ��������
 * @retval      ִ�н��(�μ�FATFS, DRESULT�Ķ���)
 */
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    uint8_t res = 0;

    if (!count) return RES_PARERR;   /* count���ܵ���0�����򷵻ز������� */

    switch (pdrv)
    {
        case SD_CARD:       /* SD�� */
            res = sd_read_disk(buff, sector, count);

            while (res)     /* ������ */
            {
                //printf("sd rd error:%d\r\n", res);
                sd_init();  /* ���³�ʼ��SD�� */
                res = sd_read_disk(buff, sector, count);
            }

            break;

        case EX_FLASH:      /* �ⲿflash */
            for (; count > 0; count--)
            {
                norflash_read(buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff += SPI_FLASH_SECTOR_SIZE;
            }

            res = 0;
            break;

        default:
            res = 1;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR; 
    }
}

/**
 * @brief       д����
 * @param       pdrv   : ���̱��0~9
 * @param       buff   : �������ݻ������׵�ַ
 * @param       sector : ������ַ
 * @param       count  : ��Ҫд���������
 * @retval      ִ�н��(�μ�FATFS, DRESULT�Ķ���)
 */
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t res = 0;

    if (!count) return RES_PARERR;  /* count���ܵ���0�����򷵻ز������� */

    switch (pdrv)
    {
        case SD_CARD:       /* SD�� */
            res = sd_write_disk((uint8_t *)buff, sector, count);

            while (res)     /* д���� */
            {
                //printf("sd wr error:%d\r\n", res);
                sd_init();  /* ���³�ʼ��SD�� */
                res = sd_write_disk((uint8_t *)buff, sector, count);
            }

            break;

        case EX_FLASH:      /* �ⲿflash */
            for (; count > 0; count--)
            {
                norflash_write((uint8_t *)buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
                sector++;
                buff += SPI_FLASH_SECTOR_SIZE;
            }

            res = 0;
            break;

        default:
            res = 1;
    }

    /* ������ֵ��������ֵת��ff.c�ķ���ֵ */
    if (res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR; 
    }
}

/**
 * @brief       ��ȡ�������Ʋ���
 * @param       pdrv   : ���̱��0~9
 * @param       ctrl   : ���ƴ���
 * @param       buff   : ����/���ջ�����ָ��
 * @retval      ִ�н��(�μ�FATFS, DRESULT�Ķ���)
 */
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    if (pdrv == SD_CARD)    /* SD�� */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(DWORD *)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = g_sd_card_info_handle.LogBlockSize;
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = g_sd_card_info_handle.LogBlockNbr;
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else if (pdrv == EX_FLASH)  /* �ⲿFLASH */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(WORD *)buff = SPI_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = SPI_FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = SPI_FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else
    {
        res = RES_ERROR;    /* �����Ĳ�֧�� */
    }
    
    return res;
}




















