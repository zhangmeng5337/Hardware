/**
    @filename   utilities.c
    @brief      This file contains math and bit conversion utility functions that can be used accross different platforms

*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"
#include "utilities.h"
#include "libUnits.h"

/**
    @brief return the reciprocal of the input value. In value is exactly 0.0, we retrun 0.0.
*/
float fpReciprocal(float value) {
    if (value == 0.0F) {
        return 0.0F;
    }
    return 1.0f/value;
}

/**
    @brief  convert a 24-bit (signed) to float
*/
float convert_24bit_to_float(uint32_t data) {
    uint32_t data24 = data & 0x007FFFFF;
    
    if ((data & 0x00800000)) {
        return -(((~data24 & 0x007FFFFF)+1) * ONEFLT23BIT);
    } else {
        return data * ONEFLT23BIT;
    }
}

/**
    @brief  convert a 20-bit (signed) to float
*/
float convert_20bit_to_float(uint32_t data) {
    uint32_t data19 = data & 0x0007FFFF;
    
    if (data & 0x00080000) {
        return -(((~data19 & 0x0007FFFF)+1) * ONEFLT19BIT);
    } else {
        return data19 * ONEFLT19BIT;
    }
}

/**
    @brief  convert a 16-bit (signed) to float
*/
float convert_16bit_to_float(uint32_t data) {
    uint32_t data15 = data & 0x00007FFF;
    
    if (data & 0x00008000) {
        return -(((~data15 & 0x00007FFF)+1) * ONEFLT15BIT);
    } else {
        return data15 * ONEFLT15BIT;
    }
}

/**
    @brief  convert 20-bit unsigned to signed integer
*/
int32_t convert_20bit_to_signed(uint32_t data) {
    int32_t data19 = data & 0x0007FFFF;
    
    if (data & 0x00080000) {
        return -((~data19 & 0x0007FFFF)+1);
    } else {
        return data19;
    }
     
}

/**
    @brief  convert 18-bit (signed) to float
*/
float convert_18bit_to_float(uint32_t data) {
    uint32_t data17 = data & 0x0003FFFF;
        
    if (data & 0x00020000) {
        return -(((~data17 & 0x0003FFFF)+1) * ONEFLT17BIT);
    } else {
        return data17 * ONEFLT17BIT;
    }
}

/**
    @brief  convert 18-bit unsigned to signed integer
*/
int32_t convert_18bit_to_signed(uint32_t data) {
    int32_t data17 = data & 0x0003FFFF;
    
    if (data & 0x00020000) {
        return -((~data17 & 0x0003FFFF)+1);
    } else {
        return data17;
    }
     
}

/**
    @brief  convert 12-bit unsigned to signed integer
*/
int16_t convert_12bit_to_signed(uint16_t data) {
    int32_t data11 = data & 0x000007FF;
    
    if (data & 0x00000800) {
        return -((~data11 & 0x000007FF)+1);
    } else {
        return data11;
    }
     
}

/**
    @brief Nth order polynomial evaluator using Horner's rule.
          
            Example:
                ax^3 + bx^2 + cx + d (requires 5mul, 3add)
            Evaluated using Horner's rule
                x*(x*(a*x + b) + c) + d (requires 3mul, 3add)
           
           Array ppointed to by coeff shall follow the following order
           Array[0] = Coeff of ^0
           Array[1] = Coeff of ^1
                :           :
           Array[order] = Coeff of ^ order
    @param coeff pointer to polynomial coefficients
    @param order polynomial order
    @param x_value x value to be evaluated
    @return y value of the polynomial at x_value
*/
float norder_fpoly_eval ( float *coeff, uint32_t order, float x_value ) /*@modifies nothing@*/{
    uint32_t i = order; 
    float y_val = coeff[order];  
    
    while ( i > 0 ) {
        i--;
        y_val = y_val * x_value + coeff[i];
    };
    
    return y_val;
}

/**
  @brief Double precision container for huge values
*/
double norder_dpoly_eval ( float *coeff, uint32_t order, float x_value ) /*@modifies nothing@*/{
    uint32_t i = order; 
    double y_val = coeff[order];  
    
    while ( i > 0 ) {
        i--;
        y_val = y_val * x_value + coeff[i];
    };
    
    return y_val;
}

/**
    @brief  math_check() function checks for Nan and Inf values. If the input float data is found to be Nan or Inf then function returns a default
            value.
*/
uint32_t math_check ( float in, float *out, float defValue ) {
    uint32_t return_code = 0;
    
    if (isnan(in) == 1) {
        *out = defValue;
        return_code |= 1;
        
    } else if (isfinite(in) == 0){
        *out = defValue;
        return_code |= 2;

    } else {
        *out = in;

    }
        
    return return_code;
}

/**
  @brief float to ascii string converter
*/
char *ftoa(float f) {
  static char buf[17];  
  snprintf(buf, 6, "%f", f);
  return buf;
}

/**
*/
char *htoa(uint16_t u16) {
  static char buf[4];
  for (int i=0; i<4; i++) {
    buf[3-i] = (u16 >> (i*4)) & 0x0f;
  }
  return buf;  
}

/**
*/
char *dtoa(uint8_t u8) {
  static char buf[5];
  snprintf(buf, 5, "%04d", u8);
  return buf;
}

/**
*/
void runstats(statistics_struct *stat, float fxin) {
  stat->valid = 0;
  if (isnan(fxin))
      return;
  
  if (stat->stat_samples < STATSAMPLES) {
      stat->stat[stat->stat_samples++] = fxin;
      stat->stdsums += fxin;
      stat->stdmean = stat->stdsums/stat->stat_samples; 
  } else {       
      stat->stdsums = stat->stdsums - stat->stat[stat->sIndex] + fxin;
      stat->stat[stat->sIndex++] = fxin;
      stat->stdmean = stat->stdsums/stat->stat_samples;        
      stat->sIndex %= stat->stat_samples;
      float sum_of_squares = 0.0f;
      for (uint8_t i = 0; i<stat->stat_samples; i++) {
          sum_of_squares += powf((stat->stat[i] - stat->stdmean),2);
      }
      stat->stdev = sqrtf(sum_of_squares/(stat->stat_samples-1));
      stat->valid = 1;
  }
}

/**
  @brief System Identification Init
*/
void SystemIdent_Init(system_identification_struct *sysIdent) {
  sysIdent->RampRateS = (sysIdent->SourceGain - sysIdent->SourceOffset)/(sysIdent->SourceSweepTime/0.002f);
  sysIdent->SourceOut = sysIdent->SourceOffset;
  sysIdent->SourceDelayCtr = sysIdent->SourceDelay;
  sysIdent->SourceDirection = 1;
  sysIdent->SourceEnabledFlag = 1;
}

/**
  @brief System Identifiaction Cycle
*/
void SystemIdent_Cycle(system_identification_struct *sysIdent) {
  
  if (sysIdent->SourceDelayCtr > 0.002f) {
    sysIdent->SourceDelayCtr -= 0.002f;
    return;    
  }
  
  if (sysIdent->SourceDirection == 1) {
    sysIdent->SourceOut += sysIdent->RampRateS;
    if (sysIdent->SourceOut > sysIdent->SourceGain) {
      sysIdent->SourceDirection ^= 1;
    }
  } else {
    sysIdent->SourceOut -= sysIdent->RampRateS;
    if (sysIdent->SourceOut < sysIdent->SourceOffset) {
      sysIdent->SourceDirection ^= 1;
    }
  }  
}
