#ifndef __DMX512_channel_H
#define __DMX512_channel_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_defs.h"

typedef struct{
	uint16_t address;
	uint8_t value;
	void(*setValue)();
}DMX512_channel_s;

DMX512_engine_err_e DMX512_channel_init(DMX512_channel_s *channel, uint16_t address);

#endif
