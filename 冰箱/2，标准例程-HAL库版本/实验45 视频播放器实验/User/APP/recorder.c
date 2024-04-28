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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//录音机 应用代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	
uint8_t *i2srecbuf1;
uint8_t *i2srecbuf2;

FIL *f_rec = 0;         /* 录音文件 */
uint32_t wavsize;       /* wav数据大小(字节数,不包括文件头!!) */

uint8_t rec_sta = 0;    /**
                         * 录音状态
                         * [7]:0,没有开启录音;1,已经开启录音;
                         * [6:1]:保留
                         * [0]:0,正在录音;1,暂停录音;
                         */
    
/**
 * @brief       录音 I2S_DMA接收中断服务函数
 *   @note      在中断里面写入数据
 * @param       无
 * @retval      无
 */
 
void rec_i2s_dma_rx_callback(void)
{
    uint16_t bw;
    uint8_t res;

    if (rec_sta == 0X80)  /* 录音模式 */
    {
        if (DMA1_Stream3->CR & (1 << 19))
        {
            res = f_write(f_rec, i2srecbuf1, I2S_RX_DMA_BUF_SIZE, (UINT *)&bw); /* 写入文件 */

            if (res)
            {
                printf("write error:%d\r\n", res);
            }
        }
        else
        {
            res = f_write(f_rec, i2srecbuf2, I2S_RX_DMA_BUF_SIZE, (UINT *)&bw); /* 写入文件 */

            if (res)
            {
                printf("write error:%d\r\n", res);
            }
        }

        wavsize += I2S_RX_DMA_BUF_SIZE;
    }
}

const uint16_t i2splaybuf[2] = {0X0000, 0X0000}; /* 2个16位数据,用于录音时I2S Master发送.循环发送0. */



