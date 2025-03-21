;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- STC 1T Series MCU Demo Programme -------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
;/* --- Web: www.STCAI.com ---------------------------------------------*/
;/* --- BBS: www.STCAIMCU.com ------------------------------------------*/
;/* --- QQ:  800003751 -------------------------------------------------*/
;/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
;/*---------------------------------------------------------------------*/


;*************  ����˵��    **************

;�����̻���STC32GΪ����оƬ��ʵ������б�д���ԡ�

;ʹ��Keil C251��������Memory Model�Ƽ�����XSmallģʽ��Ĭ�϶��������edata����ʱ�Ӵ�ȡ�����ٶȿ졣

;edata���鱣��1K����ջʹ�ã��ռ䲻��ʱ�ɽ������顢�����ñ�����xdata�ؼ��ֶ��嵽xdata�ռ䡣

;�Ƚ���������ͨ�� P3.7 �˿����ӿɵ����裬��1.19V�ڲ��̶��Ƚϣ�

;���ڿɵ����裬��P3.7�ڵ������ѹ����1.19Vʱ�����Ƚ����жϣ���ʱ�������ݵ�EEPROM��

;��P3.7�ڵ������ѹ����1.19V�󣬻��������ϵ�ʱ����ȡEEPROM���ݽ��лָ���

;��STC��MCU��IO��ʽ����8λ����ܡ�

;ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������,�û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.

;�û�������"�û������"���޸ı������ݵ�EEPROM��ַ��

;��ʾЧ��Ϊ: �ϵ����ʾ�����, ������ΧΪ0~10000����ʾ���ұߵ�5������ܡ�

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;ע�⣺����ʱ�����ؽ���"Ӳ��ѡ��"���������Ҫȷ����������:

;�����û�EEPROM��С����ȷ��"�û������"���趨�ĵ�ַ��EEPROM���õĴ�С��Χ֮�ڡ�

;�޸Ĺ�Ӳ��ѡ���Ҫ����Ƭ�������ϵ��Ż���Ч��

;ѡ��ʱ�� 24MHZ (�û��������޸�Ƶ��)��
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

;******************************************

$include (../../comm/STC32G.INC)

;/****************************** �û������ ***********************************/

Fosc_KHZ    EQU 24000   ;24MHZ

STACK_POIRTER   EQU     0D0H    ; ��ջ��ʼ��ַ

Timer0_Reload   EQU     (65536 - Fosc_KHZ)  ; Timer 0 �ж�Ƶ��, 1000��/��

EE_ADDRESS_E    EQU     000H  ;����ĵ�ַ��8λ
EE_ADDRESS_H    EQU     000H  ;����ĵ�ַ��8λ
EE_ADDRESS_L    EQU     040H  ;����ĵ�ַ��8λ

DIS_DOT         EQU     020H
DIS_BLACK       EQU     010H
DIS_            EQU     011H

;*******************************************************************
;*******************************************************************

IAP_EN          EQU     (1 SHL 7)
IAP_SWBS        EQU     (1 SHL 6)
IAP_SWRST       EQU     (1 SHL 5)
IAP_CMD_FAIL    EQU     (1 SHL 4)

TPS_WAIT        EQU     24   ;24MHZ / 1000000

;*************  IO�ڶ���    **************/


;*************  ���ر�������    **************/
Flag0           DATA    20H
B_1ms           BIT     Flag0.0 ;   1ms��־
LowVolFlag      BIT     Flag0.1 ;   �͵�ѹ��־

Temp_cnt_H      DATA    2EH     ; �����õ����������
Temp_cnt_L      DATA    2FH

LED8            DATA    30H     ;   ��ʾ���� 30H ~ 37H
display_index   DATA    38H     ;   ��ʾλ����

msecond_H       DATA    39H     ;
msecond_L       DATA    3AH     ;

Test_cnt_H      DATA    3BH     ; �����õ����������
Test_cnt_L      DATA    3CH

address_H       DATA    3DH
address_L       DATA    3EH
length          DATA    3FH

