#include "ads1118.h"

extern SPI_HandleTypeDef hspi1;
adc_stru adc_usr;

// 配置值（16位补码格式）
uint8_t config_data[3] =
{
    ADS_CONFIG_REG,     // 寄存器地址
    ADS1118_SS_ONCE << 8 |
                    ADS1118_MUX_1G  << 7 |
                    ADS1118_PGA_20  << 6 |
                    ADS1118_MODE_LX << 5,

                    ADS1118_DR_128       << 8 |
                    ADS1118_TS_MODE_ADC  << 7 |
                    ADS1118_PULL_UP_EN_N << 6 |
                    ADS1118_NOP_W        << 5
                    ,               // 单次转换+增益1+128SPS

};

// 初始化ADS1118
void ADS1118_Init()
{
    ADS1118_CS_RELEASE;
    HAL_Delay(100);  // 复位时间

    HAL_SPI_Transmit(&hspi1, config_data, 3, 1000);
    ADS1118_CS_RELEASE;
}

// 读取转换结果
int16_t ADS1118_Read()
{
    uint8_t rx_data[2] = {0};
    uint8_t regAddr;
    regAddr = ADS_CONVERSION_REG;
    ADS1118_CS_RELEASE;
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&regAddr, 1, 1000);
    HAL_SPI_Receive(&hspi1, rx_data, 2, 1000);
    ADS1118_CS_RELEASE;

    return (rx_data[0] << 8) | rx_data[1];
}

// 电压转换函数
float ADS1118_GetVoltage(int16_t adc_value)
{
    float voltage = (adc_value * 1.25f) / 32768.0f;  // 内部参考电压1.25V
    return voltage * 1000;  // 转换为mV
}
// 多通道配置表（差分输入）
//uint8_t channel_config[4] = {0x83, 0xA3, 0xC3, 0xE3}; // AIN0-1, AIN2-3等
//
//void ADS1118_MultiChannel() {
//    for(uint8_t i=0; i<4; i++) {
//        HAL_SPI_Transmit(&hspi1, channel_config, 1, 1000);
//        HAL_Delay(10);
//        adc_val[i] = ADS1118_Read();
//    }
//}

// 主循环
void adcProc(void)
{

        adc_usr.adcdat = ADS1118_Read();
        adc_usr.voltage = ADS1118_GetVoltage(adc_usr.adcdat);
}




