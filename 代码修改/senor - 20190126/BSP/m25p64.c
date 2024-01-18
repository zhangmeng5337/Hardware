#include "M25p64.h"
//#include "spi.h"
#include "stm32l1xx.h"
#include "Flash_BSP.h"
#include "global.h"
#include "flash.h"
//*+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+*
//��Ŀ����:M25P64оƬ����
//����˵��:�ⲿFLASH�洢����
//Ӳ��˵��:STM32F429����оƬƽ̨
//�汾ʱ��:2016/09/07-?
//������Ա:����ɽ@Halo
//*+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+*



uint32_t M25PXX_TYPE=0x202017;	//Ĭ����M25P64
uint32_t WriteAddressPostion;

//------------------------------------------------------------------------------------------------------------------------------
/************************************************************
*��������:void M25PXX_Init(void)
*��    ��:M25P64��ʼ������
*˵    ��:��Ҫ�ǳ�ʼ��IO��SPI1ͨ��
*�������:��
*�������:��
*************************************************************/
uint32_t tmp;
void M25PXX_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 SPI1_Init();

	//SPI1_ReadWriteByte(0Xff);                           //��������
	//W25QXX_CS(1);			                //SPI FLASH��ѡ��
	while(tmp!=M25P32)
 		tmp=M25PXX_ReadID();
	//SPI1_Init();		   			        //��ʼ��SPI
	//SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_2); //����Ϊ45Mʱ��,����ģʽ
	//M25PXX_TYPE=M25PXX_ReadID();	        //��ȡFLASH 24λID.
}

/************************************************************
*��������:unsigned char M25PXX_ReadSR(void)
*��    ��:��ȡM25PXX��״̬�Ĵ���
*˵    ��:��
*�������:��
*�������:byte=���ض�ȡ��״̬����
*************************************************************/
//unsigned char byte=0x01;
unsigned char M25PXX_ReadSR(void)
{
	unsigned char byte=0;
	W25QXX_CS(0);                             //ʹ������
	SPI1_ReadWriteByte(M25X_RDSR);          //���Ͷ�ȡ״̬�Ĵ�������
	do
	{
	  /* Send a dummy byte to generate the clock needed by the FLASH
	  and put the value of the status register in FLASH_Status variable */
	  byte=SPI1_ReadWriteByte(Dummy_Byte);    //��ȡһ���ֽ�
	
	}
	while ((byte & WIP_Flag) == 0x01); /* Write in progress */

	//printf("оƬ״̬:%d \r\n",byte); //��ʾоƬ��״̬
	W25QXX_CS(1);							//ȡ��Ƭѡ
  
	return byte;
}
/************************************************************
*��������:void M25PXX_Write_SR(unsigned char sr)
*��    ��:дM25PXX״̬�Ĵ���
*˵    ��:��
*�������:sr=Ҫд���״̬
*�������:��
*************************************************************/
void M25PXX_Write_SR(unsigned char sr)
{
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_WRSR);          //����дȡ״̬�Ĵ�������
	SPI1_ReadWriteByte(sr);                 //д��һ���ֽ�
	W25QXX_CS(1);                            //ȡ��Ƭѡ
}
/************************************************************
*��������:void M25PXX_Wait_Busy(void)
*��    ��:�ȴ�оƬ����
*˵    ��:��
*�������:��
*�������:��
*************************************************************/
void M25PXX_Wait_Busy(void)
{
	M25PXX_ReadSR();   // �ȴ�����
}
/************************************************************
*��������:void M25PXX_Write_Enable(void)
*��    ��:M25PXXдʹ��
*˵    ��:ÿ��д�����ݶ�Ҫ��WEL��λһ�£�������д
*�������:��
*�������:��
*************************************************************/
void M25PXX_Write_Enable(void)
{
	W25QXX_CS(0);                             //ʹ������
	SPI1_ReadWriteByte(M25X_WREN);          //����дʹ��
	//W25QXX_CLK(1);

	W25QXX_CS(1);                              //ȡ��Ƭѡ
	//W25QXX_SDO(0);
	//delay_us(100);
}

