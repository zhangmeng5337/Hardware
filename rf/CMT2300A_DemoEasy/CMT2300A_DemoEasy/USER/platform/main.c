
#include <stdio.h>
#include <string.h>
//******************************************cmt2300a_defs.h***
/* ---------- CMT bank defines ---------- */
#define CMT2300A_CMT_BANK_ADDR           0x00
#define CMT2300A_CMT_BANK_SIZE           12
#define CMT2300A_CUS_CMT1                0x00
#define CMT2300A_CUS_CMT2                0x01
#define CMT2300A_CUS_CMT3                0x02
#define CMT2300A_CUS_CMT4                0x03
#define CMT2300A_CUS_CMT5                0x04
#define CMT2300A_CUS_CMT6                0x05
#define CMT2300A_CUS_CMT7                0x06
#define CMT2300A_CUS_CMT8                0x07
#define CMT2300A_CUS_CMT9                0x08
#define CMT2300A_CUS_CMT10               0x09
#define CMT2300A_CUS_CMT11               0x0A
#define CMT2300A_CUS_RSSI                0x0B

/* ---------- System bank defines ---------- */
#define CMT2300A_SYSTEM_BANK_ADDR        0x0C
#define CMT2300A_SYSTEM_BANK_SIZE        12
#define CMT2300A_CUS_SYS1                0x0C
#define CMT2300A_CUS_SYS2                0x0D
#define CMT2300A_CUS_SYS3                0x0E
#define CMT2300A_CUS_SYS4                0x0F
#define CMT2300A_CUS_SYS5                0x10
#define CMT2300A_CUS_SYS6                0x11
#define CMT2300A_CUS_SYS7                0x12
#define CMT2300A_CUS_SYS8                0x13
#define CMT2300A_CUS_SYS9                0x14
#define CMT2300A_CUS_SYS10               0x15
#define CMT2300A_CUS_SYS11               0x16
#define CMT2300A_CUS_SYS12               0x17

/* ---------- Frequency bank defines ---------- */
#define CMT2300A_FREQUENCY_BANK_ADDR     0x18
#define CMT2300A_FREQUENCY_BANK_SIZE     8
#define CMT2300A_CUS_RF1                 0x18
#define CMT2300A_CUS_RF2                 0x19
#define CMT2300A_CUS_RF3                 0x1A
#define CMT2300A_CUS_RF4                 0x1B
#define CMT2300A_CUS_RF5                 0x1C
#define CMT2300A_CUS_RF6                 0x1D
#define CMT2300A_CUS_RF7                 0x1E
#define CMT2300A_CUS_RF8                 0x1F

/* ---------- Data rate bank defines ---------- */
#define CMT2300A_DATA_RATE_BANK_ADDR     0x20
#define CMT2300A_DATA_RATE_BANK_SIZE     24
#define CMT2300A_CUS_RF9                 0x20
#define CMT2300A_CUS_RF10                0x21
#define CMT2300A_CUS_RF11                0x22
#define CMT2300A_CUS_RF12                0x23
#define CMT2300A_CUS_FSK1                0x24
#define CMT2300A_CUS_FSK2                0x25
#define CMT2300A_CUS_FSK3                0x26
#define CMT2300A_CUS_FSK4                0x27
#define CMT2300A_CUS_FSK5                0x28
#define CMT2300A_CUS_FSK6                0x29
#define CMT2300A_CUS_FSK7                0x2A
#define CMT2300A_CUS_CDR1                0x2B
#define CMT2300A_CUS_CDR2                0x2C
#define CMT2300A_CUS_CDR3                0x2D
#define CMT2300A_CUS_CDR4                0x2E
#define CMT2300A_CUS_AGC1                0x2F
#define CMT2300A_CUS_AGC2                0x30
#define CMT2300A_CUS_AGC3                0x31
#define CMT2300A_CUS_AGC4                0x32
#define CMT2300A_CUS_OOK1                0x33
#define CMT2300A_CUS_OOK2                0x34
#define CMT2300A_CUS_OOK3                0x35
#define CMT2300A_CUS_OOK4                0x36
#define CMT2300A_CUS_OOK5                0x37

/* ---------- Baseband bank defines ---------- */
#define CMT2300A_BASEBAND_BANK_ADDR      0x38
#define CMT2300A_BASEBAND_BANK_SIZE      29
#define CMT2300A_CUS_PKT1                0x38
#define CMT2300A_CUS_PKT2                0x39
#define CMT2300A_CUS_PKT3                0x3A
#define CMT2300A_CUS_PKT4                0x3B
#define CMT2300A_CUS_PKT5                0x3C
#define CMT2300A_CUS_PKT6                0x3D
#define CMT2300A_CUS_PKT7                0x3E
#define CMT2300A_CUS_PKT8                0x3F
#define CMT2300A_CUS_PKT9                0x40
#define CMT2300A_CUS_PKT10               0x41
#define CMT2300A_CUS_PKT11               0x42
#define CMT2300A_CUS_PKT12               0x43
#define CMT2300A_CUS_PKT13               0x44
#define CMT2300A_CUS_PKT14               0x45
#define CMT2300A_CUS_PKT15               0x46
#define CMT2300A_CUS_PKT16               0x47
#define CMT2300A_CUS_PKT17               0x48
#define CMT2300A_CUS_PKT18               0x49
#define CMT2300A_CUS_PKT19               0x4A
#define CMT2300A_CUS_PKT20               0x4B
#define CMT2300A_CUS_PKT21               0x4C
#define CMT2300A_CUS_PKT22               0x4D
#define CMT2300A_CUS_PKT23               0x4E
#define CMT2300A_CUS_PKT24               0x4F
#define CMT2300A_CUS_PKT25               0x50
#define CMT2300A_CUS_PKT26               0x51
#define CMT2300A_CUS_PKT27               0x52
#define CMT2300A_CUS_PKT28               0x53
#define CMT2300A_CUS_PKT29               0x54

/* ---------- Tx bank defines ---------- */
#define CMT2300A_TX_BANK_ADDR            0x55
#define CMT2300A_TX_BANK_SIZE            11
#define CMT2300A_CUS_TX1                 0x55
#define CMT2300A_CUS_TX2                 0x56
#define CMT2300A_CUS_TX3                 0x57
#define CMT2300A_CUS_TX4                 0x58
#define CMT2300A_CUS_TX5                 0x59
#define CMT2300A_CUS_TX6                 0x5A
#define CMT2300A_CUS_TX7                 0x5B
#define CMT2300A_CUS_TX8                 0x5C
#define CMT2300A_CUS_TX9                 0x5D
#define CMT2300A_CUS_TX10                0x5E
#define CMT2300A_CUS_LBD                 0x5F

/* ---------- Control1 bank defines ---------- */
#define CMT2300A_CONTROL1_BANK_ADDR      0x60
#define CMT2300A_CONTROL1_BANK_SIZE      11
#define CMT2300A_CUS_MODE_CTL            0x60
#define CMT2300A_CUS_MODE_STA            0x61
#define CMT2300A_CUS_EN_CTL              0x62
#define CMT2300A_CUS_FREQ_CHNL           0x63
#define CMT2300A_CUS_FREQ_OFS            0x64
#define CMT2300A_CUS_IO_SEL              0x65
#define CMT2300A_CUS_INT1_CTL            0x66
#define CMT2300A_CUS_INT2_CTL            0x67
#define CMT2300A_CUS_INT_EN              0x68
#define CMT2300A_CUS_FIFO_CTL            0x69
#define CMT2300A_CUS_INT_CLR1            0x6A

/* ---------- Control2 bank defines ---------- */
#define CMT2300A_CONTROL2_BANK_ADDR      0x6B
#define CMT2300A_CONTROL2_BANK_SIZE      7
#define CMT2300A_CUS_INT_CLR2            0x6B
#define CMT2300A_CUS_FIFO_CLR            0x6C
#define CMT2300A_CUS_INT_FLAG            0x6D
#define CMT2300A_CUS_FIFO_FLAG           0x6E
#define CMT2300A_CUS_RSSI_CODE           0x6F
#define CMT2300A_CUS_RSSI_DBM            0x70
#define CMT2300A_CUS_LBD_RESULT          0x71


/* ********** CMT2300A_CUS_CMT2 registers ********** */
#define CMT2300A_MASK_PRODUCT_ID         0xFF

/* ********** CMT2300A_CUS_CMT5 registers ********** */
#define CMT2300A_MASK_LMT_CODE           0xC0

/* ********** CMT2300A_CUS_CMT9 registers ********** */
#define CMT2300A_MASK_RSSI_OFFSET_SIGN   0x80
#define CMT2300A_MASK_DIG_CLKDIV         0x1F

/* ********** CMT2300A_CUS_RSSI registers ********** */
#define CMT2300A_MASK_RSSI_OFFSET        0xF8
#define CMT2300A_MASK_RSSI_SLOPE         0x07

/* ********** CMT2300A_CUS_SYS1 registers ********** */
#define CMT2300A_MASK_LMT_VTR            0xC0
#define CMT2300A_MASK_MIXER_BIAS         0x30
#define CMT2300A_MASK_LNA_MODE           0x0C
#define CMT2300A_MASK_LNA_BIAS           0x03

/* ********** CMT2300A_CUS_SYS2 registers ********** */
#define CMT2300A_MASK_LFOSC_RECAL_EN     0x80
#define CMT2300A_MASK_LFOSC_CAL1_EN      0x40
#define CMT2300A_MASK_LFOSC_CAL2_EN      0x20
#define CMT2300A_MASK_RX_TIMER_EN        0x10
#define CMT2300A_MASK_SLEEP_TIMER_EN     0x08
#define CMT2300A_MASK_TX_DC_EN           0x04
#define CMT2300A_MASK_RX_DC_EN           0x02
#define CMT2300A_MASK_DC_PAUSE           0x01

/* ********** CMT2300A_CUS_SYS3 registers ********** */
#define CMT2300A_MASK_SLEEP_BYPASS_EN    0x80
#define CMT2300A_MASK_XTAL_STB_TIME      0x70
#define CMT2300A_MASK_TX_EXIT_STATE      0x0C
#define CMT2300A_MASK_RX_EXIT_STATE      0x03

/* ********** CMT2300A_CUS_SYS4 registers ********** */
#define CMT2300A_MASK_SLEEP_TIMER_M_7_0  0xFF

/* ********** CMT2300A_CUS_SYS5 registers ********** */
#define CMT2300A_MASK_SLEEP_TIMER_M_10_8 0x70
#define CMT2300A_MASK_SLEEP_TIMER_R      0x0F

