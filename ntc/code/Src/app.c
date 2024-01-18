#include "app.h"
#include "adc.h"
#include "math.h"
extern TIM_HandleTypeDef htim2;
Params_stru Params;
Params_stru *getParams()
{
    return &Params;
}
void Params_Init(void)
{
    Params.display_flag = 0;
    Params.mode = 0;
    Params.KeyStatus = 0;
    Params.fm_flag = 0;
    Params.gear_period = HAL_GetTick();
    Params.temperature_period = HAL_GetTick();

}

void SCR_ON(void)
{
    HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_SET);

}
void SCR_OFF(void)
{
    HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_RESET);

}


void FM_ON(void)
{
    HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_SET);
}
void FM_OFF(void)
{
    HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_RESET);
}
void key_process(void)
{
    switch(getParams()->KeyStatus)
    {
    case 0:
        break;
    case 1:
    {
        if(getParams()->mode == 0x01)
        {
            getParams()->mode = 0x00;//�ر���ˮģʽ��ˮģʽ
        }
        else
            getParams()->mode = 0x01;//�޸�ģʽΪ��ˮģʽ
    }
    break;
    case 2:
    {
        if(getParams()->mode == 0x02)
        {
            getParams()->mode = 0x00;
        }
        else
            getParams()->mode = 0x02;//�޸�ģʽΪ��ˮģʽ
    }
    break;
    default :
        getParams()->mode = 0x00;//�ر���ˮģʽ��ˮģʽ
        break;

    }
    getParams()->KeyStatus = 0;

}
void device_ctrl_process(void)
{
    static uint32_t tick=0;
    static unsigned char flag=0;
    if((adc_process(0)&0x01) == 1)
    {
        oled_process(1);////�¶���ʾ
        adc_process(1);
    }


    if(getParams()->mode != 0)//����ģʽ
    {
        if(getParams()->temperature>90)
        {
            getParams()->mode = 0;

        }
        else if(getParams()->temperature >= 50)//�¶ȹ��߱���
        {
            if((getParams()->display_flag&0x0020) == 0)
            {
                oled_process(6);//��ʾ����ͼ��
                getParams()->display_flag = 0x20;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
            }
            //******************************����������******************************************

            if(getParams()->fm_flag==0) 	//��ǰ״̬�ǹر�״̬�ҵ���ʱΪ0
            {
                if(flag == 0)
                {
                    getParams()->fm_period=(getParams()->temperature-50)*1000/50;		//���¶���ֵ*10��ֵ�����������е���ʱ ���1000
                    tick = HAL_GetTick();
                    flag = 1;
                    FM_ON();
                }

            }
            else
            {

                if(flag == 0)
                {
                    unsigned int tmp2;
                    tmp2 = getParams()->temperature-50;
                    tmp2 =tmp2*1000;
                    tmp2 = tmp2/50;
                    getParams()->fm_period=1000-tmp2;
                    //getParams()->fm_period=1000-(getParams()->temperature-50)*1000/50;
                    tick = HAL_GetTick();
                    flag = 1;
                }

            }
            if((HAL_GetTick()-tick)>=getParams()->fm_period&&getParams()->fm_period != 0)
            {
                tick = HAL_GetTick();
                flag = 0;
                if(getParams()->fm_flag == 1)
                {

                    getParams()->fm_flag = 0;
                    FM_OFF();
                }
                else
                {
                    getParams()->fm_flag = 1; //��λ��������־λ
                }
            }

        }
        else
        {
            if(flag == 1||getParams()->fm_flag == 1)
            {
                flag = 0;
                FM_OFF();
            }

            if((getParams()->display_flag&0x40) == 0)
            {
                oled_process(7);//����ˮģʽ��ʾ����ͼ��
                getParams()->display_flag = 0x40;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
            }
            getParams()->fm_period = 0;

        }

    }
    else //ֹͣģʽ
    {
        if((getParams()->display_flag&0x0080) == 0)
        {
            //getParams()->display_flag = 0x80;
            getParams()->fm_period = 0;
            FM_OFF();
            oled_process(8);//�ر�ģʽ��ʾֹͣͼ��
            getParams()->display_flag = 0x80;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
        }

    }
    switch(getParams()->mode)
    {
    case 0:
    {
        if((getParams()->display_flag&0x0004) == 0)
        {
            //getParams()->display_flag = 0x80;
            oled_process(3);//
            getParams()->display_flag = 0x04;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
        }

    }
    break;
    case 1:
    {
        if((getParams()->display_flag&0x0008) == 0)
        {
            //getParams()->display_flag = 0x80;
            oled_process(4);//
            getParams()->display_flag = 0x08;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
        }

    }
    break;
    case 2:
    {
        if((getParams()->display_flag&0x0010) == 0)
        {
            //getParams()->display_flag = 0x80;
            oled_process(5);//
            getParams()->display_flag = 0x10;//0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 0x100
        }

    }
    break;

    }
    if(HAL_GPIO_ReadPin(WATER_GPIO_Port, WATER_Pin)==1||getParams()->temperature>90)
    {
        getParams()->mode = 0;

    }

    if(HAL_GPIO_ReadPin(WATER_GPIO_Port, WATER_Pin)==1)
        FM_ON();
    else
        FM_OFF();

}
void gear_process(void)
{
    //��λ��������ʾ
    if((adc_process(0)&0x02)== 2)
    {
        oled_process(2);//��λ��ʾ
        adc_process(2);
    }

    if(getParams()->mode!=0) 	//���Ϊ��ˮģʽ ����ݵ�����ť��ȡ�������޸�pwmռ�ձ�
    {
        if(getParams()->mode==2&&getParams()->precent>0.2)			//���Ϊ����ˮģʽ�Ҵﵽһ����ˮ�� ��������բ��
        {
            SCR_ON();
        }
        else				//����رվ�բ��
        {
            SCR_OFF();
        }
        float tmp;
      //  Params.precent=Get_Precent();		  //��ȡ������ť���ڵİٷֱ�ֵ
        //tmp=1-getParams()->precent;
        tmp=getParams()->precent;
        if(tmp <=0.5&&tmp>=0.1)
        {
			tmp=360+tmp*200;
		}
		else
        tmp=tmp*900;
        getParams()->duty=round(tmp);	//�����Ӧ�ıȽϼĴ���ֵ 700Ϊ������ܱ�����ʱ��Ӧ�ıȽϼĴ���ֵ
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, getParams()->duty);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, getParams()->duty);

    }
    else					//���ģʽΪֹͣģʽ ����ֹͣ
    {
        SCR_OFF();	//ֹͣģʽ�رվ�բ��
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1);

    }

}
void app_process()
{
    key_process();
    gear_process();
    device_ctrl_process();



}
