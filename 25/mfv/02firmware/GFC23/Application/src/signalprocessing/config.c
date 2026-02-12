#include <stdlib.h>
#include "config.h"

CONFIG_STRU config_dat;
void config_init()
{
    config_dat.cmd = IDLE_STATE;
    config_dat.flow_mode = HIGH_FLOW;
    config_dat.pressure_error = 0.0094;  //set steady pressure error
    config_dat.vac_val = 2.04; //vacuum value
    config_dat.pressure_val = 100;//target pressure
    config_dat.flow_setpoint = 100;//target flow
    config_dat.p20_pressure_error = 0.001;
    config_dat.p20_pressure_error = 0.015;//0.0093
    config_dat.tC = 0.8;

}
CONFIG_STRU *get_config()
{
    return &config_dat;
}
