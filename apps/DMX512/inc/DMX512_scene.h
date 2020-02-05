#ifndef __DMX512_SCENE_H
#define __DMX512_SCENE_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_fixture.h"
#include "DMX512_patcher.h"
#include "DMX512_defs.h"

typedef struct{
	uint16_t fixtureId;
	uint16_t *channels;
	uint8_t *values;
}DMX512_scene_fSetting_s;

typedef struct{
	uint16_t id;
	uint16_t fCount;
	DMX512_scene_fSetting_s **fSettings;
}DMX512_scene_s;

#endif
