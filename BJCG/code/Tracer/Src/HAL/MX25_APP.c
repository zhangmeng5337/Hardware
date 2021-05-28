/*
 * COPYRIGHT (c) 2010-2017 MACRONIX INTERNATIONAL CO., LTD
 * SPI Flash Low Level Driver (LLD) Sample Code
 *
 * Application program of flash support SPI and QPI interface
 * This sample code provides a reference, not recommand for directing using.
 *
 * $Id: MX25_APP.c,v 1.4 2012/01/03 08:00:06 modelqa Exp $
 */
#include "app.h"

#include    <stdlib.h>
#include    "MX25_CMD.h"
#include "EEPROM.h"

#define  TRANS_LENGTH  16
#define  RANDOM_SEED   106
#define  FLASH_TARGET_ADDR  0x00000000
static FLashData_stru FLashData_usr;

#define  Error_inc(x)  x = x + 1;

/* function prototype */
uint8  FlashID_Test( uint8 QPI_Enable );
uint8  FlashReadWrite_Test( uint8 QPI_Enable );
unsigned char FlashDataStore2(uint32_t addr,uint8_t *pb,uint32_t len);

/*
 * Main Program
 */
void flash_process()
{
    /* Setting flash access mode: SPI or QPI */


    //FLASH_INIT();
    //Initial_Spi();
    //FlashID_Test( QPI_Enable );    // Simple test : flash ID
    // FlashReadWrite_Test( QPI_Enable );   // Simple test : flash read / write

    FlashReadWrite_Test(0);

    //FLASH_Operation_End();
}
FLashData_stru *GetFLashStatus(void)
{
    return &FLashData_usr;
}

/*
 * Simple flash id test
 */
uint8 FlashID_Test( uint8 QPI_Enable )
{
    uint32  flash_id = 0;
    uint16  error_cnt = 0;
    FlashStatus  flash_state = {0};
    ReturnMsg  msg;

    if( QPI_Enable )
        CMD_EQIO( &flash_state );    // Enter QPI mode

    /* Read flash ID */
    if( QPI_Enable )     // Call QPI command
    {
        msg = CMD_QPIID( &flash_id,&flash_state );
    }
    else
    {
        msg = CMD_RDID( &flash_id, &flash_state );
    }

    if( msg != (ReturnMsg)FlashOperationSuccess )  // Check returen message
        return FALSE;

    if( QPI_Enable )
        CMD_RSTQIO( &flash_state );  // Exit QPI mode

    if( flash_id != FlashID )  // Compare to expected value
        Error_inc( error_cnt );

    if( error_cnt != 0 )
        return FALSE;
    else
    {

        return TRUE;

    }


}
/*
 * Simple flash read/write test
 */
