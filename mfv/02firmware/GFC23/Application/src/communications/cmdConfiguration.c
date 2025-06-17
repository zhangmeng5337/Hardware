#include <stdint.h>
#include <string.h>
#include <math.h>
#include "protocol.h"
#include "nvmem.h"
#include "utilities.h"
#ifndef BOOTLOADER

#endif

binary nullAttr = {0};

/**
  @brief update calibration instance
*/
static uint8_t cmd01w(uint8_t *cmdInfo) {
#ifndef BOOTLOADER  
  command_info *cmdInfo_lcl = (command_info *)cmdInfo; 
  uint8_t instance;

  instance = cmdInfo_lcl->data_array[0];
  
  if ((instance >= 1) && (instance <= 6)) {
    if (nvmem.app.gasObj[instance-1].gasid == 0) {
      return _FAIL;   
    } else {
      // load gas properties here!!!!
      return _CONTINUE;     
    }
  }
#endif  
  return _FAIL; 
}

// 2
const command_struct cmdConfiguration[] = 
{
    {   1,    _UINT32,     4,       &nvmem.app.network.defaultbaudrate,         0x00,       0x00,     _FLASH        },
    {   2,  _OCTETSTR,    32,       &nvmem.app.manufacturing.electronicsrev,    0x00,       0x00,     _FLASH        },      
    {   3,  _OCTETSTR,    32,       &nvmem.app.manufacturing.sensorserialnumber,      0x00,       0x00,     _FLASH        },
    {   4,  _OCTETSTR,    12,       &nvmem.app.manufacturing.deviceserialnumber,      0x00,       0x00,     _FLASH        },
#ifndef BOOTLOADER		
//    {   5,	 _UINT32,      4,       &awe.manufacturer_latched.error,            0x00,       0x00,     0x00        },
//    {   6,	 _UINT32,      4,       &awe.device_latched.error,                  0x00,       0x00,     0x00        },
//    {   7,	 _UINT32,      4,       &awe.manufacturer_latched.warning,          0x00,       0x00,     0x00        },
//    {   8,	 _UINT32,      4,       &awe.device_latched.warning,                0x00,       0x00,     0x00        },
#endif    
// 9..    
    {  54,    _UINT8,      1,       &nvmem.app.network.address,                 0x00,       0x00,     _FLASH        }, 
    {  57,    _UINT8,      1,       &nvmem.app.network.length,                  0x00,       0x00,     _FLASH        },
    {  58,    _UINT8,      1,       &nvmem.app.network.parity,                  0x00,       0x00,     _FLASH        },
    {  59,    _UINT8,      1,       &nvmem.app.network.stopbits,                0x00,       0x00,     _FLASH        },  
    
    {  210,  _UINT16,      2,       &nvmem.app.gasObj[0].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  211,	_FLOAT32,      4,       &nvmem.app.gasObj[0].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  212,	_FLOAT32,      4,       &nvmem.app.gasObj[0].uref,	                                0x00,       0x00,     _FLASH        },
//    {  213,	_FLOAT32,      4,       &nvmem.app.gasObj[0].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  214, _FLOAT32,      4,       &nvmem.app.gasObj[0].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  215, _FLOAT32,      4,       &nvmem.app.gasObj[0].molweight,                           0x00,       0x00,     _FLASH        },
//    {  219, _FLOAT32,      4,       &nvmem.app.gasObj[0].Z,                                   0x00,       0x00,     _FLASH        },
    
    {  230,  _UINT16,      2,       &nvmem.app.gasObj[1].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  231,	_FLOAT32,      4,       &nvmem.app.gasObj[1].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  232,	_FLOAT32,      4,       &nvmem.app.gasObj[1].uref,	                                0x00,       0x00,     _FLASH        },
//    {  233,	_FLOAT32,      4,       &nvmem.app.gasObj[1].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  234, _FLOAT32,      4,       &nvmem.app.gasObj[1].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  235, _FLOAT32,      4,       &nvmem.app.gasObj[1].molweight,                           0x00,       0x00,     _FLASH        },
//    {  239, _FLOAT32,      4,       &nvmem.app.gasObj[1].Z,                                   0x00,       0x00,     _FLASH        },
    
    {  240,  _UINT16,      2,       &nvmem.app.gasObj[2].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  241,	_FLOAT32,      4,       &nvmem.app.gasObj[2].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  242,	_FLOAT32,      4,       &nvmem.app.gasObj[2].uref,	                                0x00,       0x00,     _FLASH        },
//    {  243,	_FLOAT32,      4,       &nvmem.app.gasObj[2].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  244, _FLOAT32,      4,       &nvmem.app.gasObj[2].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  245, _FLOAT32,      4,       &nvmem.app.gasObj[2].molweight,                           0x00,       0x00,     _FLASH        },
//    {  249, _FLOAT32,      4,       &nvmem.app.gasObj[2].Z,                                   0x00,       0x00,     _FLASH        },
    
    {  250,  _UINT16,      2,       &nvmem.app.gasObj[3].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  251,	_FLOAT32,      4,       &nvmem.app.gasObj[3].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  252,	_FLOAT32,      4,       &nvmem.app.gasObj[3].uref,	                                0x00,       0x00,     _FLASH        },
//    {  253,	_FLOAT32,      4,       &nvmem.app.gasObj[3].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  254, _FLOAT32,      4,       &nvmem.app.gasObj[3].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  255, _FLOAT32,      4,       &nvmem.app.gasObj[3].molweight,                           0x00,       0x00,     _FLASH        },
//    {  259, _FLOAT32,      4,       &nvmem.app.gasObj[3].Z,                                   0x00,       0x00,     _FLASH        },    

    {  260,  _UINT16,      2,       &nvmem.app.gasObj[4].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  261,	_FLOAT32,      4,       &nvmem.app.gasObj[4].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  262,	_FLOAT32,      4,       &nvmem.app.gasObj[4].uref,	                                0x00,       0x00,     _FLASH        },
//    {  263,	_FLOAT32,      4,       &nvmem.app.gasObj[4].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  264, _FLOAT32,      4,       &nvmem.app.gasObj[4].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  265, _FLOAT32,      4,       &nvmem.app.gasObj[4].molweight,                           0x00,       0x00,     _FLASH        },
//    {  269, _FLOAT32,      4,       &nvmem.app.gasObj[4].Z,                                   0x00,       0x00,     _FLASH        },

    {  270,  _UINT16,      2,       &nvmem.app.gasObj[5].gasid,                               0x00,       0x00,     _FLASH        },    
//    {  271,	_FLOAT32,      4,       &nvmem.app.gasObj[5].Tref,	                                0x00,       0x00,     _FLASH        }, 
//    {  272,	_FLOAT32,      4,       &nvmem.app.gasObj[5].uref,	                                0x00,       0x00,     _FLASH        },
//    {  273,	_FLOAT32,      4,       &nvmem.app.gasObj[5].uK2,	                                  0x00,       0x00,     _FLASH        },
//    {  274, _FLOAT32,      4,       &nvmem.app.gasObj[5].uK1,                                  0x00,       0x00,     _FLASH        },
//    {  275, _FLOAT32,      4,       &nvmem.app.gasObj[5].molweight,                           0x00,       0x00,     _FLASH        },
//    {  279, _FLOAT32,      4,       &nvmem.app.gasObj[5].Z,                                   0x00,       0x00,     _FLASH        },

//    {  280, _FLOAT32,      4,       &nvmem.app.gasObj[0].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  281, _FLOAT32,      4,       &nvmem.app.gasObj[0].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  282, _FLOAT32,      4,       &nvmem.app.gasObj[0].Omega,                                0x00,       0x00,     _FLASH        },  
//    {  283, _FLOAT32,      4,       &nvmem.app.gasObj[1].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  284, _FLOAT32,      4,       &nvmem.app.gasObj[1].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  285, _FLOAT32,      4,       &nvmem.app.gasObj[1].Omega,                                0x00,       0x00,     _FLASH        },
//    {  286, _FLOAT32,      4,       &nvmem.app.gasObj[2].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  287, _FLOAT32,      4,       &nvmem.app.gasObj[2].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  288, _FLOAT32,      4,       &nvmem.app.gasObj[2].Omega,                                0x00,       0x00,     _FLASH        },    
//    {  289, _FLOAT32,      4,       &nvmem.app.gasObj[3].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  290, _FLOAT32,      4,       &nvmem.app.gasObj[3].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  291, _FLOAT32,      4,       &nvmem.app.gasObj[3].Omega,                                0x00,       0x00,     _FLASH        },      
//    {  292, _FLOAT32,      4,       &nvmem.app.gasObj[4].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  293, _FLOAT32,      4,       &nvmem.app.gasObj[4].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  294, _FLOAT32,      4,       &nvmem.app.gasObj[4].Omega,                                0x00,       0x00,     _FLASH        },  
//    {  295, _FLOAT32,      4,       &nvmem.app.gasObj[5].Tc,                                   0x00,       0x00,     _FLASH        },
//    {  296, _FLOAT32,      4,       &nvmem.app.gasObj[5].Pc,                                   0x00,       0x00,     _FLASH        },
//    {  297, _FLOAT32,      4,       &nvmem.app.gasObj[5].Omega,                                0x00,       0x00,     _FLASH        },    
    
    // 300-500 for DeviceNet   
    {  302,   _UINT8,      1,       &nvmem.app.network.producer,                              0x00,       0x00,     _FLASH        },
    {  303,   _UINT8,      1,       &nvmem.app.network.consumer,                              0x00,       0x00,     _FLASH        },     
    {  304,   _OCTETSTR,   sizeof(nvmem.app.supObj.devtype),  &nvmem.app.supObj.devtype,      0x00,       0x00,     _FLASH        },
    {  305,   _OCTETSTR,   sizeof(nvmem.app.supObj.semirev),  &nvmem.app.supObj.semirev,      0x00,       0x00,     _FLASH        },
    {  306,   _OCTETSTR,   sizeof(nvmem.app.supObj.manufacturer),  &nvmem.app.supObj.manufacturer,      0x00,       0x00,     _FLASH        },
    {  307,   _OCTETSTR,   sizeof(nvmem.app.supObj.model),  &nvmem.app.supObj.model,          0x00,       0x00,     _FLASH        },
    {  308,   _OCTETSTR,   sizeof(nvmem.app.supObj.fwrev),  &nvmem.app.supObj.fwrev,          0x00,       0x00,     _FLASH        },
    {  309,   _OCTETSTR,   sizeof(nvmem.app.supObj.hwrev),  &nvmem.app.supObj.hwrev,          0x00,       0x00,     _FLASH        },
    {  310,   _OCTETSTR,   sizeof(nvmem.app.supObj.sn),  &nvmem.app.supObj.sn,                0x00,       0x00,     _FLASH        },
    {  311,   _OCTETSTR,   sizeof(nvmem.app.supObj.config),  &nvmem.app.supObj.config,        0x00,       0x00,     _FLASH        },
    {  312,  _UINT16,      2,       &nvmem.app.manufacturing.usVendorId,                      0x00,       0x00,     _FLASH        },
    {  313,  _UINT16,      2,       &nvmem.app.manufacturing.usDeviceType,                    0x00,       0x00,     _FLASH        },
    {  314,  _UINT16,      2,       &nvmem.app.manufacturing.usProductCode,                   0x00,       0x00,     _FLASH        },
    {  315,   _OCTETSTR,   sizeof(nvmem.app.manufacturing.abProductName),  &nvmem.app.manufacturing.abProductName,        0x00,       0x00,     _FLASH        },
    {  316,   _UINT8,      1,       &nvmem.app.supObj.aEnable,                                0x00,       0x00,     _FLASH        },
    {  317,   _UINT8,      1,       &nvmem.app.supObj.wEnable,                                0x00,       0x00,     _FLASH        },
        
    {  320,   _UINT8,      1,       &nvmem.app.sasObj[0].dType,                               0x00,       0x00,     _FLASH        },
    {  321,  _UINT16,      2,       &nvmem.app.sasObj[0].dUnits,                              0x00,       0x00,     _FLASH        },
    {  322,  _UINT16,      2,       &nvmem.app.sasObj[0].fscounts,                            0x00,       0x00,     _FLASH        },
    {  323, _FLOAT32,      4,       &nvmem.app.sasObj[0].fsrange,                             0x00,       0x00,     _FLASH        },
    {  324,   _UINT8,      1,       &nvmem.app.sasObj[0].warn.eflag,                          0x00,       0x00,     _FLASH        },
    {  325, _FLOAT32,      4,       &nvmem.app.sasObj[0].warn.tph,                            0x00,       0x00,     _FLASH        },
    {  326, _FLOAT32,      4,       &nvmem.app.sasObj[0].warn.tpl,                            0x00,       0x00,     _FLASH        },    
    {  327, _FLOAT32,      4,       &nvmem.app.sasObj[0].warn.hyst,                           0x00,       0x00,     _FLASH        },    
    {  328,  _UINT16,      2,       &nvmem.app.sasObj[0].warn.SetTime,                        0x00,       0x00,     _FLASH        },
    {  329,   _UINT8,      1,       &nvmem.app.sasObj[0].alarm.eflag,                         0x00,       0x00,     _FLASH        },    
    {  330, _FLOAT32,      4,       &nvmem.app.sasObj[0].alarm.tph,                           0x00,       0x00,     _FLASH        },
    {  331, _FLOAT32,      4,       &nvmem.app.sasObj[0].alarm.tpl,                           0x00,       0x00,     _FLASH        },    
    {  332, _FLOAT32,      4,       &nvmem.app.sasObj[0].alarm.hyst,                          0x00,       0x00,     _FLASH        },    
    {  333,  _UINT16,      2,       &nvmem.app.sasObj[0].alarm.SetTime,                       0x00,       0x00,     _FLASH        },
    {  334, _FLOAT32,      4,       &nvmem.app.sasObj[0].offsetA,                             0x00,       0x00,     _FLASH        },
    {  335, _FLOAT32,      4,       &nvmem.app.sasObj[0].gainA,                               0x00,       0x00,     _FLASH        },
//    {  336,  _UINT16,      2,       &nvmem.app.meterObj.calibrationinstance,                  0x00,       &cmd01w,  _FLASH        }, 
    
    {  340,   _UINT8,      1,       &nvmem.app.sasObj[1].dType,                               0x00,       0x00,     _FLASH        },
    {  341,  _UINT16,      2,       &nvmem.app.sasObj[1].dUnits,                              0x00,       0x00,     _FLASH        },
    {  342,  _UINT16,      2,       &nvmem.app.sasObj[1].fscounts,                            0x00,       0x00,     _FLASH        },
    {  343, _FLOAT32,      4,       &nvmem.app.sasObj[1].fsrange,                             0x00,       0x00,     _FLASH        },
    {  344,   _UINT8,      1,       &nvmem.app.sasObj[1].warn.eflag,                          0x00,       0x00,     _FLASH        },
    {  345, _FLOAT32,      4,       &nvmem.app.sasObj[1].warn.tph,                            0x00,       0x00,     _FLASH        },
    {  346, _FLOAT32,      4,       &nvmem.app.sasObj[1].warn.tpl,                            0x00,       0x00,     _FLASH        },    
    {  347, _FLOAT32,      4,       &nvmem.app.sasObj[1].warn.hyst,                           0x00,       0x00,     _FLASH        },    
    {  348,  _UINT16,      2,       &nvmem.app.sasObj[1].warn.SetTime,                        0x00,       0x00,     _FLASH        },
    {  349,   _UINT8,      1,       &nvmem.app.sasObj[1].alarm.eflag,                         0x00,       0x00,     _FLASH        },    
    {  350, _FLOAT32,      4,       &nvmem.app.sasObj[1].alarm.tph,                           0x00,       0x00,     _FLASH        },
    {  351, _FLOAT32,      4,       &nvmem.app.sasObj[1].alarm.tpl,                           0x00,       0x00,     _FLASH        },    
    {  352, _FLOAT32,      4,       &nvmem.app.sasObj[1].alarm.hyst,                          0x00,       0x00,     _FLASH        },    
    {  353,  _UINT16,      2,       &nvmem.app.sasObj[1].alarm.SetTime,                       0x00,       0x00,     _FLASH        },
    {  354, _FLOAT32,      4,       &nvmem.app.sasObj[1].offsetA,                             0x00,       0x00,     _FLASH        },
    {  355, _FLOAT32,      4,       &nvmem.app.sasObj[1].gainA,                               0x00,       0x00,     _FLASH        },
    {  356,  _UINT16,      2,       &nullAttr._uint16,                                        0x00,       0x00,     _READ_ONLY    },
    
    {  360,   _UINT8,      1,       &nvmem.app.sasObj[2].dType,                               0x00,       0x00,     _FLASH        },
    {  361,  _UINT16,      2,       &nvmem.app.sasObj[2].dUnits,                              0x00,       0x00,     _FLASH        },
    {  362,  _UINT16,      2,       &nvmem.app.sasObj[2].fscounts,                            0x00,       0x00,     _FLASH        },
    {  363, _FLOAT32,      4,       &nvmem.app.sasObj[2].fsrange,                             0x00,       0x00,     _FLASH        },
    {  364,   _UINT8,      1,       &nvmem.app.sasObj[2].warn.eflag,                          0x00,       0x00,     _FLASH        },
    {  365, _FLOAT32,      4,       &nvmem.app.sasObj[2].warn.tph,                            0x00,       0x00,     _FLASH        },
    {  366, _FLOAT32,      4,       &nvmem.app.sasObj[2].warn.tpl,                            0x00,       0x00,     _FLASH        },    
    {  367, _FLOAT32,      4,       &nvmem.app.sasObj[2].warn.hyst,                           0x00,       0x00,     _FLASH        },    
    {  368,  _UINT16,      2,       &nvmem.app.sasObj[2].warn.SetTime,                        0x00,       0x00,     _FLASH        },
    {  369,   _UINT8,      1,       &nvmem.app.sasObj[2].alarm.eflag,                         0x00,       0x00,     _FLASH        },    
    {  370, _FLOAT32,      4,       &nvmem.app.sasObj[2].alarm.tph,                           0x00,       0x00,     _FLASH        },
    {  371, _FLOAT32,      4,       &nvmem.app.sasObj[2].alarm.tpl,                           0x00,       0x00,     _FLASH        },    
    {  372, _FLOAT32,      4,       &nvmem.app.sasObj[2].alarm.hyst,                          0x00,       0x00,     _FLASH        },    
    {  373,  _UINT16,      2,       &nvmem.app.sasObj[2].alarm.SetTime,                       0x00,       0x00,     _FLASH        },  
    {  374, _FLOAT32,      4,       &nvmem.app.sasObj[2].offsetA,                             0x00,       0x00,     _FLASH        },
    {  375, _FLOAT32,      4,       &nvmem.app.sasObj[2].gainA,                               0x00,       0x00,     _FLASH        },
    {  376,  _UINT16,      2,       &nullAttr._uint16,                                        0x00,       0x00,     _READ_ONLY    },    

    {  380,   _UINT8,      1,       &nvmem.app.saaObj.dType,                                  0x00,       0x00,     _FLASH        },
    {  381,  _UINT16,      2,       &nvmem.app.saaObj.dUnits,                                 0x00,       0x00,     _FLASH        },
    {  382,   _UINT8,      1,       &nvmem.app.saaObj.warn.eflag,                             0x00,       0x00,     _FLASH        },
    {  383, _FLOAT32,      4,       &nvmem.app.saaObj.warn.tph,                               0x00,       0x00,     _FLASH        },
    {  384, _FLOAT32,      4,       &nvmem.app.saaObj.warn.tpl,                               0x00,       0x00,     _FLASH        },    
    {  385, _FLOAT32,      4,       &nvmem.app.saaObj.warn.hyst,                              0x00,       0x00,     _FLASH        },    
    {  386,   _UINT8,      1,       &nvmem.app.saaObj.alarm.eflag,                            0x00,       0x00,     _FLASH        },    
    {  387, _FLOAT32,      4,       &nvmem.app.saaObj.alarm.tph,                              0x00,       0x00,     _FLASH        },
    {  388, _FLOAT32,      4,       &nvmem.app.saaObj.alarm.tpl,                              0x00,       0x00,     _FLASH        },    
    {  389, _FLOAT32,      4,       &nvmem.app.saaObj.alarm.hyst,                             0x00,       0x00,     _FLASH        },  
    
    {  400,   _UINT8,      1,       &nvmem.app.sccObj.dType,                                  0x00,       0x00,     _FLASH        },
    {  401,  _UINT16,      2,       &nvmem.app.sccObj.dUnits,                                 0x00,       0x00,     _FLASH        },
    {  402,   _UINT8,      1,       &nvmem.app.sccObj.cMode,                                  0x00,       0x00,     _FLASH        },
    {  403,   _UINT8,      1,       &nvmem.app.sccObj.warn.eflag,                             0x00,       0x00,     _FLASH        },
    {  404, _FLOAT32,      4,       &nvmem.app.sccObj.warn.errBand,                           0x00,       0x00,     _FLASH        },
    {  405,  _UINT16,      2,       &nvmem.app.sccObj.warn.SetTime,                           0x00,       0x00,     _FLASH        },    
    {  406,   _UINT8,      1,       &nvmem.app.sccObj.alarm.eflag,                            0x00,       0x00,     _FLASH        },
    {  407, _FLOAT32,      4,       &nvmem.app.sccObj.alarm.errBand,                          0x00,       0x00,     _FLASH        },
    {  408,  _UINT16,      2,       &nvmem.app.sccObj.alarm.SetTime,                          0x00,       0x00,     _FLASH        },    
//    {  409,   _UINT8,      1,       &nvmem.app.controllerObj.safestate,                       0x00,       0x00,     _FLASH        },
//    {  410, _FLOAT32,      4,       &nvmem.app.controllerObj.safevalue,                       0x00,       0x00,     _FLASH        },
//    {  411,  _UINT32,      4,       &nvmem.app.controllerObj.ramprate,                        0x00,       0x00,     _FLASH        },
    
    {  420,  _UINT16,      2,       &nvmem.app.sgcObj[0].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  421,  _UINT16,      2,       &nvmem.app.sgcObj[0].sasInst,                             0x00,       0x00,     _FLASH        },
    {  422,  _OCTETSTR,  128,       &nvmem.app.sgcObj[0].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  423,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[0].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  424,  _UINT16,      2,       &nvmem.app.sgcObj[0].caldate,                             0x00,       0x00,     _FLASH        },
    {  425,  _UINT16,      2,       &nvmem.app.sgcObj[0].calGasNumber,                        0x00,       0x00,     _FLASH        },

    {  426,  _UINT16,      2,       &nvmem.app.sgcObj[1].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  427,  _UINT16,      2,       &nvmem.app.sgcObj[1].sasInst,                             0x00,       0x00,     _FLASH        },
    {  428,  _OCTETSTR,  128,       &nvmem.app.sgcObj[1].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  429,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[1].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  430,  _UINT16,      2,       &nvmem.app.sgcObj[1].caldate,                             0x00,       0x00,     _FLASH        },
    {  431,  _UINT16,      2,       &nvmem.app.sgcObj[1].calGasNumber,                        0x00,       0x00,     _FLASH        },

    {  432,  _UINT16,      2,       &nvmem.app.sgcObj[2].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  433,  _UINT16,      2,       &nvmem.app.sgcObj[2].sasInst,                             0x00,       0x00,     _FLASH        },
    {  434,  _OCTETSTR,  128,       &nvmem.app.sgcObj[2].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  435,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[2].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  436,  _UINT16,      2,       &nvmem.app.sgcObj[2].caldate,                             0x00,       0x00,     _FLASH        },
    {  437,  _UINT16,      2,       &nvmem.app.sgcObj[2].calGasNumber,                        0x00,       0x00,     _FLASH        },

    {  438,  _UINT16,      2,       &nvmem.app.sgcObj[3].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  439,  _UINT16,      2,       &nvmem.app.sgcObj[3].sasInst,                             0x00,       0x00,     _FLASH        },
    {  440,  _OCTETSTR,  128,       &nvmem.app.sgcObj[3].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  441,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[3].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  442,  _UINT16,      2,       &nvmem.app.sgcObj[3].caldate,                             0x00,       0x00,     _FLASH        },
    {  443,  _UINT16,      2,       &nvmem.app.sgcObj[3].calGasNumber,                        0x00,       0x00,     _FLASH        },

    {  444,  _UINT16,      2,       &nvmem.app.sgcObj[4].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  445,  _UINT16,      2,       &nvmem.app.sgcObj[4].sasInst,                             0x00,       0x00,     _FLASH        },
    {  446,  _OCTETSTR,  128,       &nvmem.app.sgcObj[4].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  447,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[4].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  448,  _UINT16,      2,       &nvmem.app.sgcObj[4].caldate,                             0x00,       0x00,     _FLASH        },
    {  449,  _UINT16,      2,       &nvmem.app.sgcObj[4].calGasNumber,                        0x00,       0x00,     _FLASH        },

    {  450,  _UINT16,      2,       &nvmem.app.sgcObj[5].gasNumber,                           0x00,       0x00,     _FLASH        },
    {  451,  _UINT16,      2,       &nvmem.app.sgcObj[5].sasInst,                             0x00,       0x00,     _FLASH        },
    {  452,  _OCTETSTR,  128,       &nvmem.app.sgcObj[5].gasSymbol,                           0x00,       0x00,     _FLASH        },
    {  453,  _OCTETSTR,    sizeof(S_GAS_CALIBRATION_OBJECT_RANGE_T),       &nvmem.app.sgcObj[5].sFullScale,                          0x00,       0x00,     _FLASH        },
    {  454,  _UINT16,      2,       &nvmem.app.sgcObj[5].caldate,                             0x00,       0x00,     _FLASH        },
    {  455,  _UINT16,      2,       &nvmem.app.sgcObj[5].calGasNumber,                        0x00,       0x00,     _FLASH        },
    
    {  460,  _UINT16,      2,       &nvmem.app.sgcObj[0].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  461,  _FLOAT32,     4,       &nvmem.app.sgcObj[0].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },
    {  462,  _UINT16,      2,       &nvmem.app.sgcObj[1].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  463,  _FLOAT32,     4,       &nvmem.app.sgcObj[1].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },    
    {  464,  _UINT16,      2,       &nvmem.app.sgcObj[2].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  465,  _FLOAT32,     4,       &nvmem.app.sgcObj[2].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },    
    {  466,  _UINT16,      2,       &nvmem.app.sgcObj[3].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  467,  _FLOAT32,     4,       &nvmem.app.sgcObj[3].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },    
    {  468,  _UINT16,      2,       &nvmem.app.sgcObj[4].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  469,  _FLOAT32,     4,       &nvmem.app.sgcObj[4].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },    
    {  470,  _UINT16,      2,       &nvmem.app.sgcObj[5].sFullScale.dUnits,                   0x00,       0x00,     _FLASH        },
    {  471,  _FLOAT32,     4,       &nvmem.app.sgcObj[5].sFullScale.fsrange,                  0x00,       0x00,     _FLASH        },    

    {  480, _FLOAT32,      4,       &nvmem.app.sgcObj[0].calPressure,                         0x00,       0x00,     _FLASH        },
    {  481, _FLOAT32,      4,       &nvmem.app.sgcObj[0].calTemperature,                      0x00,       0x00,     _FLASH        },
    {  482, _FLOAT32,      4,       &nvmem.app.sgcObj[1].calPressure,                         0x00,       0x00,     _FLASH        },
    {  483, _FLOAT32,      4,       &nvmem.app.sgcObj[1].calTemperature,                      0x00,       0x00,     _FLASH        },
    {  484, _FLOAT32,      4,       &nvmem.app.sgcObj[2].calPressure,                         0x00,       0x00,     _FLASH        },
    {  485, _FLOAT32,      4,       &nvmem.app.sgcObj[2].calTemperature,                      0x00,       0x00,     _FLASH        },
    {  486, _FLOAT32,      4,       &nvmem.app.sgcObj[3].calPressure,                         0x00,       0x00,     _FLASH        },
    {  487, _FLOAT32,      4,       &nvmem.app.sgcObj[3].calTemperature,                      0x00,       0x00,     _FLASH        },
    {  488, _FLOAT32,      4,       &nvmem.app.sgcObj[4].calPressure,                         0x00,       0x00,     _FLASH        },
    {  489, _FLOAT32,      4,       &nvmem.app.sgcObj[4].calTemperature,                      0x00,       0x00,     _FLASH        },
    {  490, _FLOAT32,      4,       &nvmem.app.sgcObj[5].calPressure,                         0x00,       0x00,     _FLASH        },
    {  491, _FLOAT32,      4,       &nvmem.app.sgcObj[5].calTemperature,                      0x00,       0x00,     _FLASH        },
    
    
    {  500,  _OCTETSTR,   128,       &nvmem.app.sgcObj[0].gasName,                            0x00,       0x00,     _FLASH        },
    {  501,  _OCTETSTR,   128,       &nvmem.app.sgcObj[1].gasName,                            0x00,       0x00,     _FLASH        },
    {  502,  _OCTETSTR,   128,       &nvmem.app.sgcObj[2].gasName,                            0x00,       0x00,     _FLASH        },
    {  503,  _OCTETSTR,   128,       &nvmem.app.sgcObj[3].gasName,                            0x00,       0x00,     _FLASH        },
    {  504,  _OCTETSTR,   128,       &nvmem.app.sgcObj[4].gasName,                            0x00,       0x00,     _FLASH        },
    {  505,  _OCTETSTR,   128,       &nvmem.app.sgcObj[5].gasName,                            0x00,       0x00,     _FLASH        },

    {0x0000,   0x00,    0x00,   0x00,   0x00,   0x00}
};
