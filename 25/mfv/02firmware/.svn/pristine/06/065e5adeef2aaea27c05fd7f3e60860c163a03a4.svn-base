/**
    @filename   libUnits.c
    @brief      This file contains units conversion utilities

*/
#include "main.h"
#include "libUnits.h"

const DATA_UNITS_MAP_TABLE DnetToNativePressureUnits[] = {
  { DNET_UNITS_COUNTS,   UNITS_COUNTS   },
  { DNET_UNITS_PERCENT,   UNITS_PERCENT   },
  // Pressure units
  { DNET_UNITS_KPA,       UNITS_KPA       },
  { DNET_UNITS_PSIA,      UNITS_PSIA      },
  { DNET_UNITS_TORR,      UNITS_TORR      },
  { DNET_UNITS_MTORR,     UNITS_MTORR     },
  { DNET_UNITS_MMHG0C,    UNITS_MMHG0C    },
  { DNET_UNITS_INHG0C,    UNITS_INHG0C    },
  { DNET_UNITS_BAR,       UNITS_BAR       },
  { DNET_UNITS_MBAR,      UNITS_MBAR      },
  { DNET_UNITS_ATM,       UNITS_ATM       },
  { DNET_UNITS_PA,        UNITS_PA        },
  { DNET_UNITS_PSIG,      UNITS_PSIG      },
  { 0x0000,               0x0000          }     // End of Table
};

const DATA_UNITS_MAP_TABLE DnetToNativeFlowUnits[] = {
  { DNET_UNITS_COUNTS,   UNITS_COUNTS   },
  { DNET_UNITS_PERCENT,   UNITS_PERCENT   },
  // Flow units
  { DNET_UNITS_SLM,       UNITS_SLM       },
//  { DNET_UNITS_CFM,       UNITS_CFM       },
  { DNET_UNITS_SCCM,      UNITS_SCCM      },
//  { DNET_UNITS_PACMS,     UNITS_PACMS     },
//  { DNET_UNITS_CMS,       UNITS_CMS       },
//  { DNET_UNITS_LS,        UNITS_LS        },
//  { DNET_UNITS_MLS,       UNITS_MLS       },    
//  { DNET_UNITS_LM,        UNITS_LM        },    
//  { DNET_UNITS_MLM,       UNITS_MLM       },
  { 0x0000,               0x0000          }     // End of Table
};
const DATA_UNITS_MAP_TABLE DnetToNativeTemperatureUnits[] = {
  { DNET_UNITS_COUNTS,   UNITS_COUNTS   },
  { DNET_UNITS_PERCENT,   UNITS_PERCENT   },
  // Temperature units
  { DNET_UNITS_CELSIUS,   UNITS_CELSIUS   },
  { DNET_UNITS_FAHRENHEIT,UNITS_FAHRENHEIT},
  { DNET_UNITS_KELVIN,    UNITS_KELVIN    },
  { DNET_UNITS_RANKINE,   UNITS_RANKINE   },
  { 0x0000,               0x0000          }     // End of Table
};

const DATA_UNITS_MAP_TABLE DnetToNativeValveUnits[] = {
  { DNET_UNITS_COUNTS,   UNITS_COUNTS   },
  { DNET_UNITS_PERCENT,   UNITS_PERCENT   },
  { 0x0000,               0x0000          }     // End of Table
};

const DATA_UNITS_CONV_TABLE UnitsConvPressure[] = {    
  { UNITS_PSIG,           6894.757F,  1.013250E+05F },
  { UNITS_PSIA,           6894.757F,  0.0F },
  { UNITS_KPA,            1000.0F,    0.0F },
  { UNITS_TORR,           133.3220F,  0.0F },
  { UNITS_MTORR,          0.1333220F, 0.0F },
  { UNITS_MMHG0C,         133.3220F,  0.0F },
  { UNITS_INHG0C,         3386.389F,  0.0F },
  { UNITS_BAR,            100000.0F,  0.0F },
  { UNITS_MBAR,           100.0F,     0.0F },
  { UNITS_ATM,            101325.0F,  0.0F },
  { UNITS_PA,             1.0F,       0.0F },
  { 0x0000,               1.0F,       0.0F } // end of table, use Pascal as defaults
};