uint8 FlashReadWrite_Test( uint8 QPI_Enable )
{
    uint16	i=0;
    uint32  trans_len = 0;
    uint32_t	memory_addr[TRANS_LENGTH] = {0};
    uint32_t	memory_addr_cmp[TRANS_LENGTH] = {0};
    uint32  flash_addr;
    trans_len = TRANS_LENGTH;
    for( i=0; i< (trans_len); i=i+1 )
    {
        memory_addr[i] = i;	// generate random byte data
    }
    flash_addr = 0x000000;

    FlashDataStore(flash_addr,memory_addr,TRANS_LENGTH);
    FlashReadOnebytes(flash_addr*4,(uint8_t *)memory_addr_cmp,TRANS_LENGTH*4,0);

    flash_addr = 4096-16;
    memset(memory_addr_cmp,0,TRANS_LENGTH);
    FlashDataStore(flash_addr,memory_addr,TRANS_LENGTH);
    FlashReadOnebytes(flash_addr*4,(uint8_t *)memory_addr_cmp,TRANS_LENGTH*4,0);

//    FlashStatus  flash_state = {0};
//    uint32  flash_addr;
//    uint32  trans_len = 0;
//    uint16  i=0, error_cnt = 0;
//    uint16  seed = 0;
//    uint8   st_reg = 0;


//    /* Assign initial condition */
//    flash_addr = 0x3ffffff-16+1;
//    trans_len = TRANS_LENGTH;
//    seed = RANDOM_SEED;

//    if( QPI_Enable )
//        CMD_EQIO( &flash_state );    // Enter QPI mode

//    /* Prepare data to transfer */
//    srand( seed );
//    for( i=0; i< (trans_len); i=i+1 )
//    {
//        memory_addr[i] = rand()%256;    // generate random byte data
//    }

//
//    /* Mmarked these code when not using quad IO mode (except QPI mode)
//     * Enable Quad IO mode
//     * Read status register value */
//    CMD_RDSR( &st_reg, &flash_state );
//    st_reg = st_reg | FLASH_QE_MASK;    // set QE bit to 1
//    CMD_WRSR( st_reg, &flash_state );    // write setting to flash

//    /* Check QE value */
//    CMD_RDSR( &st_reg, &flash_state );
//    if( (st_reg & FLASH_QE_MASK) != FLASH_QE_MASK )
//        Error_inc( error_cnt );
//

//    /* Erase 4K sector of flash memory
//       Note: It needs to erase dirty sector before program */
//    CMD_SE( flash_addr, &flash_state );

//    /* Program data to flash */
//    if( QPI_Enable == 1)
//        CMD_PP( flash_addr, memory_addr, trans_len, &flash_state );
//    else
//    {
//        // CMD_PP( flash_addr, memory_addr, trans_len, &flash_state );
//        CMD_4PP( flash_addr, memory_addr, trans_len, &flash_state );    // need set QE bit
//    }

//    /* Read flash data to memory buffer */
//    if ( QPI_Enable == 1)
//    {
//        CMD_4READ( flash_addr, memory_addr_cmp, trans_len, &flash_state );
//    }
//    else
//    {
//        //Non-QPI mode have different read instruction options:
//        CMD_READ( flash_addr, memory_addr_cmp, trans_len, &flash_state );
//        //CMD_FASTREAD( flash_addr, memory_addr_cmp, trans_len, &flash_state );
//        //CMD_2READ( flash_addr, memory_addr_cmp, trans_len, &flash_state );
//        //CMD_4READ( flash_addr, memory_addr_cmp, trans_len, &flash_state );    // need set QE bit
//    }

//    /* Compare flash data and patten data */
//    for( i=0; i < trans_len; i=i+1 )
//    {
//        if( memory_addr[i] != memory_addr_cmp[i] )
//            Error_inc( error_cnt );
//    }

//    /* Erase 4K sector of flash memory */
//    CMD_SE( flash_addr, &flash_state );

//    if( QPI_Enable )
//        CMD_RSTQIO( &flash_state );    // Exit QPI mode

//    if( error_cnt != 0 )
//        return FALSE;
//    else
//        return TRUE;

return 0;
}
void FlashwriteOnebytes(uint32_t addr,uint8_t *pb,uint32_t len)
{
    uint8	 st_reg = 0;
    FlashStatus  flash_state = {0};
    uint16   error_cnt = 0;
    if(len != 0)
    {
        uint32_t pagecount,addrTmp,writelen,index,i,pageOffsetaddr;
        index = 0;
        //pagecount = (addr+len);
        /***********计算首地址在本页中的偏移地址****************/
        pagecount = addr/FLASH_PAGESIZE;
        addrTmp = addr;
        pageOffsetaddr = pagecount*FLASH_PAGESIZE;
        pageOffsetaddr = addrTmp-pageOffsetaddr;

        if((pageOffsetaddr+len)>FLASH_PAGESIZE)//跨页写
        {
            pagecount = pageOffsetaddr+len;
            pagecount = pagecount/FLASH_PAGESIZE;
            for(i = 0; i<pagecount; i++)
            {

                writelen = FLASH_PAGESIZE-pageOffsetaddr;//计算本页需要写的数据长度 
                



                /* Mmarked these code when not using quad IO mode (except QPI mode)
                * Enable Quad IO mode
                * Read status register value */
                CMD_RDSR( &st_reg, &flash_state );
                st_reg = st_reg | FLASH_QE_MASK;    // set QE bit to 1
                CMD_WRSR( st_reg, &flash_state );	// write setting to flash

                /* Check QE value */
                CMD_RDSR( &st_reg, &flash_state );
                if( (st_reg & FLASH_QE_MASK) != FLASH_QE_MASK )
                    Error_inc( error_cnt );


                /* Erase 4K sector of flash memory
                  Note: It needs to erase dirty sector before program */
                // CMD_SE( addr, &flash_state );
                // CMD_PP( flash_addr, memory_addr, trans_len, &flash_state );

                CMD_4PP( addrTmp, pb+index, writelen, &flash_state );	  // need set QE bit
                index = index + writelen;//记录当前数据索引
                addrTmp = addrTmp + writelen;//记录地址

                pageOffsetaddr = pageOffsetaddr+writelen;	
                pageOffsetaddr =FLASH_PAGESIZE -  pageOffsetaddr;								
			          //pageOffsetaddr = addrTmp-pageOffsetaddr;
			   

            }
            writelen = addr+len-addrTmp;//记录非跨页数据长度

            /* Mmarked these code when not using quad IO mode (except QPI mode)
            * Enable Quad IO mode
            * Read status register value */
            CMD_RDSR( &st_reg, &flash_state );
            st_reg = st_reg | FLASH_QE_MASK;	// set QE bit to 1
            CMD_WRSR( st_reg, &flash_state );	 // write setting to flash

            /* Check QE value */
            CMD_RDSR( &st_reg, &flash_state );
            if( (st_reg & FLASH_QE_MASK) != FLASH_QE_MASK )
                Error_inc( error_cnt );


            /* Erase 4K sector of flash memory
               Note: It needs to erase dirty sector before program */
            // CMD_SE( addr, &flash_state );
            // CMD_PP( flash_addr, memory_addr, trans_len, &flash_state );
            CMD_4PP( addrTmp, pb+index, writelen, &flash_state );	   // need set QE bit
        }
        else
        {
            CMD_RDSR( &st_reg, &flash_state );
            st_reg = st_reg | FLASH_QE_MASK;	// set QE bit to 1
            CMD_WRSR( st_reg, &flash_state );	 // write setting to flash

            /* Check QE value */
            CMD_RDSR( &st_reg, &flash_state );
            if( (st_reg & FLASH_QE_MASK) != FLASH_QE_MASK )
                Error_inc( error_cnt );


            /* Erase 4K sector of flash memory
               Note: It needs to erase dirty sector before program */
            // CMD_SE( addr, &flash_state );
            // CMD_PP( flash_addr, memory_addr, trans_len, &flash_state );
            CMD_4PP( addr, pb, len, &flash_state );	   // need set QE bit

        }

    }


}

