#ifndef __DMX512_fixture_H
#define __DMX512_fixture_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_channel.h"

typedef struct{
	uint16_t id;
	uint16_t chStart;
	uint16_t chStop;
	uint16_t chCount;
	DMX512_channel_s **channels;
}DMX512_fixture_s;

DMX512_engine_err_e DMX512_fixture_init(DMX512_fixture_s *fixture, uint16_t chStart, uint16_t chStop);

#endif
