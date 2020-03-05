#ifndef SCENE_POOL_H_
#define SCENE_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_scene.h"
#include "DMX512_defs.h"

DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_s scene);
DMX512_engine_err_e DMX512_scene_pool_del(uint16_t id);
DMX512_scene_s *DMX512_scene_pool_get(uint16_t id);
DMX512_scene_s *DMX512_scene_pool_get_all(void);
uint16_t DMX512_scene_pool_get_size(void);

#endif