/* ********** CMT2300A_CUS_SYS6 registers ********** */
#define CMT2300A_MASK_RX_TIMER_T1_M_7_0  0xFF

/* ********** CMT2300A_CUS_SYS7 registers ********** */
#define CMT2300A_MASK_RX_TIMER_T1_M_10_8 0x70
#define CMT2300A_MASK_RX_TIMER_T1_R      0x0F

/* ********** CMT2300A_CUS_SYS8 registers ********** */
#define CMT2300A_MASK_RX_TIMER_T2_M_7_0  0xFF

/* ********** CMT2300A_CUS_SYS9 registers ********** */
#define CMT2300A_MASK_RX_TIMER_T2_M_10_8 0x70
#define CMT2300A_MASK_RX_TIMER_T2_R      0x0F

/* ********** CMT2300A_CUS_SYS10 registers ********** */
#define CMT2300A_MASK_COL_DET_EN         0x80
#define CMT2300A_MASK_COL_OFS_SEL        0x40
#define CMT2300A_MASK_RX_AUTO_EXIT_DIS   0x20
#define CMT2300A_MASK_DOUT_MUTE          0x10
#define CMT2300A_MASK_RX_EXTEND_MODE     0x0F

/* ********** CMT2300A_CUS_SYS11 registers ********** */
#define CMT2300A_MASK_PJD_TH_SEL         0x80
#define CMT2300A_MASK_CCA_INT_SEL        0x60
#define CMT2300A_MASK_RSSI_DET_SEL       0x18
#define CMT2300A_MASK_RSSI_AVG_MODE      0x07

/* ********** CMT2300A_CUS_SYS12 registers ********** */
#define CMT2300A_MASK_PJD_WIN_SEL        0xC0
#define CMT2300A_MASK_CLKOUT_EN          0x20
#define CMT2300A_MASK_CLKOUT_DIV         0x1F

/* ********** CMT2300A_CUS_RF1 registers ********** */
#define CMT2300A_MASK_FREQ_RX_N          0xFF

/* ********** CMT2300A_CUS_RF2 registers ********** */
#define CMT2300A_MASK_FREQ_RX_K_7_0      0xFF

/* ********** CMT2300A_CUS_RF3 registers ********** */
#define CMT2300A_MASK_FREQ_RX_K_15_8     0xFF

/* ********** CMT2300A_CUS_RF4 registers ********** */
#define CMT2300A_MASK_FREQ_PALDO_SEL     0x80
#define CMT2300A_MASK_FREQ_DIVX_CODE     0x70
#define CMT2300A_MASK_FREQ_RX_K_19_16    0x0F

/* ********** CMT2300A_CUS_RF5 registers ********** */
#define CMT2300A_MASK_FREQ_TX_N          0xFF

/* ********** CMT2300A_CUS_RF6 registers ********** */
#define CMT2300A_MASK_FREQ_TX_K_7_0      0xFF

/* ********** CMT2300A_CUS_RF7 registers ********** */
#define CMT2300A_MASK_FREQ_TX_K_15_8     0xFF

/* ********** CMT2300A_CUS_RF8 registers ********** */
#define CMT2300A_MASK_FSK_SWT            0x80
#define CMT2300A_MASK_FREQ_VCO_BANK      0x70
#define CMT2300A_MASK_FREQ_TX_K_19_16    0x0F

/* ********** CMT2300A_CUS_PKT1 registers ********** */
#define CMT2300A_MASK_RX_PREAM_SIZE      0xF8
#define CMT2300A_MASK_PREAM_LENG_UNIT    0x04
#define CMT2300A_MASK_DATA_MODE          0x03
/* CMT2300A_MASK_PREAM_LENG_UNIT options */
#define CMT2300A_PREAM_LENG_UNIT_8_BITS  0x00
#define CMT2300A_PREAM_LENG_UNIT_4_BITS  0x04
/* CMT2300A_MASK_DATA_MODE options */
#define CMT2300A_DATA_MODE_DIRECT        0x00
#define CMT2300A_DATA_MODE_PACKET        0x02

/* ********** CMT2300A_CUS_PKT2 registers ********** */
#define CMT2300A_MASK_TX_PREAM_SIZE_7_0  0xFF

/* ********** CMT2300A_CUS_PKT3 registers ********** */
#define CMT2300A_MASK_TX_PREAM_SIZE_15_8 0xFF

/* ********** CMT2300A_CUS_PKT4 registers ********** */
#define CMT2300A_MASK_PREAM_VALUE        0xFF

/* ********** CMT2300A_CUS_PKT5 registers ********** */
#define CMT2300A_MASK_SYNC_TOL           0x70
#define CMT2300A_MASK_SYNC_SIZE          0x0E
#define CMT2300A_MASK_SYNC_MAN_EN        0x01

/* ********** CMT2300A_CUS_PKT6 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_7_0     0xFF

/* ********** CMT2300A_CUS_PKT7 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_15_8    0xFF

/* ********** CMT2300A_CUS_PKT8 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_23_16   0xFF

/* ********** CMT2300A_CUS_PKT9 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_31_24   0xFF

/* ********** CMT2300A_CUS_PKT10 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_39_32   0xFF

/* ********** CMT2300A_CUS_PKT11 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_47_40   0xFF

/* ********** CMT2300A_CUS_PKT12 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_55_48   0xFF

/* ********** CMT2300A_CUS_PKT13 registers ********** */
#define CMT2300A_MASK_SYNC_VALUE_63_56   0xFF

/* ********** CMT2300A_CUS_PKT14 registers ********** */
#define CMT2300A_MASK_PAYLOAD_LENG_10_8  0x70
#define CMT2300A_MASK_AUTO_ACK_EN        0x08
#define CMT2300A_MASK_NODE_LENG_POS_SEL  0x04
#define CMT2300A_MASK_PAYLOAD_BIT_ORDER  0x02
#define CMT2300A_MASK_PKT_TYPE           0x01
/* CMT2300A_MASK_NODE_LENG_POS_SEL options */
#define CMT2300A_NODE_LENG_FIRST_NODE    0x00
#define CMT2300A_NODE_LENG_FIRST_LENGTH  0x04
/* CMT2300A_MASK_PAYLOAD_BIT_ORDER options */
#define CMT2300A_PAYLOAD_BIT_ORDER_MSB   0x00
#define CMT2300A_PAYLOAD_BIT_ORDER_LSB   0x02
/* CMT2300A_MASK_PKT_TYPE options */
#define CMT2300A_PKT_TYPE_FIXED          0x00
#define CMT2300A_PKT_TYPE_VARIABLE       0x01

/* ********** CMT2300A_CUS_PKT15 registers ********** */
#define CMT2300A_MASK_PAYLOAD_LENG_7_0   0xFF

/* ********** CMT2300A_CUS_PKT16 registers ********** */
#define CMT2300A_MASK_NODE_FREE_EN       0x20
#define CMT2300A_MASK_NODE_ERR_MASK      0x10
#define CMT2300A_MASK_NODE_SIZE          0x0C
#define CMT2300A_MASK_NODE_DET_MODE      0x03
/* CMT2300A_MASK_NODE_DET_MODE options */
#define CMT2300A_NODE_DET_NODE           0x00
#define CMT2300A_NODE_DET_VALUE          0x01
#define CMT2300A_NODE_DET_VALUE_0        0x02
#define CMT2300A_NODE_DET_VALUE_0_1      0x03

/* ********** CMT2300A_CUS_PKT17 registers ********** */
#define CMT2300A_MASK_NODE_VALUE_7_0     0xFF

/* ********** CMT2300A_CUS_PKT18 registers ********** */
#define CMT2300A_MASK_NODE_VALUE_15_8    0xFF

/* ********** CMT2300A_CUS_PKT19 registers ********** */
#define CMT2300A_MASK_NODE_VALUE_23_16   0xFF

/* ********** CMT2300A_CUS_PKT20 registers ********** */
#define CMT2300A_MASK_NODE_VALUE_31_24   0xFF

/* ********** CMT2300A_CUS_PKT21 registers ********** */
#define CMT2300A_MASK_FEC_TYPE           0x80
#define CMT2300A_MASK_FEC_EN             0x40
#define CMT2300A_MASK_CRC_BYTE_SWAP      0x20
#define CMT2300A_MASK_CRC_BIT_INV        0x10
#define CMT2300A_MASK_CRC_RANGE          0x08
#define CMT2300A_MASK_CRC_TYPE           0x06
#define CMT2300A_MASK_CRC_EN             0x01
/* CMT2300A_MASK_CRC_BYTE_SWAP options */
#define CMT2300A_CRC_ORDER_HBYTE         0x00
#define CMT2300A_CRC_ORDER_LBYTE         0x20
/* CMT2300A_MASK_CRC_RANGE options */
#define CMT2300A_CRC_RANGE_PAYLOAD       0x00
#define CMT2300A_CRC_RANGE_DATA          0x08
/* CMT2300A_MASK_CRC_TYPE options */
#define CMT2300A_CRC_TYPE_CCITT16        0x00
#define CMT2300A_CRC_TYPE_IBM16          0x02
#define CMT2300A_CRC_TYPE_ITU16          0x04

/* ********** CMT2300A_CUS_PKT22 registers ********** */
#define CMT2300A_MASK_CRC_SEED_7_0       0xFF

/* ********** CMT2300A_CUS_PKT23 registers ********** */
#define CMT2300A_MASK_CRC_SEED_15_8      0xFF

/* ********** CMT2300A_CUS_PKT24 registers ********** */
#define CMT2300A_MASK_CRC_BIT_ORDER      0x80
#define CMT2300A_MASK_WHITEN_SEED_8_8    0x40
#define CMT2300A_MASK_WHITEN_SEED_TYPE   0x20
#define CMT2300A_MASK_WHITEN_TYPE        0x18
#define CMT2300A_MASK_WHITEN_EN          0x04
#define CMT2300A_MASK_MANCH_TYPE         0x02
#define CMT2300A_MASK_MANCH_EN           0x01
/* CMT2300A_MASK_CRC_BIT_ORDER options */
#define CMT2300A_CRC_BIT_ORDER_MSB       0x00
#define CMT2300A_CRC_BIT_ORDER_LSB       0x80
/* CMT2300A_MASK_WHITEN_SEED_TYPE options */
#define CMT2300A_WHITEN_SEED_TYPE_1      0x00
#define CMT2300A_WHITEN_SEED_TYPE_2      0x20
/* CMT2300A_MASK_WHITEN_TYPE options */
#define CMT2300A_WHITEN_TYPE_PN9_CCITT   0x00
#define CMT2300A_WHITEN_TYPE_PN9_IBM     0x08
#define CMT2300A_WHITEN_TYPE_PN7         0x10
/* CMT2300A_MASK_MANCH_TYPE options */
#define CMT2300A_MANCH_TYPE_ONE_01       0x00
#define CMT2300A_MANCH_TYPE_ONE_10       0x02

