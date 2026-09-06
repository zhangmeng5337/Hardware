#include <stdint.h>
#include "protocol.h"
#include "meter.h"
#include "controller.h"
// 3  &meter.cdg[1].cdgpmvalue,
const command_struct cmdSensor[] = 
{    
  {1,    _FLOAT32,     4,      &meter.cdg[0].cdgfrac,              0x00,       0x00,      _READ_ONLY   },
  {2,    _FLOAT32,     4,      &meter.cdg[1].cdgfrac,              0x00,       0x00,      _READ_ONLY   },
  {3,    _FLOAT32,     4,      &meter.cdg[0].cdgpmvalue,           0x00,       0x00,      _READ_ONLY   },
  {4,    _FLOAT32,     4,      &meter.cdg[1].cdgpmvalue,           0x00,       0x00,      _READ_ONLY   },
  {5,    _FLOAT32,     4,      &meter.normalizedcdgpunits,         0x00,       0x00,      _READ_ONLY   },
  {6,    _FLOAT32,     4,      &meter.pressure_slope,              0x00,       0x00,      _READ_ONLY   },
  {7,    _FLOAT32,     4,      &ctrl.out,                          0x00,       0x00,      _READ_ONLY   },//ctrl.out
  {8,    _FLOAT32,     4,      &ctrl.pout,                         0x00,       0x00,      _READ_ONLY   },//ctrl.pout
  {9,    _FLOAT32,     4,      &ctrl.set_slope,                    0x00,       0x00,      0x00   },
  {10,    _FLOAT32,    4,      &ctrl.slopeCtrlVal2,                0x00,       0x00,      0x00   },
  {11,    _FLOAT32,    4,      &ctrl.slopeCtrlVal2,            	   0x00,       0x00,      0x00   },
  {12,    _FLOAT32,    4,      &meter.adativeklftcdgunits[0],      0x00,       0x00,      _READ_ONLY   },
  {13,    _FLOAT32,    4,      &meter.adativeklftcdgunits[1],      0x00,       0x00,      _READ_ONLY   },
  {14,    _FLOAT32,     4,     &adaKfP[1].norm,                    0x00,       0x00,      _READ_ONLY   },//
  {15,    _FLOAT32,     4,     &adaKfP[1].local_std,               0x00,       0x00,      _READ_ONLY   },//
  {16,    _FLOAT32,     4,     &ctrl.ppterm,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm
  {17,    _FLOAT32,     4,     &ctrl.piterm,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].local_std
  {18,    _FLOAT32,     4,     &ctrl.perror,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm
  {19,    _FLOAT32,     4,     &ctrl.igainCof,                     0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm  
  {20,    _FLOAT32,     4,     &ctrl.pterm,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm
  {21,    _FLOAT32,     4,     &ctrl.iterm,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].local_std
  {22,    _FLOAT32,     4,     &ctrl.error,                       0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm  
  {23,    _FLOAT32,     4,     &ctrl.pressureChangeSlope,         0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm  
  {24,    _FLOAT32,     4,     &ctrl.internal_state,              0x00,       0x00,      _READ_ONLY   },//adaKfP[1].norm  
  {25,    _FLOAT32,     4,     &meter.cdg[0].cdgoffsetTmp,        0x00,       0x00,      _READ_ONLY},//adaKfP[1].norm  
  {26,    _FLOAT32,     4,     &meter.cdg[1].cdgoffsetTmp,        0x00,       0x00,      _READ_ONLY},//adaKfP[1].norm  
  {27,    _FLOAT32,     4,     &meter.cdg[0].cdgvalueNow,        0x00,       0x00,       _READ_ONLY},//adaKfP[1].norm  
  {28,    _FLOAT32,     4,     &meter.cdg[1].cdgvalueNow,        0x00,       0x00,       _READ_ONLY},//adaKfP[1].norm  
  {29,    _UINT8,       1,     &meter.adjustMode,               0x00,       0x00,        _READ_ONLY},//0:no adjust 1:adjust  
  {30,    _FLOAT32,     4,     &ctrl.rampTotalTime,              0x00,       0x00,       _READ_ONLY},//adaKfP[1].norm  
  {31,    _FLOAT32,     4,     &meter.cdg[1].cdgoffsetNew,       0x00,       0x00,       _READ_ONLY},//adaKfP[1].norm  
  {32,    _FLOAT32,     4,     &meter.cdg[1].cdgoffsetNew,       0x00,       0x00,       _READ_ONLY},//adaKfP[1].norm  
  {33,   _UINT8,		1, 	   &meter.sensorMode,			     0x00,	     0x00,	      _READ_ONLY},
  

 
      
  // 200-300 for DeviceNet live attirbute reads
  
  {0x0000,   0x00,    0x00,   0x00,   0x00,   0x00}
};