void recoder_enter_rec_mode(void)
{
    DMA1_Stream4->CR&=~(1<<4);	//关闭传输完成中断(这里不用中断送数据) 
	ES8388_ADDA_Cfg(1,0);		//开启ADC
	ES8388_Input_Cfg(0);		//开启输入通道(通道1,MIC所在通道) 
	ES8388_MIC_Gain(8);			//MIC增益设置为最大 
	ES8388_ALC_Ctrl(3,4,4);		//开启立体声ALC控制,以提高录音音量
	ES8388_SPKvol_Set(0);		//关闭喇叭.
	ES8388_I2S_Cfg(0,3);		//飞利浦标准,16位数据长度

	I2S2_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_16B);	//飞利浦标准,主机发送,时钟低电平有效,16位帧长度 	
	I2S2_SampleRate_Set(REC_SAMPLERATE);	//设置采样率 
 	I2S2_TX_DMA_Init((uint8_t*)&i2splaybuf[0],(uint8_t*)&i2splaybuf[1],1); 		//配置TX DMA 
	DMA1_Stream4->CR&=~(1<<4);	//关闭传输完成中断(这里不用中断送数据) 
	I2S2ext_RX_DMA_Init(i2srecbuf1,i2srecbuf2,I2S_RX_DMA_BUF_SIZE/2); 	//配置RX DMA
  	i2s_rx_callback=rec_i2s_dma_rx_callback;//回调函数指wav_i2s_dma_callback
 	I2S_Play_Start();	//开始I2S数据发送(主机)
	I2S_Rec_Start(); 	//开始I2S数据接收(从机)
	recoder_remindmsg_show(0);
}  
//进入PCM 放音模式 		  
void recoder_enter_play_mode(void)
{
	ES8388_ADDA_Cfg(0,1);		//开启DAC 
 	ES8388_Output_Cfg(1,1);		//开启通道1和2的输出   
	ES8388_SPKvol_Set(20);		//喇叭音量设置
	I2S_Play_Stop();			//停止时钟发送
	I2S_Rec_Stop(); 			//停止录音
	recoder_remindmsg_show(1);
}
//初始化WAV头.
void recoder_wav_init(__WaveHeader* wavhead) //初始化WAV头			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//还未确定,最后需要计算
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//大小为16个字节
	wavhead->fmt.AudioFormat=0X01; 		//0X01,表示PCM;0X01,表示IMA ADPCM
 	wavhead->fmt.NumOfChannels=2;		//双声道
 	wavhead->fmt.SampleRate=REC_SAMPLERATE;//设置采样速率
 	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*4;//字节速率=采样率*通道数*(ADC位数/8)
 	wavhead->fmt.BlockAlign=4;			//块大小=通道数*(ADC位数/8)
 	wavhead->fmt.BitsPerSample=16;		//16位PCM
   	wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//数据大小,还需要计算  
} 						    
//显示录音时间和码率
//tsec:秒钟数.
void recoder_msg_show(uint32_t tsec,uint32_t kbps)
{   
    printf("tsec : %d   kbps : %d  \r\n",tsec, kbps);
    lcd_show_string(30, 210, 200, 16, 16, "TIME:", RED);    /* 显示录音时间 */
    lcd_show_num(30 + 40, 210, tsec / 60, 2, 16, RED);      /* 分钟 */
    lcd_show_char(30 + 56, 210, ':', 16, 0, RED);
    lcd_show_num(30 + 64, 210, tsec % 60, 2, 16, RED);      /* 秒钟 */

    lcd_show_string(140, 210, 200, 16, 16, "KPBS:", RED);   /* 显示码率 */
    lcd_show_num(140 + 40, 210, kbps / 1000, 4, 16, RED);   /* 码率显示 */
}  	
//提示信息
//mode:0,录音模式;1,放音模式
void recoder_remindmsg_show(uint8_t mode)
{
    lcd_fill(30, 120, lcddev.width, 180, WHITE);    /* 清除原来的显示 */

    if (mode == 0)  /* 录音模式 */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:REC/PAUSE", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "KEY2:STOP&SAVE", BLUE);
        lcd_show_string(30, 160, 200, 16, 16, "WK_UP:PLAY", BLUE);
    }
    else            /* 放音模式 */
    {
        lcd_show_string(30, 120, 200, 16, 16, "KEY0:STOP Play", BLUE);
        lcd_show_string(30, 140, 200, 16, 16, "WK_UP:PLAY/PAUSE", BLUE);
    }
}
//通过时间获取文件名
//仅限在SD卡保存,不支持FLASH DISK保存
//组合成:形如"0:RECORDER/REC20120321210633.wav"的文件名
void recoder_new_pathname(uint8_t *pname)
{	 
	uint8_t res;					 
	uint16_t index=0;
    FIL *ftemp;
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* 开辟FIL字节的内存区域 */
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECORDER/REC%05d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
		if(res==FR_NO_FILE)break;		//该文件名不存在=正是我们需要的.
		index++;
	}
} 
//WAV录音 
void wav_recorder(void)
{
	uint8_t res;
	uint8_t key;
	uint8_t rval=0;
    uint32_t bw;
	__WaveHeader *wavhead=0; 
 	DIR recdir;	 					//目录  
 	uint8_t *pname=0;
	uint8_t timecnt=0;					//计时器   
	uint32_t recsec=0;					//录音时间 
	while(f_opendir(&recdir,"0:/RECORDER"))//打开录音文件夹
 	{	 
        lcd_show_string(30, 230, 240, 16, 16, "RECORDER文件夹错误!", RED);
        delay_ms(200);
        lcd_fill(30, 230, 240, 246, WHITE);     /* 清除显示 */
        delay_ms(200);
        f_mkdir("0:/RECORDER");                 /* 创建该目录 */ 
	}   
    i2srecbuf1=mymalloc(SRAMIN,I2S_RX_DMA_BUF_SIZE);//I2S录音内存1申请
	i2srecbuf2=mymalloc(SRAMIN,I2S_RX_DMA_BUF_SIZE);//I2S录音内存2申请  
  	f_rec=(FIL *)mymalloc(SRAMIN,sizeof(FIL));		//开辟FIL字节的内存区域  
 	wavhead=(__WaveHeader*)mymalloc(SRAMIN,sizeof(__WaveHeader));//开辟__WaveHeader字节的内存区域 
	pname=mymalloc(SRAMIN,30);						//申请30个字节内存,类似"0:RECORDER/REC00001.wav" 
	if(!i2srecbuf1||!i2srecbuf2||!f_rec||!wavhead||!pname)rval=1; 	
	if(rval==0)		
	{
		recoder_enter_rec_mode();	//进入录音模式,此时耳机可以听到咪头采集到的音频   
		pname[0]=0;					//pname没有任何文件名 
 	   	while(rval==0)
		{
			key=key_scan(0);
			switch(key)
			{		
				case KEY2_PRES:	//STOP&SAVE
					if(rec_sta&0X80)//有录音
					{
						rec_sta=0;	//关闭录音
						wavhead->riff.ChunkSize=wavsize+36;		//整个文件的大小-8;
				   		wavhead->data.ChunkSize=wavsize;		//数据大小
						f_lseek(f_rec,0);						//偏移到文件头.
				  		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
						f_close(f_rec);
						wavsize=0;
					}
					rec_sta=0;
					recsec=0;
				 	LED1(1);	 						//关闭DS1
					lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* 清除显示,清除之前显示的录音文件名 */
					break;	 
				case KEY0_PRES:	//REC/PAUSE
					if(rec_sta&0X01)//原来是暂停,继续录音
					{
						rec_sta&=0XFE;//取消暂停
					}else if(rec_sta&0X80)//已经在录音了,暂停
					{
						rec_sta|=0X01;	//暂停
					}else				//还没开始录音 
					{
						recsec=0;	 
						recoder_new_pathname(pname);			//得到新的名字
						 text_show_string(30, 190, lcddev.width, 16, "录制:", 16, 0, RED);
                        text_show_string(30 + 40, 190, lcddev.width, 16, (char *)pname + 11, 16, 0, RED); /* 显示当前录音文件名字 */
				 		recoder_wav_init(wavhead);				//初始化wav数据	
	 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE); 
						if(res)			//文件创建失败
						{
							rec_sta=0;	//创建文件失败,不能录音
							rval=0XFE;	//提示是否存在SD卡
						}else 
						{
							res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
							recoder_msg_show(0,0);
 							rec_sta|=0X80;	//开始录音	 
						} 
 					}
					if(rec_sta&0X01)LED1(0);//提示录音暂停
					else LED1(1);
					break;  
				case WKUP_PRES:	//播放最近一段录音
					if(rec_sta!=0X80)//没有在录音
					{   	 		 				  
						if(pname[0])//如果触摸按键被按下,且pname不为空
						{				 
							 text_show_string(30, 190, lcddev.width, 16, "播放:", 16, 0, RED);
                            text_show_string(30 + 40, 190, lcddev.width, 16, (char*)pname + 11, 16, 0, RED); /* 显示当播放的文件名字 */
                            lcd_fill(30, 210, lcddev.width-1, 230, WHITE); /* 清除显示,清除之前显示的录音文件名 */
							recoder_enter_play_mode();	//进入播放模式
							//wav_play_song("0:/MUSIC/大海-张雨生.wav");
							audio_play_song(pname);		//播放pname
							lcd_fill(30, 190, lcddev.width, lcddev.height, WHITE); /* 清除显示,清除之前显示的录音文件名 */
							recoder_enter_rec_mode();	//重新进入录音模式 
						}
					}
					break;
			}
            delay_ms(5);
			timecnt++;
			if((timecnt%20)==0)LED0_TOGGLE();//DS0闪烁  
 			if(recsec!=(wavsize/wavhead->fmt.ByteRate))	//录音时间显示
			{	   
				LED0_TOGGLE();
				recsec=wavsize/wavhead->fmt.ByteRate;	//录音时间
				recoder_msg_show(recsec,wavhead->fmt.SampleRate*wavhead->fmt.NumOfChannels*wavhead->fmt.BitsPerSample);//显示码率
			}
		}		 
	}    
	myfree(SRAMIN,i2srecbuf1);	//释放内存
	myfree(SRAMIN,i2srecbuf2);	//释放内存  
	myfree(SRAMIN,f_rec);		//释放内存
	myfree(SRAMIN,wavhead);		//释放内存  
	myfree(SRAMIN,pname);		//释放内存  
}