/* ********** CMT2300A_CUS_PKT25 registers ********** */
#define CMT2300A_MASK_WHITEN_SEED_7_0    0xFF

/* ********** CMT2300A_CUS_PKT26 registers ********** */
#define CMT2300A_MASK_TX_PREFIX_TYPE     0x03

/* ********** CMT2300A_CUS_PKT27 registers ********** */
#define CMT2300A_MASK_TX_PKT_NUM         0xFF

/* ********** CMT2300A_CUS_PKT28 registers ********** */
#define CMT2300A_MASK_TX_PKT_GAP         0xFF

/* ********** CMT2300A_CUS_PKT29 registers ********** */
#define CMT2300A_MASK_FIFO_AUTO_RES_EN   0x80
#define CMT2300A_MASK_FIFO_TH            0x7F

/* ********** CMT2300A_CUS_MODE_CTL registers ********** */
#define CMT2300A_MASK_CHIP_MODE_SWT      0xFF
/* CMT2300A_MASK_CHIP_MODE_SWT options */
#define CMT2300A_GO_EEPROM               0x01
#define CMT2300A_GO_STBY                 0x02
#define CMT2300A_GO_RFS                  0x04
#define CMT2300A_GO_RX                   0x08
#define CMT2300A_GO_SLEEP                0x10
#define CMT2300A_GO_TFS                  0x20
#define CMT2300A_GO_TX                   0x40
#define CMT2300A_GO_SWITCH               0x80

/* ********** CMT2300A_CUS_MODE_STA registers ********** */
#define CMT2300A_MASK_RSTN_IN_EN         0x20
#define CMT2300A_MASK_CFG_RETAIN         0x10
#define CMT2300A_MASK_CHIP_MODE_STA      0x0F
/* CMT2300A_MASK_CHIP_MODE_STA options */
#define CMT2300A_STA_IDLE                0x00
#define CMT2300A_STA_SLEEP               0x01
#define CMT2300A_STA_STBY                0x02
#define CMT2300A_STA_RFS                 0x03
#define CMT2300A_STA_TFS                 0x04
#define CMT2300A_STA_RX                  0x05
#define CMT2300A_STA_TX                  0x06
#define CMT2300A_STA_EEPROM              0x07
#define CMT2300A_STA_ERROR               0x08
#define CMT2300A_STA_CAL                 0x09

/* ********** CMT2300A_CUS_EN_CTL registers ********** */
#define CMT2300A_MASK_LOCKING_EN         0x20

/* ********** CMT2300A_CUS_FREQ_CHNL registers ********** */
#define CMT2300A_MASK_FH_CHANNEL         0xFF

/* ********** CMT2300A_CUS_FREQ_OFS registers ********** */
#define CMT2300A_MASK_FH_OFFSET          0xFF

/* ********** CMT2300A_CUS_IO_SEL registers ********** */
#define CMT2300A_MASK_GPIO4_SEL          0xC0
#define CMT2300A_MASK_GPIO3_SEL          0x30
#define CMT2300A_MASK_GPIO2_SEL          0x0C
#define CMT2300A_MASK_GPIO1_SEL          0x03
/* CMT2300A_MASK_GPIO4_SEL options */
#define CMT2300A_GPIO4_SEL_RSTIN         0x00
#define CMT2300A_GPIO4_SEL_INT1          0x40
#define CMT2300A_GPIO4_SEL_DOUT          0x80
#define CMT2300A_GPIO4_SEL_DCLK          0xC0
/* CMT2300A_MASK_GPIO3_SEL options */
#define CMT2300A_GPIO3_SEL_CLKO          0x00
#define CMT2300A_GPIO3_SEL_DOUT          0x10
#define CMT2300A_GPIO3_SEL_DIN           0x10
#define CMT2300A_GPIO3_SEL_INT2          0x20
#define CMT2300A_GPIO3_SEL_DCLK          0x30
/* CMT2300A_MASK_GPIO2_SEL options */
#define CMT2300A_GPIO2_SEL_INT1          0x00
#define CMT2300A_GPIO2_SEL_INT2          0x04
#define CMT2300A_GPIO2_SEL_DOUT          0x08
#define CMT2300A_GPIO2_SEL_DIN           0x08
#define CMT2300A_GPIO2_SEL_DCLK          0x0C
/* CMT2300A_MASK_GPIO1_SEL options */
#define CMT2300A_GPIO1_SEL_DOUT          0x00
#define CMT2300A_GPIO1_SEL_DIN           0x00
#define CMT2300A_GPIO1_SEL_INT1          0x01
#define CMT2300A_GPIO1_SEL_INT2          0x02
#define CMT2300A_GPIO1_SEL_DCLK          0x03

/* ********** CMT2300A_CUS_INT1_CTL registers ********** */
#define CMT2300A_MASK_RF_SWT1_EN         0x80
#define CMT2300A_MASK_RF_SWT2_EN         0x40
#define CMT2300A_MASK_INT_POLAR          0x20
#define CMT2300A_MASK_INT1_SEL           0x1F
/* CMT2300A_MASK_INT_POLAR options */
#define CMT2300A_INT_POLAR_SEL_0         0x00
#define CMT2300A_INT_POLAR_SEL_1         0x20
/* CMT2300A_MASK_INT1_SEL options */
#define CMT2300A_INT_SEL_RX_ACTIVE       0x00
#define CMT2300A_INT_SEL_TX_ACTIVE       0x01
#define CMT2300A_INT_SEL_RSSI_VLD        0x02
#define CMT2300A_INT_SEL_PREAM_OK        0x03
#define CMT2300A_INT_SEL_SYNC_OK         0x04
#define CMT2300A_INT_SEL_NODE_OK         0x05
#define CMT2300A_INT_SEL_CRC_OK          0x06
#define CMT2300A_INT_SEL_PKT_OK          0x07
#define CMT2300A_INT_SEL_SL_TMO          0x08
#define CMT2300A_INT_SEL_RX_TMO          0x09
#define CMT2300A_INT_SEL_TX_DONE         0x0A
#define CMT2300A_INT_SEL_RX_FIFO_NMTY    0x0B
#define CMT2300A_INT_SEL_RX_FIFO_TH      0x0C
#define CMT2300A_INT_SEL_RX_FIFO_FULL    0x0D
#define CMT2300A_INT_SEL_RX_FIFO_WBYTE   0x0E
#define CMT2300A_INT_SEL_RX_FIFO_OVF     0x0F
#define CMT2300A_INT_SEL_TX_FIFO_NMTY    0x10
#define CMT2300A_INT_SEL_TX_FIFO_TH      0x11
#define CMT2300A_INT_SEL_TX_FIFO_FULL    0x12
#define CMT2300A_INT_SEL_STATE_IS_STBY   0x13
#define CMT2300A_INT_SEL_STATE_IS_FS     0x14
#define CMT2300A_INT_SEL_STATE_IS_RX     0x15
#define CMT2300A_INT_SEL_STATE_IS_TX     0x16
#define CMT2300A_INT_SEL_LED             0x17
#define CMT2300A_INT_SEL_TRX_ACTIVE      0x18
#define CMT2300A_INT_SEL_PKT_DONE        0x19

/* ********** CMT2300A_CUS_INT2_CTL registers ********** */
#define CMT2300A_MASK_LFOSC_OUT_EN       0x40
#define CMT2300A_MASK_TX_DIN_INV         0x20
#define CMT2300A_MASK_INT2_SEL           0x1F

/* ********** CMT2300A_CUS_INT_EN registers ********** */
#define CMT2300A_MASK_SL_TMO_EN          0x80
#define CMT2300A_MASK_RX_TMO_EN          0x40
#define CMT2300A_MASK_TX_DONE_EN         0x20
#define CMT2300A_MASK_PREAM_OK_EN        0x10
#define CMT2300A_MASK_SYNC_OK_EN         0x08
#define CMT2300A_MASK_NODE_OK_EN         0x04
#define CMT2300A_MASK_CRC_OK_EN          0x02
#define CMT2300A_MASK_PKT_DONE_EN        0x01

/* ********** CMT2300A_CUS_FIFO_CTL registers ********** */
#define CMT2300A_MASK_TX_DIN_EN          0x80
#define CMT2300A_MASK_TX_DIN_SEL         0x60
#define CMT2300A_MASK_FIFO_AUTO_CLR_DIS  0x10
#define CMT2300A_MASK_FIFO_TX_RD_EN      0x08
#define CMT2300A_MASK_FIFO_RX_TX_SEL     0x04
#define CMT2300A_MASK_FIFO_MERGE_EN      0x02
#define CMT2300A_MASK_SPI_FIFO_RD_WR_SEL 0x01
/* CMT2300A_MASK_TX_DIN_SEL options */
#define CMT2300A_TX_DIN_SEL_GPIO1        0x00
#define CMT2300A_TX_DIN_SEL_GPIO2        0x20
#define CMT2300A_TX_DIN_SEL_GPIO3        0x40

/* ********** CMT2300A_CUS_INT_CLR1 registers ********** */
#define CMT2300A_MASK_SL_TMO_FLG         0x20
#define CMT2300A_MASK_RX_TMO_FLG         0x10
#define CMT2300A_MASK_TX_DONE_FLG        0x08
#define CMT2300A_MASK_TX_DONE_CLR        0x04
#define CMT2300A_MASK_SL_TMO_CLR         0x02
#define CMT2300A_MASK_RX_TMO_CLR         0x01

/* ********** CMT2300A_CUS_INT_CLR2 registers ********** */
#define CMT2300A_MASK_LBD_CLR            0x20
#define CMT2300A_MASK_PREAM_OK_CLR       0x10
#define CMT2300A_MASK_SYNC_OK_CLR        0x08
#define CMT2300A_MASK_NODE_OK_CLR        0x04
#define CMT2300A_MASK_CRC_OK_CLR         0x02
#define CMT2300A_MASK_PKT_DONE_CLR       0x01

