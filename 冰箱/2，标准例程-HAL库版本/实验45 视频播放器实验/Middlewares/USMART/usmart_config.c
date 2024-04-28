#include "./USMART/usmart.h"
#include "./USMART/usmart_str.h"

/******************************************************************************************/
/* �û�������
 * ������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����)
 */
 
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./FATFS/exfuns/fattester.h"
#include "./BSP/ES8388/es8388.h"


/* �������б��ʼ��(�û��Լ����)
 * �û�ֱ������������Ҫִ�еĺ�����������Ҵ�
 */
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS == 1      /* ���ʹ���˶�д���� */
    (void *)read_addr, "uint32_t read_addr(uint32_t addr)",
    (void *)write_addr, "void write_addr(uint32_t addr, uint32_t val)",
#endif
    (void *)delay_ms, "void delay_ms(uint16_t nms)",
    (void *)delay_us, "void delay_us(uint32_t nus)",

    (void*)mf_scan_files,"u8 mf_scan_files(u8 * path)",

    (void *)es8388_write_reg, "uint8_t es8388_write_reg(uint8_t reg, uint8_t val)",
    (void *)es8388_read_reg, "uint8_t es8388_read_reg(uint8_t reg)",
    (void *)es8388_i2s_cfg, "void es8388_i2s_cfg(uint8_t fmt, uint8_t len)",
    (void *)es8388_hpvol_set, "void es8388_hpvol_set(uint8_t volume)",
    (void *)es8388_spkvol_set, "void es8388_spkvol_set(uint8_t volume)",
    (void *)es8388_3d_set, "void es8388_3d_set(uint8_t depth)",
    (void *)es8388_adda_cfg, "void es8388_adda_cfg(uint8_t dacen, uint8_t adcen)",
    (void *)es8388_output_cfg, "void es8388_output_cfg(uint8_t o1en, uint8_t o2en)",
    (void *)es8388_mic_gain, "void es8388_mic_gain(uint8_t gain)",
    (void *)es8388_alc_ctrl, "void es8388_alc_ctrl(uint8_t sel, uint8_t maxgain, uint8_t mingain)",
    (void *)es8388_input_cfg, "void es8388_input_cfg(uint8_t in)",
};


/******************************************************************************************/

/* �������ƹ�������ʼ��
 * �õ������ܿغ���������
 * �õ�����������
 */
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), /* �������� */
    0,      /* �������� */
    0,      /* ����ID */
    1,      /* ������ʾ����,0,10����;1,16���� */
    0,      /* ��������.bitx:,0,����;1,�ַ��� */
    0,      /* ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ�� */
    0,      /* �����Ĳ���,��ҪPARM_LEN��0��ʼ�� */
};



















