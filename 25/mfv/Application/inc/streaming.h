#ifndef __STREAMING_H
#define __STREAMING_H

#include <stdint.h>
#include "main.h"
#include "protocol.h"

#define MAX_SIGNALS   1

// streaming
extern void install_stream (uint8_t idx, uint8_t sigidx);
extern void streamsig(api_instance *api);

#endif