/* ********** CMT2300A_CUS_FIFO_CLR registers ********** */
#define CMT2300A_MASK_FIFO_RESTORE       0x04
#define CMT2300A_MASK_FIFO_CLR_RX        0x02
#define CMT2300A_MASK_FIFO_CLR_TX        0x01

/* ********** CMT2300A_CUS_INT_FLAG registers ********** */
#define CMT2300A_MASK_LBD_FLG            0x80
#define CMT2300A_MASK_COL_ERR_FLG        0x40
#define CMT2300A_MASK_PKT_ERR_FLG        0x20
#define CMT2300A_MASK_PREAM_OK_FLG       0x10
#define CMT2300A_MASK_SYNC_OK_FLG        0x08
#define CMT2300A_MASK_NODE_OK_FLG        0x04
#define CMT2300A_MASK_CRC_OK_FLG         0x02
#define CMT2300A_MASK_PKT_OK_FLG         0x01

/* ********** CMT2300A_CUS_FIFO_FLAG registers ********** */
#define CMT2300A_MASK_RX_FIFO_FULL_FLG   0x40
#define CMT2300A_MASK_RX_FIFO_NMTY_FLG   0x20
#define CMT2300A_MASK_RX_FIFO_TH_FLG     0x10
#define CMT2300A_MASK_RX_FIFO_OVF_FLG    0x08
#define CMT2300A_MASK_TX_FIFO_FULL_FLG   0x04
#define CMT2300A_MASK_TX_FIFO_NMTY_FLG   0x02
#define CMT2300A_MASK_TX_FIFO_TH_FLG     0x01

/* ********** CMT2300A_CUS_RSSI_CODE registers ********** */
#define CMT2300A_MASK_RSSI_CODE          0xFF

/* ********** CMT2300A_CUS_RSSI_DBM registers ********** */
#define CMT2300A_MASK_RSSI_DBM           0xFF

/* ********** CMT2300A_CUS_LBD_RESULT registers ********** */
#define CMT2300A_MASK_LBD_RESULT         0xFF
//*******************************************end cmt2300a_defs.h
//**********************************typedef***************************
#define xdata
#define data
#define code const

#define NOP() __NOP()

#ifndef u8
    #define u8  unsigned char
#endif

#ifndef u16
    #define u16 unsigned short
#endif

#ifndef u32
    #define u32 unsigned long
#endif

#ifndef U8
    #define U8  unsigned char
#endif

#ifndef S8
    #define S8  char
#endif

#ifndef U16
    #define U16 unsigned short
#endif

#ifndef U32
    #define U32 unsigned long
#endif

#ifndef BOOL
    #define BOOL u8
#endif

#ifndef TRUE
    #define TRUE (BOOL)SET
#endif

#ifndef FALSE
    #define FALSE (BOOL)RESET
#endif

#define INFINITE 0xFFFFFFFF

//*******************************************************************
//******************************gpio***********************************
#define CMT_CSB_GPIO                GPIOB
#define CMT_CSB_GPIO_PIN            GPIO_Pin_6

#define CMT_FCSB_GPIO               GPIOD
#define CMT_FCSB_GPIO_PIN           GPIO_Pin_7

#define CMT_SCLK_GPIO               GPIOD
#define CMT_SCLK_GPIO_PIN           GPIO_Pin_4

#define CMT_SDIO_GPIO               GPIOD
#define CMT_SDIO_GPIO_PIN           GPIO_Pin_2

#define CMT_GPIO1_GPIO              GPIOD
#define CMT_GPIO1_GPIO_PIN          GPIO_Pin_5

#define CMT_GPIO2_GPIO              GPIOD
#define CMT_GPIO2_GPIO_PIN          GPIO_Pin_3

#define CMT_GPIO3_GPIO              GPIOD
#define CMT_GPIO3_GPIO_PIN          GPIO_Pin_0

#define CMT_GPIO4_GPIO              GPIOD
#define CMT_GPIO4_GPIO_PIN          GPIO_Pin_1



#define SET_GPIO_OUT(x)             GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP)
#define SET_GPIO_IN(x)              GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING)
#define SET_GPIO_OD(x)              GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_OD)
#define SET_GPIO_AIN(x)             GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AIN)
#define SET_GPIO_AFOUT(x)           GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_PP)
#define SET_GPIO_AFOD(x)            GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_OD)
#define SET_GPIO_H(x)               (x->BSRR = x##_PIN) //GPIO_SetBits(x, x##_PIN)
#define SET_GPIO_L(x)               (x->BRR  = x##_PIN) //GPIO_ResetBits(x, x##_PIN)
#define READ_GPIO_PIN(x)            (((x->IDR & x##_PIN)!=Bit_RESET) ?1 :0) //GPIO_ReadInputDataBit(x, x##_PIN) 

//*******************************************************************
//****************************#include "cmt2300a_params.h"

/* [CMT Bank] */
const u8 g_cmt2300aCmtBank[CMT2300A_CMT_BANK_SIZE] =
{
    0x00,
    0x66,
    0xEC,
    0x1C,
    0xF0,
    0x80,
    0x14,
    0x08,
    0x91,
    0x02,
    0x02,
    0xD0,
};

/* [System Bank] */
const u8 g_cmt2300aSystemBank[CMT2300A_SYSTEM_BANK_SIZE] =
{
    0xAE,
    0xE0,
    0x35,
    0x00,
    0x00,
    0xF4,
    0x10,
    0xE2,
    0x42,
    0x20,
    0x00,
    0x81,
};

/* [Frequency Bank] */
const u8 g_cmt2300aFrequencyBank[CMT2300A_FREQUENCY_BANK_SIZE] =
{
    0x42,
    0x71,
    0xCE,
    0x1C,
    0x42,
    0x5B,
    0x1C,
    0x1C,
};

/* [Data Rate Bank] */
const u8 g_cmt2300aDataRateBank[CMT2300A_DATA_RATE_BANK_SIZE] =
{
    0x32,
    0x18,
    0x00,
    0x99,
    0xC1,
    0x9B,
    0x06,
    0x0A,
    0x9F,
    0x39,
    0x29,
    0x29,
    0xC0,
    0x51,
    0x2A,
    0x53,
    0x00,
    0x00,
    0xB4,
    0x00,
    0x00,
    0x01,
    0x00,
    0x00,
};

/* [Baseband Bank] */
const u8 g_cmt2300aBasebandBank[CMT2300A_BASEBAND_BANK_SIZE] =
{
    0x12,
    0x08,
    0x00,
    0xAA,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xD4,
    0x2D,
    0x00,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x60,
    0xFF,
    0x00,
    0x00,
    0x1F,
    0x10,
};

/* [Tx Bank] */
const u8 g_cmt2300aTxBank[CMT2300A_TX_BANK_SIZE] =
{
    0x50,
    0x26,
    0x03,
    0x00,
    0x42,
    0xB0,
    0x00,
    0x37,
    0x0A,
    0x3F,
    0x7F,
};
//********************end #include "cmt2300a_params.h"

//*******************************spi.c*************************


/* ************************************************************************
*  The following need to be modified by user
*  ************************************************************************ */
#define cmt_spi3_csb_out()      SET_GPIO_OUT(CMT_CSB_GPIO)
#define cmt_spi3_fcsb_out()     SET_GPIO_OUT(CMT_FCSB_GPIO)
#define cmt_spi3_sclk_out()     SET_GPIO_OUT(CMT_SCLK_GPIO)
#define cmt_spi3_sdio_out()     SET_GPIO_OUT(CMT_SDIO_GPIO)
#define cmt_spi3_sdio_in()      SET_GPIO_IN(CMT_SDIO_GPIO)

#define cmt_spi3_csb_1()        SET_GPIO_H(CMT_CSB_GPIO)
#define cmt_spi3_csb_0()        SET_GPIO_L(CMT_CSB_GPIO)

#define cmt_spi3_fcsb_1()       SET_GPIO_H(CMT_FCSB_GPIO)
#define cmt_spi3_fcsb_0()       SET_GPIO_L(CMT_FCSB_GPIO)

#define cmt_spi3_sclk_1()       SET_GPIO_H(CMT_SCLK_GPIO)
#define cmt_spi3_sclk_0()       SET_GPIO_L(CMT_SCLK_GPIO)

#define cmt_spi3_sdio_1()       SET_GPIO_H(CMT_SDIO_GPIO)
#define cmt_spi3_sdio_0()       SET_GPIO_L(CMT_SDIO_GPIO)
#define cmt_spi3_sdio_read()    READ_GPIO_PIN(CMT_SDIO_GPIO)
/* ************************************************************************ */

void cmt_spi3_delay(void)
{
    u32 n = 7;
    while (n--);
}

void cmt_spi3_delay_us(void)
{
    u16 n = 8;
    while (n--);
}

void cmt_spi3_init(void)
{
    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
    cmt_spi3_csb_1();   /* CSB has an internal pull-up resistor */

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();   /* SCLK has an internal pull-down resistor */

    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();
    cmt_spi3_sdio_1();

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();  /* FCSB has an internal pull-up resistor */

    cmt_spi3_delay();
}

void cmt_spi3_send(u8 data8)
{
    u8 i;

    for (i = 0; i < 8; i++)
    {
        cmt_spi3_sclk_0();

        /* Send byte on the rising edge of SCLK */
        if (data8 & 0x80)
            cmt_spi3_sdio_1();
        else
            cmt_spi3_sdio_0();

        cmt_spi3_delay();

        data8 <<= 1;
        cmt_spi3_sclk_1();
        cmt_spi3_delay();
    }
}

u8 cmt_spi3_recv(void)
{
    u8 i;
    u8 data8 = 0xFF;

    for (i = 0; i < 8; i++)
    {
        cmt_spi3_sclk_0();
        cmt_spi3_delay();
        data8 <<= 1;

        cmt_spi3_sclk_1();

        /* Read byte on the rising edge of SCLK */
        if (cmt_spi3_sdio_read())
            data8 |= 0x01;
        else
            data8 &= ~0x01;

        cmt_spi3_delay();
    }

    return data8;
}

void cmt_spi3_write(u8 addr, u8 dat)
{
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    /* r/w = 0 */
    cmt_spi3_send(addr & 0x7F);

    cmt_spi3_send(dat);

    cmt_spi3_sclk_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    cmt_spi3_csb_1();

    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();

    cmt_spi3_fcsb_1();
}

