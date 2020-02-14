#ifndef __DMX512_SCENE_H
#define __DMX512_SCENE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"
#include "DMX512_preset.h"
#include "DMX512_defs.h"

#define DMX512_SCENE_HASHTABLE_SIZE 8

typedef struct DMX512_fixture_preset DMX512_fixture_preset_s;

typedef struct DMX512_fixture_preset{
	uint16_t fixture_id;
	uint8_t *fixture_values;
	DMX512_fixture_preset_s *_next;
}DMX512_fixture_preset_s;

typedef struct DMX512_scene{
	uint16_t id;
	DMX512_fixture_preset_s *_fixture_presets;
}DMX512_scene_s;

DMX512_scene_s *DMX512_scene_init(uint16_t id);
DMX512_engine_err_e DMX512_scene_add(DMX512_scene_s *this, uint16_t id, uint8_t *values);

#endif