uint8 FlashReadOnebytes(uint32_t addr,uint8_t *pb,uint32_t len,unsigned char recordFlag)
{
    FlashStatus  flash_state = {0};


    uint16   error_cnt = 0;
    uint8   st_reg = 0;


    /* Assign initial condition */
    if(len>0)
    {
        FLASH_INIT();
        //Initial_Spi();



        /* Mmarked these code when not using quad IO mode (except QPI mode)
         * Enable Quad IO mode
         * Read status register value */
        CMD_RDSR( &st_reg, &flash_state );
        st_reg = st_reg | FLASH_QE_MASK;    // set QE bit to 1
        CMD_WRSR( st_reg, &flash_state );    // write setting to flash

        /* Check QE value */
        CMD_RDSR( &st_reg, &flash_state );

        if( (st_reg & FLASH_QE_MASK) != FLASH_QE_MASK )
            Error_inc( error_cnt );
        if((addr+len)>DATA_MAX_ADDR)
        {
            uint32_t ReadLen,index;
            ReadLen = DATA_MAX_ADDR - addr+1;
            CMD_READ( addr, pb, ReadLen, &flash_state );
            index = ReadLen;

            ReadLen = addr+len;//3   5  5
            ReadLen = ReadLen - DATA_MAX_ADDR-1  ;
            CMD_READ( 0, pb+index, ReadLen, &flash_state );

        }
        else
        {

            CMD_READ( addr, pb, len, &flash_state );

        }
        if(recordFlag == 1)
        {

            if(FLashData_usr.SumLen >= len)
                FLashData_usr.SumLen = FLashData_usr.SumLen- len;
            else
                FLashData_usr.SumLen = 0;
            FLashData_usr.LastReadAddr = FLashData_usr.LastReadAddr + len;
            if(FLashData_usr.LastReadAddr> DATA_MAX_ADDR)
                FLashData_usr.LastReadAddr = FLashData_usr.LastReadAddr - DATA_MAX_ADDR;

        }
        FLASH_Operation_End();

        if( error_cnt != 0 )
            return FALSE;
        else
            return TRUE;
    }
    else
        return 1;



}

