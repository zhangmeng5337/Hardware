#include "update.h"
#include "stmflash.h"

/** @bieaf 	��ȡ����ģʽ
	* @return mode	����ֵ
	*/
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash(OTA_NUM_ADDR, &mode, 1);
    return mode;
}

/**
	* @bieaf ���г���ĸ���
	* @detail 1.����Ŀ�ĵ�ַ
	* @detail 2.Դ��ַ�Ĵ��뿽����Ŀ�ĵ�ַ
	* @detail 3.����Դ��ַ
	*
	* @param  ���˵�Դ��ַ
	* @param  ���˵�Ŀ�ĵ�ַ
	* @return ���˵ĳ����С
	*/
uint32_t rd_addr;
void MoveCode(uint32_t src_addr, uint32_t des_addr, uint32_t byte_size)
{
	HAL_StatusTypeDef status;

    printf("> Start Erase......\r\n");
    uint8_t temp[1024];
    uint32_t i;
    temp[0] = 0xaa;
    status = HAL_ERROR;
    while(status == HAL_ERROR)
    status = Erase_page(des_addr, 2);//ֻ����1����128k

   rd_addr =  src_addr;
    printf("> Start copy......\r\n");
    for (i = 0; i < byte_size; i = i + 1024)
    {
			  rd_addr = rd_addr + i;
        ReadFlash((src_addr + i), temp, 1024);
        WriteFlash((des_addr + i), temp, 1024);

    }
    printf("> Copy sucessfully......\r\n");
  //  while (1);
}


/** @bieaf 	���û������ջ��ֵ
	* @return none	����ֵ
	*/
__asm void MSR_MSP(uint32_t ulAddr)
{
    MSR MSP, r0 //set Main Stack value
    BX r14
}

/** @bieaf 	������ת����
	* @return none	����ֵ
	*/
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp(uint32_t App_Addr)
{
    Jump_Fun JumpToApp;

    if (((*(__IO uint32_t *)App_Addr) & 0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
    {
			    __HAL_RCC_PWR_CLK_DISABLE();
			 HAL_RCC_DeInit();
        JumpToApp = (Jump_Fun) * (__IO uint32_t *)(App_Addr + 4);	//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
        MSR_MSP(* (__IO uint32_t *) App_Addr);	//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        JumpToApp();	//��ת��APP
    }
    else
    {
        printf("There is None APP to jump,ERROR!!!\r\n");
    }
}

/**
	* @bieaf ����BootLoader������
	* @return none
	*/
void Start_BootLoader(void)
{
    printf("\r\n");
    printf("***********************************\r\n");
    printf("*                                 *\r\n");
    printf("*           BootLoader            *\r\n");
    printf("*                                 *\r\n");
    printf("***********************************\r\n");

//    switch (Read_Start_Mode())									//��ȡ�Ƿ�����Ӧ�ó���
//    {
//        case Startup_Normal:										//��������
//        {
//            printf("> Normal Start......\r\n");
//           // MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
//            //MoveCode(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
//            break;
//        }
//        case Startup_Update:										//����������
//        {
//            MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
//            break;
//        }
//        default:																//����ʧ��
//        {
//            return;
//        }
//    }

    unsigned char read_flag=0;
		read_flag = Read_Start_Mode();
    if(read_flag<=0x2a)
		{
		if(read_flag==0x20)
		{
  
				 // read_flag = 0x10;
			   // Erase_page(OTA_NUM_ADDR, 1);
          //WriteFlash((OTA_NUM_ADDR), &read_flag, 1);	
			printf(">Jump to App2......\r\n");
			IAP_ExecuteApp(Application_2_Addr);
		}
		 

			if(read_flag==0x10)
			{
 
						read_flag = 0x20;
						//Erase_page(OTA_NUM_ADDR, 1);
					//	WriteFlash((OTA_NUM_ADDR), &read_flag, 1);
						printf(">Jump to App1......\r\n");
						IAP_ExecuteApp(Application_1_Addr);
			}			
		}			
	
			printf(">Jump to App3......\r\n");
				 read_flag = 0x10;
			    //Erase_page(OTA_NUM_ADDR, 1);
          //WriteFlash((OTA_NUM_ADDR), &read_flag, 1);	
			IAP_ExecuteApp(Application_1_Addr);		


		

}

