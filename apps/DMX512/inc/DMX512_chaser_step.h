#ifndef _CHASER_STEP_H_
#define _CHASER_STEP_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_scene.h"
#include "DMX512_defs.h"

#define DMX512_CHASER_STEP_DEFAULT {NULL, 0, 0, 0, DMX512_CHASER_STEP_UNINITIALISED}

typedef enum{
	DMX512_CHASER_STEP_UNINITIALISED,
	DMX512_CHASER_STEP_INITIALISED
}DMX512_fixture_chaser_step_status;

typedef struct{
	DMX512_scene_s *scene;
	uint16_t fadein;
	uint16_t fadeout;
	uint16_t hold;
	DMX512_fixture_chaser_step_status status;
}DMX512_chaser_step_s;


DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein, uint16_t fadeout, uint16_t hold);
void DMX512_chaser_step_trigger(DMX512_chaser_step_s *this);

#endif
