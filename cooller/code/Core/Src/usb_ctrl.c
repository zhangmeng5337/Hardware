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
UINT br, bw;         //��д����
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
        USB_fat[i] = (FATFS *)malloc(sizeof(FATFS));   /* Ϊ����i�����������ڴ� */

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
    if (Appli_state == APPLICATION_READY&&getConfig()->export_flag == 1) //U���Ѿ��������
    {
        if (status == 0 || usb_state == 2) //U�̼��غ�ֻ����һ��
        {
            status = 1;
            //  retUSBH=f_mount(USB_fat[0], "0:", 1);        /* ����SD�� */
            retUSBH = f_mount(USB_fat[0], "0:", 1);       /* ����SD�� */
            //retUSBH = f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
            if (retUSBH == FR_OK)
            {
                printf("����U�̳ɹ�!\r\n");
            }

            printf("д���ļ�����!\r\n");
            retUSBH = f_open(&USBHFile, "test.txt",
                             FA_CREATE_ALWAYS | FA_WRITE);
            if (retUSBH == FR_OK)
            {
                usb_state = 1;
                printf("���ļ�\"����.txt\"�ɹ�!\r\n");
            }

            retUSBH = f_lseek(&USBHFile, f_size(&USBHFile));
            if (retUSBH == FR_OK)
            {
                printf("ָ���ƶ����ļ�ĩβ�ɹ���\r\n");
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
                    dat=uint32Tofloat( pb); //�����¶�
                     sprintf(pb,"%.3f\r\n", dat);
                    retUSBH = f_write(&USBHFile, pb, strlen(pb), &bw);
                }
                get_flash_status()->usb_read_flag = 1;

            }

           // retUSBH = f_write(&USBHFile, FileBuf, strlen(test_txt), &bw);

            if (retUSBH == FR_OK)
            {
                printf("д���ļ��ɹ�!\r\n");
            }
            retUSBH = f_close(&USBHFile);
            if (retUSBH == FR_OK)
            {
                printf("�ر��ļ��ɹ�!\r\n");
            }
            //��ȡ�ļ�����
            printf("��ȡ�ļ�����!\r\n");
            retUSBH = f_open(&USBHFile, (const char *)"����.txt",
                             FA_OPEN_EXISTING | FA_READ);//���ļ�
            if (retUSBH == FR_OK)
            {
                printf("���ļ�\"����.txt\"�ɹ�!\r\n");
            }
            retUSBH = f_read(&USBHFile, FileBuf, 18, &bw);
            if (retUSBH == FR_OK)
            {
                printf("��ȡ�ļ��ɹ�!�ļ����ݣ�");
                //HAL_UART_Transmit(&huart1, FileBuf, bw, 100);
            }
            retUSBH = f_close(&USBHFile);
            if (retUSBH == FR_OK)
            {
                printf("�ر��ļ��ɹ�!\r\n");
            }
        }
    }

    if (Appli_state != APPLICATION_READY) //U��δ����
    {
        status = 0;//�ȴ��´�U�̲���������д��
        get_flash_status()->usb_read_flag = 0;
    }

}


