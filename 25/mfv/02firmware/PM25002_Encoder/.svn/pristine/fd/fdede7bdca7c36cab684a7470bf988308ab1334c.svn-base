/**
    @filename   awe.c
    @brief      Alarm/Warning/Exception
*/
#include "awe.h"
#include "pressure.h"
#include "meter.h"
#include "controller.h"

awe_struct awe = {0};

/**
  @brief Initialize Alarm/Warning/Exception pointers
*/
void init_awe(aw_struct *aw, uint32_t *warning, uint32_t *error) {

}  

/**
  @brief Check raw uint32 values
*/
void uint32_t_awe(uint32_t in, uint32_t lo, uint32_t hi, uint32_t lo_flag, uint32_t hi_flag, uint32_t *dst) {
  
  if (in < lo) {
    *dst |= lo_flag;
  } else {
    *dst &= ~lo_flag;
  }    
  
  if (in > hi) {
    *dst |= hi_flag;
  } else {
    *dst &= ~hi_flag;
  }    
}

/**
  @brief Check sensor values
*/
void float_awe(float in, float lo, float hi, uint32_t lo_flag, uint32_t hi_flag, uint32_t *dst) {
  
  if (in < lo) {
    *dst |= lo_flag;
  } else {
    *dst &= ~lo_flag;
  }    
  
  if (in > hi) {
    *dst |= hi_flag;
  } else {
    *dst &= ~hi_flag;
  }    
}

/**
  @brief Check if the data is NOT EQUAL TO TARGET. Set the flag if not equal, else clear the flag
*/
void bitcheck_awe(uint32_t data, uint32_t target, uint32_t flag, uint32_t *dst) {
  
  if (data != target) {
    *dst |= flag;
  } else {
    *dst &= ~flag;
  }
}

/**
*/
void awe_engine() {
  float dpzerolimit;
  float psidlowlimit, psidhighlimit;
  
  uint32_t_awe(pa.adp, 100, 9000000, PA_RAW_OUT_OF_RANGE_LOW, PA_RAW_OUT_OF_RANGE_HIGH, &awe.manufacturer.error);
  uint32_t_awe(pa.adp, 100, 9000000, FLOW_SENSOR_ERROR, FLOW_SENSOR_ERROR, &awe.device.error);
  
  uint32_t_awe(dp.adp, 100, 9000000, DP_RAW_OUT_OF_RANGE_LOW, DP_RAW_OUT_OF_RANGE_HIGH, &awe.manufacturer.error);
  uint32_t_awe(dp.adp, 100, 9000000, FLOW_SENSOR_ERROR, FLOW_SENSOR_ERROR, &awe.device.error);
  
  uint32_t_awe(pa.adt, 100000, 9000000, PAT_RAW_OUT_OF_RANGE_LOW, PAT_RAW_OUT_OF_RANGE_HIGH, &awe.manufacturer.error);
  uint32_t_awe(pa.adt, 100000, 9000000, FLOW_SENSOR_ERROR, FLOW_SENSOR_ERROR, &awe.device.error);
  
  uint32_t_awe(dp.adt, 100000, 9000000, DPT_RAW_OUT_OF_RANGE_LOW, DPT_RAW_OUT_OF_RANGE_HIGH, &awe.manufacturer.error);
  uint32_t_awe(dp.adt, 100000, 9000000, FLOW_SENSOR_ERROR, FLOW_SENSOR_ERROR, &awe.device.error);  
  
  float_awe(meter.psia, -10.0f, 150.0f, PRESSURE_OUT_OF_RANGE_LOW, PRESSURE_OUT_OF_RANGE_HIGH, &awe.device.error);
  
  psidlowlimit = -nvmem.app.pressureObj.pd.fullscale;
  psidhighlimit = 2.0f * nvmem.app.pressureObj.pd.fullscale;
  float_awe(meter.psid, psidlowlimit, psidhighlimit, DPPRESSURE_OUT_OF_RANGE_LOW, DPPRESSURE_OUT_OF_RANGE_HIGH, &awe.manufacturer.error);
  float_awe(meter.psid, psidlowlimit, psidhighlimit, FLOW_SENSOR_ERROR, FLOW_SENSOR_ERROR, &awe.device.error);  
  
  float_awe(meter.gasTempModel, -100.0f, 200.0f, TEMPERATURE_OUT_OF_RANGE_LOW, TEMPERATURE_OUT_OF_RANGE_HIGH, &awe.device.error);  
  
  dpzerolimit = nvmem.app.pressureObj.pd.fullscale * 0.1f; // get PSID 10% of FS 
  float_awe(nvmem.app.meterObj.dpzero, -dpzerolimit, dpzerolimit, DP_ZERO_LIMIT_LOW, DP_ZERO_LIMIT_HIGH, &awe.device.warning); 
  float_awe(nvmem.app.meterObj.dpzero, -dpzerolimit, dpzerolimit, DP_ZERO_LIMIT_LOW, DP_ZERO_LIMIT_HIGH, &awe.manufacturer.error); 
  
  if (awe.device.error) {
    awe.status |= AWE_DEVICE_ERROR;
  } else {
    awe.status &= ~AWE_DEVICE_ERROR;
  }

  if (awe.device.warning) {
    awe.status |= AWE_DEVICE_WARNING;
  } else {
    awe.status &= ~AWE_DEVICE_WARNING;
  }  

  awe.device_latched.warning |= awe.device.warning;  
  awe.device_latched.error |= awe.device.error;

  if (awe.manufacturer.error) {
    awe.status |= AWE_MANUFACTURING_ERROR;
  } else {
    awe.status &= ~AWE_MANUFACTURING_ERROR;
  }

  if (awe.manufacturer.warning) {
    awe.status |= AWE_MANUFACTURER_WARNING;
  } else {
    awe.status &= ~AWE_MANUFACTURER_WARNING;
  }  
  
  awe.manufacturer_latched.warning |= awe.manufacturer.warning;
  awe.manufacturer_latched.error |= awe.manufacturer.error;

  // save device latched errors for internal troubleshooting
  if (awe.device_latched.error != nvmem.app.error.deviceerrors) {
    nvmem.app.error.deviceerrors = awe.device_latched.error;
    write_to_flash(FLASH_TIMEOUT);
  } 
  
  // save manfucaturing latched errors for internal troubleshooting
  if (awe.manufacturer_latched.error != nvmem.app.error.manufacturererrors) {
    nvmem.app.error.manufacturererrors = awe.manufacturer_latched.error;
    write_to_flash(FLASH_TIMEOUT);
  }   
  
}