static unsigned char swap[DATA_4K_SIZE];
unsigned char FlashDataStore(uint32_t addr,uint32_t *pb,uint32_t len)
{
    FlashStatus  flash_state = {0};



    uint32_t sectorsNO,sectorsAddr,lenNoOverWrite,writeLen,writeAddr;
    unsigned char *pbuf;
    pbuf = (unsigned char *)pb;

    if((addr*4)<DATA_MAX_ADDR)//地址合法性验证数据4字节存储，所以地址最小单位需要乘以4
    {

        FLASH_INIT();


        sectorsAddr = addr/4096*4096;//计算扇区首地址
			  if(sectorsAddr>=0x000ffff)
					sectorsAddr = sectorsAddr;
				sectorsNO = sectorsAddr/4096;
        writeAddr = addr;//写入数据首地址
        lenNoOverWrite = writeAddr-sectorsAddr;//不需要覆盖数据长度
        writeLen = len*4;//要写入的数据长度
        FlashReadOnebytes(sectorsAddr,swap,DATA_4K_SIZE,0);//读出扇区已写入数据
        CMD_SE( sectorsAddr, &flash_state );//擦除扇区
        FlashwriteOnebytes(sectorsAddr,swap,lenNoOverWrite);//写入不需要覆盖的数据

        FLashData_usr.LastWriteAddr = FLashData_usr.LastWriteAddr + writeLen;
        FLashData_usr.SumLen = FLashData_usr.SumLen +writeLen;//数据帧数量

        if((writeLen+lenNoOverWrite)<=DATA_4K_SIZE)//数据不超过4k一个扇区
        {
            FlashwriteOnebytes(writeAddr,pbuf,writeLen);


        }
        else if ((writeLen+lenNoOverWrite)<=(2*DATA_4K_SIZE))//数据跨扇区
        {
            FlashwriteOnebytes(writeAddr,pbuf,DATA_4K_SIZE-lenNoOverWrite);//第一个扇区写入数据
            sectorsAddr = (addr+writeLen)/4096;//计算扇区首地址
            sectorsAddr = sectorsAddr*4096;//计算扇区首地址
					  sectorsNO = sectorsAddr/4096;            if(sectorsNO>DATA_MAX_SECTORS)
                sectorsAddr = 0;
            writeLen = writeLen + lenNoOverWrite;
            writeLen = writeLen - DATA_4K_SIZE;  
            FlashReadOnebytes(sectorsAddr,swap,DATA_4K_SIZE,0);//读出扇区已写入数据
            FlashwriteOnebytes(sectorsAddr,pbuf+((DATA_4K_SIZE-lenNoOverWrite)),writeLen);//第二扇区写入要覆盖的数据
            FlashwriteOnebytes(sectorsAddr+writeLen,swap+writeLen,DATA_4K_SIZE-writeLen);//第二扇区写入不需要覆盖的数据

        }

        //flash_init(0);
        systmeReconfig();
        FLASH_Operation_End();

        return 0;

    }

    else
        return 1;
    //FLashData_usr.SumLen = FLashData_usr.SumLen +1;//数据帧数量
}
/*unsigned char FlashDataStore2(uint32_t addr,uint8_t *pb,uint32_t len)
{
    FlashStatus  flash_state = {0};

     unsigned char *swap;

    uint32_t tmp,tmp2,tmp3;
    unsigned char *pbuf;
    pbuf = pb;

    if((addr*4)<DATA_MAX_ADDR)//地址合法性验证
    {

		FLASH_INIT();

        swap = malloc(DATA_4K_SIZE);
        memset(swap,0,DATA_4K_SIZE);


        tmp = addr/1024*4096;//计算扇区首地址
        tmp2 = addr*4-tmp;//不需要覆盖数据长度
        tmp3 = len*4;//要写入的数据长度
        FlashReadOnebytes(tmp,swap,DATA_4K_SIZE);//读出扇区已写入数据
        CMD_SE( addr, &flash_state );//擦除扇区
        Flashwrite4bytes(tmp,swap,tmp2);//写入不需要覆盖的数据

        if((tmp3+tmp2)<=DATA_4K_SIZE)//数据不超过4k一个扇区
        {
            Flashwrite4bytes(addr*4,pbuf,tmp3);
			FLashData_usr.LastWriteAddr = FLashData_usr.LastWriteAddr + addr*4+tmp3;
        }
        else//数据跨扇区
        {
            Flashwrite4bytes(addr*4,pbuf,DATA_4K_SIZE-tmp2);//第一个扇区

            tmp = (addr+len)/1024*4096;//计算扇区首地址
            if(tmp>DATA_MAX_SECTORS)
				    tmp = 0;
            tmp3 = DATA_4K_SIZE-tmp2;//第二扇区要写入数据偏移地址
            tmp2 = len*4+tmp2- DATA_4K_SIZE;
            FlashRead4bytes(tmp,swap,DATA_4K_SIZE);//读出扇区已写入数据
            Flashwrite4bytes(tmp,pbuf+((unsigned char)(tmp3)),tmp2);//写入要覆盖的数据
            Flashwrite4bytes(tmp+tmp2,swap+tmp2,DATA_4K_SIZE-tmp2);//写入要覆盖的数据
            FLashData_usr.LastWriteAddr = FLashData_usr.LastWriteAddr + DATA_4K_SIZE+tmp;
        }
        FLashData_usr.SumLen = FLashData_usr.SumLen +len*4;//数据帧数量
       flash_init(0);
	   uint32_t eeprom_addr,tmp4;
	   eeprom_addr = 0;
	   tmp4 = 0x5a;
	   flash_write(eeprom_addr++,&tmp4,1);//写入数据头
	   flash_write(eeprom_addr++,&FLashData_usr.LastWriteAddr,1);//
	   flash_write(eeprom_addr++,&FLashData_usr.SumLen,1);//
	    FLASH_Operation_End();
       return 0;
	   free(swap);
    }

    else
        return 1;
    FLashData_usr.SumLen = FLashData_usr.SumLen +1;//数据帧数量
}*/
void EraseChip()
{
    FlashStatus  flash_state = {0};

    CMD_CE( &flash_state );//擦除扇区

}
