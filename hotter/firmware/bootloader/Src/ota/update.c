#include "update.h"
#include "stmflash.h"

/** @bieaf 	读取启动模式
	* @return mode	返回值
	*/
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash(OTA_NUM_ADDR, &mode, 1);
    return mode;
}

/**
	* @bieaf 进行程序的覆盖
	* @detail 1.擦除目的地址
	* @detail 2.源地址的代码拷贝到目的地址
	* @detail 3.擦除源地址
	*
	* @param  搬运的源地址
	* @param  搬运的目的地址
	* @return 搬运的程序大小
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
    status = Erase_page(des_addr, 2);//只擦除1扇区128k

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


/** @bieaf 	采用汇编设置栈的值
	* @return none	返回值
	*/
__asm void MSR_MSP(uint32_t ulAddr)
{
    MSR MSP, r0 //set Main Stack value
    BX r14
}

/** @bieaf 	程序跳转函数
	* @return none	返回值
	*/
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp(uint32_t App_Addr)
{
    Jump_Fun JumpToApp;

    if (((*(__IO uint32_t *)App_Addr) & 0x2FFE0000) == 0x20000000)	//检查栈顶地址是否合法.
    {
			    __HAL_RCC_PWR_CLK_DISABLE();
			 HAL_RCC_DeInit();
        JumpToApp = (Jump_Fun) * (__IO uint32_t *)(App_Addr + 4);	//用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(* (__IO uint32_t *) App_Addr);	//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        JumpToApp();	//跳转到APP
    }
    else
    {
        printf("There is None APP to jump,ERROR!!!\r\n");
    }
}

/**
	* @bieaf 进行BootLoader的启动
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

//    switch (Read_Start_Mode())									//读取是否启动应用程序
//    {
//        case Startup_Normal:										//正常启动
//        {
//            printf("> Normal Start......\r\n");
//           // MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
//            //MoveCode(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
//            break;
//        }
//        case Startup_Update:										//升级再启动
//        {
//            MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
//            break;
//        }
//        default:																//启动失败
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

