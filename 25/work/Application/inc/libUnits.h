#ifndef _LIBUNITS_H_
#define _LIBUNITS_H_

#ifdef __cplusplus
extern "C" {
#endif
  
#include <stdint.h>
#include <stdbool.h>

#define DNET_UNITS_COUNTS     0x1001
#define DNET_UNITS_PERCENT    0x1007
#define DNET_UNITS_SCCM       0x1400
#define DNET_UNITS_SLM        0x1401
//#define DNET_UNITS_CFM        0x1402 
//#define DNET_UNITS_PACMS      0x1403
//#define DNET_UNITS_CMS        0x1405
//#define DNET_UNITS_LS         0x1406
//#define DNET_UNITS_MLS        0x1407
//#define DNET_UNITS_LM         0x1413
//#define DNET_UNITS_MLM        0x1411

#define DNET_UNITS_PSIA       0x1300
#define DNET_UNITS_TORR       0x1301
#define DNET_UNITS_MTORR      0x1302
#define DNET_UNITS_MMHG0C     0x1303 
#define DNET_UNITS_INHG0C     0x1304
#define DNET_UNITS_BAR        0x1307
#define DNET_UNITS_MBAR       0x1308
#define DNET_UNITS_PA         0x1309
#define DNET_UNITS_KPA        0x130A
#define DNET_UNITS_ATM        0x130B
#define DNET_UNITS_PSIG       0x0300

#define DNET_UNITS_FAHRENHEIT 0x1201
#define DNET_UNITS_CELSIUS    0x1200
#define DNET_UNITS_KELVIN     0x1202
#define DNET_UNITS_RANKINE    0x1203

#define DNET_UNITS_NANOMETER  0x2205

#define UNITS_COUNTS          0x1001
#define UNITS_PERCENT         0x1007

#define UNITS_SCCM            0x1400
#define UNITS_SLM             0x1401
#define UNITS_CFM             0x1402 
#define UNITS_PACMS           0x1403
#define UNITS_CMS             0x1405
#define UNITS_LS              0x1406
#define UNITS_MLS             0x1407
#define UNITS_LM              0x1413
#define UNITS_MLM             0x1411

#define UNITS_PSIA            0x1300
#define UNITS_TORR            0x1301
#define UNITS_MTORR           0x1302
#define UNITS_MMHG0C          0x1303 
#define UNITS_INHG0C          0x1304
#define UNITS_BAR             0x1307
#define UNITS_MBAR            0x1308
#define UNITS_PA              0x1309
#define UNITS_KPA             0x130A
#define UNITS_ATM             0x130B
#define UNITS_PSIG            0x0300

#define UNITS_FAHRENHEIT      0x1201
#define UNITS_CELSIUS         0x1200
#define UNITS_KELVIN          0x1202
#define UNITS_RANKINE         0x1203

typedef struct {
    uint32_t UnitsCode;
    float  scale;
    float  offset;
} DATA_UNITS_CONV_TABLE;

typedef struct {
    uint16_t UnitsCode;
    uint16_t NativeUnitsCode;
}DATA_UNITS_MAP_TABLE;

#define ECAT_INT_TYPE         1
#define ECAT_REAL_TYPE        0
// Data Units
#define DATAUNIT_COUNTS     0x1001
#define DATAUNIT_PERCENT    0x1007

#define DATAUNIT_SCCM            0x1400
#define DATAUNIT_SLM             0x1401
#define DATAUNIT_CFM             0x1402
#define DATAUNIT_PACMS           0x1403
#define DATAUNIT_CMS             0x1405
#define DATAUNIT_LS              0x1406
#define DATAUNIT_MLS             0x1407
#define DATAUNIT_LM              0x1413
#define DATAUNIT_MLM             0x1411

#define DATAUNIT_PSIA            0x1300
#define DATAUNIT_TORR            0x1301
#define DATAUNIT_MTORR           0x1302
#define DATAUNIT_MMHG0C          0x1303
#define DATAUNIT_INHG0C          0x1304
#define DATAUNIT_BAR             0x1307
#define DATAUNIT_MBAR            0x1308
#define DATAUNIT_PA              0x1309
#define DATAUNIT_KPA             0x130A
#define DATAUNIT_ATM             0x130B
#define DATAUNIT_PSIG            0x0300

#define DATAUNIT_FAHRENHEIT      0x1201
#define DATAUNIT_CELSIUS         0x1200
#define DATAUNIT_KELVIN          0x1202
#define DATAUNIT_RANKINE         0x1203

typedef struct {
	uint32_t EcUnitsCode;
	uint16_t NativeUnitsCode;
} EC_DATA_UNITS_CONV_TABLE;

typedef struct
{
	uint16_t flowunits;
	uint16_t pressureunits;
	uint16_t temperatureunits;
	
	uint16_t status;
}ECAT_VAR_STRUCT;

#define MAX_MASSFLOW_UNITS  48
#define MAX_NORMALVOLFLOW_UNITS  32
#define MAX_MASS_UNITS  28
#define MAX_VOLUME_UNITS  56

#define UNITS_MASSFLOW_G_H         0 // g/h
#define UNITS_MASSFLOW_G_DAY       1 // g/day
#define UNITS_MASSFLOW_G_MIN       2 // g/min
#define UNITS_MASSFLOW_G_MON30     3 // g/mon (30 days)
#define UNITS_MASSFLOW_G_MON31     4 // g/mon (31 days)
#define UNITS_MASSFLOW_G_SEC       5 // g/sec
#define UNITS_MASSFLOW_KG_DAY      6 // kg/day
#define UNITS_MASSFLOW_KG_H        7 // kg/h
#define UNITS_MASSFLOW_KG_MIN      8 // kg/min
#define UNITS_MASSFLOW_KG_MON30    9 // kg/mon (30 days)
#define UNITS_MASSFLOW_KG_MON31    10  // kg/mon (31 days)
#define UNITS_MASSFLOW_KG_SEC      11  // kg/sec
#define UNITS_MASSFLOW_MG_DAY      12  // mg/day
#define UNITS_MASSFLOW_MG_H        13  // mg/h
#define UNITS_MASSFLOW_MG_MIN      14  // mg/min
#define UNITS_MASSFLOW_MG_MON30    15  // mg/mon (30 days)
#define UNITS_MASSFLOW_MG_MON31    16  // mg/mon (31 days)
#define UNITS_MASSFLOW_MG_SEC      17  // mg/sec
#define UNITS_MASSFLOW_OZ_DAY      18  // oz/day
#define UNITS_MASSFLOW_OZ_H        19  // oz/h
#define UNITS_MASSFLOW_OZ_MIN      20  // oz/min
#define UNITS_MASSFLOW_OZ_MON30    21  // oz/mon (30 days)
#define UNITS_MASSFLOW_OZ_MON31    22  // oz/mon (31 days)
#define UNITS_MASSFLOW_OZ_SEC      23  // oz/sec
#define UNITS_MASSFLOW_LB_DAY      24  // lb/day
#define UNITS_MASSFLOW_LB_H        25  // lb/h
#define UNITS_MASSFLOW_LB_MIN      26  // lb/min
#define UNITS_MASSFLOW_LB_MON30    27  // lb/mon (30 days)
#define UNITS_MASSFLOW_LB_MON31    28  // lb/mon (31 days)
#define UNITS_MASSFLOW_LB_SEC      29  // lb/sec
#define UNITS_MASSFLOW_T_DAY       30  // t/d
#define UNITS_MASSFLOW_T_H         31  // t/h
#define UNITS_MASSFLOW_T_MIN       32  // t/min
#define UNITS_MASSFLOW_T_MON30     33  // t/mon (30 days)
#define UNITS_MASSFLOW_T_MON31     34  // t/mon (31 days)
#define UNITS_MASSFLOW_T_SEC       35  // t/sec
#define UNITS_MASSFLOW_TON_DAY_UK     36  // ton(UK)/day
#define UNITS_MASSFLOW_TON_H_UK       37  // ton(UK)/h
#define UNITS_MASSFLOW_TON_MIN_UK     38  // ton(UK)/min
#define UNITS_MASSFLOW_TON_MON30_UK   39  // ton(UK)/mon (30 days)  
#define UNITS_MASSFLOW_TON_MON31_UK   40  // ton(UK)/mon (31 days)
#define UNITS_MASSFLOW_TON_SEC_UK     41  // ton(UK)/sec  
#define UNITS_MASSFLOW_TON_DAY_US     42  // ton(US)/day
#define UNITS_MASSFLOW_TON_H_US       43  // ton(US)/h
#define UNITS_MASSFLOW_TON_MIN_US     44  // ton(US)/min
#define UNITS_MASSFLOW_TON_MON30_US   45  // ton(US)/mon (30 days)
#define UNITS_MASSFLOW_TON_MON31_US   46  // ton(US)/mon (31 days)
#define UNITS_MASSFLOW_TON_SEC_US     47  // ton(US)/sec

#define UNITS_VOLFLOW_L_H             0	  // L/h
#define UNITS_VOLFLOW_BBL_DAY         1	  // bbl/d
#define UNITS_VOLFLOW_BBL_H           2   // bbl/h
#define UNITS_VOLFLOW_BBL_MIN         3   // bbl/min
#define UNITS_VOLFLOW_BO_DAY          4   // bo/d
#define UNITS_VOLFLOW_BO_H            5   // bo/h
#define UNITS_VOLFLOW_BO_MIN          6   // bo/min
#define UNITS_VOLFLOW_CCM_H           7   // cm^3/h
#define UNITS_VOLFLOW_CCM_MIN         8   // cm^3/min
#define UNITS_VOLFLOW_CCM_SEC         9   // cm^3/sec
#define UNITS_VOLFLOW_CFT_MIN         10  // ft^3/min
#define UNITS_VOLFLOW_CFT_SEC         11  // ft^3/sec
#define UNITS_VOLFLOW_CIN_MIN         12  // in^3/min
#define UNITS_VOLFLOW_CIN_SEC         13  // in^3/sec
#define UNITS_VOLFLOW_CM_DAY          14  // m^3/day
#define UNITS_VOLFLOW_CM_MIN          15	// m^3/min
#define UNITS_VOLFLOW_CM_MON30        16  // m^3/month (30days)
#define UNITS_VOLFLOW_CM_SEC          17  // m^3/sec
#define UNITS_VOLFLOW_CMM_sec         18  // mm^3/sec
#define UNITS_VOLFLOW_CYD_H           19  // yd^3/h
#define UNITS_VOLFLOW_CYD_MIN         20  // yd^3/min
#define UNITS_VOLFLOW_CYD_SEC         21  // yd^3/sec
#define UNITS_VOLFLOW_GAL_DAY         22  // gal/day
#define UNITS_VOLFLOW_GAL_H           23  // gal/h
#define UNITS_VOLFLOW_GAL_MIN         24  // gal/min
#define UNITS_VOLFLOW_L_DAY           25  // L/day
#define UNITS_VOLFLOW_CM_H            26  // m^3/h
#define UNITS_VOLFLOW_L_MIN           27  // L/min
#define UNITS_VOLFLOW_L_SEC           28  // L/sec
#define UNITS_VOLFLOW_ML_SEC          29  // mL/sec
#define UNITS_VOLFLOW_PT_SEC          30  // pt/sec
#define UNITS_VOLFLOW_QT_SEC          31  // qt/sec

#define UNITS_MASS_G                  0
#define UNITS_MASS_CARAT_METRIC       1
#define UNITS_MASS_CARAT_US_UK        2
#define UNITS_MASS_DAG                3
#define UNITS_MASS_DR                 4
#define UNITS_MASS_DR_AP              5
#define UNITS_MASS_GR                 6
#define UNITS_MASS_GR_AP              7
#define UNITS_MASS_HUNDWGHT_L         8
#define UNITS_MASS_HUNDWGHT_S         9
#define UNITS_MASS_KG                 10
#define UNITS_MASS_MG                 11
#define UNITS_MASS_OZ                 12
#define UNITS_MASS_OZ_AP              13
#define UNITS_MASS_OZ_TROY            14
#define UNITS_MASS_DWT                15
#define UNITS_MASS_LB                 16
#define UNITS_MASS_LB_AP              17
#define UNITS_MASS_LB_TROY            18
#define UNITS_MASS_QT_AV_UK           19
#define UNITS_MASS_QT_AV_US           20
#define UNITS_MASS_Q                  21
#define UNITS_MASS_S                  22
#define UNITS_MASS_SL                 23
#define UNITS_MASS_ST                 24
#define UNITS_MASS_TON_UK             25
#define UNITS_MASS_TON_METRIC         26
#define UNITS_MASS_TON_US             27

#define UNITS_VOLUME_L	0
#define UNITS_VOLUME_AF	1
#define UNITS_VOLUME_AC_IN	2
#define UNITS_VOLUME_DRY_BBL	3
#define UNITS_VOLUME_LIQUID_BBL	4
#define UNITS_VOLUME_BO	5
#define UNITS_VOLUME_BU_UK	6
#define UNITS_VOLUME_BU_US	7
#define UNITS_VOLUME_CL	8
#define UNITS_VOLUME_CHD	9
#define UNITS_VOLUME_CD	10
#define UNITS_VOLUME_CM3	11
#define UNITS_VOLUME_DM3	12
#define UNITS_VOLUME_FT3	13
#define UNITS_VOLUME_IN3	14
#define UNITS_VOLUME_KM3	15
#define UNITS_VOLUME_M3	16
#define UNITS_VOLUME_MI3	17
#define UNITS_VOLUME_MM3	18
#define UNITS_VOLUME_YD3	19
#define UNITS_VOLUME_CUP	20
#define UNITS_VOLUME_DL	21
#define UNITS_VOLUME_DAL	22
#define UNITS_VOLUME_DR_UK	23
#define UNITS_VOLUME_DR_LQ_US	24
#define UNITS_VOLUME_GAL_UK	25
#define UNITS_VOLUME_DRY_GAL_US	26
#define UNITS_VOLUME_LIQUID_GAL_US	27
#define UNITS_VOLUME_GI_UK	28
#define UNITS_VOLUME_LIQUID_GI_US	29
#define UNITS_VOLUME_HL	30
#define UNITS_VOLUME_HHD	31
#define UNITS_VOLUME_HHD_UK	32
#define UNITS_VOLUME_MICROL	33
#define UNITS_VOLUME_ML	34
#define UNITS_VOLUME_MIN_UK	35
#define UNITS_VOLUME_MIN_US	36
#define UNITS_VOLUME_LQ_OZ_UK	37
#define UNITS_VOLUME_LQ_OZ_US	38
#define UNITS_VOLUME_PK_UK	39
#define UNITS_VOLUME_PK_US	40
#define UNITS_VOLUME_PT_UK	41
#define UNITS_VOLUME_DRY_PT	42
#define UNITS_VOLUME_LQ_PT	43
#define UNITS_VOLUME_QT	44
#define UNITS_VOLUME_DRY_QT	45
#define UNITS_VOLUME_LQ_QT	46
#define UNITS_VOLUME_S_UK	47
#define UNITS_VOLUME_S_US	48
#define UNITS_VOLUME_T_SHP_UK	49
#define UNITS_VOLUME_T_SHP_US	50
#define UNITS_VOLUME_TBSP_METRIC	51
#define UNITS_VOLUME_TBSP_US	52
#define UNITS_VOLUME_TSP_METRIC	53
#define UNITS_VOLUME_TSP_US	54
#define UNITS_VOLUME_T_REGS	55


typedef struct {
  uint16_t code;
  float scale;
  float offset;
} units_conversion_struct;

extern const units_conversion_struct massFlowUnitsTable[];
extern const units_conversion_struct nVolFlowUnitsTable[];
extern const units_conversion_struct massUnitsTable[];
extern const units_conversion_struct volumeUnitsTable[];

extern const EC_DATA_UNITS_CONV_TABLE EcToNativePresUnits[];
extern const EC_DATA_UNITS_CONV_TABLE EcToNativeFlowUnits[];
extern const EC_DATA_UNITS_CONV_TABLE EcToNativeTempUnits[];

extern const DATA_UNITS_CONV_TABLE UnitsConvPressure[];
extern const DATA_UNITS_CONV_TABLE UnitsConvTemperature[];
extern const DATA_UNITS_CONV_TABLE UnitsConvFlow[];

extern const DATA_UNITS_MAP_TABLE DnetToNativePressureUnits[];
extern const DATA_UNITS_MAP_TABLE DnetToNativeFlowUnits[];
extern const DATA_UNITS_MAP_TABLE DnetToNativeTemperatureUnits[];
extern const DATA_UNITS_MAP_TABLE DnetToNativeValveUnits[];

extern float ConvertUnits ( const DATA_UNITS_CONV_TABLE *table, float value, uint16_t sUnits, uint16_t dUnits );
extern uint16_t ConvertToTargetUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argNativeUnits );
extern int16_t ConvertToNativeUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argUnits, uint16_t *argNativeUnits );
extern uint32_t APPL_ConvertToEtherCATUnits( uint16_t argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr );
extern int16_t APPL_ConvertFromEtherCATUnits( uint32_t argEtherCATUnits, uint16_t *argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr );
extern float nativeToTarget(const float *table, uint8_t max, float in, uint8_t unit);
extern float uConvert( const units_conversion_struct *table, float source, uint16_t sUnits, uint16_t dUnits);
#ifdef __cplusplus
}
#endif

#endif
