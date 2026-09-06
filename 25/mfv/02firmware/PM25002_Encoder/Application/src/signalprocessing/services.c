#include <stdint.h>
#include "meter.h"
#include "controller.h"
#include "utilities.h"
#include "digitalio.h"
#include "main.h"
#include "services.h"

typedef struct {
  bool active;
  float timer;
  uint16_t idx;
  float sums;
  float average;
} zero_process;

static zero_process pmz = {0};

/**
  @brief Common function to zero process expire.
*/
bool calcZero(zero_process *z, float value) {

  if (z->active) {
    z->sums += value;
    z->idx++;
    z->average = z->sums/z->idx;
    z->timer -= (float)SIGPROC_PERIOD;
    if (z->timer <= 0.0f) {
      z->active = false;   
      return true;
    }
    return false;
  } else {
    return false;
  }  
}

/**
  @brief This function handles all zeroing services for pm, dp, meter
*/
void zeroService(void) {
  
  if (getControllerState() == CTRL_OFF) {    
    
  } else {
    // force all process related to zeroing sensors are disabled or turned off since we are in contorl state
  }
}
