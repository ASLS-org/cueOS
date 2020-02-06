#ifndef __DMX512_engine_H
#define __DMX512_engine_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"
#include "DMX512_scene.h"
#include "DMX512_chaser.h"
#include "DMX512_effect.h"

typedef struct DMX512_engine DMX512_engine_s;

typedef DMX512_engine_err_e(*enginePatchFunc)(uint16_t, uint16_t);
typedef DMX512_engine_err_e(*engineUnpatchFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineAddSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineClrSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineAddChaserFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineClrChaserFunc)(uint16_t);


typedef struct DMX512_engine{
	uint16_t fixtureCount;
	uint16_t sceneCount;
	uint16_t chaserCount;
	uint16_t effectCount;
	DMX512_fixture_s **fixtures;
	DMX512_scene_s **scene;
	DMX512_chaser_s **chaser;
	enginePatchFunc patch;
	engineUnpatchFunc unpatch;
	engineAddSceneFunc addScene;
	engineClrSceneFunc clrScene;
	engineAddChaserFunc addChaser;
	engineClrChaserFunc clrChaser;
}DMX512_engine_s;

DMX512_engine_s *DMX512_engine_init(void);


#endif
