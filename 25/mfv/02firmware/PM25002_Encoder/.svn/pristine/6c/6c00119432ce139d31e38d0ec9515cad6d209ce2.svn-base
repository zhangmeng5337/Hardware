/** \file

  File Name:
    LAN9252_SPIDriver.c

  Summary:
    Contains the functional implementation of SPI Interface Driver for Microchip 
    EtherCAT LAN9252

  This is based on sample code provided by Microchip for the EL9800 kit

*******************************************************************************/

#if defined (USE_LAN9252)
#include "main.h"
#include "ecat_def.h"
#include "ecatslv.h"
#include "ecatappl.h"
//#include "status_codes.h"
//#include "spi.h"
//#include "delay.h"

#include "mfc_hw.h"
//#include "rtc_config.h"

#define SPIWriteByte(UINT8) SPIWrite(UINT8)
#define SPIReadByte() SPIRead()

#define SPIReadRegUsingCSR HW_EscRead
#define SPIWriteRegUsingCSR HW_EscWrite

#define CMD_SERIAL_READ 0x03
#define CMD_FAST_READ 0x0B
#define CMD_SERIAL_WRITE 0x02
#define CMD_SERIAL_READ_DUMMY 0
#define CMD_FAST_READ_DUMMY 1
#define ESC_CSR_CMD_REG		0x304
#define ESC_CSR_DATA_REG	0x300
#define ESC_WRITE_BYTE 		0x80
#define ESC_READ_BYTE 		0xC0
#define ESC_CSR_BUSY		0x80

/* Specify an extension for GCC based compilers */
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif


typedef union
{
	UINT16 Val;
	UINT8 v[2];
	struct
	{
		UINT8 LB;
		UINT8 HB;
	} byte;
	struct
	{
		__EXTENSION UINT8 b0:1;
		__EXTENSION UINT8 b1:1;
		__EXTENSION UINT8 b2:1;
		__EXTENSION UINT8 b3:1;
		__EXTENSION UINT8 b4:1;
		__EXTENSION UINT8 b5:1;
		__EXTENSION UINT8 b6:1;
		__EXTENSION UINT8 b7:1;
		__EXTENSION UINT8 b8:1;
		__EXTENSION UINT8 b9:1;
		__EXTENSION UINT8 b10:1;
		__EXTENSION UINT8 b11:1;
		__EXTENSION UINT8 b12:1;
		__EXTENSION UINT8 b13:1;
		__EXTENSION UINT8 b14:1;
		__EXTENSION UINT8 b15:1;
	} bits;
} UINT16_VAL, UINT16_BITS;

typedef union
{
	UINT32 Val;
	UINT16 w[2];
	UINT8  v[4];
	struct
	{
		UINT16 LW;
		UINT16 HW;
	} word;
	struct
	{
		UINT8 LB;
		UINT8 HB;
		UINT8 UB;
		UINT8 MB;
	} byte;
	struct
	{
		UINT16_VAL low;
		UINT16_VAL high;
	}wordUnion;
	struct
	{
		__EXTENSION UINT8 b0:1;
		__EXTENSION UINT8 b1:1;
		__EXTENSION UINT8 b2:1;
		__EXTENSION UINT8 b3:1;
		__EXTENSION UINT8 b4:1;
		__EXTENSION UINT8 b5:1;
		__EXTENSION UINT8 b6:1;
		__EXTENSION UINT8 b7:1;
		__EXTENSION UINT8 b8:1;
		__EXTENSION UINT8 b9:1;
		__EXTENSION UINT8 b10:1;
		__EXTENSION UINT8 b11:1;
		__EXTENSION UINT8 b12:1;
		__EXTENSION UINT8 b13:1;
		__EXTENSION UINT8 b14:1;
		__EXTENSION UINT8 b15:1;
		__EXTENSION UINT8 b16:1;
		__EXTENSION UINT8 b17:1;
		__EXTENSION UINT8 b18:1;
		__EXTENSION UINT8 b19:1;
		__EXTENSION UINT8 b20:1;
		__EXTENSION UINT8 b21:1;
		__EXTENSION UINT8 b22:1;
		__EXTENSION UINT8 b23:1;
		__EXTENSION UINT8 b24:1;
		__EXTENSION UINT8 b25:1;
		__EXTENSION UINT8 b26:1;
		__EXTENSION UINT8 b27:1;
		__EXTENSION UINT8 b28:1;
		__EXTENSION UINT8 b29:1;
		__EXTENSION UINT8 b30:1;
		__EXTENSION UINT8 b31:1;
	} bits;
} UINT32_VAL;

