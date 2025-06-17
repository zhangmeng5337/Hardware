#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "hw_config.h"
#include "ulitity.h"
#include <math.h>

#define TIME_100MS 10
#define PI 3.14

volatile uint32  timer_tick_count = 0; //��ʱ������

uint8 cmd_buffer[CMD_MAX_SIZE];

static uint8 update_en = 0;

static uint8 page_Id_bk = 0;
static uint8 page_Id = 0;
static uint8 targe_Id = 0;

const unsigned char String01[]="������ȷ!";
const unsigned char String02[]="�������!";
const unsigned char String03[]="�쳣";

const unsigned char String04[]="������ȷ!";
const unsigned char String05[]="���ݴ���!";

void UpdateUI(void);

//�������
int main()
{
	qsize  size = 0;
	uint32 timer_tick_last_update = 0;

	/*����ʱ��*/
	Set_System();
	/*���ô����ж�*/
	Interrupts_Config();
	/*����ʱ�ӽ���*/
	systicket_init();

	/*���ڳ�ʼ��,����������Ϊ9600*/
	UartInit(9600);//9600

	/*��մ��ڽ��ջ�����*/
	queue_reset();

	/*��ʱ�ȴ���������ʼ�����,����ȴ�500ms*/
	delay_ms(500);

	/*��ʾ��ҳ��*/
	SetPage(Main_PAGE);//��ҳ��Id����4
	page_Id_bk = Main_PAGE;

	SetBackLight(50);
	//Display_String(64,64,0,0x30,17,0x00f8,0xffff,(unsigned char *)String01);

	while(1)
	{
		size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��        
		if(size>0)//���յ�ָ��
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
		}		

		/****************************************************************************************************************
		�ر�ע��
		MCU��ҪƵ���򴮿����������ݣ����򴮿������ڲ��������������Ӷ��������ݶ�ʧ(��������С����׼��8K��������4.7K)
		1) һ������£�����MCU�򴮿����������ݵ����ڴ���100ms���Ϳ��Ա������ݶ�ʧ�����⣻
		******************************************************************************************************************/

		//TODO: ����û�����
		//�����и���ʱ��ÿ100����ˢ��һ��
		if(update_en&&timer_tick_count-timer_tick_last_update>=TIME_100MS)
		{
			update_en = 0;
			timer_tick_last_update = timer_tick_count;			

			UpdateUI();
		}
	}
}


/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
	uint8 cmd_type  = msg->cmd_type;//ָ������
	uint8 control_id  = msg->control_id;//�ؼ�Id
	uint8 page_id     = msg->page_id;   //ҳ��Id
	uint8 status        = msg->status;
	uint8 key_type   = msg->key_type;
	uint8 key_value  = msg->key_value;
	

	switch(cmd_type)
	{
	case NOTIFY_SET_PAGE:
		NotifySetPage(control_id);
		break;
		
	case NOTIFY_TOUCH_BUTTON:
		NotifyTouchButton(page_id,control_id,status,key_type,key_value);
		break;

	case NOTIFY_TOUCH_CHECKBOX:
		NotifyTouchCheckbox(page_id,control_id,status,key_type,key_value);
		break;

	case NOTIFY_TOUCH_SLIDER:
		NotifyTouchSlider(page_id,control_id,status,key_type,key_value);
		break;
	
	case NOTIFY_TOUCH_EDIT:
		NotifyTouchEdit(page_id,control_id,status,key_type,key_value);
		break;
	
	case NOTIFY_GET_EDIT:
		NotifyGetEdit((PEDIT_MSG)cmd_buffer,size-4);//ȥ��֡ͷ֡β
		break;
	
	case NOTIFY_GET_TOUCH_EDIT:
		NotifyGetTouchEdit((PEDIT_MSG)cmd_buffer,size-4);//ȥ��֡ͷ֡β
		break;

	case NOTIFY_GET_PAGE:
		NotifyGetPage(page_id,status);
		break;

	case NOTIFY_GET_CHECKBOX:
		NotifyGetCheckbox(page_id,control_id,status,key_type,key_value);
		break;

	case NOTIFY_GET_SLIDER:
		NotifyGetSlider(page_id,control_id,status,key_type,key_value);
		break;
		
	default:
		break;
	}
}



