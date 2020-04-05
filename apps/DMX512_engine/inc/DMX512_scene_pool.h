#ifndef DMX512_SCENE_POOL_H_
#define DMX512_SCENE_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_defs.h"
#include "DMX512_scene.h"

/**
 * @ingroup DMX512_scene_pool
 * @struct DMX512_scene_pool_s
 * @brief Defines a DMX512 scene pool object
 *
 * up to 65535 DMX512 scenes may be stored for each individual DMX512 universe. scene pools are used
 * to store and manage a lists of pre-configured scenes.
 */
typedef struct{
	DMX512_scene_s *scenes;				/**< pointer to a list of scenes*/
	uint16_t scene_count;				/**< The amount of scenes contained within the pool*/
}DMX512_scene_pool_s;


DMX512_scene_pool_s *DMX512_scene_pool_new(void);
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_pool_s *this, DMX512_scene_s scene);
DMX512_engine_err_e DMX512_scene_pool_del(DMX512_scene_pool_s *this, uint16_t id);
DMX512_scene_s *DMX512_scene_pool_get(DMX512_scene_pool_s *this, uint16_t id);
void DMX512_scene_pool_manage(DMX512_scene_pool_s *this);

#endif
