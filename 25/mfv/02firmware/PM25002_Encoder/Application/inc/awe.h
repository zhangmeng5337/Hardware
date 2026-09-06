#ifndef __AWE_H_
#define __AWE_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define TEMPERATURE_OUT_OF_RANGE_HIGH         0x00000001
#define TEMPERATURE_OUT_OF_RANGE_LOW          0x00000002
#define PRESSURE_OUT_OF_RANGE_HIGH            0x00000004
#define PRESSURE_OUT_OF_RANGE_LOW             0x00000008
#define IWDG_MCU_TIMEOUT                      0x00000010
#define RTOS_FAILTURE                         0x00000020
#define RAMP_DATA_INVALID                     0x00000040
#define FLOW_SENSOR_ERROR                     0x00000080

//#define DP_PRESSURE_LIMIT_HIGH                0x00000080
#define DPPRESSURE_OUT_OF_RANGE_HIGH          0x00000100
#define DPPRESSURE_OUT_OF_RANGE_LOW           0x00000200
#define DP_CALCULATION_ERROR                  0x00000400
#define PA_CALCULATION_ERROR                  0x00000800

#define DP_ZERO_LIMIT_HIGH                    0x00001000
#define DP_ZERO_LIMIT_LOW                     0x00002000

#define DP_RAW_OUT_OF_RANGE_HIGH              0x00004000
#define DP_RAW_OUT_OF_RANGE_LOW               0x00008000
#define PA_RAW_OUT_OF_RANGE_HIGH              0x00010000
#define PA_RAW_OUT_OF_RANGE_LOW               0x00020000
#define DPT_RAW_OUT_OF_RANGE_HIGH             0x00040000
#define DPT_RAW_OUT_OF_RANGE_LOW              0x00080000
#define PAT_RAW_OUT_OF_RANGE_HIGH             0x00100000
#define PAT_RAW_OUT_OF_RANGE_LOW              0x00200000

//#define VALVE_MALFUNCTION                     0x00000010
//#define RAMP_ERROR                            0x00000040
//#define FLOW_OUT_OF_RANGE_HIGH                0x00000001
//#define FLOW_OUT_OF_RANGE_LOW                 0x00000002
//#define TEMPERATURE_SENSOR_ERROR              0x00200000
//#define PRESSURE_SENSOR_ERROR                 0x00400000
//#define FLOW_SENSOR_ERROR                     0x00800000
//#define INDENTITY_INFORMATION_MISMATCH        0x02000000

#define SELECTED_CALIBRATION_INVALID          0x04000000
#define NETWORK_INTERFACE_FAILURE             0x08000000
#define NVMEM_PAGEVALIDATE_ERROR              0x10000000
#define NVMEM_REDPAGEVALIDATE_ERROR           0x20000000
#define NVMEM_FAILURE                         0x40000000
//#define SENSOR_FAILURE                        0x80000000


#define AWE_DEVICE_WARNING                    0x01
#define AWE_DEVICE_ERROR                      0x04
#define AWE_MANUFACTURER_WARNING              0x02
#define AWE_MANUFACTURING_ERROR               0x08

#define AWE_BASE_ERROR                        100

typedef struct {
  uint32_t warning;
  uint32_t error;
} aw_struct;

typedef struct {
  uint8_t   status;
  aw_struct device;
  aw_struct manufacturer;
  aw_struct device_latched;
  aw_struct manufacturer_latched;
} awe_struct;

extern awe_struct awe;
extern void bitcheck_awe(uint32_t data, uint32_t limit, uint32_t flag, uint32_t *dst);
extern void awe_engine(void);
#endif