//9252 HW DEFINES
#define ECAT_REG_BASE_ADDR              0x0300

#define CSR_DATA_REG_OFFSET             0x00
#define CSR_CMD_REG_OFFSET              0x04
#define PRAM_READ_ADDR_LEN_OFFSET       0x08
#define PRAM_READ_CMD_OFFSET            0x0c
#define PRAM_WRITE_ADDR_LEN_OFFSET      0x10
#define PRAM_WRITE_CMD_OFFSET           0x14

#define PRAM_SPACE_AVBL_COUNT_MASK      0x1f
#define IS_PRAM_SPACE_AVBL_MASK         0x01


#define CSR_DATA_REG                    ECAT_REG_BASE_ADDR+CSR_DATA_REG_OFFSET
#define CSR_CMD_REG                     ECAT_REG_BASE_ADDR+CSR_CMD_REG_OFFSET
#define PRAM_READ_ADDR_LEN_REG          ECAT_REG_BASE_ADDR+PRAM_READ_ADDR_LEN_OFFSET
#define PRAM_READ_CMD_REG               ECAT_REG_BASE_ADDR+PRAM_READ_CMD_OFFSET
#define PRAM_WRITE_ADDR_LEN_REG         ECAT_REG_BASE_ADDR+PRAM_WRITE_ADDR_LEN_OFFSET
#define PRAM_WRITE_CMD_REG              ECAT_REG_BASE_ADDR+PRAM_WRITE_CMD_OFFSET

#define PRAM_READ_FIFO_REG              0x04
#define PRAM_WRITE_FIFO_REG             0x20

#define HBI_INDEXED_DATA0_REG           0x04
#define HBI_INDEXED_DATA1_REG           0x0c
#define HBI_INDEXED_DATA2_REG           0x14

#define HBI_INDEXED_INDEX0_REG          0x00
#define HBI_INDEXED_INDEX1_REG          0x08
#define HBI_INDEXED_INDEX2_REG          0x10

#define HBI_INDEXED_PRAM_READ_WRITE_FIFO    0x18

#define PRAM_RW_ABORT_MASK      /*0x40000000*/((UINT32)1 << 30)
#define PRAM_RW_BUSY_32B        /*0x80000000*/((UINT32)1 << 31)
#define PRAM_RW_BUSY_8B         (1 << 7)
#define PRAM_SET_READ           (1 << 6)
#define PRAM_SET_WRITE          0


//extern void spi_select_device(Spi *p_spi, struct spi_device *device);
//extern struct spi_device spi_dev_ET1100;
extern UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc

//void CSLOW(void) {
//	spi_select_device(SPI_MASTER_BASE, &spi_dev_ET1100);
//}

//void CSHIGH(void) {
//	spi_deselect_device(SPI_MASTER_BASE, &spi_dev_ET1100);
//}

void Delay(UINT16 Count)
{
    do
    {
//        asm volatile ("nop");
      __nop();
        
    } while (Count--);
}

void SPIWrite(UINT8 data)
{
uint8_t dummy;

//	char dummy_byte;
// 	uint32_t timeout = SPI_TIMEOUT;
	
    //SPIPut(data);
    //SPIGet();

  HAL_SPI_Transmit(&ECAT_IF, &data, 1, 1000);  
//	spi_write(SPI_MASTER_BASE, data, dummy,0);
  //HAL_SPI_Receive(&ECAT_IF, &dummy, 1, 1000);  	
//	spi_read(SPI_MASTER_BASE, &dummy_byte, &dummy);
	
}
UINT8 SPIRead(void) {
    uint8_t dummy=0;
    UINT8 data;
  
  //HAL_SPI_Transmit(&ECAT_IF, &dummy, 1, 1000);  
	//spi_write(SPI_MASTER_BASE, 0, dummy,0);
  HAL_SPI_Receive(&ECAT_IF, &data, 1, 1000);  	
//    spi_read(SPI_MASTER_BASE, &data, &dummy);

    return (data);
}

