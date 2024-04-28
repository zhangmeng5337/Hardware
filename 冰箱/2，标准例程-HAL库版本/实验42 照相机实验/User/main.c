/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-10
 * @brief       照相机 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./MALLOC/malloc.h"
#include "./USMART/usmart.h"
#include "./TEXT/text.h"
#include "./PICTURE/piclib.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/sccb.h"
#include "./BSP/DCMI/dcmi.h"
#include "./BSP/BEEP/beep.h"
#include "string.h"
#include "math.h"


uint8_t g_ov_mode = 0;                                  /* bit0: 0, RGB565模式;  1, JPEG模式 */

#define jpeg_buf_size       300 * 1024                  /* 定义JPEG数据缓存jpeg_buf的大小(字节) */
#define jpeg_line_size      1 * 1024                    /* 定义DMA接收数据时,一行数据的最大值 */

uint32_t *p_dcmi_line_buf[2];                           /* JPEG数据 DMA双缓存buf指针 */
uint32_t *p_jpeg_data_buf;                              /* JPEG数据缓存buf指针 */

volatile uint32_t g_jpeg_data_len = 0;                  /* buf中的JPEG有效数据长度 */


/**
 * 0,数据没有采集完;
 * 1,数据采集完了,但是还没处理;
 * 2,数据已经处理完成了,可以开始下一帧接收
 */
volatile uint8_t g_jpeg_data_ok = 0;                    /* JPEG数据采集完成标志 */

/**
 * @brief       处理JPEG数据
 *   @ntoe      在DCMI_IRQHandler中断服务函数里面被调用
 *              当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
 *
 * @param       无
 * @retval      无
 */
void jpeg_data_process(void)
{
    uint16_t i, rlen;
    uint32_t *pbuf;

    if (g_ov_mode)   /* 只有在JPEG格式下,才需要做处理. */
    {
        if (g_jpeg_data_ok == 0)    /* jpeg数据还未采集完? */
        {
            __HAL_DMA_DISABLE(&g_dma_dcmi_handle);      /* 关闭DMA */
            while(DMA2_Stream1->CR & 0x01);             /* 等待DMA2_Stream1可配置 */
            rlen = jpeg_line_size - __HAL_DMA_GET_COUNTER(&g_dma_dcmi_handle);    /* 得到剩余长度 */
            pbuf = p_jpeg_data_buf + g_jpeg_data_len;   /* 偏移到有效数据末尾，继续添加 */
            
            if (DMA2_Stream1->CR & (1 << 19))
            {
                for(i = 0; i < rlen; i++)
                {
                    pbuf[i] = p_dcmi_line_buf[1][i];    /* 读取buf1里面的剩余数据 */
                }
            }
            else
            {
                for(i = 0; i < rlen; i++)
                {
                    pbuf[i] = p_dcmi_line_buf[0][i];    /* 读取buf0里面的剩余数据 */
                }
            }
            g_jpeg_data_len += rlen;    /* 加上剩余长度 */
            g_jpeg_data_ok = 1;         /* 标记JPEG数据采集完成,等待其他函数处理 */
        }
        
        if (g_jpeg_data_ok == 2)        /* 上一次的jpeg数据已经被处理了 */
        {
            __HAL_DMA_SET_COUNTER(&g_dma_dcmi_handle, jpeg_line_size);    /* 传输长度为jpeg_buf_size*4字节 */
            __HAL_DMA_ENABLE(&g_dma_dcmi_handle);                         /* 重新传输 */
            g_jpeg_data_ok = 0;         /* 标记数据未采集 */
            g_jpeg_data_len = 0;        /* 数据重新开始 */
        }
    }
    else
    {
        lcd_set_cursor(0, 0);
        lcd_write_ram_prepare();        /* 开始写入GRAM */
    }
}

/**
 * @brief       JPEG数据接收回调函数
 * @param       无
 * @retval      无
 */
