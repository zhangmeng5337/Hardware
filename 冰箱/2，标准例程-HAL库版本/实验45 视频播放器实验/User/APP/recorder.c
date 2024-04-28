#include "./APP/recorder.h"
#include "./APP/audioplay.h"
#include "./FATFS/source/ff.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/ES8388/es8388.h"
#include "./BSP/I2S/i2s.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/text.h"
#include "./FATFS/source/ffconf.h"
#include "./FATFS/exfuns/fattester.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//¼���� Ӧ�ô���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	
uint8_t *i2srecbuf1;
uint8_t *i2srecbuf2;

FIL *f_rec = 0;         /* ¼���ļ� */
uint32_t wavsize;       /* wav���ݴ�С(�ֽ���,�������ļ�ͷ!!) */

uint8_t rec_sta = 0;    /**
                         * ¼��״̬
                         * [7]:0,û�п���¼��;1,�Ѿ�����¼��;
                         * [6:1]:����
                         * [0]:0,����¼��;1,��ͣ¼��;
                         */
    
/**
 * @brief       ¼�� I2S_DMA�����жϷ�����
 *   @note      ���ж�����д������
 * @param       ��
 * @retval      ��
 */
 
void rec_i2s_dma_rx_callback(void)
{
    uint16_t bw;
    uint8_t res;

    if (rec_sta == 0X80)  /* ¼��ģʽ */
    {
        if (DMA1_Stream3->CR & (1 << 19))
        {
            res = f_write(f_rec, i2srecbuf1, I2S_RX_DMA_BUF_SIZE, (UINT *)&bw); /* д���ļ� */

            if (res)
            {
                printf("write error:%d\r\n", res);
            }
        }
        else
        {
            res = f_write(f_rec, i2srecbuf2, I2S_RX_DMA_BUF_SIZE, (UINT *)&bw); /* д���ļ� */

            if (res)
            {
                printf("write error:%d\r\n", res);
            }
        }

        wavsize += I2S_RX_DMA_BUF_SIZE;
    }
}

const uint16_t i2splaybuf[2] = {0X0000, 0X0000}; /* 2��16λ����,����¼��ʱI2S Master����.ѭ������0. */



