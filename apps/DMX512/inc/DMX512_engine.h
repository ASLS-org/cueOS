#ifndef __DMX512_engine_H
#define __DMX512_engine_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"

typedef struct DMX512_engine DMX512_engine_s;

typedef DMX512_engine_err_e(*patchFunc)(uint16_t, uint16_t);
typedef DMX512_engine_err_e(*unpatchFunc)(uint16_t);
typedef DMX512_engine_err_e(*addSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*clrSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*addChaserFunc)(uint16_t);
typedef DMX512_engine_err_e(*clrChaserFunc)(uint16_t);


typedef struct DMX512_engine{
	uint16_t fixtureCount;
	uint16_t sceneCount;
	uint16_t chaserCount;
	uint16_t effectCount;
	DMX512_fixture_s **fixtures;
	DMX512_scene_s **scene;
	DMX512_chaser_s **chaser;
	patchFunc patch;
	unpatchFunc unpatch;
}DMX512_engine_s;

DMX512_engine_s *DMX512_engine_init(void);


#endif
