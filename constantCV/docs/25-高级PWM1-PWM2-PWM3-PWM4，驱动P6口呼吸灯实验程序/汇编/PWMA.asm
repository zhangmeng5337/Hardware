;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- STC 1T Series MCU Demo Programme -------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
;/* --- Web: www.STCAI.com ---------------------------------------------*/
;/* --- BBS: www.STCAIMCU.com ------------------------------------------*/
;/* --- QQ:  800003751 -------------------------------------------------*/
;/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
;/*---------------------------------------------------------------------*/


;/************* 功能说明    **************

;本例程基于STC32G为主控芯片的实验箱进行编写测试。

;使用Keil C251编译器，Memory Model推荐设置XSmall模式，默认定义变量在edata，单时钟存取访问速度快。

;edata建议保留1K给堆栈使用，空间不够时可将大数组、不常用变量加xdata关键字定义到xdata空间。

;高级PWM定时器 PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N 每个通道都可独立实现PWM输出，或者两两互补对称输出.

;8个通道PWM设置对应P6的8个端口.

;通过P6口上连接的8个LED灯，利用PWM实现呼吸灯效果.

;PWM周期和占空比可以根据需要自行设置，最高可达65535.

;下载时, 选择时钟 24MHZ (用户可自行修改频率).

;******************************************/

$include (../../comm/STC32G.INC)

;/****************************** 用户定义宏 ***********************************/

Fosc_KHZ    EQU 24000   ;24000KHZ

STACK_POIRTER   EQU     0D0H    ;堆栈开始地址

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 中断频率, 1000次/秒

;*******************************************************************
;*******************************************************************


;*************  本地变量声明    **************/
PWM1_Flag       BIT     20H.0
PWM2_Flag       BIT     20H.1
PWM3_Flag       BIT     20H.2
PWM4_Flag       BIT     20H.3

PWM1_Duty       DATA    30H
PWM2_Duty       DATA    32H
PWM3_Duty       DATA    34H
PWM4_Duty       DATA    36H

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;程序复位入口，编译器自动定义到 0FF0000H 地址
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

;******************** 主程序 **************************/
        ORG     0100H       ;编译器自动定义到 0FF0100H 地址
F_Main:
    MOV     WTST, #00H     ;设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    MOV     CKCON,#00H     ;提高访问XRAM速度
    ORL     P_SW2,#080H    ;使能访问XFR

    MOV     P0M1, #30H     ;设置P0.4、P0.5为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P0M0, #30H
    MOV     P1M1, #30H     ;设置P1.4、P1.5为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P1M0, #30H
    MOV     P2M1, #3cH     ;设置P2.2~P2.5为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P2M0, #3cH
    MOV     P3M1, #50H     ;设置P3.4、P3.6为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P3M0, #50H
    MOV     P4M1, #3cH     ;设置P4.2~P4.5为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P4M0, #3cH
    MOV     P5M1, #0cH     ;设置P5.2、P5.3为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P5M0, #0cH
    MOV     P6M1, #0ffH    ;设置为漏极开路(实验箱加了上拉电阻到3.3V)
    MOV     P6M0, #0ffH
    MOV     P7M1, #00H     ;设置为准双向口
    MOV     P7M0, #00H

    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0
    USING   0       ;选择第0组R0~R7

;================= 用户初始化程序 ====================================
    CLR     PWM1_Flag
    CLR     PWM2_Flag
    CLR     PWM3_Flag
    CLR     PWM4_Flag
    MOV     WR6, #0
    MOV     PWM1_Duty, WR6
    MOV     WR6, #256
    MOV     PWM2_Duty, WR6
    MOV     WR6, #512
    MOV     PWM3_Duty, WR6
    MOV     WR6, #1024
    MOV     PWM4_Duty, WR6

    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();
    SETB    EA          ; 打开总中断
    
    LCALL   F_PWM_Init          ; PWM初始化
    CLR     P4.0

;=================== 主循环 ==================================
L_Main_Loop:

    LJMP    L_Main_Loop

