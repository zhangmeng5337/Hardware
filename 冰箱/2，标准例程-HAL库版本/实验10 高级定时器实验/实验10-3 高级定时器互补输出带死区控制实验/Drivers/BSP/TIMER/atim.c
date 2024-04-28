/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.2
 * @date        2021-10-19
 * @brief       高级定时器 驱动代码
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
 * 修改说明
 * V1.0 20211019
 * 第一次发布
 * V1.1 20211019
 * 1, 新增atim_timx_comp_pwm_init函数, 实现输出比较模式PWM输出功能
 * V1.2 20211019
 * 1, 增加atim_timx_cplm_pwm_init函数
 * 2, 增加atim_timx_cplm_pwm_set函数
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_npwm_chy_handle;     /* 定时器x句柄 */

/* g_npwm_remain表示当前还剩下多少个脉冲要发送
 * 每次最多发送256个脉冲
 */
static uint32_t g_npwm_remain = 0;

/**
 * @brief       高级定时器TIMX 通道Y 输出指定个数PWM 初始化函数
 * @note
 *              高级定时器的时钟来自APB2, 而PCLK2 = 168Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 168Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 预分频系数
 * @retval      无
 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef timx_oc_npwm_chy = {0};  /* 定时器输出 */
    ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE();       /* TIMX 通道IO口时钟使能 */
    ATIM_TIMX_NPWM_CHY_CLK_ENABLE();            /* TIMX 时钟使能 */

    g_timx_npwm_chy_handle.Instance = ATIM_TIMX_NPWM;                   /* 定时器x */
    g_timx_npwm_chy_handle.Init.Prescaler = psc;                        /* 定时器分频 */
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 递增计数模式 */
    g_timx_npwm_chy_handle.Init.Period = arr;                           /* 自动重装载值 */
    g_timx_npwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* 使能TIMx_ARR进行缓冲 */
    g_timx_npwm_chy_handle.Init.RepetitionCounter = 0;                  /* 重复计数器初始值 */
    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);                          /* 初始化PWM */

    gpio_init_struct.Pin = ATIM_TIMX_NPWM_CHY_GPIO_PIN;                 /* 通道y的GPIO口 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                            /* 复用推完输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                      /* 高速 */
    gpio_init_struct.Alternate = ATIM_TIMX_NPWM_CHY_GPIO_AF;            /* 端口复用 */
    HAL_GPIO_Init(ATIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);

    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;                          /* 模式选择PWM 1 */
    timx_oc_npwm_chy.Pulse = arr / 2;                                   /* 设置比较值,此值用来确定占空比 */
                                                                        /* 这里默认设置比较值为自动重装载值的一半,即占空比为50% */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                  /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &timx_oc_npwm_chy, ATIM_TIMX_NPWM_CHY); /* 配置TIMx通道y */

    HAL_NVIC_SetPriority(ATIM_TIMX_NPWM_IRQn, 1, 3);                    /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(ATIM_TIMX_NPWM_IRQn);                            /* 开启ITMx中断 */

    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);        /* 允许更新中断 */
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, ATIM_TIMX_NPWM_CHY);     /* 开启对应PWM通道 */
}

/**
 * @brief       高级定时器TIMX NPWM设置PWM个数
 * @param       rcr: PWM的个数, 1~2^32次方个
 * @retval      无
 */
void atim_timx_npwm_chy_set(uint32_t npwm)
{
    if (npwm == 0) return;

    g_npwm_remain = npwm;                                                   /* 保存脉冲个数 */
    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* 使能定时器TIMX */
}

/**
 * @brief       高级定时器TIMX NPWM中断服务函数
 * @param       无
 * @retval      无
 */
void ATIM_TIMX_NPWM_IRQHandler(void)
{
    uint16_t npwm = 0;

    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式 */
    if (__HAL_TIM_GET_FLAG(&g_timx_npwm_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain >= 256)           /* 还有大于256个脉冲需要发送 */
        {
            g_npwm_remain = g_npwm_remain - 256;
            npwm = 256;
        }
        else if (g_npwm_remain % 256)       /* 还有位数（不到256）个脉冲要发送 */
        {
            npwm = g_npwm_remain % 256;
            g_npwm_remain = 0;              /* 没有脉冲了 */
        }

        if (npwm) /* 有脉冲要发送 */
        {
            ATIM_TIMX_NPWM->RCR = npwm - 1;                                         /* 设置重复计数寄存器值为npwm-1, 即npwm个脉冲 */
            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* 产生一次更新事件,在中断里面处理脉冲输出 */
            __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* 使能定时器TIMX */
        }
        else
        {
            ATIM_TIMX_NPWM->CR1 &= ~(1 << 0); /* 关闭定时器TIMX，使用HAL Disable会清除PWM通道信息，此处不用 */
        }

        __HAL_TIM_CLEAR_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
    }
}

/*******************************以下是高级定时器输出比较模式实验程序**************************************/

TIM_HandleTypeDef g_timx_comp_pwm_handle;       /* 定时器x句柄 */

