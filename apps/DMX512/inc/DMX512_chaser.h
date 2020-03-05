#ifndef __DMX512_CHASER_H
#define __DMX512_CHASER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_chaser_step.h"
#include "DMX512_defs.h"

#define DMX512_CHASER_DEFAULT {0, NULL, CHASER_STEP_MODE_LOOP, CHASER_STEP_DIRECTION_FORWARD, 0}

typedef enum{
	CHASER_STEP_MODE_LOOP,
	CHASER_STEP_MODE_SINGLE_SHOT,
	CHASER_STEP_MODE_PING_PONG,
	CHASER_STEP_MODE_RANDOM
}DMX512_chaser_step_mode;

typedef enum{
	CHASER_STEP_DIRECTION_FORWARD,
	CHASER_STEP_DIRECTION_BACKWARD
}DMX512_chaser_step_direction;

typedef struct{
	uint16_t id;
	DMX512_chaser_step_s *steps;
	DMX512_chaser_step_mode mode;
	DMX512_chaser_step_direction dir;
	uint16_t step_count;
}DMX512_chaser_s;

DMX512_chaser_s DMX512_chaser_init(uint16_t id, DMX512_chaser_step_mode mode, DMX512_chaser_step_direction dir);
DMX512_engine_err_e DMX512_chaser_add(DMX512_chaser_s *this, DMX512_chaser_step_s step);
DMX512_engine_err_e DMX512_chaser_del(DMX512_chaser_s *this, uint16_t id);
DMX512_fixture_preset_s *DMX512_chaser_get(DMX512_chaser_s *this, uint16_t id);

#endif