const DATA_UNITS_CONV_TABLE UnitsConvTemperature[] = {
  { UNITS_CELSIUS,        1.8F,       4.916700E+02F },
  { UNITS_FAHRENHEIT,     1.0F,       4.596700E+02F },
  { UNITS_KELVIN,         1.8F,       0.0F },
  { UNITS_RANKINE,        1.0F,       0.0F },
  { 0x0000,               1.0F,       0.0F } // end of table, use Rankine as defaults
};

const DATA_UNITS_CONV_TABLE UnitsConvFlow[] = {
  { UNITS_SLM,            1000.0F,    0.0F },
  { UNITS_CFM,            28316.85F,  0.0F },
  { UNITS_SCCM,           1.0F,       0.0F },
  { UNITS_PACMS,          6.0E+07F,   0.0F },
  { UNITS_CMS,            6.0E+02F,   0.0F }, // This is wrong anyway...
  { UNITS_LS,             6.0E+04F,   0.0F },
  { UNITS_MLS,            60.0F,      0.0F },    
  { UNITS_LM,             1000.0F,    0.0F },    
  { UNITS_MLM,            1.0F,       0.0F },
  { 0x0000,               1.0F,       0.0F } // end of table, use SCCM as defaults
};    

const EC_DATA_UNITS_CONV_TABLE EcToNativePresUnits[] = {
	// Pressure units
	{ 0x00A10000, DATAUNIT_TORR    },
	{ 0x00A20000, DATAUNIT_ATM     },
	{ 0x00A30000, DATAUNIT_PSIA    },
	{ 0x00A40000, DATAUNIT_PSIG    },
	{ 0x004E0000, DATAUNIT_BAR     },
	{ 0xFD4E0000, DATAUNIT_MBAR    },
	{ 0x00220000, DATAUNIT_PA      },
	{ 0x03220000, DATAUNIT_KPA     },
	// End of List
	{ 0x00000000, 0x0000        }
};
const EC_DATA_UNITS_CONV_TABLE EcToNativeFlowUnits[] = {
	{ 0x00A00000, DATAUNIT_SCCM    },
	{ 0x03A00000, DATAUNIT_SLM     },
	// End of List
	{ 0x00000000, 0x0000        }
};
const EC_DATA_UNITS_CONV_TABLE EcToNativeTempUnits[] = {
	{ 0x002D0000, DATAUNIT_CELSIUS },
	{ 0x00050000, DATAUNIT_KELVIN  },
	// End of List
	{ 0x00000000, 0x0000        }
};

/**
*/
static void ScaleTable ( const DATA_UNITS_CONV_TABLE *table, float *scale, float *offset, uint16_t dUnits ) {
    uint8_t i=0;
    uint8_t found = 0;
    
    while ( ( table[i].UnitsCode != 0x0000 ) & ( !found ) ) {
        if ( table[i].UnitsCode == dUnits ) {
            *scale = table[i].scale;
            *offset = table[i].offset;
            found = 1;
        } else {
            i++;
        }        
    }
    
    if ( !found ) {
        *scale = table[i].scale;
        *offset = table[i].offset;    
    }
}

/** 
*/
float ConvertUnits ( const DATA_UNITS_CONV_TABLE *table, float value, uint16_t sUnits, uint16_t dUnits ) {
    float sScale, sOffset, dScale, dOffset;
    float baseValue;
    ScaleTable ( table, &sScale, &sOffset, sUnits ); // scale values to convert source units
    ScaleTable ( table, &dScale, &dOffset, dUnits ); // scale values to convert target units
    baseValue = value * sScale + sOffset;
    return ( baseValue - dOffset ) / dScale;
}

