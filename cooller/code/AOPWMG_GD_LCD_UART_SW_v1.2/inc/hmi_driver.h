/*! 
 *  \file hmi_driver.h
 *  \brief �����������ļ�
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#define CMD_MAX_SIZE 65        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 128   /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

#include "hmi_user_uart.h"



/*! 
 *  \brief  ��ʱ
 *  \param  n ��ʱʱ��(���뵥λ)
 */
void DelayMS(unsigned int n);

/*! 
 *  \brief  ������������
 */
void SetConnect(void);
/*! 
 *  \brief  ������ʾ����λ����
 */
void SetReset(void);

/*! 
 *  \brief     ͼƬ��ȫ��ʾ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  image_id ͼƬ���
 */
void Display_Image(uint16 x,uint16 y,uint8 image_id);

/*! 
 *  \brief     ͼƬ��ͼ��ʾ
 *  \param  image_x ��ͼx����
 *  \param  image_y ��ͼY����
 *  \param  image_w ��ͼ���
 *  \param  image_h ��ͼ�߶�
 *  \param  image_id ͼƬ���
 */
void DisplayCut_Image(uint16 image_x,uint16 image_y,uint16 image_w, uint16 image_h,uint8  image_id);


/*! 
 *  \brief  �������
 */
void GUI_CleanScreen(uint16 color);

/*! 
 *  \brief  ���ù���ɫ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  DisplayType
 *  \param  FontSize ����
 *  \param  ImageNo ����ͼƬ���
 *  \param  BackColor ���屳��ɫ
 *  \param  ForeColor ������ɫ
 *  \param  strings �ַ�������
 */
void Display_String(uint16 x, uint16 y,uint8 DisplayType,uint8 FontSize,uint8 ImageNo,uint16 BackColor,uint16 ForeColor, uchar *strings );


/*! 
 *  \brief    ��Ϣ��ʾ
 *  \param  FontSize  �ַ���С����
 *  \param  time   ��ʾ�ӳ�ʱ�䣬����Ϊ��λ
 *  \param  strings �ַ�������
 */
void Display_Message(uint8 FontSize,uint8 time,uchar *strings );

/*! 
 *  \brief      ����
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ֱ����ɫ
 */
void GUI_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1,uint16 ForeColor);


/*! 
 *  \brief      �����ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ������ɫ
 */
void GUI_Rectangle(uint16 x0, uint16 y0, uint16 x1,uint16 y1 ,uint16 ForeColor);

/*! 
 *  \brief      ��ʵ�ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ������ɫ
 */
void GUI_RectangleFill(uint16 x0, uint16 y0, uint16 x1,uint16 y1 ,uint16 ForeColor);



/*! 
 *  \brief      ���ñ���
 *  \param light_level ��������ֵ
 */
void SetBackLight(uint8 light_level);

/*! 
 *  \brief   ����������
 *  \param enable ����ʹ��
 */
void SetTouchPaneOption(uint8 enbale);

/*! 
 *  \brief   У׼������
 */
void	CalibrateTouchPane(void);

/*! 
 *  \brief  ����������
 *  \param enable ����ʹ��
 */
void TestTouchPane(uint8 enbale);




/*! 
 *  \brief      ���õ�ǰ����
 *  \param  page_id ����ID
 */
void SetPage(uint8 page_id);

/*! 
 *  \brief      ��ȡ��ǰ����
 */
void GetPage(void);


/*! 
 *  \brief     �����ı�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  str �ı�ֵ
 */
void SetLableValue(uint8 page_id,uint8 control_id,uchar *str);
/*! 
 *  \brief     �������ֿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  number����ֵ
 */
void SetNumberValue(uint8 page_id,uint8 control_id,uint16 number);

/*! 
 *  \brief     ���ñ༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  str �ı�ֵ
 */
void SetEditValue(uint8 page_id,uint8 control_id,uchar *str);

/*! 
 *  \brief     ��ȡ�༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetEditValue(uint8 page_id,uint8 control_id);

/*! 
 *  \brief     ��ȡ�����༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetTouchEditValue(uint8 page_id,uint8 control_id);
/*! 
 *  \brief      ���ý���ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetProgressbarValue(uint8 page_id,uint8 control_id,uint8 value);

/*! 
 *  \brief      ���ø�ѡ��ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetCheckboxValue(uint8 page_id,uint8 control_id,uint8 value);

/*! 
 *  \brief      ��ȡ��ѡ��ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetCheckboxValue(uint8 page_id,uint8 control_id);

/*! 
 *  \brief     ����ָ��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetCircleGaugeValue(uint8 page_id,uint8 control_id,uint16 value);
/*! 
 *  \brief     ���ñ�߿ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetBarGaugeValue(uint8 page_id,uint8 control_id,uint16 value);
/*! 
 *  \brief     ����ˮλ�ƿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetWaterGaugeValue(uint8 page_id,uint8 control_id,uint16 value);
/*! 
 *  \brief     �����¶ȼƿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetThermometerValue(uint8 page_id,uint8 control_id,uint16 value);
/*! 
 *  \brief     ���õ�ؿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetBatteryValue(uint8 page_id,uint8 control_id,uint16 value);
/*! 
 *  \brief     �������߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 *  \param  value ��ֵ
 */
void SetWaveformValue(uint8 page_id,uint8 control_id,uint8 channelNo,uint8 value);
/*! 
 *  \brief     ������߲��οؼ�
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void WaveformDataClear(uint8 page_id,uint8 control_id);
/*! 
 *  \brief     �������߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 *  \param  value ��ֵ
 */
void WaveformDataInsert(uint8 page_id,uint8 control_id,uint8 channelNo,uint8 value);
/*! 
 *  \brief     ˢ�����߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 */
void WaveformDataRefress(uint8 page_id,uint8 control_id,uint8 channelNo);

/*! 
 *  \brief      ���û�����
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetSliderValue(uint8 page_id,uint8 control_id,uint8 value);
/*! 
 *  \brief      ��ȡ������λ������
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetSliderValue(uint8 page_id,uint8 control_id);


void SendNU8(uint8 *pData,uint16 nDataLen);
void SetLableValueColor(uint8 page_id,uint8 control_id,uint16 BackColor,uint16 ForeColor,uchar *str);
void SetEditValueColor(uint8 page_id,uint8 control_id,uint16 BackColor,uint16 ForeColor,uchar *str);



#endif