;========================================================================
; 函数: F_PWM_Init
; 描述: PWM初始化程序.
; 参数: none
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_PWM_Init:
    MOV     A, #00H              ;写 CCMRx 前必须先清零 CCxE 关闭通道
    MOV     WR6, #WORD0 PWMA_CCER1
    MOV     WR4, #WORD2 PWMA_CCER1
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCER2
    MOV     WR4, #WORD2 PWMA_CCER2
    MOV     @DR4, R11
    MOV     A, #060H             ;置 PWMx 模式1 输出
    MOV     WR6, #WORD0 PWMA_CCMR1
    MOV     WR4, #WORD2 PWMA_CCMR1
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR2
    MOV     WR4, #WORD2 PWMA_CCMR2
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR3
    MOV     WR4, #WORD2 PWMA_CCMR3
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR4
    MOV     WR4, #WORD2 PWMA_CCMR4
    MOV     @DR4, R11
    MOV     A, #055H             ;配置通道输出使能和极性
    MOV     WR6, #WORD0 PWMA_CCER1
    MOV     WR4, #WORD2 PWMA_CCER1
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCER2
    MOV     WR4, #WORD2 PWMA_CCER2
    MOV     @DR4, R11

    MOV     A, #068H             ;开启PWMA_CCRx预转载功能(需要CCxE=1才可写)
    MOV     WR6, #WORD0 PWMA_CCMR1
    MOV     WR4, #WORD2 PWMA_CCMR1
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR2
    MOV     WR4, #WORD2 PWMA_CCMR2
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR3
    MOV     WR4, #WORD2 PWMA_CCMR3
    MOV     @DR4, R11
    MOV     WR6, #WORD0 PWMA_CCMR4
    MOV     WR4, #WORD2 PWMA_CCMR4
    MOV     @DR4, R11

    MOV     A, #3                ;设置周期时间
    MOV     WR6, #WORD0 PWMA_ARRH
    MOV     WR4, #WORD2 PWMA_ARRH
    MOV     @DR4, R11
    MOV     A, #0FFH
    MOV     WR6, #WORD0 PWMA_ARRL
    MOV     WR4, #WORD2 PWMA_ARRL
    MOV     @DR4, R11

    MOV     A, #0FFH             ;使能 PWM1~4 输出
    MOV     WR6, #WORD0 PWMA_ENO
    MOV     WR4, #WORD2 PWMA_ENO
    MOV     @DR4, R11
    MOV     A, #0AAH             ;高级 PWM 通道输出脚选择位, P6
    MOV     WR6, #WORD0 PWMA_PS
    MOV     WR4, #WORD2 PWMA_PS
    MOV     @DR4, R11
    MOV     A, #080H             ;使能主输出
    MOV     WR6, #WORD0 PWMA_BKR
    MOV     WR4, #WORD2 PWMA_BKR
    MOV     @DR4, R11

    MOV     WR6, #WORD0 PWMA_CR1
    MOV     WR4, #WORD2 PWMA_CR1
    MOV     R11, @DR4
    ORL     A,#081H              ;使能ARR预装载,开始计时
    MOV     @DR4, R11
    RET

;========================================================================
; 函数: F_UpdatePwm
; 描述: 更新PWM占空比值. 
; 参数: [PWMn_Duty_H PWMn_Duty_L]: pwm占空比值.
; 返回: none.
; 版本: V1.0, 2021-3-3
;========================================================================
F_UpdatePwm:
    MOV     WR2, PWM1_Duty      ;设置占空比时间
    MOV     WR6, #WORD0 PWMA_CCR1H
    MOV     WR4, #WORD2 PWMA_CCR1H
    MOV     @DR4, R2
    MOV     WR6, #WORD0 PWMA_CCR1L
    MOV     @DR4, R3

    MOV     WR2, PWM2_Duty      ;设置占空比时间
    MOV     WR6, #WORD0 PWMA_CCR2H
    MOV     WR4, #WORD2 PWMA_CCR2H
    MOV     @DR4, R2
    MOV     WR6, #WORD0 PWMA_CCR2L
    MOV     @DR4, R3

    MOV     WR2, PWM3_Duty      ;设置占空比时间
    MOV     WR6, #WORD0 PWMA_CCR3H
    MOV     WR4, #WORD2 PWMA_CCR3H
    MOV     @DR4, R2
    MOV     WR6, #WORD0 PWMA_CCR3L
    MOV     @DR4, R3

    MOV     WR2, PWM4_Duty      ;设置占空比时间
    MOV     WR6, #WORD0 PWMA_CCR4H
    MOV     WR4, #WORD2 PWMA_CCR4H
    MOV     @DR4, R2
    MOV     WR6, #WORD0 PWMA_CCR4L
    MOV     @DR4, R3
    RET

