#ifndef DMX512_SCENE_POOL_H_
#define DMX512_SCENE_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"

#include "DMX512_defs.h"
#include "DMX512_scene.h"

typedef struct{
	DMX512_scene_s *scenes;
	uint16_t scene_count;
}DMX512_scene_pool_s;


DMX512_scene_pool_s *DMX512_scene_pool_new(void);
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_pool_s *this, DMX512_scene_s scene);
DMX512_engine_err_e DMX512_scene_pool_del(DMX512_scene_pool_s *this, uint16_t id);
DMX512_scene_s *DMX512_scene_pool_get(DMX512_scene_pool_s *this, uint16_t id);
void DMX512_scene_pool_manage(DMX512_scene_pool_s *this);

#endif
