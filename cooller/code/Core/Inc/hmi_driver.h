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
#define TIME_100MS 100
#include "main.h"



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
void Display_Image(uint16_t x,uint16_t y,uint8_t image_id);

/*! 
 *  \brief     ͼƬ��ͼ��ʾ
 *  \param  image_x ��ͼx����
 *  \param  image_y ��ͼY����
 *  \param  image_w ��ͼ���
 *  \param  image_h ��ͼ�߶�
 *  \param  image_id ͼƬ���
 */
void DisplayCut_Image(uint16_t image_x,uint16_t image_y,uint16_t image_w, uint16_t image_h,uint8_t  image_id);


/*! 
 *  \brief  �������
 */
void GUI_CleanScreen(uint16_t color);

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
void Display_String(uint16_t x, uint16_t y,uint8_t DisplayType,uint8_t FontSize,uint8_t ImageNo,uint16_t BackColor,uint16_t ForeColor, unsigned char *strings );


/*! 
 *  \brief    ��Ϣ��ʾ
 *  \param  FontSize  �ַ���С����
 *  \param  time   ��ʾ�ӳ�ʱ�䣬����Ϊ��λ
 *  \param  strings �ַ�������
 */
void Display_Message(uint8_t FontSize,uint8_t time,unsigned char *strings );

/*! 
 *  \brief      ����
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ֱ����ɫ
 */
void GUI_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t ForeColor);


/*! 
 *  \brief      �����ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ������ɫ
 */
void GUI_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 ,uint16_t ForeColor);

/*! 
 *  \brief      ��ʵ�ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 *  \param ForeColor ������ɫ
 */
void GUI_RectangleFill(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 ,uint16_t ForeColor);



/*! 
 *  \brief      ���ñ���
 *  \param light_level ��������ֵ
 */
void SetBackLight(uint8_t light_level);

/*! 
 *  \brief   ����������
 *  \param enable ����ʹ��
 */
void SetTouchPaneOption(uint8_t enbale);

/*! 
 *  \brief   У׼������
 */
void	CalibrateTouchPane(void);

/*! 
 *  \brief  ����������
 *  \param enable ����ʹ��
 */
void TestTouchPane(uint8_t enbale);




/*! 
 *  \brief      ���õ�ǰ����
 *  \param  page_id ����ID
 */
void SetPage(uint8_t page_id);

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
void SetLableValue(uint8_t page_id,uint8_t control_id,unsigned char *str);
/*! 
 *  \brief     �������ֿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  number����ֵ
 */
void SetNumberValue(uint8_t page_id,uint8_t control_id,uint16_t number);

/*! 
 *  \brief     ���ñ༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  str �ı�ֵ
 */
void SetEditValue(uint8_t page_id,uint8_t control_id,unsigned char *str);

/*! 
 *  \brief     ��ȡ�༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetEditValue(uint8_t page_id,uint8_t control_id);

/*! 
 *  \brief     ��ȡ�����༭��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetTouchEditValue(uint8_t page_id,uint8_t control_id);
/*! 
 *  \brief      ���ý���ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetProgressbarValue(uint8_t page_id,uint8_t control_id,uint8_t value);

/*! 
 *  \brief      ���ø�ѡ��ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetCheckboxValue(uint8_t page_id,uint8_t control_id,uint8_t value);

/*! 
 *  \brief      ��ȡ��ѡ��ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetCheckboxValue(uint8_t page_id,uint8_t control_id);

/*! 
 *  \brief     ����ָ��ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetCircleGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     ���ñ�߿ؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetBarGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     ����ˮλ�ƿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetWaterGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     �����¶ȼƿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetThermometerValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     ���õ�ؿؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetBatteryValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     �������߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 *  \param  value ��ֵ
 */
void SetWaveformValue(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value);
/*! 
 *  \brief     ������߲��οؼ�
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void WaveformDataClear(uint8_t page_id,uint8_t control_id);
/*! 
 *  \brief     �������߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 *  \param  value ��ֵ
 */
void WaveformDataInsert(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value);
/*! 
 *  \brief     ˢ�����߲��οؼ�ֵ
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  channelNo ����ͨ����
 */
void WaveformDataRefress(uint8_t page_id,uint8_t control_id,uint8_t channelNo);

/*! 
 *  \brief      ���û�����
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetSliderValue(uint8_t page_id,uint8_t control_id,uint8_t value);
/*! 
 *  \brief      ��ȡ������λ������
 *  \param  page_id ҳ��ID
 *  \param  control_id �ؼ�ID
 */
void GetSliderValue(uint8_t page_id,uint8_t control_id);


void SendNU8(uint8_t *pData,uint16_t nDataLen);



#endif

