#ifndef __DMX512_fixture_H
#define __DMX512_fixture_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_channel.h"
#include "DMX512_defs.h"


typedef struct DMX512_fixture DMX512_fixture_s;
typedef DMX512_engine_err_e (*fixtureSetFunc)(DMX512_fixture_s *, uint16_t, uint8_t);
typedef void (*freeFunc)(DMX512_fixture_s *);

typedef struct DMX512_fixture{
	uint16_t id;
	uint16_t chStart;
	uint16_t chStop;
	uint16_t chCount;
	DMX512_channel_s **channels;
	fixtureSetFunc set;
	freeFunc free;
}DMX512_fixture_s;

DMX512_engine_err_e DMX512_fixture_init(DMX512_fixture_s *fixture, uint16_t id, uint16_t chStart, uint16_t chStop);

#endif
