/**
    @filename   dnsengine.c
    @brief      devicenet related updaes via co-processor should be placed in this file.
*/
#include "dnsengine.h"
#include "meter.h"
#include "controller.h"

dns_struct dns = {
.devstate = EXECUTING_STATUS,
.wException.commonExceptionSize = 2,
.wException.detailedExceptionSize = 2,
.wException.manufacturerExceptionSize = 1, 
.aException.commonExceptionSize = 2,
.aException.detailedExceptionSize = 2,
.aException.manufacturerExceptionSize = 1, 
};

/**
  @brief Generic warning/alarm handler
*/
uint8_t wae(uint8_t en, float in, float hyst, float tpl, float tph, uint16_t *timer, uint16_t settime, uint8_t lo, uint8_t hi, uint8_t *status) {
  uint8_t valid = 1;
  if (en == 1) { // check if warning feature is enabled
    if ((in - hyst) < tpl) {
      if (*timer > settime) {
        *status |= lo;
      } else {
       *status &= ~lo;
        *timer+=5;  // 5 increments as our signal processing rate is 200Hz and settling time is in ms
      }
    } else if ((in + hyst) > tph) {
      if (*timer > settime) {
        *status |= hi;
      } else {
        *status &= ~hi;
        *timer+=5;  // 5 increments as our signal processing rate is 200Hz and settling time is in ms
      }
    } else {
      *status &= ~(lo|hi);
      *timer = 0;
    }
  } else {
    *status &= ~(lo|hi);
  }
  
  return valid;
}

/**
  @brief general bit masker for alarms and warning
*/
void awbitprocessor(uint8_t flag, uint8_t *in, uint8_t mask) {
  if (flag) {
    *in |= mask;
  } else {
    *in &= ~mask;
  }
}