;**************** 中断函数 ***************************************************
F_Timer0_Interrupt: ;Timer0 1ms中断函数
    PUSH    PSW     ;PSW入栈
    PUSH    ACC     ;ACC入栈
    PUSH    R1      ;R1入栈
    PUSH    R0      ;R0入栈

    JB      PWM1_Flag, T0_PWM1_SUB
    MOV     WR0, PWM1_Duty
    INC     WR0, #1
    MOV     PWM1_Duty, WR0   ;PWM1_Duty + 1
    CMP     WR0, #1023
    JC      T0_PWM2_Start
    SETB    PWM1_Flag
    SJMP    T0_PWM2_Start
T0_PWM1_SUB:
    MOV     WR0, PWM1_Duty
    DEC     WR0, #1
    MOV     PWM1_Duty, WR0   ;PWM1_Duty - 1
    CMP     WR0, #0
    JG      T0_PWM2_Start
    CLR     PWM1_Flag

T0_PWM2_Start:
    JB      PWM2_Flag, T0_PWM2_SUB
    MOV     WR0, PWM2_Duty
    INC     WR0, #1
    MOV     PWM2_Duty, WR0   ;PWM2_Duty + 1
    CMP     WR0, #1023
    JC      T0_PWM3_Start
    SETB    PWM2_Flag
    SJMP    T0_PWM3_Start
T0_PWM2_SUB:
    MOV     WR0, PWM2_Duty
    DEC     WR0, #1
    MOV     PWM2_Duty, WR0   ;PWM2_Duty - 1
    CMP     WR0, #0
    JG      T0_PWM3_Start
    CLR     PWM2_Flag

T0_PWM3_Start:
    JB      PWM3_Flag, T0_PWM3_SUB
    MOV     WR0, PWM3_Duty
    INC     WR0, #1
    MOV     PWM3_Duty, WR0   ;PWM3_Duty + 1
    CMP     WR0, #1023
    JC      T0_PWM4_Start
    SETB    PWM3_Flag
    SJMP    T0_PWM4_Start
T0_PWM3_SUB:
    MOV     WR0, PWM3_Duty
    DEC     WR0, #1
    MOV     PWM3_Duty, WR0   ;PWM3_Duty - 1
    CMP     WR0, #0
    JG      T0_PWM4_Start
    CLR     PWM3_Flag

T0_PWM4_Start:
    JB      PWM4_Flag, T0_PWM4_SUB
    MOV     WR0, PWM4_Duty
    INC     WR0, #1
    MOV     PWM4_Duty, WR0   ;PWM4_Duty + 1
    CMP     WR0, #1023
    JC      F_QuitTimer0
    SETB    PWM4_Flag
    SJMP    F_QuitTimer0
T0_PWM4_SUB:
    MOV     WR0, PWM4_Duty
    DEC     WR0, #1
    MOV     PWM4_Duty, WR0   ;PWM4_Duty - 1
    CMP     WR0, #0
    JG      F_QuitTimer0
    CLR     PWM4_Flag

F_QuitTimer0:
    LCALL   F_UpdatePwm

    POP     R0      ;R0出栈
    POP     R1      ;R1出栈
    POP     ACC     ;ACC出栈
    POP     PSW     ;PSW出栈
    RETI

;======================================================================

    END

