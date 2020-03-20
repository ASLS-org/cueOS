#ifndef DMX512_CHASER_H
#define DMX512_CHASER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_chaser_step.h"
#include "DMX512_defs.h"

#define DMX512_CHASER_DEFAULT {0, NULL, DMX512_CHASER_MODE_LOOP, DMX512_CHASER_DIRECTION_FORWARD, 0, 0, DMX512_CHASER_IDLE, DMX512_CHASER_UNINITIALISED, DMX512_CHASER_PINGPONG_PING}

typedef enum{
	DMX512_CHASER_UNINITIALISED,
	DMX512_CHASER_INITIALISED
}DMX512_chaser_status_e;

typedef enum{
	DMX512_CHASER_MODE_LOOP,
	DMX512_CHASER_MODE_SINGLE_SHOT,
}DMX512_chaser_mode_e;

typedef enum{
	DMX512_CHASER_DIRECTION_FORWARD,
	DMX512_CHASER_DIRECTION_BACKWARD,
	DMX512_CHASER_DIRECTION_PINGPONG,
	DMX512_CHASER_DIRECTION_RANDOM
}DMX512_chaser_direction_e;

typedef enum{
	DMX512_CHASER_PINGPONG_PING,
	DMX512_CHASER_PINGPONG_PONG,
}DMX512_chaser_pingpong_state_e;

typedef enum{
	DMX512_CHASER_PLAYING,
	DMX512_CHASER_IDLE
}DMX512_chaser_step_state_e;


typedef struct{
	uint16_t id;
	DMX512_chaser_step_s *steps;
	DMX512_chaser_mode_e mode;
	DMX512_chaser_direction_e direction;
	uint16_t step_count;
	uint16_t current_step;
	DMX512_chaser_step_state_e state;
	DMX512_chaser_status_e status;
	DMX512_chaser_pingpong_state_e pingpong;
}DMX512_chaser_s;

DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_mode_e mode, DMX512_chaser_direction_e direction);
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *this, DMX512_chaser_step_s step);
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *this, uint16_t id);
DMX512_chaser_step_s *DMX512_chaser_get_step(DMX512_chaser_s *this, uint16_t id);
void DMX512_chaser_manage(DMX512_chaser_s *this);
void DMX512_chaser_start(DMX512_chaser_s *this);

#endif