static UINT32 SPIReadDWord (UINT16 Address)
{
	UINT32_VAL dwResult;
	UINT16_VAL wAddr;
	

	wAddr.Val  = Address;
	//Assert CS line
	CSLOW();
	//Write Command
	SPIWriteByte(CMD_FAST_READ);
	//Write Address
	SPIWriteByte(wAddr.byte.HB);
	SPIWriteByte(wAddr.byte.LB);
	
	//dummy Write Command
	SPIWriteByte(0);
	
//delay_us(15);
  Delay(15);
    // dummy read
//	dwResult.byte.LB = SPIReadByte();

	//Read Bytes
	dwResult.byte.LB = SPIReadByte();
	dwResult.byte.HB = SPIReadByte();
	dwResult.byte.UB = SPIReadByte();
	dwResult.byte.MB = SPIReadByte();
	//De-Assert CS line
	CSHIGH();
	
	return dwResult.Val;
}

static UINT32 SPIReadDWord_fast (UINT16 Address)
{
    UINT32_VAL dwResult;
    UINT16_VAL wAddr;
	

    wAddr.Val  = Address;
    //Assert CS line
    CSLOW();
    //Write Command
    SPIWriteByte(CMD_FAST_READ);
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
    
    //Dummy Byte
    SPIWriteByte(CMD_FAST_READ_DUMMY);
    //Read Bytes
    dwResult.byte.LB = SPIReadByte();
    dwResult.byte.HB = SPIReadByte();
    dwResult.byte.UB = SPIReadByte();
    dwResult.byte.MB = SPIReadByte();
    //De-Assert CS line
    CSHIGH();
   
    return dwResult.Val;
}

static void SPISendAddr (UINT16 Address) {
    UINT16_VAL wAddr;

    wAddr.Val  = Address;
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
}

UINT32 SPIReadBurstMode (int something_we_dont_use) {
    UINT32_VAL dwResult;
    //Read Bytes
    dwResult.byte.LB = SPIReadByte();
    dwResult.byte.HB = SPIReadByte();
    dwResult.byte.UB = SPIReadByte();
    dwResult.byte.MB = SPIReadByte();
    
    return dwResult.Val;
}
void SPIWriteBurstMode (UINT32 Val)
{
    UINT32_VAL dwData;
    dwData.Val = Val;
    
    //Write Bytes
    SPIWriteByte(dwData.byte.LB);
    SPIWriteByte(dwData.byte.HB);
    SPIWriteByte(dwData.byte.UB);
    SPIWriteByte(dwData.byte.MB);
}

void SPIWriteDWord (UINT16 Address, UINT32 Val)
{
    UINT32_VAL dwData;
    UINT16_VAL wAddr;

    wAddr.Val  = Address;
    dwData.Val = Val;
    //Assert CS line
    CSLOW();
    //Write Command
    SPIWriteByte(CMD_SERIAL_WRITE);
    //Write Address
    SPIWriteByte(wAddr.byte.HB);
    SPIWriteByte(wAddr.byte.LB);
    //Write Bytes
    SPIWriteByte(dwData.byte.LB);
    SPIWriteByte(dwData.byte.HB);
    SPIWriteByte(dwData.byte.UB);
    SPIWriteByte(dwData.byte.MB);

    //De-Assert CS line
    CSHIGH();
}
//void SPIReadRegUsingCSR(UINT8 *ReadBuffer, UINT16 Address, UINT8 Count)
/** 

    @param count in bytes  CAN NOT BE GREATER THAN 4 !!!!!!!!!!!!!!!!!!!!!!!!
*/

void HW_EscRead(MEM_ADDR *ReadBuffer, UINT16 Address, UINT16 Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0;
    UINT16_VAL wAddr;
    UINT8 *pTmpData = (UINT8 *)ReadBuffer;

    if (Count > 4 )
        return;
        
    wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_READ_BYTE;

    SPIWriteDWord (ESC_CSR_CMD_REG, param32_1.Val);

    do
    {
        param32_1.Val = SPIReadDWord (ESC_CSR_CMD_REG);
		
    }while(param32_1.v[3] & ESC_CSR_BUSY);

    param32_1.Val = SPIReadDWord (ESC_CSR_DATA_REG);

    
    for(i=0;i<Count;i++)
         *pTmpData++ = param32_1.v[i];
   
    return;
}

