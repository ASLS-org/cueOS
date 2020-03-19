#ifndef DMX512_CHASER_H
#define DMX512_CHASER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_chaser_step.h"
#include "DMX512_defs.h"

#define DMX512_CHASER_DEFAULT {0, NULL, CHASER_MODE_LOOP, CHASER_DIRECTION_FORWARD, CHASER_IDLE, 0, 0}

typedef enum{
	CHASER_MODE_LOOP,
	CHASER_MODE_SINGLE_SHOT,
	CHASER_MODE_PING_PONG,
	CHASER_MODE_RANDOM
}DMX512_chaser_step_mode_e;

typedef enum{
	CHASER_DIRECTION_FORWARD,
	CHASER_DIRECTION_BACKWARD
}DMX512_chaser_step_direction_e;

typedef enum{
	CHASER_PLAYING,
	CHASER_IDLE
}DMX512_chaser_step_state_e;


typedef struct{
	uint16_t id;
	DMX512_chaser_step_s *steps;
	DMX512_chaser_step_mode_e mode;
	DMX512_chaser_step_direction_e dir;
	DMX512_chaser_step_state_e state;
	uint16_t step_count;
	uint16_t current_step;
}DMX512_chaser_s;

DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_step_mode_e mode, DMX512_chaser_step_direction_e dir);
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *this, DMX512_chaser_step_s step);
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *this, uint16_t id);
DMX512_chaser_step_s *DMX512_chaser_get_step(DMX512_chaser_s *this, uint16_t id);
void DMX512_chaser_manage(DMX512_chaser_s *this);
void DMX512_chaser_start(DMX512_chaser_s *this);

#endif
