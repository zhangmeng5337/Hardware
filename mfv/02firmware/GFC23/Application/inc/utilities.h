#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <stdint.h>

#define ONEFLT15BIT 9.1552734375e-5 // for 16BIT ADC, VREF=2.999V, two's complement
#define ONEFLT17BIT 31.25E-6        // for 18BIT ADC, VREF=4.096V, two's complement
#define ONEFLT19BIT 5.7220458E-6F   // for 20BIT ADC, VREF=2.999V, two's complement
#define ONEFLT23BIT 4.8828125E-7F   // for 24BIT ADC, VREF=4.096V, two's complement

#define max(x1, x2) ((x1) > (x2) ? (x1) : (x2))
#define min(x1, x2) ((x1) < (x2) ? (x1) : (x2))
#define signum(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))

#define STATSAMPLES 32

typedef struct {
  float stat[STATSAMPLES];
  uint8_t stat_samples;
  uint8_t sIndex;
  float stdsums;
  float stdmean;
  float stdev;
  uint8_t valid;
} statistics_struct;

typedef struct {
  float    SourceFreq1;      //!< Chirp initial frequency
  float    SourceFreq2;      //!< Chirp target frequency
  float    SourceSweepTime;  //!< Chirp time to target frequency
  float    SourceGain;       //!< Chirp signal gain
  float    SourceOffset;     //!< Chirp signal offset
  float    SourceSineFreq;   //!< Sine frequency
  uint8_t  SourceMode;       //!< Source mode
  uint8_t  SourceEnabledFlag;// source generation status
  
  uint8_t  SourceDirection;
  float    RampRateS;
  float    SourceDelay;
  
  float    SourceDelayCtr;
  float    SourceOut;
} system_identification_struct;

extern uint32_t math_check ( float in, float *out, float defValue );
extern float convert_16bit_to_float(uint32_t data);
extern float convert_20bit_to_float(uint32_t data);
extern int16_t convert_12bit_to_signed(uint16_t data);
extern float norder_fpoly_eval ( float *coeff, uint32_t order, float x_value );
extern double norder_dpoly_eval ( float *coeff, uint32_t order, float x_value );
extern float fpReciprocal(float value);
extern char *ftoa(float f);
extern char *htoa(uint16_t u16);
extern char *dtoa(uint8_t u8);
//extern void flot(char* p, float x);
//extern void ftoa2(float n, char* res, int afterpoint);
extern void runstats(statistics_struct *stat, float fxin);
extern void SystemIdent_Init(system_identification_struct *sysIdent);
extern void SystemIdent_Cycle(system_identification_struct *sysIdent);
#endif