/**
*/
uint16_t ConvertToTargetUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argNativeUnits ) {
    uint16_t locNativeUnits;
    uint8_t i = 0;

    // Loop thru table until a match is found OR until end of table is reached
    while ( 1 ) {
        locNativeUnits = table[i].NativeUnitsCode;

        if ( locNativeUnits == argNativeUnits ) {
            // Match found, return the DeviceNet equivalent
            return table[i].UnitsCode;

        } else {
            if ( locNativeUnits == 0 ) {
                // END of Table, no match, return 0
                return 0;
            }
        }

        // Not found, go to next entry in table
        i++;
    }
}

int16_t ConvertToNativeUnits ( const DATA_UNITS_MAP_TABLE *table, uint16_t argUnits, uint16_t *argNativeUnits ) {
    uint16_t locEcUnits;
    uint8_t i = 0;

    // Loop thru table until a match is found OR until end of table is reached
    while ( 1 ) {
        locEcUnits = table[i].UnitsCode;

        if ( locEcUnits == 0 ) {
            // END of Table, no match, return error
            return -1;
        }
        else {
            if ( locEcUnits == argUnits ) {
                // Match found, return the Ethercat equivalent
                *argNativeUnits = table[i].NativeUnitsCode;
                return 0;

            }
        }
        // Not found, go to next entry in table
        i++;
    }
}

uint32_t APPL_ConvertToEtherCATUnits( uint16_t argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr ) {
	uint8_t i = 0;
	// Loop thru table until a match is found OR until end of table is reached
	while( 0 != tablePtr[i].EcUnitsCode ) {
		if ( tablePtr[i].NativeUnitsCode == argNativeUnits ) {
			// Match found, return the Ethercat equivalent
			return tablePtr[i].EcUnitsCode;
		}
		i++;
	}
	// END of Table, no match, return 0
	return 0;
}

int16_t APPL_ConvertFromEtherCATUnits( uint32_t argEtherCATUnits, uint16_t *argNativeUnits, const EC_DATA_UNITS_CONV_TABLE *tablePtr ) {
	uint8_t i = 0;
	// Loop thru table until a match is found OR until end of table is reached
	while( 0 != tablePtr[i].EcUnitsCode ) {
		if( tablePtr[i].EcUnitsCode == argEtherCATUnits ) {
			// Match found, return the Ethercat equivalent
			*argNativeUnits = tablePtr[i].NativeUnitsCode;
			return 0;
		}
		i++;
	}
	// END of Table, no match, return error
	return -1;
}

