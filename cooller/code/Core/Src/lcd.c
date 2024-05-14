#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "lcd.h"
#include "rf_drv.h"
#include "config.h"
#include "adc.h"
#include "stdio.h"
#include "lcd.h"
#include "usb_ctrl.h"
#include "usb_host.h"
#include "stdlib.h"
#include "string.h"
#include "mystring.h"

uint8_t cmd_buffer[CMD_MAX_SIZE];
static uint8_t update_en = 0;
static uint8_t page_Id_bk = 0;
static uint8_t page_Id = 0;
static uint8_t targe_Id = 0;
/*!
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage(PCTRL_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//ָ������
    uint8_t control_id  = msg->control_id;//�ؼ�Id
    uint8_t page_id     = msg->page_id;   //ҳ��Id
    uint8_t status        = msg->status;
    uint8_t key_type   = msg->key_type;
    uint8_t key_value  = msg->key_value;


    switch (cmd_type)
    {
        case NOTIFY_SET_PAGE:
            NotifySetPage(control_id);
            break;

        case NOTIFY_TOUCH_BUTTON:
            NotifyTouchButton(page_id, control_id, status, key_type, key_value);
            break;

        case NOTIFY_TOUCH_CHECKBOX:
            NotifyTouchCheckbox(page_id, control_id, status, key_type, key_value);
            break;

        case NOTIFY_TOUCH_SLIDER:
            NotifyTouchSlider(page_id, control_id, status, key_type, key_value);
            break;

        case NOTIFY_TOUCH_EDIT:
            NotifyTouchEdit(page_id, control_id, status, key_type, key_value);
            break;

        case NOTIFY_GET_EDIT:
            NotifyGetEdit((PEDIT_MSG)cmd_buffer, size - 4); //ȥ��֡ͷ֡β
            break;

        case NOTIFY_GET_TOUCH_EDIT:
            NotifyGetTouchEdit((PEDIT_MSG)cmd_buffer, size - 4); //ȥ��֡ͷ֡β
            break;

        case NOTIFY_GET_PAGE:
            NotifyGetPage(page_id, status);
            break;

        case NOTIFY_GET_CHECKBOX:
            NotifyGetCheckbox(page_id, control_id, status, key_type, key_value);
            break;

        case NOTIFY_GET_SLIDER:
            NotifyGetSlider(page_id, control_id, status, key_type, key_value);
            break;

        default:
            break;
    }
}


void set_label_proc(unsigned char num)
{
    switch (num)
    {
        case 0:
            SetLableValue(Setting_PAGE, USB_LOG_ID, "");
            break;
        case 1:
            SetLableValue(Setting_PAGE, USB_LOG_ID, "�����U��");
            break;
        case 2:
            SetLableValue(Setting_PAGE, USB_LOG_ID, "δʶ��U��");
            break;
        case 3:
            SetLableValue(Setting_PAGE, USB_LOG_ID, "���ݵ�����...");
            break;
        case 4:
            SetLableValue(Setting_PAGE, USB_LOG_ID, "���ݵ������");
            break;

    }
}


void UpdateUI()
{
    int i;
    int value;
    char str[65];
    if (page_Id == Setting_PAGE)
    {
        if (getConfig()->export_flag == 1)
        {
            if (get_usb_state() == APPLICATION_READY)
            {
                set_label_proc(3);
            }
            else if (get_usb_state() == APPLICATION_IDLE
                     || get_usb_state() == APPLICATION_DISCONNECT)
            {
                set_label_proc(1);
            }
            else
            {
                set_label_proc(2);
            }
        }
        if (*get_usb_wr() == 1)
        {
            set_label_proc(4);
            getConfig()->export_flag = 0;
        }

    }
    else if (page_Id == Main_PAGE)
    {

        if (getConfig()->update_T == 1) //figure ctrl
        {
            static uint16_t fig_count = 0;
            getConfig()->update_T = 0;
            // sprintf(str, "%f", get_temperature()->average_T);
            //SetLableValue(page_Id, TEMPERATURE_ID, str);
            if (get_rf_status()->average_T < 0)
            {
                SetWaveformValue(page_Id, WAVE_ID, 1,
                                 get_rf_status()->average_T * 8 + 40); //       -5---0 0---40

            }
            else
            {
                SetWaveformValue(page_Id, WAVE_ID, 1,
                                 get_rf_status()->average_T * 20 / 3 + 40);
//          SetWaveformValue(page_Id, WAVE_ID, 1,
//                       get_rf_status()->average_T * 40 / 3 + 40);



            }
            fig_count++;
            if (fig_count >= WAVE_HEIGHT)
            {
                fig_count = 0;
                //   WaveformDataClear(page_Id, WAVE_ID);

            }


        }
        static uint32_t last_mode = 2;
        static uint32_t last_v_value = 0;
        static uint32_t last_t = 0;
        if (getConfig()->mode  == 0 && last_mode != getConfig()->mode) //cooller heater
        {

            last_mode = getConfig()->mode;
            Display_Image(110, 167, COOLLER_ID);
        }
        else if (getConfig()->mode  == 1 && last_mode != getConfig()->mode)
            Display_Image(110, 167, HEATER_ID);

        if (get_temperature()->T_value[1] <= 1
                && get_temperature()->T_value[1] != last_v_value)
        {
            unsigned char tmp;
            float tmp2;
            last_v_value = get_temperature()->T_value[1];
            tmp = get_temperature()->T_value[1] * 100;
            tmp2 = tmp;
            memset(str, 0, 65);
            float2char(tmp2, str, 4);

            str[3] = '%';
            SetLableValue(page_Id, BATTERY_ID, str);
        }

        if (get_rf_status()->average_T != last_t)
        {

            float tmp2;
            tmp2 = get_rf_status()->average_T;
            last_t = tmp2;

            float2char(tmp2, str, 4);  //�����������洢���ַ����飬�ַ�����ĳ���
            // sprintf(str, "%f", tmp2);
            SetLableValue(page_Id, TEMPERATURE_ID, str);
        }

    }

}

/*!
 *  \brief  ҳ���л�֪ͨ
 *  \details  ��ǰ����ı�ʱ��ִ�д˺���
 *  \param status �л�ҳ���Ƿ�ɹ�
 */
