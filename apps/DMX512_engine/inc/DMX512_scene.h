#ifndef DMX512_SCENE_H
#define DMX512_SCENE_H

#include <stdint.h>

#include "DMX512_defs.h"
#include "DMX512_fixture_preset.h"
#include "DMX512_utils.h"

#define DMX512_SCENE_DEFAULT {0, 0, 0, 0, NULL, {}, DMX512_SCENE_IDLE, DMX512_SCENE_UNINITIALISED}

typedef enum{
	DMX512_SCENE_UNINITIALISED,
	DMX512_SCENE_INITIALISED
}DMX512_scene_status_e;

typedef enum{
	DMX512_SCENE_IDLE,
	DMX512_SCENE_FADEIN,
	DMX512_SCENE_FADEOUT,
}DMX512_scene_state_e;

typedef struct DMX512_scene{
	uint16_t id;
	uint16_t fadein_time;
	uint16_t fadeout_time;
	uint16_t preset_count;
	DMX512_fixture_preset_s *presets;
	DMX512_utils_mschronometer_s mschronometer;
	DMX512_scene_state_e state;
	DMX512_scene_status_e status;
}DMX512_scene_s;

DMX512_scene_s DMX512_scene_new(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time);
DMX512_engine_err_e DMX512_scene_add_preset(DMX512_scene_s *this, DMX512_fixture_preset_s fp);
DMX512_engine_err_e DMX512_scene_del_preset(DMX512_scene_s *this, uint16_t id);
DMX512_fixture_preset_s *DMX512_scene_get_preset(DMX512_scene_s *this, uint16_t id);
void DMX512_scene_start(DMX512_scene_s *this);
void DMX512_scene_stop(DMX512_scene_s *this);
void DMX512_scene_manage(DMX512_scene_s *this);

#endif