/**
*/
const units_conversion_struct massFlowUnitsTable[] = {
  { UNITS_MASSFLOW_G_H, 	      1.0f, 0.0f },           // g/h
  { UNITS_MASSFLOW_G_DAY,	    24.0f, 0.0f },          // g/day
  { UNITS_MASSFLOW_G_MIN,	    0.0167f, 0.0f },        // g/min
  { UNITS_MASSFLOW_G_MON30,    720.0f, 0.0f },         // g/mon (30 days)
  { UNITS_MASSFLOW_G_MON31,    744.0f, 0.0f },         // g/mon (31 days)
  { UNITS_MASSFLOW_G_SEC,	    0.000278f, 0.0f },      // g/sec
  { UNITS_MASSFLOW_KG_DAY,     0.024f, 0.0f },         // kg/day
  { UNITS_MASSFLOW_KG_H,       0.001f, 0.0f },         // kg/h
  { UNITS_MASSFLOW_KG_MIN,     0.000016667f, 0.0f },   // kg/min
  { UNITS_MASSFLOW_KG_MON30,   0.72f, 0.0f },          // kg/mon (30 days)
  { UNITS_MASSFLOW_KG_MON31,   0.744f, 0.0f },         // kg/mon (31 days)
  { UNITS_MASSFLOW_KG_SEC,     2.7778e-7f, 0.0f },     // kg/sec
  { UNITS_MASSFLOW_MG_DAY,     24000.0f, 0.0f },       // mg/day
  { UNITS_MASSFLOW_MG_H,       1000.0f, 0.0f },        // mg/h
  { UNITS_MASSFLOW_MG_MIN,     16.6667f, 0.0f },       // mg/min
  { UNITS_MASSFLOW_MG_MON30,   720000.0f, 0.0f },      // mg/mon (30 days)
  { UNITS_MASSFLOW_MG_MON31,	744000.0f, 0.0f },      // mg/mon (31 days)
  { UNITS_MASSFLOW_MG_SEC,	0.2778f, 0.0f },        // mg/sec
  { UNITS_MASSFLOW_OZ_DAY,	0.8466f, 0.0f },        // oz/day
  { UNITS_MASSFLOW_OZ_H,	0.0353f, 0.0f },        // oz/h
  { UNITS_MASSFLOW_OZ_MIN,	0.0006f, 0.0f },        // oz/min
  { UNITS_MASSFLOW_OZ_MON30,	25.3973f, 0.0f },       // oz/mon (30 days)
  { UNITS_MASSFLOW_OZ_MON31,	26.2438f, 0.0f },       // oz/mon (31 days)
  { UNITS_MASSFLOW_OZ_SEC,	0.000009798f, 0.0f },   // oz/sec
  { UNITS_MASSFLOW_LB_DAY,	0.0529f, 0.0f },        // lb/day
  { UNITS_MASSFLOW_LB_H,	0.0022f, 0.0f },        // lb/h
  { UNITS_MASSFLOW_LB_MIN,	0.000036744f, 0.0f },   // lb/min
  { UNITS_MASSFLOW_LB_MON30,	1.5873f, 0.0f },        // lb/mon (30 days)
  { UNITS_MASSFLOW_LB_MON31,	1.6402f, 0.0f },        // lb/mon (31 days)
  { UNITS_MASSFLOW_LB_SEC,	6.124e-7f, 0.0f },      // lb/sec
  { UNITS_MASSFLOW_T_DAY,	0.000024f, 0.0f },      // t/d
  { UNITS_MASSFLOW_T_H, 	0.000001f, 0.0f },      // t/h
  { UNITS_MASSFLOW_T_MIN,	1.6667e-8f, 0.0f },     // t/min
  { UNITS_MASSFLOW_T_MON30,	0.00072f, 0.0f },       // t/mon (30 days)
  { UNITS_MASSFLOW_T_MON31,	0.00074f, 0.0f },       // t/mon (31 days)
  { UNITS_MASSFLOW_T_SEC,	2.7778e-10f, 0.0f },    // t/sec
  { UNITS_MASSFLOW_TON_DAY_UK,	0.000023621f, 0.0f },   // ton(UK)/day
  { UNITS_MASSFLOW_TON_H_UK,	9.8421e-7f, 0.0f },     // ton(UK)/h
  { UNITS_MASSFLOW_TON_MIN_UK,	1.6403e-8f, 0.0f },     // ton(UK)/min
  { UNITS_MASSFLOW_TON_MON30_UK,	0.00071f, 0.0f },       // ton(UK)/mon (30 days)        
  { UNITS_MASSFLOW_TON_MON31_UK,	0.00073f, 0.0f },       // ton(UK)/mon (31 days)
  { UNITS_MASSFLOW_TON_SEC_UK,	2.7339e-10f, 0.0f },    // ton(UK)/sec  
  { UNITS_MASSFLOW_TON_DAY_US,	0.000026455f, 0.0f },   // ton(UK)/day
  { UNITS_MASSFLOW_TON_H_US,	0.0000011023f, 0.0f },  // ton(UK)/h
  { UNITS_MASSFLOW_TON_MIN_US,	1.8372e-8f, 0.0f },     // ton(UK)/min
  { UNITS_MASSFLOW_TON_MON30_US,	0.0008f, 0.0f },        // ton(UK)/mon (30 days)
  { UNITS_MASSFLOW_TON_MON31_US,	0.0008f, 0.0f },        // ton(UK)/mon (31 days)
  { UNITS_MASSFLOW_TON_SEC_US,	3.062e-10f, 0.0f },     // ton(UK)/sec
  { 0xFFFF,   1.0f,   0.0f }  // NULL
};

