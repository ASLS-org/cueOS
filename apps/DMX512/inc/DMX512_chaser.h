#ifndef __DMX512_CHASER_H
#define __DMX512_CHASER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_scene.h"
#include "DMX512_defs.h"

typedef struct DMX512_chaser DMX512_chaser_s;

typedef DMX512_engine_err_e (*chaserAddFunc)(DMX512_chaser_s *, DMX512_scene_s *);
typedef DMX512_engine_err_e (*chaserClrFunc)(DMX512_chaser_s *, uint16_t);
typedef void  (*trigFunc)(DMX512_chaser_s *);

typedef struct DMX512_chaser{
	uint16_t id;
	uint16_t sceneCount;
	DMX512_scene_s **scenes;
	chaserAddFunc add;
	chaserClrFunc clr;
	trigFunc trigger;
}DMX512_chaser_s;


DMX512_chaser_s *DMX512_chaser_init(DMX512_chaser_s *this, uint16_t id);

#endif
