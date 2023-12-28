#include "update.h"
#include "stmflash.h"

/** @bieaf 	读取启动模式
	* @return mode	返回值
	*/
unsigned int Read_Start_Mode(void)
{
    uint8_t mode = 0;
    ReadFlash((Application_2_Addr + Application_Size - 8), &mode, 1);
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
void MoveCode(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size)
{

	erase:Erase_page(des_addr, 2);//只擦除1扇区128k
    uint8_t temp[1024];
	  uint8_t read_temp;
	  temp[0]= 0xaa;
	  unsigned int i,j;
    //printf("> Start copy......\r\n");
    for(int i = 0; i < byte_size/1024; i++)
    {
        ReadFlash((src_addr + i*1024), temp, 1024);
        WriteFlash((des_addr + i*1024), temp, 1024);
			
			  for(j=0;j<=1024;j++)
			  {
						ReadFlash((des_addr + j+i*1024), &read_temp, 1);
						if(read_temp != temp[j])
						{
							goto erase;
						}					
				}

    }
   // printf("> Copy sucessfully......\r\n");

}


/** @bieaf 	采用汇编设置栈的值
	* @return none	返回值
	*/
__asm void MSR_MSP (uint32_t ulAddr)
{
    MSR MSP, r0 //set Main Stack value			                   
    BX r14
}

/** @bieaf 	程序跳转函数
	* @return none	返回值
	*/
typedef void (*Jump_Fun)(void);
void IAP_ExecuteApp (uint32_t App_Addr)
{
	Jump_Fun JumpToApp; 
    
	if (((*(__IO uint32_t *)App_Addr) & 0x2FFE0000) == 0x20000000)	//检查栈顶地址是否合法.
	{ 
		JumpToApp = (Jump_Fun) * (__IO uint32_t *)(App_Addr + 4);	//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP( * (__IO uint32_t *) App_Addr);	//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		JumpToApp();	//跳转到APP
	}else{
	//printf("There is None APP to jump,ERROR!!!\r\n");
	}
}

/**
	* @bieaf 进行BootLoader的启动
	* @return none
	*/
void Start_BootLoader(void)
{
//	 printf("\r\n");
//	 printf("***********************************\r\n");
//	 printf("*                                 *\r\n");
//	 printf("*           BootLoader            *\r\n");
//	 printf("*                                 *\r\n");
//	 printf("***********************************\r\n");	
//	 printf("> Choose a startup method......\r\n");	
	switch(Read_Start_Mode())									//读取是否启动应用程序
	{
		case Startup_Normal:										//正常启动
		{
			//MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
			//MoveCode(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
			break;
		}
		case Startup_Update:										//升级再启动
		{
			MoveCode(Application_2_Addr, Application_1_Addr, Application_Size);
			break;
		}
		default:																//启动失败
		{
			return;			
		}
	}	

	IAP_ExecuteApp(Application_1_Addr);
}
void MoveCode1(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size)
{
    /*1.擦除目的地址*/
   // printf("> Start erase des flash......\r\n");
    //Erase_page(des_addr, 1);//只擦除1扇区128k
   // printf("> Erase des flash sucessfully......\r\n");

    /*2.开始拷贝*/
    uint8_t temp[1];
	  temp[0]= 0xaa;
   // printf("> Start copy......\r\n");
    for(int i = 0; i < 1; i++)
    {
       // ReadFlash((src_addr + i*1024), temp, 1024);
        WriteFlash((des_addr + i*1024), temp, 1);
    }
   // printf("> Copy sucessfully......\r\n");

}
void test()
{
MoveCode1(Application_2_Addr, Application_2_Addr + Application_Size - 8, Application_Size);
}