;*******************************************************************
;*******************************************************************
        ORG     0000H               ;����λ��ڣ��������Զ����嵽 0FF0000H ��ַ
        LJMP    F_Main

        ORG     000BH               ;1  Timer0 interrupt
        LJMP    F_Timer0_Interrupt

        ORG     00ABH               ;21 CMP interrupt
        LJMP    F_CMP_Interrupt


;*******************************************************************
;*******************************************************************


;******************** ������ **************************/
        ORG     0100H       ;�������Զ����嵽 0FF0100H ��ַ
F_Main:
    MOV     WTST, #00H     ;���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    MOV     CKCON,#00H     ;��߷���XRAM�ٶ�
    ORL     P_SW2,#080H    ;ʹ�ܷ���XFR

    MOV     P0M1, #30H     ;����P0.4��P0.5Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P0M0, #30H
    MOV     P1M1, #30H     ;����P1.4��P1.5Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P1M0, #30H
    MOV     P2M1, #3cH     ;����P2.2~P2.5Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P2M0, #3cH
    MOV     P3M1, #50H     ;����P3.4��P3.6Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P3M0, #50H
    MOV     P4M1, #3cH     ;����P4.2~P4.5Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P4M0, #3cH
    MOV     P5M1, #0cH     ;����P5.2��P5.3Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P5M0, #0cH
    MOV     P6M1, #0ffH    ;����Ϊ©����·(ʵ��������������赽3.3V)
    MOV     P6M0, #0ffH
    MOV     P7M1, #00H     ;����Ϊ׼˫���
    MOV     P7M0, #00H

    
    MOV     SP, #STACK_POIRTER
    MOV     PSW, #0
    USING   0       ;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================

    MOV     display_index, #0
    MOV     R0, #LED8
    MOV     R2, #8
L_ClearLoop:
    MOV     @R0, #DIS_BLACK     ;�ϵ�����
    INC     R0
    DJNZ    R2, L_ClearLoop
    
    CLR     TR0
    ORL     AUXR, #(1 SHL 7)    ; Timer0_1T();
    ANL     TMOD, #NOT 04H      ; Timer0_AsTimer();
    ANL     TMOD, #NOT 03H      ; Timer0_16bitAutoReload();
    MOV     TH0, #Timer0_Reload / 256   ;Timer0_Load(Timer0_Reload);
    MOV     TL0, #Timer0_Reload MOD 256
    SETB    ET0         ; Timer0_InterruptEnable();
    SETB    TR0         ; Timer0_Run();

    SETB    EA          ; �����ж�
    
    MOV     R2, #200    ; delay 200ms
L_PowerUpDelay:
    JNB     B_1ms, $    ; �ȴ�1ms��
    CLR     B_1ms
    DJNZ    R2, L_PowerUpDelay

    MOV     length, #2      ;����2�ֽ�
    MOV     R0, #Test_cnt_H
    LCALL   F_EEPROM_read_n     ;��N���ֽ�
    MOV     A, Test_cnt_L
    CLR     C
    SUBB    A, #LOW 10000
    MOV     A, Test_cnt_H
    SUBB    A, #HIGH 10000
    JC      L_LessThan10000
    MOV     Test_cnt_H, #0
    MOV     Test_cnt_L, #0
L_LessThan10000:

    LCALL   F_CMP_Config    ; �Ƚ�����ʼ��
    LCALL   F_Display       ; ��ʾ�����
    
;=================== ��ѭ�� ==================================
L_MainLoop:
    JB      LowVolFlag, L_MainLoop ;�͵�ѹʱ������
    JNB     B_1ms, L_MainLoop      ;1msδ��
    CLR     B_1ms
    
    INC     msecond_L       ;msecond + 1
    MOV     A, msecond_L
    JNZ     $+4
    INC     msecond_H
    
    MOV     A, msecond_L    ;msecond - 1000
    CLR     C
    SUBB    A, #LOW 1000
    MOV     A, msecond_H
    SUBB    A, #HIGH 1000
    JC      L_MainLoop      ;if(msecond < 1000), jmp
    
    MOV     msecond_L, #0   ;if(msecond >= 1000)
    MOV     msecond_H, #0

    INC     Test_cnt_L      ;Test_cnt + 1
    MOV     A, Test_cnt_L
    JNZ     $+4
    INC     Test_cnt_H

    MOV     A, Test_cnt_L   ;if(Test_cnt > 10000)   Test_cnt = 0;   �������ΧΪ0~10000
    CLR     C
    SUBB    A, #LOW 10000
    MOV     A, Test_cnt_H
    SUBB    A, #HIGH 10000
    JC      L_LessThan10000A
    MOV     Test_cnt_H, #0
    MOV     Test_cnt_L, #0