/**
  @brief Supervisor Object updates
*/
uint8_t update_sup(void) {
  uint8_t results = 0;
  uint8_t status = EXPANDED_METHOD;
  
  dns.dStatus = dns.devstate;   

  // update exception status bit map
  if (1 == nvmem.app.supObj.wEnable) {
    dns.wException.commonException[0] = 0;
    dns.wException.commonException[1] = 0;
    
    // bit0: Reading not valid sas[0]
    awbitprocessor((meter.dnet_sas[0].valid == 0), 
                   &dns.wException.detailedException[0],
                   BIT0);    
    // bit1: TPL sas[0]
    awbitprocessor(meter.dnet_sas[0].status & WARNING_LOW, 
                   &dns.wException.detailedException[0],
                   BIT1);
    // bit2: TPH sas[0]
    awbitprocessor(meter.dnet_sas[0].status & WARNING_HIGH, 
                   &dns.wException.detailedException[0],
                   BIT2);
    // bit3: ssc
    awbitprocessor(ctrl.dnet_scc.status & WARNING_SSC, 
                   &dns.wException.detailedException[0],
                   BIT3);
    // bit4: TPL saa
    awbitprocessor(ctrl.dnet_saa.status & WARNING_LOW, 
                   &dns.wException.detailedException[0],
                   BIT4);    
    // bit5: TPH saa
    awbitprocessor(ctrl.dnet_saa.status & WARNING_HIGH, 
                   &dns.wException.detailedException[0],
                   BIT5);

    // bit0: TPL sas[1]
    awbitprocessor(meter.dnet_sas[1].status & WARNING_LOW, 
                   &dns.wException.detailedException[1],
                   BIT0);
    // bit1: TPH sas[1]
    awbitprocessor(meter.dnet_sas[1].status & WARNING_HIGH, 
                   &dns.wException.detailedException[1],
                   BIT1);
    // bit2: TPL sas[2]
    awbitprocessor(meter.dnet_sas[2].status & WARNING_LOW, 
                   &dns.wException.detailedException[1],
                   BIT2);
    // bit3: TPH sas[2]
    awbitprocessor(meter.dnet_sas[2].status & WARNING_HIGH, 
                   &dns.wException.detailedException[1],
                   BIT3);
    // bit4: Reading not valid sas[1]
    awbitprocessor((meter.dnet_sas[1].valid == 0), 
                   &dns.wException.detailedException[1],
                   BIT4);   
    // bit5: Reading not valid sas[2]
    awbitprocessor((meter.dnet_sas[2].valid == 0), 
                   &dns.wException.detailedException[1],
                   BIT5);

    dns.wException.manufacturerException = 0;
    
  } else {
    dns.wException.commonException[0] = 0;
    dns.wException.commonException[1] = 0;
    dns.wException.detailedException[0] = 0;
    dns.wException.detailedException[1] = 0;
    dns.wException.manufacturerException = 0;
  }  
  
  if (1 == nvmem.app.supObj.aEnable) {
    dns.aException.commonException[0] = 0;
    dns.aException.commonException[1] = 0;
    
    // bit0: Reading not valid sas[0]
    awbitprocessor((meter.dnet_sas[0].valid == 0), 
                   &dns.aException.detailedException[0],
                   BIT0);    
    // bit1: TPL sas[0]
    awbitprocessor(meter.dnet_sas[0].status & ALARM_LOW, 
                   &dns.aException.detailedException[0],
                   BIT1);
    // bit2: TPH sas[0]
    awbitprocessor(meter.dnet_sas[0].status & ALARM_HIGH, 
                   &dns.aException.detailedException[0],
                   BIT2);
    // bit3: ssc
    awbitprocessor(ctrl.dnet_scc.status & ALARM_SSC, 
                   &dns.aException.detailedException[0],
                   BIT3);
    // bit4: TPL saa
    awbitprocessor(ctrl.dnet_saa.status & ALARM_LOW, 
                   &dns.aException.detailedException[0],
                   BIT4);    
    // bit5: TPH saa
    awbitprocessor(ctrl.dnet_saa.status & ALARM_HIGH, 
                   &dns.aException.detailedException[0],
                   BIT5);

    // bit0: TPL sas[1]
    awbitprocessor(meter.dnet_sas[1].status & ALARM_LOW, 
                   &dns.aException.detailedException[1],
                   BIT0);
    // bit1: TPH sas[1]
    awbitprocessor(meter.dnet_sas[1].status & ALARM_HIGH, 
                   &dns.aException.detailedException[1],
                   BIT1);
    // bit2: TPL sas[2]
    awbitprocessor(meter.dnet_sas[2].status & ALARM_LOW, 
                   &dns.aException.detailedException[1],
                   BIT2);
    // bit3: TPH sas[2]
    awbitprocessor(meter.dnet_sas[2].status & ALARM_HIGH, 
                   &dns.aException.detailedException[1],
                   BIT3);
    // bit4: Reading not valid sas[1]
    awbitprocessor((meter.dnet_sas[1].valid == 0), 
                   &dns.aException.detailedException[1],
                   BIT4);   
    // bit5: Reading not valid sas[2]
    awbitprocessor((meter.dnet_sas[2].valid == 0), 
                   &dns.aException.detailedException[1],
                   BIT5);
                   
    dns.aException.manufacturerException = 0;
    
  } else {
    dns.aException.commonException[0] = 0;
    dns.aException.commonException[1] = 0;
    dns.aException.detailedException[0] = 0;
    dns.aException.detailedException[1] = 0;
    dns.aException.manufacturerException = 0;    
  }
  
  if ((dns.wException.detailedException[0] != 0) || (dns.wException.detailedException[1] != 0)) {
    status |= WARN_DEV_SPECIFIC;
  } else {
    status &= ~WARN_DEV_SPECIFIC;
  }
  if ((dns.aException.detailedException[0] != 0) || (dns.aException.detailedException[1] != 0)) {
    status |= ALARM_DEV_SPECIFIC;
  } else {
    status &= ~ALARM_DEV_SPECIFIC;
  }
  dns.eStatus = status;
     
  return results;
}

