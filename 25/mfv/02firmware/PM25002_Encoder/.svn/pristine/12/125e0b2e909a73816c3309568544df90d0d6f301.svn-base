#ifndef __DNSENGINE_H_
#define __DNSENGINE_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define DNS_FS_COUNTS 0x6000
#define DNS_FS_VOLTS  24.0f

#define DNS_EMFC_OBJECT_SUPERVISOR_MAX_INST           1
#define DNS_EMFC_OBJECT_S_ANALOG_SENSOR_MAX_INST      3
#define DNS_EMFC_OBJECT_S_ANALOG_ACTUATOR_MAX_INST    1
#define DNS_EMFC_OBJECT_S_STAGE_CONTROLLER_MAX_INST   1
#define DNS_EMFC_OBJECT_S_GAS_CALIBRATION_MAX_INST    6

#define WARNING_LOW   0x08
#define WARNING_HIGH  0x04
#define ALARM_LOW     0x02
#define ALARM_HIGH    0x01

#define WARNING_SSC   0x02
#define ALARM_SSC     0x01

#define ALARM_DEV_COMMON      0x01
#define ALARM_DEV_SPECIFIC    0x02
#define ALARM_MAN_SPECIFC     0x04
#define WARN_DEV_COMMON       0x10
#define WARN_DEV_SPECIFIC     0x20
#define WARN_MAN_SPECIFC      0x40

#define EXPANDED_METHOD 0x80

#define BIT0          0x01
#define BIT1          0x02
#define BIT2          0x04
#define BIT3          0x08
#define BIT4          0x10
#define BIT5          0x20
#define BIT6          0x40
#define BIT7          0x80

typedef enum {
 SELF_TEST           = 1,
 IDLE_STATUS         = 2,
 EXECUTING_STATUS    = 4,
 ABORT_STATUS        = 5,  
} device_state;

#pragma pack(push,1)
typedef struct {
  uint8_t commonExceptionSize;
  uint8_t commonException[2];
  uint8_t detailedExceptionSize;
  uint8_t detailedException[2];
  uint8_t manufacturerExceptionSize;
  uint8_t manufacturerException;
} S_DEVICE_SUPERVISOR_OBJECT_EXCEPTION_T;

typedef struct {
  uint8_t dStatus;
  uint8_t eStatus;
  device_state devstate;
  S_DEVICE_SUPERVISOR_OBJECT_EXCEPTION_T aException;
  S_DEVICE_SUPERVISOR_OBJECT_EXCEPTION_T wException;
} dns_struct;
#pragma pack(pop)

extern dns_struct dns;

extern uint8_t dns_update_objects(void);

#endif