L_LessThan10000A:
    LCALL   F_Display       ; ��ʾ�����

    LJMP    L_MainLoop

;**********************************************/

;========================================================================
; ����: F_CMP_Config
; ����: �Ƚ�����ʼ������.
; ����: non.
; ����: non.
; �汾: V1.0, 2021-3-3
;========================================================================
F_CMP_Config:
    CLR     A
    ANL     A,#NOT 03H                  ; P3.7ΪCMP+�����
;    ORL     A,#01H                      ; P5.0ΪCMP+�����
;    ORL     A,#02H                      ; P5.1ΪCMP+�����
;    ORL     A,#03H                      ; ADC�����ΪCMP+�����
;    ANL     A,#NOT 04H                  ; P3.6ΪCMP-�����
    ORL     A,# 04H                     ; �ڲ�1.19V�ο��ź�ԴΪCMP-�����
    MOV     WR6, #WORD0 CMPEXCFG
    MOV     WR4, #WORD2 CMPEXCFG
    MOV     @DR4, R11

    MOV     CMPCR2,#00H
    ANL     CMPCR2,#NOT 80H             ;�Ƚ����������
;    ORL     CMPCR2,#80H                 ;�Ƚ����������
    ANL     CMPCR2,#NOT 40H             ;ʹ��0.1us�˲�
;    ORL     CMPCR2,#40H                 ;��ֹ0.1us�˲�
;    ANL     CMPCR2,#NOT 3FH             ;�Ƚ������ֱ�����
    ORL     CMPCR2,#10H                 ;�Ƚ����������16��ȥ��ʱ�Ӻ����
    MOV     CMPCR1,#00H
    ORL     CMPCR1,#30H                 ;ʹ�ܱȽ��������ж�
;    ANL     CMPCR1,#NOT 20H             ;��ֹ�Ƚ����������ж�
;    ORL     CMPCR1,#20H                 ;ʹ�ܱȽ����������ж�
;    ANL     CMPCR1,#NOT 10H             ;��ֹ�Ƚ����½����ж�
;    ORL     CMPCR1,#10H                 ;ʹ�ܱȽ����½����ж�
;    ANL     CMPCR1,#NOT 02H             ;��ֹ�Ƚ������
    ORL     CMPCR1,#02H                 ;ʹ�ܱȽ������
    ORL     CMPCR1,#80H                 ;ʹ�ܱȽ���ģ��
    RET

;========================================================================
; ����: F_CMP_Interrupt
; ����: �Ƚ����жϺ���.
; ����: non.
; ����: non.
; �汾: V1.0, 2021-3-3
;========================================================================
F_CMP_Interrupt:
    PUSH    PSW
    PUSH    ACC
    PUSH    AR2

    ANL     CMPCR1, #NOT 040H      ; ���жϱ�־
    MOV     A, CMPCR1
	RRC     A
	MOV     P4.7,C

    MOV     A, CMPCR1
	ANL     A, #01H
	JZ      F_CMP_Powerdown
    JNB     LowVolFlag, L_QuitCMP_Init
	CLR     LowVolFlag      ;����͵�ѹ��־
	
	MOV     A, Test_cnt_H
	XRL     A, Temp_cnt_H
	JNZ     F_CMP_Read
	MOV     A, Test_cnt_L
	XRL     A, Temp_cnt_L
	JZ     L_QuitCMP_Init
