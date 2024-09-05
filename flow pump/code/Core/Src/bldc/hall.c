#include "hall.h"
#include "motor.h"

extern uint8_t Duty;

extern MOTOR my_motor;


/*HALL�жϷ�����*/
/*��HALLֵ�����仯��ʱ��ᴥ������ж�*/
/*�����غ��½��ش���*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //static uint8_t HALL_state; 
    LED0_ON;
    #if 1//�������룬������
        if(my_motor.Status == RUN)
        {
            /*1.�޸�����ͨ����ռ�ձ�*/
            my_motor.Duty = Duty;//���˵���ռ�ձȸ�ֵ���������ͳ��������
            Change_PWM_Duty(my_motor.Duty);
            /*2.��ʼ��ȡHALLֵ*/
            //user_main_info("HALL��ֵΪ��%d",get_hall_state());
            my_motor.Hall_status = get_hall_state();
            /*3.���濪��6������*///--->����HALLֵ���л���
            //�Ϲܵ��ƣ��¹�����
            Six_Step_Phase(my_motor.Hall_status,my_motor.Direction);
        }

    #endif
    LED0_OFF;

}


//��ȡ����������ֵ
uint8_t get_hall_state(void)
{
    uint8_t state = 0; 
    uint8_t A,B,C;
    A = HAL_GPIO_ReadPin(HALLA_GPIO_Port, HALLA_Pin);
    B = HAL_GPIO_ReadPin(HALLB_GPIO_Port, HALLB_Pin);
    C = HAL_GPIO_ReadPin(HALLC_GPIO_Port, HALLC_Pin);
    state = (C << 2) + (B << 1) + A;
    return state;
}