void jpeg_dcmi_rx_callback(void)
{
    uint16_t i;
    uint32_t *pbuf;

    pbuf = p_jpeg_data_buf + g_jpeg_data_len;   /* 偏移到有效数据末尾 */

    if (DMA2_Stream1->CR & (1 << 19))           /* buf0已满,正常处理buf1 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = p_dcmi_line_buf[0][i];    /* 读取buf0里面的数据 */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* 偏移 */
    }
    else    /* buf1已满,正常处理buf0 */
    {
        for (i = 0; i < jpeg_line_size; i++)
        {
            pbuf[i] = p_dcmi_line_buf[1][i];    /* 读取buf1里面的数据 */
        }
        
        g_jpeg_data_len += jpeg_line_size;      /* 偏移 */
    }
}

/**
 * @brief       切换为OV2640模式
 *   @note      切换PC8/PC9/PC11为DCMI复用功能(AF13)
 * @param       无
 * @retval      无
 */
void sw_ov2640_mode(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    OV2640_PWDN(0);    /* OV2640 power up */
    
    /* GPIOC8/9/11切换为 DCMI接口 */
    gpio_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF13_DCMI;        /* 复用为DCMI */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);            /* 初始化PC8，9, 11引脚 */
}

/**
 * @brief       切换为SD卡模式
 *   @note      切换PC8/PC9/PC11为SDMMC复用功能(AF12)
 * @param       无
 * @retval      无
 */
void sw_sdcard_mode(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    OV2640_PWDN(1);     /* OV2640 power down */
    
    /* GPIOC8/9/11切换为 SDIO接口 */
    gpio_init_struct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_SDIO;        /* 复用为SDIO */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);            /* 初始化PC8，9, 11引脚 */
}

/**
 * @brief       文件名自增（避免覆盖）
 *   @note      bmp组合成: 形如 "0:PHOTO/PIC13141.bmp" 的文件名
 *              jpg组合成: 形如 "0:PHOTO/PIC13141.jpg" 的文件名
 * @param       pname : 有效的文件名
 * @param       mode  : 0, 创建.bmp文件;  1, 创建.jpg文件;
 * @retval      无
 */
void camera_new_pathname(uint8_t *pname, uint8_t mode)
{
    uint8_t res;
    uint16_t index = 0;
    FIL *ftemp;
    
    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* 开辟FIL字节的内存区域 */

    if (ftemp == NULL) return;  /* 内存申请失败 */

    while (index < 0XFFFF)
    {
        if (mode == 0)  /* 创建.bmp文件名 */
        {
            sprintf((char *)pname, "0:PHOTO/PIC%05d.bmp", index);
        }
        else  /* 创建.jpg文件名 */
        {
            sprintf((char *)pname, "0:PHOTO/PIC%05d.jpg", index);
        }
        
        res = f_open(ftemp, (const TCHAR *)pname, FA_READ); /* 尝试打开这个文件 */

        if (res == FR_NO_FILE) break;   /* 该文件名不存在, 正是我们需要的 */

        index++;
    }
    myfree(SRAMIN, ftemp);
}
/**
 * @brief       OV2640拍照jpg图片
 * @param       pname : 要创建的jpg文件名(含路径)
 * @retval      0, 成功; 其他,错误代码;
 */
