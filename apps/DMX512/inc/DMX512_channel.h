#ifndef __DMX512_channel_H
#define __DMX512_channel_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_defs.h"


typedef struct DMX512_channel DMX512_channel_s;
typedef DMX512_engine_err_e (*setValueFunct)(DMX512_channel_s *, uint8_t);

typedef struct DMX512_channel{
	uint16_t address;
	uint8_t value;
	setValueFunct setValue;
}DMX512_channel_s;

DMX512_engine_err_e DMX512_channel_init(DMX512_channel_s *channel, uint16_t address);

#endif
