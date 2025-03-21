#include "key.h"
key_irq_stru key_irq_usr;



void KeySw1Irq(unsigned key_irq_num)
{
    switch (key_irq_num)
    {
        case KEY1_NUM:
            key_irq_usr.key_irq_status = key_irq_usr.key_irq_status | 0x01;
            break;
        case KEY2_NUM:
            key_irq_usr.key_irq_status  = key_irq_usr.key_irq_status | 0x02;
            break;
        case KEY3_NUM:
            key_irq_usr.key_irq_status  = key_irq_usr.key_irq_status | 0x04;
            break;
    }
	key_irq_usr.update = 1;
}
void KeyProc(void)
{
	if(key_irq_usr.update == 1)
	{
		key_irq_usr.update = 0;
	}
}
void GPIOA_IRQHandlerCallback(void)
{
    if (CW_GPIOA->ISR_f.PIN3)
    {
        GPIOA_INTFLAG_CLR(bv3);
        KeySw1Irq(KEY2_NUM);
    }
    if (CW_GPIOA->ISR_f.PIN4)
    {
        GPIOA_INTFLAG_CLR(bv4);
        KeySw1Irq(KEY3_NUM);
    }

}
void GPIOB_IRQHandlerCallback(void)
{
    if (CW_GPIOA->ISR_f.PIN2)
    {
        GPIOA_INTFLAG_CLR(bv2);
        KeySw1Irq(KEY1_NUM);

    }
}