uint8_t ov2640_jpg_photo(uint8_t *pname)
{
    FIL *f_jpg;
    uint8_t res = 0, headok = 0;
    uint32_t bwr;
    uint32_t i, jpgstart, jpglen;
    uint8_t *pbuf;
    
    uint16_t datasize = 0;          /* 单次写入数据量 */
    uint32_t datalen = 0;           /* 总写入数据量 */
    uint8_t  *databuf;              /* 数据缓存，避免直接写外部SRAM数据到SD卡，导致写入下溢错误 */
    
    f_jpg = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* 开辟FIL字节的内存区域 */
    databuf = mymalloc(SRAMIN, 4096);   /* 申请4K内存 */
    if (databuf == NULL) return 0XFF;   /* 内存申请失败 */

    g_ov_mode = 1;
    g_jpeg_data_ok = 0;
    
    sw_ov2640_mode();               /* 切换为OV2640模式 */
    ov2640_jpeg_mode();             /* JPEG模式 */
    
    dcmi_rx_callback = jpeg_dcmi_rx_callback;   /* JPEG接收数据回调函数 */
    dcmi_dma_init((uint32_t)p_dcmi_line_buf[0], (uint32_t)p_dcmi_line_buf[1], jpeg_line_size, DMA_MDATAALIGN_WORD, DMA_MINC_ENABLE); /* DCMI DMA配置 */
    
    ov2640_image_win_set(0, 0, 1600, 1200);
    ov2640_outsize_set(1600, 1200); /* 设置输出尺寸(1600 * 1200) */
    
    dcmi_start();                   /* 启动传输 */
    while (g_jpeg_data_ok != 1);    /* 等待第一帧图片采集完 */
    g_jpeg_data_ok = 2;             /* 忽略本帧图片,启动下一帧采集 */
    while (g_jpeg_data_ok != 1);    /* 等待第二帧图片采集完  */
    g_jpeg_data_ok = 2;             /* 忽略本帧图片,启动下一帧采集 */
    while (g_jpeg_data_ok != 1);    /* 等待第三帧图片采集完,第三帧,才保存到SD卡去 */

    dcmi_stop();                    /* 停止DMA搬运 */
    g_ov_mode = 0;
    sw_sdcard_mode();               /* 切换为SD卡模式 */
    
    printf("jpeg data size:%d\r\n", g_jpeg_data_len * 4);   /* 串口打印JPEG文件大小 */
    pbuf = (uint8_t *)p_jpeg_data_buf;
    jpglen = 0;                     /* 设置jpg文件大小为0 */
    headok = 0;                     /* 清除jpg头标记 */

    for (i = 0; i < g_jpeg_data_len * 4; i++)   /* 查找0XFF,0XD8和0XFF,0XD9,获取jpg文件大小 */
    {
        if ((pbuf[i] == 0XFF) && (pbuf[i + 1] == 0XD8))     /* 找到FF D8 */
        {
            jpgstart = i;
            headok = 1;             /* 标记找到jpg头(FF D8) */
        }

        if ((pbuf[i] == 0XFF) && (pbuf[i + 1] == 0XD9) && headok)   /* 找到头以后,再找FF D9 */
        {
            jpglen = i - jpgstart + 2;
            break;
        }
    }

    if (jpglen)                     /* 正常的jpeg数据 */
    {
        res = f_open(f_jpg, (const TCHAR *)pname, FA_WRITE | FA_CREATE_NEW);    /* 模式0,或者尝试打开失败,则创建新文件 */

        if (res == 0)
        {
            pbuf += jpgstart;       /* 偏移到0XFF,0XD8处 */
            
            while(datalen < jpglen) /* 循环写入！不能直接写外部SRAM数据到SDIO，否则可能引起FIFO下溢错误 */
            {
                if((jpglen - datalen) > 4096)
                {
                    datasize = 4096;
                }else
                {
                    datasize = jpglen - datalen;    /* 最后的数据 */
                }

                my_mem_copy(databuf, pbuf, datasize);
                res = f_write(f_jpg, databuf, datasize, (UINT *)&bwr); /* 写入内容 */
                pbuf += datasize;
                jpglen -= datasize;

                if (res) break;
            }
        }

        f_close(f_jpg);
    }
    else
    {
        res = 0XFD;
    }
    
    g_jpeg_data_len = 0;
    sw_ov2640_mode();       /* 切换为OV2640模式 */
    ov2640_rgb565_mode();   /* RGB565模式 */

    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE); /* DCMI DMA配置,MCU屏,竖屏 */
    myfree(SRAMIN, f_jpg);
    myfree(SRAMIN, databuf);
    
    return res;
}

