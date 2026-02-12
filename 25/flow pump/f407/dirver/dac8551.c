#ifndef _DAC8551_H_
#define _DAC8551_H_
#include "Common.h"
#include "spi.h"
 
 
struct Dac8551Command
{
    inline SpiCmd operator()(uint op)
    {
        return {};
    }
 
    inline SpiCmd operator()(const SpiOp & op)
    {
        return {};
    }
 
    inline SpiCmd operator()(uint op, uint reg_addr)
    {
        return {};
    }
};
 
 
enum 
{
    PD_NORMAL = 0b00,     // 正常工作模式
    PD_1K     = 0b01,     // 掉电模式1 (1kΩ接地)
    PD_100K   = 0b10,     // 掉电模式2 (100kΩ接地)
    PD_HIGH_Z = 0b11      // 掉电模式3 (高阻态)
};
 
 
class CDac8551
{
    private:
        uint8_t  mode;
        uint16_t level;
        uint16_t last_update;
        uint16_t min_gap;
        uint16_t retry;
        CSpiProtocol<DAC8551_BPS_K, Dac8551Command, true, 8, SPI_MSB_FIRST, SPI_CPHA_1, SPI_CPOL_0> spi;
 
 
    public: 
        inline CDac8551(uint cs_pin, uint tx_pin, uint clk_pin) :
        mode(PD_NORMAL),level(0),last_update(0),min_gap(0),retry(0),spi(cs_pin, UINT32_MAX, tx_pin, clk_pin)
        {
        }
 
        inline virtual ~CDac8551() {
        }
 
 
        inline bool Start(uint16_t gap)
        {
            min_gap = gap;
            return spi.Initiate();
        }
 
        inline void SetPowerDropMode(uint8_t value) 
        {
            mode = value & 0x03;
 
            uint8_t data[3];
            data[0] = mode & 0x03;
            data[1] = static_cast<uint8_t>( level >> 8 );
            data[2] = static_cast<uint8_t>( level & 0xFF );
            spi.WriteData( data, 3 );
            last_update = level;
            retry = 0;
        }
        
        inline uint8_t GetPowerDropMode() {
            return mode;
        }
 
        inline void SetLevel(uint16_t value) 
        {
            uint16_t gap = last_update > value ? last_update - value : value - last_update;
            level = value;
 
            bool send = gap >= min_gap;
            if( !send )
            {
                // 如果超过10次反复请求修改，则强制设置
                if( ++ retry > 10 ) send = true;
            }
 
            if( send )
            {
                uint8_t data[3];
                data[0] = mode & 0x03;
                data[1] = static_cast<uint8_t>( level >> 8 );
                data[2] = static_cast<uint8_t>( level );
                spi.WriteData( data, 3 );
                last_update = level;
                retry = 0;
 
                //Serial.printf( "%d, data: 0x%02X, 0x%02X, 0x%02X\n", value, data[0], data[1], data[2] );
            }
        }
        
        inline uint16_t GetLevel() {
            return level;
        }
 
        inline void Adjust(double percent)
        {
            percent = atom_min( 1.0, percent );
            percent = atom_max( 0.0, percent );
        
            double value = 65535 * percent;
            SetLevel( static_cast<uint16_t>(value) );
        }
};
 
 
 
#endif