/**
  @brief S-Analog Sensor updates
*/
uint8_t update_sas(void) {
  uint8_t results = 0;
  static uint16_t wTimer[DNS_EMFC_OBJECT_S_ANALOG_SENSOR_MAX_INST] = {0};
  static uint16_t aTimer[DNS_EMFC_OBJECT_S_ANALOG_SENSOR_MAX_INST] = {0};  
  
  for (uint8_t inst=0; inst<DNS_EMFC_OBJECT_S_ANALOG_SENSOR_MAX_INST; inst++) {
    switch (inst) {
      case 0: // meter
        meter.dnet_sas[inst].value = meter.normalizedFlow;      
        if (meter.expf.state == 1) {
          meter.dnet_sas[inst].valid = 1;
        } else {
          meter.dnet_sas[inst].valid = 0;
        }
        break;
      case 1: // pressure
        meter.dnet_sas[inst].value = meter.normalizedP1;
        meter.dnet_sas[inst].valid = 1;
        break;
      case 2: // temperature
        meter.dnet_sas[inst].value = meter.normalizedT;
        if (meter.tflt[0].state == 1) {
          meter.dnet_sas[inst].valid = 1;
        } else {
          meter.dnet_sas[inst].valid = 0;
        }
        break;
    }    
    // process warnings
    wae(nvmem.app.sasObj[inst].warn.eflag, 
        meter.dnet_sas[inst].value,
        nvmem.app.sasObj[inst].warn.hyst,
        nvmem.app.sasObj[inst].warn.tpl,
        nvmem.app.sasObj[inst].warn.tph,
        &wTimer[inst],
        nvmem.app.sasObj[inst].warn.SetTime,
        WARNING_LOW,
        WARNING_HIGH,
        &meter.dnet_sas[inst].status);
    // process alarms
    wae(nvmem.app.sasObj[inst].alarm.eflag, 
        meter.dnet_sas[inst].value,
        nvmem.app.sasObj[inst].alarm.hyst,
        nvmem.app.sasObj[inst].alarm.tpl,
        nvmem.app.sasObj[inst].alarm.tph,
        &aTimer[inst],
        nvmem.app.sasObj[inst].alarm.SetTime,
        ALARM_LOW,
        ALARM_HIGH,
        &meter.dnet_sas[inst].status);
        
  }     
  return results;
}

/**
  @brief S-Analog Actuator updates
*/
uint8_t update_saa(void) {
  uint8_t results = 0;
  static uint16_t wTimer = 0;
  static uint16_t aTimer = 0; 

  // process warnings
    wae(nvmem.app.saaObj.warn.eflag, 
        ctrl.ctrlv,
        nvmem.app.saaObj.warn.hyst,
        nvmem.app.saaObj.warn.tpl,
        nvmem.app.saaObj.warn.tph,
        &wTimer,
        100,
        WARNING_LOW,
        WARNING_HIGH,
        &ctrl.dnet_saa.status);
    // process alarms
    wae(nvmem.app.saaObj.alarm.eflag, 
        ctrl.ctrlv,
        nvmem.app.saaObj.alarm.hyst,
        nvmem.app.saaObj.alarm.tpl,
        nvmem.app.saaObj.alarm.tph,
        &aTimer,
        100,
        ALARM_LOW,
        ALARM_HIGH,
        &ctrl.dnet_saa.status);
  
  
  // update internal override flags to be used by the contorller only if device is in IDLE state
  if (dns.devstate == IDLE_STATUS) {
    ctrl.override = ctrl.dnet_saa.override;
  }
  
  return results;
}

/**
  @brief S-Stage Controller updates
*/
uint8_t update_ssc(void) {
  uint8_t results = 0; 
  static uint16_t wTimer = 0;
  static uint16_t aTimer = 0; 

  ctrl.dnet_scc.pvariable = *ctrl.feedback;
  
  // process warnings
  wae(nvmem.app.sccObj.warn.eflag, 
      ctrl.ctrlv,
      nvmem.app.sccObj.warn.errBand/2.0f,
      *ctrl.feedback,
      *ctrl.feedback,
      &wTimer,
      100,
      WARNING_SSC,
      WARNING_SSC,
      &ctrl.dnet_scc.status);
  // process alarms
  wae(nvmem.app.sccObj.alarm.eflag, 
      ctrl.ctrlv,
      nvmem.app.sccObj.alarm.errBand/2.0f,
      *ctrl.feedback,
      *ctrl.feedback,
      &aTimer,
      100,
      ALARM_SSC,        
      ALARM_SSC,
      &ctrl.dnet_scc.status);
  
  return results;
}

/**
  @brief Wrapper to user object updates
*/
uint8_t dns_update_objects(void) {
  uint8_t results = 0;
      
  results |= update_sas();
  results |= update_saa();
  results |= update_ssc();
  results |= update_sup();
  
  return results;
  
}