/************************************************************
*��������:void M25PXX_Write_Disable(void)
*��    ��:M25PXXд��ֹ
*˵    ��:��WEL����
*�������:��
*�������:��
*************************************************************/
void M25PXX_Write_Disable(void)
{
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_WRDI); 		    //����д��ָֹ��
	W25QXX_CS(1);                            //ȡ��Ƭѡ
}
/************************************************************
*��������:void M25PXX_Erase_Sector(uint32_t Dst_Addr)
*��    ��:����һ������
*˵    ��:����һ������������ʱ��:150ms
*�������:Dst_Addr:������ַ ����ʵ����������
*�������:��
*************************************************************/
uint32_t ttt;
void M25PXX_Erase_Sector(uint32_t Dst_Addr)
{
	//unsigned char addr_tmp;
	//����falsh�������,������
	//printf("fe:%x\r\n",Dst_Addr);
	Dst_Addr*=M25PXX_SECTOR_BYTES_SIZE; //����*ҳ(64K)
	//M25PXX_Wait_Busy();
	M25PXX_Write_Enable();            //дʹ��
	W25QXX_CS(0);                          //ʹ������
	SPI1_ReadWriteByte(M25X_SE);   			//������������ָ��
	SPI1_ReadWriteByte((Dst_Addr&0x00FF0000)>>16);  //����24bit��ַ
	SPI1_ReadWriteByte((Dst_Addr&0x0000FF00)>>8);
	SPI1_ReadWriteByte((Dst_Addr&0x000000FF));
	W25QXX_CS(1);                          //ȡ��Ƭѡ
	//M25PXX_Write_Disable();
	delay_ms(1) ;
	M25PXX_Wait_Busy();   				    //�ȴ�оƬ����
}
/************************************************************
*��������:void M25PXX_Erase_Chip(void)
*��    ��:��������оƬ
*˵    ��:�ȴ�ʱ�䳬��...
*�������:��
*�������:��
*************************************************************/
void M25PXX_Erase_Chip(void)
{
	M25PXX_Write_Enable();                  //дʹ��
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_BE);            //����Ƭ��������
	W25QXX_CS(1);
	//delay_ms(200) ;                             //ȡ��Ƭѡ
	M25PXX_Wait_Busy();   				    //�ȴ�оƬ��������
}
/*****************************************************************************
*��������:void M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,unsigned int NumByteToRead)
*��    ��:��ȡSPI FLASH
*˵    ��:��ָ����ַ��ʼ��ȡָ�����ȵ�����
*�������:pBuffer:���ݴ洢����ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)��
		 NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
*�������:��
******************************************************************************/
uint32_t read_count;
unsigned char M25PXX_Read_Length(unsigned int* pBuffer)
{
        unsigned char pb[4];
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_READ);      	//���Ͷ�ȡ����
	SPI1_ReadWriteByte((0 & 0x00FF0000)>>16);   //����24bit��ַ
	SPI1_ReadWriteByte((0& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(0& 0x000000FF);
	pb[0]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pb[1]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pb[2]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pb[3]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	W25QXX_CS(1);

	if(pb[0]==0xff&&pb[1]==0xff&&pb[2]==0xff&&pb[3]==0xff)
	{
	   // return 0;
		return 0;
	}
	else
	{
		*pBuffer=pb[0]<<24+pb[1]<<16+pb[2]<<8+pb[3];
		return 1;

	}


}
unsigned char M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
	uint32_t i;
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_READ);      	//���Ͷ�ȡ����
	SPI1_ReadWriteByte((ReadAddr & 0x00FF0000)>>16);   //����24bit��ַ
	SPI1_ReadWriteByte((ReadAddr& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(ReadAddr& 0x000000FF);
	/*pBuffer[0]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[1]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[2]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[3]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������*/
	/*if(pBuffer[0]==0xff&&pBuffer[1]==0xff&&pBuffer[2]==0xff&&pBuffer[3]==0xff)
	{
	   // return 0;
	
	    NumByteToRead=0;
		for(i=4; i<NumByteToRead; i++)
		{
			read_count=i;
			pBuffer[i]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
		}
			W25QXX_CS(1);
		return 0;
	}
		
	else*/
	{
		//NumByteToRead= pBuffer[0]<<24+pBuffer[1]<<16+pBuffer[2]<<8+pBuffer[3];
		for(i=0; i<NumByteToRead; i++)
		{
			read_count=i;
			pBuffer[i]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
		}
			W25QXX_CS(1);
		return 1;

	}


}
/*****************************************************************************
*��������:void M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,unsigned int NumByteToRead)
*��    ��:��ȡSPI FLASH
*˵    ��:��ָ����ַ��ʼ��ȡָ�����ȵ�����
*�������:pBuffer:���ݴ洢����ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)��
		 NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
*�������:��
******************************************************************************/
uint32_t read_count;
unsigned char M25PXX_Read_len(uint32_t *NumByteToRead)
{
//	uint32_t i;
	unsigned char pBuffer[4];
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_READ);      	//���Ͷ�ȡ����
	SPI1_ReadWriteByte((0 & 0x00FF0000)>>16);   //����24bit��ַ
	SPI1_ReadWriteByte((0& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(0& 0x000000FF);
	pBuffer[0]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[1]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[2]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	pBuffer[3]=SPI1_ReadWriteByte(Dummy_Byte);    //ѭ��������
	if(pBuffer[0]==0xff&&pBuffer[1]==0xff&&pBuffer[2]==0xff&&pBuffer[3]==0xff)
	{
	   // return 0;
	
		*NumByteToRead=0;
		return 0;
	}
		
	else
	{
        *NumByteToRead=pBuffer[0]<<24+pBuffer[1]<<16+pBuffer[2]<<8+pBuffer[3];
		return 1;

	}


}

/*****************************************************************************
*��������:void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
*��    ��:SPI��һҳ(0~65535)��д������256���ֽڵ�����
*˵    ��:��ָ����ַ��ʼд�����256�ֽڵ�����
*�������:pBuffer:���ݴ洢����WriteAddr:��ʼд��ĵ�ַ(24bit)��
         NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
*�������:��
******************************************************************************/
uint32_t write_count;
void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
{
	unsigned int i;
	//M25PXX_Wait_Busy();
	M25PXX_Write_Enable();                  //SET WEL
	//M25PXX_Wait_Busy();
	W25QXX_CS(0);                            //ʹ������
	SPI1_ReadWriteByte(M25X_PP);   			//����дҳ����
	SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>16)); //����24bit��ַ
	SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>8));
	SPI1_ReadWriteByte((unsigned char)WriteAddr);
	
		for(i=0; i<NumByteToWrite; i++)
		{
			SPI1_ReadWriteByte(pBuffer[i]); //ѭ��д����
			write_count = write_count+1;
		}
	
		W25QXX_CS(1);
		delay_ms(1) ;						  //ȡ��Ƭѡ
		M25PXX_Wait_Busy(); 					//�ȴ�д�����
	}