void cmt_spi3_read(u8 addr, u8 *p_dat)
{
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    /* r/w = 1 */
    cmt_spi3_send(addr | 0x80);

    /* Must set SDIO to input before the falling edge of SCLK */
    cmt_spi3_sdio_in();

    *p_dat = cmt_spi3_recv();

    cmt_spi3_sclk_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    cmt_spi3_csb_1();

    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();

    cmt_spi3_fcsb_1();
}

void cmt_spi3_write_fifo(const u8 *p_buf, u16 len)
{
    u16 i;

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
    cmt_spi3_csb_1();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_sdio_out();

    for (i = 0; i < len; i++)
    {
        cmt_spi3_fcsb_0();

        /* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_delay();

        cmt_spi3_send(p_buf[i]);

        cmt_spi3_sclk_0();

        /* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();

        cmt_spi3_fcsb_1();

        /* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }

    cmt_spi3_sdio_in();

    cmt_spi3_fcsb_1();
}

void cmt_spi3_read_fifo(u8 *p_buf, u16 len)
{
    u16 i;

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
    cmt_spi3_csb_1();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_sdio_in();

    for (i = 0; i < len; i++)
    {
        cmt_spi3_fcsb_0();

        /* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_delay();

        p_buf[i] = cmt_spi3_recv();

        cmt_spi3_sclk_0();

        /* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();

        cmt_spi3_fcsb_1();

        /* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }

    cmt_spi3_sdio_in();

    cmt_spi3_fcsb_1();
}

//*************************************************************
//*******************************************hal.c*****************
#define CMT2300A_SetGpio1In()           SET_GPIO_IN(CMT_GPIO1_GPIO)
#define CMT2300A_SetGpio2In()           SET_GPIO_IN(CMT_GPIO2_GPIO)
#define CMT2300A_SetGpio3In()           SET_GPIO_IN(CMT_GPIO3_GPIO)
#define CMT2300A_ReadGpio1()            READ_GPIO_PIN(CMT_GPIO1_GPIO)
#define CMT2300A_ReadGpio2()            READ_GPIO_PIN(CMT_GPIO2_GPIO)
#define CMT2300A_ReadGpio3()            READ_GPIO_PIN(CMT_GPIO3_GPIO)
#define CMT2300A_DelayMs(ms)            system_delay_ms(ms)
#define CMT2300A_DelayUs(us)            system_delay_us(us)
#define CMT2300A_GetTickCount()         g_nSysTickCount

/*! ********************************************************
* @name    CMT2300A_InitGpio
* @desc    Initializes the CMT2300A interface GPIOs.
* *********************************************************/
void CMT2300A_InitGpio(void)
{
    CMT2300A_SetGpio1In();
    CMT2300A_SetGpio2In();
    CMT2300A_SetGpio3In();

    cmt_spi3_init();
}

/*! ********************************************************
* @name    CMT2300A_ReadReg
* @desc    Read the CMT2300A register at the specified address.
* @param   addr: register address
* @return  Register value
* *********************************************************/
u8 CMT2300A_ReadReg(u8 addr)
{
    u8 dat = 0xFF;
    cmt_spi3_read(addr, &dat);

    return dat;
}

/*! ********************************************************
* @name    CMT2300A_WriteReg
* @desc    Write the CMT2300A register at the specified address.
* @param   addr: register address
*          dat: register value
* *********************************************************/
void CMT2300A_WriteReg(u8 addr, u8 dat)
{
    cmt_spi3_write(addr, dat);
}

/*! ********************************************************
* @name    CMT2300A_ReadFifo
* @desc    Reads the contents of the CMT2300A FIFO.
* @param   buf: buffer where to copy the FIFO read data
*          len: number of bytes to be read from the FIFO
* *********************************************************/
void CMT2300A_ReadFifo(u8 buf[], u16 len)
{
    cmt_spi3_read_fifo(buf, len);
}

/*! ********************************************************
* @name    CMT2300A_WriteFifo
* @desc    Writes the buffer contents to the CMT2300A FIFO.
* @param   buf: buffer containing data to be put on the FIFO
*          len: number of bytes to be written to the FIFO
* *********************************************************/
void CMT2300A_WriteFifo(const u8 buf[], u16 len)
{
    cmt_spi3_write_fifo(buf, len);
}

//*********************************************************************

//*******************cmt2300a.c****************************************


/*! ********************************************************
* @name    CMT2300A_SoftReset
* @desc    Soft reset.
* *********************************************************/
void CMT2300A_SoftReset(void)
{
    CMT2300A_WriteReg(0x7F, 0xFF);
}

/*! ********************************************************
* @name    CMT2300A_GetChipStatus
* @desc    Get the chip status.
* @return
*          CMT2300A_STA_PUP
*          CMT2300A_STA_SLEEP
*          CMT2300A_STA_STBY
*          CMT2300A_STA_RFS
*          CMT2300A_STA_TFS
*          CMT2300A_STA_RX
*          CMT2300A_STA_TX
*          CMT2300A_STA_EEPROM
*          CMT2300A_STA_ERROR
*          CMT2300A_STA_CAL
* *********************************************************/
u8 CMT2300A_GetChipStatus(void)
{
    return  CMT2300A_ReadReg(CMT2300A_CUS_MODE_STA) & CMT2300A_MASK_CHIP_MODE_STA;
}

/*! ********************************************************
* @name    CMT2300A_AutoSwitchStatus
* @desc    Auto switch the chip status, and 10 ms as timeout.
* @param   nGoCmd: the chip next status
* @return  TRUE or FALSE
* *********************************************************/
#define ENABLE_AUTO_SWITCH_CHIP_STATUS 0
BOOL CMT2300A_AutoSwitchStatus(u8 nGoCmd)
{
//#ifdef ENABLE_AUTO_SWITCH_CHIP_STATUS
if(ENABLE_AUTO_SWITCH_CHIP_STATUS == 1)
{
u32 nBegTick = CMT2300A_GetTickCount();
    u8  nWaitStatus;

    switch (nGoCmd)
    {
        case CMT2300A_GO_SLEEP:
            nWaitStatus = CMT2300A_STA_SLEEP;
            break;
        case CMT2300A_GO_STBY :
            nWaitStatus = CMT2300A_STA_STBY ;
            break;
        case CMT2300A_GO_TFS  :
            nWaitStatus = CMT2300A_STA_TFS  ;
            break;
        case CMT2300A_GO_TX   :
            nWaitStatus = CMT2300A_STA_TX   ;
            break;
        case CMT2300A_GO_RFS  :
            nWaitStatus = CMT2300A_STA_RFS  ;
            break;
        case CMT2300A_GO_RX   :
            nWaitStatus = CMT2300A_STA_RX   ;
            break;
    }

    CMT2300A_WriteReg(CMT2300A_CUS_MODE_CTL, nGoCmd);

    while (CMT2300A_GetTickCount() - nBegTick < 10)
    {
        CMT2300A_DelayUs(100);

        if (nWaitStatus == CMT2300A_GetChipStatus())
            return TRUE;

        if (CMT2300A_GO_TX == nGoCmd)
        {
            CMT2300A_DelayUs(100);

            if (CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))
                return TRUE;
        }

        if (CMT2300A_GO_RX == nGoCmd)
        {
            CMT2300A_DelayUs(100);

            if (CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG))
                return TRUE;
        }
    }

    return FALSE;
}
else
{
    CMT2300A_WriteReg(CMT2300A_CUS_MODE_CTL, nGoCmd);
    return TRUE;
}
    

//#else
//
//#endif
}

/*! ********************************************************
* @name    CMT2300A_GoSleep
* @desc    Entry SLEEP mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoSleep(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_SLEEP);
}

/*! ********************************************************
* @name    CMT2300A_GoStby
* @desc    Entry Sleep mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoStby(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_STBY);
}

/*! ********************************************************
* @name    CMT2300A_GoTFS
* @desc    Entry TFS mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoTFS(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_TFS);
}

/*! ********************************************************
* @name    CMT2300A_GoRFS
* @desc    Entry RFS mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoRFS(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_RFS);
}

/*! ********************************************************
* @name    CMT2300A_GoTx
* @desc    Entry Tx mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoTx(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_TX);
}

/*! ********************************************************
* @name    CMT2300A_GoRx
* @desc    Entry Rx mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoRx(void)
{
    return CMT2300A_AutoSwitchStatus(CMT2300A_GO_RX);
}

/*! ********************************************************
* @name    CMT2300A_ConfigGpio
* @desc    Config GPIO pins mode.
* @param   nGpioSel: GPIO1_SEL | GPIO2_SEL | GPIO3_SEL | GPIO4_SEL
*          GPIO1_SEL:
*            CMT2300A_GPIO1_SEL_DOUT/DIN
*            CMT2300A_GPIO1_SEL_INT1
*            CMT2300A_GPIO1_SEL_INT2
*            CMT2300A_GPIO1_SEL_DCLK
*
*          GPIO2_SEL:
*            CMT2300A_GPIO2_SEL_INT1
*            CMT2300A_GPIO2_SEL_INT2
*            CMT2300A_GPIO2_SEL_DOUT/DIN
*            CMT2300A_GPIO2_SEL_DCLK
*
*          GPIO3_SEL:
*            CMT2300A_GPIO3_SEL_CLKO
*            CMT2300A_GPIO3_SEL_DOUT/DIN
*            CMT2300A_GPIO3_SEL_INT2
*            CMT2300A_GPIO3_SEL_DCLK
*
*          GPIO4_SEL:
*            CMT2300A_GPIO4_SEL_RSTIN
*            CMT2300A_GPIO4_SEL_INT1
*            CMT2300A_GPIO4_SEL_DOUT
*            CMT2300A_GPIO4_SEL_DCLK
* *********************************************************/
void CMT2300A_ConfigGpio(u8 nGpioSel)
{
    CMT2300A_WriteReg(CMT2300A_CUS_IO_SEL, nGpioSel);
}

