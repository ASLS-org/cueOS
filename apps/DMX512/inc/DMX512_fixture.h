#ifndef DMX512_fixture_H
#define DMX512_fixture_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_defs.h"


typedef struct DMX512_fixture DMX512_fixture_s;

typedef struct DMX512_fixture{
	uint16_t id;
	uint16_t addr;
	uint16_t ch_count;
	uint16_t ch_stop;
}DMX512_fixture_s;

DMX512_fixture_s DMX512_fixture_new(uint16_t id, uint16_t addr, uint16_t ch_count);

#endif