/******************************************************************************/
void M25PXX_Write_NoCheck(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	uint32_t pageremain;
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{
		M25PXX_Write_Page(pBuffer,WriteAddr,pageremain); //дҳ
		if(NumByteToWrite==pageremain)break;//д�������
		else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};
}


/*****************************************************************************
*��������:void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
*��    ��:дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ�����
*˵    ��:�ú�������������!
*�������:pBuffer:���ݴ洢����WriteAddr:��ʼд��ĵ�ַ(24bit)��
         NumByteToWrite:Ҫд����ֽ���(���65535)
*�������:��
******************************************************************************/

	unsigned char * M25PXX_BUF;
	unsigned char M25PXX_BUFFER[1024];
void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite,u8 ease_flag)
{
	uint32_t secpos;
	uint32_t secoff;
	uint32_t secremain;
	uint32_t i;



	M25PXX_BUF=M25PXX_BUFFER;	//�õ���ֵ������
	secpos=WriteAddr/M25PXX_SECTOR_BYTES_SIZE;		//������ַ
	secoff=WriteAddr%M25PXX_SECTOR_BYTES_SIZE;		//�������ڵ�ƫ��
	secremain=M25PXX_SECTOR_BYTES_SIZE-secoff;		//����ʣ��ռ��С
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//������64K���ֽ�
	
	M25PXX_Read(M25PXX_BUF,secpos*M25PXX_SECTOR_BYTES_SIZE,secremain);//������������������
	i=NumByteToWrite+M25PXX_BUF[0]<<24+M25PXX_BUF[1]<<16+
	M25PXX_BUF[2]<<8+M25PXX_BUF[3];
	if(i>=65535)
	{
		M25PXX_BUF[0]=NumByteToWrite>>24;
		M25PXX_BUF[1]=NumByteToWrite>>16;
		M25PXX_BUF[2]=NumByteToWrite>>8;
		M25PXX_BUF[3]=NumByteToWrite;

	}
	else
	{
		M25PXX_BUF[0]=i>>24;
		M25PXX_BUF[1]=i>>16;
		M25PXX_BUF[2]=i>>8;
		M25PXX_BUF[3]=i;

	}
     M25PXX_BUF=pBuffer;

         while(1)
	 {
		
		if(i<secremain)//��Ҫ����
		{
			M25PXX_Erase_Sector(secpos);//�����������
			/*for(i=0; i<secremain; i++)	 //����
			{
				M25PXX_BUF[i+secoff+4]=pBuffer[i];
			}*/	
			
			for(i=0; i<secremain; i++)	 //����
			{
				M25PXX_BUF[i+secoff]=pBuffer[i];
			}		
			M25PXX_Write_NoCheck(M25PXX_BUF,secpos*M25PXX_SECTOR_BYTES_SIZE,NumByteToWrite);//д����������

		}
		else
			M25PXX_Write_NoCheck(pBuffer,WriteAddr,secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
		if(NumByteToWrite==secremain)
			break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0

			pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��
			NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>M25PXX_SECTOR_BYTES_SIZE)
				secremain=M25PXX_SECTOR_BYTES_SIZE;	//��һ����������д����
			else
				secremain=NumByteToWrite;			//��һ����������д����
		}
	    };
}
/************************************************************
*��������:uint32_t M25PXX_ReadID(void)
*��    ��:��ȡоƬID����
*˵    ��:M25p64��8Mλ��ID��=0x202017
*�������:��
*�������:Temp=���ص�ID��
*************************************************************/
uint32_t M25PXX_ReadID(void)
{
	uint32_t Temp=0,Temp0=0,Temp1=0,Temp2=0;
	W25QXX_CS(0);
	SPI1_ReadWriteByte(M25X_RDID);		   //���Ͷ�ȡID����
	Temp0=SPI1_ReadWriteByte(Dummy_Byte);  //��һ���ֽ�
	Temp1=SPI1_ReadWriteByte(Dummy_Byte);
	Temp2=SPI1_ReadWriteByte(Dummy_Byte);
	W25QXX_CS(1);
	Temp = (Temp0<<16)|(Temp1<<8)|Temp2;   //���һ��24λ����
	//printf("M25P64��ID��0x%x��\r\n",Temp);  //M25P64��ʶ��ID��0x202017
	return Temp;
}