F_CMP_Read:
    MOV     length, #2      ;����2�ֽ�
    MOV     R0, #Test_cnt_H
    LCALL   F_EEPROM_read_n     ;��N���ֽ�
    MOV     A, Test_cnt_L
    CLR     C
    SUBB    A, #LOW 10000
    MOV     A, Test_cnt_H
    SUBB    A, #HIGH 10000
    JC      F_CMP_LessThan10000
    MOV     Test_cnt_H, #0
    MOV     Test_cnt_L, #0
F_CMP_LessThan10000:
    MOV     Temp_cnt_H, Test_cnt_H
    MOV     Temp_cnt_L, Test_cnt_L
	SJMP    L_QuitCMP_Init

F_CMP_Powerdown:
    MOV     P7, #0xff

    JB      LowVolFlag, L_QuitCMP_Init
	SETB    LowVolFlag      ;���õ͵�ѹ��־

	MOV     A, Test_cnt_H
	XRL     A, Temp_cnt_H
	JNZ     F_CMP_Write
	MOV     A, Test_cnt_L
	XRL     A, Temp_cnt_L
	JZ     L_QuitCMP_Init
F_CMP_Write:
    MOV     Temp_cnt_H, Test_cnt_H
    MOV     Temp_cnt_L, Test_cnt_L
    LCALL   F_EEPROM_SectorErase    ; ����һ������

    MOV     length, #2
    MOV     R0, #Temp_cnt_H
    LCALL   F_EEPROM_write_n        ;дN���ֽ�

L_QuitCMP_Init:
    POP     AR2
    POP     ACC
    POP     PSW
    RETI

;========================================================================
; ����: F_HEX2_DEC
; ����: ��˫�ֽ�ʮ��������ת��ʮ����BCD��.
; ����: (R6 R7): Ҫת����˫�ֽ�ʮ��������.
; ����: (R3 R4 R5) = BCD��.
; �汾: V1.0, 2013-10-22
;========================================================================
F_HEX2_DEC:         ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    MOV     R2,#16
    MOV     R3,#0
    MOV     R4,#0
    MOV     R5,#0

L_HEX2_DEC:
    CLR     C   
    MOV     A,R7
    RLC     A   
    MOV     R7,A
    MOV     A,R6
    RLC     A   
    MOV     R6,A

    MOV     A,R5
    ADDC    A,R5
    DA      A   
    MOV     R5,A

    MOV     A,R4
    ADDC    A,R4
    DA      A   
    MOV     R4,A

    MOV     A,R3
    ADDC    A,R3
    DA      A   
    MOV     R3,A

    DJNZ    R2,L_HEX2_DEC
    RET
;**********************************************/

;========================================================================
; ����: F_Display
; ����: ��ʾ��������.
; ����: non.
; ����: non.
; �汾: V1.0, 2014-1-22
;========================================================================
F_Display:
    MOV     R6, Test_cnt_H
    MOV     R7, Test_cnt_L
    LCALL   F_HEX2_DEC      ;(R6 R7) HEX Change to DEC ---> (R3 R4 R5), use (R2~R7)
    MOV     LED8+4, R3  ;��λ
    MOV     A, R4
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+3, A
    MOV     A, R4
    ANL     A, #0x0F
    MOV     LED8+2, A
    MOV     A, R5
    SWAP    A
    ANL     A, #0x0F
    MOV     LED8+1, A
    MOV     A, R5
    ANL     A, #0x0F
    MOV     LED8+0, A
    
    MOV     R0, #LED8+4
L_Cut0_Loop:            ;����λ0
    MOV     A,@R0
    JNZ     L_QuitCut0
    MOV     @R0, #DIS_BLACK
    DEC     R0
    CJNE    R0, #LED8+0, L_Cut0_Loop
L_QuitCut0: 
    RET


; *********************** ��ʾ��س��� ****************************************
T_Display:                      ;��׼�ֿ�
;    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
DB  03FH,006H,05BH,04FH,066H,06DH,07DH,007H,07FH,06FH,077H,07CH,039H,05EH,079H,071H
;  black  -    H    J    K    L    N    o    P    U    t    G    Q    r    M    y
DB  000H,040H,076H,01EH,070H,038H,037H,05CH,073H,03EH,078H,03dH,067H,050H,037H,06EH
;    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   -1
DB  0BFH,086H,0DBH,0CFH,0E6H,0EDH,0FDH,087H,0FFH,0EFH,046H