/**
 * @brief       高级定时器TIMX 输出比较模式 初始化函数（使用输出比较模式）
 * @note
 *              配置高级定时器TIMX 4路输出比较模式PWM输出,实现50%占空比,不同相位控制
 *              注意,本例程输出比较模式,每2个计数周期才能完成一个PWM输出,因此输出频率减半
 *              另外,我们还可以开启中断在中断里面修改CCRx,从而实现不同频率/不同相位的控制
 *              但是我们不推荐这么使用,因为这可能导致非常频繁的中断,从而占用大量CPU资源
 *
 *              高级定时器的时钟来自APB2, 而PCLK2 = 168Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 168Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 预分频系数
 * @retval      无
 */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_comp_pwm = {0};

    g_timx_comp_pwm_handle.Instance = ATIM_TIMX_COMP;                   /* 定时器8 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc  ;                      /* 预分频系数 */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* 向上计数模式 */
    g_timx_comp_pwm_handle.Init.Period = arr;                           /* 自动重装载值 */
    g_timx_comp_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* 不使能影子寄存器TIMx_ARR */
    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);                           /* 输出比较模式初始化 */

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;                        /* 比较输出模式翻转功能 */
    timx_oc_comp_pwm.Pulse = 250 - 1;                                   /* 设置输出比较寄存器的值 */
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                  /* 输出比较极性为高 */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_1); /* 初始化定时器的输出比较通道1 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);                 /* 通道1 预装载使能 */

    timx_oc_comp_pwm.Pulse = 500 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_2); /* 初始化定时器的输出比较通道2 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);                 /* 通道2 预装载使能 */

    timx_oc_comp_pwm.Pulse = 750 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_3); /* 初始化定时器的输出比较通道3 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);                 /* 通道3 预装载使能 */

    timx_oc_comp_pwm.Pulse = 1000 - 1;
    timx_oc_comp_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm,  TIM_CHANNEL_4); /* 初始化定时器的输出比较通道4 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);                  /* 通道4 预装载使能 */

    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_OC_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ATIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct;

        ATIM_TIMX_COMP_CLK_ENABLE();            /* 使能定时器时钟 */

        ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE();

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH2_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH3_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH4_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH4_GPIO_PORT, &gpio_init_struct);
    }
}

/*******************************互补输出带死区控制程序**************************************/

TIM_HandleTypeDef g_timx_cplm_pwm_handle;                              /* 定时器x句柄 */
TIM_BreakDeadTimeConfigTypeDef g_sbreak_dead_time_config = {0};        /* 死区时间设置 */

/**
 * @brief       高级定时器TIMX 互补输出 初始化函数（使用PWM模式1）
 * @note
 *              配置高级定时器TIMX 互补输出, 一路OCy 一路OCyN, 并且可以设置死区时间
 *
 *              高级定时器的时钟来自APB2, 而PCLK2 = 168Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 168Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率, 单位 : Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 预分频系数
 * @retval      无
 */
void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIMX_CPLM_CLK_ENABLE();            /* TIMx 时钟使能 */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();   /* 通道X对应IO口时钟使能 */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();  /* 通道X互补通道对应IO口时钟使能 */
    ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE();  /* 通道X刹车输入对应IO口时钟使能 */
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_BKIN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = ATIM_TIMX_CPLM_CHY_GPIO_AF;                /* 端口复用 */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);


    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;                       /* 定时器x */
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;                            /* 预分频系数 */
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* 向上计数模式 */
    g_timx_cplm_pwm_handle.Init.Period = arr;                               /* 自动重装载值 */
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 42Mhz*/
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* 使能影子寄存器TIMx_ARR */
  
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle) ;

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                               /* PWM模式1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                       /* OCy 高电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCPOLARITY_HIGH;                      /* OCyN 高电平有效 */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_SET;                      /* 当MOE=0，OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_SET;                    /* 当MOE=0，OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX_CPLM_CHY);

    /* 设置死区参数，开启死区中断 */
    g_sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_DISABLE;           /* 运行模式的关闭输出状态 */
    g_sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;          /* 空闲模式的关闭输出状态 */
    g_sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                /* 不用寄存器锁功能 */
    g_sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE;                /* 使能刹车输入 */
    g_sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       /* 刹车输入有效信号极性为高 */
    g_sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* 使能AOE位，允许刹车结束后自动恢复输出 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);         /* OCy 输出使能 */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);      /* OCyN 输出使能 */
}

/**
 * @brief       定时器TIMX 设置输出比较值 & 死区时间
 * @param       ccr: 输出比较值
 * @param       dtg: 死区时间
 *   @arg       dtg[7:5]=0xx时, 死区时间 = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10x时, 死区时间 = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110时, 死区时间 = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111时, 死区时间 = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 42M = 23.8ns
 * @retval      无
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    g_sbreak_dead_time_config.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);      /*重设死区时间*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);      /* MOE=1,使能主输出 */    
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;                      /* 设置比较寄存器 */

}