/*! ********************************************************
* @name    CMT2300A_ConfigInterrupt
* @desc    Config interrupt on INT1 and INT2.
* @param   nInt1Sel, nInt2Sel
*            CMT2300A_INT_SEL_RX_ACTIVE
*            CMT2300A_INT_SEL_TX_ACTIVE
*            CMT2300A_INT_SEL_RSSI_VLD
*            CMT2300A_INT_SEL_PREAM_OK
*            CMT2300A_INT_SEL_SYNC_OK
*            CMT2300A_INT_SEL_NODE_OK
*            CMT2300A_INT_SEL_CRC_OK
*            CMT2300A_INT_SEL_PKT_OK
*            CMT2300A_INT_SEL_SL_TMO
*            CMT2300A_INT_SEL_RX_TMO
*            CMT2300A_INT_SEL_TX_DONE
*            CMT2300A_INT_SEL_RX_FIFO_NMTY
*            CMT2300A_INT_SEL_RX_FIFO_TH
*            CMT2300A_INT_SEL_RX_FIFO_FULL
*            CMT2300A_INT_SEL_RX_FIFO_WBYTE
*            CMT2300A_INT_SEL_RX_FIFO_OVF
*            CMT2300A_INT_SEL_TX_FIFO_NMTY
*            CMT2300A_INT_SEL_TX_FIFO_TH
*            CMT2300A_INT_SEL_TX_FIFO_FULL
*            CMT2300A_INT_SEL_STATE_IS_STBY
*            CMT2300A_INT_SEL_STATE_IS_FS
*            CMT2300A_INT_SEL_STATE_IS_RX
*            CMT2300A_INT_SEL_STATE_IS_TX
*            CMT2300A_INT_SEL_LED
*            CMT2300A_INT_SEL_TRX_ACTIVE
*            CMT2300A_INT_SEL_PKT_DONE
* *********************************************************/
void CMT2300A_ConfigInterrupt(u8 nInt1Sel, u8 nInt2Sel)
{
    nInt1Sel &= CMT2300A_MASK_INT1_SEL;
    nInt1Sel |= (~CMT2300A_MASK_INT1_SEL) & CMT2300A_ReadReg(CMT2300A_CUS_INT1_CTL);
    CMT2300A_WriteReg(CMT2300A_CUS_INT1_CTL, nInt1Sel);

    nInt2Sel &= CMT2300A_MASK_INT2_SEL;
    nInt2Sel |= (~CMT2300A_MASK_INT2_SEL) & CMT2300A_ReadReg(CMT2300A_CUS_INT2_CTL);
    CMT2300A_WriteReg(CMT2300A_CUS_INT2_CTL, nInt2Sel);
}

/*! ********************************************************
* @name    CMT2300A_SetInterruptPolar
* @desc    Set the polarity of the interrupt.
* @param   bEnable(TRUE): active-high (default)
*          bEnable(FALSE): active-low
* *********************************************************/
void CMT2300A_SetInterruptPolar(BOOL bActiveHigh)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_INT1_CTL);

    if (bActiveHigh)
        tmp &= ~CMT2300A_MASK_INT_POLAR;
    else
        tmp |= CMT2300A_MASK_INT_POLAR;

    CMT2300A_WriteReg(CMT2300A_CUS_INT1_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_SetFifoThreshold
* @desc    Set FIFO threshold.
* @param   nFifoThreshold
* *********************************************************/
void CMT2300A_SetFifoThreshold(u8 nFifoThreshold)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_PKT29);

    tmp &= ~CMT2300A_MASK_FIFO_TH;
    tmp |= nFifoThreshold & CMT2300A_MASK_FIFO_TH;

    CMT2300A_WriteReg(CMT2300A_CUS_PKT29, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableAntennaSwitch
* @desc    Enable antenna switch, output TX_ACTIVE/RX_ACTIVE
*          via GPIO1/GPIO2.
* @param   nMode
*            0: RF_SWT1_EN=1, RF_SWT2_EN=0
*               GPIO1: RX_ACTIVE, GPIO2: TX_ACTIVE
*            1: RF_SWT1_EN=0, RF_SWT2_EN=1
*               GPIO1: RX_ACTIVE, GPIO2: ~RX_ACTIVE
* *********************************************************/
void CMT2300A_EnableAntennaSwitch(u8 nMode)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_INT1_CTL);

    if (0 == nMode)
    {
        tmp |= CMT2300A_MASK_RF_SWT1_EN;
        tmp &= ~CMT2300A_MASK_RF_SWT2_EN;
    }
    else if (1 == nMode)
    {
        tmp &= ~CMT2300A_MASK_RF_SWT1_EN;
        tmp |= CMT2300A_MASK_RF_SWT2_EN;
    }

    CMT2300A_WriteReg(CMT2300A_CUS_INT1_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableInterrupt
* @desc    Enable interrupt.
* @param   nEnable
*            CMT2300A_MASK_SL_TMO_EN   |
*            CMT2300A_MASK_RX_TMO_EN   |
*            CMT2300A_MASK_TX_DONE_EN  |
*            CMT2300A_MASK_PREAM_OK_EN |
*            CMT2300A_MASK_SYNC_OK_EN  |
*            CMT2300A_MASK_NODE_OK_EN  |
*            CMT2300A_MASK_CRC_OK_EN   |
*            CMT2300A_MASK_PKT_DONE_EN
* *********************************************************/
void CMT2300A_EnableInterrupt(u8 nEnable)
{
    CMT2300A_WriteReg(CMT2300A_CUS_INT_EN, nEnable);
}

/*! ********************************************************
* @name    CMT2300A_EnableRxFifoAutoClear
* @desc    Auto clear Rx FIFO before entry Rx mode.
* @param   bEnable(TRUE): Enable it(default)
*          bEnable(FALSE): Disable it
* *********************************************************/
void CMT2300A_EnableRxFifoAutoClear(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);

    if (bEnable)
        tmp &= ~CMT2300A_MASK_FIFO_AUTO_CLR_DIS;
    else
        tmp |= CMT2300A_MASK_FIFO_AUTO_CLR_DIS;

    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableFifoMerge
* @desc    Enable FIFO merge.
* @param   bEnable(TRUE): use a single 64-byte FIFO for either Tx or Rx
*          bEnable(FALSE): use a 32-byte FIFO for Tx and another 32-byte FIFO for Rx(default)
* *********************************************************/
void CMT2300A_EnableFifoMerge(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);

    if (bEnable)
        tmp |= CMT2300A_MASK_FIFO_MERGE_EN;
    else
        tmp &= ~CMT2300A_MASK_FIFO_MERGE_EN;

    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableReadFifo
* @desc    Enable SPI to read the FIFO.
* *********************************************************/
void CMT2300A_EnableReadFifo(void)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);
    tmp &= ~CMT2300A_MASK_SPI_FIFO_RD_WR_SEL;
    tmp &= ~CMT2300A_MASK_FIFO_RX_TX_SEL;
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableWriteFifo
* @desc    Enable SPI to write the FIFO.
* *********************************************************/
void CMT2300A_EnableWriteFifo(void)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);
    tmp |= CMT2300A_MASK_SPI_FIFO_RD_WR_SEL;
    tmp |= CMT2300A_MASK_FIFO_RX_TX_SEL;
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_RestoreFifo
* @desc    Restore the FIFO.
* *********************************************************/
void CMT2300A_RestoreFifo(void)
{
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_RESTORE);
}

/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Tx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
u8 CMT2300A_ClearTxFifo(void)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_FLAG);
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_TX);
    return tmp;
}

/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Rx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
u8 CMT2300A_ClearRxFifo(void)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_FLAG);
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_RX);
    return tmp;
}

/*! ********************************************************
* @name    CMT2300A_ClearInterruptFlags
* @desc    Clear all interrupt flags.
* @return  Some interrupt flags
*            CMT2300A_MASK_SL_TMO_EN    |
*            CMT2300A_MASK_RX_TMO_EN    |
*            CMT2300A_MASK_TX_DONE_EN   |
*            CMT2300A_MASK_PREAM_OK_FLG |
*            CMT2300A_MASK_SYNC_OK_FLG  |
*            CMT2300A_MASK_NODE_OK_FLG  |
*            CMT2300A_MASK_CRC_OK_FLG   |
*            CMT2300A_MASK_PKT_OK_FLG
* *********************************************************/
u8 CMT2300A_ClearInterruptFlags(void)
{
    u8 nFlag1, nFlag2;
    u8 nClr1 = 0;
    u8 nClr2 = 0;
    u8 nRet  = 0;
    u8 nIntPolar;

    nIntPolar = CMT2300A_ReadReg(CMT2300A_CUS_INT1_CTL);
    nIntPolar = (nIntPolar & CMT2300A_MASK_INT_POLAR) ? 1 : 0;

    nFlag1 = CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG);
    nFlag2 = CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1);

    if (nIntPolar)
    {
        /* Interrupt flag active-low */
        nFlag1 = ~nFlag1;
        nFlag2 = ~nFlag2;
    }

    if (CMT2300A_MASK_LBD_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_LBD_CLR;         /* Clear LBD_FLG */
    }

    if (CMT2300A_MASK_COL_ERR_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;    /* Clear COL_ERR_FLG by PKT_DONE_CLR */
    }

    if (CMT2300A_MASK_PKT_ERR_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;    /* Clear PKT_ERR_FLG by PKT_DONE_CLR */
    }

    if (CMT2300A_MASK_PREAM_OK_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_PREAM_OK_CLR;    /* Clear PREAM_OK_FLG */
        nRet  |= CMT2300A_MASK_PREAM_OK_FLG;    /* Return PREAM_OK_FLG */
    }

    if (CMT2300A_MASK_SYNC_OK_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_SYNC_OK_CLR;    /* Clear SYNC_OK_FLG */
        nRet  |= CMT2300A_MASK_SYNC_OK_FLG;    /* Return SYNC_OK_FLG */
    }

    if (CMT2300A_MASK_NODE_OK_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_NODE_OK_CLR;    /* Clear NODE_OK_FLG */
        nRet  |= CMT2300A_MASK_NODE_OK_FLG;    /* Return NODE_OK_FLG */
    }

    if (CMT2300A_MASK_CRC_OK_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_CRC_OK_CLR;    /* Clear CRC_OK_FLG */
        nRet  |= CMT2300A_MASK_CRC_OK_FLG;    /* Return CRC_OK_FLG */
    }

    if (CMT2300A_MASK_PKT_OK_FLG & nFlag1)
    {
        nClr2 |= CMT2300A_MASK_PKT_DONE_CLR;  /* Clear PKT_OK_FLG */
        nRet  |= CMT2300A_MASK_PKT_OK_FLG;    /* Return PKT_OK_FLG */
    }

    if (CMT2300A_MASK_SL_TMO_FLG & nFlag2)
    {
        nClr1 |= CMT2300A_MASK_SL_TMO_CLR;    /* Clear SL_TMO_FLG */
        nRet  |= CMT2300A_MASK_SL_TMO_EN;     /* Return SL_TMO_FLG by SL_TMO_EN */
    }

    if (CMT2300A_MASK_RX_TMO_FLG & nFlag2)
    {
        nClr1 |= CMT2300A_MASK_RX_TMO_CLR;    /* Clear RX_TMO_FLG */
        nRet  |= CMT2300A_MASK_RX_TMO_EN;     /* Return RX_TMO_FLG by RX_TMO_EN */
    }

    if (CMT2300A_MASK_TX_DONE_FLG & nFlag2)
    {
        nClr1 |= CMT2300A_MASK_TX_DONE_CLR;   /* Clear TX_DONE_FLG */
        nRet  |= CMT2300A_MASK_TX_DONE_EN;    /* Return TX_DONE_FLG by TX_DONE_EN */
    }

    CMT2300A_WriteReg(CMT2300A_CUS_INT_CLR1, nClr1);
    CMT2300A_WriteReg(CMT2300A_CUS_INT_CLR2, nClr2);

    if (nIntPolar)
    {
        /* Interrupt flag active-low */
        nRet = ~nRet;
    }

    return nRet;
}