//void SPIWriteRegUsingCSR( UINT8 *WriteBuffer, UINT16 Address, UINT8 Count)
void HW_EscWrite( MEM_ADDR *WriteBuffer, UINT16 Address, UINT16 Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0;
    UINT16_VAL wAddr;
    UINT8 *pTmpData = (UINT8 *)WriteBuffer;

    if (Count > 4 )
        return;

    for(i=0;i<Count;i++)
         param32_1.v[i] = *pTmpData++;

    SPIWriteDWord (ESC_CSR_DATA_REG, param32_1.Val);


    wAddr.Val = Address;

    param32_1.v[0] = wAddr.byte.LB;
    param32_1.v[1] = wAddr.byte.HB;
    param32_1.v[2] = Count;
    param32_1.v[3] = ESC_WRITE_BYTE;

    SPIWriteDWord (0x304, param32_1.Val);
    do
    {
        param32_1.Val = SPIReadDWord (0x304);

    }while(param32_1.v[3] & ESC_CSR_BUSY);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister(void)
{
unsigned char my_data[2];
UINT16 my_address=0x220;

    //GetInterruptRegister();
    HW_EscRead((MEM_ADDR *)&my_data[0],my_address,2);
    EscALEvent.Byte[0] = my_data[0] ;

//    my_address = 0x221;
//    HW_EscRead((MEM_ADDR *)&my_data[1],my_address,1);
    EscALEvent.Byte[1] = my_data[1] ;
    
    return EscALEvent.Word;
}


static void SPIReadPDRamRegister(UINT8 *ReadBuffer, UINT16 Address, UINT16 Count) {
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition;
    UINT8 nReadSpaceAvblCount;


    /*Reset/Abort any previous commands.*/
    param32_1.Val = (unsigned long int)PRAM_RW_ABORT_MASK;                                                 

    SPIWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);

    /*The host should not modify this field unless the PRAM Read Busy
    (PRAM_READ_BUSY) bit is a 0.*/
    do
    {
        param32_1.Val = SPIReadDWord (PRAM_READ_CMD_REG);

    }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

    /*Write address and length in the EtherCAT Process RAM Read Address and
     * Length Register (ECAT_PRAM_RD_ADDR_LEN)*/
    param32_1.w[0] = Address;
    param32_1.w[1] = Count;

    SPIWriteDWord (PRAM_READ_ADDR_LEN_REG, param32_1.Val);


    /*Set PRAM Read Busy (PRAM_READ_BUSY) bit(-EtherCAT Process RAM Read Command Register)
     *  to start read operatrion*/

    param32_1.Val = PRAM_RW_BUSY_32B; /*TODO:replace with #defines*/

    SPIWriteDWord (PRAM_READ_CMD_REG, param32_1.Val);

    /*Read PRAM Read Data Available (PRAM_READ_AVAIL) bit is set*/
    do
    {
        param32_1.Val = SPIReadDWord (PRAM_READ_CMD_REG);

    }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    nReadSpaceAvblCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Fifo registers are aliased address. In indexed it will read indexed data reg 0x04, but it will point to reg 0
     In other modes read 0x04 FIFO register since all registers are aliased*/

    /*get the UINT8 lenth for first read*/
    //Auto increment is supported in SPIO
    param32_1.Val = SPIReadDWord (PRAM_READ_FIFO_REG);
    nReadSpaceAvblCount--;
    nBytePosition = (Address & 0x03);
    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);
    memcpy(ReadBuffer+i ,&param32_1.v[nBytePosition],nlength);
    Count-=nlength;
    i+=nlength;

    //Lets do it in auto increment mode
    CSLOW();

    //Write Command
    SPIWriteByte(CMD_FAST_READ);

    SPISendAddr(PRAM_READ_FIFO_REG);
    
    //Dummy Byte
    SPIWriteByte(CMD_FAST_READ_DUMMY);

    while(Count)
    {
        param32_1.Val = SPIReadBurstMode (PRAM_READ_FIFO_REG);

        nlength = Count > 4 ? 4: Count;
        memcpy((ReadBuffer+i) ,&param32_1,nlength);

        i+=nlength;
        Count-=nlength;
        nReadSpaceAvblCount --;
    }

    CSHIGH();

    return;
}

