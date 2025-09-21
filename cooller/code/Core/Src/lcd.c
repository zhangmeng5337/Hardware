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
#include "air.h"

uint8_t cmd_buffer[CMD_MAX_SIZE];
static uint8_t update_en = 0;
static uint8_t page_Id_bk = 0;
static uint8_t page_Id = 0;
static uint8_t targe_Id = 0;
static unsigned char msg_type;
/*!
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage(PCTRL_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//指令类型
    uint8_t control_id  = msg->control_id;//控件Id
    uint8_t page_id     = msg->page_id;   //页面Id
    uint8_t status        = msg->status;
    uint8_t key_type   = msg->key_type;
    uint8_t key_value  = msg->key_value;


    switch (cmd_type)
    {
        case NOTIFY_SET_PAGE:
            NotifySetPage(control_id);
            msg_type = 1;
            break;

        case NOTIFY_TOUCH_BUTTON:
            NotifyTouchButton(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        case NOTIFY_TOUCH_CHECKBOX:
            NotifyTouchCheckbox(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        case NOTIFY_TOUCH_SLIDER:
            NotifyTouchSlider(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        case NOTIFY_TOUCH_EDIT:
            NotifyTouchEdit(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        case NOTIFY_GET_EDIT:
            NotifyGetEdit((PEDIT_MSG)cmd_buffer, size - 4); //去除帧头帧尾
            msg_type = 1;
            break;

        case NOTIFY_GET_TOUCH_EDIT:
            NotifyGetTouchEdit((PEDIT_MSG)cmd_buffer, size - 4); //去除帧头帧尾
            msg_type = 1;
            break;

        case NOTIFY_GET_PAGE:
            NotifyGetPage(page_id, status);
            msg_type = 1;
            break;

        case NOTIFY_GET_CHECKBOX:
            NotifyGetCheckbox(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        case NOTIFY_GET_SLIDER:
            NotifyGetSlider(page_id, control_id, status, key_type, key_value);
            msg_type = 1;
            break;

        default:
            msg_type = 0;
            break;
    }
}


void set_label_proc(unsigned char num)
{
    switch (num)
    {
        case 0:
            SetLableValue(WAVE_PAGE, USB_LOG_ID, "");
			//Display_String(172, 121,1,32,0,0,255, "          " );
            break;
        case 1:
            SetLableValue(WAVE_PAGE, USB_LOG_ID, "请插入U盘");
			//Display_String(172, 121,1,32,0,0,255, "请插入U盘" );
            break;
        case 2:
            SetLableValue(WAVE_PAGE, USB_LOG_ID, "未识别到U盘");
            break;
        case 3:
            SetLableValue(WAVE_PAGE, USB_LOG_ID, "数据导出中...");
			//Display_String(172, 121,1,32,0,0,255, "数据导出中..." );
            break;
        case 4:
            SetLableValue(WAVE_PAGE, USB_LOG_ID, "数据导出完成");
			//Display_String(172, 121,1,32,0,0,255, "数据导出完成" );
            break;
	

    }
}

void UpdateUI()
{
    int i;
    int value;
    char str[65];
    static unsigned char flag;
    if (page_Id == Setting_PAGE )
    {
        if (flag == 0)
        {
            flag = 1;
            sprintf(str, "%.0f", getConfig()->max_T);
            // float2char(getConfig()->max_T, str, 4);
            SetEditValue(Setting_PAGE, UP_T_ID, str);
            //float2char(getConfig()->min_T, str, 4);
            sprintf(str, "%.0f", getConfig()->min_T);
            SetEditValue(Setting_PAGE, DOW_T_ID, str);

            sprintf(str, "%.0f", getConfig()->warn_T);
            //float2char(getConfig()->warn_T, str, 4);
            SetEditValue(Setting_PAGE, WAR_T_ID, str);

            sprintf(str, "%.0f", getConfig()->record_interval);
            //float2char(getConfig()->record_interval, str, 4);
            SetEditValue(Setting_PAGE, REC_T_ID, str);

            sprintf(str, "%.0f", getConfig()->power_save);
            //float2char(getConfig()->power_save, str, 4);
            SetEditValue(Setting_PAGE, POW_T_ID, str);
        }
//        if (getConfig()->export_flag == 1)
//        {
//            if (get_usb_state() == APPLICATION_READY)
//            {
//                set_label_proc(3);
//            }
//            else if (get_usb_state() == APPLICATION_IDLE
//                     || get_usb_state() == APPLICATION_DISCONNECT)
//            {
//                set_label_proc(1);
//            }
//            else
//            {
//                set_label_proc(2);
//            }
//        }
//        else
//            set_label_proc(0);
//        if (*get_usb_wr() == 1)
//        {
//
//            set_label_proc(4);
//            getConfig()->export_flag = 0;
//        }

    }
    else if (page_Id == WAVE_PAGE )
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
        else
            set_label_proc(0);
        if (*get_usb_wr() == 1)
        {

            set_label_proc(4);
            getConfig()->export_flag = 0;
        }




        if (getConfig()->update_fig == 1) //figure ctrl
        {
            static uint16_t fig_count = 0;
            //getConfig()->update_T = 0;
            // sprintf(str, "%f", get_temperature()->average_T);
            //SetLableValue(page_Id, TEMPERATURE_ID, str);
            if (get_temperature()->T_value[0] < 0)
            {
                if(TEMPER_SEL == 1)
                SetWaveformValue(page_Id, WAVE_ID, 1,
                                 get_rf_status()->average_T * 8 + 40); //       -5---0 0---40
                 else
                SetWaveformValue(page_Id, WAVE_ID, 1,
                               get_temperature()->T_value[0] * 8 + 40); //       -5---0 0---40				 	
                fig_count++;
            }
#if DEBUG_EN == 0
            else if (get_temperature()->T_value[0] <= 15)
#else if
            else
#endif
            {
#if DEBUG_EN == 0
                SetWaveformValue(page_Id, WAVE_ID, 1,
                                 get_rf_status()->average_T * 40 / 4 + 40);//0---40  15---240

#else if
				if(TEMPER_SEL == 1)

                SetWaveformValue(page_Id, WAVE_ID, 1,
                                 get_rf_status()->average_T * 40 / 8.6 + 40);//0---40  15---240
                 else
                SetWaveformValue(page_Id, WAVE_ID, 1,
                               get_temperature()->T_value[0] *40/ 8.6 + 40); //       -5---0 0---40	


#endif

                fig_count++;
            }

            if (fig_count >= WAVE_HEIGHT)
            {
                fig_count = 0;
                //   WaveformDataClear(page_Id, WAVE_ID);

            }
		getConfig()->update_fig = 0;


        }
    }

    else if (page_Id == Main_PAGE )
    {

        static uint32_t last_mode = 2;
        static uint32_t last_v_value = 0;
        static uint32_t last_t = 0;
        if (getConfig()->mode  == MODE_COOLLER
           ) //cooller heater&& last_mode != getConfig()->mode&& last_mode != getConfig()->mode
        {

            last_mode = getConfig()->mode;
            Display_Image(MODE_X_POS, MODE_Y_POS, COOLLER_ID);
        }
        else if (getConfig()->mode  == MODE_HEATER)
        {
            Display_Image(MODE_X_POS, MODE_Y_POS, HEATER_ID);
            last_mode = getConfig()->mode;

        }


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

            str[3] = '%';//100%
            SetLableValue(page_Id, BATTERY_ID, str);
        }
        //if (get_rf_status()->average_T != last_t)
       
//         if ( getConfig()->max_T!= last_t)
        if (get_temperature()->T_value[0] != last_t)
        {

            float tmp2;
//			 tmp2 = getConfig()->max_T;
            tmp2 = get_temperature()->T_value[0];
            last_t = tmp2;

            float2char(tmp2, str, 4);  //浮点型数，存储的字符数组，字符数组的长度
            // sprintf(str, "%f", tmp2);
            SetLableValue(page_Id, TEMPERATURE_ID, str);
        }

    }

}

/*!
 *  \brief  页面切换通知
 *  \details  当前画面改变时，执行此函数
 *  \param status 切换页面是否成功
 */