/**
*/
const units_conversion_struct nVolFlowUnitsTable[] = {
  { UNITS_VOLFLOW_L_H, 	  1000.0f, 0.0f },        // L/h
  { UNITS_VOLFLOW_BBL_DAY, 	  201.27f, 0.0f },        // bbl/d
  { UNITS_VOLFLOW_BBL_H, 	  8.39f, 0.0f },          // bbl/h
  { UNITS_VOLFLOW_BBL_MIN, 	  0.14f, 0.0f },          // bbl/min
  { UNITS_VOLFLOW_BO_DAY, 	  150.96f, 0.0f },        // bo/d
  { UNITS_VOLFLOW_BO_H, 	  6.29f, 0.0f },          // bo/h
  { UNITS_VOLFLOW_BO_MIN, 	  0.1f, 0.0f },           // bo/min
  { UNITS_VOLFLOW_CCM_H, 	  1000000.0f, 0.0f },     // cm^3/h
  { UNITS_VOLFLOW_CCM_MIN, 	  16666.67f, 0.0f },      // cm^3/min
  { UNITS_VOLFLOW_CCM_SEC, 	  277.78f, 0.0f },        // cm^3/sec
  { UNITS_VOLFLOW_CFT_MIN, 	  0.59f, 0.0f },          // ft^3/min
  { UNITS_VOLFLOW_CFT_SEC, 	  0.01f, 0.0f },          // ft^3/sec
  { UNITS_VOLFLOW_CIN_MIN, 	  1017.06f, 0.0f },       // in^3/min
  { UNITS_VOLFLOW_CIN_SEC, 	  16.95f, 0.0f },         // in^3/sec
  { UNITS_VOLFLOW_CM_DAY, 	  24.0f, 0.0f },          // m^3/day
  { UNITS_VOLFLOW_CM_MIN, 	  0.02f, 0.0f },          // m^3/min
  { UNITS_VOLFLOW_CM_MON30, 	  720.0f, 0.0f },         // m^3/month (30days)
  { UNITS_VOLFLOW_CM_SEC, 	  0.000278f, 0.0f },      // m^3/sec
  { UNITS_VOLFLOW_CMM_sec, 	  277777.78f, 0.0f },     // mm^3/sec
  { UNITS_VOLFLOW_CYD_H, 	  1.31f, 0.0f },          // yd^3/h
  { UNITS_VOLFLOW_CYD_MIN, 	  0.02f, 0.0f },          // yd^3/min
  { UNITS_VOLFLOW_CYD_SEC, 	  0.000363f, 0.0f },      // yd^3/sec
  { UNITS_VOLFLOW_GAL_DAY, 	  6340.13f, 0.0f },       // gal/day
  { UNITS_VOLFLOW_GAL_H, 	  264.17f, 0.0f },        // gal/h
  { UNITS_VOLFLOW_GAL_MIN, 	  4.4f, 0.0f },           // gal/min
  { UNITS_VOLFLOW_L_DAY, 	  24000.0f, 0.0f },       // L/day
  { UNITS_VOLFLOW_CM_H, 	  1.0f, 0.0f },           // m^3/h
  { UNITS_VOLFLOW_L_MIN, 	  16.67f, 0.0f },         // L/min
  { UNITS_VOLFLOW_L_SEC, 	  0.28f, 0.0f },          // L/sec
  { UNITS_VOLFLOW_ML_SEC, 	  277.78f, 0.0f },        // mL/sec
  { UNITS_VOLFLOW_PT_SEC, 	  0.59f, 0.0f },          // pt/sec
  { UNITS_VOLFLOW_QT_SEC, 	  0.25f, 0.0f },           // qt/sec
  { 0xFFFF,   1.0f,   0.0f }  // NULL
};
  
