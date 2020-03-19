#ifndef DMX512_FIXTURE_PRESET_H_
#define DMX512_FIXTURE_PRESET_H_

#include <stdint.h>
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

#define DMX512_FIXTURE_PRESET_DEFAULT {NULL, 0, NULL, NULL, DMX512_FIXTURE_PRESET_UNINITIALISED}

typedef enum{
	DMX512_FIXTURE_PRESET_UNINITIALISED,
	DMX512_FIXTURE_PRESET_INITIALISED
}DMX512_fixture_preset_status;

typedef struct DMX512_fixture_preset{
	DMX512_fixture_s *fixture;
	uint16_t ch_count;
	uint16_t *channels;
	uint8_t *values;
	DMX512_fixture_preset_status status;
}DMX512_fixture_preset_s;


DMX512_fixture_preset_s DMX512_fixture_preset_new(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values);

#endif
