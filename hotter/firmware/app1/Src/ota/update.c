#include "update.h"
#include "stmflash.h"
#include "config.h"
HAL_StatusTypeDef status = HAL_ERROR;

/* ????,???????? */
void boot_jump(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp, result;
    result = 0;
    if (OTA_UPDATE_TO == UAPP2)
    {

        Erase_page(OTA_NUM_ADDR, 1);
        update_flag = 0x10;//now is app 1
        WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag, 1);

        ReadFlash(OTA_NUM_ADDR, &tmp, 1);
        while (tmp != update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR, &tmp, 1);
            if (result >= 5)
                break;
            else
                result++;

        }
    }
    result = 0;
    if (OTA_UPDATE_TO == UAPP1)
    {
        Erase_page(OTA_NUM_ADDR, 1);
        update_flag = 0x20;//now is app 2
        WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);

        ReadFlash(OTA_NUM_ADDR, &tmp, 1);

        while (tmp != update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            if (result >= 5)
                break;
            else
                result++;
        }
    }

}
void Update_Success(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp, result;
    if (OTA_UPDATE_TO == UAPP2)
    {
        result = 0;
        status = HAL_ERROR;
        while (status == HAL_ERROR)
        {
            if (result >= 5)
                break;
            else
                result++;
            status = Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x20;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR, &tmp, 1);
        result = 0;
        status = HAL_ERROR;
        if (tmp != update_flag)
        {
            while (status == HAL_ERROR)
            {
                if (result >= 5)
                    break;
                else
                    result++;
                status = Erase_page(OTA_NUM_ADDR, 1);

            }

            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR, &tmp, 1);


        }
    }


    if (OTA_UPDATE_TO == UAPP1)
    {
        result = 0;
        status = HAL_ERROR;
        while (status == HAL_ERROR)
        {
            if (result >= 5)
                break;
            else
                result++;
            status = Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x10;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR, &tmp, 1);
        result = 0;
        status = HAL_ERROR;

        if (tmp != update_flag)
        {
            while (status == HAL_ERROR)
            {
                if (result >= 5)
                    break;
                else
                    result++;
                status = Erase_page(OTA_NUM_ADDR, 1);

            }
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR, &tmp, 1);


        }
    }



    //uint8_t update_flag = 0xBB;
    //WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);
}
void Update_To_Sel(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp, result;
    if (OTA_UPDATE_TO == UAPP2)
    {
        result = 0;
        status = HAL_ERROR;
        while (status == HAL_ERROR)
        {
            if (result >= 5)
                break;
            else
                result++;
            status = Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x10;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR, &tmp, 1);
    }


    if (OTA_UPDATE_TO == UAPP1)
    {
        status = HAL_ERROR;
        while (status == HAL_ERROR)
        {
            if (result >= 5)
                break;
            else
                result++;
            status = Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x20;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR, &tmp, 1);

    }
}
/* ???? */
void OTA_Task(void)
{
    if (get_config()->reboot_flag == 1)
    {
        get_config()->reboot_flag = 0;
        Update_Success();
        HAL_NVIC_SystemReset();
    }
}
/** @bieaf  ��ȡ����ģʽ
    * @return mode  ����ֵ
    */
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash(OTA_NUM_ADDR, &mode, 1);
    return mode;
}
/** @bieaf  ���û������ջ��ֵ
    * @return none  ����ֵ
    */
__asm void MSR_MSP(uint32_t ulAddr)
{
    MSR MSP, r0 //set Main Stack value
    BX r14
}

/** @bieaf  ������ת����
    * @return none  ����ֵ
    */
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp(uint32_t App_Addr)
{
    Jump_Fun JumpToApp;

    if (((*(__IO uint32_t *)App_Addr) & 0x2FFE0000) ==
            0x20000000)  //���ջ����ַ�Ƿ�Ϸ�.
    {
        __HAL_RCC_PWR_CLK_DISABLE();
        HAL_RCC_DeInit();
        __disable_irq();
        JumpToApp = (Jump_Fun) * (__IO uint32_t *)(App_Addr +
                    4);   //�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
        MSR_MSP(* (__IO uint32_t *)
                App_Addr);  //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        JumpToApp();    //��ת��APP
    }
    else
    {
        ;//printf("There is None APP to jump,ERROR!!!\r\n");
    }
}

/**
    * @bieaf ����BootLoader������
    * @return none
    */
void Start_BootLoader(void)
{

    unsigned char read_flag = 0,buf[2];
    read_flag = Read_Start_Mode();
    if (read_flag == 0x20)
    {

         ReadFlash(OTA_NUM_ADDR + 1, &read_flag, 1);
        if (read_flag == 0xa5)
        {
            buf[0] = 0x10;
            buf[1] = 0xa5;
					Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash(OTA_NUM_ADDR, buf,2);
            IAP_ExecuteApp(Application_1_Addr);
        }
        else
        {
            buf[0] = 0x20;
            buf[1] = 0xa5;
			Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash(OTA_NUM_ADDR ,buf, 2);
            //read_flag = 0xa5;

            //printf(">Jump to App2......\r\n");
            IAP_ExecuteApp(Application_2_Addr);
        }

    }
    else
    {
         ReadFlash(OTA_NUM_ADDR + 1, &read_flag, 1);
        if (read_flag == 0xa5)
        {
            buf[0] = 0x20;
            buf[1] = 0xa5;
            read_flag = 0x20;
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash(OTA_NUM_ADDR , buf, 2);
            //read_flag = 0xa5;
            IAP_ExecuteApp(Application_2_Addr);
        }
        else
        {
            buf[0] = 0x10;
            buf[1] = 0xa5;
            read_flag = 0x10;
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash(OTA_NUM_ADDR , buf, 2);
            //read_flag = 0xa5;

            //printf(">Jump to App2......\r\n");
            IAP_ExecuteApp(Application_1_Addr);
        }

    }

}