void UpdateUI()
{
	int i;
	int value;
	
	if(page_Id == Text_PAGE)
	{
		SetNumberValue(page_Id,19,20);
		SetNumberValue(page_Id,20,21);
		SetNumberValue(page_Id,21,28);
		SetNumberValue(page_Id,22,35);
		SetNumberValue(page_Id,23,46);
		SetNumberValue(page_Id,24,58);
		SetLableValue(page_Id,16,(unsigned char *)String03);
		SetLableValue(page_Id,17,(unsigned char *)String03);
		SetLableValue(page_Id,18,(unsigned char *)String03);
		
	}
	else if(page_Id == Waveform_PAGE)
	{
		for(i=-125;i<125;i+=1)
		{
			value = 20*sin(PI/50*i);
			SetWaveformValue(page_Id,9,2,value+119);
			SetWaveformValue(page_Id,9,1,value+59);
		}
		//WaveformDataRefress(page_Id,9,3);
	}
	else if(page_Id == Temperture_PAGE)
	{
		value =20;
		for(i=0;i<=10;i+=1)
		{
			SetThermometerValue(page_Id,9,value);
			SetNumberValue(page_Id,11,value-20);
			value +=10;
			delay_ms(200);
		}

		value =120;
		for(i=0;i<=10;i+=1)
		{
			SetThermometerValue(page_Id,9,value);
			SetNumberValue(page_Id,11,value-20);
			value -=10;
			delay_ms(200);
		}
	}
	else if(page_Id == Slider_PAGE)
	{
		value =0;
		for(i=0;i<10;i+=1)
		{
			SetProgressbarValue(page_Id,18,value);
			SetProgressbarValue(page_Id,20,value);
			value +=10;
			delay_ms(200);
		}
	}
	else if(page_Id == CircleGauge_PAGE)
	{
		value =0;
		for(i=0;i<=10;i+=1)
		{
			SetCircleGaugeValue(page_Id,37,value);
			SetBarGaugeValue(page_Id,40,value);
			
			value +=10;
			delay_ms(200);
		}

		value =100;
		for(i=0;i<=10;i+=1)
		{
			SetCircleGaugeValue(page_Id,37,value);
			SetBarGaugeValue(page_Id,40,value);
			
			value -=10;
			delay_ms(200);
		}
	}
	else if(page_Id == BarGauge_PAGE)
	{
		value =0;
		for(i=0;i<=10;i+=1)
		{
			SetBatteryValue(page_Id,44,value);
			SetWaterGaugeValue(page_Id,42,value);
			value +=10;
			delay_ms(100);
		}

		value =100;
		for(i=0;i<=10;i+=1)
		{
			SetBatteryValue(page_Id,44,value);
			SetWaterGaugeValue(page_Id,42,value);
			value -=10;
			delay_ms(100);
		}
	}
}

/*! 
 *  \brief  ҳ���л�֪ͨ
 *  \details  ��ǰ����ı�ʱ��ִ�д˺���
 *  \param status �л�ҳ���Ƿ�ɹ�
 */
 void NotifySetPage(uint8 status)
{
	//TODO: ����û�����
	if(status == SUCCESS)
		page_Id = page_Id_bk;
	
}

/*! 
 *  \brief  ��ȡ��ǰҳ��Id��֪ͨ
 *  \details  
 *  \param page_id ��ǰҳ��Id��
 */
