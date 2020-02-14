#ifndef __DMX512_engine_H
#define __DMX512_engine_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"
#include "DMX512_scene.h"
#include "DMX512_chaser.h"
#include "DMX512_effect.h"


/** Instance declaration */
typedef struct DMX512_engine DMX512_engine_s;


/** Methods type definitions ******/
typedef DMX512_engine_err_e(*enginePatchFunc)(uint16_t, uint16_t, uint16_t);
typedef DMX512_engine_err_e(*engineUnpatchFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineAddSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineClrSceneFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineAddChaserFunc)(uint16_t);
typedef DMX512_engine_err_e(*engineClrChaserFunc)(uint16_t);

//typdef DMX512_engine_err(*)



//typedef struct DMX512_scene_pool{
//	uint16_t _fixture_pool_size;
//	DMX512_scene_s **_instance;
//
//}DMX512_scene_pool_s;
//
//typedef struct DMX512_chaser_pool{
//	uint16_t _size;
//	DMX512_chaser_s **_instance;
//}DMX512_chaser_pool_s;


/** Instance definition */
typedef struct DMX512_engine{
	/** Private parameters declaration */
	DMX512_fixture_s *_fixtures;
	uint16_t _fixtureCount;
//	DMX512_scene_pool_s _scene_pool;
//	DMX512_chaser_pool_s _chaser_pool;
	/** Methods declaration ******/
	enginePatchFunc patch;
	engineUnpatchFunc unpatch;
	engineAddSceneFunc addScene;
	engineClrSceneFunc clrScene;
	engineAddChaserFunc addChaser;
	engineClrChaserFunc clrChaser;
}DMX512_engine_s;


/** Public functions definition */
DMX512_engine_s *DMX512_engine_init(void);
DMX512_fixture_s *DMX512_engine_getFixture(uint16_t id);


#endif
