/*
*********************************************************************************************************
*
*	ģ������ : cpu�ڲ�falsh����ģ��
*	�ļ����� : bsp_cpu_flash.c
*	��    �� : V1.0
*	˵    �� : �ṩ��дCPU�ڲ�Flash�ĺ���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 Liujingtao  ��ʽ����
*
*	Copyright (C), 2013-2014, �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd.
*
*********************************************************************************************************
*/

#include "bsp_cpu_flash.h"
#include "fmc_operation.h"
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetSector
*	����˵��: ���ݵ�ַ���������׵�ַ
*	��    �Σ���
*	�� �� ֵ: �����׵�ַ
*********************************************************************************************************
*/
uint32_t bsp_GetSector(uint32_t _ulWrAddr)
{
	uint32_t sector = 0;

	sector = _ulWrAddr & SECTOR_MASK;

	return sector;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ReadCpuFlash
*	����˵��: ��ȡCPU Flash������
*	��    �Σ�_ucpDst : Ŀ�껺����
*			 _ulFlashAddr : ��ʼ��ַ
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 0=�ɹ���1=ʧ��
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpDst, uint32_t _ulSize)
{
//	uint32_t i;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* ����Ϊ0ʱ����������,������ʼ��ַΪ���ַ����� */
	if (_ulSize == 0)
	{
		return 1;
	}
	#if CPU == ST
addr = _ulFlashAddr + FLASH_BASE_ADDR;
	
	for (i = 0; i < _ulSize; i++)
	{
		*_ucpDst++ = *(uint8_t *)(addr)++;
	}
	#else
	
    

    /* read 8 bit length data from a given address */
    fmc_read_32bit_data(_ulFlashAddr+FLASH_BASE_ADDR, _ulSize, _ucpDst);
	#endif

	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CmpCpuFlash
*	����˵��: �Ƚ�Flashָ����ַ������.
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpBuf : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ:
*			FLASH_IS_EQU			0 Flash���ݺʹ�д���������ȣ�����Ҫ������д����
*			FLASH_REQ_WRITE		1	Flash����Ҫ������ֱ��д
*			FLASH_REQ_ERASE		2	Flash��Ҫ�Ȳ���,��д
*			FLASH_PARAM_ERR		3	������������
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* ��ȱ�־ */
	uint8_t ucByte;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*��������������*/
	}

	/* ����Ϊ0ʱ������ȷ */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash���ݺʹ�д���������� */
	}

	ucIsEqu = 1;			/* �ȼ��������ֽںʹ�д���������ȣ���������κ�һ������ȣ�������Ϊ 0 */
	for (i = 0; i < _ulSize; i++)
	{
		ucByte = *(uint8_t *)_ulFlashAddr;

		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFF)
			{
				return FLASH_REQ_ERASE;		/* ��Ҫ��������д */
			}
			else
			{
				ucIsEqu = 0;	/* ����ȣ���Ҫд */
			}
		}

		_ulFlashAddr++;
		_ucpBuf++;
	}

	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash����Ҫ������ֱ��д */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_WriteCpuFlash
*	����˵��: д���ݵ�CPU �ڲ�Flash��
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpSrc : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 0-�ɹ���1-���ݳ��Ȼ��ַ�����2-дFlash����(����Flash������)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint32_t *_ucpSrc, uint32_t _ulSize)
{
//	uint32_t i;
//	uint8_t ucRet;
//	uint16_t usTemp;
	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* ����Ϊ0 ʱ����������  */
	if (_ulSize == 0)
	{
		return 1;
	}
  #if CPU == ST
	for(i = 0; i<_ulSize;i++)
	{
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,_ulFlashAddr+i+FLASH_BASE_ADDR, *(uint64_t *)(_ucpSrc+i));

	}

	HAL_FLASH_Lock();
	//ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);
    return 0;
 #else
   /* write 8 bit length data to a given address */
    
	fmc_write_32bit_data(_ulFlashAddr+FLASH_BASE_ADDR, _ulSize,_ucpSrc);


	#endif

	return 0;
}
void FlashEease(uint32_t _ulFlashAddr)
{
	#if CPU == ST
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(bsp_GetSector(_ulFlashAddr), FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
	#else
    /* erases the sector of a given sector number */
    fmc_erase_sector_by_address(_ulFlashAddr+FLASH_BASE_ADDR);	
	#endif

}


    
/***************************** �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd. (END OF FILE) *********************************/
