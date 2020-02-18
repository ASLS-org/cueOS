#ifndef __DMX512_fixture_H
#define __DMX512_fixture_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_channel.h"
#include "DMX512_defs.h"


typedef struct DMX512_fixture DMX512_fixture_s;

typedef struct DMX512_fixture{
	uint16_t id;
	uint16_t ch_start;
	uint16_t ch_stop;
	//uint16_t chCount;
	//DMX512_fixture_s *_next;
}DMX512_fixture_s;

DMX512_fixture_s DMX512_fixture_init(uint16_t id, uint16_t ch_start, uint16_t ch_stop);
DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value);

#endif