//void M25PXX_Write_Test()
//{
//    unsigned char *crc_pbuffer;
//	uint32_t buff_size;
//	unsigned char j;
//	uint32_t i;
//	buff_size=1024;
//	for(i=0; i<buff_size; i++)
//	{		
//			test_tx[i]=1+j++;
//	}    

// CRC_cal(test_tx,crc_pbuffer,10);
// crc_result=CRC_decode(test_tx,12);


//	M25PXX_Write((unsigned char*)test_tx,0x00000000,buff_size);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
//	M25PXX_Read(test_rx,0x00000000,buff_size);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�

//}

uint32_t Read_AddressWrite()
{
	uint32_t addr_tmp;
	//�����ڲ�flash��ȡ����
	FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080000),&addr_tmp);
	if(addr_tmp<=0x400000)
		WriteAddressPostion = addr_tmp;
	
	else
		WriteAddressPostion = 0;
		return WriteAddressPostion;
}
void SPI_FLASH_READ(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
    
    M25PXX_Read( M25PXX_BUFFER,ReadAddr,NumByteToRead);
	  pBuffer = M25PXX_BUFFER;
}
u32 wraddr_count;
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 tmp;
   Read_AddressWrite();
  if(WriteAddressPostion == 0xffffffff)	
		WriteAddressPostion = NumByteToWrite;
  else 
		WriteAddressPostion = WriteAddressPostion + NumByteToWrite;	

		#if DEBUG
			WriteAddressPostion=0;
		#endif		

		//�����ڲ�flashд����������WriteAddressPostion���ڲ�flash��
    FLASH_Write(((uint32_t)WRITEADDR),((uint32_t)WRITEADDR),&WriteAddressPostion);	
	
	M25PXX_Write_NoCheck(pBuffer,WriteAddressPostion,NumByteToWrite); 	//WriteAddr��ַ��Χ0x000003---0x1ffffff
    
}
	
void write_all_chip()
{  
    u8 p[1];
	  u32 i;
	p[0] = 0x56;
    for(i=0;i<4194303;i++)
    {
	M25PXX_Write_NoCheck(p,i,1);
   }
}