void NotifySetPage(uint8_t status)
{
    //TODO: 添加用户代码
    if (status == SUCCESS)
        page_Id = page_Id_bk;

}

/*!
 *  \brief  获取当前页面Id号通知
 *  \details
 *  \param page_id 当前页面Id号
 */
void NotifyGetPage(uint8_t page_id, uint8_t status)
{
    //TODO: 添加用户代码
    if (status == SUCCESS)
        page_Id = page_id;
}

/*!
 *  \brief  按键控件通知
 *  \details  当触摸按键状态改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0x55弹起，0x44按下
 *  \param type 按键功能码
 *  \param value 按键值
 */
void NotifyTouchButton(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码

    if (page_id == Setting_PAGE)
    {
        if (state == KEY_RELEASE)
        {
            if (type == UPLOAD_CONTROL_ID)
            {
//                if (control_id == USB_ID)
//                {
//                    //page_Id = value;
//                    update_en = 1;
//                    getConfig()->export_flag = 1;//usb
//                    //get_flash_status()->usb_read_flag = 0;
//                    if (*get_usb_wr() == 1)
//                        *get_usb_wr() = 2;
//                }
//                else if (control_id == MODE_CHANGE_ID)
//                {
//                    if (getConfig()->mode == MODE_COOLLER)
//                        getConfig()->mode = MODE_HEATER;
//                    else
//                        getConfig()->mode == MODE_COOLLER;
//                }
//                else
//              if (control_id == RETURN_ID)
//                {
//                    SetPage(Main_PAGE);//主页面Id号是4
//
//                }

            }



        }

    }
    else  if (page_id == WAVE_PAGE)
    {
        if (state == KEY_RELEASE)
        {
            if (type == UPLOAD_CONTROL_ID)
            {
                if (control_id == USB_ID)
                {
                    //page_Id = value;
                    update_en = 1;
                    getConfig()->export_flag = 1;//usb
                    //get_flash_status()->usb_read_flag = 0;
                    if (*get_usb_wr() == 1)
                        *get_usb_wr() = 2;
                }
//                else if (control_id == MODE_CHANGE_ID)
//                {
//                    if (getConfig()->mode == MODE_COOLLER)
//                        getConfig()->mode = MODE_HEATER;
//                    else
//                        getConfig()->mode == MODE_COOLLER;
//                }
//                else if (control_id == RETURN_ID)
//                {
//                    SetPage(Main_PAGE);//主页面Id号是4
//
//                }

            }



        }

    }
    if (page_id == Main_PAGE)
    {
        if (state == KEY_RELEASE)
        {
            if (type == UPLOAD_CONTROL_ID)
            {

                if (control_id == MODE_CHANGE_ID)
                {
                    if (getConfig()->mode == MODE_COOLLER)
                        getConfig()->mode = MODE_HEATER;
                    else
                        getConfig()->mode = MODE_COOLLER;
                }

            }

        }

    }

    if (type == CHANGE_PAGE && state == KEY_RELEASE)
    {
        page_Id = value;
        update_en = 1;
       // UpdateUI();
    }
    else if (type == ENTER && state == KEY_RELEASE)
    {
        targe_Id = value;//目标编辑框Edit Id
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
 *  \brief  触摸屏复选框控件状态通知
 *  \details  当复选框控件状态改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0X53被选中，0x55没有选中
 */
void NotifyTouchCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,
                         uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码
    if (state == SELECT)
        update_en = 1;
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  获取复选框控件状态通知
 *  \details  当显示屏返回复选框状态时，执行此函数
 *  \param page_id 画面ID
 *  \param control_id 控件ID
 *  \param state 复选框状态：0X53被选中，0x55没有选中
 */
void NotifyGetCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码
    if (state == SELECT)
    {
        update_en = 1;
    }
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  滑块控件通知
 *  \details  当在触摸屏上滑动条改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyTouchSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,
                       uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码

    if (update_en != 1)
        SetNumberValue(page_Id, 28, (uint16_t)value);
    //UpdateUI();
    //update_en = 1;
}


/*!
 *  \brief  触摸编辑框控件通知
 *  \details  当在触摸屏上滑动条改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyTouchEdit(uint8_t page_id, uint8_t control_id, uint8_t  state,
                     uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码

    if (update_en != 1)
        //SetNumberValue(page_Id,28,(uint16_t)value);
        GetTouchEditValue(page_Id, control_id);

}


/*!
 *  \brief  滑块控件通知
 *  \details  获取滑块控件的数据，发送获取命令后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyGetSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,
                     uint8_t type, uint8_t value)
{
    //TODO: 添加用户代码
    if (state == SUCCESS)
    {
        //成功获取数据
    }

    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  获取编辑框文本数据通知
 *  \details  当向显示屏发送获取指令后，返回数据调用此函数
*  \param msg 待处理消息
 *  \param size 消息长度
 */
void NotifyGetEdit(PEDIT_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//指令类型
    uint8_t control_id  = msg->control_id;//控件ID
    uint8_t page_id     = msg->page_id;
    uint8_t status        = msg->status;
    /*msg->param[1]*/

    //测试密码值为1 2 3 4 对应的ASCII值是0x31 0x32 0x33 0x34

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
                    //getConfig()->warn_T = atof(pb);
                    break;
                case REC_T_ID:
                    getConfig()->record_interval = atof(pb);
                    break;
                case POW_T_ID:
                    getConfig()->power_save = atof(pb);
                    break;
            }
            air_init();

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
 *  \brief  获取触摸编辑框文本数据通知
 *  \details  当向显示屏发送获取指令后，返回数据调用此函数
*  \param msg 待处理消息
 *  \param size 消息长度
 */
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16_t size)
{
    uint8_t cmd_type  = msg->cmd_type;//指令类型
    uint8_t control_id  = msg->control_id;//控件ID
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
                    string_to_float((char *)pb, &getConfig()->record_interval, &t2);
                    //getConfig()->record_time = atof(pb);
                    break;
                case POW_T_ID:
                    string_to_float((char *)pb, &getConfig()->power_save, &t2);
                    //getConfig()->power_save = atof(pb);
                    break;
            }
            getConfig()->update_params = 2;

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
 *  \brief  文本控件通知
 *  \details  当文本控件更新后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 文本控件更新是否成功0x6f成功 0x65失败
 */
