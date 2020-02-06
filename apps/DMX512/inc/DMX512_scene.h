#ifndef __DMX512_SCENE_H
#define __DMX512_SCENE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_preset.h"
#include "DMX512_defs.h"

#define DMX512_PRESET_NOT_FOUND -1

typedef struct DMX512_scene DMX512_scene_s;

typedef DMX512_engine_err_e (*sceneAddPresetFunc)(DMX512_scene_s *, uint16_t, uint16_t *, uint8_t *);
typedef DMX512_engine_err_e (*sceneUpdFunc)(DMX512_scene_s *, uint16_t, uint16_t *, uint8_t *);
typedef DMX512_engine_err_e (*sceneClrFunc)(DMX512_scene_s *, uint16_t);
typedef void  (*sceneTriggerFunc)(DMX512_scene_s *);

typedef struct DMX512_scene{
	uint16_t id;
	uint16_t presetCount;
	DMX512_preset_s **presets;
	sceneAddPresetFunc add;
	sceneUpdFunc upd;
	sceneClrFunc clr;
	sceneTriggerFunc trigger;
}DMX512_scene_s;


DMX512_scene_s *DMX512_scene_init(uint16_t id);

#endif
