/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       磁力计 实验
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
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/ST480MC/st480mc.h"
#include <math.h>


/* 磁力计平面校准参数 
 * 校准原理请参考(方法2):http://blog.sina.com.cn/s/blog_402c071e0102v8ie.html
 */
int16_t g_magx_offset = 0;      /* x轴补偿值 */
int16_t g_magy_offset = 0;      /* y轴补偿值 */

/**
 * @brief       显示磁力计原始数据
 * @param       x, y : 坐标
 * @param       title: 标题
 * @param       val  : 值
 * @retval      无
 */
void user_show_mag(uint16_t x, uint16_t y, char * title, int16_t val)
{
    char buf[20];

    sprintf(buf,"%s%d", title, val);                /* 格式化输出 */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* 清除上次数据(最多显示20个字符,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* 显示字符串 */
}

/**
 * @brief       显示方位角
 * @param       x, y : 坐标
 * @param       angle: 角度
 * @retval      无
 */
void user_show_angle(uint16_t x, uint16_t y, float angle)
{
    char buf[20];

    sprintf(buf,"Angle:%3.1fC", angle);             /* 格式化输出 */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* 清除上次数据(最多显示20个字符,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* 显示字符串 */
}

/**
 * @brief       显示温度
 * @param       x, y : 坐标
 * @param       temp : 温度
 * @retval      无
 */
void user_show_temprate(uint16_t x, uint16_t y, float temp)
{
    char buf[20];

    sprintf(buf,"Temp:%2.1fC", temp);               /* 格式化输出 */
    lcd_fill(x, y, x + 160, y + 16, WHITE);         /* 清除上次数据(最多显示20个字符,20*8=160) */
    lcd_show_string(x, y, 160, 16, 16, buf, BLUE);  /* 显示字符串 */
}

/**
 * @brief       罗盘(磁力计)校准函数
 *   @note      这里我们使用最简单的平面校准方法.
 *              进入此函数后,请水平转动开发板至少一周(360°),转动完成后, 按WKUP键退出!
 * @param       无
 * @retval      无
 */
void compass_calibration(void)
{
    int16_t x_min = 0;
    int16_t x_max = 0;
    int16_t y_min = 0;
    int16_t y_max = 0;

    int16_t magx, magy, magz;
    uint8_t res;
    uint8_t key = 0;
    
    lcd_clear(WHITE);
    lcd_show_string(10, 90, 240, 16, 16, "Compass Calibration", RED);
    lcd_show_string(10, 110, 240, 16, 16, "Pls rotate horiz one cycle!", RED);
    lcd_show_string(10, 130, 240, 16, 16, "If done, press WKUP key!", RED);
    
    while (1)
    {
        key = key_scan(0);
        
        if (key == WKUP_PRES)   /* 结束校准 */
        {
            break;
        }
        
        res = st480mc_read_magdata(&magx, &magy, &magz);    /* 读取数据 */
        
        if (res == 0)
        {
            x_max = x_max < magx ? magx : x_max;            /* 记录x最大值 */
            x_min = x_min > magx ? magx : x_min;            /* 记录x最小值 */
            y_max = y_max < magy ? magy : y_max;            /* 记录y最大值 */
            y_min = y_min > magy ? magy : y_min;            /* 记录y最小值 */
        }
        
        LED0_TOGGLE();          /* LED0闪烁,提示程序运行 */
    }
    
    g_magx_offset = (x_max + x_min) / 2;    /* X轴偏移量 */
    g_magy_offset = (y_max + y_min) / 2;    /* Y轴偏移量 */
   
    /* 串口打印水平校准相关参数 */
    printf("x_min:%d\r\n", x_min);
    printf("x_max:%d\r\n", x_max);
    printf("y_min:%d\r\n", y_min);
    printf("y_max:%d\r\n", y_max);
    
    printf("g_magx_offset:%d\r\n", g_magx_offset);
    printf("g_magy_offset:%d\r\n", g_magy_offset);
}

/**
 * @brief       罗盘获取角度
 *   @note      获取当前罗盘的角度(地磁角度)
 * @param       无
 * @retval      角度
 */
float compass_get_angle(void)
{
    float angle;
    int16_t magx, magy, magz;
    
    st480mc_read_magdata_average(&magx, &magy, &magz, 10);  /* 读取原始数据, 10次取平均 */
    
    magx = (magx - g_magx_offset) ;     /* 根据校准参数, 计算新的输出 */
    magy = (magy - g_magy_offset) ;     /* 根据校准参数, 计算新的输出 */

    /* 根据不同的象限情况, 进行方位角换算 */
    if ((magx > 0) && (magy > 0))
    {
        angle = (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx > 0) && (magy < 0))
    {
        angle = 360 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    else if ((magx == 0) && (magy > 0))
    {
        angle = 90;
    }
    else if ((magx == 0) && (magy < 0))
    {
        angle = 270;
    }
    else if (magx < 0)
    {
        angle = 180 + (atan((double)magy / magx) * 180) / 3.14159f;
    }
    
    if (angle > 360) angle = 360; /* 限定方位角范围 */
    if (angle < 0) angle = 0;     /* 限定方位角范围 */

    return angle;
}

int main(void)
{
    uint8_t t = 0;
    uint8_t key;
    float angle;
    float temperature;
    int16_t magx, magy, magz;

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    usmart_dev.init(84);                    /* 初始化USMART */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    lcd_init();                             /* 初始化LCD */

    while (st480mc_init())    /* ST480MC初始化 */
    {
        lcd_show_string(30, 110, 200, 16, 16, "ST480MC Error", RED);
        delay_ms(200);
        lcd_fill(30, 110, 239, 130 + 16, WHITE);
        delay_ms(200);
    }

RST:
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "ST480MC TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0 to calibration", RED);

    while (1)
    {
        key = key_scan(0);
        
        if (key == KEY0_PRES)       /* KEY0 按下 ,执行校准 */
        {
            compass_calibration();  /* 校准函数 */
            lcd_clear(WHITE);       /* 清屏 */
            goto RST;               /* 校准完后,跳到RST, 重新显示提示信息 */
        }
        
        delay_ms(10);

        t++;
        
        if (t == 20)     /* 0.2秒左右更新一次温度/磁力计原始值 */
        {        
            angle = compass_get_angle();                /* 执行一次约150ms */
            user_show_angle(30, 130, angle);            /* 显示角度 */
      
            st480mc_read_temperature(&temperature);     /* 读取温湿度值 */
            user_show_temprate(30, 150, temperature);   /* 显示温度 */
            
            st480mc_read_magdata(&magx, &magy, &magz);  /* 读取原始数据 */
            user_show_mag(30, 170, "MagX:", magx);      /* 显示magx */
            user_show_mag(30, 190, "MagY:", magy);      /* 显示magy */
            user_show_mag(30, 210, "MagZ:", magz);      /* 显示magz */
            
            t = 0;
            LED0_TOGGLE();  /* LED0闪烁 */
        }
    }
}








