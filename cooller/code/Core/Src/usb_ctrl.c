/* USER CODE BEGIN Includes */
#include "uart.h"
#include "usb_host.h"
#include "fatfs.h"
#include "usb_host.h"
#include "ff.h"
#include "exfuns.h"
#include "fattester.h"
#include "malloc.h"
#include "string.h"
#include "usb_ctrl.h"
#include "config.h"
#include "flash.h"

/* USER CODE END Includes */
extern ApplicationTypeDef Appli_state ;
UINT br, bw;         //读写变量
FATFS *USB_fat[FF_VOLUMES];

//extern FATFS *fs[2];
unsigned char *FileBuf;
unsigned char test_txt[] = "hello usb";
unsigned char status;

uint8_t exfuns_init(void)
{
    uint8_t i;
    uint8_t res = 0;

    for (i = 0; i < FF_VOLUMES; i++)
    {
        USB_fat[i] = (FATFS *)malloc(sizeof(FATFS));   /* 为磁盘i工作区申请内存 */

        if (!USB_fat[i])break;
    }


}
unsigned char get_usb_state()
{
    return Appli_state;
}

unsigned char usb_state = 0;
unsigned char *get_usb_wr()
{
    return &usb_state;
}
void usb_init()
{
    exfuns_init();


}

void usb_ctrl()
{
    if (Appli_state == APPLICATION_READY&&getConfig()->export_flag == 1) //U盘已经加载完成
    {
        if (status == 0 || usb_state == 2) //U盘加载后只运行一次
        {
            status = 1;
            //  retUSBH=f_mount(USB_fat[0], "0:", 1);        /* 挂载SD卡 */
            retUSBH = f_mount(USB_fat[0], "0:", 1);       /* 挂载SD卡 */
            //retUSBH = f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
            if (retUSBH == FR_OK)
            {
                printf("挂载U盘成功!\r\n");
            }

            printf("写入文件测试!\r\n");
            retUSBH = f_open(&USBHFile, "test.txt",
                             FA_CREATE_ALWAYS | FA_WRITE);
            if (retUSBH == FR_OK)
            {
                usb_state = 1;
                printf("打开文件\"测试.txt\"成功!\r\n");
            }

            retUSBH = f_lseek(&USBHFile, f_size(&USBHFile));
            if (retUSBH == FR_OK)
            {
                printf("指针移动到文件末尾成功！\r\n");
            }
            FileBuf = test_txt;
            // get_flash_status()->used_len
            uint32_t i, addr;
            unsigned char pb[32];
            float dat;
            if (get_flash_status()->usb_read_flag == 0)
            {
                i = get_flash_status()->used_len;
                addr = Application_2_Addr;
                while (i)
                {
                     i=i-4;
                    ReadFlash(addr, pb, 4);
                    addr = addr + 4;
                    dat=uint32Tofloat( pb); //上限温度
                     sprintf(pb,"%.3f\r\n", dat);
                    retUSBH = f_write(&USBHFile, pb, strlen(pb), &bw);
                }
                get_flash_status()->usb_read_flag = 1;

            }

           // retUSBH = f_write(&USBHFile, FileBuf, strlen(test_txt), &bw);

            if (retUSBH == FR_OK)
            {
                printf("写入文件成功!\r\n");
            }
            retUSBH = f_close(&USBHFile);
            if (retUSBH == FR_OK)
            {
                printf("关闭文件成功!\r\n");
            }
            //读取文件测试
            printf("读取文件测试!\r\n");
            retUSBH = f_open(&USBHFile, (const char *)"测试.txt",
                             FA_OPEN_EXISTING | FA_READ);//打开文件
            if (retUSBH == FR_OK)
            {
                printf("打开文件\"测试.txt\"成功!\r\n");
            }
            retUSBH = f_read(&USBHFile, FileBuf, 18, &bw);
            if (retUSBH == FR_OK)
            {
                printf("读取文件成功!文件内容：");
                //HAL_UART_Transmit(&huart1, FileBuf, bw, 100);
            }
            retUSBH = f_close(&USBHFile);
            if (retUSBH == FR_OK)
            {
                printf("关闭文件成功!\r\n");
            }
        }
    }

    if (Appli_state != APPLICATION_READY) //U盘未插入
    {
        status = 0;//等待下次U盘插入则重新写入
        get_flash_status()->usb_read_flag = 0;
    }

}