int main(void)
{
    uint8_t res;
    float fac;
    uint8_t *pname;         /* 带路径的文件名 */
    uint8_t key;            /* 键值 */
    uint8_t i;
    uint8_t sd_ok = 1;      /* 0,sd卡不正常;1,SD卡正常 */
    uint8_t scale = 1;      /* 默认是全尺寸缩放 */
    uint8_t msgbuf[15];     /* 消息缓存区 */

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口1初始化为115200 */
    usmart_dev.init(84);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    beep_init();                            /* 初始化蜂鸣器 */
    lcd_init();                             /* 初始化LCD */
    key_init();                             /* 初始化按键 */
    sram_init();                            /* 初始化SRAM */
    btim_timx_int_init(10000 - 1, 8400 - 1);/* 10KHz计数频率，1秒中断一次 */
    piclib_init();                          /* 初始化画图 */

    my_mem_init(SRAMIN);                    /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);                    /* 初始化外部SRAM内存池 */
    my_mem_init(SRAMCCM);                   /* 初始化CCM内存池 */
    
    exfuns_init();                          /* 为fatfs相关变量申请内存 */
    f_mount(fs[0], "0:", 1);                /* 挂载SD卡 */
    f_mount(fs[1], "1:", 1);                /* 挂载FLASH */

    while (fonts_init())                    /* 检查字库 */
    {
        lcd_show_string(30, 90, 200, 16, 16, "Font error!", RED);
        delay_ms(200);
        lcd_fill(20, 90, 200 + 20, 90 + 16, WHITE);
        delay_ms(200);
    }

    text_show_string(30, 50, 200, 16, "正点原子STM32开发板", 16, 0, RED);
    text_show_string(30, 70, 200, 16, "照相机实验", 16, 0, RED);
    text_show_string(30, 90, 200, 16, "KEY0:拍照(bmp格式)", 16, 0, RED);
    text_show_string(30, 110, 200, 16, "KEY1:拍照(jpg格式)", 16, 0, RED);
    text_show_string(30, 130, 200, 16, "WK_UP:FullSize/Scale", 16, 0, RED);
    
    res = f_mkdir("0:/PHOTO");              /* 创建PHOTO文件夹 */
    
    if (res != FR_EXIST && res != FR_OK)    /* 发生了错误 */
    {
        res = f_mkdir("0:/PHOTO");          /* 创建PHOTO文件夹 */
        text_show_string(30, 150, 240, 16, "SD卡错误!", 16, 0, RED);
        delay_ms(200);
        text_show_string(30, 150, 240, 16, "拍照功能将不可用!", 16, 0, RED);
        delay_ms(200);
        sd_ok = 0;
    }

    p_dcmi_line_buf[0] = mymalloc(SRAMIN, jpeg_line_size * 4);  /* 为jpeg dma接收申请内存 */
    p_dcmi_line_buf[1] = mymalloc(SRAMIN, jpeg_line_size * 4);  /* 为jpeg dma接收申请内存 */
    p_jpeg_data_buf = mymalloc(SRAMEX, jpeg_buf_size);          /* 为jpeg文件申请内存 */
    pname = mymalloc(SRAMIN, 30);                               /* 为带路径的文件名分配30个字节的内存 */

    while (pname == NULL || !p_dcmi_line_buf[0] || !p_dcmi_line_buf[1] || !p_jpeg_data_buf) /* 内存分配出错 */
    {
        text_show_string(30, 150, 240, 16, "内存分配失败!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(30, 150, 240, 146, WHITE); /* 清除显示 */
        delay_ms(200);
    }

    while (ov2640_init())   /* 初始化OV2640 */
    {
        text_show_string(30, 170, 240, 16, "OV2640 错误!", 16, 0, RED);
        delay_ms(200);
        lcd_fill(30, 150, 239, 206, WHITE);
        delay_ms(200);
    }

    delay_ms(100);
    text_show_string(30, 170, 230, 16, "OV2640 正常", 16, 0, RED);
    
    ov2640_rgb565_mode();       /* RGB565模式 */
    dcmi_init();                /* DCMI配置 */
    dcmi_dma_init((uint32_t)&LCD->LCD_RAM, 0, 1, DMA_MDATAALIGN_HALFWORD, DMA_MINC_DISABLE);    /* DCMI DMA配置,MCU屏,竖屏 */
    
    ov2640_outsize_set(lcddev.width, lcddev.height);    /* 满屏缩放显示 */
    dcmi_start();               /* 启动传输 */
    ov2640_flash_intctrl();     /* 闪光灯控制 */
    lcd_clear(BLACK);
    
    while (1)
    {
        key = key_scan(0);
        
         if (key && (key != KEY2_PRES))
        {
            dcmi_stop();                /* 停止显示 */
            
            if (key == WKUP_PRES)       /* 缩放处理 */
            {
                scale = !scale;

                if (scale == 0)
                {
                    ov2640_image_win_set((1600 - lcddev.width) / 2, (1200 - lcddev.height) / 2, lcddev.width, lcddev.height);
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                    sprintf((char *)msgbuf, "Full Size 1:1");
                }
                else
                {
                    ov2640_image_win_set(0,0,1600,1200);    /* 全尺寸缩放 */
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                    sprintf((char *)msgbuf, "Scale");
                }
                
                lcd_show_string(30, 50, 200, 16, 16, (char *)msgbuf, RED);
                delay_ms(800);
            }
            else if (sd_ok)             /* SD卡正常才可以拍照 */
            {
                sw_sdcard_mode();       /* 切换为SD卡模式 */

                if (key == KEY0_PRES)   /* BMP拍照 */
                {
                    camera_new_pathname(pname, 0);  /* 得到文件名 */
                    res = bmp_encode(pname, 0, 0, lcddev.width, lcddev.height, 0);
                    sw_ov2640_mode();               /* 切换为OV2640模式 */
                }
                else if (key == KEY1_PRES)          /* JPG拍照 */
                {
                    camera_new_pathname(pname, 1);  /* 得到文件名 */
                    res = ov2640_jpg_photo(pname);

                    if (scale == 0)
                    {
                        ov2640_image_win_set((1280 - fac * lcddev.width) / 2, (800 - lcddev.height) / 2, lcddev.width, lcddev.height);
                    }
                    else
                    {
                        ov2640_image_win_set(0, 0, 1600, 1200);     /* 全尺寸缩放 */
                    }
                    ov2640_outsize_set(lcddev.width, lcddev.height);
                }
                sw_ov2640_mode();   /* 切换为OV2640模式 */
                
                if (res)            /* 拍照有误 */
                {
                    text_show_string(30, 130, 240, 16, "写入文件错误!", 16, 0, RED);
                }
                else
                {
                    text_show_string(30, 130, 240, 16, "拍照成功!", 16, 0, RED);
                    text_show_string(30, 150, 240, 16, "保存为:", 16, 0, RED);
                    text_show_string(30 + 56, 150, 240, 16, (char*)pname, 16, 0, RED);
                    BEEP(1);        /* 蜂鸣器短叫，提示拍照完成 */
                    delay_ms(100);
                    BEEP(0);        /* 关闭蜂鸣器 */
                }

                delay_ms(1000);     /* 等待1秒钟 */
                dcmi_start();       /* 这里先使能dcmi,然后立即关闭DCMI,后面再开启DCMI,可以防止RGB屏的侧移问题 */
                dcmi_stop();
            }
            else    /* 提示SD卡错误 */
            {
                text_show_string(30, 130, 240, 16, "SD卡错误!", 16, 0, RED);
                text_show_string(30, 150, 240, 16, "拍照功能不可用!", 16, 0, RED);
            }
            
            BEEP(0);
            
            if (key != WKUP_PRES) delay_ms(1800);
            
            dcmi_start();   /* 开始显示 */
        }

        delay_ms(10);
        i++;

        if (i == 20)
        {
            i = 0;
            LED0_TOGGLE();
        }
    }
}