void recoder_enter_rec_mode(void)
{
    DMA1_Stream4->CR&=~(1<<4);	//�رմ�������ж�(���ﲻ���ж�������) 
	ES8388_ADDA_Cfg(1,0);		//����ADC
	ES8388_Input_Cfg(0);		//��������ͨ��(ͨ��1,MIC����ͨ��) 
	ES8388_MIC_Gain(8);			//MIC��������Ϊ��� 
	ES8388_ALC_Ctrl(3,4,4);		//����������ALC����,�����¼������
	ES8388_SPKvol_Set(0);		//�ر�����.
	ES8388_I2S_Cfg(0,3);		//�����ֱ�׼,16λ���ݳ���

	I2S2_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_16B);	//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ֡���� 	
	I2S2_SampleRate_Set(REC_SAMPLERATE);	//���ò����� 
 	I2S2_TX_DMA_Init((uint8_t*)&i2splaybuf[0],(uint8_t*)&i2splaybuf[1],1); 		//����TX DMA 
	DMA1_Stream4->CR&=~(1<<4);	//�رմ�������ж�(���ﲻ���ж�������) 
	I2S2ext_RX_DMA_Init(i2srecbuf1,i2srecbuf2,I2S_RX_DMA_BUF_SIZE/2); 	//����RX DMA
  	i2s_rx_callback=rec_i2s_dma_rx_callback;//�ص�����ָwav_i2s_dma_callback
 	I2S_Play_Start();	//��ʼI2S���ݷ���(����)
	I2S_Rec_Start(); 	//��ʼI2S���ݽ���(�ӻ�)
	recoder_remindmsg_show(0);
}  
//����PCM ����ģʽ 		  
void recoder_enter_play_mode(void)
{
	ES8388_ADDA_Cfg(0,1);		//����DAC 
 	ES8388_Output_Cfg(1,1);		//����ͨ��1��2�����   
	ES8388_SPKvol_Set(20);		//������������
	I2S_Play_Stop();			//ֹͣʱ�ӷ���
	I2S_Rec_Stop(); 			//ֹͣ¼��
	recoder_remindmsg_show(1);
}
//��ʼ��WAVͷ.
void recoder_wav_init(__WaveHeader* wavhead) //��ʼ��WAVͷ			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//��δȷ��,�����Ҫ����
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//��СΪ16���ֽ�
	wavhead->fmt.AudioFormat=0X01; 		//0X01,��ʾPCM;0X01,��ʾIMA ADPCM
 	wavhead->fmt.NumOfChannels=2;		//˫����
 	wavhead->fmt.SampleRate=REC_SAMPLERATE;//���ò�������
 	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*4;//�ֽ�����=������*ͨ����*(ADCλ��/8)
 	wavhead->fmt.BlockAlign=4;			//���С=ͨ����*(ADCλ��/8)
 	wavhead->fmt.BitsPerSample=16;		//16λPCM
   	wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//���ݴ�С,����Ҫ����  
} 						    
//��ʾ¼��ʱ�������
//tsec:������.
void recoder_msg_show(uint32_t tsec,uint32_t kbps)
{   
    printf("tsec : %d   kbps : %d  \r\n",tsec, kbps);
    lcd_show_string(30, 210, 200, 16, 16, "TIME:", RED);    /* ��ʾ¼��ʱ�� */
    lcd_show_num(30 + 40, 210, tsec / 60, 2, 16, RED);      /* ���� */
    lcd_show_char(30 + 56, 210, ':', 16, 0, RED);
    lcd_show_num(30 + 64, 210, tsec % 60, 2, 16, RED);      /* ���� */

    lcd_show_string(140, 210, 200, 16, 16, "KPBS:", RED);   /* ��ʾ���� */
    lcd_show_num(140 + 40, 210, kbps / 1000, 4, 16, RED);   /* ������ʾ */
}  	
//��ʾ��Ϣ
//mode:0,¼��ģʽ;1,����ģʽ
void recoder_remindmsg_show(uint8_t mode)
{
    lcd_fill(30, 120, lcddev.width, 180, WHITE);    /* ���ԭ������ʾ */

    if (mode == 0)  /* ¼��ģʽ */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:REC/PAUSE", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "KEY2:STOP&SAVE", BLUE);
        lcd_show_string(30, 160, 200, 16, 16, "WK_UP:PLAY", BLUE);
    }
    else            /* ����ģʽ */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:STOP Play", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "WK_UP:PLAY/PAUSE", BLUE);
    }
}
//ͨ��ʱ���ȡ�ļ���
//������SD������,��֧��FLASH DISK����
//��ϳ�:����"0:RECORDER/REC20120321210633.wav"���ļ���
void recoder_new_pathname(uint8_t *pname)
{	 
	uint8_t res;					 
	uint16_t index=0;
    FIL *ftemp;
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* ����FIL�ֽڵ��ڴ����� */
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECORDER/REC%05d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
} 
//WAV¼�� 
void wav_recorder(void)
{
	uint8_t res;
	uint8_t key;
	uint8_t rval=0;
    uint32_t bw;
	__WaveHeader *wavhead=0; 
 	DIR recdir;	 					//Ŀ¼  
 	uint8_t *pname=0;
	uint8_t timecnt=0;					//��ʱ��   
	uint32_t recsec=0;					//¼��ʱ�� 
	while(f_opendir(&recdir,"0:/RECORDER"))//��¼���ļ���
 	{	 
        lcd_show_string(30, 230, 240, 16, 16, "RECORDER�ļ��д���!", RED);
        delay_ms(200);
        lcd_fill(30, 230, 240, 246, WHITE);     /* �����ʾ */
        delay_ms(200);
        f_mkdir("0:/RECORDER");                 /* ������Ŀ¼ */ 
	}   
    i2srecbuf1=mymalloc(SRAMIN,I2S_RX_DMA_BUF_SIZE);//I2S¼���ڴ�1����
	i2srecbuf2=mymalloc(SRAMIN,I2S_RX_DMA_BUF_SIZE);//I2S¼���ڴ�2����  
  	f_rec=(FIL *)mymalloc(SRAMIN,sizeof(FIL));		//����FIL�ֽڵ��ڴ�����  
 	wavhead=(__WaveHeader*)mymalloc(SRAMIN,sizeof(__WaveHeader));//����__WaveHeader�ֽڵ��ڴ����� 
	pname=mymalloc(SRAMIN,30);						//����30���ֽ��ڴ�,����"0:RECORDER/REC00001.wav" 
	if(!i2srecbuf1||!i2srecbuf2||!f_rec||!wavhead||!pname)rval=1; 	
	if(rval==0)		
	{
		recoder_enter_rec_mode();	//����¼��ģʽ,��ʱ��������������ͷ�ɼ�������Ƶ   
		pname[0]=0;					//pnameû���κ��ļ��� 
 	   	while(rval==0)
		{
			key=key_scan(0);
			switch(key)
			{		
				case KEY2_PRES:	//STOP&SAVE
					if(rec_sta&0X80)//��¼��
					{
						rec_sta=0;	//�ر�¼��
						wavhead->riff.ChunkSize=wavsize+36;		//�����ļ��Ĵ�С-8;
				   		wavhead->data.ChunkSize=wavsize;		//���ݴ�С
						f_lseek(f_rec,0);						//ƫ�Ƶ��ļ�ͷ.
				  		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
						f_close(f_rec);
						wavsize=0;
					}
					rec_sta=0;
					recsec=0;
				 	LED1(1);	 						//�ر�DS1
					lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* �����ʾ,���֮ǰ��ʾ��¼���ļ��� */
					break;	 
				case KEY0_PRES:	//REC/PAUSE
					if(rec_sta&0X01)//ԭ������ͣ,����¼��
					{
						rec_sta&=0XFE;//ȡ����ͣ
					}else if(rec_sta&0X80)//�Ѿ���¼����,��ͣ
					{
						rec_sta|=0X01;	//��ͣ
					}else				//��û��ʼ¼�� 
					{
						recsec=0;	 
						recoder_new_pathname(pname);			//�õ��µ�����
						 text_show_string(30, 190, lcddev.width, 16, "¼��:", 16, 0, RED);
                        text_show_string(30 + 40, 190, lcddev.width, 16, (char *)pname + 11, 16, 0, RED); /* ��ʾ��ǰ¼���ļ����� */
				 		recoder_wav_init(wavhead);				//��ʼ��wav����	
	 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE); 
						if(res)			//�ļ�����ʧ��
						{
							rec_sta=0;	//�����ļ�ʧ��,����¼��
							rval=0XFE;	//��ʾ�Ƿ����SD��
						}else 
						{
							res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
							recoder_msg_show(0,0);
 							rec_sta|=0X80;	//��ʼ¼��	 
						} 
 					}
					if(rec_sta&0X01)LED1(0);//��ʾ¼����ͣ
					else LED1(1);
					break;  
				case WKUP_PRES:	//�������һ��¼��
					if(rec_sta!=0X80)//û����¼��
					{   	 		 				  
						if(pname[0])//�����������������,��pname��Ϊ��
						{				 
							 text_show_string(30, 190, lcddev.width, 16, "����:", 16, 0, RED);
                            text_show_string(30 + 40, 190, lcddev.width, 16, (char*)pname + 11, 16, 0, RED); /* ��ʾ�����ŵ��ļ����� */
                            lcd_fill(30, 210, lcddev.width-1, 230, WHITE); /* �����ʾ,���֮ǰ��ʾ��¼���ļ��� */
							recoder_enter_play_mode();	//���벥��ģʽ
							//wav_play_song("0:/MUSIC/��-������.wav");
							audio_play_song(pname);		//����pname
							lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* �����ʾ,���֮ǰ��ʾ��¼���ļ��� */
							recoder_enter_rec_mode();	//���½���¼��ģʽ 
						}
					}
					break;
			}
            delay_ms(5);
			timecnt++;
			if((timecnt%20)==0)LED0_TOGGLE();//DS0��˸  
 			if(recsec!=(wavsize/wavhead->fmt.ByteRate))	//¼��ʱ����ʾ
			{	   
				LED0_TOGGLE();
				recsec=wavsize/wavhead->fmt.ByteRate;	//¼��ʱ��
				recoder_msg_show(recsec,wavhead->fmt.SampleRate*wavhead->fmt.NumOfChannels*wavhead->fmt.BitsPerSample);//��ʾ����
			}
		}		 
	}    
	myfree(SRAMIN,i2srecbuf1);	//�ͷ��ڴ�
	myfree(SRAMIN,i2srecbuf2);	//�ͷ��ڴ�  
	myfree(SRAMIN,f_rec);		//�ͷ��ڴ�
	myfree(SRAMIN,wavhead);		//�ͷ��ڴ�  
	myfree(SRAMIN,pname);		//�ͷ��ڴ�  
}