void NotifyGetPage(uint8 page_id,uint8 status)
{
	//TODO: ����û�����
	if(status == SUCCESS)
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
void NotifyTouchButton(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����
	
	if(type == CHANGE_PAGE&& state == KEY_RELEASE)
	{
		page_Id = value;
		update_en = 1;
		//UpdateUI();
	}
	else if(type == ENTER&& state == KEY_RELEASE)
	{
		targe_Id = value;//Ŀ��༭��Edit Id
		GetEditValue(page_Id,targe_Id);
	}
	else if(type == CHAR)
	{
	
	}
	else if(type == UPLOAD_CONTROL_ID)
	{
	}
	else if(type == CLEAR)
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
void NotifyTouchCheckbox(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����
	if(state == SELECT)
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
void NotifyGetCheckbox(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����
	if(state == SELECT)
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
void NotifyTouchSlider(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����

	if(update_en != 1)
		SetNumberValue(page_Id,28,(uint16)value);
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
void NotifyTouchEdit(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����

	if(update_en != 1)
		//SetNumberValue(page_Id,28,(uint16)value);
		GetTouchEditValue(page_Id,control_id);
	
}


/*! 
 *  \brief  ����ؼ�֪ͨ
 *  \details  ��ȡ����ؼ������ݣ����ͻ�ȡ�����ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyGetSlider(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value)
{
	//TODO: ����û�����
	if(state == SUCCESS)
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
void NotifyGetEdit(PEDIT_MSG msg, uint16 size)
{
	uint8 cmd_type  = msg->cmd_type;//ָ������
	uint8 control_id  = msg->control_id;//�ؼ�ID
	uint8 page_id     = msg->page_id;
	uint8 status        = msg->status;
	/*msg->param[1]*/

	//��������ֵΪ1 2 3 4 ��Ӧ��ASCIIֵ��0x31 0x32 0x33 0x34

	if(msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33 && msg->param[3] == 0x34)
	{
		Display_Message(0X18,2,(unsigned char *)String01);	
	}
	else
	{
		Display_Message(0X18,2,(unsigned char *)String02);	
	}
	
}

/*! 
 *  \brief  ��ȡ�����༭���ı�����֪ͨ
 *  \details  ������ʾ�����ͻ�ȡָ��󣬷������ݵ��ô˺���
*  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16 size)
{
	uint8 cmd_type  = msg->cmd_type;//ָ������
	uint8 control_id  = msg->control_id;//�ؼ�ID
	uint8 page_id     = msg->page_id;
	uint8 status        = msg->status;
	/*msg->param[1]*/

	//��������ֵΪ1 2 3 4 ��Ӧ��ASCIIֵ��0x31 0x32 0x33 0x34

	if(msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33 && msg->param[3] == 0x34)
	{
		Display_Message(0X18,2,(unsigned char *)String04);	
	}
	else
	{
		Display_Message(0X18,2,(unsigned char *)String05);	
	}
	
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı��ؼ����º�ִ�д˺���
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status �ı��ؼ������Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
 void NotifyLable(uint8 page_id, uint8 control_id,uint8 status)
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
 void NotifyNumber(uint8 page_id, uint8 control_id,uint8 status)
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
void NotifyProgressbar(uint8 page_id, uint8 control_id, uint8 status)
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
void NotifyCircleGauge(uint8 page_id, uint8 control_id, uint8 status)
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
void NotifyBarGauge(uint8 page_id, uint8 control_id, uint8  status)
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
void NotifyThermometer(uint8 page_id, uint8 control_id, uint8  status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ˮλ�ƿؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyWaterGauge(uint8 page_id, uint8 control_id, uint8  status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ؿؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyBattery(uint8 page_id, uint8 control_id, uint8  status)
{
	//TODO: ����û�����
}


/*! 
 *  \brief �������߲��οؼ�֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifySetWaveformValue(uint8 page_id, uint8 control_id, uint8  status)
{
	//TODO: ����û�����
}


/*! 
 *  \brief �������߲��οؼ�����֪ͨ����
 *  \param page_id ҳ��ID
 *  \param control_id �ؼ�ID
 *  \param status ֵ�����Ƿ�ɹ�0x6f�ɹ� 0x65ʧ��
 */
void NotifyWaveformDataInsert(uint8 page_id, uint8 control_id, uint8  status)
{
	//TODO: ����û�����
}



