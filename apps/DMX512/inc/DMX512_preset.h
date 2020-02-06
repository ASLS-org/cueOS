#ifndef __DMX512_PRESET_H_
#define __DMX512_PRESET_H_

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_fixture.h"
#include "DMX512_defs.h"


typedef struct DMX512_preset DMX512_preset_s;
typedef DMX512_engine_err_e(*updFunc)(DMX512_preset_s *, uint16_t *, uint8_t *);
typedef DMX512_engine_err_e(*applyFunc)(DMX512_preset_s *);

typedef struct DMX512_preset{
	uint16_t id;
	uint16_t chCount;
	uint8_t *values;
	uint16_t *channels;
	DMX512_fixture_s *fixture;
	updFunc update;
	applyFunc apply;
}DMX512_preset_s;


DMX512_engine_err_e DMX512_preset_init(DMX512_preset_s *preset, uint16_t fixtureId, uint16_t *channels, uint8_t *values);

#endif
