#ifndef __DMX512_PRESET_H_
#define __DMX512_PRESET_H_

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

typedef struct{
	uint16_t id;
	uint8_t *values;
	uint16_t *channels;
	DMX512_fixture_s *fixture;
	DMX512_engine_err_e (*update)();
}DMX512_preset_s;


DMX512_engine_err_e DMX512_preset_init(DMX512_preset_s *preset, uint16_t fixtureId, uint8_t *values);

#endif