/**
*/
const units_conversion_struct massUnitsTable[] = {
  { UNITS_MASS_G, 1, 0.0f },
  { UNITS_MASS_CARAT_METRIC, 5, 0.0f },
  { UNITS_MASS_CARAT_US_UK, 4.82, 0.0f },
  { UNITS_MASS_DAG, 0.1, 0.0f },
  { UNITS_MASS_DR, 0.56, 0.0f },
  { UNITS_MASS_DR_AP, 0.26, 0.0f },
  { UNITS_MASS_GR, 15.43, 0.0f },
  { UNITS_MASS_GR_AP, 15.43, 0.0f },
  { UNITS_MASS_HUNDWGHT_L, 0.0000197, 0.0f },
  { UNITS_MASS_HUNDWGHT_S, 0.000022, 0.0f },
  { UNITS_MASS_KG, 0.001, 0.0f },
  { UNITS_MASS_MG, 1000, 0.0f },
  { UNITS_MASS_OZ, 0.04, 0.0f },
  { UNITS_MASS_OZ_AP, 0.03, 0.0f },
  { UNITS_MASS_OZ_TROY, 0.03, 0.0f },
  { UNITS_MASS_DWT, 0.64, 0.0f },
  { UNITS_MASS_LB, 0.0022, 0.0f },
  { UNITS_MASS_LB_AP, 0.00268, 0.0f },
  { UNITS_MASS_LB_TROY, 0.00268, 0.0f },
  { UNITS_MASS_QT_AV_UK, 0.000079, 0.0f },
  { UNITS_MASS_QT_AV_US, 0.000088, 0.0f },
  { UNITS_MASS_Q, 0.00001, 0.0f },
  { UNITS_MASS_S, 0.77, 0.0f },
  { UNITS_MASS_SL, 0.000069, 0.0f },
  { UNITS_MASS_ST, 0.000157, 0.0f },
  { UNITS_MASS_TON_UK, 9.84e-7, 0.0f },
  { UNITS_MASS_TON_METRIC, 0.000001, 0.0f },
  { UNITS_MASS_TON_US, 0.0000011, 0.0f },
  { 0xFFFF,   1.0f,   0.0f }  // NULL
};