T_COM:
DB  001H,002H,004H,008H,010H,020H,040H,080H     ;   λ��


;//========================================================================
;// ����: F_DisplayScan
;// ����: ��ʾɨ���ӳ���
;// ����: none.
;// ����: none.
;// �汾: VER1.0
;// ����: 2013-4-1
;// ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ
;//========================================================================
F_DisplayScan:
    PUSH    DPH     ;DPH��ջ
    PUSH    DPL     ;DPL��ջ
    PUSH    00H     ;R0 ��ջ
    
    MOV     DPTR, #T_COM
    MOV     A, display_index
    MOVC    A, @A+DPTR
    CPL     A
    MOV     P7,A
    
    MOV     DPTR, #T_Display
    MOV     A, display_index
    ADD     A, #LED8
    MOV     R0, A
    MOV     A, @R0
    MOVC    A, @A+DPTR
    CPL     A
    MOV     P6,A

    INC     display_index
    MOV     A, display_index
    ANL     A, #0F8H            ; if(display_index >= 8)
    JZ      L_QuitDisplayScan
    MOV     display_index, #0;  ;8λ������0
L_QuitDisplayScan:
    POP     00H     ;R0 ��ջ
    POP     DPL     ;DPL��ջ
    POP     DPH     ;DPH��ջ
    RET


;*******************************************************************
;**************** �жϺ��� ***************************************************

F_Timer0_Interrupt: ;Timer0 1ms�жϺ���
    PUSH    PSW     ;PSW��ջ
    PUSH    ACC     ;ACC��ջ

    JB      LowVolFlag, F_QuitTimer0_Int
    LCALL   F_DisplayScan   ; 1msɨ����ʾһλ
    SETB    B_1ms           ; 1ms��־

F_QuitTimer0_Int:
    POP     ACC     ;ACC��ջ
    POP     PSW     ;PSW��ջ
    RETI
    


;========================================================================
; ����: F_DisableEEPROM
; ����: ��ֹ����ISP/IAP.
; ����: non.
; ����: non.
; �汾: V1.0, 2012-10-22
;========================================================================
F_DisableEEPROM:
    MOV     IAP_CONTR, #0       ; ��ֹISP/IAP����
    MOV     IAP_CMD,  #0        ; ȥ��ISP/IAP����
    MOV     IAP_TRIG, #0        ; ��ֹISP/IAP�����󴥷�
    MOV     IAP_ADDRH, #0FFH    ; ��0��ַ���ֽ�
    MOV     IAP_ADDRL, #0FFH    ; ��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
    RET

;========================================================================
; ����: F_EEPROM_read_n
; ����: ��ָ��EEPROM�׵�ַ����n���ֽڷ�ָ���Ļ���.
; ����: EE_ADDRESS:           ����EEPROM���׵�ַ.
;       R0:                   �������ݷŻ�����׵�ַ.
;       length:               �������ֽڳ���.
; ����: non.
; �汾: V1.0, 2012-10-22
;========================================================================
F_EEPROM_read_n:
    PUSH    AR2

    MOV     R2, length
    MOV     IAP_ADDRE, #EE_ADDRESS_E    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRH, #EE_ADDRESS_H    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRL, #EE_ADDRESS_L    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_CONTR, #IAP_EN      ; ����ISP/IAP��������һ�ξ͹�
	MOV     IAP_TPS, #TPS_WAIT      ; ���õȴ�ʱ�䣬
    MOV     IAP_CMD, #1             ; ISP�����������ֽڶ���������ı�ʱ����������������
    MOV     C, EA
    MOV     F0, C   ;����EA
