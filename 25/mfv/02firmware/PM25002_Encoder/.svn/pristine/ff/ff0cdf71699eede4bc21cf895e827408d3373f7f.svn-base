#include <string.h>
#include "main.h" 
#include "analogIO.h"
#include "dacx0004.h"
#include "dacx0501.h"
#include "nvmem.h"
#include "utilities.h"

analogIO_struct dac[MAX_ANALOG_IO] = {{0,0,0,0},
                          {0,0,0,0},
                          {0,0,0,0},
													{0,0,0,0}};

                          
/**
  @brief 
           
*/
void analogOutput(float normalized, uint8_t idx, float saturation_pt) {
    
  normalized = min(normalized, saturation_pt);
  
  if (dac[idx].freeze == 0) {
     dac[idx].dacNormalized =  normalized;
  }
  dac[idx].dacRaw = MAX_DAC_COUNTS_14BIT * (dac[idx].dacNormalized * nvmem.app.cal[idx].gain + nvmem.app.cal[idx].offset);
  // call the DAC update funciton
  dac[idx].cback(dac[idx].dacRaw, (uint32_t)(idx<<20));    
}

/**
  @brief 
*/
float analogInput(uint8_t idx) {
  return 0;
}
   
/**
  @brief install signal redirection to analog output
*/
void analogIOinstall(float *signal, uint8_t aochannel, float saturation, uint8_t dactype) {
  dac[aochannel].signal = signal;
  dac[aochannel].saturation = saturation;
  switch (dactype) {
    case 2:      
      dac[aochannel].cback = &update_dacx0004; 
      break;
    default:
      init_dacx0501();
      dac[aochannel].cback = &update_dacx0501;         
      break;
  }
}