void NotifySetPage(uint8_t status)
{
    //TODO: ����û�����
    if (status == SUCCESS)
        page_Id = page_Id_bk;

}

/*!
 *  \brief  ��ȡ��ǰҳ��Id��֪ͨ
 *  \details
 *  \param page_id ��ǰҳ��Id��
 */
void NotifyGetPage(uint8_t page_id, uint8_t status)
{
    //TODO: ����û�����
    if (status == SUCCESS)
        page_Id = page_id;
}

/*!
 *  \brief  �����ؼ�֪ͨ
 *  \details  ����������״̬�ı�ʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0x55����0x44����
 *  \param type ����������
 *  \param value ����ֵ
 */
void NotifyTouchButton(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: ����û�����

    if (page_id == Setting_PAGE)
    {
        if (type == UPLOAD_CONTROL_ID && state == KEY_RELEASE)
        {
            //page_Id = value;
            update_en = 1;
            getConfig()->export_flag = 1;//usb
            if (*get_usb_wr() == 1)
                *get_usb_wr() = 2;
        }

    }

    if (type == CHANGE_PAGE && state == KEY_RELEASE)
    {
        page_Id = value;
        update_en = 1;
        //UpdateUI();
    }
    else if (type == ENTER && state == KEY_RELEASE)
    {
        targe_Id = value;//Ŀ��༭��Edit Id
        GetEditValue(page_Id, targe_Id);
    }
    else if (type == CHAR)
    {

    }
    else if (type == UPLOAD_CONTROL_ID)
    {
    }
    else if (type == CLEAR)
    {
    }

}


/*!
 *  \brief  ��������ѡ��ؼ�״̬֪ͨ
 *  \details  ����ѡ��ؼ�״̬�ı�ʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0X53��ѡ�У�0x55û��ѡ��
 */
void NotifyTouchCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,
                         uint8_t type, uint8_t value)
{
    //TODO: ����û�����
    if (state == SELECT)
        update_en = 1;
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  ��ȡ��ѡ��ؼ�״̬֪ͨ
 *  \details  ����ʾ�����ظ�ѡ��״̬ʱ��ִ�д˺���
 *  \param page_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ѡ��״̬��0X53��ѡ�У�0x55û��ѡ��
 */
void NotifyGetCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: ����û�����
    if (state == SELECT)
    {
        update_en = 1;
    }
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  ����ؼ�֪ͨ
 *  \details  ���ڴ������ϻ������ı�ʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyTouchSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: ����û�����

    if (update_en != 1)
        SetNumberValue(page_Id, 28, (uint16_t)value);
    //UpdateUI();
    //update_en = 1;
}


/*!
 *  \brief  �����༭��ؼ�֪ͨ
 *  \details  ���ڴ������ϻ������ı�ʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyTouchEdit(uint8_t page_id, uint8_t control_id, uint8_t  state,
                     uint8_t type, uint8_t value)
{
    //TODO: ����û�����

    if (update_en != 1)
        //SetNumberValue(page_Id,28,(uint16_t)value);
        GetTouchEditValue(page_Id, control_id);

}


/*!
 *  \brief  ����ؼ�֪ͨ
 *  \details  ��ȡ����ؼ������ݣ����ͻ�ȡ�����ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyGetSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,
                     uint8_t type, uint8_t value)
{
    //TODO: ����û�����
    if (state == SUCCESS)
    {
        //�ɹ���ȡ����
    }

    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  ��ȡ�༭���ı�����֪ͨ
 *  \details  ������ʾ�����ͻ�ȡָ��󣬷������ݵ��ô˺���
*  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void NotifyGetEdit(PEDIT_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//ָ������
    uint8_t control_id  = msg->control_id;//�ؼ�ID
    uint8_t page_id     = msg->page_id;
    uint8_t status        = msg->status;
    /*msg->param[1]*/

    //��������ֵΪ1 2 3 4 ��Ӧ��ASCIIֵ��0x31 0x32 0x33 0x34

    if (page_id == Setting_PAGE)
    {
        if (status == 0x6f)
        {
            unsigned char pb[32];
            strcpy((char *)pb, (const char *)msg->param);
            switch (control_id)
            {
                case UP_T_ID:
                    getConfig()->max_T = atof(pb);
                    break;
                case DOW_T_ID:
                    getConfig()->min_T = atof(pb);
                    break;
                case WAR_T_ID:
                    getConfig()->warn_T = atof(pb);
                    break;
                case REC_T_ID:
                    getConfig()->record_time = atof(pb);
                    break;
                case POW_T_ID:
                    getConfig()->power_save = atof(pb);
                    break;
            }

        }
    }

    if (msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33
            && msg->param[3] == 0x34)
    {
        ;//Display_Message(0X18,2,(unsigned char *)String01);
    }
    else
    {
        ;//Display_Message(0X18,2,(unsigned char *)String02);
    }

}

