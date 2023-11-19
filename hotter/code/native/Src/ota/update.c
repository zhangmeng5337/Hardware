#include "update.h"
#include "stmflash.h"

/** @bieaf 	��ȡ����ģʽ
	* @return mode	����ֵ
	*/
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash((Application_2_Addr + Application_Size - 8), &mode, 1);
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
void MoveCode(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size)
{
    /*1.����Ŀ�ĵ�ַ*/
   // printf("> Start erase des flash......\r\n");
    //Erase_page(des_addr, 1);//ֻ����1����128k
   // printf("> Erase des flash sucessfully......\r\n");

    /*2.��ʼ����*/
    uint8_t temp[1];
	  temp[0]= 0xaa;
    printf("> Start copy......\r\n");
    for(int i = 0; i < 1; i++)
    {
       // ReadFlash((src_addr + i*1024), temp, 1024);
        WriteFlash((des_addr + i*1024), temp, 1);
    }
    printf("> Copy sucessfully......\r\n");

}


/** @bieaf 	���û������ջ��ֵ
	* @return none	����ֵ
	*/
__asm void MSR_MSP (uint32_t ulAddr)
{
    MSR MSP, r0 //set Main Stack value			                   
    BX r14
}

/** @bieaf 	������ת����
	* @return none	����ֵ
	*/
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp (uint32_t App_Addr)
{
	Jump_Fun JumpToApp; 
    
	if (((*(__IO uint32_t *)App_Addr) & 0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		JumpToApp = (Jump_Fun) * (__IO uint32_t *)(App_Addr + 4);	//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP( * (__IO uint32_t *) App_Addr);	//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		JumpToApp();	//��ת��APP
	}else{
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
	 printf("> Choose a startup method......\r\n");	
	switch(Read_Start_Mode())									//��ȡ�Ƿ�����Ӧ�ó���
	{
		case Startup_Normal:										//��������
		{
			MoveCode(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
			break;
		}
		case Startup_Update:										//����������
		{
			printf("> Start update......\r\n");	
			MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
			printf("> Update sucessfully......\r\n");
			break;
		}
		default:																//����ʧ��
		{
			printf("> Error:%X!!!......\r\n", Read_Start_Mode());
			return;			
		}
	}	
	/* ��ת��Ӧ�ó��� */
	// __disable_irq() ;  //����Ҫ��������STM32F4��Ҫ��  ò��F105Ҳ��Ҫ   L431 ��� ȴ����Ҫ  RTOS��Ҫ
	printf("> Start up......\r\n\r\n");	
	IAP_ExecuteApp(Application_1_Addr);
}
void MoveCode1(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size)
{
    /*1.����Ŀ�ĵ�ַ*/
   // printf("> Start erase des flash......\r\n");
    //Erase_page(des_addr, 1);//ֻ����1����128k
   // printf("> Erase des flash sucessfully......\r\n");

    /*2.��ʼ����*/
    uint8_t temp[1];
	  temp[0]= 0xaa;
    printf("> Start copy......\r\n");
    for(int i = 0; i < 1; i++)
    {
       // ReadFlash((src_addr + i*1024), temp, 1024);
        WriteFlash((des_addr + i*1024), temp, 1);
    }
    printf("> Copy sucessfully......\r\n");

}
void test()
{
MoveCode1(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
}
