#include <string.h>
#include "protocol.h"
#include "crc16.h"
#include "nvmem.h"
#include "main.h"
#include "streaming.h"

float fxinput = 0.0f;
float *signals[MAX_SIGNALS] =
{
  0,
};

/**
*/
void install_stream (uint8_t idx, uint8_t sigidx) {
  usbd_api.stream.signals[idx] = signals[sigidx];
}

/**
*/
void streamsig(api_instance *api) {
  uint8_t prep[sizeof(float)*MAX_STREAMABLE_SIGNALS];
  uint8_t sigcount = 0;
  static uint8_t divctr = 0;
  // are we streaming signals?
  if (api->stream.enabled) {
    if (divctr == 0) {
      if (api->stream.count) {
        // assemble th epackets per signal assignment (if there is a signal assigned)
        for (uint8_t i=0; i<MAX_STREAMABLE_SIGNALS; i++) {
          if (api->stream.signals[i] != 0) {
            memcpy(&prep[i*sizeof(float)], (uint8_t *)api->stream.signals[i], sizeof(float));
            sigcount++;
          }
        }
        // transmit to USB channel
        comms_send_bytes(api->comms_driver, prep, sigcount*sizeof(float));
        divctr = api->stream.spdivider-1; // reload divider
        api->stream.count--;
      } else {
        api->stream.enabled = 0;
      }
    } else {
      divctr--;
    }
  }
}