/*!
 *  \brief  ��ȡ�����༭���ı�����֪ͨ
 *  \details  ������ʾ�����ͻ�ȡָ��󣬷������ݵ��ô˺���
*  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//ָ������
    uint8_t control_id  = msg->control_id;//�ؼ�ID
    uint8_t page_id     = msg->page_id;
    uint8_t status        = msg->status;
    /*msg->param[1]*/

    if (page_id == Setting_PAGE)
    {
        if (status == 0x6f)
        {
            unsigned char pb[128];
            unsigned char tmp[128];
            float tt;
            uint32_t t2;
            memset(pb, 0, 128);
            memcpy((char *)pb, (const char *)msg->param, size - 3);

            switch (control_id)
            {
                case UP_T_ID:
                    string_to_float((char *)pb, &getConfig()->max_T, &t2);
                    // = tmp;
                    break;
                case DOW_T_ID:
                    string_to_float((char *)pb, &getConfig()->min_T, &t2);
                    //getConfig()->min_T = atof(pb);
                    break;
                case WAR_T_ID:
                    string_to_float((char *)pb, &getConfig()->warn_T, &t2);
                    //getConfig()->warn_T = atof(pb);
                    break;
                case REC_T_ID:
                    string_to_float((char *)pb, &tt, &getConfig()->record_time);
                    //getConfig()->record_time = atof(pb);
                    break;
                case POW_T_ID:
                    string_to_float((char *)pb, &tt, &getConfig()->power_save);
                    //getConfig()->power_save = atof(pb);
                    break;
            }

        }

    }

    if (msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33
            && msg->param[3] == 0x34)
    {
        ;//Display_Message(0X18,2,(unsigned char *)String01);
    }
    else
    {
        ;//Display_Message(0X18,2,(unsigned char *)String02);
    }


}

/*!
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı��ؼ����º�ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status �ı��ؼ������Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyLable(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  ���ֿؼ�֪ͨ
 *  \details  �����ֿؼ����º�ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ���ֿؼ������Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyNumber(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}


/*!
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����SetProgressbarValueʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyProgressbar(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: ����û�����
}


/*!
 *  \brief  ָ���Ǳ�ؼ�֪ͨ
 *  \details  ����SetCircleGaugeValueʱ��ִ�д˺���
 *  \param page_id ����ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyCircleGauge(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: ����û�����
}

/*!
 *  \brief  ��߿ؼ�֪ͨ
 *  \details ����SetBarGaugeValueʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyBarGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: ����û�����
}

/*!
 *  \brief  �¶ȼƿؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyThermometer(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: ����û�����
}

/*!
 *  \brief  ˮλ�ƿؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyWaterGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: ����û�����
}

/*!
 *  \brief  ��ؿؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyBattery(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: ����û�����
}


/*!
 *  \brief �������߲��οؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifySetWaveformValue(uint8_t page_id, uint8_t control_id,
                            uint8_t  status)
{
    //TODO: ����û�����
}


/*!
 *  \brief �������߲��οؼ�����֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyWaveformDataInsert(uint8_t page_id, uint8_t control_id,
                              uint8_t  status)
{
    //TODO: ����û�����
}
void lcd_init()
{
    queue_reset();

    /*��ʾ��ҳ��*/
    SetPage(Main_PAGE);//��ҳ��Id����4
    page_Id_bk = Main_PAGE;
    page_Id = Main_PAGE;
    SetBackLight(50);

}
void lcd_proc()
{
    qsize  size = 0;


    // while (1)
    {
        static uint32_t timeout;
        size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��
        if (size > 0) //���յ�ָ��
        {
            timeout = HAL_GetTick();
            ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
        }
        else
        {
            static uint32_t timeout;
            if ((HAL_GetTick() - timeout) >= 15000 && getConfig()->export_flag == 0)
            {
                if (page_Id != Main_PAGE)
                {
                    // SetPage(Main_PAGE);//��ҳ��Id����4
                    //  page_Id_bk = Main_PAGE;

                }

                timeout = HAL_GetTick();
                if (getConfig()->update_params == 2)
                    getConfig()->update_params = 1;//update setting
            }
        }

        /****************************************************************************************************************
        �ر�ע��
        MCU��ҪƵ���򴮿����������ݣ����򴮿������ڲ��������������Ӷ��������ݶ�ʧ(��������С����׼��8K��������4.7K)
        1) һ������£�����MCU�򴮿����������ݵ����ڴ���100ms���Ϳ��Ա������ݶ�ʧ�����⣻
        ******************************************************************************************************************/

        //TODO: ����û�����
        //�����и���ʱ��ÿ100����ˢ��һ��
        static uint32_t timer_tick_count;
        if ((HAL_GetTick() - timer_tick_count) >= TIME_100MS)
        {
            //update_en &&
            update_en = 0;
            timer_tick_count = HAL_GetTick();
            UpdateUI();
        }
    }

}