L_EE_Read_Loop:
    CLR     EA      ; ��ֹ�ж�
    MOV     IAP_TRIG, #0x5A         ;ISP��������
    MOV     IAP_TRIG, #0xA5
    NOP     ;����STC32G�Ƕ༶��ˮ�ߵ�ָ��ϵͳ��������������4��NOP����֤IAP_DATA���������׼��
    NOP
    NOP
    NOP
    MOV     C, F0
    MOV     EA, C       ; �����ж�(�������)
    MOV     @R0, IAP_DATA

    MOV     A, IAP_ADDRL    ;
    ADD     A, #1
    MOV     IAP_ADDRL, A
    MOV     A, IAP_ADDRH
    ADDC    A, #0
    MOV     IAP_ADDRH, A
    INC     R0
    DJNZ    R2, L_EE_Read_Loop

    LCALL   F_DisableEEPROM
    POP     AR2
    RET

;========================================================================
; ����: F_EEPROM_SectorErase
; ����: ��ָ����ַ��EEPROM��������.
; ����: EE_ADDRESS:  Ҫ����������EEPROM�ĵ�ַ.
; ����: non.
; �汾: V1.0, 2013-5-10
;========================================================================
F_EEPROM_SectorErase:
                                            ;ֻ������������û���ֽڲ�����512�ֽ�/������
                                            ;����������һ���ֽڵ�ַ����������ַ��
    MOV     IAP_ADDRE, #EE_ADDRESS_E    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRH, #EE_ADDRESS_H    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRL, #EE_ADDRESS_L    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_CONTR, #IAP_EN      ; ����ISP/IAP��������һ�ξ͹�
	MOV     IAP_TPS, #TPS_WAIT      ; ���õȴ�ʱ�䣬
    MOV     IAP_CMD, #3     ;��������������
    MOV     C, EA
    MOV     F0, C   ;����EA
    CLR     EA      ; ��ֹ�ж�
    MOV     IAP_TRIG, #0x5A         ;ISP��������
    MOV     IAP_TRIG, #0xA5
    NOP     ;����STC32G�Ƕ༶��ˮ�ߵ�ָ��ϵͳ��������������4��NOP����֤IAP_DATA���������׼��
    NOP
    NOP
    NOP
    MOV     C, F0
    MOV     EA, C       ; �����ж�(�������)
    LCALL   F_DisableEEPROM
    RET

;========================================================================
; ����: F_EEPROM_write_n
; ����: �ѻ����n���ֽ�д��ָ���׵�ַ��EEPROM.
; ����: EE_ADDRESS:             д��EEPROM���׵�ַ.
;       R0:                     д��Դ���ݵĻ�����׵�ַ.
;       length:                 д����ֽڳ���.
; ����: non.
; �汾: V1.0, 2014-1-22
;========================================================================
F_EEPROM_write_n:
    PUSH    AR2
    MOV     R2, length
    MOV     IAP_ADDRE, #EE_ADDRESS_E    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRH, #EE_ADDRESS_H    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_ADDRL, #EE_ADDRESS_L    ; �͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
    MOV     IAP_CONTR, #IAP_EN      ; ����ISP/IAP��������һ�ξ͹�
	MOV     IAP_TPS, #TPS_WAIT      ; ���õȴ�ʱ�䣬
    MOV     IAP_CMD, #2     ;���ֽ�д��������ı�ʱ����������������
    MOV     C, EA
    MOV     F0, C   ;����EA
L_EE_W_Loop:
    MOV     IAP_DATA, @R0   ; �����ݵ�IAP_DATA��ֻ�����ݸı�ʱ����������
    CLR     EA      ; ��ֹ�ж�
    MOV     IAP_TRIG, #0x5A         ;ISP��������
    MOV     IAP_TRIG, #0xA5
    NOP     ;����STC32G�Ƕ༶��ˮ�ߵ�ָ��ϵͳ��������������4��NOP����֤IAP_DATA���������׼��
    NOP
    NOP
    NOP
    MOV     C, F0
    MOV     EA, C       ; �����ж�(�������)
    MOV     A, IAP_ADDRL    ;
    ADD     A, #1
    MOV     IAP_ADDRL, A
    MOV     A, IAP_ADDRH
    ADDC    A, #0
    MOV     IAP_ADDRH, A
    INC     R0
    DJNZ    R2, L_EE_W_Loop

    LCALL   F_DisableEEPROM
    POP     AR2
    RET


    END

