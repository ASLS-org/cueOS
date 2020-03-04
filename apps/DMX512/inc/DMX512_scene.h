#ifndef __DMX512_SCENE_H
#define __DMX512_SCENE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"
#include "DMX512_preset.h"
#include "DMX512_defs.h"

typedef struct DMX512_fixture_preset DMX512_fixture_preset_s;

typedef struct DMX512_fixture_preset{
	uint16_t id;
	uint16_t count;
	uint16_t *channels;
	uint8_t *values;
}DMX512_fixture_preset_s;

typedef struct DMX512_scene{
	uint16_t id;
	uint16_t fadein_time;
	uint16_t fadeout_time;
	uint16_t fixture_preset_count;
	DMX512_fixture_preset_s *_fixture_presets;
}DMX512_scene_s;

DMX512_scene_s DMX512_scene_init(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time);
DMX512_engine_err_e DMX512_scene_add(DMX512_scene_s *this, uint16_t id,  uint16_t *channels, uint8_t *values, uint16_t count);
DMX512_engine_err_e DMX512_scene_del(DMX512_scene_s *this, uint16_t id);
DMX512_fixture_preset_s *DMX512_scene_get(DMX512_scene_s *this, uint16_t id);

#endif