void SPIWritePDRamRegister(UINT8 *WriteBuffer, UINT16 Address, UINT16 Count)
{
    UINT32_VAL param32_1 = {0};
    UINT8 i = 0,nlength, nBytePosition,nWrtSpcAvlCount;

    /*Reset or Abort any previous commands.*/
    param32_1.Val = PRAM_RW_ABORT_MASK;                                                

    SPIWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);

    /*Make sure there is no previous write is pending
    (PRAM Write Busy) bit is a 0 */
    do
    {
        param32_1.Val = SPIReadDWord (PRAM_WRITE_CMD_REG);

    }while((param32_1.v[3] & PRAM_RW_BUSY_8B));

    /*Write Address and Length Register (ECAT_PRAM_WR_ADDR_LEN) with the
    starting UINT8 address and length)*/
    param32_1.w[0] = Address;
    param32_1.w[1] = Count;

    SPIWriteDWord (PRAM_WRITE_ADDR_LEN_REG, param32_1.Val);

    /*write to the EtherCAT Process RAM Write Command Register (ECAT_PRAM_WR_CMD) with the  PRAM Write Busy
    (PRAM_WRITE_BUSY) bit set*/

    param32_1.Val = PRAM_RW_BUSY_32B; /*TODO:replace with #defines*/

    SPIWriteDWord (PRAM_WRITE_CMD_REG, param32_1.Val);

    /*Read PRAM write Data Available (PRAM_READ_AVAIL) bit is set*/
    do
    {
       param32_1.Val = SPIReadDWord (PRAM_WRITE_CMD_REG);

    }while(!(param32_1.v[0] & IS_PRAM_SPACE_AVBL_MASK));

    /*Check write data available count*/
    nWrtSpcAvlCount = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Write data to Write FIFO) */ 
    /*get the byte lenth for first read*/
    nBytePosition = (Address & 0x03);

    nlength = (4-nBytePosition) > Count ? Count:(4-nBytePosition);

    param32_1.Val = 0;
    memcpy(&param32_1.v[nBytePosition],WriteBuffer+i, nlength);

    SPIWriteDWord (PRAM_WRITE_FIFO_REG,param32_1.Val);

    nWrtSpcAvlCount--;
    Count-=nlength;
    i+=nlength;

    //Auto increment mode
    CSLOW();

    //Write Command
    SPIWriteByte(CMD_SERIAL_WRITE);

    SPISendAddr(PRAM_WRITE_FIFO_REG);

    while(Count)
    {
        nlength = Count > 4 ? 4: Count;
        param32_1.Val = 0;
        memcpy(&param32_1, (WriteBuffer+i), nlength);

        SPIWriteBurstMode (param32_1.Val);
        i+=nlength;
        Count-=nlength;
        nWrtSpcAvlCount--;
    }

    CSHIGH();
    return;
}
void SPIReadDRegister(UINT8 *ReadBuffer, UINT16 Address, UINT16 Count)
{
    if (Address >= 0x1000)
    {
         SPIReadPDRamRegister(ReadBuffer, Address,Count);
    }
    else
    {
         SPIReadRegUsingCSR(ReadBuffer, Address,Count);
    }
}
void SPIWriteRegister( UINT8 *WriteBuffer, UINT16 Address, UINT16 Count)
{
   
   if (Address >= 0x1000)
   {
		SPIWritePDRamRegister(WriteBuffer, Address,Count);
   }
   else
   {
		SPIWriteRegUsingCSR(WriteBuffer, Address,Count);
   }
    
}


TSYNCMAN		SyncManInfo;
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \return     pPdSyncMan        Pointer to the settings of requested SYNC Manager channel

 \brief    This function is called to read the SYNC Manager channel descriptions of the
             process data SYNC Managers.
*////////////////////////////////////////////////////////////////////////////////////////

TSYNCMAN ESCMEM * GetSyncMan( UINT8 channel )
{
    UINT16 PhysicalStartAddress;
	UINT16 Length;
	
    //HW_EscRead((MEM_ADDR *)&SyncManInfo.PhysicalStartAddress, ESC_SYNCMAN_REG_OFFSET + (channel * SIZEOF_SM_REGISTER), 2 );
    //HW_EscRead((MEM_ADDR *)&SyncManInfo.Length,               ESC_SYNCMAN_REG_OFFSET +2 + (channel * SIZEOF_SM_REGISTER), 2 );

    HW_EscRead((MEM_ADDR *)&PhysicalStartAddress, ESC_SYNCMAN_REG_OFFSET + (channel * SIZEOF_SM_REGISTER), 2 );
    HW_EscRead((MEM_ADDR *)&Length,               ESC_SYNCMAN_REG_OFFSET +2 + (channel * SIZEOF_SM_REGISTER), 2 );    
	
	SyncManInfo.AddressLength = PhysicalStartAddress & SM_ADDRESS_MASK;	
	SyncManInfo.AddressLength |= ((Length << SM_LENGTH_SHIFT) & SM_LENGTH_MASK);	
	
	HW_EscRead((MEM_ADDR *)&SyncManInfo.Settings,             ESC_SYNCMAN_REG_OFFSET +4 + (channel * SIZEOF_SM_REGISTER), 4 );

    return &SyncManInfo;
}

#endif // USE_LAN9252