/*! ********************************************************
* @name    CMT2300A_ConfigTxDin
* @desc    Used to select whether to use GPIO1 or GPIO2 or GPIO3
*          as DIN in the direct mode. It only takes effect when
*          call CMT2300A_EnableTxDin(TRUE) in the direct mode.
* @param   nDinSel
*            CMT2300A_TX_DIN_SEL_GPIO1
*            CMT2300A_TX_DIN_SEL_GPIO2
*            CMT2300A_TX_DIN_SEL_GPIO3
* *********************************************************/
void CMT2300A_ConfigTxDin(u8 nDinSel)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);
    tmp &= ~CMT2300A_MASK_TX_DIN_SEL;
    tmp |= nDinSel;
    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableTxDin
* @desc    Used to change GPIO1/GPIO2/GPIO3 between DOUT and DIN.
* @param   bEnable(TRUE): used as DIN
*          bEnable(FALSE): used as DOUT(default)
* *********************************************************/
void CMT2300A_EnableTxDin(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FIFO_CTL);

    if (bEnable)
        tmp |= CMT2300A_MASK_TX_DIN_EN;
    else
        tmp &= ~CMT2300A_MASK_TX_DIN_EN;

    CMT2300A_WriteReg(CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableTxDinInvert
* @desc    Used to invert DIN data in direct mode.
* @param   bEnable(TRUE): invert DIN
*          bEnable(FALSE): not invert DIN(default)
* *********************************************************/
void CMT2300A_EnableTxDinInvert(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_INT2_CTL);

    if (bEnable)
        tmp |= CMT2300A_MASK_TX_DIN_INV;
    else
        tmp &= ~CMT2300A_MASK_TX_DIN_INV;

    CMT2300A_WriteReg(CMT2300A_CUS_INT2_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_IsExist
* @desc    Chip indentify.
* @return  TRUE: chip is exist, FALSE: chip not found
* *********************************************************/
BOOL CMT2300A_IsExist(void)
{
    u8 back, dat;

    back = CMT2300A_ReadReg(CMT2300A_CUS_PKT17);
    CMT2300A_WriteReg(CMT2300A_CUS_PKT17, 0xAA);

    dat = CMT2300A_ReadReg(CMT2300A_CUS_PKT17);
    CMT2300A_WriteReg(CMT2300A_CUS_PKT17, back);

    if (0xAA == dat)
        return TRUE;
    else
        return FALSE;
}

/*! ********************************************************
* @name    CMT2300A_GetRssiCode
* @desc    Get RSSI code.
* @return  RSSI code
* *********************************************************/
u8 CMT2300A_GetRssiCode(void)
{
    return CMT2300A_ReadReg(CMT2300A_CUS_RSSI_CODE);
}

/*! ********************************************************
* @name    CMT2300A_GetRssiDBm
* @desc    Get RSSI dBm.
* @return  dBm
* *********************************************************/
int CMT2300A_GetRssiDBm(void)
{
    return (int)CMT2300A_ReadReg(CMT2300A_CUS_RSSI_DBM) - 128;
}

/*! ********************************************************
* @name    CMT2300A_SetFrequencyChannel
* @desc    This defines up to 255 frequency channel
*          for fast frequency hopping operation.
* @param   nChann: the frequency channel
* *********************************************************/
void CMT2300A_SetFrequencyChannel(u8 nChann)
{
    CMT2300A_WriteReg(CMT2300A_CUS_FREQ_CHNL, nChann);
}

/*! ********************************************************
* @name    CMT2300A_SetFrequencyStep
* @desc    This defines the frequency channel step size
*          for fast frequency hopping operation.
*          One step size is 2.5 kHz.
* @param   nOffset: the frequency step
* *********************************************************/
void CMT2300A_SetFrequencyStep(u8 nOffset)
{
    CMT2300A_WriteReg(CMT2300A_CUS_FREQ_OFS, nOffset);
}

/*! ********************************************************
* @name    CMT2300A_SetPayloadLength
* @desc    Set payload length.
* @param   nLength
* *********************************************************/
void CMT2300A_SetPayloadLength(u16 nLength)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_PKT14);

    tmp &= ~CMT2300A_MASK_PAYLOAD_LENG_10_8;
    tmp |= (nLength >> 4) & CMT2300A_MASK_PAYLOAD_LENG_10_8;
    CMT2300A_WriteReg(CMT2300A_CUS_PKT14, tmp);

    tmp = nLength & CMT2300A_MASK_PAYLOAD_LENG_7_0;
    CMT2300A_WriteReg(CMT2300A_CUS_PKT15, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableLfosc
* @desc    If you need use sleep timer, you should enable LFOSC.
* @param   bEnable(TRUE): Enable it(default)
*          bEnable(FALSE): Disable it
* *********************************************************/
void CMT2300A_EnableLfosc(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_SYS2);

    if (bEnable)
    {
        tmp |= CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL2_EN;
    }
    else
    {
        tmp &= ~CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL2_EN;
    }

    CMT2300A_WriteReg(CMT2300A_CUS_SYS2, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableLfoscOutput
* @desc    LFOSC clock is output via GPIO3.
* @param   bEnable(TRUE): Enable it
*          bEnable(FALSE): Disable it(default)
* *********************************************************/
void CMT2300A_EnableLfoscOutput(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_INT2_CTL);

    if (bEnable)
        tmp |= CMT2300A_MASK_LFOSC_OUT_EN;
    else
        tmp &= ~CMT2300A_MASK_LFOSC_OUT_EN;

    CMT2300A_WriteReg(CMT2300A_CUS_INT2_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableAfc
* @desc    AFC enable or disanble.
* @param   bEnable(TRUE): Enable it
*          bEnable(FALSE): Disable it(default)
* *********************************************************/
void CMT2300A_EnableAfc(BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(CMT2300A_CUS_FSK5);

    if (bEnable)
        tmp |= 0x10;
    else
        tmp &= ~0x10;

    CMT2300A_WriteReg(CMT2300A_CUS_FSK5, tmp);
}

/*! ********************************************************
* @name    CMT2300A_SetAfcOvfTh
* @desc    This is optional, only needed when using Rx fast frequency hopping.
* @param   afcOvfTh: AFC_OVF_TH see AN142 and AN197 for details.
* *********************************************************/
void CMT2300A_SetAfcOvfTh(u8 afcOvfTh)
{
    CMT2300A_WriteReg(CMT2300A_CUS_FSK4, afcOvfTh);
}

/*! ********************************************************
* @name    CMT2300A_Init
* @desc    Initialize chip status.
* *********************************************************/
void CMT2300A_Init(void)
{
    u8 tmp;

    CMT2300A_SoftReset();
    CMT2300A_DelayMs(20);

    CMT2300A_GoStby();

    tmp  = CMT2300A_ReadReg(CMT2300A_CUS_MODE_STA);
    tmp |= CMT2300A_MASK_CFG_RETAIN;         /* Enable CFG_RETAIN */
    tmp &= ~CMT2300A_MASK_RSTN_IN_EN;        /* Disable RSTN_IN */
    CMT2300A_WriteReg(CMT2300A_CUS_MODE_STA, tmp);

    tmp  = CMT2300A_ReadReg(CMT2300A_CUS_EN_CTL);
    tmp |= CMT2300A_MASK_LOCKING_EN;         /* Enable LOCKING_EN */
    CMT2300A_WriteReg(CMT2300A_CUS_EN_CTL, tmp);

    CMT2300A_EnableLfosc(FALSE);             /* Diable LFOSC */

    CMT2300A_ClearInterruptFlags();
}

/*! ********************************************************
* @name    CMT2300A_ConfigRegBank
* @desc    Config one register bank.
* *********************************************************/
BOOL CMT2300A_ConfigRegBank(u8 base_addr, const u8 bank[], u8 len)
{
    u8 i;
    for (i = 0; i < len; i++)
        CMT2300A_WriteReg(i + base_addr, bank[i]);

    return TRUE;
}

//******************end cmt2300a**************************************

//*********************************radio.c***********************************
/* RF state machine */
typedef enum
{
    RF_STATE_IDLE = 0,
    RF_STATE_RX_START,
    RF_STATE_RX_WAIT,
    RF_STATE_RX_DONE,
    RF_STATE_RX_TIMEOUT,
    RF_STATE_TX_START,
    RF_STATE_TX_WAIT,
    RF_STATE_TX_DONE,
    RF_STATE_TX_TIMEOUT,
    RF_STATE_ERROR,
} EnumRFStatus;

/* RF process function results */
typedef enum
{
    RF_IDLE = 0,
    RF_BUSY,
    RF_RX_DONE,
    RF_RX_TIMEOUT,
    RF_TX_DONE,
    RF_TX_TIMEOUT,
    RF_ERROR,
} EnumRFResult;

/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, ** SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * *** (C) ** SZ.
 */

/*!
 * @file    radio.c
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @**  ** **
 */






static EnumRFStatus g_nNextRFState = RF_STATE_IDLE;
static u8 *g_pRxBuffer = NULL;
static u8 *g_pTxBuffer = NULL;
static u16 g_nRxLength = 0;
static u16 g_nTxLength = 0;

static u32 g_nRxTimeout = INFINITE;
static u32 g_nTxTimeout = INFINITE;
static u32 g_nRxTimeCount = 0;
static u32 g_nTxTimeCount = 0;

static u8 g_nInterrutFlags = 0;

void RF_Init(void)
{
    u8 tmp;

    CMT2300A_InitGpio();//配置io中断
    CMT2300A_Init();

    /* Config registers */
    CMT2300A_ConfigRegBank(CMT2300A_CMT_BANK_ADDR, g_cmt2300aCmtBank, CMT2300A_CMT_BANK_SIZE);
    CMT2300A_ConfigRegBank(CMT2300A_SYSTEM_BANK_ADDR, g_cmt2300aSystemBank, CMT2300A_SYSTEM_BANK_SIZE);
    CMT2300A_ConfigRegBank(CMT2300A_FREQUENCY_BANK_ADDR, g_cmt2300aFrequencyBank, CMT2300A_FREQUENCY_BANK_SIZE);
    CMT2300A_ConfigRegBank(CMT2300A_DATA_RATE_BANK_ADDR, g_cmt2300aDataRateBank, CMT2300A_DATA_RATE_BANK_SIZE);
    CMT2300A_ConfigRegBank(CMT2300A_BASEBAND_BANK_ADDR, g_cmt2300aBasebandBank, CMT2300A_BASEBAND_BANK_SIZE);
    CMT2300A_ConfigRegBank(CMT2300A_TX_BANK_ADDR, g_cmt2300aTxBank, CMT2300A_TX_BANK_SIZE);

    // xosc_aac_code[2:0] = 2
    tmp = (~0x07) & CMT2300A_ReadReg(CMT2300A_CUS_CMT10);
    CMT2300A_WriteReg(CMT2300A_CUS_CMT10, tmp | 0x02);

    RF_Config();
}

void RF_Config(void)
{
#ifdef ENABLE_ANTENNA_SWITCH
    /* If you enable antenna switch, GPIO1/GPIO2 will output RX_ACTIVE/TX_ACTIVE,
       and it can't output INT1/INT2 via GPIO1/GPIO2 */
    CMT2300A_EnableAntennaSwitch(0);

#else
    /* Config GPIOs */
    CMT2300A_ConfigGpio(
        CMT2300A_GPIO1_SEL_INT1 | /* INT1 > GPIO1 */
        CMT2300A_GPIO2_SEL_INT2 | /* INT2 > GPIO2 */
        CMT2300A_GPIO3_SEL_DOUT
    );

    /* Config interrupt */
    CMT2300A_ConfigInterrupt(
        CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
        CMT2300A_INT_SEL_PKT_OK   /* Config INT2 */
    );
#endif

    /* Enable interrupt */
    CMT2300A_EnableInterrupt(
        CMT2300A_MASK_TX_DONE_EN  |
        CMT2300A_MASK_PREAM_OK_EN |
        CMT2300A_MASK_SYNC_OK_EN  |
        CMT2300A_MASK_NODE_OK_EN  |
        CMT2300A_MASK_CRC_OK_EN   |
        CMT2300A_MASK_PKT_DONE_EN
    );

    /* Disable low frequency OSC calibration */
    CMT2300A_EnableLfosc(FALSE);

    /* Use a single 64-byte FIFO for either Tx or Rx */
    //CMT2300A_EnableFifoMerge(TRUE);

    //CMT2300A_SetFifoThreshold(16);

    /* This is optional, only needed when using Rx fast frequency hopping */
    /* See AN142 and AN197 for details */
    //CMT2300A_SetAfcOvfTh(0x27);

    /* Go to sleep for configuration to take effect */
    CMT2300A_GoSleep();
}

void RF_SetStatus(EnumRFStatus nStatus)
{
    g_nNextRFState = nStatus;
}

EnumRFStatus RF_GetStatus(void)
{
    return g_nNextRFState;
}

u8 RF_GetInterruptFlags(void)
{
    return g_nInterrutFlags;
}

void RF_StartRx(u8 buf[], u16 len, u32 timeout)
{
    g_pRxBuffer = buf;
    g_nRxLength = len;
    g_nRxTimeout = timeout;

    memset(g_pRxBuffer, 0, g_nRxLength);

    g_nNextRFState = RF_STATE_RX_START;
}

void RF_StartTx(u8 buf[], u16 len, u32 timeout)
{
    g_pTxBuffer = buf;
    g_nTxLength = len;
    g_nTxTimeout = timeout;

    g_nNextRFState = RF_STATE_TX_START;
}

EnumRFResult RF_Process(void)
{
    EnumRFResult nRes = RF_BUSY;

    switch (g_nNextRFState)
    {
        case RF_STATE_IDLE:
        {
            nRes = RF_IDLE;
            break;
        }

        case RF_STATE_RX_START:
        {
            CMT2300A_GoStby();
            CMT2300A_ClearInterruptFlags();

            /* Must clear FIFO after enable SPI to read or write the FIFO */
            CMT2300A_EnableReadFifo();
            CMT2300A_ClearRxFifo();

            if (FALSE == CMT2300A_GoRx())
                g_nNextRFState = RF_STATE_ERROR;
            else
                g_nNextRFState = RF_STATE_RX_WAIT;

            g_nRxTimeCount = CMT2300A_GetTickCount();

            break;
        }

        case RF_STATE_RX_WAIT:
        {
#ifdef ENABLE_ANTENNA_SWITCH
            if (CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG)) /* Read PKT_OK flag */
#else
            if (CMT2300A_ReadGpio2()) /* Read INT2, PKT_OK */
#endif
            {
                g_nNextRFState = RF_STATE_RX_DONE;
            }

            if ((INFINITE != g_nRxTimeout) && ((g_nSysTickCount - g_nRxTimeCount) > g_nRxTimeout))
                g_nNextRFState = RF_STATE_RX_TIMEOUT;

            break;
        }

        case RF_STATE_RX_DONE:
        {
            CMT2300A_GoStby();

            /* The length need be smaller than 32 */
            CMT2300A_ReadFifo(g_pRxBuffer, g_nRxLength);

            g_nInterrutFlags = CMT2300A_ClearInterruptFlags();

            CMT2300A_GoSleep();

            g_nNextRFState = RF_STATE_IDLE;
            nRes = RF_RX_DONE;
            break;
        }

        case RF_STATE_RX_TIMEOUT:
        {
            CMT2300A_GoSleep();

            g_nNextRFState = RF_STATE_IDLE;
            nRes = RF_RX_TIMEOUT;
            break;
        }

        case RF_STATE_TX_START:
        {
            CMT2300A_GoStby();
            CMT2300A_ClearInterruptFlags();

            /* Must clear FIFO after enable SPI to read or write the FIFO */
            CMT2300A_EnableWriteFifo();
            CMT2300A_ClearTxFifo();

            /* The length need be smaller than 32 */
            CMT2300A_WriteFifo(g_pTxBuffer, g_nTxLength);

            if (0 == (CMT2300A_MASK_TX_FIFO_NMTY_FLG & CMT2300A_ReadReg(CMT2300A_CUS_FIFO_FLAG)))
                g_nNextRFState = RF_STATE_ERROR;

            if (FALSE == CMT2300A_GoTx())
                g_nNextRFState = RF_STATE_ERROR;
            else
                g_nNextRFState = RF_STATE_TX_WAIT;

            g_nTxTimeCount = CMT2300A_GetTickCount();

            break;
        }

        case RF_STATE_TX_WAIT:
        {
#ifdef ENABLE_ANTENNA_SWITCH
            if (CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1)) /* Read TX_DONE flag */
#else
            if (CMT2300A_ReadGpio1()) /* Read INT1, TX_DONE */
#endif
            {
                g_nNextRFState = RF_STATE_TX_DONE;
            }

            if ((INFINITE != g_nTxTimeout) && ((g_nSysTickCount - g_nTxTimeCount) > g_nTxTimeout))
                g_nNextRFState = RF_STATE_TX_TIMEOUT;

            break;
        }

        case RF_STATE_TX_DONE:
        {
            CMT2300A_ClearInterruptFlags();
            CMT2300A_GoSleep();

            g_nNextRFState = RF_STATE_IDLE;
            nRes = RF_TX_DONE;
            break;
        }

        case RF_STATE_TX_TIMEOUT:
        {
            CMT2300A_GoSleep();

            g_nNextRFState = RF_STATE_IDLE;
            nRes = RF_TX_TIMEOUT;
            break;
        }

        case RF_STATE_ERROR:
        {
            CMT2300A_SoftReset();
            CMT2300A_DelayMs(20);

            CMT2300A_GoStby();
            RF_Config();

            g_nNextRFState = RF_STATE_IDLE;
            nRes = RF_ERROR;
            break;
        }

        default:
            break;
    }

    return nRes;
}



//******************************  end radio.c********************************
#define RF_PACKET_SIZE 6               /* Define the payload size here */

static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

static BOOL g_bEnableMaster = TRUE;     /* Master/Slave selection */

static u16 g_nRecvCount = 0;
static u16 g_nSendCount = 0;
static u16 g_nErrCount  = 0;



/* Manages the master operation */
void OnMaster(void)
{
    char str[32];

    switch (RF_Process())
    {
        case RF_IDLE:
        {
            g_nSendCount++;
            g_txBuffer[0] = (u8)g_nSendCount;
            g_txBuffer[1] = g_nSendCount >> 8;

            RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);//发送数据
            break;
        }

        case RF_TX_DONE:
        {


            RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, 1000);
            break;
        }

        case RF_RX_DONE:
        {

            g_nRecvCount++;


            break;
        }

        case RF_RX_TIMEOUT:
        {

            break;
        }

        case RF_ERROR:
        {


            break;
        }

        default:
            break;
    }
}

/* Manages the slave operation */
void OnSlave(void)
{
    char str[32];

    switch (RF_Process())
    {
        case RF_IDLE:
        {
            RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, INFINITE);
            break;
        }

        case RF_RX_DONE:
        {

            g_nRecvCount++;

            CMT2300A_DelayMs(10);

            g_nSendCount++;
            g_txBuffer[0] = (u8)g_nSendCount;
            g_txBuffer[1] = g_nSendCount >> 8;

            RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);

            break;
        }

        case RF_TX_DONE:
        {


            break;
        }

        case RF_ERROR:
        {


            break;
        }

        default:
            break;
    }
}

/* Main application entry point */
int main(void)
{
    int i;

    for (i = 0; i < RF_PACKET_SIZE; i++)
        g_txBuffer[i] = i + 1;

    RF_Init();

    if (FALSE == CMT2300A_IsExist())
    {
        while (1);
    }

    while (1)
    {
        if (TRUE == g_bEnableMaster)
        {
            OnMaster();//主动发射，发射完成后进入接受模式
        }
        else
        {
            OnSlave();//主动接收，接收完成后进入发射模式
        }
    }
}