void NotifyLable(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  数字控件通知
 *  \details  当数字控件更新后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 数字控件更新是否成功0x6f成功 0x65失败
 */
void NotifyNumber(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}


/*!
 *  \brief  进度条控件通知
 *  \details  调用SetProgressbarValue时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyProgressbar(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief  指针仪表控件通知
 *  \details  调用SetCircleGaugeValue时，执行此函数
 *  \param page_id 画面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyCircleGauge(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  标尺控件通知
 *  \details 调用SetBarGaugeValue时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyBarGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  温度计控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyThermometer(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  水位计控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyWaterGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  电池控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyBattery(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief 更新曲线波形控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifySetWaveformValue(uint8_t page_id, uint8_t control_id,
                            uint8_t  status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief 输入曲线波形控件数据通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyWaveformDataInsert(uint8_t page_id, uint8_t control_id,
                              uint8_t  status)
{
    //TODO: 添加用户代码
}
void lcd_init()
{
    queue_reset();

    /*显示主页面*/
    SetPage(Main_PAGE);//主页面Id号是4
    page_Id_bk = Main_PAGE;
    page_Id = Main_PAGE;
    SetBackLight(50);

}
void lcd_proc()
{
    qsize  size = 0;


    // while (1)
    {
        static uint32_t timeout, timeout2;
        size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); //从缓冲区中获取一条指令
        if (size > 0) //接收到指令
        {
            timeout = HAL_GetTick();
            ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
            if (getConfig()->update_params == 2)
                getConfig()->update_params = 1;//update setting
            if (msg_type != 0)
            {
                msg_type = 0;
                timeout2 = HAL_GetTick();
            }

        }
        else
        {
            //  static uint32_t timeout2;
            #if DEBUG_EN
            if ((HAL_GetTick() - timeout2) >= SLEEP_TIME && getConfig()->export_flag == 0)
            {

                if (getConfig()->status == SLEEP)
                {               

				   SetPage(Main_PAGE);//主页面Id号是4
                    SetBackLight(0);
                    flash_save();
                    sys_enter_standy_mode();
                    HAL_NVIC_SystemReset();

                }
                timeout2 = HAL_GetTick();
            }
			#endif
        }

        /****************************************************************************************************************
        特别注意
        MCU不要频繁向串口屏发送数据，否则串口屏的内部缓存区会满，从而导致数据丢失(缓冲区大小：标准型8K，基本型4.7K)
        1) 一般情况下，控制MCU向串口屏发送数据的周期大于100ms，就可以避免数据丢失的问题；
        ******************************************************************************************************************/

        //TODO: 添加用户代码
        //数据有更新时，每100毫秒刷新一次
        static uint32_t timer_tick_count;
        if ((HAL_GetTick() - timer_tick_count) >= TIME_100MS)
        {
            //update_en &&
            update_en = 0;
            timer_tick_count = HAL_GetTick();
            UpdateUI();

        }
       // else
         //   UpdateUI(WAVE_PAGE);
    }

}
