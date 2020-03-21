#ifndef DMX512_CHASER_STEP_H_
#define DMX512_CHASER_STEP_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "DMX512_defs.h"
#include "DMX512_scene.h"
#include "DMX512_utils.h"

#define DMX512_CHASER_STEP_DEFAULT {NULL, 0, 0, 0, {}, DMX512_CHASER_STEP_UNINITIALISED, DMX512_CHASER_STEP_IDLE}

typedef enum{
	DMX512_CHASER_STEP_UNINITIALISED,
	DMX512_CHASER_STEP_INITIALISED
}DMX512_fixture_chaser_step_status_e;

typedef enum{
	DMX512_CHASER_STEP_FADE_IN,
	DMX512_CHASER_STEP_HOLD,
	DMX512_CHASER_STEP_FADE_OUT,
	DMX512_CHASER_STEP_IDLE,
}DMX512_fixture_chaser_step_state_e;


typedef struct{
	DMX512_scene_s *scene;
	uint16_t fadein_time;
	uint16_t fadeout_time;
	uint16_t hold_time;
	DMX512_utils_mschronometer_s mschronometer;
	DMX512_fixture_chaser_step_status_e status;
	DMX512_fixture_chaser_step_state_e state;
}DMX512_chaser_step_s;


DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein_time, uint16_t fadeout_time, uint16_t hold_time);
void DMX512_chaser_step_manage(DMX512_chaser_step_s *this);
void DMX512_chaser_step_start(DMX512_chaser_step_s *this);
void DMX512_chaser_step_stop(DMX512_chaser_step_s *this);

#endif