/**
*/
const units_conversion_struct volumeUnitsTable[] = {
  { UNITS_VOLUME_AF,0.00081,0.0f},
  { UNITS_VOLUME_AC_IN,0.01,0.0f},
  { UNITS_VOLUME_DRY_BBL,8.65,0.0f},
  { UNITS_VOLUME_LIQUID_BBL,8.39,0.0f},
  { UNITS_VOLUME_BO,6.29,0.0f},
  { UNITS_VOLUME_BU_UK,27.5,0.0f},
  { UNITS_VOLUME_BU_US,28.38,0.0f},
  { UNITS_VOLUME_CL,100000,0.0f},
  { UNITS_VOLUME_CHD,0.76,0.0f},
  { UNITS_VOLUME_CD,0.28,0.0f},
  { UNITS_VOLUME_CM3,1000000,0.0f},
  { UNITS_VOLUME_DM3,1000,0.0f},
  { UNITS_VOLUME_FT3,35.31,0.0f},
  { UNITS_VOLUME_IN3,61023.74,0.0f},
  { UNITS_VOLUME_KM3,1.00e-9,0.0f},
  { UNITS_VOLUME_M3,1,0.0f},
  { UNITS_VOLUME_MI3,2.40e-10,0.0f},
  { UNITS_VOLUME_MM3,1.00e+9,0.0f},
  { UNITS_VOLUME_YD3,1.31,0.0f},
  { UNITS_VOLUME_CUP,4226.75,0.0f},
  { UNITS_VOLUME_DL,10000,0.0f},
  { UNITS_VOLUME_DAL,100,0.0f},
  { UNITS_VOLUME_DR_UK,281560.64,0.0f},
  { UNITS_VOLUME_DR_LQ_US,270512.18,0.0f},
  { UNITS_VOLUME_GAL_UK,219.97,0.0f},
  { UNITS_VOLUME_DRY_GAL_US,227.02,0.0f},
  { UNITS_VOLUME_LIQUID_GAL_US,264.17,0.0f},
  { UNITS_VOLUME_GI_UK,7039.02,0.0f},
  { UNITS_VOLUME_LIQUID_GI_US,8453.51,0.0f},
  { UNITS_VOLUME_HL,10,0.0f},
  { UNITS_VOLUME_HHD,4.19,0.0f},
  { UNITS_VOLUME_HHD_UK,4.19,0.0f},
  { UNITS_VOLUME_L,1000,0.0f},
  { UNITS_VOLUME_MICROL,1.00e+9,0.0f},
  { UNITS_VOLUME_ML,1000000,0.0f},
  { UNITS_VOLUME_MIN_UK,1.69e+7,0.0f},
  { UNITS_VOLUME_MIN_US,1.62e+7,0.0f},
  { UNITS_VOLUME_LQ_OZ_UK,35195.08,0.0f},
  { UNITS_VOLUME_LQ_OZ_US,33814.02,0.0f},
  { UNITS_VOLUME_PK_UK,109.98,0.0f},
  { UNITS_VOLUME_PK_US,113.51,0.0f},
  { UNITS_VOLUME_PT_UK,1759.75,0.0f},
  { UNITS_VOLUME_DRY_PT,1816.17,0.0f},
  { UNITS_VOLUME_LQ_PT,2113.38,0.0f},
  { UNITS_VOLUME_QT,879.88,0.0f},
  { UNITS_VOLUME_DRY_QT,908.08,0.0f},
  { UNITS_VOLUME_LQ_QT,1056.69,0.0f},
  { UNITS_VOLUME_S_UK,844681.91,0.0f},
  { UNITS_VOLUME_S_US,811536.54,0.0f},
  { UNITS_VOLUME_T_SHP_UK,0.84,0.0f},
  { UNITS_VOLUME_T_SHP_US,0.88,0.0f},
  { UNITS_VOLUME_TBSP_METRIC,66666.67,0.0f},
  { UNITS_VOLUME_TBSP_US,67628.05,0.0f},
  { UNITS_VOLUME_TSP_METRIC,200000,0.0f},
  { UNITS_VOLUME_TSP_US,202884.14,0.0f},
  { UNITS_VOLUME_T_REGS,0.35,0.0f},
  { 0xFFFF,   1.0f,   0.0f }  // NULL
};

/**
  @brief converting native units g/h to target units
*/
float nativeToTarget(const float *table, uint8_t max, float in, uint8_t unit) {
  unit = unit >= max ? 0 : unit;
  return in * table[unit];
}

/**
*/
static void sTable(const units_conversion_struct *table, float *scale, float *offset, uint16_t code) {
  uint8_t i = 0;
  bool found = false;
  
  while ((table[i].code != 0xFFFF) & (!found)) {
    if (table[i].code == code) {
      *scale = table[i].scale;
      *offset = table[i].offset;
      found = true;
    } else {
      i++;
    }
  }
  
  if (!found) {
    *scale = table[i].scale;
    *offset = table[i].offset;
  }
}

/**
*/
float uConvert( const units_conversion_struct *table, float source, uint16_t sUnits, uint16_t dUnits) {
  float sScale, sOffset, dScale, dOffset;
  float bValue;
  
  sTable(table, &sScale, &sOffset, sUnits); // retrieve scale and offset values of source unit
  sScale = 1.0f/sScale;
  sTable(table, &dScale, &dOffset, dUnits); // retrieve scale and offset values of destination units
  dScale = 1.0f/dScale;
  bValue = source * sScale + sOffset;
  return (bValue - dOffset)/dScale;
    
}
